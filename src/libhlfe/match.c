/************************************************************************
Demo software: Invariant keypoint matching.
Author: David Lowe

match.c:
This file contains a sample program to read images and keypoints, then
draw lines connecting matched keypoints.


given row, column endpoints.
ReadKeyFile(const char *filename) - Read file of keypoints.
*************************************************************************/



#include "defs.h"
#include "match.h"
#include <stdarg.h>



/* -------------------- Local function prototypes ------------------------ */

void FatalError(char *fmt, ...);
Keypoint ReadKeys(FILE *fp, int * pnbfeat, int nbim, Keypoint key_no_match);

long DistSquared(Keypoint k1, Keypoint k2);


/*----------------------------- Routines ----------------------------------*/

/** This searches through the keypoints in klist for the two closest matches to key.
*  If the closest is less than distratio times the distance to second closest,
* then return the closest match.  Otherwise, return NULL.
*/
Keypoint CheckForMatch(Keypoint key, Keypoint klist, float distRatio)
{
	int dsq, distsq1 = 100000000, distsq2 = 100000000;
	Keypoint k, minkey = NULL;

	/* Find the two closest matches, and put their squared distances in
	distsq1 and distsq2.
	*/
	for (k = klist; k != NULL; k = k->next) {
		dsq = DistSquared(key, k);

		/*if (dsq < distsq1) {
			distsq2 = distsq1;
			distsq1 = dsq;
			minkey = k;
		} else if (dsq < distsq2) {
			distsq2 = dsq;
		}*/
		if (dsq < distsq2){
			if (dsq < distsq1) {
				distsq2 = distsq1;
				distsq1 = dsq;
				minkey = k;
			} else  {
				distsq2 = dsq;
			}
		}
	}
	/* Check whether closest distance is less than distRatio of second. */
	//    if (10 * 10 * distsq1 < 6 * 6 * distsq2)
//    if(key->sq_dist_thres==0){
	if (distsq1 <= distRatio * distRatio * distsq2)
		return minkey;
//    }
//    else{
//printf("CheckForMatch : default threshold %f : %d\n",key->distsq2,distsq1);
//        if (distsq1 < distRatio * distRatio * key->sq_dist_thres)
//        return minkey;
//    }
	return NULL;
}


/** Return squared distance between two keypoint descriptors.
* @param k1 first keypoint
* @param k2 second keypoint
*/
long DistSquared(Keypoint k1, Keypoint k2)
{
	int i, dif;
	long distsq = 0;
	unsigned char *pk1, *pk2;

	pk1 = k1->descrip;
	pk2 = k2->descrip;
	for (i = 0; i < 128; i++) {
		dif = (int) *pk1++ - (int) *pk2++;
		distsq += dif * dif;
	}
	return distsq;
}

/*------------------------ Error reporting ----------------------------*/

/** This function prints an error message and exits.  It takes a variable
* number of arguments that function just like those in printf.
*/
void FatalError(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr,"\n");
	va_end(args);
	exit(1);
}



/** This reads a keypoint file from a given filename and returns the list of keypoints.
*/
Keypoint ReadKeyFile(const char *filename, int * pnbfeat, int nbim, Keypoint key_no_match)
{
	FILE *file;
	Keypoint kp=NULL;
	file = fopen (filename, "r");
	if (! file)
		FatalError("Could not open file: %s", filename);

	kp=ReadKeys(file, pnbfeat, nbim, key_no_match);
	fclose(file);
	return kp;
}


/** Read keypoints from the given file pointer and return the list of
* keypoints.  The file format starts with 2 integers giving the total
* number of keypoints and the size of descriptor vector for each
* keypoint (currently assumed to be 128). Then each keypoint is
* specified by 4 floating point numbers giving subpixel row and
* column location, scale, and orientation (in radians from -PI to
* PI).  Then the descriptor vector for each keypoint is given as a
* list of integers in range [0,255].

* @param key_no_match is not used
*/
Keypoint ReadKeys(FILE *fp, int * pnbfeat, int nbim, Keypoint key_no_match)
{
	int i, j, num, len, val, nbfeat=0;
	Keypoint k, keys = NULL;

	if (fscanf(fp, "%d %d", &num, &len) != 2)
		FatalError("Invalid keypoint file beginning.");
	if (len != 128)
		FatalError("Keypoint descriptor length invalid (should be 128).");

	for (i = 0; i < num; i++) {
		nbfeat++;
		/* Allocate memory for the keypoint. */
		k = (KeypointSt *) malloc(sizeof(struct KeypointSt));
		k->next = keys;
		/* Added by Laurent */
		k->nbim=nbim;
		k->match=NULL;
		k->next2=NULL;
		keys = k;
		k->descrip = (unsigned char*) malloc(len);

//        if(key_no_match==NULL){
		k->sq_dist_thres=0;
//        }
//        else {
//            /* Set default threshold*/
//
//            k->sq_dist_thres=0; // Initialization
//            Keypoint k2=CheckForMatch(k, key_no_match, 999);
//            if(k2==NULL)
//            {printf("Error setting default threshold\n");exit(0);}
//            else {
//                k->sq_dist_thres=(float)DistSquared(k, k2);
////   printf("default threshold %f\n",k->sq_dist_thres);
//            }
//        }

		if (fscanf(fp, "%f %f %f %f", &(k->row), &(k->col), &(k->scale),
				   &(k->ori)) != 4)
			FatalError("Invalid keypoint file format.");
		if (k->descrip==NULL)
			FatalError("Invalid keypoint file format (2).");

		k->ori=-k->ori; /* I make this because the image space is upside down, Laurent*/

		for (j = 0; j < len; j++) {
			if (fscanf(fp, "%d", &val) != 1 || val < 0 || val > 255)
				FatalError("Invalid keypoint file value.");
			k->descrip[j] = (unsigned char) val;
		}
	}
	if (pnbfeat!=NULL)*pnbfeat=nbfeat;
	return keys;
}

/** This deletes a list of keypoints.
*/
int UnloadKeys(Keypoint* keylist)
{
	if(keylist!=NULL){
		Keypoint knext=NULL;
		for(Keypoint k=*keylist; k!=NULL; k=knext){
			free(k->descrip);
			knext=k->next;
			k->next=NULL;
			free(k); 
		}
		*keylist=NULL;
	}
	return 1;
}



/**
*  Duplicate a keypoint (without duplicating descr array)
*/
Keypoint Duplicate(Keypoint key)
{
	Keypoint knew=(Keypoint) malloc(sizeof(struct KeypointSt));

	knew->col=key->col;
	knew->row=key->row;
	knew->ori=key->ori;
	knew->scale=key->scale;
	knew->nbim=key->nbim;
	//knew->descrip=key->descrip; // Watch out! they both point on the same descrip array
	knew->descrip=(unsigned char*)malloc(128); 
	for (int j = 0; j < 128; j++) {
		knew->descrip[j] = key->descrip[j];
	}
	
	
	knew->next2=NULL;
	knew->match=key->match;
	knew->sq_dist_thres=key->sq_dist_thres;
	knew->flag=key->flag;

	return knew;
}

/**
*  Check if k1 does match with k2
*/

int IsMatch(Keypoint k1, Keypoint k2, Keypoint klist2, float distRatio) {

//    if(k1->sq_dist_thres==0){

	/*No default distance threshold set*/

	return k2==CheckForMatch(k1, klist2, distRatio);
//    }
//    else {
	/* Simply compare the distances */
	//        return (DistSquared(k1,k2)<distRatio*distRatio*k1->sq_dist_thres);
//    }

}

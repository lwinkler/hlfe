// +--------------------------------------------------------------------------+
// | File      : maincam.cpp                                                  |
// | Utilite   : Feature detection using SIFT with webcam                     |
// | Author    : Laurent Winkler (original source by David Lowe)              |
// | Date      : 28.11.2005                                                   |
// | Remarque  : aucune.                                                      |
// +--------------------------------------------------------------------------+

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#endif

#include "defs.h"
#include <iostream>
#include <string>
#include <dirent.h>
using namespace std;


/*------------------------------ Global variables -------------------------*/
IplImage *image = 0, *grey = 0, *prev_grey = 0, *pyramid = 0, *prev_pyramid = 0, *swap_temp;


/*------------------------------ Constants ---------------------------------*/
int win_size = 10;
const int MAX_COUNT = 500;
CvPoint2D32f* points[2] = {0,0}, *swap_points;
char* status = 0;
int count1 = 0;
int need_to_init = 0;
int learn_kp = 0;
//int night_mode = 0;
int nbpics=0;
int flags = 0;
int add_remove_pt = 0;
CvPoint pt;

/*------------------------------ Local functions declarations -------------------------*/
void on_mouse( int event, int x, int y, int flags, void* param );
Keypoint ReadKeyFile(char *filename);
Keypoint ReadKeys(FILE *fp);
Keypoint * loadTemplates(int *nbkp, int *tpcols, int *tprows);
/*-------------------------------------------------------------------------*/
/* Main routine                                                            */
/*-------------------------------------------------------------------------*/

int main( int argc, char** argv )
{
	need_to_init=1;
	CvCapture* capture = 0;
	int nbtp, tpWidth[NBMAXTEMPLATES], tpHeight[NBMAXTEMPLATES];
	Keypoint *klist=NULL;

	if ( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
		capture = cvCaptureFromCAM( argc == 2 ? argv[1][0] - '0' : 0 );
	else if ( argc == 2 )
		capture = cvCaptureFromAVI( argv[1] );

	if ( !capture )
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}

	printf( "Hot keys: \n"
			"\tESC - quit the program\n"
			"\tl - detect features\n"
			"\tr - take a picture and try to detect\n"
			"To add/remove a feature point click it\n" );

	cvNamedWindow( "SIFT", 0 );
	cvSetMouseCallback( "SIFT", on_mouse, 0 );

	for (;;)
	{
		IplImage* frame = 0;
		int i, k, c;

		frame = cvQueryFrame( capture );
		if ( !frame )
			break;

		if ( !image )
		{
			/* allocate all the buffers */
			image = cvCreateImage( cvGetSize(frame), 8, 3 );
			image->origin = frame->origin;
			grey = cvCreateImage( cvGetSize(frame), 8, 1 );
			prev_grey = cvCreateImage( cvGetSize(frame), 8, 1 );
			pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
			prev_pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
			points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
			status = (char*)cvAlloc(MAX_COUNT);
			flags = 0;
		}

		cvCopy( frame, image, 0 );
		cvCvtColor( image, grey, CV_BGR2GRAY );

		if ( need_to_init ) {
			/* automatic initialization */
			IplImage* eig = cvCreateImage( cvGetSize(grey), 32, 1 );
			IplImage* temp = cvCreateImage( cvGetSize(grey), 32, 1 );
			//            double quality = 0.01;
			//            double min_distance = 10;

			//            add_remove_pt = 0;
			klist=loadTemplates(&nbtp, tpWidth, tpHeight);
			need_to_init=0;
		}

		if (learn_kp) {
			char buf[50];
//            sprintf(buf,"jpg/im%d.jpg                                                  ",nbpics);
//            cvSaveImage( buf,image );
//            sprintf(buf,"convert im%d                                                  ",nbpics);
//            system(buf);
//            sprintf(buf,"siftwin32 <pgm/im%d.pgm >key/im%d.key                                                  ",nbpics,nbpics);
//            system(buf);
//            learn_kp=0;

			sprintf(buf,"templates/im%d.jpg                                                  ",nbpics);
			nbpics++;
			cvSaveImage( buf,image );
			learn_kp=0;
		}


//        cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
//        points[0], points[1], count, cvSize(win_size,win_size), 3, status, 0,
//        cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );
		flags |= CV_LKFLOW_PYR_A_READY;
		//            for( i = k = 0; i < count1; i++ )
		//            {
		//                if( add_remove_pt )
		//                {
		//                    double dx = pt.x - points[1][i].x;
		//                    double dy = pt.y - points[1][i].y;
		//
		//                    if( dx*dx + dy*dy <= 25 )
		//                    {
		//                        add_remove_pt = 0;
		//                        continue;
		//                    }
		//                }
		//
		//                if( !status[i] )
		//                    continue;
		//
		//                points[1][k++] = points[1][i];
		//                cvCircle( image, cvPointFrom32f(points[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);
		//            }
		//            count1 = k;
		//}

		if ( add_remove_pt && count1 < MAX_COUNT )
		{
			points[1][count1++] = cvPointTo32f(pt);
			cvFindCornerSubPix( grey, points[1] + count1 - 1, 1,
								cvSize(win_size,win_size), cvSize(-1,-1),
								cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
			add_remove_pt = 0;
		}

		CV_SWAP( prev_grey, grey, swap_temp );
		CV_SWAP( prev_pyramid, pyramid, swap_temp );
		CV_SWAP( points[0], points[1], swap_points );
		need_to_init = 0;
		cvShowImage( "SIFT", image );

		c = cvWaitKey(10);
		if ( c == 27 )
			break;
		switch ( c )
		{
		case 'r':
			need_to_init = 1;
			break;
		case 'l':
			learn_kp = 1;
			nbpics++;
			break;
		case 'n':
			//            night_mode ^= 1;
			break;
		default:
			;
		}
	}

	cvReleaseCapture( &capture );
	cvDestroyWindow("LkDemo");

	return 0;
}

/*----------------------------- ROUTINES ----------------------------------*/

/*-------------------------------------------------------------------------*/
/* Some mouse function                                                     */
/*-------------------------------------------------------------------------*/

void on_mouse( int event, int x, int y, int flags, void* param )
{
	if ( !image )
		return;

	if ( image->origin )
		y = image->height - y;

	if ( event == CV_EVENT_LBUTTONDOWN ) {
		pt = cvPoint(x,y);
		add_remove_pt = 1;
	}
}



//#ifdef _EiC
//main(1,"lkdemo.c");
//#endif

/*-------------------------------------------------------------------------*/
/* Load templates into keypoints lists                                                 */
/*-------------------------------------------------------------------------*/
Keypoint * loadTemplates(int *nbtp, int *tpWidth, int *tpHeight)
{
//  unsigned int ext_len = ext ? strlen(ext) : 0;
	char * dir("./templates");
	DIR *d = opendir(dir);
	Keypoint *klist, kp1;
	struct dirent *dirent;
	char full_name[256], buf[50], name[20];
	IplImage* im;

//  int *tpWidth=*ptpWidth;
//  int *tpHeight=*ptpHeight;

	klist=(Keypoint*)malloc(NBMAXTEMPLATES*sizeof(Keypoint));
	if (klist==NULL)
		{cout<<"Error allocating memory in loading templates : klist"<<endl;exit(0);}
//  if(*tpWidth==NULL)tpWidth=(int*)malloc(NBMAXTEMPLATES*sizeof(int));
//  if(tpHeight==NULL)tpHeight=(int*)malloc(NBMAXTEMPLATES*sizeof(int));

//  struct stat stat_struct;

	*nbtp=-1;

	if (d)
	{

		system("del ./key/*.key");
		system("del ./pgm/*.pgm");
		while ((dirent = readdir(d)) != NULL)
		{
			if (strcmp(dirent->d_name, ".") && strcmp(dirent->d_name, ".."))
			{
				sprintf(full_name, "%s/%s", dir, dirent->d_name);

				if (dirent->d_namlen-4<1)
					{cout<<"Error in image file name :"<<full_name<<endl;exit(0);}
				strncpy(name,dirent->d_name,dirent->d_namlen-4);

				im=cvLoadImage(full_name, 0);

				/*Convert all images in templates to .pgm */
				sprintf(buf,"convert %s %s                              ",dirent->d_name,name);
				system(buf);
				/*Extract keypoints*/
				sprintf(buf,"siftwin32 <pgm/%s.pgm >key/%s.key                         ",name,name);
				system(buf);

				/*Read keypoints*/
				sprintf(buf,"key/%s.key                               ",name);
				kp1 = ReadKeyFile(buf);

				if (kp1) {
					*nbtp+=1;
//            if(klist=)klist=(Keypoint*)realloc(klist,(*nbtp+1)*sizeof(Keypoint));
//            klist=(Keypoint*)realloc(klist,(*nbtp+1)*sizeof(Keypoint));

//            else
//            tpWidth=(int*)realloc(tpWidth,(*nbtp+1)*sizeof(int));

//            if(tpHeight==NULL)tpHeight=(int*)malloc(1*sizeof(int));
//            else
//            tpHeight=(int*)realloc(tpHeight,(*nbtp+1)*sizeof(int));

					if (klist==NULL||tpWidth==NULL||tpHeight==NULL||im==NULL)
						{cout<<"Error allocating memory while loading templates"<<endl;exit(0);}
					cout<<"nbtp "<<*nbtp<<endl;
					klist[*nbtp]=kp1;


					/*get size of image*/
//            cout<<"im"<<im<<endl;
					tpWidth[*nbtp]=im->width;
					tpHeight[*nbtp]=im->height;

					cout<<"template : "<<dirent->d_name<<" | image : "<<tpHeight[*nbtp]<<"x"<<tpWidth[*nbtp]<<endl;
				} else cout<<"Error on template "<<dirent->d_name<<endl;
//        cout<<full_name<<endl;

//        if(!stat(full_name, &stat_struct))
//        {
//          if(stat_struct.st_mode & S_IFDIR)
//          { /* It's a directory entry */
//            scan(full_name, ext);
//          }
//          else
//          { /* It's probably a file */
//            if(ext == NULL ||
//               (strlen(dirent->d_name) >= ext_len && !strcmp(&dirent->d_name[strlen(dirent->d_name) - ext_len], ext)))
//            {
//              printf("%s\n", full_name);
//            }
//          }
//        }
			}
		}
		closedir(d);
	}
	return klist;
}

/*-------------------------------------------------------------------------*/
/* Read keypoint file                                                      */
/*-------------------------------------------------------------------------*/
/* This reads a keypoint file from a given filename and returns the list
of keypoints.
*/
Keypoint ReadKeyFile(const char *filename)
{
	FILE *file;

	file = fopen (filename, "r");
	if (! file) {
		cout<<"Could not open file: "<<filename<<endl;
		exit(0);
	}
//    FatalError("Could not open file: %s", filename);

	return ReadKeys(file);
}

/*-------------------------------------------------------------------------*/
/* Read keypoint file                                                */
/*-------------------------------------------------------------------------*/

/* Read keypoints from the given file pointer and return the list of
keypoints.  The file format starts with 2 integers giving the total
number of keypoints and the size of descriptor vector for each
keypoint (currently assumed to be 128). Then each keypoint is
specified by 4 floating point numbers giving subpixel row and
column location, scale, and orientation (in radians from -PI to
PI).  Then the descriptor vector for each keypoint is given as a
list of integers in range [0,255].

*/
Keypoint ReadKeys(FILE *fp)
{
	int i, j, num, len, val;
	Keypoint k, keys = NULL;

	if (fscanf(fp, "%d %d", &num, &len) != 2) {
		cout<<"Invalid keypoint file beginning."<<endl;
		exit(0);
	}

	if (len != 128) {
		cout<<"Keypoint descriptor length invalid (should be 128)."<<endl;
		exit(0);
	}

	for (i = 0; i < num; i++) {
		/* Allocate memory for the keypoint. */
		k = (Keypoint) malloc(sizeof(struct KeypointSt));
		k->next = keys;
		keys = k;
		k->descrip = (unsigned char*) malloc(len);

		if (fscanf(fp, "%f %f %f %f", &(k->row), &(k->col), &(k->scale),
				   &(k->ori)) != 4)
			{cout<<"Invalid keypoint file format."<<endl;exit(0);}

		for (j = 0; j < len; j++) {
			if (fscanf(fp, "%d", &val) != 1 || val < 0 || val > 255) {
				cout<<"Invalid keypoint file value."<<endl;
				exit(0);
			}
			k->descrip[j] = (unsigned char) val;
		}
	}
	return keys;
}

/*
* This module contains most of the functions I created for this project
* author   Laurent Winkler
* date     28.11.2005
*/
// +--------------------------------------------------------------------------+
// | File      : main.cpp                                                     |
// | Utility   : Feature detection using SIFT                                 |
// | Author    : Laurent Winkler                                              |
// | Date      : 28.11.2005                                                   |
// | Remarque  : aucune.                                                      |
// +--------------------------------------------------------------------------+

#include "sift_object.h"

#ifndef WINSYS
#define DWORD long
long GetTickCount() {
	return 0;
}		/*sprintf(cmd, "make pgm/%s.pgm", name.ascii());
		system(cmd);*/
#endif

float SiftObject::distance_ratio=0;
int  SiftObject::x_div_HT=0;
int SiftObject::y_div_HT=0;
int SiftObject::theta_div_HT=0;
int SiftObject::scale_div_HT=0;
float SiftObject::scale_factor=0;
int SiftObject::nb_match_min_AT=0;
float SiftObject::agree_pos_AT=0;
int SiftObject::nb_match_min_ST=0;
float SiftObject::agree_pos_ST=0;
float SiftObject::agree_ori_ST=0;
float SiftObject::agree_scale_ST=0;
int SiftObject::border_threshold=0;
//Keypoint SiftObject::klistEmpty=NULL;
float SiftObject::clustering_threshold=0;

const CvScalar SiftObject::color_array[NBCOLORS] =
{
	CV_RGB(255,255,255), // white
	CV_RGB(255,0,0), // red
	CV_RGB(0,255,0), // green
	CV_RGB(0,0,255), // blue
	CV_RGB(255,255,0), // yellow
	CV_RGB(0,150,255), // blue
	CV_RGB(130,77,191), // purple
	CV_RGB(255,100,0), // yellow
	CV_RGB(185,135,95), // brown
	CV_RGB(160, 32, 240),
	CV_RGB(255, 165, 0),
	CV_RGB(132, 112, 255),
	CV_RGB(0, 250, 154),
	CV_RGB(255, 192, 203),
	CV_RGB(0, 255, 255),
	CV_RGB(185, 185, 185),
	CV_RGB(216, 191, 216),
	CV_RGB(255, 105, 180),
	CV_RGB(0, 255, 255),
	CV_RGB(240, 255, 240),
	CV_RGB(173, 216, 230),
	CV_RGB(127, 255, 212),
	CV_RGB(100, 149, 237),
	CV_RGB(255, 165, 0),
	CV_RGB(255, 255, 0),
	CV_RGB(210, 180, 140),
	CV_RGB(211, 211, 211),
	CV_RGB(222, 184, 135),
	CV_RGB(205, 133, 63),
	CV_RGB(139, 69, 19),
	CV_RGB(165, 42, 42),
	CV_RGB(84, 134, 11),
	CV_RGB(210, 105, 30),
	CV_RGB(255, 127, 80),
	CV_RGB(255, 0, 255),
	CV_RGB(70, 130, 180),
	CV_RGB(95, 158, 160),
	CV_RGB(199, 21, 133),
	CV_RGB(255, 182, 193),
	CV_RGB(255, 140, 0),
	CV_RGB(240, 255, 255),
	CV_RGB(152, 251, 152),
	CV_RGB(143, 188, 143),
	CV_RGB(240, 230, 140),
	CV_RGB(240, 128, 128),
	CV_RGB(0, 191, 255),
	CV_RGB(250, 128, 114),
	CV_RGB(189, 183, 107),
	CV_RGB(255, 218, 185),
	CV_RGB(60, 179, 113),
	CV_RGB(178, 34, 34),
	CV_RGB(30, 144, 255),
	CV_RGB(255, 140, 0),
	CV_RGB(175, 238, 238)
};

/*-------------------------------------------------------------------------*/
/* Constr                                                                  */
/*-------------------------------------------------------------------------*/
SiftObject::SiftObject()
{
	sprintf(fileName,"");
	sprintf(name,"");
	nb_feat=0;
	nb_feat_model=0;
	nb_feat_tot=0;

	im=NULL;
	keypoints=NULL;
}

/*-------------------------------------------------------------------------*/
/* Constr                                                                  */
/*-------------------------------------------------------------------------*/
SiftObject::SiftObject(const char* fileNamePgm)
{
	Load(fileNamePgm);
}

/*-------------------------------------------------------------------------*/
/* Destr                                                                   */
/*-------------------------------------------------------------------------*/
SiftObject::~SiftObject()
{
	Reset();
}

/*-------------------------------------------------------------------------*/
/* Check if empty                                                          */
/*-------------------------------------------------------------------------*/
bool SiftObject::IsEmpty () const
{
	return im==NULL;
}

/*-------------------------------------------------------------------------*/
/* Load                                                                   */
/*-------------------------------------------------------------------------*/
int SiftObject::Load (const char* fileNamePgm){
	
	if(distance_ratio==0)ReadConfig();
	//Reset();
	
	char* tmp1, tmp2[256], buf[256];
	strcpy(fileName, fileNamePgm);
	strcpy(tmp2, fileNamePgm);
	tmp1=strrchr(tmp2, '.');
	*tmp1='\0';
	tmp1=strrchr(tmp2, '/');
	if(tmp1==NULL) tmp1=tmp2;
	else tmp1++;
	
	strcpy(name, tmp1);
	
	nb_feat=0;
	nb_feat_model=0;
	nb_feat_tot=0;

	sprintf(buf, CMDMAKE, name, name);
	system(buf);
	im=cvLoadImage(fileName, 0);
	if(im==NULL)
	{
		cout<<"Error : Cannot load image "<<fileName<<endl;
		exit(-1);
	}
	loadKeyPoints();
	
	return 1;
}


/*-------------------------------------------------------------------------*/
/* Reset                                                                   */
/*-------------------------------------------------------------------------*/
void SiftObject::Reset () {
	
	sprintf(fileName,"");
	sprintf(name,"");
	nb_feat=0;
	nb_feat_model=0;
	nb_feat_tot=0;

	if(im!=NULL)cvReleaseImage(&im);
	im=NULL;
	if(keypoints!=NULL)UnloadKeys(&keypoints);
	keypoints=NULL;

}

/*-------------------------------------------------------------------------*/
/* Main routine                                                            */
/*-------------------------------------------------------------------------*/
int SiftObject::main1 (int argc, char **argv) {
	char buf[100];
	SiftComparison compare;
	
	Match *tabmatch;
	IplImage * im1, *im2;
	Keypoint k1 = NULL, k2 = NULL;
	int nb_match_good=0;//, nb_match=0, nb_feat_model=0;
	long nb_feat_tot=0;
	DWORD time1, time2;
	Matrice param, param1, param2;
	
	char fileName1[123], fileName2[2143];
	//ReadConfig();
	
	int c=' ';
	IplImage *imlist[NB_MAX_TEMPLATES];
	Keypoint klist[NB_MAX_TEMPLATES];
	int nbtp=0;
	
	while (c!='q') {
		c=getc(stdin);
		cout<<endl;
		switch (c) {
			case '1':
				

				/*---------------------------------------------------------*/
				case '2':
					
					/* We load the templates from template folder*/
					
					if (nbtp==0) LoadTemplates(klist, imlist, &nbtp, &nb_feat_tot);
					break;
					
				case '3':
					cout<<"Enter image name (e.g. cd1): ";
					cin>>fileName1;
					cout<<endl;
					
					/* Make files */
					
					sprintf(buf,CMDMAKE,fileName1,fileName1);
					system(buf);
					
					try {
						/* Load images and keypoints files */
						
						sprintf(buf,"pgm/%s.pgm",fileName1);
						im1=cvLoadImage(buf, 0);
						
						sprintf(buf,"key/%s.key", fileName1);
						k1 = ReadKeyFile(buf, &nb_feat_model, 0/*, klistEmpty*/);
						
						
						for (int i=0;i<nbtp;i++) {
							
							/* Compare the image with each model view*/
							
							nb_match_good=0;
							cout<<"________________________________________"<<endl;
							cout<<"| Compare "<<fileName1<<" to model view "<<i<<endl;
							tabmatch = FindMatchesWithHT(imlist[i]->width, imlist[i]->height, klist[i], im1->width, im1->height, k1, &compare.nb_match);
							cout<<"----------------------------------------"<<endl;
							param=SimilarityParamApprox(tabmatch, compare.nb_match, MAXOF2(imlist[i]->width, imlist[i]->height), NULL, &nb_match_good);
							
							if (param.size_m()==4 && nb_match_good>=nb_match_min_ST) {
								sprintf(buf,"out/%s_matches_with_model_view_%d.jpg",fileName1,i);
								DisplayMatches(imlist[i], im1, tabmatch, compare.nb_match, param, buf, klist[i], k1);
								
								/*Compute probability of presence of obj*/
								
								int featinproj=CountFeatInProj(imlist[i]->width, imlist[i]->height, klist[i], param);
								
								cout<<"Features in model : "<<nb_feat_model<<endl;
								cout<<"Total nb of features in db : "<<nb_feat_tot<<endl;
								cout<<"Good matches : "<<nb_match_good<<endl;
								cout<<"Features inside projection : "<<featinproj<<endl;
								float pmf=ProbOfPresence(nb_feat_model, nb_feat_tot, nb_match_good, featinproj);
								cout<<"Probability of presence : "<<pmf<<endl;
								
							}
						}
					}
					catch (Erreur &tmp)
					{
						cout << tmp.cause() << " - " << tmp.explic() << endl;
					}
					break;
					case '4':
						#ifdef WINSYS
						system("del out\\*.jpg");
						#else
						system("rm out\\*.jpg");
						#endif
						break;
		}
	}
	
	/*End of program*/
	
	//free(klist);
	
	
	
	return 0;
	
}

SiftComparison SiftObject::Compare1(const SiftObject& obj) const {
		
	SiftComparison compare;
	compare.name+= name;
	compare.name+= " with ";
	compare.name+= obj.name;
	char buf[256];
	
	assert(!IsEmpty());
	assert(!obj.IsEmpty());
	
	//Match *tabmatch;
	IplImage * im1=im, *im2=obj.im;
	Keypoint k1 = keypoints, k2 = obj.keypoints;
	//int nb_match=0, nb_match_good=0;//, nb_feat_model=0;
	//long nb_feat_tot=0;
	int nb_match_good=0;
	//nb_match=0;//, nb_feat_model=0;
	//long 
	//nb_feat_tot=0;
	DWORD time1, time2;
	Matrice param, param1, param2;
	/* Load images and keypoints files */
	
	if (im1 == NULL || im2 == NULL || k1 == NULL || k2 == NULL) {
		cout << "Program does not specify all images and keys."<<endl;
		exit(-1);
	}
	else {
		time1 = GetTickCount(); // This works only with Windows

		/* Find matches from the 2 lists of keypoints */
		
		compare.tab_match=FindMatchesWithHT(im1->width, im1->height, k1, im2->width, im2->height, k2, &compare.nb_match);
		time2 = GetTickCount(); // This works only with Windows
		cout<<endl<<"Matches found and HT in "<<(int) time2-time1<<" ms."<<endl;
		
		
		/* get affine parameters from result table  */
		
		if (compare.nb_match>=nb_match_min_AT) {
			time1 = GetTickCount(); // This works only with Windows
			param1=AffineParamApprox(compare.tab_match, compare.nb_match, MAXOF2(im2->width, im2->height));
			time2 = GetTickCount(); // This works only with Windows
			
			cout<<endl<<"Affine parameters : "<<endl<<param1.t()<<endl;
			cout<<endl<<"Affine transformation computed in "<<(int) time2-time1<<" ms."<<endl;
			
			sprintf(buf,"./out/%s_with_%s_AT.jpg",name, obj.name);
			DisplayMatches(im1, im2, compare.tab_match, compare.nb_match, param1, buf, k1, k2);
			
		}
		else {
			cout<<endl<<"Not enough matches after Hough transform to compute affine transform."<<endl;
		}
		if (compare.nb_match>nb_match_min_ST) {
			time1 = GetTickCount(); // This works only with Windows
			param2=SimilarityParamApprox(compare.tab_match, compare.nb_match, MAXOF2(im2->width, im2->height), NULL, &nb_match_good);
			time2 = GetTickCount(); // This works only with Windows
			
			sprintf(buf,"./out/%s_with_%s_ST.jpg",name, obj.name);
			DisplayMatches(im1, im2, compare.tab_match, compare.nb_match, param2, buf, k1, k2);
			
			cout<<endl<<"Similarity transf. parameters : "<<param2.t()<<endl;
			cout<<endl<<"Similarity transform computed in "<<(int) time2-time1<<" ms."<<endl;
			
			if (nb_match_good>=nb_match_min_ST) {
				compare.feat_in_proj=CountFeatInProj(im1->width, im1->height, k2, param2);
				compare.prob_presence=ProbOfPresence(nb_feat, nb_feat_tot, nb_match_good, compare.feat_in_proj);
				cout<<"Features inside projection : "<<compare.feat_in_proj<<endl;
				cout<<"Probability of presence : "<<compare.prob_presence<<endl;
			}
			else cout<<endl<<"Not enough matches after min square verification."<<endl;
		}
		else {
			cout<<endl<<"Not enough matches after Hough transform to compute similarity transform."<<endl;
			
		}
	}
	
	return compare;
}

/*----------------------------- ROUTINES ----------------------------------*/

/**
* Find matches and filter result using Hough Transform
* @param      width1: width of first image
* @param      height1: height of first image
* @param      keys1: keypoint list extracted from first image
* @param      width2: width of first image
* @param      height2: height of first image
* @param      keys2: keypoint list extracted from first image
* @param      *pnb_match: pointer to return total number of matches
* @param      *pnb_feat_tot: (optional) pointer to return total nb of feat in keys1
* @return     tabmatch : a table containing the filtered matches
*/

Match * SiftObject::FindMatchesWithHT(int width1, int height1, Keypoint keys1, int width2, int height2, Keypoint keys2, int *pnb_match, int *pnb_feat_tot)
{
	Keypoint k=NULL, match, kmax=NULL, rtabfirst[ThetaDivMax][ScaleDivMax][XDivMax][YDivMax]; //, rtablast[ThetaDivMax][ScaleDivMax][XDivMax][YDivMax];
	int rtabnb[ThetaDivMax][ScaleDivMax][XDivMax][YDivMax];
	Match * tabmatch;
	float tmpX, tmpY, alpha;
	float cx, cy; // Center of first image
	int i,j,l,m,dOri,dScale,dX,dY,nbmax=0, count1,count2;
	tabmatch=NULL;
	DWORD time1,timeHT1,sumtime1=0,sumtime2=0;
	cx=width1/2;
	cy=height1/2;
	float log_scale_factor=log(scale_factor);
	
	time1 = GetTickCount(); // This works only with Windows
	
	/* Initialize table */
	for (i=0;i<ThetaDivMax;i++){
		for (j=0;j<ScaleDivMax;j++){
			for (l=0;l<XDivMax;l++){
				for (m=0;m<YDivMax;m++) {
					rtabfirst[i][j][l][m]=NULL;
					//        rtablast[i][j][l][m]=NULL;
					rtabnb[i][j][l][m]=0;
				}
			}
		}
	}
	/* Match the keys in list keys1 to their best matches in keys2.*/
	count1=0;
	int count3=0;
	for (k= keys1; k != NULL; k = k->next) {
		count3++;
		time1=GetTickCount();
		match = CheckForMatch(k, keys2, distance_ratio);
		sumtime1+=GetTickCount()-time1;
		
		/* If we have a match, perform double check (key2 with key list 1) */
		
		if (match != NULL && IsMatch(match, k, keys1, distance_ratio)) {
			k->match=match;
			k->match->flag=0;
			k->flag=0;
			timeHT1=GetTickCount();
			count1++;
			//cout<<"Found match("<<k->row<<","<<k->col<<","<<k->scale<<","<<k->ori<<")";
			//cout<<"  --->("<<match->row<<","<<match->col<<","<<match->ori<<","<<match->scale<<")"<<endl;
			
			/* Fill table of Hough Transform */
			// first calculate table entries
			dOri=ROUND(((Set2Range2Pi(match->ori - k->ori))/2/PI*theta_div_HT))-1;
			//dScale=ROUND((log((double)match->scale /(double)k->scale)/log(scale_factor))+ scale_div_HT/2)-1;
			dScale=ROUND((log(match->scale /k->scale)/log_scale_factor)+ scale_div_HT/2)-1;
			
			// Compute angle between Kp-center vector and feature vector
			float normPO=sqrt(SQUARE(k->col-cx,k->row-cy));
			alpha=atan2(cy-k->row,cx-k->col)-k->ori;
			tmpX= match->col + (match->scale/k->scale)* cos(match->ori+alpha)*normPO;
			tmpY= match->row + (match->scale/k->scale)* sin(match->ori+alpha)*normPO;
			dX=ROUND((tmpX * x_div_HT) / width2)-1;
			dY=ROUND((tmpY * y_div_HT) / height2)-1;
			//cout<<"  --->("<<dX<<","<<dY<<")"<<endl;
			
			/* Fill table of Hough transform */
			for (i=0;i<=1;i++){
				for (j=0;j<=1;j++){
					for (l=0;l<=1;l++){
						for (m=0;m<=1;m++) {
							int ii,jj,ll,mm;
							
							// Vote for the 2 best guess in each of the 4 dimension
							ii=(dOri+i+theta_div_HT)%theta_div_HT;
							if (ii>=theta_div_HT) {printf("Error : %s : %d", __FILE__, __LINE__);exit(-1);}
							jj=dScale+j;
							ll=dX+l;
							mm=dY+m;
							
							if ((jj>=0)&&(jj<scale_div_HT)&&(ll>=0)&&(ll<x_div_HT)&&(mm>=0)&&(mm<y_div_HT)) {
								/*Add to chain list*/
								Keypoint knew=Duplicate(k);
								knew->next2=rtabfirst[ii][jj][ll][mm];
								rtabfirst[ii][jj][ll][mm]=knew;
								
								rtabnb[ii][jj][ll][mm]+=1;
								//cout<<"rtabnb["<<ii<<"]["<<jj<<"]["<<ll<<"]["<<mm<<"]"<<rtabnb[ii][jj][ll][mm]<<endl;
								if (rtabnb[ii][jj][ll][mm]>nbmax)  // We keep the best bet
								{
									nbmax=rtabnb[ii][jj][ll][mm];
									kmax=rtabfirst[ii][jj][ll][mm];
								}
							}
						}
					}
				}
			}
													
			sumtime2+=GetTickCount()-timeHT1;
												
		}
	}
	
	/* Put all good matches in a table : */
	time1 = GetTickCount(); // This works only with Windows
	tabmatch = (Match*) malloc (nbmax * sizeof(Match));
	count2=0;
	if (tabmatch==NULL) { 
		cout<<"Error allocating memory"<<endl; exit(-1); 
	}
	if (kmax!=NULL)for (k= kmax; k != NULL; k = k->next2) {
		if (count2>=nbmax) {
			cout<<"Error while accessing tabmatch"<<endl; exit(-1);
		}
		tabmatch[count2].x1=k->col;
		tabmatch[count2].y1=k->row;
		tabmatch[count2].ori1=k->ori;
		tabmatch[count2].scale1=k->scale;
		tabmatch[count2].x2=k->match->col;
		tabmatch[count2].y2=k->match->row;
		tabmatch[count2].ori2=k->match->ori;
		tabmatch[count2].scale2=k->match->scale;
		tabmatch[count2].id=1;
		//        cout<<"Match found "<<tabmatch[count2].x1<<" "<<tabmatch[count2].y1<<" "<<tabmatch[count2].ori1<<" "<<tabmatch[count2].scale1<<endl;
		//        cout<<"  with "<<tabmatch[count2].x2<<" "<<tabmatch[count2].y2<<" "<<tabmatch[count2].ori2<<" "<<tabmatch[count2].scale2<<endl;
		count2++;
		
		k->match->flag=1;// Flag keypoint so that it cannot be used again
		
	}
	//cout<<"count2 "<<count2<<endl;
	//cout<<"nbmax "<<nbmax<<endl;
	*pnb_match=nbmax;
	cout<<"| Total time to find matches : "<<sumtime1<<endl;
	cout<<"| Total time for HT : "<<sumtime2<<endl;
	cout<<"| Build table in : "<<GetTickCount()-time1<<endl;
	
	//    cout<<"Found "<<count1<<" matches and "<<count2<<" coherent matches (after HT) : "<<"("<<orimax<<","<<scalemax<<")"<<endl;
	cout<<"| Found "<<count1<<" matches and "<<count2<<" coherent matches (after HT) "<<endl;
	
	//    while(nbmax>=3){
       time1=GetTickCount();
       
       /* Scan the discretization array to find 2nd max */
       nbmax=0;
       kmax=NULL;
       for (i=0;i<theta_div_HT;i++){
	       for (j=0;j<scale_div_HT;j++){
		       for (l=0;l<x_div_HT;l++){
			       for (m=0;m<y_div_HT;m++){
				       if (rtabfirst[i][j][l][m]!=NULL) {
					       rtabnb[i][j][l][m]=0;
					       for (k= rtabfirst[i][j][l][m]; k != NULL; k = k->next2) {
						       //            if(k->match->nbim!=1)rtabnb[i][j][l][m]+=1;
						       if (k->match->flag==0)rtabnb[i][j][l][m]=rtabnb[i][j][l][m]+1;
					       }
				       }
			       }
		       }
		}
					       
		if (rtabnb[i][j][l][m]>=nbmax) {
			nbmax=rtabnb[i][j][l][m];
			kmax=rtabfirst[i][j][l][m];
		}
	}
	if (nbmax>=3) cout<<"| Another set of coherent matches found !! : "<<nbmax<<endl;
	cout<<"| Time to scan table : "<<GetTickCount()-time1<<" ms"<<endl;
	if (pnb_feat_tot!=NULL)*pnb_feat_tot=count3;
	
	return tabmatch;
}
/*******************************************************************************/
/**
* Returns the estimated position and orientation using Mean Square approximation
*
*    -->matches that disagree with the transform are discarded
* @param      *tabmatch pointer to a table of Matches
* @param      nb_match size of tabmatch
* @param      maxdim maximal dimension of the 2nd image
* @param      *perr (optional) pointer to return error on transform
* @param      *pnb_match_good (optional) pointer to return error on transform
* @return The parameters of the approximed transform
*/
Matrice SiftObject::AffineParamApprox(Match * tabmatch, int nb_match, int maxdim, double * perr, int *pnb_match_good)
//int modelWidth), int modelHeight)
{
	Matrice *a, *u ,b(6,6),param(6,1,0),p1(2,1),p2(2,1),q1(2,1),q2(2,1),empty(0,0,0);
	int i, j;
	int maxindex;
	float maxerror=1;
	cout<<"(AT) Approx of affine param with "<<nb_match<<" matches."<<endl;
	if (perr!=NULL)*perr=1e99;
	
	/* count good matches*/
	//nb_match_good=0;
	for (i=0;i<nb_match;i++) {tabmatch[i].id=1;}//nb_match_good++;}
	int nb_match_good=nb_match;
	
	while (maxerror>0) {
		a=new Matrice(2*nb_match_good,6);
		u=new Matrice(2*nb_match_good,1);
		
		j=0;
		for (i=0;i<nb_match;i++) {
			if (tabmatch[i].id!=0) {
				/* First create the matrix A and u */
				a->set_val(2*j,0,tabmatch[i].x1);
				a->set_val(2*j,1,tabmatch[i].y1);
				a->set_val(2*j,4,1);
				a->set_val(2*j+1,2,tabmatch[i].x1);
				a->set_val(2*j+1,3,tabmatch[i].y1);
				a->set_val(2*j+1,5,1);
				u->set_val(2*j,0,tabmatch[i].x2);
				u->set_val(2*j+1,0,tabmatch[i].y2);
				j++;
			}
		}
		if (nb_match_good<nb_match_min_AT) {
			if (pnb_match_good!=NULL)*pnb_match_good=0;
			return empty;
		}
		b=a->t()*(*a);
		cout<<"Determinant of b ="<<b.det()<<endl;
		try
		{
			param = (b.inverse()*a->t())*(*u);
		}
		catch (Erreur &tmp)
		{
			cout << tmp.cause() << " - " << tmp.explic() << endl;
		}
		
		/* Test if a match fits the model */
		maxerror=0;
		/*Get size of projected image*/
		//        p1.set_val(0,0,0);
		//        p1.set_val(1,0,0);
		//        q1.set_val(0,0,modelWidth);
		//        q1.set_val(1,0,modelHeight);
		//        p2=Affine(p1,param);
		//        q2=Affine(p2,param);
		// float sqSizeOfProj=(q2-p2).sqnorm();
		//        float sqSizeOfProj=modelWidth*modelWidth+modelHeight*modelHeight;
		for (i=0;i<nb_match;i++) {
			if (tabmatch[i].id!=0) {
				
				/* Compute transform of keypoint vector*/
				
				p1.set_val(0,0,tabmatch[i].x1);
				p1.set_val(1,0,tabmatch[i].y1);
				q1.set_val(0,0,tabmatch[i].x1+tabmatch[i].scale1*cos(tabmatch[i].ori1));
				q1.set_val(1,0,tabmatch[i].y1+tabmatch[i].scale1*sin(tabmatch[i].ori1));
				p2=Affine(p1,param);
				q2=Affine(p2,param);
				
				/*Error between transform and reel match*/
				
				float dx=tabmatch[i].x2-p2.get_val(0,0);
				float dy=tabmatch[i].y2-p2.get_val(1,0);
				p2=q2-p2;
				//                float dtheta=abs(Set2RangePi(tabmatch[i].ori2-atan2(p2.get_val(1,0),p2.get_val(0,0))));
				//                cout<<i<<" "<<tabmatch[i].ori2<<" "<<atan2(p2.get_val(1,0),p2.get_val(0,0))<<" "<<dtheta<<endl;
				//                float rscale=abs(log(tabmatch[i].scale2/p2.norm()));
				
				if (dx*dx + dy*dy>agree_pos_AT * agree_pos_AT * maxdim * maxdim) {
					float tmp=sqrt(dx*dx+dy*dy)/agree_pos_AT/maxdim;
					//cout<<"(AT) match to be discarded :  rel error : pos "<<(dx*dx+dy*dy)/agree_pos_AT/agree_pos_AT<<endl;
					if (tmp>maxerror) {
						maxerror=tmp;
						maxindex=i;
					}
				}
			}
		}//end for
		
		if (maxerror>0) {
			tabmatch[maxindex].id=0;
			nb_match_good-=1;
			if (nb_match_good<nb_match_min_AT) {
				if (pnb_match_good!=NULL)*pnb_match_good=0;
				return empty;
			}
		}
	}//end while
	
	Matrice * c=new Matrice(2*nb_match_good,1);
	*c=((*a)*param)-(*u);
	float err=sqrt(2*c->sqnorm()/(2*nb_match_good-6));
	delete(c);
	if (perr!=NULL)*perr=err;
	
	delete(a);
	delete(u);
	
	
	cout<<"(AT) We keep "<<nb_match_good<<" matches:"<<maxerror<<" "<<maxindex<<endl;
	return param;
}
/*-------------------------------------------------------------------------*/
/** Returns the estimated position and orientation using Mean Square approx.
*    -->matches that disagree with the transform are discarded
* @param      *tabmatch pointer to a table of Matches
* @param      nb_match size of tabmatch
* @param      maxdim maximal dimension of the image
* @param      *perr (optional) pointer to return error on transform
* @param      *pnb_match_good (optional) pointer to return error on transform
* @return The parameters of the approximed transform
*/
Matrice SiftObject::SimilarityParamApprox(Match * tabmatch, int nb_match, int maxdim, double * perr, int *pnb_match_good)
{
	Matrice *a, *u, b(4,4),param(4,1,0),p1(2,1),p2(2,1),q1(2,1),q2(2,1),empty(0,0,0);
	int i, j;
	int maxindex;
	float maxerror=1, maxdtheta=0, maxrscale=0, maxdpos=0;
	
	int nb_match_good=nb_match;
	cout<<"(ST) Approx of similarity param with "<<nb_match<<" matches."<<endl;
	if (perr!=NULL)*perr=1e99;
	
	/* count good matches*/
	//    int nb_match_good=0;
	for (i=0;i<nb_match;i++) {tabmatch[i].id=1;} //;nb_match_good+=1;}
	while (maxerror>0) {
		
		a=new Matrice(2*nb_match_good,4,0);
		u=new Matrice(2*nb_match_good,1,0);
		
		j=0;
		for (i=0;i<nb_match;i++) {
			if (tabmatch[i].id!=0) {
				if (2*j+1>=2*nb_match_good) {cout<<"Error in ST"<<endl;}
				// First create the matrix A and u
				a->set_val(2*j,0,tabmatch[i].x1);
				a->set_val(2*j,1,-tabmatch[i].y1);
				a->set_val(2*j,2,1);
				a->set_val(2*j,3,0);
				a->set_val(2*j+1,0,tabmatch[i].y1);
				a->set_val(2*j+1,1,tabmatch[i].x1);
				a->set_val(2*j+1,2,0);
				a->set_val(2*j+1,3,1);
				u->set_val(2*j,0,tabmatch[i].x2);
				u->set_val(2*j+1,0,tabmatch[i].y2);
				j++;
			}
		}
		if (nb_match_good<nb_match_min_ST) {
			if (pnb_match_good!=NULL)*pnb_match_good=0;
			return empty;
		}
		b=a->t()*(*a);
		
		//      cout<<"Determinant of b = "<<b.det()<<endl;
		try
		{
			param = (b.inverse()*a->t())*(*u);
		}
		catch (Erreur &tmp)
		{
			cout << tmp.cause() << " - " << tmp.explic() << endl;
		}
		// Test if a match fits the model
		
		/*Get size of projected image*/
		//        p1.set_val(0,0,0);
		//        p1.set_val(1,0,0);
		//        q1.set_val(0,0,modelWidth);
		//        q1.set_val(1,0,modelHeight);
		//        p2=Similarity(p1,param);
		//        q2=Similarity(p2,param);
		//        float sqSizeOfProj=(q2-p2).sqnorm();
		////        float sqSizeOfProj=MAXOF2(1,SQUARE(modelWidth, modelHeight));
		
		// Test if a match fits the model
		maxerror=0;
		float theta=atan2(param.get_val(1,0),param.get_val(0,0));
		float logscale=sqrt(SQUARE(param.get_val(1,0),param.get_val(0,0)));
		logscale=log(logscale);
		
		for (i=0;i<nb_match;i++) {
			
			if (tabmatch[i].id!=0) {
				p1.set_val(0,0,tabmatch[i].x1);
				p1.set_val(1,0,tabmatch[i].y1);
				p2=Similarity(p1,param);
				/* Compute error for each match*/
				
				float dx=tabmatch[i].x2-p2.get_val(0,0);
				float dy=tabmatch[i].y2-p2.get_val(1,0);
				
				float dtheta=abs(Set2RangePi(theta-(tabmatch[i].ori2-tabmatch[i].ori1)));
				float rscale=abs(logscale-log(tabmatch[i].scale2/tabmatch[i].scale1));
				
				//Each match must agree within  orientation, scale and position
				if (dtheta>agree_ori_ST||rscale>agree_scale_ST||dx*dx+dy*dy>agree_pos_ST*agree_pos_ST*maxdim*maxdim) {
					//                if(dx*dx+dy*dy>agreePos*agreePos*sqSizeOfProj){
       float tmp=dtheta/agree_ori_ST+rscale/agree_scale_ST+sqrt(dx*dx+dy*dy)/agree_pos_ST/maxdim;
       if (tmp>maxerror) {
	       //cout<<"(ST)Match to be discarded : rel err : in ori:"<<dtheta/agree_ori_ST<<" in scale:"<<rscale/agree_scale_ST<<" in pos:"<<(dx*dx+dy*dy)/agree_pos_ST/agree_pos_ST<<endl;
	       
	       /* maxerror is the sum of weighted errors on position angle and scale*/
	       
	       maxerror=tmp;
	       maxdtheta=dtheta/agree_ori_ST;
	       maxrscale=rscale/agree_scale_ST;
	       maxdpos=sqrt(dx*dx+dy*dy)/agree_pos_ST/maxdim;
	       maxindex=i;
       }
				}
			}
		}//end for
		if (maxerror>0) {
			tabmatch[maxindex].id=0;
			cout<<"(ST) one match discarded on "<<nb_match_good<<" : err in pos:"<<maxdpos<<" angle:"<<maxdtheta<<" scale:"<<maxrscale<<endl;
			nb_match_good-=1;
			if (nb_match_good<nb_match_min_ST) {
				if (pnb_match_good!=NULL)*pnb_match_good=0;
				return empty;
			}
		}
	}
	cout<<"(ST) We keep "<<nb_match_good<<" matches:"<<maxerror<<" "<<maxindex<<endl;
	
	
	/*Compute transformation error*/
	
	
	Matrice * c=new Matrice(2*nb_match_good,1);
	*c=((*a)*param)-(*u);
	float err=sqrt(2*c->sqnorm()/(2*nb_match_good-4));
	delete(c);
	if (perr!=NULL)*perr=err;
	cout<<"Error in pose approximation : e="<<err<<endl;
	
	delete(a);
	delete(u);
	//if(nb_match_good<nb_match_min_ST) nb_match_good=0;
	if (pnb_match_good!=NULL)
		*pnb_match_good=nb_match_good;
	if (nb_match_good<nb_match_min_ST) return empty;
	else return param;
	
}
/*-------------------------------------------------------------------------*/
/** Returns the affine transform of a vector
* @param      vect : a 2 dimensional vector
* @param     param : 6x1 parameter matrix
*/
/*-------------------------------------------------------------------------*/
Matrice SiftObject::Affine(Matrice vect, Matrice param)
{
	if (param.size_m()!=6||param.size_n()!=1){
		cout<<"Wrong size of param in affine transform"<<endl; 
		exit(-1);
	}
	
	Matrice rot(2,2,0), transl(2,1,0);
	rot.set_val(0,0,param.get_val(0,0));
	rot.set_val(0,1,param.get_val(1,0));
	rot.set_val(1,0,param.get_val(2,0));
	rot.set_val(1,1,param.get_val(3,0));
	transl.set_val(0,0,param.get_val(4,0));
	transl.set_val(1,0,param.get_val(5,0));
	
	return Affine(vect,rot,transl);
}
/*-------------------------------------------------------------------------*/
/** Returns the affine transform of a vector
* @param     vect  a 2 dimensional vector
* @param     rot  2x2 rotation matrix
* @param     transl  2x1 translation vector
*/
/*-------------------------------------------------------------------------*/
Matrice SiftObject::Affine(Matrice vect, Matrice rot, Matrice transl)
{
	return rot*vect+transl;
}
/*-------------------------------------------------------------------------*/
/** Returns the similarity transform of a vector
* @param      vect  a 2 dimensional vector
* @param     param parameters of transform, 4x1 Matrix : (m=s*cos(t), n=s*sin(t), tx, ty)
*/
/*-------------------------------------------------------------------------*/
Matrice SiftObject::Similarity(Matrice vect, Matrice param)
{
	if (param.size_m()!=4||param.size_n()!=1)
	{cout<<"Wrong size of param in Similarity transform"<<endl;exit(1);}
	Matrice rot(2,2,0), transl(2,1,0);
	rot.set_val(0,0,param.get_val(0,0));
	rot.set_val(0,1,-param.get_val(1,0));
	rot.set_val(1,0,param.get_val(1,0));
	rot.set_val(1,1,param.get_val(0,0));
	transl.set_val(0,0,param.get_val(2,0));
	transl.set_val(1,0,param.get_val(3,0));
	
	return Affine(vect,rot,transl);
}
/*-------------------------------------------------------------------------*/
/** Returns the affine or similarity transform of a vector
* @param      vect  a 2 dimensional vector
* @param      param parameters of transform, a 6x1 matrix
*/
/*-------------------------------------------------------------------------*/
Matrice SiftObject::Transform(Matrice vect, Matrice param)
{
	if (param.size_m()==6 && param.size_n()==1) {
		return Affine(vect, param);
	}
	else if (param.size_m()==4 && param.size_n()==1) {
		return Similarity(vect, param);
	}
	else {cout<<"Wrong size of param in transform"<<endl;exit(1);}
}
/*-------------------------------------------------------------------------*/
/** Load templates into keypoints lists
* @param     * klist : Pointer to a list of keypoint to return
* @param     **imlist : Pointer to one image for each view
* @param     *pnbtp : Pointer to return total number of model views
* @param     *pnb_feat_tot : Pointer to return total number of features in all views
*/
/*-------------------------------------------------------------------------*/
void SiftObject::LoadTemplates(Keypoint * klist, IplImage **imlist, int *pnbtp, long * pnb_feat_tot)
{
	int nb_feat_tot=0; // FIX name
	
	const char* dir="./templates";
	DIR *d = opendir(dir);
	char namelist[NB_MAX_TEMPLATES][50];
	Keypoint klistInitial[NB_MAX_TEMPLATES];/* Contains the address of the klist before clustering any view to it*/
	Keypoint kp1;
	struct dirent *dirent;
	char full_name[50], buf[256], name[50];
	IplImage* im;
	int i, nb_match, bestnb_match, nb_match_good, bestMatch1, bestMatch2, bestMatch3;
	Match * tabmatch, * besttabmatch;
	double err, bestError1, bestError2, bestError3;
	//long nb_feat_tot=0;
	Matrice param(4,1,0);
	Matrice bestparam(4,1,0), emptymat(0,0,0);
	
	/*Allocate memory for keypoints list*/
	
	//    klist=(Keypoint*)malloc(NB_MAX_TEMPLATES * sizeof(Keypoint));
	if (klist==NULL)
	{cout<<"Error allocating memory in loading templates : klist"<<endl;exit(1);}
	
	int nbtp=0;
	int nbim=0;
	if (d==NULL) {cout<<"Template directory not found"<<endl;exit(1);}
	
	/*For all templates in directory*/
	
	while ((dirent = readdir(d)) != NULL)
	{
		if (strcmp(dirent->d_name, ".") && strcmp(dirent->d_name, ".."))
		{
			sprintf(full_name, "%s/%s", dir, dirent->d_name);
			sprintf(name,"%s",dirent->d_name);
			
			if (strlen(dirent->d_name)-4<1)
			{cout<<"Error in image file name :"<<full_name<<endl;exit(1);}
			//            strncpy(name,dirent->d_name,dirent->d_namlen-4);
			name[strlen(dirent->d_name)-4]='\0';
			
			
			im=cvLoadImage(full_name, 0);
			if (im==NULL)
			{cout<<"Error allocating memory for image while loading templates"<<endl;exit(1);}
			
			/*Call makekeyfile to build keyfile*/
			
			sprintf(buf,CMDMAKE, name, name);
			system(buf);
			
			/*Read features*/
			
			sprintf(buf,"key/%s.key",name);
			int nbfeat=0;
			kp1 = ReadKeyFile(buf, &nbfeat, nbim/*, klistEmpty*/);
			nb_feat_tot+=nbfeat;
			
			if (kp1==NULL) {cout<<"Error reading keyfile"<<buf<<endl;exit(1);}
			
			#if defined DISPLAY
			sprintf(buf,"./out/template%d-%s.jpg",nbim,name);
			DisplayMatches(im, NULL, NULL, 0, emptymat, buf, kp1, NULL);
			#endif
			
			/*-----View Clustering-----*/
			
			bestMatch1=-1;
			bestMatch2=-1;
			bestMatch3=-1;
			bestError1=99999;
			bestError2=99999;
			bestError3=99999;
			
			/* Try to match with all previous models view (with similar names only)*/
			
			for (i=0;i<nbtp;i++)if (CompareStringSpecial(name, namelist[i]) && clustering_threshold>0) {
				cout<<"________________________________________"<<endl;
				cout<<"| Compare "<<name<<" to model view "<<i<<endl;
				//                tabmatch=FindMatchesWithHT(im->width, im->height, kp1, imlist[i]->width, imlist[i]->height, klist[i], &nb_match, NULL);
				tabmatch=FindMatchesWithHT(im->width, im->height, kp1, imlist[i]->width, imlist[i]->height, klistInitial[i], &nb_match, NULL);
				cout<<"----------------------------------------"<<endl;
				
				if (nb_match>=nb_match_min_ST) {
					
					/* The 2 images match*/
					
					param=SimilarityParamApprox(tabmatch, nb_match, MAXOF2(imlist[i]->width, imlist[i]->height), &err, &nb_match_good);
					//param=AffineParamApprox(tabmatch, nb_match, &err, &nb_match_good);
					
					/*If a result for the transform is found */
					if (CountMatches(tabmatch,nb_match)>=nb_match_min_ST) {
						
						/*We keep the 3 best matches for later*/
						if (err<bestError1) {
							bestMatch3=bestMatch2;
							bestError3=bestError2;
							bestMatch2=bestMatch1;
							bestError2=bestError1;
							bestMatch1=i;
							bestError1=err;
							bestparam=param;
							besttabmatch=tabmatch;
							bestnb_match=nb_match;
						}
						else if (err<bestError2) {
							bestMatch3=bestMatch2;
							bestError3=bestError2;
							bestMatch2=i;
							bestError2=err;
						}
						else if (err<bestError3) {
							bestMatch3=i;
							bestError3=err;
						}
					}
				}
			}/*end for i*/
			
			/* Compare error to threshold */
			
			float thres=MAXOF2(im->width,im->height) * clustering_threshold;
			
			if (bestMatch1!=-1 && bestError1<=thres) {
				cout<<"It is a close match ("<<bestError1<<"<"<<thres<<") with model view "<<bestMatch1<<endl;
				
				/*The error is small : we cluster the 2 images*/
				
				ClusterViews(kp1,&klist[bestMatch1],bestparam);
				cout<<"--->Image "<<name<<" clustered with view "<<bestMatch1<<"(err in approx "<<bestError1<<", threshold "<<thres<<")"<<endl;
				#if defined DISPLAY
				sprintf(buf,"./out/%d-%s_added_to_model_view_%d.jpg",nbim,name,bestMatch1);
				DisplayMatches(im, imlist[bestMatch1], besttabmatch, bestnb_match, bestparam, buf, kp1, klist[bestMatch1]);
				#endif
				
			}
			else {
				
				/* The image is used as a new model view */
				if (bestMatch1!=-1)cout<<"It is not a close match ("<<bestError1<<">"<<thres<<") with model view "<<bestMatch1<<endl;
				else cout<<"No match found with any model view."<<endl;
				
				klist[nbtp]=kp1;
				klistInitial[nbtp]=kp1;
				imlist[nbtp]=im;
				sprintf(namelist[nbtp],"%s\n",name);
				
				cout<<"--->Image "<<name<<" used as new object model view "<<nbtp<<endl;
				#if defined DISPLAY
				sprintf(buf,"./out/%d-%s_used_as_new_model_view_%d.jpg",nbim,name, nbtp);
				DisplayMatches(im, NULL, NULL, 0, emptymat, buf, kp1, NULL);
				#endif
				nbtp+=1;
				if (nbtp>=NB_MAX_TEMPLATES) {cout<<"Error : too many templates"<<endl;exit(1);}
				
				if (bestMatch1>-1) {
					/* We link the new template with the model*/
					cout<<"--->Image "<<name<<" should be linked with model view "<<bestMatch1<<" (not implemented)"<<endl;
				}
				if (bestMatch2>-1) {
					/* We link the new template with the model*/
					cout<<"--->Image "<<name<<" should be linked with model view "<<bestMatch2<<" (not implemented)"<<endl;
				}
				if (bestMatch3>-1) {
					/* We link the new template with the model*/
					cout<<"--->Image "<<name<<" should be linked with model view "<<bestMatch3<<" (not implemented)"<<endl;
				}
			}/*end if(bestMatch1!=-1 && bestError1<=thres)*/
			//}/*End if(kp1)*/
			nbim++;
		}/*Endif condition on file name*/
	}/*End while*/
	closedir(d);
	
	#if defined DISPLAY
	for (i=0;i<nbtp;i++) {
		sprintf(buf,"./out/model_view_%d.jpg",i);
		DisplayMatches(imlist[i], NULL, NULL, 0, emptymat, buf, klist[i], NULL);
	}
	#endif
	
	
	if (pnb_feat_tot!=NULL)*pnb_feat_tot=nb_feat_tot;
	*pnbtp=nbtp;
	
	//    return klist;
}
/*-------------------------------------------------------------------------*/
/** Compute binomial distribution                                          */
/*-------------------------------------------------------------------------*/
double SiftObject::Binomial(double p, int k, int n) {
	double sum=0;
	//cout<<"p:"<<p<<"k:"<<k<<"n:"<<n<<endl;
	if (p*n>5 && (1-p)*n>5) {
		long factk=1;
		for (int i=2;i<=k;i++)factk*=k;
		sum=exp(-n*p)*n*pow(p,k)/factk;
	}
	else {
		for (int j=k;j<=n;j++) {
			BinomialProbability nbp(n,j,p);
			sum+=nbp.GetResult();
		}
	}
	return sum;
}

/*-------------------------------------------------------------------------*/
/** Cluster 2 close views of the same object
* @param     klist  keypoint list to be added
* @param     *pklist  pointer on list to be modified
* @param     param  parameters of similarity transform, a 4x1 Matrix
*/
/*-------------------------------------------------------------------------*/

void SiftObject::ClusterViews(Keypoint klist1, Keypoint *pklist2, Matrice param) {
	Keypoint k1, k2, knew;
	Matrice vect(2,1,0);
	if (param.size_m()!=4) {cout<<"Error in param in ClusterViews"<<endl;exit(1);}
	float dtheta=atan2(param.get_val(1,0),param.get_val(0,0));
	float rscale=sqrt(SQUARE(param.get_val(1,0),param.get_val(0,0)));
	int count=0;
	int nbdisc=0;
	
	for (k1=klist1;k1!=NULL;k1=k1->next) {
		//    knext=k1->next;
		/*Transform into coord of model view*/
		
		vect.set_val(0,0,k1->col);
		vect.set_val(1,0,k1->row);
		
		//vect=Similarity(vect,param);
		vect=Similarity(vect,param);
		
		if (k1->match!=NULL) {
			float sqDistToMatch=SQUARE(k1->match->col-vect.get_val(0,0),k1->match->row-vect.get_val(1,0));
			
			/*Compute closer non matching point*/
			
			float sqDistToNonMatch=1e99;
			for (k2=*pklist2;k2!=NULL;k2=k2->next) {
				float tmp=SQUARE(k2->col-vect.get_val(0,0),k2->row-vect.get_val(1,0));
				if (tmp<sqDistToNonMatch && tmp>4 && k2!=k1->match) sqDistToNonMatch=tmp;
				//                if (tmp<sqDistToNonMatch && tmp>1 && !IsMatch(k1, k2, *pklist2,distance_ratio)) sqDistToNonMatch=tmp;
			}
			
			/*If the transformed kp is close to match we discard it*/
			
			
			if (sqDistToMatch<=sqDistToNonMatch/9.0) {
				//                cout<<"kp discarded"<<endl;
				//                free(k1->descript);
				//                free(k1);
				nbdisc++;
				continue;
			}
		}
		/*Add kp to list*/
		knew=(Keypoint) malloc(sizeof(struct KeypointSt));
		
		knew->col=vect.get_val(0,0);
		knew->row=vect.get_val(1,0);
		
		knew->ori=k1->ori+dtheta;
		knew->scale=k1->scale*rscale;
		knew->nbim=k1->nbim;
		knew->descrip=k1->descrip; // Watch out! they both point on the same descrip array
		//     knew->next2=NULL;
		knew->match=NULL;
		knew->sq_dist_thres=k1->sq_dist_thres;
		knew->flag=k1->flag;
		knew->next=*pklist2;
		
		
		*pklist2=knew;
		
		
		count ++;
		//        suivant:;
	}
	cout<<"List modified : "<<nbdisc<<" keypoint discarded to avoid repetitions."<<endl;
	
}
/*-------------------------------------------------------------------------*/
/** Creates an image file displaying certain results of the computation
* @param     im1 : first image
* @param     im1 : second image
* @param     *tabmatch : table of matches
* @param     nb_match : size of tabmatch
* @param     param : parameters of affine or similarity transform
* @param     fileName : name of image file
* @param     k1 : (optional) 1st list of keypoints to be displayed
* @param     k2 : (optional) 2nd list of keypoints to be displayed
*/
/*-------------------------------------------------------------------------*/
void SiftObject::DisplayMatches(IplImage* im1, IplImage* im2, Match * tabmatch, int nb_match, Matrice param, char * fileNameOut, Keypoint k1, Keypoint k2) 
{
	IplImage * im,* imcol;
	CvSize s;
	CvPoint pt1, pt2;
	CvScalar color;
	int i,j,im2null=0;
	
	if (im2==NULL) {
		s.width=1;
		s.height=1;
		im2=cvCreateImage(s, 8, 1);
		im2null=1;
	}
	
	/* Paste the two images in one */
	s.width=MAXOF2(im1->widthStep,im2->widthStep);
	//s.width=((s.width-1)/10)*10+10;  // This magical line allows the output to be correct (width=widthstep)
	s.height=im1->height+im2->height;
	im=cvCreateImage(s, 8, 1);
	cvZero(im);
	imcol=cvCreateImage(s, 8, 3);
	cvZero(imcol);
	
	
	for (i = 0; i < im1->height; i++)
		for (j = 0; j < im1->width; j++) {
			im->imageData[i*im->width+j] = im1->imageData[i*im1->widthStep+j];
		}
		for (i = 0; i < im2->height; i++)
			for (j = 0; j < im2->width; j++) {
				im->imageData[(i+im1->height)*im->width+j] = im2->imageData[i*im2->widthStep+j];
			}
			
		/*Copy image in colors*/
		
		for (i=0;i<imcol->imageSize;i++)imcol->imageData[i]=im->imageData[i/3];
		
		/*Display matches*/
		
		for (i=0;i<nb_match;i++) {
			pt1.x=ROUND(tabmatch[i].x1);
			pt1.y=ROUND(tabmatch[i].y1);
			pt2.x=ROUND(tabmatch[i].x2);
			pt2.y=ROUND(tabmatch[i].y2+im1->height);
			if (tabmatch[i].id==0)color=CV_RGB(255,0,0); //red for discarded matches
				else color=CV_RGB(0,255,0); //green
					
					cvLine(imcol, pt1, pt2, color, 1);
				cvCircle(imcol, pt1, 3, color, 1);
			cvCircle(imcol, pt2, 3, color, 1);
		}
		
		
		/*Affine transform of the image border*/
		
		if (param.size_m()>0) {
			Matrice p1(2,1), p2(2,1), p3(2,1), p4(2,1), transl(2,1);
			transl.set_val(0,0,0);
			transl.set_val(1,0,im1->height);
			p1.set_val(0,0,0);
			p1.set_val(1,0,0);
			p2.set_val(0,0,im1->width);
			p2.set_val(1,0,0);
			p3.set_val(0,0,im1->width);
			p3.set_val(1,0,im1->height);
			p4.set_val(0,0,0);
			p4.set_val(1,0,im1->height);
			
			p1=Transform(p1,param)+transl;
			p2=Transform(p2,param)+transl;
			p3=Transform(p3,param)+transl;
			p4=Transform(p4,param)+transl;
			
			color=CV_RGB(0,128,255); //light blue
			pt1.x=ROUND(p1.get_val(0,0));
			pt1.y=ROUND(p1.get_val(1,0));
			pt2.x=ROUND(p2.get_val(0,0));
			pt2.y=ROUND(p2.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			pt1.x=ROUND(p2.get_val(0,0));
			pt1.y=ROUND(p2.get_val(1,0));
			pt2.x=ROUND(p3.get_val(0,0));
			pt2.y=ROUND(p3.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			pt1.x=ROUND(p3.get_val(0,0));
			pt1.y=ROUND(p3.get_val(1,0));
			pt2.x=ROUND(p4.get_val(0,0));
			pt2.y=ROUND(p4.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			pt1.x=ROUND(p4.get_val(0,0));
			pt1.y=ROUND(p4.get_val(1,0));
			pt2.x=ROUND(p1.get_val(0,0));
			pt2.y=ROUND(p1.get_val(1,0));
			cvLine(imcol, pt1, pt2, color, 1);
			
			/* Draw the border of the object */
			CvMemStorage *storage= cvCreateMemStorage (0); /* Memory used by openCV */
			int header_size = sizeof( CvContour );
			CvSeq *contours;
			
			IplImage* imthres = cvCreateImage(cvSize(im1->width,im1->height),IPL_DEPTH_8U, 1 );
			cvCopy( im1, imthres, 0 );
			
			/* First find the contour of a thresholded image*/
			
			cvThreshold(imthres, imthres, border_threshold, 255, CV_THRESH_BINARY );
			cvFindContours ( imthres, storage, &contours, header_size, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
			
			/* For each contour found*/
			
			while ( contours != NULL) {
				double area=fabs(cvContourArea(contours,CV_WHOLE_SEQ)); // compute area
				if ( area > 20) {
					for (int i=0;i<contours->total;i++) {
						
						/* Compute transform of contour*/
						
						CvPoint* cvpt=(CvPoint*)cvGetSeqElem( contours, i);
						p1.set_val(0,0,cvpt->x);
						p1.set_val(1,0,cvpt->y);
						p1= Transform(p1,param) + transl;
						cvpt->x=ROUND(p1.get_val(0,0));
						cvpt->y=ROUND(p1.get_val(1,0));
					}
					//                cvDrawContours( imcol, contours, CV_RGB(0,0,255),CV_RGB(255,0,0),0,2,8);
					cvDrawContours( imcol, contours, CV_RGB(0,0,255),CV_RGB(0,0,255),0,2,8);
				}
				contours = contours->h_next; // ?????
			}
			free( contours );
			cvReleaseMemStorage( &storage );
			
		}
		/*Display keypoints*/
		if (k1!=NULL)for (Keypoint k=k1;k!=NULL;k=k->next) {
			//        color=CV_RGB(255,255,255); //white
			color=color_array[k->nbim % NBCOLORS];
			pt1.x=ROUND(k->col);
			pt1.y=ROUND(k->row);
			cvCircle(imcol, pt1, 3, color, 1);
			
			if (im2null) {

				// Display keypoints vectors
				pt2.x=ROUND(k->col + k->scale * 1 * cos(k->ori));
				pt2.y=ROUND(k->row + k->scale * 1 * sin(k->ori));
				cvLine(imcol, pt1, pt2, color, 1);
				
			}
		}
		if (k2!=NULL)
		for (Keypoint k=k2;k!=NULL;k=k->next) {
			//        color=CV_RGB(255,255,255); //white
			color=color_array[k->nbim % NBCOLORS];
			pt1.x=ROUND(k->col);
			pt1.y=ROUND(k->row+im1->height);
			cvCircle(imcol, pt1, 3, color, 1);
		}
		cvSaveImage(fileNameOut,imcol);
							
}
/*-------------------------------------------------------------------------*/
/** Compute probability of presence given matches
* (this function should be improved)
*/
/*-------------------------------------------------------------------------*/
float SiftObject::ProbOfPresence(int nb_feat_model, int nb_feat_tot, int nb_match_good, int nbfeatinbound) {
	//int k,n;
	//cout<<"Probofpres  "<<nb_feat_model<<" "<<nb_feat_tot<<" "<<nb_match_good<<" "<<nbfeatinbound<<endl;
	float l=0.2*0.2;
	float r=30.0/360.0;
	float s=0.5;
	float pm=0.1;
	/*Probability of matching if model not present*/
	double p=(double)nb_feat_model/nb_feat_tot * l*r*s ;
	/*Probability of matching a single feature*/
	float pfnm=Binomial(p, nb_match_good, nbfeatinbound);
	
	float pmf=pm/(pm+pfnm);
	cout<<"p="<<p<<" pfnm="<<pfnm<<" pmf="<<pmf<<endl;
	
	return pmf;
}
/*-------------------------------------------------------------------------*/
/** Count features in the projection (in second image) of the border of first image
* @param   width dimension of first image
* @param   height dimension of first image
* @param   klist List of kp of second image
* @param   param similarity transform parameters
*/
/*-------------------------------------------------------------------------*/
int SiftObject::CountFeatInProj(int width, int height, Keypoint klist, Matrice param) {
	int count=0;
	Keypoint k;
	Matrice vect(2,1,0),va1, va2, va3, va4;
	
	if (param.size_m()!=4||param.size_n()!=1)
	{
		cout<<"no valid projection"<<endl;
		return 0;
	}
	
	/* Projection of bound of 1st image*/
	vect.set_val(0,0,0);
	vect.set_val(1,0,0);
	Matrice p1=Similarity(vect,param);
	vect.set_val(0,0,0);
	vect.set_val(1,0,height);
	Matrice p2=Similarity(vect,param);
	vect.set_val(0,0,width);
	vect.set_val(1,0,height);
	Matrice p3=Similarity(vect,param);
	vect.set_val(0,0,width);
	vect.set_val(1,0,0);
	Matrice p4=Similarity(vect,param);
	va1=p2-p1;
	va2=p3-p2;
	va3=p4-p3;
	va4=p1-p4;
	
	/*For each kp we check if it is inside the boundaries*/
	
	for (k=klist;k!=NULL;k=k->next) {
		vect.set_val(0,0,k->col);
		vect.set_val(1,0,k->row);
		
		Matrice vb1=vect-p1;
		Matrice vb2=vect-p2;
		Matrice vb3=vect-p3;
		Matrice vb4=vect-p4;
		
		if (Prodvect(va1,vb1)<=0 &&Prodvect(va2,vb2)<=0 && Prodvect(va3,vb3)<=0 && Prodvect(va4,vb4)<=0 )
			count++;
	}
	
	
	return count;
}
/*-------------------------------------------------------------------------*/
/** SQNorm of vect product                               */
/*-------------------------------------------------------------------------*/
float SiftObject::Prodvect(Matrice v1, Matrice v2) {
	
	return v1.get_val(0,0)*v2.get_val(1,0)-v1.get_val(1,0)*v2.get_val(0,0);
}

/*-------------------------------------------------------------------------*/
/** Set an angle in range [0;2Pi]                                           */
/*-------------------------------------------------------------------------*/
double SiftObject::Set2Range2Pi(double angle)
{
	double a=angle;
	while (a<0)a+=2*PI;
	while (a>=2*PI)a-=2*PI;
	return a;
}

/*-------------------------------------------------------------------------*/
/** Set an angle in range [-Pi,Pi]                                           */
/*-------------------------------------------------------------------------*/
double SiftObject::Set2RangePi(double angle)
{
	double a=angle;
	while (a<-PI)a+=2*PI;
	while (a>=PI)a-=2*PI;
	return a;
	
}


/*-------------------------------------------------------------------------*/
/** Read data from config.ini and update global variables                  */
/*-------------------------------------------------------------------------*/
void SiftObject::ReadConfig() {
	char *buf, section[50], key[50];//, emptyTemplate[50];
	
	/* param for SIFT */
	strcpy(section,"sift_features_extraction");
	strcpy(key,"distance_ratio");
	
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%f",&distance_ratio)==0) {
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl;
		exit(-1);
	}
	
//	strcpy(key,"empty_template");
//	klistEmpty=NULL;
	
	strcpy(section,"hough_transform");
	
	strcpy(key,"division_in_position_x");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%d",&x_div_HT)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	strcpy(key,"division_in_position_y");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%d",&y_div_HT)==0) 
	{
		cout<<"Error in "<<INIFILE<<" : "<<section<<" "<<key<<endl; exit(-1); 
	}
	
	strcpy(key,"division_in_angle");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%d",&theta_div_HT)==0) 
	{ 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1); 
	}
	
	strcpy(key,"division_in_scale");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%d",&scale_div_HT)==0) {
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1); 
	}
	
	strcpy(key,"scale_factor");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%f",&scale_factor)==0) {
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1); 
	}
	
	
	/* param for affine transform */
	
	strcpy(section,"affine_transform");
	
	strcpy(key,"agree_in_position");
	if (readINI (section, key, &buf, INIFILE)==0||
		sscanf(buf,"%f",&agree_pos_AT)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	strcpy(key,"nb_match_min");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%d",&nb_match_min_AT)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	/* param for similarity transform */
	
	strcpy(section,"similarity_transform");
	
	strcpy(key,"agree_in_position");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%f",&agree_pos_ST)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	strcpy(key,"agree_in_angle");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%f",&agree_ori_ST)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	strcpy(key,"agree_in_scale");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%f",&agree_scale_ST)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	strcpy(key,"nb_match_min");
	if (readINI (section, key, &buf, INIFILE)==0||
		sscanf(buf,"%d",&nb_match_min_ST)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	/* param for display */
	
	strcpy(section,"display");
	
	strcpy(key,"border_threshold");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%d",&border_threshold)==0) { 
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	/* param for clustering */
	
	strcpy(section,"clustering");
	
	strcpy(key,"clustering_threshold");
	if (readINI (section, key, &buf, INIFILE)==0||sscanf(buf,"%f",&clustering_threshold)==0) {
		cout<<"Error in "<<INIFILE<<" "<<section<<" "<<key<<endl; exit(-1);
	}
	
	// Keeps param in good range
	agree_scale_ST=log(agree_scale_ST);
	agree_ori_ST=agree_ori_ST*PI/180;
	theta_div_HT=RANGE(theta_div_HT,1,ThetaDivMax);
	scale_div_HT=RANGE(scale_div_HT,1,ScaleDivMax);
	x_div_HT=RANGE(x_div_HT,1,XDivMax);
	y_div_HT=RANGE(y_div_HT,1,YDivMax);
	
	cout<<"distance_ratio:"<<distance_ratio<<endl;
	cout<<"x_div_HT:"<<x_div_HT<<endl;
	cout<<"y_div_HT:"<<y_div_HT<<endl;
	cout<<"theta_div_HT:"<<theta_div_HT<<endl;
	cout<<"scale_div_HT:"<<scale_div_HT<<endl;
	cout<<"scale_factor:"<<scale_factor<<endl;
	cout<<"agree_pos_AT:"<<agree_pos_AT<<endl;
	cout<<"agree_pos_ST:"<<agree_pos_ST<<endl;
	cout<<"agree_ori_ST:"<<agree_ori_ST<<endl;
	cout<<"clustering_threshold:"<<clustering_threshold<<endl;
	cout<<"agree_scale_ST:"<<agree_scale_ST<<endl;
	cout<<"border_threshold:"<<border_threshold<<endl;
	//cout<<"emptyTemplate:"<<emptyTemplate<<endl;
	
}
/*-------------------------------------------------------------------------*/
/** Compares two string (stops comparaison at character '_')                  */
/*-------------------------------------------------------------------------*/
int SiftObject::CompareStringSpecial(const char * str1, const char * str2) {
	int i=0;
	
	while (str1[i]==str2[i]) {
		if (str1[i]=='_'||str1[i]=='.'||str1[i]=='\0'||str1[i]=='\n')
			return 1;
		else i++;
	}
	return 0;
	
}
/*-------------------------------------------------------------------------*/
/** Counts numbers of matches with different coordinates                  */
/*-------------------------------------------------------------------------*/
int SiftObject::CountMatches(Match * tabmatch, int nb_match0) {
	int cpt=nb_match0;
	for (int i=0;i<nb_match0;i++)
		if (tabmatch[i].id!=0)
			for (int j=i+1;j<nb_match0;j++)
				if (tabmatch[j].id!=0 && tabmatch[i].x1==tabmatch[j].x1 && tabmatch[i].y1==tabmatch[j].y1) {
					cpt--;
					j+=nb_match0; /*to get out of the loop*/
				}
				
				cout<<"realnbm"<<cpt<<" on "<<nb_match0<<endl;
			
	return cpt;
		
}

/*-------------------------------------------------------------------------*/
/** Load keypoints              */
/*-------------------------------------------------------------------------*/

void SiftObject::loadKeyPoints()
{
	char buf[256];
	//assert(!IsEmpty());
	sprintf(buf,"key/%s.key", name);
	keypoints= ReadKeyFile(buf, &nb_feat, 0, NULL);
	nb_feat_tot=nb_feat;

	printf("open key/%s.key %d feat\n", name, nb_feat);
}

/*-------------------------------------------------------------------------*/
/** SiftComparison                                                         */
/*-------------------------------------------------------------------------*/


SiftComparison::SiftComparison()
{
	name="";
	prob_presence=0;
	feat_in_proj=0;
	nb_match=0;
	//int nb_match_good;
	//nb_feat_model=0;
	//nb_feat_tot=0;
}


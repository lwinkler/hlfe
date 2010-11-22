
#ifndef COMPARATOR_H
#define COMPARATOR_H

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
#include "match.h"

#include "matrice.h"
#include "classprob/Probability.h"
#include "classprob/BinomialProbability.h"
#include <iostream>
#include <string>
#include <cmath>
#include <time.h>
#include <dirent.h>

#include "inifile/filetools.h"
/// The number of elements of color_array
#define NBCOLORS 54


class SiftObject;

class SiftComparison
{
	public :
		SiftComparison();
		string name;
		/// Sift properties of comparison
		float prob_presence;
		int feat_in_proj;
		int nb_match;
		
		Match *tab_match;
		//int nb_match_good;
		//int nb_feat_model;
		//int nb_feat_tot;
};

class SiftObject
{
	/*------------------------------ Static variables -------------------------*/
	private:
		/// Distance ratio threshold for matching of two sets of features
		static float distance_ratio;
		/// Parameter for Hough transform : division in position X
		static int  x_div_HT;
		/// Parameter for Hough transform : division in position Y
		static int y_div_HT;
		/// Parameter for Hough transform : division in angle
		static int theta_div_HT;
		/// Parameter for Hough transform : division in scale
		static int scale_div_HT;
		
		/// Parameter for Hough transform : ratio between two divisions in scale
		static float scale_factor;
		/// Number min of match to make an affine transform
		static int nb_match_min_AT;
		/// Param for match filtering in position with Affine transform
		static float agree_pos_AT;
		/// Number min of match to make a similarity transform
		static int nb_match_min_ST;
		/// Param for match filtering in position with Similarity transform
		static float agree_pos_ST;
		/// Param for match filtering in orientation with Similarity transform
		static float agree_ori_ST;
		/// Param for match filtering in scale with Similarity transform
		static float agree_scale_ST;
		
		/// Display : how to set the threshold to extract border
		static int border_threshold;
		
		/// Keypoint list to set the default template (NOT USED YET)
		//static Keypoint klistEmpty;
		/// Threshold to determine if we cluster the views (norm 0.05)
		static float clustering_threshold;
		
		static const CvScalar color_array[54];
	
	/*------------------------------ Methods declarations -------------------------*/
	public:
		SiftObject();
		SiftObject(const char* _name);
		~SiftObject();
		
		SiftComparison Compare1(const SiftObject& obj)const;
		bool IsEmpty()const;
		void Reset();
		int Load(const char* name);
	
	private:
		int main1 (int argc, char **argv);
		static Match * FindMatchesWithHT(int width1, int height1, Keypoint keys1, int width2, int height2, Keypoint keys2, int *nbmatch, int * pnbfeatmodel=NULL);
		//void DrawLine(Image image,Matrice p1, Matrice p2, int color);
		static void LoadTemplates(Keypoint * klist, IplImage **imlist, int *pnbtp, long * pnbfeattot);
		static Matrice SimilarityParamApprox(Match * tabmatch, int nbmatch, int maxdim, double * perr, int *pnbmatchgood);
		static Matrice Similarity(Matrice vect, Matrice similarityParam);
		static Matrice Transform(Matrice vect, Matrice param);
		static void DisplayMatches(IplImage* im1, IplImage* im2, Match * tabmatch, int nbmatch, Matrice param, char * fileName, Keypoint k1=NULL, Keypoint k2=NULL);
		static double Binomial(double p, int k, int n);
		static void ClusterViews(Keypoint klist1, Keypoint *pklist2, Matrice param);
		static float ProbOfPresence(int nbfeatmodel, int nbfeattot, int nbmatchgood, int nbfeatinbound);
		static int CountFeatInProj(int width, int height, Keypoint klist, Matrice param);
		static float Prodvect(Matrice v1, Matrice v2);
		static int CompareStringSpecial(const char * str1, const char * str2);
		static int CountMatches(Match * tabmatch, int nbmatch);
		static Matrice Affine(Matrice vect, Matrice affineParam);
		static Matrice Affine(Matrice vect, Matrice rot, Matrice transl);
		static Matrice AffineParamApprox(Match * tabmatch, int nbmatch, int maxdim, double * error=NULL, int * nbmatchgood=NULL);
		static double Set2RangePi(double angle);
		static double Set2Range2Pi(double angle);
		void loadKeyPoints();
		
		static void ReadConfig();
		
	public:
		char fileName[256];
		char name[256];
		IplImage* im;
		Keypoint keypoints;
		
		/// Sift properties of image
		int nb_feat;
		int nb_feat_model;
		int nb_feat_tot;

};

#endif

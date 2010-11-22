/************************************************************************
Demo software: Invariant keypoint matching.
Author: David Lowe

defs.h:
This file contains the headers for a sample program to read images and
  keypoints, then perform simple keypoint matching.
*************************************************************************/
#ifndef DEFS_H
#define DEFS_H
/* From the standard C libaray: */
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

//#define OS_WINDOWS
#define OS_LINUX

#define DISPLAY

/*------------------------------ Constants ---------------------------------*/

#define ThetaDivMax 12
#define ScaleDivMax 10
#define XDivMax 10
#define YDivMax 10
#define NB_MAX_TEMPLATES 300
//#define NB_MIN_MATCH_AT 3
//#define NB_MIN_MATCH_ST 6

#define PI 3.14159265
#define ClWhite 255
#define ClBlack 0
#define ClGray 127

#ifdef OS_WINDOWS
/*For windows : */
#define CMDMAKE "makekeyfile %s"
#endif

#ifdef OS_LINUX
/*For Linux : */
#define CMDMAKE "make pgm/%s.pgm key/%s.key"
#endif


#define INIFILE "config.ini"

/*------------------------------ Macros  ---------------------------------*/

#define ABS(x)    (((x) > 0) ? (x) : (-(x)))
#define MAXOF2(x,y)  (((x) > (y)) ? (x) : (y))
#define MINOF2(x,y)  (((x) < (y)) ? (x) : (y))
#define ROUND(x)  ((int)(x+0.5))
#define RANGE(x,a,b) (MINOF2(MAXOF2(x,a),b))
#define PRINT(x) cout<<"x"<<x<<endl;
#define SQUARE(x,y) (x)*(x)+(y)*(y)


#ifdef OS_LINUX
#define GET_TIME_MS
#endif
#ifdef OS_WINDOWS
#define GET_TIME_MS
#endif


/*---------------------------- Structures --------------------------------*/

/* Data structure for a float image.
*/
//typedef struct ImageSt {
//  int rows, cols;          /* Dimensions of image. */
//  float **pixels;          /* 2D array of image pixels. */
//  struct ImageSt *next;    /* Pointer to next image in sequence. */
//} *Image;


/* Data structure for a keypoint.  Lists of keypoints are linked
   by the "next" field.
*/
typedef struct KeypointSt {
	float row, col;             /* Subpixel location of keypoint. */
	float scale, ori;           /* Scale and orientation (range [-PI,PI]) */
	int nbim;                   /* Image number */
	char flag;                   /* Flag for use in program */
	float sq_dist_thres;            /// Default match threshold
	unsigned char *descrip;     /* Vector of descriptor values */
	struct KeypointSt *next;    /* Pointer to next keypoint in list. */
	struct KeypointSt *next2;    /* Pointer to next keypoint in list. */
	struct KeypointSt *match;    /* Pointer to matched point. */
} *Keypoint;

/*Datastruct of a match : 2 keypoint coordinates*/
typedef struct MatchSt {
	int id;
	float x1;
	float y1;
	float ori1;
	float scale1;
	float x2;
	float y2;
	float ori2;
	float scale2;
} Match;

#endif
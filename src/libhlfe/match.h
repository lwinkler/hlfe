#ifndef MATCH_H
#define MATCH_H

//Image ReadPGMFile(char *filename);
Keypoint ReadKeyFile(const char *filename, int * pnbfeat, int nbim, Keypoint key_no_match=NULL);
int UnloadKeys(Keypoint* keylist);
//void FindMatches(Image im1, Keypoint keys1, Image im2, Keypoint keys2, float distRatio);
Keypoint CheckForMatch(Keypoint key, Keypoint klist, float distRatio);
//Image CombineImagesVertically(Image im1, Image im2);
//void DrawLine(Image image, int r1, int c1, int r2, int c2, int color);
//void WritePGM(FILE *fp, Image image);
Keypoint Duplicate(Keypoint key);
int IsMatch(Keypoint k1, Keypoint k2, Keypoint klist2, float distRatio);

#endif
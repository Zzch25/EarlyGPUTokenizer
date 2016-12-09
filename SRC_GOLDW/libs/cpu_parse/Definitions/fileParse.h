/*
 *This puppy is gonna go up on github since I'll be using
 *it in my next project so before I forget...
 *
 *(c) Zachary Job
 *Sharing is caring, use requires that I am contacted and
 *give permission. OR the code can be displayed without
 *modification including original comments for educational
 *purposes!
 *
 *fileParse.h
 *4/15/2015
 *
 *prototypes for file parser
 */

#ifndef _FILEPARSE_H_
#define _FILEPARSE_H_

#include "../Definitions/defs.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
void seekThreadOffs(long *readIdx, int TID, int *laststate);
void nextToken(long *readIdx, int *laststate, int *pages, 
				int *wrTOP, int *wrLOW, int *last, int *lToken);
void initThreadData(int *memMapSin, int *memMapEin, int *dirtyBitsin, int *memOffsetin,
						int *trimArrin, struct tags_t *configTagsin, char **fileBuffer, char *data);

#ifdef __cplusplus
}
#endif

int getUTFencode(char *fileBuffer, long *readIdx);

#endif
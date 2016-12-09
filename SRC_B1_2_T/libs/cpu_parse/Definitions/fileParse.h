/*
 *This puppy is gonna go up on github since I'll be using
 *it in my next project so before I forget...
 *
 *(c) Zachary Job
 *All rights reserved, I am not liable for damages.
 *Re-distributable and use with my permission contact me
 *at zachjob@verizon.net
 *
 *Presentable without modification including comments 
 *for educational purposes
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
		
int seekThreadOffs(long *readIdx, int TID);
int nextToken(long *readIdx, int laststate, int *pages, 
				int *wrTOP, int *wrLOW, int *last, int *lToken);
void initThreadData(int *memMapSin, int *memMapEin, int *dirtyBitsin, int *memOffsetin,
						int *trimArrin, struct tags_t *configTagsin, char **fileBuffer, char *data);

#ifdef __cplusplus
}
#endif

int getUTFencode(char *fileBuffer, long *readIdx);

#endif
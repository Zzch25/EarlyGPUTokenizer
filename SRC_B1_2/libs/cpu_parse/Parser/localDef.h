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
 *localdef.h
 *4/15/2015
 *
 *prototype for file parser
 */

#ifndef _LOCALDEF_H_
#define _LOCALDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Definitions/defs.h"

int parseContent(char *fileBuffer, long *readIdx, int eot, int *wrTOP, int *wrLOW, int *last, int *lToken);
int seekInnerTag(char *fileBuffer, long *readIdx, int *tags);
int seekOuterTag(char *fileBuffer, long *readIdx, int *tags);
void dataInit(char **fileBuffer, long *readIdx, char *dataName);
void initTags(char *fileBuffer, struct tags_t *configTagsIn);
void initParse(int *memMapSin, int *memMapEin, int *dirtyBitsin, int *memOffsetin, int *trimArrin);

#endif
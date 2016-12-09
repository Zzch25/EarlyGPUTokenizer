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
 *fileInit.h
 *4/15/2015
 *
 *prototypes for the loader call
 */

#ifndef _FILESINIT_H_
#define _FILESINIT_H_

#include "defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

void cfgInit(char *cfg, int **memMapS, int **memMapE, int **dirtyBits, 
			int **memOffset, int optCmp, struct tags_t **configTags, int **trimArr);

#ifdef __cplusplus
}
#endif
			
#endif
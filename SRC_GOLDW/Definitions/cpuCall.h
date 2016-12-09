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
 *cpuCall.h
 *4/15/2015
 *
 *Prototypes for CPU module wrapper and function calls
 */

#ifndef _CPUCALL_H_
#define _CPUCALL_H_

#include "../libs/cpu_parse/cpuFeed.h"

#ifdef __cplusplus
extern "C"
{
#endif

//Initialise parsing data
void fillBufferInit(char *config, int optCmp, int **memMapS, int **memMapE, int **dirtyBits,
						int **memOffset, int **trimArr, struct tags_t **configTags);

#ifdef __cplusplus
}
#endif

void *fBThreadMgr(void *arg);

#endif
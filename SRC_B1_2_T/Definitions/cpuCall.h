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

#endif
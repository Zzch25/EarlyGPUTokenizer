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

#include "../Definitions/defs.h"

void getHeaders(FILE *fd, struct tags_t **configTags);
int makeMaps(FILE *fd, int **memMapS, int **memMapE, int **dirtyBits, 
							int **memOffset, int **trimArr, int prec);
void fillMaps(FILE *fd, int *memMapS, int *memMapE, int *dirtyBits, 
					int *memOffset, int *trimArr, int prec);
void preCompute(FILE *fd);
void loadPreCompute(FILE *fd, int **memMapS, int **memMapE,
					int **dirtyBits, int **memOffset, int **trimArr);

#endif
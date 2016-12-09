/*
 *Zachary Job
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
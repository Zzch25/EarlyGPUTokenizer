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
 *preCompute.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Definitions/bits.h"
#include "../Definitions/defs.h"

#include "localDef.h"

/*
 *Grabs computations and writes a precompute
 *file
 *
 *@PARAM: The configuration file
 */ 
void preCompute(FILE *fd)
{
	int totalMem, idxElems = 0,
		*memMapS, *memMapE, *dirtyBits, *memOffset,
		*trimArr;
	
	FILE *precData;
	
	const int dbSZp = ((ASCBINSZ << 2) / 8) + sizeof(int);
	
	/*Run through computations grabbing compact versions of the data*/
	idxElems = makeMaps(fd, &memMapS, &memMapE, &dirtyBits, &memOffset, &trimArr, 1);
	fillMaps(fd, memMapS, memMapE, dirtyBits, memOffset, trimArr, 1);
	
	/*In case this becomes variable, unnecessary now*/
	totalMem = sumAtBit(TRIMSZ, TRIMMX);
	totalMem = ((totalMem + (totalMem % (sizeof(int) << 3))) / 8) + sizeof(int);
	
	if((precData = fopen(LAST_PRECMP, "wb")) == NULL)
		fprintf(stderr, "Dependency failure, %s could not open\n", LAST_PRECMP), exit(1);
	
	fwrite(&totalMem, sizeof(int), 1, precData);
	fwrite(&idxElems, sizeof(int), 1, precData);
	
	fwrite(memMapS, sizeof(int), idxElems, precData);
	fwrite(memMapE, sizeof(int), idxElems, precData);
	fwrite(dirtyBits, sizeof(int), dbSZp / sizeof(int), precData);
	fwrite(trimArr, sizeof(int), ASCBINSZ, precData);
	fwrite(memOffset, sizeof(int), TRIMSZ, precData);
	
	fclose(precData);
	
	free(memMapS);
	free(memMapE);
	free(memOffset);
	free(dirtyBits);
	free(trimArr);
}
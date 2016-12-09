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
 *loadPreCompute.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Definitions/bits.h"
#include "../Definitions/defs.h"

/*
 *Loads data from the precompute file
 *
 *@PARAM: The configuration file
 *@PARAM: The exclude section map
 *@PARAM: The exclude key map
 *@PARAM: Character trigger action map
 *@PARAM: Memory offsets into the memmap
 *@PARAM: Offsets to shrink size of memmaps
 */ 
void loadPreCompute(FILE *fd, int **memMapS, int **memMapE,
					int **dirtyBits, int **memOffset, int **trimArr)
{
	int totalMem, idxElems, idxElemsSZ, i,
		*idxElemsArrS, *idxElemsArrE;
	
	FILE *precData;
	
	const int dbSZ = ((UTFBINSZ << 2) / 8) + sizeof(int);
	const int dbSZp = ((ASCBINSZ << 2) / 8) + sizeof(int);
	const int taSZ = UTFBINSZ * sizeof(int);
	
	if((precData = fopen(LAST_PRECMP, "rb")) == NULL)
		fprintf(stderr, "Dependency failure, %s could not open\n", LAST_PRECMP), exit(1);
	
	/*Get sizes that required computation*/
	fread(&totalMem, sizeof(int), 1, precData);
	fread(&idxElems, sizeof(int), 1, precData);
	idxElemsSZ = idxElems * sizeof(int);
	
	/*Assign everything to memory*/
	*memMapS = malloc(totalMem);								/*TRIMMED, must catch in code*/
	*memMapE = malloc(totalMem);								/*TRIMMED, must catch in code*/
	idxElemsArrS = malloc(idxElemsSZ);
	idxElemsArrE = malloc(idxElemsSZ);
	*memOffset = malloc(sizeof(int) * TRIMSZ); 					/*UTF 8 keys could become large...*/
	*dirtyBits = malloc(dbSZ);
	*trimArr = malloc(taSZ);
	
	if(!(*memMapS) || !(*memMapE) || !(*dirtyBits) || !(*memOffset)
		|| !idxElemsArrS || !idxElemsArrE || !(*trimArr))
		fprintf(stderr, "Malloc reports failure\n"), exit(7);
	
	memset(*memMapS, 0, totalMem);
	memset(*memMapE, 0, totalMem);
	memset(*dirtyBits, 0, dbSZ);
	memset(*trimArr, 0, taSZ);
	
	/*Read the primary data*/
	fread(idxElemsArrS, sizeof(int), idxElems, precData);
	fread(idxElemsArrE, sizeof(int), idxElems, precData);
	fread(*dirtyBits, sizeof(int), dbSZp / sizeof(int), precData);
	fread(*trimArr, sizeof(int), ASCBINSZ, precData);
	fread(*memOffset, sizeof(int), TRIMSZ, precData);
	
	/*Mark map using the offset lists*/
	for(i = 0; i < idxElems; i++)
	{
		markMap(idxElemsArrS[i], *memMapS);
		markMap(idxElemsArrE[i], *memMapE);
	}
	
	fclose(precData);
	
	free(idxElemsArrS);
	free(idxElemsArrE);
}
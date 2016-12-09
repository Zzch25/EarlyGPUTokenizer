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
 *makeMaps.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Definitions/defs.h"
#include "../Definitions/bits.h"

/*
 *Generates and fills data from the input file
 *
 *@PARAM: The configuration file
 *@PARAM: The exclude section map
 *@PARAM: The exclude key map
 *@PARAM: Character trigger action map
 *@PARAM: Offsets to shrink size of memmaps
 *@PARAM: Shrink/Modify structures for precompute
 */ 
int makeMaps(FILE *fd, int **memMapS, int **memMapE, int **dirtyBits, 
							int **memOffset, int **trimArr, int prec)
{
	int i = 0, j, totalMem = 0, *iTemp, countMem = 1;
	char temp, *buffer, *btemp,
		 trimCount = 0, canSpace;
		 
	const int dbSZ = ((UTFBINSZ << 2) / 8) + sizeof(int);
	const int dbSZp = ((ASCBINSZ << 2) / 8) + sizeof(int);
	const int taSZ = UTFBINSZ * sizeof(int);
	const int taSZp = ASCBINSZ * sizeof(int);
	
	btemp = buffer = malloc(BUF_SZ * sizeof(char));
	if(!prec)
		*trimArr = malloc(taSZ);
	else
		*trimArr = malloc(taSZp);
	iTemp = *trimArr;
	if(!buffer || iTemp == 0)
		fprintf(stderr, "Malloc reports failure\n"), exit(10);
	if(!prec)
		memset(iTemp, 0, taSZ);
	else
		memset(iTemp, 0, taSZp);
	
	/*Skip two lines - assumes top of file*/
	fgets(btemp,BUF_SZ,fd);
	fgets(btemp,BUF_SZ,fd);
	
	for(j = 3, temp = ' '; j <= 4; j++, i = 0)
	{
		if(fgets(btemp,BUF_SZ,fd) == NULL)
			fprintf(stderr, "Failed to get line %d of CFG\n", j), exit(-4);
		
		while(*btemp != 0)
		{
			temp = *btemp;
			btemp++;
			if(temp != ' ')
			{
				if(temp == '$' && *btemp == '^')
					temp = ' ', btemp++, canSpace = 1;
				if((temp != ' ' || canSpace == 1) && iTemp[(int)temp] == 0)
					iTemp[(int)temp] = ++trimCount;
				i++, canSpace = 0;
			}
			else /*Count the total elements between the memmaps*/
				countMem++;
		}
		/*
		 *UNLIKELY ERROR
		 *
		 *safey for end of buffer characters
		 *:::::::::::::::added after testing
		 */
		countMem++;
	}
	
	if(!prec)
	{
		totalMem = sumAtBit(TRIMSZ, TRIMMX);
		totalMem = ((totalMem + (totalMem % (sizeof(int) << 3))) / 8) + sizeof(int);
	}
	else
		totalMem = sizeof(int) * countMem;
	
	*memMapS = malloc(totalMem);
	*memMapE = malloc(totalMem);
	*memOffset = malloc(sizeof(int) * TRIMSZ);
	if(!prec )
		*dirtyBits = malloc(dbSZ);
	else
		*dirtyBits = malloc(dbSZp);
	if((*memMapS) == 0 || (*memMapE) == 0 || (*dirtyBits) == 0 || (*memOffset) == 0) 
		fprintf(stderr, "Malloc reports failure\n"), exit(7);
	memset(*memMapS, 0, totalMem);
	memset(*memMapE, 0, totalMem);
	memset(*memOffset, 0, sizeof(int) * TRIMSZ);
	if(!prec)
		memset(*dirtyBits, 0, dbSZ);
	else
		memset(*dirtyBits, 0, dbSZp);
	
	rewind(fd);
	free(buffer);

	if(trimCount > TRIMMX)
		fprintf(stderr, "Too many unique characters in CFG\n"), exit(93);
	
	return countMem;
}
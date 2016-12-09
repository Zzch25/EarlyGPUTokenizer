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
 *fillMaps.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Definitions/bits.h"
#include "../Definitions/defs.h"

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
void fillMaps(FILE *fd, int *memMapS, int *memMapE, int *dirtyBits, 
					int *memOffset, int *trimArr, int prec)
{
	int i, j, k = 0, l = 0, idx, spcs, flip = 0;
	char temp, *buffer, *btemp;
	
	btemp = buffer = malloc(BUF_SZ * sizeof(char));
	if(!(buffer))
		fprintf(stderr, "Malloc reports failure\n"), exit(10);
	
	/*Skip two lines - assumes top of file*/
	fgets(btemp,BUF_SZ,fd);
	fgets(btemp,BUF_SZ,fd);
	
	/*Set memory offsets into memmaps*/
	memOffset[0] = 1;
	for(i = 1; i < TRIMSZ; i++)
		memOffset[i] = offsetToBit(i, TRIMMX);
	/*Fill memmaps and trigger map*/
	for(i = 2, j = 0, idx = 0, spcs = 0; ++i <= 5; flip = 0)
	{
		if(fgets(btemp,BUF_SZ,fd) == NULL)
			fprintf(stderr, "Failed to get line %d of CFG\n", i), exit(-5);
		
		/*Process the line*/
		while(*btemp != 0)
		{
			/*Valid character to modify information*/
			if(*btemp != ' ' && *btemp != '\n')
			{
				if(i == 5 && spcs > 0)
					fprintf(stderr, "More than one character on line %d of CFG\n", i), exit(-5);
				
				temp = *btemp;
				
				if(*btemp == '$' && *(btemp + 1) == '^')
					temp = ' ', btemp++;
				
				if(i < 5 && spcs == 0)
					markDB(utfCompress(temp,1), 1, dirtyBits);
				
				idx += (int)trimArr[(int)temp] * (int)memOffset[j++];
				spcs++;
			}
			
			/*eh, just because I can right here*/
			btemp++;
			
			/*End valid characters, mark previous result*/
			if(*btemp < 33 || *btemp > 126)
			{
				if(spcs != 0)
				{
					if(i == 3)
					{
						if(!prec)
							markMap(idx, memMapS);
						else
							memMapS[k++] = idx;
					}
					else if(i == 4)
					{
						if(!flip)
						{
							
							if(!prec)
								markMap(idx, memMapE);
							else
								memMapE[l++] = idx;
						}
						else
							markDB(utfCompress(temp,1), 2, dirtyBits);
					}
					else
						markDB(utfCompress(temp,1), 8, dirtyBits);
					idx = 0;
				}
				spcs = 0, j = 0, flip ^= 1;
			}
		}
	}
	
	/*
	 *POSSIBLE BUG FOR OTHER FILE FORMATS
	 *
	 *Block reading any crap one may find in a file
	 *Does not include UTF8 special characters.. Not
	 *an issue yet
	 */
	for(i = 0; i < 33; i++)
		markDB(utfCompress(i,1), 8, dirtyBits);
	markDB(utfCompress(127,1), 8, dirtyBits);
	/*just a reminder ;)*/
	if(!prec) memMapS[0] = 0;
	if(!prec) memMapE[0] = 0;
	
	rewind(fd);
	free(buffer);
}
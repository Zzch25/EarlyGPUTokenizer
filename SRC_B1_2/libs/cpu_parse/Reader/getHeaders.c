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
 *getHeaders.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Definitions/defs.h"

/*
 *Loads the seek tags
 *
 *@PARAM: The configuration file
 *@PARAM: The seek tags
 */ 
void getHeaders(FILE *fd, struct tags_t **configTags)
{
	int i = 0, j, currTag, spcs = 0;
	char temp, *buffer, *btemp;
	struct tags_t *tagTemp;
	
	tagTemp = malloc(sizeof(struct tags_t));
	btemp = buffer = malloc(BUF_SZ * sizeof(char));
	if(tagTemp == 0 || btemp == 0)
		fprintf(stderr, "Malloc reports failure\n"), exit(8);
	memset(tagTemp, 0, sizeof(struct tags_t));
	
	for(currTag = 0, j = 1, i = 0; j <= 2; j++, i = 0, spcs = 0, currTag = 0)
	{
		if(fgets(btemp,BUF_SZ,fd) == NULL)
			fprintf(stderr, "Failed to get line %d of CFG\n", j), exit(-1);
		
		while(*btemp != 0)
		{
			if(i == CNTAIN_ACT || (currTag >> 1) > ATTAGS_SZ)
				fprintf(stderr, "Too many tags/Tags too large ins line %d of CFG\n", j), exit(-2);
			
			/*Because why not do this here*/
			temp = *btemp, btemp++;
			
			/*Process valid character*/
			if(temp > 31 && temp < 127)
			{
				if(temp == '$' && *btemp == '^')
					temp = ' ', btemp++;
				
				if(j == 1)
					tagTemp->container[0][currTag][i] = temp;
				else
					tagTemp->atTags[(currTag / 2)][(currTag % 2)][i] = temp;
				
				i++, spcs++;
			}
			
			if(currTag == 2 && j == 1)
				fprintf(stderr, "Too many tags\n");
			
			/*If end of valid characters*/
			if(*btemp == ' ' || *btemp == '\n' || *btemp == '\0')
			{
				if(spcs == 0)
					fprintf(stderr, "Extraneous space on line %d of CFG\n", j), exit(-30);
				
				if(j == 1)
					tagTemp->container[0][currTag][CNTAIN_SZ] = i;
				else
					tagTemp->atTags[(currTag / 2)][(currTag % 2)][CNTAIN_SZ] = i;
				
				currTag++, i = 0, spcs = 0, btemp++;
			}
		}
		
		if(currTag == 0)
			fprintf(stderr, "Too few tags in line %d of CFG\n", j), exit(-3);
		
		/*If multiple attack tag sets*/
		if(j == 2)
			tagTemp->attackTagSZ = currTag / 2;
	}
	
	*configTags = tagTemp;
	free(buffer);
}
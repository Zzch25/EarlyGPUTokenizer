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
 *cfgInit.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "../Definitions/defs.h"

#include "localDef.h"

/*
 *Calls the desired option, a wrapper
 *
 *@PARAM: The configuration file name
 *@PARAM: The exclude section map
 *@PARAM: The exclude key map
 *@PARAM: Character trigger action map
 *@PARAM: The selected option eg precompute
 *@PARAM: The tags to seek
 *@PARAM: Offsets to shrink size of memmaps
 */ 
void cfgInit(char *cfg, int **memMapS, int **memMapE, int **dirtyBits, 
			int **memOffset, int optCmp, struct tags_t **configTags, int **trimArr)
{
	FILE *configIn;
	/*Initialise requested structures*/
	if((configIn = fopen(cfg,"r")) == NULL)
		fprintf(stderr, "Dependency failure, %s could not open\n", cfg), exit(1);
	if(optCmp == 0)
	{
		/*Setup maps*/
		makeMaps(configIn, memMapS, memMapE, dirtyBits, memOffset, trimArr, 0);
		fillMaps(configIn, *memMapS, *memMapE, *dirtyBits, *memOffset, *trimArr, 0);
	}
	else if(optCmp == 1) /*Compute and exit*/
		preCompute(configIn);
	else if(optCmp == 2) /*Load last pre-compute*/
		loadPreCompute(configIn, memMapS, memMapE, dirtyBits, memOffset, trimArr); 
	/*Load the page headers*/
	getHeaders(configIn, configTags);
	
	fclose(configIn);
}
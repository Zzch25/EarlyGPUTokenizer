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
 *initThreadData.c
 *4/15/2015
 *
 *Runs parser and reads to the buffer
 */

#include <stdio.h>
#include "../Definitions/defs.h"

/*
 *Initialises a threads data
 *
 *@PARAM: The file to parse
 *@PARAM: The pages to process
 *@PARAM: The recursion match map
 *@PARAM: The key match map
 *@PARAM: The trigger map
 *@PARAM: The offset into the map given an index
 *@PARAM: The size compaction  reference array
 *@PARAM: The tags to seek
 */ 
void initThreadData(int *memMapSin, int *memMapEin, int *dirtyBitsin,
					int *memOffsetin, int *trimArrin, struct tags_t *configTagsin,
					char **fileBuffer, char *data)
{	
	//Initialise the parse information
	initParse(memMapSin, memMapEin, dirtyBitsin, memOffsetin, trimArrin);
	//Return file contents in a buffer
	dataInit(fileBuffer, data);
	//Initialise the target tags
	initTags(*fileBuffer, configTagsin);
}
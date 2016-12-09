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
 *initThreadData.c
 *4/15/2015
 *
 *Runs parser and reads to the buffer
 */

#include "../Definitions/defs.h"

/*
 *Initialises a threads data
 *
 *@PARAM: The recursion match map
 *@PARAM: The key match map
 *@PARAM: The trigger map
 *@PARAM: The offset into the map given an index
 *@PARAM: The size compaction  reference array
 *@PARAM: The tags to seek
 *@PARAM: The file buffer
 *@PARAM: The name of the data file
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
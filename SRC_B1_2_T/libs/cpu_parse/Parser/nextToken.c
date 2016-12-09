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
 *nextToken.c
 *4/15/2015
 */

#include "localDef.h"

#include "../../../settings.h"

/*
 *WHY on earth global? Well
 *
 *A)
 *reworking the cpu lib would be needed to eliminate difficulties
 *
 *B)
 *less overhead in some respects given the way the code
 *has been organised for readability.
 *
 *C)
 *Depending on the compiler I've seen static overhead... Not sure
 *if it would be optimized away here.. Also would require an if block.
 */
////////////////////////////////////////////////////////////////
char
				*fileBuffer;
struct tags_t 	
				*tags;
////////////////////////////////////////////////////////////////
//READ ABOVE AS TO WHY I DECIDED ON GLOBALS V STATICS V ANYTHING

/*
 *Initialises a threads data
 *
 *@PARAM: The file buffer
 *@PARAM: The tags to seek
 */ 
void initTags(char *file, struct tags_t *configTagsin)
{	
	tags = configTagsin;
	fileBuffer = file;
}

/*
 *Finds the container tag for the page into the threads offset
 *
 *@PARAM: The threads buffer index
 *@PARAM: The thread ID
 *@RETURN: Status
 */
int seekThreadOffs(long *readIdx, int TID)
{
	int it = 0, isIT, debTID;
	int debug;
	
	debTID = TID;

	/*go until match TID times*/
	while(TID > 0 && debug != -1)
	{
		isIT = (int)(it <  tags->container[0][0][CNTAIN_SZ]);
		it *= isIT;
		TID -= !isIT;
		
		if((debug = (int)fileBuffer[(*readIdx)++]) == tags->container[0][0][it])
			it++;
		else it = 0;
	}
	if(debug == -1)
		return 0;
	return 2;
}

/*
 *Issues the functions necessary to find a token
 *
 *@PARAM: The threads buffer index
 *@PARAM: The previous status
 *@PARAM: The pages left for the thread
 *@PARAM: The repetition bit
 *@PARAM: The key bit
 *@PARAM: The last byte
 *@PARAM: The last key (token)
 *@RETURN: Status of seek, which action comes next
 */
int nextToken(long *readIdx, int laststate, int *pages, 
				int *wrTOP, int *wrLOW, int *last, int *lToken)
{
	int numTag = 0;
	
	for(; numTag < tags->attackTagSZ && laststate > 0; numTag++)
	{
		// Look for an outer tag
		if(laststate == 1)
		{
			*pages -= HST_THRD;
			if((laststate *= (int)(pages > 0)))
				laststate = seekOuterTag(fileBuffer, readIdx, tags->container[0][0]);
		}
		// Look for an inner tag 
		if(laststate == 2)
			laststate = seekInnerTag(fileBuffer, readIdx, tags->atTags[numTag][0]);
		// Get a token or boundary indicator
		if(laststate == 3)
			laststate = parseContent(fileBuffer, readIdx, '<', wrTOP, wrLOW, last, lToken); /*FIX THIS*/
	}
	return laststate;
}
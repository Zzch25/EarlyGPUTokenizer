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
 *@PARAM: The file to parse
 *@PARAM: The tag to search for
 *@PARAM: The thread ID
 *@RETURN: Status
 */
void seekThreadOffs(long *readIdx, int TID, int *laststate)
{
	int it = 0, isIT, debTID;
	int debug, tempCompute;
	
	debTID = TID;

	/*Expects to find tags - getc handled in UTF func*/
	while(TID > 0 && debug != -1)
	{
		isIT = (int)(it <  tags->container[0][0][CNTAIN_SZ]);
		it *= isIT;
		TID -= !isIT;
		
		tempCompute = (int)((debug = (int)fileBuffer[(*readIdx)++]) == tags->container[0][0][it]);
		it += tempCompute;
		it *= tempCompute;
	}
	
	if(debug != -1)
		*laststate = 2;
	else
		*laststate = 0;
}

/*
 *Issues the functions necessary to find a token
 *
 *@PARAM: The file to parse
 *@PARAM: The buffer
 *@PARAM: The final buffer index
 *@PARAM: Trim to keep memmaps down on size
 *@PARAM: The previous status
 *@PARAM: The exclude section map
 *@PARAM: The exclude key map
 *@PARAM: The offset into either map at an index
 *@PARAM: Character trigger action map
 *@PARAM: Outer and inner tags
 *@PARAM: Pages - 1 to process ~ who cares, pages - 1
			is for pages of fun!
 *@RETURN: Status of seek, which action comes next
 */
void nextToken(long *readIdx, int *laststate, int *pages, 
				int *wrTOP, int *wrLOW, int *last, int *lToken)
{
	int numTag = 0;
	
	for(; numTag < tags->attackTagSZ && *laststate > 0; numTag++)
	{
		if(*laststate == 1)
		{
			*pages -= HST_THRD;
			if((*laststate *= (int)(pages > 0)))
				seekOuterTag(fileBuffer, readIdx, tags->container[0][0], laststate);
		}	
		if(*laststate == 2)
			seekInnerTag(fileBuffer, readIdx, tags->atTags[numTag][0], laststate);
		if(*laststate == 3)
			parseContent(fileBuffer, readIdx, '<', wrTOP, wrLOW, last, lToken, laststate); /*FIX THIS*/
	}
}
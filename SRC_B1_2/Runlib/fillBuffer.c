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
 *fillBuffer.c
 *4/15/2015
 *
 *Runs parser and reads to the buffer
 */

#include <stdio.h>
#include <string.h>

#include "../Definitions/defs.h"
#include "../Definitions/cpuCall.h"

#include "../settings.h"

/*
 *Fills the buffer with as many tokens as permits
 *
 *@PARAM: The buffer to fill
 *@PARAM: The thread calling, for multi-threading
 *@PARAM: The last state on the file read
 *@PARAM: The pages traversed
 *@PARAM: Exclusion flip bit
 *@PARAM: Key flip bit
 *@PARAM: The last char in the parse
 *@PARAM: The index in the file buffer of this thread
 */ 
int fillBuffer(int *bufferIn, int TID, int *laststate, int *pages,
				int *wrTOP, int *wrLOW, int *last, long *readIdx)
{
	int totalWritten = 0, lToken;

	// Fill keys to the buffer max
	while(*laststate > 0 && *pages > 0 && totalWritten < FEED_BY)
	{
		// Grab a token or end boundary indicator
		*laststate = nextToken(readIdx, *laststate, pages, wrTOP, wrLOW, last, &lToken);
		
		// If not boundary indicator
		if(lToken > 0)
		{
			// Add the key to the buffer
			bufferIn[totalWritten++] = lToken;
		}
	}
	
	if(*laststate > 0 && *pages > 0)
		return 1;
	return 0;
}
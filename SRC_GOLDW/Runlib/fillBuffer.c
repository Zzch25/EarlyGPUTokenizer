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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../Definitions/defs.h"
#include "../Definitions/cpuCall.h"

#include "../settings.h"

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
 *@PARAM: The file to parse
 */ 
void fillBuffer(int *bufferIn, int TID, int *laststate, int *pages,
				int *wrTOP, int *wrLOW, int *last, long *readIdx, int *reading)
{
	int 	totalWritten = 0,lToken;

	while(*laststate > 0 && *pages > 0 && totalWritten < FDBY_HF)
	{
		nextToken(readIdx, laststate, pages, wrTOP, wrLOW, last, &lToken);

		//NOT AT ALL EFFICIENT.. TESTING INDEX SYSTEM BLUNTLY
		if(lToken > 0)
		{
			lToken += (lToken << 12);
			lToken ^= (lToken >> 22);
			lToken += (lToken << 4);
			lToken ^= (lToken >> 9);
			lToken += (lToken << 10);
			lToken ^= (lToken >> 2);
			lToken += (lToken << 7);
			lToken ^= (lToken >> 12);
		
			pthread_mutex_lock(&lock);
			bufferIn[lToken & LANG_BITS_MSK] += 1;
			pthread_mutex_unlock(&lock);
		}
	}
	
	if(*laststate > 0 && *pages > 0)
		*reading = 1;
	else
		*reading = 0;
}
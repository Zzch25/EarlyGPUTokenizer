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
 *fBThreadMgr.cu
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "../Definitions/defs.h"
#include "../Definitions/cpuCall.h"

#include "../settings.h"

#include "localDef.h"

/*
 * Launches a thread to run the parser and launch kernels
 * in parallel
 * 
 * @PARAM: The arguments
 */
void *fBThreadMgr(void *arg)
{
	int		
		reading, laststate, TID, pages,
		
		*histogram, 
	
		wrTOP = 1, wrLOW = 1, last = 0,
		feedBytes;
	long
		readIdx = 0;
		
	TID = (int)((long *)arg)[0];
	histogram = (int *)(long)((long *)arg)[1];
	pages = (int)((long *)arg)[2] - TID;
	TID += 1;
	
	//Move to the correct thread offset in the file
	if(pages > 0)
		seekThreadOffs(&readIdx, TID, &laststate);
	reading = (int)(laststate > 0);
 
	// Launch the device computation threads!
	while(reading)
	{
		fillBuffer(histogram, TID, &laststate, &pages, 
					&wrTOP, &wrLOW, &last, &readIdx, &reading);
	}
	
	free(arg);

	return NULL;
}
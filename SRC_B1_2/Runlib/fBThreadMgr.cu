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

#include "../Definitions/cpuCall.h"
#include "../Definitions/gpuCall.h"

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
		
		*feedbufferA, *feedbufferB,
		*histogram,
	
		wrTOP = 1, wrLOW = 1, last = 0,
		feedBytes;
	long
		readIdx = 0;
		
	cudaStream_t stream;
	
	// Get back pointers and values - Long for said pointers
	TID = (int)((long *)arg)[0];
	histogram = (int *)(long)((long *)arg)[1];
	pages = (int)((long *)arg)[2] - TID;
	feedBytes = sizeof(int) * FEED_BY;
	TID += 1;
	
	//enable 
	cudaSetDevice(0);
	//set for parrallel work
	cudaStreamCreate(&stream);
	
	//Allocate the feed buffers
	//feedbuffer = (int *)malloc(feedBytes);
	cudaMallocHost((void**)&feedbufferA, feedBytes);
	cudaMallocHost((void**)&feedbufferB, feedBytes);

	// if GPU memory allocation failed, report an error message
	if(!feedbufferA || !feedbufferB)
		fprintf(stderr, "CudaMalloc thread reports failure\n"), exit(-7);
	
	// Setup the execution configuration
	dim3 threads(THREADS_PER_BUFF, 1, 1);
	dim3 blocks(FEED_BUFFERS, 1, 1);
	
	//Move to the correct thread offset in the file
	if(pages > 0)
		laststate = seekThreadOffs(&readIdx, TID);
	reading = (int)(laststate > 0);
 
	// CPU batching with co-processing
	while(reading)
	{
		reading = fillBuffer(feedbufferA, TID, &laststate, &pages, 
								&wrTOP, &wrLOW, &last, &readIdx);
		cudaStreamSynchronize(stream);
		wattoken_kernel<<<blocks,threads,0,stream>>>(feedbufferA, histogram);
		
		if(reading)
		{
			reading = fillBuffer(feedbufferB, TID, &laststate, &pages, 
									&wrTOP, &wrLOW, &last, &readIdx);
			cudaStreamSynchronize(stream);
			wattoken_kernel<<<blocks,threads,0,stream>>>(feedbufferB, histogram);
		}
	}
 	
	//End work, clean
	cudaStreamDestroy(stream);
	cudaFreeHost(feedbufferB);
	cudaFreeHost(feedbufferA);	
	free(arg);

	return NULL;
}
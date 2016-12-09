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
#include <sys/time.h>

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
	
		wrTOP = 1, wrLOW = 1, last = 0;
	long
		readIdx = 0;
	float 
		ms = 0;
		
	FILE
		*gpuTime, *cpuTime;
		
	struct timeval
		startCPU, endCPU;
		
	cudaStream_t 
		stream;
		
	cudaEvent_t 
		startGPU, stopGPU;

	// Get back pointers and values - Long for said pointers
	TID = (int)((long *)arg)[0];
	histogram = (int *)(long)((long *)arg)[1];
	pages = (int)((long *)arg)[2] - TID;
	feedbufferA = (int *)(long)((long *)arg)[3];
	feedbufferB = (int *)(long)((long *)arg)[4];
	TID += 1;
	
	//enable 
	cudaSetDevice(0);
	//set for parrallel work
	cudaStreamCreate(&stream);
	cudaEventCreate(&startGPU);
	cudaEventCreate(&stopGPU);
	
	//Allocate the feed buffers
	//feedbuffer = (int *)malloc(feedBytes);
	//cudaMallocHost((void**)&feedbufferA, feedBytes);
	//cudaMallocHost((void**)&feedbufferB, feedBytes);

	// if GPU memory allocation failed, report an error message
	//if(!feedbufferA || !feedbufferB)
	//	fprintf(stderr, "CudaMalloc thread reports failure\n"), exit(-7);
	
	// Setup the execution configuration
	dim3 threads(THREADS_PER_BUFF, 1, 1);
	dim3 blocks(FEED_BUFFERS, 1, 1);
	
	if((cpuTime = fopen(CPU_TIME,"w")) == NULL)
		fprintf(stderr, "Dependency failure, %s could not open\n", CPU_TIME), exit(24);
	if((gpuTime = fopen(GPU_TIME,"w")) == NULL)
		fprintf(stderr, "Dependency failure, %s could not open\n", GPU_TIME), exit(24);
	
	//Move to the correct thread offset in the file
	if(pages > 0)
		laststate = seekThreadOffs(&readIdx, TID);
	reading = (int)(laststate > 0);
 
	// CPU batching with co-processing
	while(reading)
	{
		gettimeofday(&startCPU, NULL);
		reading = fillBuffer(feedbufferA, TID, &laststate, &pages, 
								&wrTOP, &wrLOW, &last, &readIdx);
		gettimeofday(&endCPU, NULL);
		fprintf(cpuTime, "%d\n",
		(int)((endCPU.tv_sec * 1000000 + endCPU.tv_usec) -
		(startCPU.tv_sec * 1000000 + startCPU.tv_usec)));
		
		cudaStreamSynchronize(stream);
		cudaEventRecord(startGPU);
		wattoken_kernel<<<blocks,threads,0,stream>>>(feedbufferA, histogram);
		cudaEventRecord(stopGPU);
		cudaEventSynchronize(stopGPU);
		cudaEventElapsedTime(&ms, startGPU, stopGPU);
		fprintf(gpuTime, "%f\n", ms);
		ms = 0;
		if(reading)
		{
			gettimeofday(&startCPU, NULL);
			reading = fillBuffer(feedbufferB, TID, &laststate, &pages, 
									&wrTOP, &wrLOW, &last, &readIdx);
			gettimeofday(&endCPU, NULL);
			fprintf(cpuTime, "%d\n",
			(int)((endCPU.tv_sec * 1000000 + endCPU.tv_usec) -
			(startCPU.tv_sec * 1000000 + startCPU.tv_usec)));
			
			cudaStreamSynchronize(stream);
			cudaEventRecord(startGPU);
			wattoken_kernel<<<blocks,threads,0,stream>>>(feedbufferB, histogram);
			cudaEventRecord(stopGPU);
			cudaEventSynchronize(stopGPU);
			cudaEventElapsedTime(&ms, startGPU, stopGPU);
			fprintf(gpuTime, "%f\n", ms);
			ms = 0;
		}
	}
 	
	//End work, clean
	cudaStreamDestroy(stream);
	//cudaFreeHost(feedbufferB);
	//cudaFreeHost(feedbufferA);
	fclose(cpuTime);
	fclose(gpuTime);
	free(arg);

	return NULL;
}
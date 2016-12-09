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
 *waterfall_tokenize.cu
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#include "Definitions/defs.h"
#include "Definitions/cpuCall.h"

#include "settings.h"

/*
 * Executes a tokenize system using the exclusion parser
 *
 * Designed to coalesce and avoid all control statements.
 * This allows for lots of math tweaks to have a 0 divergence
 * kernel.
 *
 *
 * U S I N G   A   N A I V E   F I L E   B U F F E R... will be fixed
 */
int main(int argc, char **argv)
{
	
	//V A R I A B L E S//////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	char
		*data, *cfg,
		
		//THIS IS A SUPER NAIVE FIX, CHUNKS ARE NEEDED
		*fileBuffer;
	
	int
		histBytes,
		optCmp, pages,
		
		*histogram,
		
		hostThreads,
				
		*memMapS, *dirtyBits, *memOffset,
		*memMapE, *trimArr;
		
	//Debug
	int
		i;
		
	long
		*arg;
		
	struct tags_t
		*configTags;
		
	pthread_t
		threads[HST_THRD];
		
	struct timeval
		start, end;
		
	FILE
		*output;
	
	//C O N F I G U R A T I O N   A N D   I N S T A T A N T I A T I O N//////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
  
	if(argc < 5)
	{
		fprintf(stderr, "usage: ./program_name\nREQUIRED ARGS\n"
			   "FILE 1: Search configuration [search.cfg]\n"
			   "FILE 2: The tagged data to process [someData.type]\n"
			   "3: Task compute configuration, pre-compute,\n"
			   "or run with key pre-computation\n"
			   "filter [0:2]\n"
			   "EX) im_the_settings.cfg im_the_big_data.type 0\n"
			   "4: Container tags to be processed in a document EG 7000\n");
		return -1;
	}
	
	optCmp = atoi(argv[3]);
	pages = atoi(argv[4]);
	data = strdup(argv[2]);
	cfg = strdup(argv[1]);
	
	if(optCmp < 0 || optCmp > 2)
	{
		fprintf(stderr,"Invalid compute option, run ./program_name for usage\n");
		return 2;
	}
	if(pages < 1)
	{
		fprintf(stderr,"Insufficient page count, run ./program_name for usage\n");
		return 3;
	}
	
	hostThreads = HST_THRD;
	
	histogram = calloc(sizeof(int), HIST_SZ);

	// init the parser variables
	cfgInit(cfg, &memMapS, &memMapE, &dirtyBits, 
		&memOffset, optCmp, &configTags, &trimArr);
	
	// Enter if not precompute
	if(optCmp != 1)
	{
	
	//H O S T   T H R E A D I N G////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
		// Open the file to be parsed
		initThreadData(memMapS, memMapE, dirtyBits, memOffset,
						trimArr, configTags, &fileBuffer, data);
	
		fprintf(stderr, "Start CPU Compute\n");
		
		gettimeofday(&start, NULL);
	
		while(hostThreads > 0)
		{
			arg = (long *)malloc(sizeof(long *) * 3);
			if(!arg)
				fprintf(stderr, "Malloc reports failure\n"), exit(-7);
			
			arg[0] = (long)(HST_THRD - hostThreads);
			arg[1] = (long)histogram;
			arg[2] = (long)pages;
			
			if(pthread_create(&threads[--hostThreads],NULL,fBThreadMgr,(void *)arg) != 0)
				fprintf(stderr, "Host thread could not start\n"), exit(1);
		}
		
		hostThreads = HST_THRD;
		
		// Post join the threads for a clean death, a soldiers death
		while(hostThreads > 0)
		{
			if(pthread_join(threads[--hostThreads],NULL) != 0)
				fprintf(stderr, "Host thread could not join\n"), exit(1);
		}
		
		gettimeofday(&end, NULL);
		
		fprintf(stderr, "TIME(us)::%ld\nEnd CPU Compute\n\n", 
												(end.tv_sec * 1000000 + end.tv_usec) -
												(start.tv_sec * 1000000 + start.tv_usec));
												
		if((output = fopen(OUTPUT_HIST,"w")) == NULL)
			fprintf(stderr, "Dependency failure, %s could not open\n", OUTPUT_HIST), exit(24);
	
		for(i = 0; i < HIST_SZ; i++)
			if(histogram[i] != 0) fprintf(output, "%d,%d\n", i, histogram[i]);
		
	//C L E A N U P//////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
		
		free(memMapS);
		free(memMapE);
		free(memOffset);
		free(dirtyBits);
		free(configTags);
		free(trimArr);
		free(data);
		free(cfg);
		free(fileBuffer);
	}
	free(histogram);
	
	return 0;
}
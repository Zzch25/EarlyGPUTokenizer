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
 *wattoken_kernel.cu
 *4/15/2015
 *
 *This is designed for a minimum architecture of the
 *GK104 specifications (specifically the k520). 
 *Otherwise go away.
 *
 *Tokenize the input buffer and update the global histogram
 */

#include <stdio.h> 

#include "../Definitions/defs.h"

#include "../../../settings.h"

/*
 *
 * The most intense unrolling you may ever see, behold...
 *
 * The goal is to utilize EVERY drop of memory that is lbuffer
 * >global to regs
 * >process regs and move to shared
 * >waterfall process
 *		>parallel reduction where their content will only survive if they are the first
 *			occurrence of a key from left to right in the buffer
 * EXAMPLE... REMEMBER, high memory volume is required for effectiveness unlike here
 *	buffer of 8x4 w/ 16 threads
							
						ENTIRE SH MEM IS PROCESSED
						******** 24 threads check 24 elements each and modify their count
						********
						********
						********
						
						EX
						*123****->	*123****
						********->	********
						********->	********
						******15->	******05
						
 * >write shared to global with all threads, also using the SH 0 bucket to recklessly
 *	remove contention because life in the fast lane has caused me to loose my mind 
 *
 */
__global__  void wattoken_kernel(int *bufferLoc, int *cpuHist)
{
	__shared__	int shmem[FEED_BY];	//The waterfall buffer where computed hashes are stored
	
	char							//iterators and offset chunk into shared
			trip;				    //the buffer flush indicator and the waterfall trip
	int		
			idxL, j, i,
			shchnk, count;							//hold the result of a hash
			//shoffs;
	
	/*
	idxL = threadIdx.x * FDBY_LD;
	
	//Fetch everything to memory in every other position
	for(i = idxL; i < idxL + FDBY_LD; i++)
		shmem[i] = bufferLoc[i];
	
	__syncthreads();
	*/
	
	//unrolled - see above for comments
	
	idxL = threadIdx.x * FDBY_LD;
	
	shmem[idxL] = bufferLoc[idxL];
	shmem[idxL + 1] = bufferLoc[idxL + 1];
	shmem[idxL + 2] = bufferLoc[idxL + 2];
	shmem[idxL + 3] = bufferLoc[idxL + 3];
	shmem[idxL + 4] = bufferLoc[idxL + 4];
	shmem[idxL + 5] = bufferLoc[idxL + 5];
	shmem[idxL + 6] = bufferLoc[idxL + 6];
	shmem[idxL + 7] = bufferLoc[idxL + 7];
	shmem[idxL + 8] = bufferLoc[idxL + 8];
	shmem[idxL + 9] = bufferLoc[idxL + 9];
	shmem[idxL + 10] = bufferLoc[idxL + 10];
	shmem[idxL + 11] = bufferLoc[idxL + 11];
	shmem[idxL + 12] = bufferLoc[idxL + 12];
	shmem[idxL + 13] = bufferLoc[idxL + 13];
	shmem[idxL + 14] = bufferLoc[idxL + 14]; 
	shmem[idxL + 15] = bufferLoc[idxL + 15];
	shmem[idxL + 16] = bufferLoc[idxL + 16];
	shmem[idxL + 17] = bufferLoc[idxL + 17];
	shmem[idxL + 18] = bufferLoc[idxL + 18];
	shmem[idxL + 19] = bufferLoc[idxL + 19];
	shmem[idxL + 20] = bufferLoc[idxL + 20];
	shmem[idxL + 21] = bufferLoc[idxL + 21];
	shmem[idxL + 22] = bufferLoc[idxL + 22];
	shmem[idxL + 23] = bufferLoc[idxL + 23];
	shmem[idxL + 24] = bufferLoc[idxL + 24]; 
	shmem[idxL + 25] = bufferLoc[idxL + 25]; 
	shmem[idxL + 26] = bufferLoc[idxL + 26]; 
	shmem[idxL + 27] = bufferLoc[idxL + 27]; 
	shmem[idxL + 28] = bufferLoc[idxL + 28]; 
	shmem[idxL + 29] = bufferLoc[idxL + 29]; 
	shmem[idxL + 30] = bufferLoc[idxL + 30]; 
	shmem[idxL + 31] = bufferLoc[idxL + 31]; 
	shmem[idxL + 32] = bufferLoc[idxL + 32]; 
	shmem[idxL + 33] = bufferLoc[idxL + 33]; 
	shmem[idxL + 34] = bufferLoc[idxL + 34]; 
	shmem[idxL + 35] = bufferLoc[idxL + 35]; 
	shmem[idxL + 36] = bufferLoc[idxL + 36]; 
	shmem[idxL + 37] = bufferLoc[idxL + 37]; 
	shmem[idxL + 38] = bufferLoc[idxL + 38]; 
	shmem[idxL + 39] = bufferLoc[idxL + 39]; 
	shmem[idxL + 40] = bufferLoc[idxL + 40]; 
	shmem[idxL + 41] = bufferLoc[idxL + 41]; 
	shmem[idxL + 42] = bufferLoc[idxL + 42]; 
	shmem[idxL + 43] = bufferLoc[idxL + 43]; 
	shmem[idxL + 44] = bufferLoc[idxL + 44]; 
	shmem[idxL + 45] = bufferLoc[idxL + 45]; 
	shmem[idxL + 46] = bufferLoc[idxL + 46]; 
	shmem[idxL + 47] = bufferLoc[idxL + 47];
	
	__syncthreads();
	
	
	for(shchnk = 0, trip = 1; shchnk < FEED_BY; shchnk += THREADS_PER_BUFF, trip = 1)
	{
		//index
		idxL = threadIdx.x + shchnk;
		//get value of index
		j = shmem[idxL];
		//occurrences
		count = 0;
		
		//Warp target code. Each element in the shared buffer is checked
		//per thread with this calculation. Instances are shifted
		//backwards as the priority potential lessens as you go
		//down the buffer. Waterfall reduction is my best analogy
		//via zeroing greater indexes of already existing values
		//
		//  SEE FUNCTION COMMENT EXAMPLE 
		//

		for(i = shchnk, trip = 1; i < THREADS_PER_BUFF + shchnk; i++)
			count += (int)(shmem[i] == j), trip &= (char)((!((i < idxL) && shmem[i] == j)) || (i >= idxL));
		
		//Flush if bad result
		j *= trip;
		count *= trip;
		
		//Now the tricky part. Multiple blocks will be running. Four
		//is the intended amount for the k520. Testing must be performed
		//I may end up using only four feeders and have four cpu threads
		//process the results into the main hashmap. However, this is
		//a single test kernel. Once integrated these concerns will be
		//dealt with
		
		//The home histogram 0 position will be recklessly written. That
		//is the point. This is meant to allow concurrency.
		//
		//cannot use j, could have been flushed
		
		// Hash
		j += (j << 12);
		j ^= (j >> 22);
		j += (j << 4);
		j ^= (j >> 9);
		j += (j << 10);
		j ^= (j >> 2);
		j += (j << 7);
		j ^= (j >> 12);
		j &= LANG_BITS_MSK;
		
		if(trip == 1)
			atomicAdd(&cpuHist[j], count);
	}
	
	//unrolled - see above for comments
	/*
	for(shchnk = 0, trip = 1; shchnk < FEED_BY; shchnk += THREADS_PER_BUFF, trip = 1)
	{
		idxL = threadIdx.x + shchnk;
		j = shmem[idxL];
		count = 0;
		
		//0
		count += (int)(shmem[shchnk] == j), trip &= (char)((!((shchnk < idxL) && shmem[shchnk] == j)) || shchnk >= idxL);
		shoffs = 1 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 2 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 3 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 4 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 5 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 6 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 7 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 8 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 9 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 10 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 11 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 12 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 13 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 14 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 15 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 16 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 17 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 18 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 19 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 20 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 21 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 22 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 23 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 24 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 25 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 26 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 27 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 28 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 29 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 30 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 31 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 32 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 33 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 34 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 35 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 36 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 37 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 38 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 39 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 40 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 41 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 42 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 43 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 44 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 45 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 46 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 47 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 48 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 49 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 50 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 51 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 52 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 53 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 54 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 55 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 56 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 57 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 58 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 59 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 60 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 61 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 62 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 63 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 64 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 65 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 66 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 67 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 68 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 69 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 70 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 71 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 72 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 73 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 74 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 75 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 76 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 77 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 78 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 79 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 80 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 81 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 82 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 83 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 84 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 85 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 86 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 87 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 88 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 89 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 90 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 91 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 92 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 93 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 94 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 95 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 96 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 97 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 98 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 99 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 100 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 101 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 102 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 103 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 104 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 105 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 106 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 107 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 108 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 109 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 110 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 111 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 112 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 113 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 114 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 115 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 116 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 117 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 118 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 119 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 120 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 121 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 122 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 123 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 124 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 125 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 126 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 127 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 128 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 129 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 130 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 131 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 132 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 133 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 134 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 135 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 136 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 137 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 138 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 139 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 140 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 141 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 142 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 143 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 144 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 145 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 146 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 147 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 148 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 149 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 150 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 151 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 152 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 153 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 154 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 155 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 156 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 157 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 158 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 159 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 160 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 161 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 162 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 163 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 164 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 165 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 166 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 167 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 168 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 169 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 170 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 171 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 172 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 173 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 174 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 175 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 176 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 177 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 178 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 179 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 180 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 181 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 182 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 183 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 184 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 185 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 186 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 187 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 188 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 189 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 190 + shchnk;
		count += (int)(shmem[shoffs] == j), trip &= (char)((!((shoffs < idxL) && shmem[shoffs] == j)) || shoffs >= idxL);
		shoffs = 191 + shchnk;
		count += (int)(shmem[shoffs] == j);
		
		// Hash
		j += (j << 12);
		j ^= (j >> 22);
		j += (j << 4);
		j ^= (j >> 9);
		j += (j << 10);
		j ^= (j >> 2);
		j += (j << 7);
		j ^= (j >> 12);
		j &= LANG_BITS_MSK;
		
		j *= trip;
		count *= trip;
		
		if(trip == 1)
			atomicAdd(&cpuHist[j], count);
	}
	*/
	//end unroll - see above for comments
}
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
 *defs.h
 *4/15/2015
 *
 *This is designed for a minimum architecture of the
 *GK104 specifications. Otherwise go away, buy a better
 *graphics card.
 *
 *General definitions
 *
 *NOTE COULD DO 32-34 at a time at curr amount
 */
 
#ifndef _DEFS_GPU_H_
#define _DEFS_GPU_H_

//EXCEPT FEED BUFFS AND MASKS MUST BE MULT OF 2!!!

#define THREADS_PER_BUFF 	192			//Temporary.. pending use of bound threads
#define FEED_BUFFERS		1			//How many to utilize 1+ host threads

//NOTE 12288 ints possible in SHMEM
//#define SHMEM_HIST_WR		9216		//equal to possible words + counts from all threads

#endif
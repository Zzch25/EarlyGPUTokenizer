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
 *markMap.c
 *4/15/2015
 */

/*
 *Mark the index with a 1 to indicate it exists
 *
 *@PARAM: Calculated index
 *@PARAM: memory map
 */
void markMap(int id, int *memMap)
{
	memMap[(id / (sizeof(int) << 3)) + 1] |= (1 << (id % (sizeof(int) << 3)));
}
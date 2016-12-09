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
 *readMap.c
 *4/15/2015
 *
 *performs manipulations to read the computed
 */

/*
 *bitmap set
 *
 *@PARAM: Calculated index into memmaps
 *@PARAM: memory map
 *@RETURN: If the bit exists
 */
int readMap(int id, int *memMap)
{
	return (memMap[(id / (sizeof(int) << 3)) + 1] >> (id % (sizeof(int) << 3))) & 1;
}
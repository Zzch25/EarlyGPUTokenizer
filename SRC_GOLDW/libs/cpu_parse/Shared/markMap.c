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
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
 *sumAtBit.c
 *4/15/2015
 */

/*
 *Grab the size of
 *a chunk into the memory map
 *
 *SUM(i) 0 TO LargestSZ: PRODS(j) 0 TO i trimIDX^j
 *
 *@PARAM: Largest amount of chars
 *@PARAM: Amount of characters possible
 *@RETURN: The offset
 */
int sumAtBit(int largestSZ, int trimIdx)
{
	int i, j,
	    res, temp = trimIdx;
	
	for(res = 0, i = 0; i < largestSZ; i++)
	{
		for(temp = trimIdx, j = 0; j < i; j++)
			temp *= trimIdx;
		res += temp;
	}
	
	return res;
}
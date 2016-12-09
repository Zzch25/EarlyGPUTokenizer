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
 *offsetToBit.c
 *4/15/2015
 */

/*
 *Grab the offset into a bit
 *
 *PRODS(j) 0 TO LIM trimIDX^j
 *
 *@PARAM: Largest amount of chars
 *@PARAM: Amount of characters possible
 *@RETURN: The offset
 */
int offsetToBit(int largestSZ, int trimIdx)
{
	int i, j,
	    res = trimIdx;
	
	for(res = 0, i = 0; i < largestSZ; i++)
	{
		for(res = trimIdx, j = 0; j < i; j++)
			res *= trimIdx;
	}
	
	return res;
}
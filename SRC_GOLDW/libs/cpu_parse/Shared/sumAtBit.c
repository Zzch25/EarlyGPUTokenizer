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
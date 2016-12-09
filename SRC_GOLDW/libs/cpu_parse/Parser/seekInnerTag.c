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
 *seekInnerTag.c
 *4/15/2015
 */

#include <stdio.h>

#include "../Definitions/defs.h"

/*
 *Finds the attack tags
 *
 *@PARAM: The file to parse
 *@PARAM: The tags to search for
 *@PARAM: The desired of the inner tags
 *@RETURN: Status
 */
void seekInnerTag(char *fileBuffer, long *readIdx, int *tags, int *laststate)
{
	int it = 0,
		debug, tempCompute;
	
	/*Expects to find tags - getc handled in UTF func*/
	while(it < tags[CNTAIN_SZ] && debug != -1)
	{
		tempCompute = (int)((debug = (int)fileBuffer[(*readIdx)++]) == tags[it]);
		it += tempCompute;
		it *= tempCompute;
	}
	if(debug != -1)
		*laststate = 3;
	else
		*laststate = 0;
}
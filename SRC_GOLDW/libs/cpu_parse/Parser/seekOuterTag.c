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
 *seekOuterTag.c
 *4/15/2015
 */

#include <stdio.h>

#include "../Definitions/defs.h"
#include "../../../settings.h"

/*
 *Finds the container tag
 *
 *@PARAM: The file to parse
 *@PARAM: The tag to search for
 *@RETURN: Status
 */
void seekOuterTag(char *fileBuffer, long *readIdx, int *tags, int *laststate)
{
	int it = 0, isIT, interval = HST_THRD;
	int debug, tempCompute;

	/*Expects to find tags - getc handled in UTF func*/
	while(interval > 0 && debug != -1)
	{
		isIT = (int)(it < tags[CNTAIN_SZ]);
		it *= isIT;
		interval -= !isIT;
		
		tempCompute = (int)((debug = (int)fileBuffer[(*readIdx)++]) == tags[it]);
		it += tempCompute;
		it *= tempCompute;
	}
	if(debug != -1)
		*laststate = 2;
	else
		*laststate = 0;
}
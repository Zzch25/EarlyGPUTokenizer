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
 *seekInnerTag.c
 *4/15/2015
 */

#include "../Definitions/defs.h"

/*
 *Finds the attack tags
 *
 *@PARAM: The file buffer
 *@PARAM: The threads file buffer index
 *@PARAM: The desired of the inner tags
 *@RETURN: Status
 */
int seekInnerTag(char *fileBuffer, long *readIdx, int *tags)
{
	int it = 0;
	int debug;
	
	/*Go until a tag match*/
	while(it < tags[CNTAIN_SZ] && debug != -1)
	{
		if((debug = (int)fileBuffer[(*readIdx)++]) == tags[it])
			it++;
		else it = 0;
	}
	if(debug == -1)
		return 0;
	return 3;
}
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
 *seekOuterTag.c
 *4/15/2015
 */

#include "../Definitions/defs.h"
#include "../../../settings.h"

/*
 *Finds the container tag
 *
 *@PARAM: The file buffer
 *@PARAM: The threads buffer index
 *@PARAM: The tag to search for
 *@RETURN: Status
 */
int seekOuterTag(char *fileBuffer, long *readIdx, int *tags)
{
	int it = 0, isIT, interval = HST_THRD;
	int debug;

	/*go until tag match INTERVAL times*/
	while(interval > 0 && debug != -1)
	{
		isIT = (int)(it < tags[CNTAIN_SZ]);
		it *= isIT;
		interval -= !isIT;
		
		if((debug = (int)fileBuffer[(*readIdx)++]) == tags[it])
			it++;
		else it = 0;
	}
	if(debug == -1)
		return 0;
	return 2;
}
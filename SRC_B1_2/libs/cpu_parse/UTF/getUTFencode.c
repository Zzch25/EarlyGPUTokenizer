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
 *getUTFencode.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "localDef.h"

/*
 *Read and compress UTF8
 *
 *@PARAM: The file buffer
 *@PARAM: The threads buffer index
 *@RETURN: The compressed UTF8 character value
 */
int getUTFencode(char *fileBuffer, long *readIdx)
{
	char i = 1, temp;
	int id = 0;

    temp = fileBuffer[(*readIdx)++];
	id = temp;
	
	//Compact bits to an int if they exist on top of the
	//first read
    while((i < 	(char)((temp & MASK1) == MASK1) +
				(char)((temp & MASK2) == MASK2) +
				(char)((temp & MASK3) == MASK3) + 1)
				&& temp != -1)
	{
		temp = fileBuffer[(*readIdx)++];
        id += temp << (i++ << 3);
	}

	if(temp != -1)
		return utfCompress(id, i);
	return '<'; /*fix dis*/
}
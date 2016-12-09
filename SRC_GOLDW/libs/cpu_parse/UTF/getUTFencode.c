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
 *getUTFencode.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "localDef.h"

/*
 *Read and compress UTF8
 *
 *@PARAM: The file being parsed
 *@RETURN: The compressed UTF8 character value
 */
int getUTFencode(char *fileBuffer, long *readIdx)
{
	char i = 1, temp;
	int id = 0;

    temp = fileBuffer[(*readIdx)++];
	id = temp;
	
	//ASSUME NO EOF SINCE THE HEADER WAS RECIEVED
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
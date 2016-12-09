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
 *dataInit.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
 
/*
 *Opens the file stream
 *
 *@PARAM: The file descriptor
 *@PARAM: The tfile to parse
 */
void dataInit(char **fileBuffer, char *dataName)
{
	long sz;
	
	FILE *fd;
	
	if((fd = fopen(dataName,"r")) == NULL)
		fprintf(stderr, "Dependency failure, %s could not open\n", dataName), exit(24);
	
	fseek(fd, 0L, SEEK_END);
	sz = ftell(fd) + 1;
	fseek(fd, 0L, SEEK_SET);
	
	if((*fileBuffer = (char *)calloc(sz, sizeof(char))) == 0)
		fprintf(stderr, "Calloc reports failure\n"), exit(-8);
	
	fread(*fileBuffer, sizeof(char), sz, fd);
	(*fileBuffer)[sz - 1] = -1;

	fclose(fd);
}
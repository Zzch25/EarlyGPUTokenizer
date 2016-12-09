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
 *dataInit.c
 *4/15/2015
 */

#include <stdio.h>
#include <stdlib.h>
 
/*
 *Opens the file stream
 *
 *@PARAM: The file buffer
 *@PARAM: The data file name
 */
void dataInit(char **fileBuffer, char *dataName)
{
	long sz;
	
	FILE *fd;
	
	if((fd = fopen(dataName,"r")) == NULL)
		fprintf(stderr, "Dependency failure, %s could not open\n", dataName), exit(24);
	
	// Get the size then rewind the descriptor
	fseek(fd, 0L, SEEK_END);
	sz = ftell(fd) + 1;
	fseek(fd, 0L, SEEK_SET);
	
	// 0 Alloc
	if((*fileBuffer = (char *)calloc(sz, sizeof(char))) == 0)
		fprintf(stderr, "Calloc reports failure\n"), exit(-8);
	
	// Load
	fread(*fileBuffer, sizeof(char), sz, fd);
	(*fileBuffer)[sz - 1] = -1;

	fclose(fd);
}
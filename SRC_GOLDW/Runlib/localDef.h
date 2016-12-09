/*
 *Zachary Job
 *localdef.h
 *4/15/2015
 *
 *prototype for thread buffer mgr
 */

#ifndef _LOCALDEF_H_
#define _LOCALDEF_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif
//Thread ID for multi-core version
void fillBuffer(int *bufferIn, int TID, int *laststate, int *pages, 
							int *wrTOP, int *wrLOW, int *last, long *readIdx, int *reading);

#ifdef __cplusplus
}
#endif
							
#endif
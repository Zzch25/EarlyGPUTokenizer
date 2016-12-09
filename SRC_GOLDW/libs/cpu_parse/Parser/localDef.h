/*
 *Zachary Job
 *localdef.h
 *4/15/2015
 *
 *prototype for file parser
 */

#ifndef _LOCALDEF_H_
#define _LOCALDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Definitions/defs.h"

void parseContent(char *fileBuffer, long *readIdx, int eot, int *wrTOP, int *wrLOW, int *last, int *lToken, int *laststate);
void seekInnerTag(char *fileBuffer, long *readIdx, int *tags, int *laststate);
void seekOuterTag(char *fileBuffer, long *readIdx, int *tags, int *laststate);
void dataInit(char **fileBuffer, long *readIdx, char *dataName);
void initTags(char *fileBuffer, struct tags_t *configTagsIn);
void initParse(int *memMapSin, int *memMapEin, int *dirtyBitsin, int *memOffsetin, int *trimArrin);

#endif
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
 *utf.h
 *4/15/2015
 *
 *prototypes for utf translation
 */

#ifndef _UTF_H_
#define _UTF_H_

int getUTFencode(char *fileBuffer, long *readIdx);
int utfCompress(int value, int bytes);

#endif
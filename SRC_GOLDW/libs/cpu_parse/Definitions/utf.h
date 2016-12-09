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
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
 *localdef.h
 *4/15/2015
 *
 *prototype for UTF work
 */

#ifndef _LOCALDEF_H_
#define _LOCALDEF_H_

/*
 *The various header masks that may be needed
 *per byte amount of UTF8
 */
#define MASK1 192
#define MASK2 224
#define MASK3 240

int utfCompress(int value, int bytes);

#endif
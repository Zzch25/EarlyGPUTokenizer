/*
 *Zachary Job
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
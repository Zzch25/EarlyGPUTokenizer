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
 *bits.h
 *4/15/2015
 *
 *prototypes for bit tools and the maps
 */

#ifndef _BITS_H_
#define _BITS_H_

#include "defs.h"

/*Masks to remove the UTF8 padding*/
#define SHMG1 	2130706432
#define SHMG2 	4128768
#define SHMG3 	16128 
#define SHMG4 	63

/*Mask to retrieve xxxx from the trigger map*/
#define DBMSK	15

/*Mask to remove byte number added at compression*/
#define BYMASK 	536870911

void markMap(int id, int *memMap);
int readMap(int id, int *memMap);
void markDB(int id, char opt, int *dirtyBits);
int readDB(int id, int *dirtyBits);
int offsetToBit(int largestSZ, int trimIdx);
int sumAtBit(int largestSZ, int trimIdx);

#endif
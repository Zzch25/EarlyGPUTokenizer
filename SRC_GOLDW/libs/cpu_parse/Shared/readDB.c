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
 *readDB.c
 *4/15/2015
 */
 
#include "../Definitions/bits.h"

/*
 *Check a character trigger status
 *
 *@PARAM: Calculated index
 *@PARAM: action map
 *@RETURN: return trigger status
 */
int readDB(int id, int *dirtyBits)
{
	return (dirtyBits[(id << 2) / (sizeof(int) << 3)] 
				>> ((id << 2) % (sizeof(int) << 3))) & DBMSK;
}
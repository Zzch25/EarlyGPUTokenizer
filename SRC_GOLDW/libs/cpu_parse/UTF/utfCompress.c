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
 *utfCompress.c
 *4/15/2015
 *
 *Exploits UTF8 structure
 *
 *UTF8 - can be 4 bits
 *
 *B1  - see below
 *B2  - 10xxxxxx
 *B3  - 10xxxxxx
 *B4  - 10xxxxxx
 *
 *B1:
 *0	  - 0xxxxxxx
 *192 - 110xxxxx
 *224 - 1110xxxx
 *240 - 11110xxx
 */
 
#include "../Definitions/defs.h"
#include "../Definitions/bits.h"

/*
 *Static vs Global overheads must be tested for these masks
 */
 
/*The byte count affects the farthest left mask, this eliminates conditionals*/
const int hMask[] 		= {2130706432, 520093696, 251658240, 117440512};
/*The number of bytes is encoded for decompression*/
//const int numBytes[] 	= {0, 536870912, 1610612736, 1610612736};

/*
 *
 *utfCompress
 *
 *Compress bits then insert bytes worth
 *This can be used for easy reference
 *later on
 *
 *@PARAM: character value
 *@PARAM: The number of bytes
 *@RETURN: Return the 21 compression
 */
int utfCompress(int value, int bytes)
{
	value 	= (value << ((UTF8_BYTES - bytes) << 3));
	
	
	// No comment for you - this simple
	return 	((( ((value & hMask[bytes - 1]) >> ((sizeof(int) - 1) << 3))
			| 	((value & SHMG2) >> ((sizeof(int) - 2) << 3)))
			| 	((value & SHMG3) >> ((sizeof(int) - 3) << 3)))
			| 	( value & SHMG4));
			//|	numBytes[bytes - 1]); //not gonna worry about dis now
}
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
 *markDB.c
 *4/15/2015
 */

/*
 *Set the trigger map to the option input at an index
 *
 *@PARAM: Calculated index
 *@PARAM: 0 ignore, 1 dirty, 2 L5 End char, 
			4 extra for future use, 8 ignore
 *@PARAM: Trigger map
 */
void markDB(int id, char opt, int *dirtyBits)
{
	dirtyBits[(id << 2) / (sizeof(int) << 3)]
				|=	(opt << ((id << 2) % (sizeof(int) << 3)));
}
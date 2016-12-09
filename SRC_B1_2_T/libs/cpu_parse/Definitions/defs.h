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
 *defs.h
 *4/15/2015
 *
 *General definitions for the program
 */

#ifndef _DEFS_CPU_H_
#define _DEFS_CPU_H_

/*Generic file read buffer size*/
#define BUF_SZ 		1024
/*Integer buffer size for grabbing tokens*/
#define IBUF_SZ		64
/*The amount of characters allowed in a tag*/
#define CNTAIN_SZ 	27
/*The actual size as the last space contains the tag size*/
#define CNTAIN_ACT	28
/*How many inner tags may be searched*/
#define ATTAGS_SZ 	2
/*The max possible UTF8 size after compression*/
#define UTFBINSZ  	2097151
/*The max possible ASCII size*/
#define ASCBINSZ  	127

/*How large a key may be, trims memmaps*/
#define TRIMSZ 5
/*Max mappings to the trim offset map*/
#define TRIMMX 64

/*
 *UTF8 expected bytes
 *
 *CANNOT CHANGE WITHOUT SERIOUS MODIFICATION
 *The code is designed around a four byte encoding
 *The compression and reading would need modification
 *Other things may need changes
 */
#define UTF8_BYTES 4

/*Precompute file name*/
#define LAST_PRECMP 	"prec.dat"

/*Don't question the min max*/
#define fmin(a, b) (((a) < (b)) ? (a) : (b))
#define fmax(a, b) (((a) > (b)) ? (a) : (b))

/*Tags to seek*/
struct tags_t
{
	int attackTagSZ;
	int container[1][2][CNTAIN_ACT];
	int atTags[ATTAGS_SZ][2][CNTAIN_ACT];
};

#endif
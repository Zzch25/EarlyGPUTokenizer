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
 *settings.h
 *4/15/2015
 *
 *Settings that are used by files at multiple levels to 
 *modify execution. DOES NOT GUARANTEE correct execution
 *with modified values ---Y E T---
 */

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

//The number of host threads
#define HST_THRD		1
//Max words + spacers to feed + 2 flush loc + 2 for the show + 0 for my boat
#define FEED_SZ			9216
//Max words + spacers to feed + 2 flush loc + 2 for the show + 0 for my boat
#define FEED_BY			9216
//Max words + spacers to feed
#define FDBY_HF			4608
//FEED_BY / THREADSPB
#define FDBY_ST			48
//FDBY_HF / THREADSPB
#define FDBY_LD			24
//The language bits for more precise hashing will be implemented later
#define LANG_BITS_MSK	268435455

#define OUTPUT_HIST		"resultsGold.csv"

#endif
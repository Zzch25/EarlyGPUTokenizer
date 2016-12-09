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
 *parseContent.c
 *4/15/2015
 */

#include <stdio.h> 

#include "../Definitions/bits.h"
#include "../Definitions/utf.h"

/*
 *WHY on earth global? Well
 *
 *A)
 *reworking the cpu lib would be needed to eliminate difficulties
 *
 *B)
 *less overhead in some respects given the way the code
 *has been organised for readability.
 *
 *C)
 *Depending on the compiler I've seen static overhead... Not sure
 *if it would be optimized away here.. Also would require an if block.
 */
////////////////////////////////////////////////////////////////
int 							
				*memMapS, *dirtyBits, *memOffset,
				*memMapE, *trimArr;
////////////////////////////////////////////////////////////////
//READ ABOVE AS TO WHY I DECIDED ON GLOBALS V STATICS V ANYTHING

/*
 *Initialises a threads data
 *
 *@PARAM: The recursion match map
 *@PARAM: The key match map
 *@PARAM: The trigger map
 *@PARAM: The offset into the map given an index
 *@PARAM: The size compaction  reference array
 *@PARAM: The file to processs
 */ 
void initParse(int *memMapSin, int *memMapEin, int *dirtyBitsin,
				int *memOffsetin, int *trimArrin)
{	
	memMapS = memMapSin;
	memMapE = memMapEin;
	dirtyBits = dirtyBitsin;
	memOffset = memOffsetin;
	trimArr = trimArrin;
}

/*
 *Seeks through the given inner tag
 *uses heavy bit manipulation to modify
 *whether or not something remains in the
 *buffer for the final result
 *
 *
 *ENGINE V1
 *
 *TBD V2:	implement bit 3 (x_xx) of target map
 *     		and modify memMapE to target both
 *     		the start of sequence and end of
 *     		sequence to prevent premature flip
 *
 *@PARAM: The file to parse
 *@PARAM: The buffer
 *@PARAM: The final buffer index
 *@PARAM: The exclude section map
 *@PARAM: The exclude key map
 *@PARAM: The offset into either map at an index
 *@PARAM: Character trigger action map
 *@PARAM: Trim to keep memmaps down on size
 *@PARAM: End of inner tag - FIX THIS TO ACTUALLY CHECK THE TAG.. ONLY WORKS FOR XML
 *@RETURN: Status of seek, did the buffer grab something
			and is there more in the tag
 */ 
void parseContent(char *fileBuffer, long *readIdx, int eot, 
					int *wrTOP, int *wrLOW, int *last, int *lToken, int *laststate)
{
	int 		idx = 0, j = 0, ret, i = 0, eor = 0,
				dirtyChar = 0, overLim = 0, flag = 0,
				exists = 0, exists2 = 0, temp = 0;
	
	*lToken = 0;
	ret = getUTFencode(fileBuffer, readIdx);
	*last = eor = readDB(ret, dirtyBits);
	
	while(ret != eot)
	{
		/*
		 *IF writeable character, set it 
		 *else flush to garbage loc
		 */
		overLim 	= (int)(i < TRIMMX - 2);
		*lToken		+= ret;
		i			+= overLim;

		/*
		 *Ignore sequence or key sector, also sets for the event
		 *that this code may exit leaving the parsing interrupted
		 *The write bit must be set with the current mode
		 */
		 
			/*
			 *Update any check into any dirty sequences
			 *Reset idx calculation if modifiers permit
			 */
		dirtyChar 	= (int)((eor & 1) == 1);
		overLim 	= (int)(j < TRIMSZ);
		j			+= 1 * (int)(j > 0 || dirtyChar);
		j			*= overLim * (int)((eor & 8) != 8);
		idx			*= (int)(j > 1) * (int)(trimArr[ret] > 0);
		idx 		+= trimArr[ret]
						* memOffset[j - 1];
		
			/*
			 *The dirty sequence modifiers, change write bit
			 *and flush buffer write if modifiers permit
			 */
		exists 		=  readMap(idx, memMapS);
		exists2		= (int)(readMap(idx, memMapE) ||
						(((readDB(ret, dirtyBits) & 2) == 2) && !wrLOW));
		*wrTOP		^= exists;
		*wrLOW		^= exists2;
		/*fprintf(stderr, ":wr: %d %d :char: %c :exists: %d %d :eor: %d :j: %d :idx: %d :i: %d\n", wrLOW, wrTOP, temp, exists, exists2, eor, j, idx, i);*/
		i			*= !exists * !exists2 * (int)(eor == 0);
		*lToken 	*= (int)(i > 0);
		
		/*
		 *Check if return should occur set the flag
		 *and force eot to ret
		 */
		temp = ret 	= getUTFencode(fileBuffer, readIdx);
		*last		= eor;
		eor 		= readDB(ret, dirtyBits);
		/*If actually end of tag*/
		flag 		= (int)(ret == eot);
		/*reuse of exists - terminate loop if token found*/
		exists		= (int)((eor != 0 && *last == 0 
							&& exists == 0 && exists2 == 0 
							&& *wrTOP  == 1 && *wrLOW   == 1)
							|| flag);
		ret			= ret * !exists + exists * eot;
	}
	
	/*
	 *ret is lost, a loop restructure
	 *is needed to eliminate this.. however, 
	 *if it works and doesn't murder efficiency
	 *with a hammer...
	*/
	dirtyChar 	= (int)((eor & 1) == 1);
	overLim 	= (int)(j < TRIMSZ);
	j			+= 1 * (int)(j > 0 || dirtyChar);
	j			*= overLim * (int)((eor & 8) != 8);
	idx			*= (int)(j > 1) * (int)(trimArr[temp] > 0);
	idx 		+= trimArr[temp]
					* memOffset[j - 1];
					
	exists 		=  readMap(idx, memMapS);
	exists2		= (int)(readMap(idx, memMapE) ||
					(((readDB(ret, dirtyBits) & 2) == 2) && !*wrLOW));
	*wrTOP		^= exists;
	*wrLOW		^= exists2;
	/*fprintf(stderr, ":wr: %d %d :char: %c :exists: %d %d :eor: %d :j: %d :idx: %d :i: %d\n", wrLOW, wrTOP, temp, exists, exists2, eor, j, idx, i);*/
	
	/*
	for(og = 0; og <= *endToken; og++)
		fprintf(stderr, "%c", encToken[og]);
	fprintf(stderr, "\n");
	*/
	
	/*Flush on eot for debugging sake*/
	*wrTOP = !flag * *wrTOP + flag;
	*wrLOW = !flag * *wrLOW + flag;
	
	*laststate = (3 - (flag << 1));
}
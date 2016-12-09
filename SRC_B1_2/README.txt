
 (c) Zachary Job
 All rights reserved, I am not liable for damages.
 Re-distributable and use with my permission contact me at zachjob@verizon.net
 Presentable without modification including comments for educational purposes

VERSION 0.7 (BETA - PROOF OF CONCEPT)

INDEX+++++++++++++++++++++++++++++++++++++++++++++
(1) TOKENIZE GENERAL USAGE
(2) CONFIGURATION FILES
(3) VERSION NOTES
INDEX+++++++++++++++++++++++++++++++++++++++++++++

(1) Tokenize general usage
==================================================

See notes for the current version before continuing.
The beta is a highly limited concept version.......

This program is designed to find the frequency of all occurrences
of data in big data and web formats. By specifying the correct
configuration in a file you may name however you please, and 
the structure is as follows.

>A container tag can be targeted to search for a sub tag
>A multiple tag sets can be targeted once inside
>Characters can be ignored
>Characters can stop a read and can be defined as a repeating
 sequence. EG write off for [ would turn to write on with [[
 and back to write off with [[[.
>Keys can be excluded

Once configured, the program may be run with switches

>configuration file
>data file
>system mode EG run or load pre-compute
>container tags to process

Data will be retrieved and the frequency counted.
Retrieval not yet implemented. However a query on a string
is not difficult. Simply apply the hash to the string
and look up the index.

(2) Configuration file format
==================================================

The loading asks that it finds the following lines in order. 
Please avoid extra newlines and spaces, behaviour may stay 
undefined till later versions.

The current options are basic, however they will be expanded
on in later versions.
	
CURRENT OPTIONS:

LINE 1: CONTAINER TAG START AND END
LINE 2: CONTENT ONLY TAGS START AND END - IN ORDER OF REF
LINE 3: START AND END KEYS, IGNORE INNER CONTENT 
LINE 4:	SINGULAR KEYS TO IGNORE
		<<can be a unique subset>>
LINE 5: CHARACTERS TO EXCLUDE FROM WRITE, WILL BE EXPANDED
		DO NOT ADD CHARS STARTING OR ENDING KEYS
		THE FORMAT IS UP TO FIVE OF THE INITIAL CHARACTERS
		NOT INCLUDING THE START INDICATOR, SPACE THE END
		INDICATOR.
		TO ACCEPT RANGE. THE KEYS FIRST AND LAST CHARACTER
		<<<wILL BE AUTOMATICALLY BE IGNORED>>>
		
		
NOTE: 	space indicated by $^ AND unimportant characters 
		are ignored that are not in a valid range since 
		the initial use of this code is to strip words
		
		Also assuming ACSII configurations intermixed with UTF8...
		
		Only supports 64 unique characters excluding tags and ignore
		characters...
		
		//////////////////////////////////////////////////////////
		DO NOT USE SAME START AND END CHARACTERS ON LINE 4
		DO NOT USE MORE THAN ONE END INDICATOR CHARACTER ON LINE 5
		----------CPUPPV1 -> CPUPPV2 will bring fixes-------------
		//////////////////////////////////////////////////////////

Example - 	Search primarily user or editor defined information
			while excluding a good deal of formatting that is
			useless. Also ignore tild because I'm not feeling it
			right now

<page> </page>
<text$^xml:space="preserve"> </text>
&lt; &gt; [ ] {{ }} {| |}
|fram | |upri | |url= | |upri = |thum | |tria | |tria = |bott | |bott = |thum | |left | |righ | [User : [Spec : |spec = [Cate : |cate = [File : |File : |file = [Imag : |Imag =
~ . ' " ( ) , / \ ? ! @ # * _ + 0 1 2 3 4 5 6 7 8 9

(3) Version Notes
==================================================

TODO (beyond word tokenizer application):

	1) Implement expression EG |_I__I__I_px| would = |###px|
	2) Function generator using the above addition and a translator
		to the existing engine. One of the larger reasons precompute
		 exists.
	3) PARSING SIDE - IMPLEMENT CPUPPV2 FOR IMPROVED KEY DETECTION

---------------------------------------------------

V(8.0):
>CPUPP
	>V1
		>Repeatable flip write sequences
		>Short keys, longer are prone to false flips
			>Fix coming in V2
		>Ignore characters
		>1 Container tag to seek
		>2 Interior tags to seek
		
		>Pre-process data
			>unstable, fails on some systems
		>runtime load data
		>run pre-processed data
		
		>File loaded to buffer
			>Naive, loads file wholly to memory
			>1.0 to implement threaded chunk loading
		
		>Result store
			>Results are written to CSV
			
		Threaded buffer processing and batch issuing
			>0 contention, string seek needs improvement
			
		UTF8/ASCII Support
			>currently dumb, automatically treats files as
			UTF8
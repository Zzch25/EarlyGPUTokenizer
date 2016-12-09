(c) Zachary Job 4/15/2015

Configuration file format
==================================================

The loading asks that it finds the following lines in order. 
Please avoid extra newlines and spaces, behaviour may stay 
undefined till later versions.

The current options are basic, however they will be expanded
on in later versions.

TODO (beyond word tokenizer application):

	1) Implement expression EG |_I__I__I_px| would = |###px|
	2) Implement accept/reject chars with ignore list
	3) Implement space rejection at processing
	4) Ignore case
	5) Function generator using above additions and a translator
		to the existing engine. One of the larger reasons precompute
		 exists.
	
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
		
		Also assuming tags are ACSII...
		
		Only supports 73 unique characters excluding the
		start indicators, end indicators, and tag innards...
		
		//////////////////////////////////////////////////////////
		DO NOT USE SAME START AND END CHARACTERS ON LINE 4
		DO NOT USE MORE THAN ONE END INDICATOR CHARACTER ON LINE 5
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
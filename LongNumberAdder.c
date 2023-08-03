#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>	//strlen() (getStringLength), strdup() (duplicateString)

#define ERR_LENGTH 2
#define ERR_NON_DIGIT 3

//ADDS TWO LARGE-VALUED BASE10 NUMBERS


//PREREQUISITE KNOWLEDGE:
//LSD = Least Significant Digit, MSD = Most Significant Digit, sumPlaceholder = Result of adding together both MSDs,
//  addend = # to add to another # (i.e., input #, not the resultant #)
//Mutable = Modifiable/Changeable, Immutable = Static and unchanging (after its value has been initially set)
//char, short, int, long, float, double, bool are all ways of storing bits. Some just use more bits to store the same #.
//  (char)255 = 1111 1111, but (int)255 = 00000000 00000000 00000000 11111111. (these two #s assume only positive values can be represented, making the MSD always positive)
//  int unnecessarily uses more bits to store a value in the same range that char can handle
//Speeds (unit=time):  Writing to storage/disk(cache blocks of #s)(longest time) >> Writing to memory(cache blocks of #s) > Division (2 #s) > Multiplication (2 #s) > Addition (2 #s) > Shifting (1 #)(shortest time)
//  Note: Shifting a single # is different from shifting an entire array.
//strdup() allocates memory but does NOT free it. When used, strdup() should either be free()d or only be used when creating known-length arrays at compile-time
//  I.e., Don't do `char* ary2 = strdup(ary);` without using free(),
//    but `char ary2[501] = strdup(ary);` should be fine (if ary has 500 or fewer elements and ary is NULL-terminated) without doing `free(ary2)`



void printDigitArray(const char* ary, int aryLen)
{
	printf("{");
	for(int i=0; i<aryLen-1; i++)	//Prints entire array except the last element
		{printf("%d,", ary[i]);}
	printf("%d}\n", ary[aryLen-1]);	//Prints the last element
}
void printCharArray(const char* ary, int aryLen)
{
	printf("{");
	for(int i=0; i<aryLen-1; i++)	//Prints entire array except the last element
		{printf("%c,", ary[i]);}
	printf("%c}\n", ary[aryLen-1]);	//Prints the last element
}

//Yes, the input type is an array of CHARs, not integers. Chars can be read as either numbers (short, int, long, float, double) or characters (char).
const char* convertDigitsInCharArrayToString(char* digAry, const int aryLen)
{
	char str[aryLen+1];	//Allocate memory to new array at compile-time without modifying the array's data

	for(int i=0; i<aryLen; i++)	//Iterate through entire new-array EXCEPT for last element
	{
		//WARNING: isdigit() IS ONLY FOR CHAR REPRESENTATIONS, NOT REGULAR #S, MEANING IT WILL CHECK FOR 48<=num<58 INSTEAD OF 0<=num<10
		if( digAry[i] < 0  ||  digAry[i] > 9 )	// if(isNotDigit).
		{
			printf("convertDigitsInCharArrayToString():  Digit array contained a non-digit character (int %d) at index %d. Expected Base10 input numbers. Exiting program.\n", digAry[i], i);
			//Do I need to free anything before exiting (to avoid SEGMENTATION FAULT)? FIXME (validate)
			//  I don't think I need to free a compile-time allocated array.
	        exit(ERR_NON_DIGIT);
		}
		str[i] = digAry[i]+'0';	//Convert single digit into char
	}
	str[aryLen]= '\0';	//Terminate the string by making the last character of the array be the NULL char.

	//return str;	//Can't do this because the locally-scoped-and-compile-time-created array gets deallocated after this function ends,
	//  meaning the returned pointer points to something that variable "str" no longer owns
	return strdup(str);
}


//Prevents printing the leading zeroes. E.g. {0,1,3,0,4} -> 1304
void print_decimalRepresented_CharArrayWithoutLeadingZeroes(char* ary, int aryLen)
{
	char prevMostSignificantDigitsAreZero = 1;	//booleanVariable = true;
	for(int i=0; i<aryLen; i++)
	{
		char currDigit = ary[i];	//char because it only needs to represent values 0 through 9. char can represent 0 through 127 (maybe 255?)
		//if(prevMostSignificantDigitsAreZero && currDigit==0)	//e.g. [0]01023 or 0[0]1023
		//{prevMostSignificantDigitsAreZero = 1;}	//boolVar = true;	//This doesn't change anything, so these lines are unnecessary, except to show the human the logic more clearly
		if( prevMostSignificantDigitsAreZero && currDigit!=0)	//if(isFirstNonzeroDigit)				//e.g. 00[1]023
		{
			printf("%d", currDigit);
			prevMostSignificantDigitsAreZero = 0;	//boolVar = false;
		}
		else if(!prevMostSignificantDigitsAreZero)				//if(notFirstZeroes AND isAnyDigit)		//e.g. 001[0]23 or 0010[2]3 or 00102[3]
			{printf("%d", currDigit);}
	}	//Print the digit (%d), NOT the character because I changed all the characters to their equivalent digit representation
	printf("\n");
}


//Shift every array element left (one index closer to index 0, truncating/overwriting the element at index 0)
//USES AND MODIFIES THE PASSED-IN ARRAY TO SAVE MEMORY. DOES NOT ALLOCATE EXTRA ARRAY THAT WOULD NEED TO BE FREED.
//NEVERMIND, IMMUTABILITY TRUMPS MEMORY USAGE NOWADAYS, SO I WON'T MODIFY THE PASSED-IN ARRAY.
//{1,2,3,4,5,6,7,8,9,10} -> newlyAllocatedArray{2,3,4,5,6,7,8,9,10,0}
const char* removeMSDfromCharArray_aryShiftLeft(char* ary, const int originalAryLen)
{
	//  Goes element by element. Inefficient.
	//for(int i=1; i<aryLen; i++)
	//	{ary[i-1] = ary[i];}	//E.g. element0 = element1; Last element of array is left unchanged

	//  Best for saving memory, but not security due to mutability.
	//I can't guarantee that the last element will always end up 0, but it always ended up to be 0 in my tests (using memcpy() alone). Idk why.
	//memcpy(destinationArray,sourceAry,lengthToCopyFromSrcToDest)
	//Start at memory location (ary+1), copy everything from that location to (ary+1)+aryLen INTO starting memory location (ary)
	//memcpy(ary, ary+1, aryLen);	//Utilizes transferring entire blocks of memory at a time instead of going individually, # by #

	//Relying on the last char being NULL after using `ary={}`, I will use string duplication (shouldn't be used on regular arrays, only strings)
	char copiedAry[originalAryLen] = {};	//Creates locally-scoped-and-compile-time-created array that gets deallocated after this function ends.
	//Above array is filled with NULLs (thanks to ={};)
	memcpy(copiedAry, ary+1, originalAryLen);
	return strdup(copiedAry);	//Can't simply return copiedAry because its pointer is invalid after this function finishes execution
}




//Adding 999,999,999,999,999 and 111,111,111,111,111 (each # is 15 decimal digits)
//Array (char):  (LSD)9,1, 9,1, 9,1,  9,1, 9,1, 9,1,    9,1, 9,1, 9,1,      9,1, 9,1, 9,1,  9,1, 9,1, 9,1,  sumPlaceholder(MSD)
//  LSD = Least Significant Digit, MSD = Most Significant Digit
//  Actually, addition is way faster than copying #s to arrays, so I'll leave the two strings as they are to minimize the # of array modifications
//*****Shifting is faster than addition, so I should use shifting (instead of addition) where I can too. I can have three separate mutable arrays,
//*****    start at the LSD's index in all 3 arrays,
//*****    add the values then store at the same index in the results array (and increment the next index of one of the addends if addedResult>9),
//*****    then shift the two addend arrays to the right. No, because I only need to modify one array, not all three. The less writing to memory, the better.
//*****    Shifting an array is infinitely worse than index addition (i--, i++) because shifting requires writing the array back to memory, where an index can
//*****    simply be stored in a register without modifying (writing) the array.

// 1) Copy inputted #s (in C string form) into new mutable char arrays
//      E.g. num1[0] = num1_parameterInput[0];		//left side is changeable but right side is const and hence immutable
// 2) Create two new arrays (one array per input #) of (arrays that each have enough bools to store any decimal digit 0 through 9).
//      E.g. '9''7''2' => [abcd][efgh][ijkl] = [ 1,0,0,1 ][ 0,1,1,1 ][ 0,0,1,0 ]	then do that for the other input # too
// 3) Convert each char element in both mutable arrays to the integer equivalent (still using the char datatype because it requires only one byte of memory)
//      E.g. '9''7''2'  =>  '9'-'0', '7'-'0', '2'-'0'  =>  9,7,2					then do that for the other input # too
// 4) Convert each integer-equivalent char into binary, storing it into the array that contains boolean arrays.
//      This encoding is Binary-Coded Decimal (BCD), which cannot be added because place values reset and don't stack ([8*10,4*10,2*10,1*10][8,4,2,1] instead of [128,64,32,16][8,4,2,1]).
//      E.g. 9,7,2  =>  [ 1,0,0,1 ][ 0,1,1,1 ][ 0,0,1,0 ]							then do that for the other input # too
// 5) To change BCD into pure-binary, multiply each successive digit by 10 (PROBLEM: THIS WILL OVERFLOW), then ...
//      E.g. 9*100, 7*10, 2*1
//    Converting the entirety of a huge # into Binary is guaranteed to have an overflow problem, hence this entire program,
//    so BCD is actually perfect because, just like place values in Base 10, each block of [8421] can be its own place value (as long as 1001 is treated as the biggest #).
//    E.g. 19 =  [0001][1001]_BCD
//         +7   +[0000][0111]_BCD
//               [0010][0000]     = 20_BCD... (not 26...) There's no practical/useful way for this to work. Ignore all this.
// ...
// ?) Now, utilizing both pure-binary bool array results simultaneously, bitwise add them (XOR with AND)
//      E.g. 972    0011 1100 1100
//          +564 =  0010 0011 0100
// ?+1) Finally, convert the result back to a decimal
//Well that was a bad idea.

//Nah, what if I just implement rules for Base 10 addition without ever converting to binary?
//I still need to make sure the lengths/place-values will be accounted for (123+123,000 != 246,000)
//    E.g. 19 =  [1][9]
//         +7   +[0][7]
//               [1][16] = [1+1][16-10] = [2][6]. Yay this should work!
//Implement via:
// 1) Find the maximum length of the two #s, create the results array with size (maxNumLength + 1)
//      (+1 for the extra place value, because 10-18 (result of adding things like 5+5 and 1+9 up to 9+9) requires two place values)
// ?) Add two respective place values, then store into result (same place value)
// ?) Subtract 10, then if(result>0){addOneTo nextHigherPlaceValue, storeResult} else{goto nextHigherPlaceValue}
const char* b10_add(const char* base10num1_immutable, const char* base10num2_immutable)
{
	int i, numDigitsInNum1, numDigitsInNum2, inxResult, inxNum1, inxNum2;
	char *num1, *num2;

	//Check that the strings are actual strings and not non-NULL-terminated arrays.
	//strlen() starts at the address given by the pointer, then increments a counter for each char traversed until '\0' (NULL char) is found
	//strlen("Hi!") == 3  because "Hi!" == 'H' 'i' '!' '\0' == length of 3 nonNULL chars
	//If strlen() produces a humongous # (especially one that overflows into negative length), it very likely means that a NULL character was
	//  forgotten and there luckily was at least one random NULL character in memory
	numDigitsInNum1 = strlen(base10num1_immutable);	//num1Len
	numDigitsInNum2 = strlen(base10num2_immutable);	//num2Len

    //Check that the strings only contain digits
	for(i=0; i<numDigitsInNum1; i++)
	{
	    if( !isdigit( base10num1_immutable[i] ) )
	    {
	        printf("String num1 contained a non-digit character at index %d. Expected Base10 input numbers. Exiting program.\n", i);
	        exit(ERR_NON_DIGIT);
	    }
	}
	for(i=0; i<numDigitsInNum2; i++)
	{
	    if( !isdigit( base10num2_immutable[i] ) )
	    {
	        printf("String num2 contained a non-digit character at index %d. Expected Base10 input numbers. Exiting program.\n", i);
	        exit(ERR_NON_DIGIT);
	    }
	}

	//Duplicate the passed-in string data into new char array. I think num1 and num2 will need to be freed, but I'm uncertain. FIXME (verify)
    /* char* */num1 = strdup(base10num1_immutable);
    /* char* */num2 = strdup(base10num2_immutable);

	//lengthOfResultArray = maxSizeOf(num1,num2) + 1
	int numDigitsInSumResult = numDigitsInNum2;
	if(numDigitsInNum1 > numDigitsInNum2)
		{numDigitsInSumResult = numDigitsInNum1;}
	const int resultArrayLength = numDigitsInSumResult + 1;	//+1 for sumPlaceholder
	char addingAry[resultArrayLength];	//Allocate memory to array at compile-time without modifying array's likely-random data.
	
	//Set every element of resultArray to NULL characters ('\0') so no accidental #s pop up randomly
	for(i=0; i<resultArrayLength; i++)
	{addingAry[i] = '\0';}
	
	//Convert the chars to easily understandable #s that support arithmetic
	for(i=0; i<numDigitsInNum1; i++)
	{
		//ASCII char '0' is something like decimal value 48.
		//48 is a mathematical # that is easy to work with, unlike the confusing '0' char value not being value 0.
		// "num - '0'" works on all systems but "num - 48" may not (ex: EBCDIC).
		num1[i] = num1[i] - '0';
	}
	for(i=0; i<numDigitsInNum2; i++)
	{num2[i] = num2[i] - '0';}



	//Doesn't work because Most Significant place value can be different between #s
	/*for(i = resultArrayLength-1; i>0; i--)	//Start at far end of # (LSD), work toward the left until reaching second-greatest place value
	{
		//To not allocate extra memory, I will use the results array (addingAry) as my temporary array
		addingAry[i] = num1[i] + num2[i];
		if(addingAry[i] > 9)
		{
			addingAry[i] -= 10;	//Subtract 10 from current-place-value's value (e.g. val15->val5)
			addingAry[i-1]++;	//Increment next-higher-place-value's value (e.g. val3->val4)
		}
	}*/

	//Start at far right end of all #s (LSD), work toward the left until reaching second-greatest place value, using results array (addingAry) for carry digits
	//Shorter names for readability. "inx" == "index"
	//-1 to not illegally access outside the max array index (done here instead of having to write "index-1" a billion times in the below while() block)
	inxResult = resultArrayLength - 1;
	inxNum1   = numDigitsInNum1 - 1;
	inxNum2   = numDigitsInNum2 - 1;
	while(inxResult>=0)
	{
		//To not allocate extra memory, I will use the results array (addingAry) as my temporary array
		if(inxResult>=0 && inxNum1>=0 && inxNum2>=0)
			{addingAry[inxResult] = num1[inxNum1] + num2[inxNum2] + addingAry[inxResult];}
		//   12[3]		inxNum1==2
		//+ 456[7]		inxNum2==3
		//________   Here, "[]" indicates which digits are being focused on
		// 0000[0]		inxResult==4

		//In this situation (where one array ends but the other still has remaining digits),
		//  copy the remaining digit to the resultsArray (think of it as adding 0 to the nonzero addend)
		//"x" indicates that it isn't part of the array, but should be mathematically treated as a leading 0.
		//"x" isn't any actual value in memory because it doesn't exist in memory: "x" is symbolic.
		//  [x]123		inxNum1== -1
		//+ [4]567		inxNum2==  0
		//________   Here, "[]" indicates which digits are being focused on
		// 0[0]000		inxResult==1
		else if(inxNum1>=0)	//elif(numOne hasRemainingDigitsToAdd butNumTwoDoesNot)
			{addingAry[inxResult]  =  num1[inxNum1] + 0 + addingAry[inxResult];}
		else if(inxNum2>=0)	//elif(numTwo hasRemainingDigitsToAdd butNumOneDoesNot)
			{addingAry[inxResult]  =  0 + num2[inxNum2] + addingAry[inxResult];}


		if(addingAry[inxResult] > 9  &&  inxResult>0)	//if(currentValue takesTwoDigitsToRepresentInBase10 AND nextHigherPlaceValue_InResultsArray_Exists)
		{
			//printf("addingAry[inxResult] == %d  >  9. inxResult == %d\n", addingAry[inxResult], inxResult);
			addingAry[inxResult] -= 10;	//Subtract 10 from current-place-value's value (e.g. val15->val5)
			addingAry[inxResult-1]++;	//Increment next-higher-place-value's ([i-1]) value (e.g. val3->val4)(++) in resultsArray
		}

		//Update the loop-variable (inxResult) and Move all three indices to next-higher place value
		inxResult--;
		inxNum1--;
		inxNum2--;
	}


	//PRINT
	print_decimalRepresented_CharArrayWithoutLeadingZeroes(addingAry, resultArrayLength);

	

	return convertDigitsInCharArrayToString(addingAry, resultArrayLength);
}



void testConversion()
{
	printf("\n\nCommence conversion test...\n");

	char digAry0[] = {1,2,3,4,5,6,7,8,9,10};
	char len0 = 10;
	const char *tempAry = removeMSDfromCharArray_aryShiftLeft(digAry0, len0);
	printf("Original Array's address: %p\nReturned Array's address: %p\n", digAry0, tempAry);
	printf("Original Array after removeMSDfromCharArray_aryShiftLeft( {1,2,3,4,5,6,7,8,9,10} ): ");
	printDigitArray(digAry0, len0);
	printf("Returned Array from  removeMSDfromCharArray_aryShiftLeft( {1,2,3,4,5,6,7,8,9,10} ): ");
	printDigitArray(tempAry, len0);
	printf("\n\n");


	char digAry1[] = {0,1,2,3,4,5,6,7,8,9};
	fprintf(stderr, "Legal 0-9: %s\n", convertDigitsInCharArrayToString(digAry1, 10) );

	char digAry2[] = {2,3,4,5, 10,11,12};
	fprintf(stderr, "Illegal 10-12: {2,3,4,5, 10,11,12} -> {%s}\n", convertDigitsInCharArrayToString(digAry2, 7) );
	//Should crash before printing this

	//Stuff after this line (within this testing block) shouldn't be reached (unless you comment some stuff out)
	char digAry3[] = {2,3,4,5, -1};
	fprintf(stderr, "Illegal -1: {2,3,4,5, -1} -> {%s}\n", convertDigitsInCharArrayToString(digAry2, 5) );
	//Should crash before printing this

	printf("\nEnd conversion test...\n\n");
}





int main()
{
	const char* num1_immutable =   "12345";//[0][1][2][3][4]'\0'		//strlen("12345")==5
	//const char* num2_immutable = "456";//[0][1][2]'\0'	//strlen("456")==3
	const char* num2_immutable =   "12345000123450001234500012345";

	const int numDigitsInNum1 = strlen(num1_immutable);
	const int numDigitsInNum2 = strlen(num2_immutable);

    //add(const char* num1_immutable, const char* num2_immutable)
    printf("Result: %s\n", b10_add(num1_immutable, num2_immutable));

	testConversion();

    return 0;
}
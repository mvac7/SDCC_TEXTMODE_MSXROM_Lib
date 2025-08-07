/* =============================================================================
# Test80c

- Version: 1.5 (27/02/2024)
- Author: mvac7/303bcn
- Architecture: MSX
- Format: 8K ROM
- Programming language: C and Z80 assembler
- Compiler: SDCC 4.4

## Description:
	Test fR3eL's textmode_MSXBIOS library in Text2 mode with 80-column.
	For MSX computers with V9938 and BIOS that supports 80 columns.
	(fR3eL Project)

## Histoy of versions: (dd/mm/yyyy)
- v1.5 (27/02/2024) Improvements and changes to the v1.5 library
- v1.4 (27/11/2023) update to SDCC v4.3
- v1.3 (06/04/2018)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/textmode_MSX.h"



// ---------------------------------------------------------------------------- Labels
#define  HALT __asm halt __endasm   //wait for the next interrupt



// ---------------------------------------------------------------------------- Declaration of functions
char PEEK(uint address);
char VPEEK(uint address);
char INKEY(void);
void WAIT(unsigned int cicles);

void test_SC080(void);

void testWIDTH(void);
void testPRINT(void);
void testPrintNumber(void);
void testCLS(void);

void PressAnyKey(void);

void PrintExtendedGFXchar(char A);
void PrintLine(char size);
void DrawBox(char width, char height);



// ---------------------------------------------------------------------------- Constants
const char text01[] = "textmode_MSXBIOS Lib";
const char text02[] = "Test Text2 mode 80 columns";
const char text10[] = ">Test CLS()";

const char text_80col[] = "----5----1----1----2----2----3----3----4----4----5----5----6----6----7----7----8         0    5    0    5    0    5    0    5    0    5    0    5    0    5    0";

const char text_LF[] = "\n"; // LF line Feed
const char text_CR[] = "\r"; // CR Carriage Return

const char testString[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."; 
// Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

const char presskey[] = "Press a key to continue";

const char CheckResult[2][8] = {"=ERROR!","=Ok    "};



// ---------------------------------------------------------------------------- Global Variables




// ---------------------------------------------------------------------------- Definition of functions


void main(void)
{
	COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
	WIDTH(32);
	SCREEN1();

	LOCATE(2,10);
	DrawBox(28, 4);
	
	LOCATE(3,11);
	PRINT(text01);
	LOCATE(3,12);
	PRINT(text02);

	PressAnyKey();
	  
	test_SC080();

	CLS();
	PRINT("END");
	WAIT(30*5);
}



/* =============================================================================
PEEK
Description:	Read a 8 bit value from the memory.
Input:			[unsigned int] memory address
Output:			[char] value
============================================================================= */
char PEEK(uint address) __naked
{
address;
__asm
	ld   A,(HL)
	ret
__endasm;
}



/* =============================================================================
VPEEK
Description:	Reads a value from video RAM. 
Input:			[unsigned int] VRAM address
Output:			[char] value
============================================================================= */ 
char VPEEK(uint address) __naked
{
address;
__asm
	jp BIOS_RDVRM
__endasm;
}



/* =============================================================================
INKEY
Description:	Waits for a key press and returns its value
Input:			-
Output:			[char] key code
============================================================================= */
char INKEY(void) __naked
{
__asm   
	jp  BIOS_CHGET
__endasm;
}




/* =============================================================================
WAIT
Description:	Generates a pause in the execution of n interruptions.
Input:			[unsigned int]  cicles number (VBLANKs)
				(Note: PAL: 50=1second. ; NTSC: 60=1second.)
============================================================================= */
void WAIT(unsigned int cicles)
{
	unsigned int i;
	for(i=0;i<cicles;i++) HALT;
}





// ############################################################### TEST functions


/* =============================================================================
Test set in Text2 mode (Screen 0/80col)
============================================================================= */
void test_SC080(void)
{
	COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);      
	WIDTH(80);
	SCREEN0();

	PrintLN("Test TEXT2 mode (Screen 0 80col)");

	testWIDTH();

	testPRINT();
	testPrintNumber();

	testCLS();
	
	PressAnyKey();
}



void testWIDTH(void)
{
	char columns = GetColumns();

	PrintLine(columns);
   
	PRINT("Test WIDTH - Columns=");
	PrintNumber(columns);

	PRINT(text_LF);
	PRINT(text_80col);
}



/* =============================================================================
   Test the functions to print texts. (LOCATE, PRINT and PrintLN)
============================================================================= */
void testPRINT(void)
{
	//char cursorX;
	char cursorY;
	char columns = GetColumns();
	
	PrintLine(columns);
	
	PrintLN("Test PRINT and PrintLN");
	PRINT(testString);
	
	PRINT(text_LF);	// CR Carriage Return	
	PrintLine(columns);
	
	PrintLN("Test PRINT Escape Sequences:");
	PRINT("\t<tab>Line 1\n\t<tab>Line 2\n\t<tab>Line 3"); // \t Horizontal Tab
	PRINT(text_CR);	// CR Carriage Return
	PRINT(">CR"); 
	PRINT(text_LF);	// LF line Feed
	
	//PRINT("\v");	// Place the cursor at the top of the screen
	
	cursorY = GetCursorRow(); 
	LOCATE(0,cursorY);
	DrawBox(columns, 7);
	
	cursorY++;
	LOCATE(1,cursorY++);
	PRINT("\1\x42 <-- extended graphic chars");  // print extended graphic characters (0x42 = smile)
	LOCATE(1,cursorY++);
	PRINT("\\ <-- Backslash");  // Backslash
	LOCATE(1,cursorY++);
	PRINT("\' <-- Single quotation mark");  // Single quotation mark
	LOCATE(1,cursorY++);
	PRINT("\" <-- Double quotation mark");         // \" Double quotation mark
	LOCATE(1,cursorY);
	PRINT("\? <-- Question mark");  // Question mark
	
	PressAnyKey();
	PrintLN("\f <-- Formfeed (clear screen)");	// Formfeed (CLS)
	PRINT("\a [Beep!]");  // Beep
	PressAnyKey();
}




/* =============================================================================
   Test the functions to print numbers. (PrintNumber and PrintFNumber)
============================================================================= */
void testPrintNumber(void)
{
	char charValue=42;
	unsigned int uintValue=12345;
	char columns = GetColumns();
	
	CLS();

	PrintLN("Test Print Numbers");
	PrintLine(columns);
	
	PrintLN("Variables:");
	
	PrintLN("char value = 42");
	PrintLN("uint value = 12345");
		
	PRINT(text_LF);
	
	PrintLine(columns);

	PRINT(">PrintNumber(7)   :");
	PrintNumber(7);
	
	PRINT("\n>PrintNumber(2400):"); 
	PrintNumber(2400);

	PRINT("\n>PrintNumber(uint):");
	PrintNumber(uintValue);

	PRINT("\n>PrintNumber(char):");
	PrintNumber(charValue);

	PRINT(text_LF);

	PRINT("\n>PrintFNumber(2400,32,6) :");
	PrintFNumber(2400,32,6); //" 2400" Testing when the length parameter is above the maximum size (5) 

	PRINT("\n>PrintFNumber(uint,32,5) :");
	PrintFNumber(uintValue,32,5); //"12345"

	PRINT("\n>PrintFNumber(12345,0,3) :");
	PrintFNumber(12345,0,3); //"345"

	PRINT("\n>PrintFNumber(7,0,3)     :");
	PrintFNumber(7,0,3); //"7"

	PRINT("\n>PrintFNumber(char,' ',3):");
	PrintFNumber(charValue,' ',3); //" 42"

	PRINT("\n>PrintFNumber(7,48,3)    :");
	PrintFNumber(7,48,3);  //"007"

	PRINT("\n>PrintFNumber(char,'0',4):");
	PrintFNumber(charValue,'0',4); //"0042"
	
	PRINT("\n>PrintFNumber(char,'_',5):");
	PrintFNumber(charValue,'_',5); //"___42"

	PressAnyKey();
}



/* =============================================================================
   Test the CLS (Clear Screen) function.
============================================================================= */
void testCLS(void)
{
	unsigned int i;
	unsigned int vaddr = 0;	
	unsigned int vsize = 80*24;
	boolean testResult=true;
	
	SCREEN0();
	
	LOCATE(0,0);
	PRINT(text10);
	WAIT(100);
	
	CLS();
		
	for(i=0;i<vsize;i++) if(VPEEK(vaddr++)!=32) testResult=false;
		
	LOCATE(0,0);
	PRINT(text10);
	PRINT(CheckResult[testResult]);	
}



/* =============================================================================
   Print a message on line 24 and wait for a key press.
============================================================================= */
void PressAnyKey(void)
{	
	LOCATE(0,23);
	PRINT(presskey);
	INKEY();	
}



/* =============================================================================
   Print an Extended Graphic char
============================================================================= */
void PrintExtendedGFXchar(char A)
{
	bchput(1);
	bchput(A);
}



/* =============================================================================
   Print a line.
============================================================================= */
void PrintLine(char size)
{
	char i;
	
	for(i=0;i<size;i++) PrintExtendedGFXchar(0x57);	
}



/* =============================================================================
   Prints a box from the cursor position.
============================================================================= */
void DrawBox(char width, char height)
{
	char i;
	char box_winside = width-2;
	
	char x = GetCursorColumn();
	char y = GetCursorRow();
	
/*  PrintLN("\1\x58\1\x57\1\x57\1\x59");
	PrintLN("\1\x56  \1\x56");
	PrintLN("\1\x5A\1\x57\1\x57\1\x5B");*/
	
	PrintExtendedGFXchar(0x58);
	PrintLine(box_winside);
	PrintExtendedGFXchar(0x59);
	
	width--;
	
	for(i=y+1;i<y+(height-1);i++)
	{
		LOCATE(x,i);
		PrintExtendedGFXchar(0x56);
		LOCATE(x+width,i);
		PrintExtendedGFXchar(0x56);
	}
	
	LOCATE(x,y+height-1);
	PrintExtendedGFXchar(0x5A);
	PrintLine(box_winside);
	PrintExtendedGFXchar(0x5B);	
}


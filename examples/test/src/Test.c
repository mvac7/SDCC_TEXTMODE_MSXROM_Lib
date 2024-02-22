/* =============================================================================
	Test TEXTMODE MSX ROM Library (fR3eL Project)
	Version: 1.3 (25/11/2023)
	Author: mvac7/303bcn
	Architecture: MSX
	Format: ROM
	Programming language: C and Assembler
	Compiler: SDCC 4.3 or newer
	Description:
		Test Textmode Library Text 1 (40 columns) and Graphic 1 (32 columns) modes.

	History of versions:
	- v1.3 (25/11/2023) update to SDCC (4.1.12) Z80 calling conventions
	- v1.2 (6/04/2018)
	- v1.1 (27/02/2017)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"

#include "../include/textmode_MSX.h"



#define  HALT __asm halt __endasm   //wait for the next interrupt


// Function Declarations -------------------------------------------------------
char PEEK(uint address);

char VPEEK(uint address);

char INKEY(void);

void WAIT(uint cicles);

void test_SC0(void);
void test_SC1(void);

void testWIDTH(void);
void testPRINT(void);
void testPrintNumber(void);
void testCLS(void);

void PressAnyKey(void);



// constants  ------------------------------------------------------------------
const char text01[] = "Test Textmode Lib v1.4";

const char text_32col[] = "----5----1----1----2----2----3--         0    5    0    5    0  ";
const char text_40col[] = "----5----1----1----2----2----3----3----4         0    5    0    5    0    5    0";

const char text_LF[] = "\n"; // LF line Feed
const char text_CR[] = "\r"; // CR Carriage Return

const char testString[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

const char presskey[] = "Press any key to continue";
// global variable definition --------------------------------------------------




// ------------------------------------------------------------------- Functions


void main(void)
{
	COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
	WIDTH(32);
	SCREEN1();

	LOCATE(3,11);
	PRINT(text01);
	//LOCATE(7,11);
	//PRINT(text02);

	PressAnyKey(); 
	  
	test_SC0();

	test_SC1();

	CLS();
	PRINT("END");
	PressAnyKey();  
}



char PEEK(uint address) __naked
{
address;
__asm

	ld   A,(HL)

	ret
__endasm;
}



char VPEEK(uint address) __naked
{
address;
__asm
	jp BIOS_RDVRM
__endasm;
}



/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY(void) __naked
{
__asm   
	jp  BIOS_CHGET
__endasm;
}



// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
	uint i;
	for(i=0;i<cicles;i++) HALT;
}




// ############################################################### TEST functions

void test_SC0(void)
{
	COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);
	WIDTH(40);
	SCREEN0();

	LOCATE(0,0);  
	PrintLN(">Test SCREEN0()");

	testWIDTH();

	testPRINT();

	testPrintNumber();

	testCLS();
	LOCATE(0,17);
	if (VPEEK(0x0000)==62) PRINT(">>> ERROR"); 
	else PRINT(">> OK");

	PressAnyKey();
}



void test_SC1(void)
{
	COLOR(WHITE,LIGHT_BLUE,DARK_BLUE);
	WIDTH(32);
	SCREEN1();  

	LOCATE(0,0);
	PrintLN(">Test SCREEN1()");

	testWIDTH();

	testPRINT();

	testPrintNumber();

	testCLS();
	LOCATE(0,17);
	if (VPEEK(0x1800)==62) PRINT(">>> ERROR");  // ">"=62
	else PRINT(">> OK");

	PressAnyKey();
}



void testWIDTH(void)
{
	char columns;

	columns = PEEK(LINLEN);
    
	PRINT(">Test WIDTH(");
	PrintNumber(columns);
	PrintLN(")");

	PRINT(text_LF);

	if(columns<40) PrintLN(text_32col);  
	else PrintLN(text_40col);
}



void testPRINT(void)
{
	PrintLN(">Test PRINT and PrintLN");
	PRINT(testString);
	PrintLN("\n\n>Test PRINT Escape Sequences:");
	PRINT("\tLine 1\n\tLine 2\n\tLine 3"); // \t Horizontal Tab
	PRINT(text_CR);	// CR Carriage Return
	PRINT(">"); 
	PRINT(text_LF);	// LF line Feed

	PRINT("\\ <-- print Backslash\n");  // Backslash

	PRINT("\1\x42 <-- extended graphic chars");  // print extended graphic characters (0x42 = smile)

	//Draw a box
	/*  PrintLN("\1\x58\1\x57\1\x57\1\x59");
	PrintLN("\1\x56  \1\x56");
	PrintLN("\1\x5A\1\x57\1\x57\1\x5B");*/

	PRINT(text_LF);	// Newline

	//PRINT("\v");	// Place the cursor at the top of the screen
	PrintLN("\' <-- Single quotation mark");  // Single quotation mark
	PrintLN("\" <-- Double quotation mark");         // \" Double quotation mark
	PrintLN("\? <-- Question mark");  // Question mark
	PRINT("\a [Beep!]");  // Beep

	PressAnyKey();
	PRINT("\f <-- Formfeed (CLS)");	// Formfeed (CLS)
	PressAnyKey();
}



void testPrintNumber(void)
{
	unsigned int uintvalue=12345;
	char charValue=71;

	CLS();

	PRINT(">Test PrintNumber(2400):"); 
	PrintNumber(2400);

	PRINT("\n>Test PrintNumber(uint):");
	PrintNumber(uintvalue);

	PRINT("\n>Test PrintNumber(char):");
	PrintNumber(charValue);

	PRINT("\n>Test PrintNumber(0)   :");
	PrintNumber(0);

	PRINT("\n\n");

	PRINT(">Test PrintFNumber(2400,32,6):\n");
	PrintFNumber(2400,32,6); //" 2400" Testing when the length parameter is above the maximum size (5) 

	PRINT("\n>Test PrintFNumber(uint,32,5):\n");
	PrintFNumber(uintvalue,32,5); //"12345"

	PRINT("\n>Test PrintFNumber(12345,0,3):\n");
	PrintFNumber(12345,0,3); //"345"

	PRINT("\n>Test PrintFNumber(71,0,3):\n");
	PrintFNumber(71,0,3); //"71"

	PRINT("\n>Test PrintFNumber(char,' ',3):\n");
	PrintFNumber(charValue,' ',3); //" 71"

	PRINT("\n>Test PrintFNumber(71,48,3):\n");
	PrintFNumber(7,48,3);  //"077"

	PRINT("\n>Test PrintFNumber(char,'0',4):\n");
	PrintFNumber(charValue,'0',4); //"0071"

	PressAnyKey();
}



void testCLS(void)
{
	CLS();   //Comment this, to test an error. 
	LOCATE(0,16);
	PRINT(">Test CLS()");
}



void PressAnyKey(void)
{	
	LOCATE(0,23);
	PRINT(presskey);
	INKEY();	
}

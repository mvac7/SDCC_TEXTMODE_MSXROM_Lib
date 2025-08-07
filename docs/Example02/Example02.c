/* =============================================================================
# Example02.c

- Version: 1.0
- Architecture: MSX
- Format: MSX-DOS
- Programming language: C and Z80 assembler
- Compiler: SDCC 4.4

## Description:
	Simple example of the textmode_MSXDOS Library (fR3eL Project)
============================================================================= */
#include "textmode_MSX.h"

const char text01[] = "Example textmode_MSXDOS Lib\n";

char main(void)
{
	unsigned int uintValue=1234;
	char charValue=71;

	COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
	WIDTH(40);
	SCREEN0();

	PrintLN(text01);

	PRINT(">PRINT+\\n: ");
	PRINT("Line 1\n");
	
	PRINT(">PrintLN: ");
	PrintLN("Line 2");
	
	PrintLN("");					//print a new line (CR)

	PrintLN(">Print Extended Graphic Characters");
	PRINT("\1\x42");				//print smile (2 + 64) = 42 hexadecimal
	
	PrintLN("\n");					//print 2 Carriage Return (CR) with Line Feed (LF)

	PRINT(">PrintNumber:");
	PrintNumber(1024);

	PRINT("\n>PrintFNumber:");
	PrintFNumber(charValue,'0',4);	//"0071"

	PRINT("\n>Print Integer:");
	PrintFNumber(uintValue,32,5);	//" 1234"

	PRINT("\n>Print cut number:");
	PrintFNumber(uintValue,32,2);	//"34"
	
	PrintLN("\n");
	
	//Draw a box
	PrintLN("\1\x58\1\x57\1\x57\1\x57\1\x57\1\x59");
	PrintLN("\1\x56    \1\x56");
	PrintLN("\1\x5A\1\x57\1\x57\1\x57\1\x57\1\x5B");

	PrintLN("");
	PRINT("End");
	
	return 0;						//Exit to DOS
}

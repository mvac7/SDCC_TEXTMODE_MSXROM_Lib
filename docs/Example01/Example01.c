/* =============================================================================
# Example01.c

- Version: 1.0
- Architecture: MSX
- Format: 8K ROM
- Programming language: C and Z80 assembler
- Compiler: SDCC 4.4

## Description:
	Simple example of the textmode_MSXBIOS Library (fR3eL Project)
============================================================================= */
#include "textmode_MSX.h"

const char text01[] = "Example TEXTMODE Lib\n";
const char text02[] = "Press a key to continue";

void main(void)
{
	unsigned int uintValue=1234;
	char charValue=71;

	COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
	WIDTH(40);
	SCREEN0();

	PrintLN(text01);

	PRINT("PRINT:");
	PRINT("Line 1\n");
	
	PRINT("PrintLN:");
	PrintLN("Line 2");
	
	PrintLN("Line 3");

	PrintLN("");					//print a new line (CR)

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

	LOCATE(8,20);
	PRINT(text02);
    
// execute BIOS CHGET - One character input (waiting)
__asm call 0x009F __endasm;	
}

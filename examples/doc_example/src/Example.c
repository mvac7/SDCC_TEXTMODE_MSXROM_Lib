/* =====================================================
	Example TEXTMODE MSX ROM Library (fR3eL Project)
======================================================== */

#include "../include/textmode_MSX.h"

const char text01[] = "Example textmode Lib";

void main(void)
{
  unsigned int uintValue=1234;
  char charValue=71;
  
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
  WIDTH(40);
  SCREEN0();
  
  LOCATE(0,0);
  PRINT("Line 1\n");
  PrintLN("Line 2");
  PRINT("Line 3");
  
  PRINT("\n");
  
  PRINT("\n>PrintNumber:");
  PrintNumber(1024);
  
  PRINT("\n>PrintFNumber:");
  PrintFNumber(charValue,'0',4); //"0071"
  
  PRINT("\n>Print Integer:");
  PrintFNumber(uintValue,32,5); //" 1234"
  
  PRINT("\n>Print cut number:");
  PrintFNumber(uintValue,32,2); //"34"
  
  LOCATE(8,11);
  PRINT(text01);
  
__asm   
  call  0x009F ;BIOS CHGET One character input (waiting)
__endasm;
}	

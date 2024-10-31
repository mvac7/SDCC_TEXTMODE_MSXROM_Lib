/* =====================================================
   Example TEXTMODE MSX ROM Library (fR3eL Project)
======================================================== */

#include "../include/textmode_MSX.h"

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
  
  PRINT("Line 1\n");
  PrintLN("Line 2");
  PrintLN("Line 3\n");
    
  PRINT("\1\x42");   //print smile (2 + 64) = 42 hexadecimal
  PRINT("\n");
  
  PRINT("\n>PrintNumber:");
  PrintNumber(1024);
  
  PRINT("\n>PrintFNumber:");
  PrintFNumber(charValue,'0',4); //"0071"
  
  PRINT("\n>Print Integer:");
  PrintFNumber(uintValue,32,5); //" 1234"
  
  PRINT("\n>Print cut number:");
  PrintFNumber(uintValue,32,2); //"34"
  
  LOCATE(8,20);
  PRINT(text02);
    
__asm   
  call  0x009F ;BIOS CHGET One character input (waiting)
  rst   0
__endasm;
}	

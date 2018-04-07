/* =============================================================================
  Test > SDCC MSX ROM TEXTMODE Functions Library (object type)
  Version: 1.2 (6/04/2018) 
   
  Description:
    Test Textome Lib T1 40 columns and G1 32 columns modes.
    
  History of versions:
    v1.1 (27/02/2017)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxsystemvars.h"
#include "../include/msxBIOS.h"

#include "../include/textmode.h"



#define  HALT __asm halt __endasm   //wait for the next interrupt


// Function Declarations -------------------------------------------------------
char PEEK(uint address);
void POKE(uint address, char value);

char VPEEK(uint address);

char INKEY();

void WAIT(uint cicles);

void test_SC0();
void test_SC1();

void testWIDTH();
void testPRINT();
void testPrintNumber();
void testCLS();


// constants  ------------------------------------------------------------------
const char text01[] = "Test SDCC textmode ROM Lib";
const char text02[] = "v1.2 (6/04/2018)";

const char testString[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

const char presskey[] = "Press a key to continue";
// global variable definition --------------------------------------------------




// Functions -------------------------------------------------------------------


//
void main(void)
{
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
          
  SCREEN1();
  WIDTH(32);  
  
  LOCATE(3,10);
  PRINT(text01);
  LOCATE(7,11);
  PRINT(text02);  
  
  INKEY();
      
  test_SC0();
  test_SC1();
  
  CLS();
  PRINT("END");
  WAIT(30*5);  
}



char PEEK(uint address)
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   A,(HL)

  ld   L,A
  pop  IX
__endasm;
}



void POKE(uint address, char value)
{
address;value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   A,6(IX)
  ld   (HL),A

  pop  IX  
__endasm;
}



char VPEEK(uint address)
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX) 
   
  call RDVRM
  
  ld   L,A
  pop  IX
__endasm;
}



/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY(){
__asm   
   call CHGET
   ld   L,A
__endasm;
}



// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
  return;
}



void test_SC0()
{
 
  COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);      

  WIDTH(40);
  SCREEN0();
  
  LOCATE(0,0);  
  PRINT(">Test SCREEN0()\n\r");
  
  testWIDTH();
  
  testPRINT();
  testPrintNumber();

  testCLS();
  LOCATE(0,17);
  if (VPEEK(0x0000)==62) PRINT(">>> ERROR"); 
  else PRINT(">> OK");
  
  LOCATE(0,22);
  PRINT(presskey);
  INKEY();
  
  return;
}



// TEST ###############################################################
void test_SC1()
{

  COLOR(WHITE,LIGHT_BLUE,DARK_BLUE);      

  WIDTH(32);
  SCREEN1();  
  
  LOCATE(0,0);
  PRINT(">Test SCREEN1()\n\r");
  
  testWIDTH();
  
  testPRINT();
  testPrintNumber();
  
  testCLS();
  LOCATE(0,17);
  if (VPEEK(0x1800)==62) PRINT(">>> ERROR");  // ">"=62
  else PRINT(">> OK");
  
  LOCATE(0,22);
  PRINT("Press a key to exit");
  INKEY();
 
}



void testWIDTH()
{
  char columns;
  
  columns = PEEK(LINLEN);
  
  PRINT(">Test WIDTH(");
  PrintNumber(columns);
  PRINT(")\n");
  PRINT("----5---10---15---20---25---30---35---40\n");
  PRINT("\n");
}



void testPRINT()
{
  PRINT(">Test PRINT():\n");
  PRINT(testString);
  PRINT("\n\n>Test PRINT() with Escape Sequences:");
  PRINT("\n\tLine 1\n\tLine 2\n\tLine 3"); // \t Horizontal Tab
  PRINT("\r");  // Carriage Return
  PRINT(">\n");
  PRINT("\\");  // Backslash
  PRINT("\a");  // Beep
  PRINT("\n");  // Newline
  //PRINT("\f");  // Formfeed (CLS)
  //PRINT("\v");  // Place the cursor at the top of the screen
  PRINT("\'Single quotation mark\'\n");  // Single quotation mark
  PRINT("\"Double quotation mark\"\n");         // \" Double quotation mark
  PRINT("Question mark\?\n");  // Question mark
  
  LOCATE(0,22);
  PRINT(presskey);
  INKEY();
}




void testPrintNumber()
{
  uint uintvalue=12345;
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
  PrintFNumber(2400,' ',6); //" 2400" Testing when the length parameter is above the maximum size (5) 

  PRINT("\n>Test PrintFNumber(uint,32,5):\n");
  PrintFNumber(uintvalue,' ',5); //"12345"
  
  PRINT("\n>Test PrintFNumber(uint,32,3):\n");
  PrintFNumber(uintvalue,' ',3); //"345"
  
  PRINT("\n>Test PrintFNumber(7,48,3):\n");
  PrintFNumber(7,48,3);  //"007"

  PRINT("\n>Test PrintFNumber(char,' ',4):\n");
  PrintFNumber(charValue,' ',4); //"  71"
  
  PRINT("\n>Test PrintFNumber(char,'0',4):\n");
  PrintFNumber(charValue,'0',4); //"0071"

  LOCATE(0,22);
  PRINT(presskey);
  INKEY();
}



void testCLS()
{
  CLS();   //Comment this, to test an error. 
  LOCATE(0,16);
  PRINT(">Test CLS()");
}

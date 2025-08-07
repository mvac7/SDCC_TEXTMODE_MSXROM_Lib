/* =============================================================================
# MSX-DOS TEXTMODE Library (fR3eL Project)

- Version: 1.5 (22/10/2024)
- Author: mvac7/303bcn
- Architecture: MSX
- Format: SDCC Relocatable object file (.rel)
- Programming language: C and Z80 assembler
- Compiler: SDCC 4.4 or newer

## Description:
Library of functions for developing text-mode applications.
Supports the following display modes:
- Text 1 (screen 0, 40 columns) 
- Text 2 (screen 0, 80 columns) Requires MSX with V9938 and BIOS that 
								supports this mode.
- Graphic 1 (screen 1, 32 columns) 
 
This library uses MSX BIOS functions, called through interslot (CALSLT). 
It is designed to develop MSXDOS applications.

Use them for developing MSX applications using Small Device C Compiler (SDCC) 
compilator.

16-bit Integer to ASCII based on num2Dec16 by baze
http://baze.sk/3sc/misc/z80bits.html#5.1

## History of versions (dd/mm/yyyy):
- v1.5 (22/10/2024)
	- Update to SDCC (4.1.12) Z80 calling conventions,
	- Added functions: PrintLN, GetColumns, GetCursorRow and GetCursorColumn.
- v1.4 (04/09/2019) 
	- Integer printing functions improved (PrintNumber & PrintFNumber).
	- num2Dec16 becomes PrintFNumber
- v1.3 (29/08/2019) nakeds and PrintNumber improvements
- v1.2 (05/05/2018)
- v1.1 (27/02/2017)
============================================================================= */
#include "../include/textmode_MSX.h"

#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"



char PrintNumber_Digits;


/* =============================================================================
SCREEN0
Description: 
		Initialice TEXT 1 (40 columns) or TEXT 2 (80 columns) screen mode.
	   
		Note: 
		To set the T2 mode, you must first set 80 columns with the WIDTH
		function (only MSX computers with V9938 and BIOS that supports
		this mode).
Input:	-
Output:	-
============================================================================= */
void SCREEN0(void) __naked
{
__asm
	push IX

	ld   A,(#LINLEN)
	ld   (#LINL40),A   ;copy columns seting with WIDTH to LINL40 system var

	ld   IX,#BIOS_INITXT
$CallBIOS:
	ld   IY,(#EXPTBL-1)
	call BIOS_CALSLT

	ei    
	pop  IX
	ret  
__endasm;
}



/* =============================================================================
SCREEN1
Description: 
		Initialice GRAPHIC 1 screen mode (32 columns x 24 lines).
Input:	-
Output:	- 
============================================================================= */
void SCREEN1(void) __naked
{
__asm
	push IX

	ld   A,(#LINLEN)   ;get a last value set with WIDTH function 
	ld   (#LINL32),A   ;set system variable

	ld   IX,#BIOS_INIT32
	jp   $CallBIOS  
__endasm;
}



/* =============================================================================
WIDTH
Description: 
		Specifies the number of characters per line in text mode.

Input:	1 to 40 in T1 40 columns mode
		41 to 80 in T2 80 columns mode (only MSX with V9938 and a BIOS that 
										supports this mode)
		 1 to 32 in G1 mode
Output:	- 
============================================================================= */
void WIDTH(char columns) __naked
{
columns;
__asm

	ld   (#LINLEN),A

	ret
__endasm;  
}



/* =============================================================================
COLOR
Description:
		Specifies the colors of the foreground, background, and border area.
		Note: In TEST 1 mode the border color has no effect.
Input:	(char) ink (0 to 15)
		(char) background (0 to 15)
		(char) border (0 to 15)
Output:	- 
============================================================================= */
void COLOR(char ink, char background, char border)
{
ink;		//A
background;	//L
border;		//STack
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   (#FORCLR),A	//14c
	ld   A,L          	//5c
	ld   (#BAKCLR),A	//14c
	ld   A,4(IX)     	//21c
	ld   (#BDRCLR),A	//14c
//total ----------------->68c
  
	ld   IX,#BIOS_CHGCLR
	ld   IY,(#EXPTBL-1)
	call BIOS_CALSLT

	ei    
	pop  IX

__endasm;
}



/* =============================================================================
CLS
Description: 
		Clear Screen. Fill Pattern Name Table with 0x20 character.
Input:	-        
Output:	- 
============================================================================= */
void CLS(void) __naked
{
__asm
	push IX

	xor  A

	ld   IX,#BIOS_BCLS
	jp   $CallBIOS
__endasm;
}



/* =============================================================================
LOCATE
Description: 
	   Moves the cursor to the specified location.
	   
Input:	(char) Position X of the cursor. TEXT 1 (0 to 39) 
										 TEXT 2 (0 to 79)
										 GRAPHIC 1 (0 to 31)
		(char) Position Y of the cursor. (0 to 23)         
Output:   -
============================================================================= */
void LOCATE(char x, char y) __naked
{
x;y;
__asm
	push IX

	inc  A
	ld   H,A

	inc  L
	 
	ld   IX,#BIOS_POSIT
	jp   $CallBIOS
__endasm;
}



/* =============================================================================
PRINT
Description: 
		Displays a text string at the current cursor position.
		Use LOCATE function when you need to indicate a specific position.             
					
Input:	(char*) String    
Output:	-

Notes:
		Supports escape sequences:
		 \a (0x07)	- Beep
		 \b (0x08)	- Backspace. Cursor left, wraps around to previous line, 
					  stop at top left of screen.
		 \t (0x09)	- Horizontal Tab. Tab, overwrites with spaces up to next 
					  8th column, wraps around to start of next line, scrolls
					  at bottom right of screen.
		 \n (0x0A)	- Newline > Line Feed and Carriage Return (CRLF) 
					  Note: CR added in this Library.
		 \v (0x0B)	- Cursor home. Place the cursor at the top of the screen.
					  Warning: This does not correspond to Vertical Tab, 
					  standardized in C.
		 \f (0x0C)	- Formfeed. Clear screen and place the cursor at the top. 
		 \r (0x0D)	- CR (Carriage Return)
		
		 \" (0x22)	- Double quotation mark
		 \' (0x27)	- Single quotation mark
		 \? (0x3F)	- Question mark
		 \\ (0x5C)	- Backslash
		 
		 \xhh		- Print in the output the character/code given in the 
					 hexadecimal value (hh).
					 
		 \1\xHH		- Print Extended Graphic Characters. HH = character + 0x40
============================================================================= */
void PRINT(char* text) __naked
{
text;  
__asm
	push IX
	call PRNTXT$
    ei
	pop  IX
    ret
	
PRNTXT$:
	ld   IX,#BIOS_CHPUT
	ld   IY,(#EXPTBL-1)
  
PRNLOOP$:
	ld	 A,(HL)
	or	 A		;IF \0 (null terminating character)
	ret	 Z		;End of print

	inc	 HL
	
	cp   #0x0A    ;\n
	call Z,PRN_LFCR
	
	call BIOS_CALSLT
//	ei
	jr	 PRNLOOP$
	
PRN_LFCR:
	call BIOS_CALSLT     ;print /n
	ld	 A,#0x0D         ;for print /r
	ret
	
__endasm;
}



/* =============================================================================
PrintLN
Description: 
		Displays a text string at the current cursor position and adds a 
		new line (CRLF) at the end.
Input:	(char*) String    
Output:	-
============================================================================= */
void PrintLN(char* text) __naked
{
text;
__asm
	push IX
	
	call PRNTXT$
	
	ld	 A,#0x0A  ;\n
	call PRN_LFCR
	call BIOS_CALSLT
	
	ei
	pop  IX
	ret
__endasm;
}





/* =============================================================================
PrintNumber
Description: 
		Displays an unsigned integer at the current cursor position.
		
Input:	(unsigned int) or (char) numeric value          
Output:	-
============================================================================= */
void PrintNumber(unsigned int value) __naked
{
	value;
//  PrintFNumber(value,0,5); 
__asm
	push IX

// HL = value
	ld   D,#0
	ld   E,#5 
	call PRNUM$

	ei
	pop  IX
	ret



/* ------------------------------------------------ 
16-bit Integer to ASCII (decimal)
Based on num2Dec16 by baze
https://baze.sk/3sc/misc/z80bits.html#5.1 
 HL = value
 D  = zero/empty Char (0,32,48)
 E  = length
------------------------------------------------ */
PRNUM$:

  ld   A,#5
  ld   (_PrintNumber_Digits),A
  
  ld   IX,#BIOS_CHPUT
  ld   IY,(#EXPTBL-1)
  
//for a future version with negative numbers  
//if (HL<0) Print "-" 
//   ld   A,#45
//   call $Num4

  	
  ld   BC,#-10000
  call $Num1
  ld   BC,#-1000
  call $Num1
  ld   BC,#-100
  call $Num1
  ld   C,#-10
  call $Num1

//Last figure
  ld   C,B
  ld   D,#48		//"0"

//  call $Num1
//  ei
//  ret   ; END
    
$Num1:	
  ld   A,#47		//"0" ASCII code - 1
   
$Num2:
  inc  A
  add  HL,BC
  jr   C,$Num2
	
  sbc  HL,BC
	
  cp   #48			//"0" ASCII code    
  jr   NZ,$Num3		//if A!=48 then goto $Num3
	
  ld   A,D  //(DE)
  jr   $Num4


$Num3:
//change space for 0 zero ASCII code
  ld   D,#48
	
$Num4:
  push AF
  ld   A,(_PrintNumber_Digits)
  dec  A
  ld   (_PrintNumber_Digits),A
  cp   E  
  jr  NC,$next5

  pop  AF
  or   A
  ret  Z  ;only print A>0
  
  call BIOS_CALSLT
//  ei
  ret

$next5:
  pop  AF
  ret  
// ------------------------------------------------ 
  
__endasm;

}



/* =============================================================================
PrintFNumber
Description: 
		Displays an unsigned integer with formatting parameters, 
		at the current cursor position.
Input:	(unsigned int) or (char) numeric value
		(char) zero/empty Char: (0 = "", 32=' ', 48='0', etc.)
		(char) length: 1 to 5          
Output:	-
============================================================================= */
void PrintFNumber(unsigned int value, char emptyChar, char length) 
{
value;		//HL
emptyChar;	//Stack
length;		//Stack
  
__asm
	push IX
	ld   IX,#0
	add  IX,SP
  
// HL = value
	ld   D,4(IX)        ;emptyChar
	ld   E,5(IX)        ;length
	call PRNUM$

	ei
	pop  IX
		    
__endasm;
}



/* =============================================================================
bchput
Description: 
		Displays a character or executes control code.
		(MSX BIOS CHPUT)
Input:	(char) character number
Output:	-
============================================================================= */
void bchput(char value) __naked
{
value;	//A
__asm
  push IX
  
  ld   IX,#BIOS_CHPUT
  jp   $CallBIOS
__endasm;
}



/* =============================================================================
GetColumns
Description:
		Returns the number of columns in the current screen.
Input:	-
Output:	(char)
============================================================================= */
char GetColumns(void) __naked
{
__asm
  ld   A,(#LINLEN)
  ret
__endasm;
}



/* =============================================================================
GetCursorRow
Description:
		Returns the current row-position of the cursor.
Input:	-
Output:	(char) (0-23)
============================================================================= */
char GetCursorRow(void) __naked
{
__asm
  ld   A,(#CSRY)
  dec  A
  ret
__endasm;
}



/* =============================================================================
GetCursorColumn
Description:
		Returns the current column-position of the cursor.
Input:	-
Output:	(char)	TEXT 1 (0 to 39) 
				TEXT 2 (0 to 79)
				GRAPHIC 1 (0 to 31)
============================================================================= */
char GetCursorColumn(void) __naked
{
__asm
  ld   A,(#CSRX)
  dec  A
  ret
__endasm;
}



/* =============================================================================
 Displays the function keys
============================================================================= */
/*void KEYON(void) __naked
{
__asm
  push IX
   
  ld   IX,#BIOS_DSPFNK
  jp   $CallBIOS
__endasm;
}*/



/* =============================================================================
   Erase functionkey display
============================================================================= */
/*void KEYOFF(void) __naked
{
__asm
  push IX
   
  ld   IX,#BIOS_ERAFNK
  jp   $CallBIOS
__endasm;
}*/



//void SetKEY(char keyn,char* text);



/* =============================================================================
   Indicates whether Text 1 mode is active.
   Output:	1=Yes/True ; 0=No/False
============================================================================= */
/*
char isText1Mode(void)
{
	char *A;
	A=(unsigned int *) RG1SAV;
	if (*A&0b00010000) return 1; //Text 40col Mode
	return 0;	
}
*/


/* =============================================================================
 SetG1colors

 Description: 
			Assigns colors to a group of GRAPHIC1 tiles.
			MSX-DOS environment.
		   
 Input:		(char) Octet. Group of 8 tiles.
			(char) Ink color (0-15)
			(char) Background color (0-15)      
 Output:   -
============================================================================= */
/*void SetG1colors(char octet, char INKcolor, char BGcolor)
{
octet;		//A
INKcolor;	//L
BGcolor;	//Stack	
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   B,L	

	ld   HL,#0x2000
	ld   D,#0
	ld   E,A
	add  HL,DE

	//ld   C,4(IX)	//(21ts)
	ld   A,B
	add  A	//multiply x16
	add  A
	add  A
	add  A
	//or   C	//(5ts)
	or  4(IX)	//(21ts)

	ld   IX,#0x004D     ;MSX BIOS   WRTVRM Writes data in VRAM
	ld   IY,(#0xFCC0)   ;System var EXPTBL-1 (FCC1h-1) main BIOS-ROM slot address
	call 0x001C         ;MSX BIOS   CALSLT Executes inter-slot call
	ei

	pop  IX
__endasm;	
}*/

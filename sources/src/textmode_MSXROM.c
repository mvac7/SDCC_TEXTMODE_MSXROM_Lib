/* =============================================================================
	MSX ROM TEXTMODE Library (fR3eL Project)
	Version: 1.5 (24/02/2024)
	Author: mvac7/303bcn
	Architecture: MSX
	Format: C Object (SDCC .rel)
	Programming language: C and Z80 assembler
	Compiler: SDCC 4.3 or newer 

	Description:
	 Opensource library with functions in Text 1 (screen 0, 40 columns), 
	 Text 2 (screen 0, 80 columns), and Graphic 1 (screen 1, 32 columns) modes. 

	 It is designed to develop MSX applications using Small Device C Compiler 
	 (SDCC), although it is an opensource project. Feel free to use part or 
	 all of it to adapt it to other systems or development environments.

	 16-bit Integer to ASCII based on num2Dec16 by baze
	 http://baze.sk/3sc/misc/z80bits.html#5.1

	History of versions:
	- v1.5 (24/02/2024) bchput recovery, 
						add GetColumns, GetCursorRow and GetCursorColumn
	- v1.4 (24/11/2023) update to SDCC (4.1.12) Z80 calling conventions,
					    add PrintLN function, remove bchput,
					    and more improvements.
	- v1.3 (05/09/2019)
	- v1.2 (03/04/2018)
	- v1.1 (27/02/2017) 
============================================================================= */
#include "../include/textmode_MSX.h"

#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"



/* =============================================================================
 SCREEN0
 
 Description: 
			Initialice TEXT 1 (40 columns) or TEXT 2 (80 columns) screen mode.
		   
			Note: 
			To set the T2 mode, you must first set 80 columns with the WIDTH 
			function (only MSX computers with V9938 and BIOS that supports 
			this mode).
		   
 Input:    -
 Output:   -
============================================================================= */
void SCREEN0(void) __naked
{
__asm
 
  ld   A,(#LINLEN)
  ld   (#LINL40),A   ;copy columns seting with WIDTH to LINL40 system var
  
  jp   BIOS_INITXT

__endasm;
}



/* =============================================================================
 SCREEN1
 
 Description: 
           Initialice GRAPHIC 1 screen mode (32 columns x 24 lines).
 Input:    -
 Output:   -
============================================================================= */
void SCREEN1(void) __naked
{
__asm
  
  ld   A,(#LINLEN)   ;get a last value set with WIDTH function 
  ld   (#LINL32),A   ;set system variable

  jp   BIOS_INIT32
  
__endasm;
}



/* =============================================================================
 WIDTH
 
 Description: 
           Specifies the number of characters per line in text mode.
 Input:     1 to 40 in TEXT 1 mode (40 columns)
           41 to 80 in TEXT 2 mode (80 columns)
            1 to 32 in GRAPHIC 1 mode
 Output:   - 
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
			
  Input:    (char) ink (0 to 15)
			(char) background (0 to 15)
			(char) border (0 to 15)
============================================================================= */
void COLOR(char ink, char background, char border) 
{
ink;background,border;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   B,L         ; 5c
  ld   C,4(IX)     ;21c

  ld   HL,#FORCLR  ;11c
  ld   (HL),A      ; 8c
  inc  HL          ; 7c
  ld   (HL),B      ; 8c
  inc  HL          ; 7c
  ld   (HL),C      ; 8c
;total ------------>75c
  
;  ld   IY,#FORCLR  ; 16c 
;  ld   (IY),A      ; 21c
;  ld   1(IY),L     ; 21c
;  ld   A,4(IX)     ; 21c
;  ld   2(IY),A     ; 21c
;total-------------->100c  

  call BIOS_CHGCLR
  
  pop  IX

__endasm;
}



/* =============================================================================
  CLS
 
  Description: 
			Clear Screen. Fill Pattern Name Table with 0x20 character.
  Input:    -        
  Output:   -
============================================================================= */
void CLS(void) __naked
{
__asm
  xor  A
  jp   BIOS_BCLS
__endasm;
}



/* =============================================================================
 LOCATE
 
 Description: 
           Moves the cursor to the specified location.
		   
 Input:    (char) Position X of the cursor. TEXT 1 (0 to 39) 
											TEXT 2 (0 to 79)
											GRAPHIC 1 (0 to 31)
           (char) Position Y of the cursor. (0 to 23)         
 Output:   -
============================================================================= */
void LOCATE(char x, char y) __naked
{
x;
y;
__asm

  inc  A
  ld   H,A

  inc  L
  
  jp   BIOS_POSIT

__endasm;
}



/* =============================================================================
 PRINT
  
 Description: 
			Displays a text string in the last position where the cursor is.
			Use the LOCATE function when you need to indicate a specific position.
                        
 Input:    (char*) String    
 Output:   -
 
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
PRNLOOP$:
	ld   A,(hl)
	or   A
	ret	 Z

	inc	 HL
	
	cp   #0x0A       ;\n
	call Z,PRN_LFCR
	
	call BIOS_CHPUT
	jr   PRNLOOP$
	
PRN_LFCR:
;	ld	 A,#0x0a  ;\n
	call BIOS_CHPUT
	ld	 A,#0x0d  ;\r
;   call BIOS_CHPUT	
	ret
	
__endasm;
}



/* =============================================================================
 PrintLN
  
 Description: 
           Displays a text string in the last position where the cursor is 
		   and adds a new line (CRLF).   
                        
 Input:    (char*) String    
 Output:   -
============================================================================= */
void PrintLN(char* text) __naked
{
text;
__asm
    call PRNLOOP$
	ld	 A,#0x0a
	call PRN_LFCR
	jp   BIOS_CHPUT
__endasm;
}



/* =============================================================================
 PrintNumber

 Description: 
            Displays an unsigned integer in the last position where the cursor is.

			16-bit Integer to ASCII (decimal) based on num2Dec16 by baze
			https://baze.sk/3sc/misc/z80bits.html#5.1
			
 Input:    (unsigned int) or (char) numeric value          
 Output:   -
============================================================================= */
void PrintNumber(unsigned int value) __naked
{
value;	//HL 

//PrintFNumber(value,0,5);
__asm  
  ld   D,#0
  ld   E,#5
  
; ------------------------------------------------  
;  HL = value
;  D  = zero/empty Char (0,32,48)
;  E  = length
PRNUM$:

  ld   A,#5		;number Digit
  ex   AF,AF
  
;for a future version with negative numbers  
;if (HL<0) Print "-" 
;   ld   A,#45
;   call $Num4

  	
  ld   BC,#-10000
  call $Num1
  ld   BC,#-1000
  call $Num1
  ld   BC,#-100
  call $Num1
  ld   C,#-10
  call $Num1

;Last figure
  ld   C,B
  ld   D,#48          ;"0"

;  call $Num1
;  ret   ; END
    
$Num1:	
  ld   A,#47     ;"0" ASCII code - 1
   
$Num2:
  inc  A
  add  HL,BC
  jr   C,$Num2
	
  sbc  HL,BC
	
  cp   #48       ;"0" ASCII code
  jr   NZ,$Num3  ;if A!=48 then goto $Num3
	
  ld   A,D  ;(DE)
  jr   $Num4


$Num3:
  ;change space/empty for 0 zero ASCII code
  ld   D,#48
	
$Num4:
  ex   AF,AF
  dec  A  ;number Digit
  cp   E  ;length
  jr  NC,$next5  ;IF num.Digit>length THEN dont print

  ex   AF,AF
  or   A
  ret  Z  ;only print A>0
  
  jp   BIOS_CHPUT  ;Print digit

$next5:
  ex   AF,AF
  ret
  
__endasm;
}



/* =============================================================================
 PrintFNumber

 Description: 
			Displays an unsigned integer with formatting parameters, 
			in the last position where the cursor is.
		   
 Input:		(unsigned int) or (char) numeric value
			(char) zero/empty Char: (0 = "", 32=' ', 48='0', etc.)
			(char) length: 1 to 5          
 Output:   -
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
  
  ;HL = value
  
  ld   D,4(IX)        ;emptyChar
  ld   E,5(IX)        ;length
   
  call PRNUM$
  
  pop  IX
		    
__endasm;
}



/* =============================================================================
 bchput
 
 Description: 
			Displays a character or executes control code. 
			(MSX BIOS CHPUT)
 Input:		(char) character number
 Output:	-
============================================================================= */
void bchput(char value) __naked
{
value;	//A
__asm
  jp   BIOS_CHPUT
__endasm;
}



/* =============================================================================
 GetColumns
 
 Description:
			Provides the columns from current screen.
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
			Provides the current row-position of the cursor
 Input:		-
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
			Provides the current column-position of the cursor
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
  jp   BIOS_DSPFNK
__endasm;
}*/



/* =============================================================================
   Erase functionkey display
============================================================================= */
/*void KEYOFF(void) __naked
{
__asm
  jp   BIOS_ERAFNK
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
			Assigns colors to a group of GRAPHIC1 mode tiles.
		   
 Input:		(char) Octet. Group of 8 tiles.
			(char) Ink color (0-15)
			(char) Background color (0-15)      
 Output:   -
============================================================================= */
/*void SetG1colors(char octet, char INKcolor,char BGcolor)
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
	
	ld   C,4(IX)
	ld   A,B
	SLA  A
	SLA  A
	SLA  A
	SLA  A	
	or   C	
	
	call  0x004D	;MSX BIOS WRTVRM - Writes data in VRAM
	
	pop  IX
__endasm;	
}*/
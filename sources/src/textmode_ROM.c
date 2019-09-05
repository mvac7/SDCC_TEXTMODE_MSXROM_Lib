/* =============================================================================
  SDCC MSX ROM TEXTMODE Functions Library (object type)
  Version: 1.3 (5/9/2019)
  Author: mvac7/303bcn
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C + Assembler
  WEB: 
  mail: mvac7303b@gmail.com

  Description:
    Opensource library with functions to work in text mode in Screen 0 (40/80
    columns), and Screen 1. 
    
    Use them for developing MSX applications using Small Device C Compiler  
    (SDCC) compilator.
    
    16-bit Integer to ASCII based on num2Dec16 by baze
    http://baze.sk/3sc/misc/z80bits.html#5.1
    
  History of versions:
  - v1.3 (05/09/2019)
  - v1.2 (03/04/2018)
  - v1.1 (27/02/2017) 
============================================================================= */
#include "../include/textmode.h"

#include "../include/msxsystemvars.h"
#include "../include/msxBIOS.h"


char CSTATE;
char FNUMSIZE;


/* =============================================================================
  SCREEN0
 
  Description: 
           Switch to T1 or T2 mode (SCREEN 0), 40 or 80 columns x 24 lines.
           Notice: To set the T2 mode, you must first set 80 columns with the 
           WIDTH instruction.
  Input:    -
  Output:   -
============================================================================= */
void SCREEN0() __naked
{
__asm
 
  ld   A,(#LINLEN)
  ld   (#LINL40),A   ;copy columns seting with WIDTH to LINL40 system var
                     
  call INITXT
  ret
__endasm;
}



/* =============================================================================
  SCREEN1
 
  Description: 
           Switch to G1 mode (SCREEN 1), 32 columns x 24 lines.
  Input:    -
  Output:   -
============================================================================= */
void SCREEN1() __naked
{
__asm
  
  ld   A,(#LINLEN)   ;get a last value set with WIDTH function 
  ld   (#LINL32),A   ;set system variable
     
  call INIT32
  ret
__endasm;
}



/* =============================================================================
  WIDTH
 
  Description: 
            Specifies the number of characters per line in text mode.
  Input:     1 to 40 in T1 40 columns mode
            41 to 80 in T2 80 columns mode (only MSX with V9938 and a BIOS that 
                                            supports this mode)
             1 to 32 in G1 mode 
============================================================================= */
void WIDTH(char columns) __naked
{
columns;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)
  ld   (#LINLEN),A
    
  pop  IX
  ret
__endasm;  
}



/* =============================================================================
  COLOR
 
  Description: 
            Specifies the colors of the foreground, background, and border area.
  Input:    (char) ink (0 to 15)
            (char) background (0 to 15)
            (char) border (0 to 15)
============================================================================= */
void COLOR(char ink, char background, char border) __naked
{
ink;background,border;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   IY,#FORCLR
  ld   A,4(IX)
  ld   (IY),A

  ld   A,5(IX)
  ld   1(IY),A
  
  ld   A,6(IX)
  ld   2(IY),A 
  
  call CHGCLR
  
  pop  IX
  ret
__endasm;
}



/*
;detect screen 0 or screen 1
  ld   A,(#RG1SAV)  ;mira el valor guardado en las variables de sistema del registro 1 del VDP
  bit  4,A
  jr   NZ,SCR0    ;IF A=1 THEN is screen 0 

  jr  END
SCR0:

*/


/* =============================================================================
  CLS
 
  Description: 
            Clear the contents of the screen.
  Input:    -        
  Output:   -
============================================================================= */
void CLS() __naked
{
__asm
  xor  A
  call BCLS
  ret
__endasm;
}



/* =============================================================================
  LOCATE
 
  Description: 
            Moves the cursor to the specified location.
  Input:    (char) Position X of the cursor. (0 to 31 or 79)
            (char) Position Y of the cursor. (0 to 23)         
  Output:   -
============================================================================= */
void LOCATE(char x, char y) __naked
{
x;y;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX) ;x
  inc  A       ;incrementa las posiciones para que se situen correctamente en la pantalla
  ld   H,A
  ld   A,5(IX) ;y
  inc  A
  ld   L,A   
  call POSIT
  
  pop  IX
  ret
__endasm;

}



/* =============================================================================
  PRINT
  
  Description: 
           Displays a text string on the screen.
           
           Supports escape secuences:
             \n - Newline > Line Feed and Carriage Return (CRLF) 
             \r - Carriage Return
             \t - Horizontal Tab
             \a - Beep
             \f - Formfeed. Clear screen and place the cursor at the top.
             \\ - Backslash
             \' - Single quotation mark
             \" - Double quotation mark
             \? - Question mark
             
             \v - Place the cursor at the top of the screen
                  Warning: This does not correspond to Vertical Tab, 
                           standardized in C.
                        
  Input:    (char*) String    
  Output:   -
============================================================================= */
void PRINT(char* text)
{
  char character;
  
  while(*(text)) 
  {
    character=*(text++);
    if (character=='\n')
    {
      bchput(10); //LF (Line Feed)
      bchput(13); //CR (Carriage Return)
    }else{ 
      bchput(character);
    } 
  }
}



/*void printf(char* text)
{
  PRINT(text);
  
  bchput(10);
  bchput(13); 
}*/



/* =============================================================================
  PrintNumber

  Description: 
            Prints an unsigned integer on the screen.  
  Input:    (unsigned int) numeric value          
  Output:   -
============================================================================= */
void PrintNumber(unsigned int value)
{
  PrintFNumber(value,0,5);   
}



/* =============================================================================
  PrintFNumber

  Description: 
           Prints an unsigned integer on the screen with formatting parameters.
  Input:   (unsigned int or char) numeric value
           (char) empty Char: (32=' ', 48='0', etc.)
           (char) length: 1 to 5          
  Output:   -
============================================================================= */
void PrintFNumber(unsigned int value, char emptyChar, char length) __naked
{
  value;       //to avoid warnings
  emptyChar;
  length;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(ix)
  ld   H,5(ix)
 
  ld   A,6(ix)        ;empty char
  ld   (#_CSTATE),A
  
  ld   D,7(ix)        ;length

  ld   IY,#_FNUMSIZE  ;variable for print size control
  ld   (IY),#5        ;init
  

;for a future version with negative numbers  
;if (HL<0) Print "-" 
;   ld   A,#45
;   call $Num4

  	
  ld   BC,#-10000
	call $Num1
	ld   BC,#-1000
	call $Num1
	ld	 BC,#-100
	call $Num1
	ld	 C,#-10
	call $Num1
  ;Last figure
	ld	 C,B
  ld   A,#48          ;"0"
  ld   (#_CSTATE),A
	call $Num1

;END
  pop  IX
  ret
    
$Num1:	
  ld	 A,#47     ;"0" ASCII code - 1
   
$Num2:
  inc	 A
	add	 HL,BC
	jr	 C,$Num2
	
	sbc	 HL,BC
	
	cp   #48       ;"0" ASCII code    
	jr   NZ,$Num3  ;if A!=48 then goto $Num3
	
	ld   A,(#_CSTATE)
  jr   $Num4


$Num3:
  ;change space for 0 zero ASCII code
  ex   AF,AF
  ld   A,#48
  ld   (#_CSTATE),A
  ex   AF,AF	
	
$Num4:
  ld   E,A
;  ld   IY,#_FNUMSIZE
  dec  (IY)
  ld   A,(IY)
  cp   D
  ret  NC
  
  ld   A,E
  or   A
  ret  Z  ;only print A>0
  
  jp   CHPUT
		
;	ret
    
__endasm;
}



/* =============================================================================
  bchput
  
  Description: Displays one character (MSX BIOS CHPUT)
  Input:    (char) - char value          
  Output:   -
============================================================================= */
void bchput(char value) __naked
{
value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)   
  call CHPUT
  
  pop  IX
  ret
__endasm;
}



/* =============================================================================
   Current row-position of the cursor
============================================================================= */
/*char getLine()
{
__asm
  ld   HL,#CSRY
  ld   L,(HL)
__endasm;
}*/


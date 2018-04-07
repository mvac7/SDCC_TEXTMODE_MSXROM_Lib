/* =============================================================================
  SDCC MSX ROM TEXTMODE Functions Library (object type)
  Version: 1.2 (3/4/2018)
  Author: mvac7/303bcn
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C
  WEB: 
  mail: mvac7303b@gmail.com

  Description:
    Opensource library with functions to work in text mode in Screen 0 and 
    Screen 1. 
    
  History of versions:
    v1.1 (27/2/2017) 
============================================================================= */
#include "../include/textmode.h"

#include "../include/msxsystemvars.h"
#include "../include/msxBIOS.h"


char CSTATE;



/* =============================================================================
  SCREEN0
 
  Description: 
           Switch to T1 or T2 mode (SCREEN 0), 40 or 80 columns x 24 lines.
  Input:    -
  Output:   -
============================================================================= */
void SCREEN0()
{
__asm
 
  ld   A,(#LINLEN)
  ld   (#LINL40),A   ;copy columns seting with WIDTH to LINL40 system var
                     
  call INITXT
  
__endasm;
}



/* =============================================================================
  SCREEN1
 
  Description: 
           Switch to G1 mode (SCREEN 1), 32 columns x 24 lines.
  Input:    -
  Output:   -
============================================================================= */
void SCREEN1()
{
__asm
  
  ld   A,(#LINLEN)   ;get a last value set with WIDTH function 
  ld   (#LINL32),A   ;set system variable
     
  call INIT32
  
__endasm;
}



/* =============================================================================
  WIDTH
 
  Description: 
            Specifies the number of characters per line in text mode.
  Input:     1 to 40 in T1 40 columns mode
            41 to 80 in T2 80 columns mode (only MSX with V9938 and BIOS support 
                                           this mode)
             1 to 32 in G1 mode 
============================================================================= */
void WIDTH(char columns)
{
columns;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)
  ld   (#LINLEN),A
    
  pop  IX
__endasm;  
}



/* =============================================================================
  COLOR
 
  Description: 
            Specifies the colors of foreground, background, and border area.
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
  
  ld   IY,#FORCLR
  ld   A,4(IX)
  ld   (IY),A

  ld   A,5(IX)
  ld   1(IY),A
  
  ld   A,6(IX)
  ld   2(IY),A 
  
  call CHGCLR
  
  pop  IX
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
void CLS()
{
__asm
  xor  A
  call BCLS
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
void LOCATE(char x, char y)
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
                  Warning!: Does not correspond to Vertical Tab, of standardized 
                            use in C.
                        
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
  char character;
  char text[]="     ";
  char *p;	

  num2Dec16(value, text,32); 
  
  p = text;  
  
  while(*(p))
  {
    character=*(p++);
    if (character!=32) bchput(character);
  }   
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
void PrintFNumber(unsigned int value, char emptyChar, char length)
{
  char pos=5;
  char text[]="     ";

  num2Dec16(value, text,emptyChar); //32=space, 48=zero 
  
  if(length>5) length=5;
  if(length==0) length=5;
  //coloca el puntero en la posicion donde se ha de empezar a mostrar 
  pos-=length;
  
  // muestra el numero en la pantalla
  while (length-->0){ bchput(text[pos++]);}
}



/* =============================================================================
  num2Dec16
 
  Description: 
            16-bit Integer to ASCII (decimal)
            Based on the code by baze.
  Input:    (unsigned int) a number
            (char*) Address where the output string is provided.
            (char) empty Char: 32=space, 48=zero
  Output:   -
============================================================================= */
void num2Dec16(unsigned int aNumber, char *address, char emptyChar)
{
  aNumber;
  address;
  emptyChar;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(ix)
  ld   H,5(ix)
  
;if (HL<0) Print "-" 
  
  ld   E,6(ix)
  ld   D,7(ix)
  
  ld   A,8(ix)   ;#32
  ld   (#_CSTATE),A
  	
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
  ld   A,#48
  ld   (#_CSTATE),A
	call $Num1

;END
  pop  IX
  ret
    
$Num1:	
  ld	 A,#47 ; 0 ASCII code - 1
   
$Num2:	
  inc	 A
	add	 HL,BC
	jr	 C,$Num2
	
	sbc	 HL,BC
	
	cp   #48   ;"0" ASCII code    
	jr   NZ,$Num3  ;if A!=48 then goto $Num3
	
	ld   A,(#_CSTATE)
  jr   $Num4


$Num3:
  ;change space for 0 zero ASCII code
  push AF
  ld   A,#48
  ld   (#_CSTATE),A
  pop  AF	
	
$Num4:	
	ld	 (DE),A   ;save a ascii code
	inc	 DE
		
	ret
  
$Num5:
    
__endasm;
}



/* =============================================================================
  bchput
  
  Description: Displays one character (MSX BIOS CHPUT)
  Input:    (char) - char value          
  Output:   -
============================================================================= */
void bchput(char value)
{
value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)   
  call CHPUT
  
  pop  IX
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


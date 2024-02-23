/* =============================================================================
  MSX TEXTMODE Library (fR3eL Project)

  Description:
	 Library with functions to work in text mode in Screen 0 (40/80 columns), 
	 and Screen 1. 

	 It is designed to develop MSX applications using Small Device C Compiler 
	 (SDCC), although it is an opensource project. Feel free to use part or 
	 all of it to adapt it to other systems or development environments.

============================================================================= */
#ifndef __TEXTMODE_H__
#define __TEXTMODE_H__



// palette color codes
#ifndef _TMS_COLORS
#define _TMS_COLORS
#define TRANSPARENT   0
#define BLACK         1
#define GREEN         2
#define LIGHT_GREEN   3
#define DARK_BLUE     4
#define LIGHT_BLUE    5
#define DARK_RED      6
#define CYAN          7
#define RED           8
#define LIGHT_RED     9
#define DARK_YELLOW  10
#define LIGHT_YELLOW 11
#define DARK_GREEN   12
#define MAGENTA      13
#define GRAY         14
#define GREY         14
#define WHITE        15
#endif


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
void SCREEN0(void);



/* =============================================================================
 SCREEN1
 
 Description: 
           Initialice GRAPHIC 1 screen mode (32 columns x 24 lines).
 Input:    -
 Output:   -
============================================================================= */
void SCREEN1(void);



/* =============================================================================
 WIDTH
 
 Description: 
           Specifies the number of characters per line in text mode.
 Input:     1 to 40 in TEXT 1 mode (40 columns)
           41 to 80 in TEXT 2 mode (80 columns)
            1 to 32 in GRAPHIC 1 mode
 Output:   - 
============================================================================= */
void WIDTH(char columns);



/* =============================================================================
  COLOR
 
  Description: 
			Specifies the colors of the foreground, background, and border area.
			Note: In TEST 1 mode the border color has no effect. 
			
  Input:    (char) ink (0 to 15)
			(char) background (0 to 15)
			(char) border (0 to 15)
============================================================================= */
void COLOR(char ink, char background, char border);



/* =============================================================================
 CLS
 
 Description: 
           Clear Screen. Fill Pattern Name Table with 0x20 character.
 Input:    -        
 Output:   - 
============================================================================= */
void CLS(void);



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
void LOCATE(char x, char y);



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
void PRINT(char* text);


/* =============================================================================
 PrintLN
  
 Description: 
           Displays a text string in the last position where the cursor is 
		   and adds a new line (CRLF).   
                        
 Input:    (char*) String    
 Output:   -
============================================================================= */
void PrintLN(char* text);



/* =============================================================================
 PrintNumber

 Description: 
            Displays an unsigned integer in the last position where the cursor is.

			16-bit Integer to ASCII (decimal) based on num2Dec16 by baze
			https://baze.sk/3sc/misc/z80bits.html#5.1
			
 Input:    (unsigned int) numeric value          
 Output:   -
============================================================================= */
void PrintNumber(unsigned int value);




/* =============================================================================
 PrintFNumber

 Description: 
			Displays an unsigned integer with formatting parameters, 
			in the last position where the cursor is.
		   
 Input:		(unsigned int or char) numeric value
			(char) zero/empty Char: (0 = "", 32=' ', 48='0', etc.)
			(char) length: 1 to 5          
 Output:   -
============================================================================= */
void PrintFNumber(unsigned int value, char emptyChar, char length);




/* =============================================================================
 bchput
 
 Description: 
         Displays one character (MSX BIOS CHPUT).
 Input:   (char) text char
 Output:  -
============================================================================= */
//void bchput(char value);



/* =============================================================================
   Current row-position of the cursor
============================================================================= */
//char GetRow(void)



/* =============================================================================
Current column-position of the cursor
============================================================================= */
//char GetColumn(void)



/* =============================================================================
   Displays the function keys
============================================================================= */
//void KEYON(void)



/* =============================================================================
   Erase functionkey display
============================================================================= */
//void KEYOFF(void)



/* =============================================================================
   Indicates whether Text 1 mode is active.
   Output:	1=Yes/True ; 0=No/False
============================================================================= */
//char isText1Mode(void)


#endif
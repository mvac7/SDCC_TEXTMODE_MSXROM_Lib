# How to use the MSX SDCC Library

| Attention! |
| :---       |
| The following document has been written using an English translator.<br/>You can participate in the improvement of this document, transmitting your corrections or suggestions in the issues of this project or the main fR3eL project.<br/>Thanks for understanding. |

<br/>

## Index

- [1 Description](#1-Description)
- [2 Requirements](#2-Requirements)
- [3 Definitions](#3-Definitions)
    - [3.1 Color Names](#31-Color-Names)
- [4 Functions](#4-Functions)
	- [4.1 WIDTH](#41-WIDTH)
	- [4.2 COLOR](#42-COLOR)
	- [4.3 SCREEN0](#43-SCREEN0)
	- [4.4 SCREEN1](#44-SCREEN1)
	- [4.5 CLS](#45-CLS)
	- [4.6 LOCATE](#46-LOCATE)
	- [4.7 PRINT](#47-PRINT)
	- [4.8 PrintLN](#48-PrintLN)
	- [4.9 PrintNumber](#49-PrintNumber)
	- [4.10 PrintFNumber](#410-PrintFNumber)
	- [4.11 bchput](#411-bchput)
	- [4.12 GetColumns](#412-GetColumns)
	- [4.13 GetCursorRow](#413-GetCursorRow)
	- [4.14 GetCursorColumn](#414-GetCursorColumn)
- [5 How does it work?](#5-How-does-it-work)
	- [5.1 TEXT 1 and TEXT 2 modes](#51-TEXT-1-and-TEXT-2-modes)
	- [5.2 GRAPHIC 1 mode](#52-GRAPHIC-1-mode)
	- [5.2.1 Set GRAPHIC 1 Colors](#521-Set-GRAPHIC-1-Colors)
	- [5.3 More things](#53-More-things)
- [6 Code Example](#6-Code-Example)
- [7 Appendices](#7-Appendices)
    - [7.1 Escape sequences](#71-Escape-sequences)
	- [7.2 New line](#72-New-line)
	- [7.3 Other scape codes](#73-Other-scape-codes)
	- [7.4 Extended Graphic Characters](#74-Extended-Graphic-Characters)
- [8 References](#8-References)

<br/>

---

## 1 Description

C function library with functions for developing text-mode applications.
Includes functions for screen initialization and printing of texts and numbers.

Supports the following display modes:
- Text1 (Screen 0, 40 columns) 
- Text2 (Screen 0, 80 columns) Requires MSX with V9938 and BIOS that supports this mode.
- Graphic1 (Screen 1, 32 columns)

In this project you will find two libraries for different environments:
- **textmode_MSXBIOS** Uses the MSX BIOS. It takes up very little memory. You can use it to develop applications in ROM format or programs that run from MSX BASIC environment.
- **textmode_MSXDOS** Uses the MSX BIOS functions via inter-slot call (CALSLT). You can use it to develop applications for the MSX-DOS environment.

This library is designed to develop MSX applications using Small Device C Compiler [`SDCC`](http://sdcc.sourceforge.net/).

These libraries are part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

This project is open source under the [MIT license](LICENSE).
You can add part or all of this code in your application development or include it in other libraries/engines.

<br/>

---

## 2 Requirements

- [Small Device C Compiler (SDCC) v4.4](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)


<br/>

---

## 3 Definitions

### 3.1 Color Names

Label		| Value
:---		| ---:
TRANSPARENT	| 0
BLACK		| 1
GREEN		| 2
LIGHT_GREEN	| 3
DARK_BLUE	| 4
LIGHT_BLUE	| 5
DARK_RED	| 6
CYAN		| 7
RED			| 8
LIGHT_RED	| 9
DARK_YELLOW	| 10
LIGHT_YELLOW	| 11
DARK_GREEN	| 12
MAGENTA		| 13
GRAY		| 14
GREY		| 14
WHITE		| 15


<br/>

---


## 4 Functions


### 4.1 WIDTH

<table>
<tr><td colspan=3><b>WIDTH</b></td></tr>
<tr><td colspan=3>Specifies the number of characters per line in text mode.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>WIDTH(columns)</td></tr>
<tr><td rowspan=3>Input</td><td rowspan=3>[char]</td><td>1 to 40 in TEXT 1 mode (40 columns)</td></tr>
<tr><td>41 to 80 in TEXT 2 mode (80 columns)</td></tr>
<tr><td>1 to 32 in GRAPHIC 1 mode</td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>WIDTH(24);<br/>SCREEN1();</pre></td></tr>
</table>

<br/>

### 4.2 COLOR

<table>
<tr><td colspan=3><b>COLOR</b></td></tr>
<tr><td colspan=3>Specifies the colors of the foreground, background, and border area.<br/><b>Note:</b> In TEXT 1 mode the border color has no effect.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>COLOR(ink, background, border)</td></tr>
<tr><td rowspan=3><b>Input</b></td><td>[char]</td><td>Ink color (0 to 15)</td></tr>
<tr><td>[char]</td><td>Background color (0 to 15)</td></tr>
<tr><td>[char]</td><td>Border color (0 to 15)</td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>COLOR(BLACK,GRAY,DARK_BLUE);</pre></td></tr>
</table>

<br/>

### 4.3 SCREEN0

<table>
<tr><td colspan=3><b>SCREEN0</b></td></tr>
<tr><td colspan=3>Initialice TEXT 1 (40 columns) or TEXT 2 (80 columns) screen mode.<br/><b>Note:</b> To set the TEXT 2 mode, you must first set 80 columns with the WIDTH function (only MSX computers with V9938 and BIOS that supports this mode).</td></tr>
<tr><td><b>Function</b></td><td colspan=2>SCREEN0()</td></tr>
<tr><td><b>Input</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>WIDTH(40);<br/>SCREEN0();</pre></td></tr>
</table>

<br/>

### 4.4 SCREEN1

<table>
<tr><td colspan=3><b>SCREEN1</b></td></tr>
<tr><td colspan=3>Initialice GRAPHIC 1 screen mode (32 columns x 24 lines).</td></tr>
<tr><td><b>Function</b></td><td colspan=2>SCREEN1()</td></tr>
<tr><td><b>Input</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>COLOR(14,4,5);<br/>WIDTH(32);<br/>SCREEN1();</pre></td></tr>
</table>

<br/>

### 4.5 CLS

<table>
<tr><td colspan=3><b>CLS</b></td></tr>
<tr><td colspan=3>Clear Screen.<br/>Fill Pattern Name Table with 0x20 character.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>CLS()</td></tr>
<tr><td><b>Input</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>CLS();</pre></td></tr>
</table>

<br/>

### 4.6 LOCATE

<table>
<tr><td colspan=3><b>LOCATE</b></td></tr>
<tr><td colspan=3>Moves the cursor to the specified location.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>LOCATE(column,line)</td></tr>
<tr><td rowspan=2><b>Input</b></td><td>[char]</td><td>Position X of the cursor.<br/>TEXT 1 (0 to 39)<br/>TEXT 2 (0 to 79)<br/>GRAPHIC 1 (0 to 31)</td></tr>
<tr><td>[char]</td><td>Position Y of the cursor. (0 to 23) </td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>LOCATE(8,12);<br/>PRINT("Hello World!");</pre></td></tr>
</table>

| Note: |
| :---  |
| While in the MSX system BIOS the coordinates start at 1, in the MSX BASIC `LOCATE` instruction, it does so at 0.<br/>The fR3eL project libraries try to provide similar functions to those of MSX BASIC, which is why it uses the same range of values.<br/>This also affects the `GetCursorRow` and `GetCursorColumn` functions. |

<br/>

### 4.7 PRINT

<table>
<tr><td colspan=3><b>PRINT</b></td></tr>
<tr><td colspan=3>Displays a text string at the current cursor position.<br/>Use the LOCATE function to indicate a specific position.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>PRINT(text)</td></tr>
<tr><td><b>Input</b></td><td>[char*]</td><td>String</td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>LOCATE(0,8);<br/>PRINT("\tLine 1\n\tLine 2\n\tLine 3\n");</pre></td></tr>
</table>

Read [Appendix 1](#71-Escape-sequences) for supported C escape secuences.

<br/>

### 4.8 PrintLN

<table>
<tr><td colspan=3><b>PrintLN</b></td></tr>
<tr><td colspan=3>Displays a text string at the current cursor position and adds a new line (CRLF) at the end.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>PrintLN(text)</td></tr>
<tr><td><b>Input</b></td><td>[char*]</td><td>String<br/>(See escape codes table)</td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>PrintLN("Put your hands up in the air");</pre></td></tr>
</table>

<br/>

### 4.9 PrintNumber

<table>
<tr><td colspan=3><b>PrintNumber</b></td></tr>
<tr><td colspan=3>Displays an unsigned integer at the current cursor position.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>PrintNumber(number)</td></tr>
<tr><td><b>Input</b></td><td>[unsigned int] or [char]</td><td>numeric value</td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td rowspan=2><b>Example:</b></td><td colspan=2><pre>PrintNumber(1234);</pre></td></tr>
<tr><td colspan=2><pre>1234</pre></td></tr>
</table>

| Note: |
| :---  |
| This function operates like PrintFNumber(number,0,5); |

<br/>

### 4.10 PrintFNumber

<table>
<tr><td colspan=3><b>PrintFNumber</b></td></tr>
<tr><td colspan=3>Displays an unsigned integer with formatting parameters, at the current cursor position.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>PrintFNumber(number, emptyChar, length)</td></tr>
<tr><td rowspan=3>Input</td><td>[unsigned int] or [char]</td><td>numeric value</td></tr>
<tr><td>[char]</td><td>Empty character value: (0 = nothing, 32=' ', 48='0')</td></tr>
<tr><td>[char]</td><td>Length (1 to 5)</td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td rowspan=2><b>Example:</b></td><td colspan=2><pre>unsigned int value=303;<br/>PrintFNumber(value,48,5);</pre></td></tr>
<tr><td colspan=2><pre>00303</pre></td></tr>
</table>

<br/>

| Note: |
| :---  |
| The empty character parameter is used to indicate that the first characters that do not correspond to a numerical figure must be printed.<br/><b>Example:</b> If we want a 5 character output for the number `123`, if we indicate that the empty character is `0`, the display will show `00123`. |

<br/>

### 4.11 bchput

<table>
<tr><td colspan=3><b>bchput</b></td></tr>
<tr><td colspan=3>Displays a character or executes control code</td></tr>
<tr><td><b>Function</b></td><td colspan=2>bchput(character)</td></tr>
<tr><td>Input</td><td>[char]</td><td>character value or Control Codes/td></tr>
<tr><td><b>Output</b></td><td colspan=2> --- </td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>bchput(0xA);//execute NewLine Control Code<br/>bchput('A');//display A</pre></td></tr>
</table>

<br/>

### 4.12 GetColumns

<table>
<tr><td colspan=3><b>GetColumns</b></td></tr>
<tr><td colspan=3>Provides the columns from current screen.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>GetColumns()</td></tr>
<tr><td>Input</td><td colspan=2> --- </td></tr>
<tr><td><b>Output</b></td><td>[char]</td><td>Columns value</td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>char columns=GetColumns();</pre></td></tr>
</table>

<br/>

### 4.13 GetCursorRow

<table>
<tr><td colspan=3><b>GetCursorRow</b></td></tr>
<tr><td colspan=3>Provides the current row-position of the cursor.</td></tr>
<tr><td><b>Function</b></td><td colspan=2>GetCursorRow()</td></tr>
<tr><td>Input</td><td colspan=2> --- </td></tr>
<tr><td><b>Output</b></td><td>[char]</td><td>Cursor Row value (0-23)</td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>char curY=GetCursorRow();</pre></td></tr>
</table>

<br/>

### 4.14 GetCursorColumn

<table>
<tr><td colspan=3><b>GetCursorColumn</b></td></tr>
<tr><td colspan=3>Provides the current column-position of the cursor</td></tr>
<tr><td><b>Function</b></td><td colspan=2>GetCursorColumn()</td></tr>
<tr><td>Input</td><td colspan=2> --- </td></tr>
<tr><td><b>Output</b></td><td>[char]</td><td>Cursor Column value (0-79)</td></tr>
<tr><td><b>Example:</b></td><td colspan=2><pre>char curX=GetCursorColumn();</pre></td></tr>
</table>

<br/>

---

## 5 How does it work?

In this document we are going to focus on the operation of this library.
For aspects related to the use of the compiler, I recommend consulting the fR3eL project page or tutorials on how to program applications for MSX with SDCC.

The name and operation of the functions has been tried to be similar to that of MSX BASIC so that those who already know this programming language will find it easier to program with this library.

The first thing we must do is initialize the screen mode. 
Before using the `SCREEN0()` or `SCREEN1()` functions, it is recommended to first indicate the total number of columns `WIDTH()` and the colors `COLOR`, since it may affect differently how the screen will be displayed.

Once this is done, you can print texts or numbers using the `PRINT`, `PrintLN`, `PrintNumber` or `PrintFNumber` functions.

To have control over how your texts are displayed, you can use the `LOCATE` function or the supported control characters.
Please note that when you initialize the screen with `SCREEN0` or `SCREEN1`, or when you clear the screen with `CLS`, the cursor will be placed at position 0.0.

<br/>

### 5.1 TEXT 1 and TEXT 2 modes

An example of a use case would be to start the TEXT 2 screen mode (Screen 0 in 80 columns), to develop an application for MSX 2 or higher.

To do this we must use `SCREEN0()`, but this function does not know if we want TEXT 1 mode (up to 40 columns) or TEXT 2 (41 to 80). 
The way to differentiate between the two modes is to first indicate the number of columns in which we want to work with the `WIDTH()` function. 
When it is greater than 40, the TEXT 2 mode will be activated.

```c
void main(void)
{
   COLOR(15,4,4);
   WIDTH(80);
   SCREEN0();  
}
```

In Screen 0, we can change the colors at any time since this mode only uses two colors for the entire screen.

<br/>

### 5.2 GRAPHIC 1 mode

Another use case would be to use GRAPHIC 1 mode with 32 columns.
 
It is also recommended to assign the total columns and colors before initializing the screen mode. 
In this case, the initialization function fills the VRAM color table with the values that we have given with the `COLOR` function. 
For this reason, changing the colors after initialization will not generate any effect, except for the border color.

```c
void main(void)
{
   COLOR(WHITE,DARK_BLUE,BLACK);
   WIDTH(32);
   SCREEN1();  
}
```

#### 5.2.1 Set GRAPHIC 1 Colors

32-column text mode allows for the display of more colors but with some limitations. 
You can only assign two colors (ink and background) for each group of 8 tiles, a total of 32 values (Colour Table) for the 256 tiles.

This library is not compiled with a function to change colors, but is included in the commented sources, to reduce the size of the library. 
If you need it, you can uncomment it (in the source and header) and compile the library, or you can copy-paste this function into your project.

![G1 colors example](pics/Graphic1_colors.png)<br/>
_Graphic1 colors example_

##### Example:

```c
void main(void)
{
	COLOR(WHITE,DARK_BLUE,BLACK);
	WIDTH(32);
	SCREEN1();
   
	SetG1colors(48/8,CYAN,LIGHT_BLUE);	//7th octet for '0' to '7' character codes
	SetG1colors(56/8,GRAY,LIGHT_BLUE);	//8th octet for '8' to '?' character codes

	PRINT("0123456789:;<=>?");
	
__asm call 0x009F __endasm;	
}
```

<br/>

##### ROM or MSXBASIC

```c
/* =============================================================================
SetG1colors
Description: 
		Assigns colors to a group of GRAPHIC1 tiles.
		ROM/MSX-BASIC environment
	   
Input:	(char) Octet. Group of 8 tiles.
		(char) Ink color (0-15)
		(char) Background color (0-15)      
Output:	-
============================================================================= */
void SetG1colors(char octet, char INKcolor, char BGcolor)
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

	ld   A,B
	add  A
	add  A
	add  A
	add  A	
	or   4(IX)	

	call  0x004D	//MSX BIOS WRTVRM - Writes data in VRAM

	pop  IX
__endasm;	
}
```

<br/>

##### MSX-DOS

```c
/* =============================================================================
SetG1colors
Description: 
		Assigns colors to a group of GRAPHIC1 tiles.
		MSX-DOS environment.
	   
Input:	(char) Octet. Group of 8 tiles.
		(char) Ink color (0-15)
		(char) Background color (0-15)      
Output:	-
============================================================================= */
void SetG1colors(char octet, char INKcolor, char BGcolor)
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

	ld   A,B
	add  A
	add  A
	add  A
	add  A	
	or   4(IX)	

	ld   IX,#0x004D     //MSX BIOS   WRTVRM Writes data in VRAM
	ld   IY,(#0xFCC0)   //System var EXPTBL-1 (FCC1h-1) main BIOS-ROM slot address
	call 0x001C         //MSX BIOS   CALSLT Executes inter-slot call
	ei

	pop  IX
__endasm;	
}
```

<br/>

### 5.3 More things

You can improve the appearance by changing the graphic font. 
To do this you can design a new one based on the one in the MSX system. 
You will need a graphics tool and then obtain the data in C array format directly (if the tool has that option) or through a converter.
In your project you must add code that dumps this data into the VRAM pattern table.

<br/>

---

## 6 Code Example

In the following source code you can see a simple example of using the library. 

You can find more extensive examples in the git project sources.

<br/>

### 6.1 Example 1 ROM

In this source code you will find a simple example of how to use this library in the ROM environment.

Requires the following items:
- Startup file for MSX 8/16K ROM [crt0_MSX816kROM4000](https://github.com/mvac7/SDCC_startup_MSX816kROM4000)
- textmode_MSXBIOS Library

<br/>

And you need the following applications to compile and generate the final ROM:
- [Small Device C Compiler (SDCC) v4.4](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)

![Example screenshot](pics/Example01_01.png)

<br/>

#### Source Code
```c
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

const char text01[] = "Example textmode_MSXBIOS Lib\n";
const char text02[] = "Press a key to continue";

void main(void)
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

	PrintLN("\n>LOCATE(8,20)+PRINT");
	LOCATE(8,20);
	PRINT("a located text");
	
	PrintLN("\n");
	PRINT(text02);
    
// execute BIOS CHGET - One character input (waiting)
__asm call 0x009F __endasm;	
}
```

[`Sourcecode project`](Example01)

<br/>

#### For compile:

First you must compile the source with SDCC as follows:

```
sdcc -mz80 --code-loc 0x4020 --data-loc 0xC000 --use-stdout --no-std-crt0 crt0_MSX816kROM4000.rel textmode_MSXBIOS.rel Example01.c
```

If no error is displayed, you should run hex2bin to convert the SDCC output to a binary file.

```
hex2bin -e ROM -l 2000 Example01.ihx
```

<br/>



### 6.2 Example 2 MSX-DOS

In this source code you will find a simple example of how to use this library in the MSX-DOS environment.

Requires the following items:
- Startup file for MSX-DOS environment [crt0_MSXDOS.rel](https://github.com/mvac7/SDCC_startup_MSXDOS)
- textmode_MSXDOS Library

<br/>

And you need the following applications to compile and generate the final ROM:
- [Small Device C Compiler (SDCC) v4.4](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)

![Example screenshot](pics/Example02_01.png)


#### Source Code

```c
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
```

[`Sourcecode project`](Example02)

<br/>

#### For compile:

First you must compile the source with SDCC as follows:

```
sdcc -mz80 -o build\ --code-loc 0x0170 --data-loc 0 --use-stdout --no-std-crt0 crt0_MSXDOS.rel textmode_MSXDOS.rel Example02.c
```

If no error is displayed, you should run hex2bin to convert the SDCC output to a binary file.

```
hex2bin -e COM build\Example02.ihx
```

<br/>


---

## 7 Appendices

### 7.1 Escape sequences

#### Table of escape sequences

| Esc  | value | Description |
| :--- | :---  | :---        |
| `\a` | 0x07  | Beep |
| `\b` | 0x08  | Backspace. Cursor left, wraps around to previous line, stop at top left of screen. |
| `\t` | 0x09  | Horizontal Tab. Tab, overwrites with spaces up to next 8th column, wraps around to start of next line, scrolls at bottom right of screen. |
| `\n` | 0x0A  | Newline > Line Feed and Carriage Return (CRLF)<br/>Note: CR added in this Lib. |
| `\v` | 0x0B  | Cursor home. Place the cursor at the top of the screen.<br/>**Note:** This does not correspond to Vertical Tab, standardized in C. |
| `\f` | 0x0C  | Formfeed. Clear screen and place the cursor at the top. |
| `\r` | 0x0D  | CR (Carriage Return) |
| `\"` | 0x22  | Double quotation mark |
| `\'` | 0x27  | Single quotation mark |
| `\?` | 0x3F  | Question mark |
| `\\` | 0x5C  | Backslash |
 
<br/>

### 7.2 New line

In the MSX system, in order to process a New Line, it requires the use of two control codes: Carriage Return (0x0D) and Line Feed (0x0A).
If we add a `\n` to a text string in C, when compiling the code of an LF will be obtained, so in execution mode we would obtain a positioning of the cursor at the beginning of the line but without the jump to the next line .
To get new line behavior (CRLF), we would need to add a `\n\r` to the text string.
To simplify this functionality and make it similar to other operating systems, the PRINT function performs a CR when processing an LF code.

**Example:**
```c
  PRINT("Hello World!\nHello everybody!");
```

Output:
```
Hello World!
Hello everybody!
```
 
<br/>

### 7.3 Other scape codes

- `\xhh` Print in the output the character/code given in the hexadecimal value (hh).

- `\nnn` Print in the output the character/code given in the octal value (nnn).

<br/>

### 7.4 Extended Graphic Characters

To print the graphic characters that are in the first positions of the MSX system graphic set (overlapping with the control codes in text mode), 
you will have to use add the value 1 after the character code.

`\1` + `\xhh`

It must be taken into account that the character code is equivalent to the corresponding one of the graphic set added to 64.

```c
  PRINT("\1\x42");   //print smile (2 + 64) = 42 hexadecimal
```

#### Example:

```c
	//Draw a box
	PrintLN("\1\x58\1\x57\1\x57\1\x57\1\x57\1\x59");
	PrintLN("\1\x56    \1\x56");
	PrintLN("\1\x5A\1\x57\1\x57\1\x57\1\x57\1\x5B");
```

#### Extended Graphic Characters Table
![Extended Graphic Characters Table](pics/extended_graphic_characters.png)<br/>
_(By [MSX Resource Center](https://www.msx.org/wiki/MSX_Characters_and_Control_Codes#International_codes))_


<br/>

---

## 8 References

- MSX Resource Center > [Wiki](https://www.msx.org/wiki/) > Main-ROM BIOS > [1.3 Displaying ](https://www.msx.org/wiki/Main-ROM_BIOS#Displaying)
- MSX Resource Center > [Wiki](https://www.msx.org/wiki/) > [MSX Characters and Control Codes](https://www.msx.org/wiki/MSX_Characters_and_Control_Codes)
- Wikipedia > [Escape sequences in C](https://en.wikipedia.org/wiki/Escape_sequences_in_C)
- Wikipedia > [Newline](https://en.wikipedia.org/wiki/Newline)

<br/>

---

![Creative Commons License](https://i.creativecommons.org/l/by-nc/4.0/88x31.png) 
<br/>This document is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/).

# SDCC MSX ROM TEXTMODE Functions Library (object type)

Version: 1.2 (3/4/2018)

Author: mvac7/303bcn

Architecture: MSX

Format: C Object (SDCC .rel)

Programming language: C

WEB:
 
mail: mvac7303b@gmail.com

Doc version: 1.0



Sorry. This text is pending correction.



## 1. Introduction

It's a opensource library of functions for create aplications in text mode.

It uses the functions of the MSX BIOS, so it is designed to create applications 
in ROM format.
  
Works in T1 (screen 0, 40 columns), T2 (screen 0, 80 columns), and G1 (screen 1, 
32 columns) modes.

For develop MSX applications using Small Device C Compiler (SDCC) compilator.



## 2. Acknowledgments
  
Thanks for Info & help, to:

* Avelino Herrera > http://msx.atlantes.org/index_es.html
* Nerlaska > http://albertodehoyonebot.blogspot.com.es
* Fubu > http://www.gamerachan.org/fubu/
* Marq/Lieves!Tuore > http://www.kameli.net/lt/
* Sapphire/Z80ST > http://z80st.auic.es/
* Pentacour > http://pentacour.com/
* JamQue/TPM > http://www.thepetsmode.com/
* Andrear > http://andrear.altervista.org/home/msxsoftware.php
* Konamiman > https://www.konamiman.com
* MSX Assembly Page > http://map.grauw.nl/resources/msxbios.php
* Portar MSX Tech Doc > http://nocash.emubase.de/portar.htm
* MSX Resource Center > http://www.msx.org/
* Karoshi MSX Community > http://karoshi.auic.es/
* BlueMSX >> http://www.bluemsx.com/
* OpenMSX >> http://openmsx.sourceforge.net/
* Meisei  >> ?



## 3. Requirements

Small Device C Compiler (SDCC) v3.6  

http://sdcc.sourceforge.net/



## 4. Functions

* void SCREEN0()
* void SCREEN1()
* void WIDTH(char columns)
* void COLOR(char ink, char background, char border)
* void CLS()
* void LOCATE(char x, char y)
* void PRINT(char* text)
* void PrintNumber(unsigned int value)
* void PrintFNumber(unsigned int value, char emptyChar, char length)
* void num2Dec16(unsigned int aNumber, char *address, char emptyChar)
* void bchput(char value)

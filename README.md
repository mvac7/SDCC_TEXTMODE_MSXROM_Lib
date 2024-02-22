# TEXTMODE MSX ROM Library (fR3eL Project)

```
Architecture: MSX
Format: C Object (SDCC .rel)
Programming language: C and Z80 assembler
```



## Description

This project is a library of functions for creating aplications in text mode.

Works in T1 (screen 0, 40 columns), T2 (screen 0, 80 columns), and G1 (screen 1, 32 columns) modes.

It uses the functions from the MSX BIOS, so it is designed to create applications in ROM format or binaries on MSX BASIC.

It is designed to develop MSX applications using Small Device C Compiler (SDCC), although it is an opensource project. Feel free to use part or all of it to adapt it to other systems or development environments.

You can access the documentation here with [`How to use the library`](docs/HOWTO.md).

In the source code [`examples/`](examples/), you can find applications for testing and learning purposes.

I have adapted a routine for converting a 16 Bits value to ASCII for printing numbers, extracted from the Baze collection [(WEB)](http://baze.sk/3sc/misc/z80bits.html#5.1). 

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!



## History of versions:

- v1.4 (24/11/2023) Update to SDCC (4.1.12) Z80 calling conventions, add PrintLN function, remove bchput, and more improvements.
- v1.3 (05/09/2019) Integer printing functions improved (PrintNumber & PrintFNumber). num2Dec16 becomes PrintFNumber.
- v1.2 (03/04/2018)
- v1.1 (27/02/2017)



## Requirements

- [Small Device C Compiler (SDCC) v4.3](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)



## Functions

| Function | Description |
| :---     | :---        |
| **WIDTH**(columns) | Specifies the number of characters per line in text mode |
| **COLOR**(ink, background, border) | Specifies the colors of the foreground, background, and border area |
| **SCREEN0**() | Initialice TEXT 1 (40 columns) or TEXT 2 (80 columns) screen mode |
| **SCREEN1**() | Initialice GRAPHIC 1 screen mode (32 columns x 24 lines) |
| **CLS**() | Clear Screen. Fill Pattern Name Table with 0x20 character |
| **LOCATE**(column, line) | Moves the cursor to the specified location |
| **PRINT**(text) | Displays a text string in the last position where the cursor is |
| **PrintLN**(text) | Displays a text string in the last position where the cursor is and adds a new line |
| **PrintNumber**(number) | Prints an unsigned integer on the screen |
| **PrintFNumber**(number, emptyChar, length) | Prints an unsigned integer on the screen with formatting parameters |

@echo off
REM MAKEFILE for 8/16K ROMs using SDCC compiler
if exist build\ goto NEXTSTEP1
md build
goto COMPILA
:NEXTSTEP1
REM get SDCC version
sdcc -v
echo Delete build...
del build\*.* /Q
:COMPILA
echo Compiling...
sdcc -mz80 -o build\ --code-loc 0x4020 --data-loc 0xC000 --use-stdout --no-std-crt0 crt0_MSX816kROM4000.rel textmode_MSXBIOS.rel Example01.c
if not exist build\Example01.ihx goto SALIDA
hex2bin -e bin -l 2000 build\Example01.ihx
copy build\Example01.bin TEXTEXA1.ROM
pause
exit /b
:SALIDA
echo ERROR in Compile
pause
exit /b
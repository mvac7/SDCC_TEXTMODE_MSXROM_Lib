@echo off
REM MAKEFILE for MSXDOS .COM using SDCC compiler
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
sdcc -mz80 -o build\ --code-loc 0x0170 --data-loc 0 --use-stdout --no-std-crt0 crt0_MSXDOS.rel textmode_MSXDOS.rel Example02.c
if not exist build\Example02.ihx goto SALIDA
hex2bin -e COM build\Example02.ihx
copy build\Example02.COM TEXTEXA2.COM
pause
exit /b
:SALIDA
echo ERROR in Compile
pause
exit /b
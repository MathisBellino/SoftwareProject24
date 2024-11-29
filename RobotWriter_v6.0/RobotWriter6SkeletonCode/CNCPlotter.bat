@echo off
title CNC Plotter System
color 0A

:compile
cls
echo Compiling CNC Plotter...
gcc -Wall -Wextra -Wno-unused-variable -o build/Debug/CNCPlotter.exe ^
    main.c ^
    calculateWordWidth.c ^
    processWord.c ^
    generateGCode.c ^
    serial.c ^
    storeFontData.c ^
    ui.c ^
    freeFontData.c ^
    rs232.c

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilation successful!
    echo Starting CNC Plotter...
    echo.
    build\Debug\CNCPlotter.exe
    echo.
    echo Program finished. Press any key to recompile, or Ctrl+C to exit.
    pause >nul
    goto compile
) else (
    echo.
    echo Compilation failed!
    echo.
    echo Press any key to attempt recompilation, or Ctrl+C to exit.
    pause >nul
    goto compile
)
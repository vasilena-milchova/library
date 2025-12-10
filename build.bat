@echo off
REM Build script for Library Management System
REM Project structure: main.cpp, includes/, src/

echo Building LibrarySystem...

if not exist build mkdir build
cd build

REM Compile all .cpp files - explicit file listing for Windows
g++ -std=c++17 -Wall -Wextra ..\main.cpp ..\src\Library.cpp ..\src\BookInfo.cpp -I..\includes -o LibrarySystem.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful! Executable: build\LibrarySystem.exe
    echo.
    pause
) else (
    echo Build failed!
    pause
    exit /b 1
)

cd ..
@echo off
REM Build script for SearchEngine (Windows)
REM Usage: build.bat

echo Creating build directory...
if not exist build mkdir build
cd build

echo Configuring CMake...
cmake ..

echo Building project...
cmake --build .

echo Build complete! Executable is in: build\bin\SearchEngine.exe
echo Run with: build\bin\SearchEngine.exe

pause


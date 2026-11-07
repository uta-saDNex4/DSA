@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\msys64\mingw64\bin;%PATH%"

if not exist "build\DSA_Console.exe" (
    echo [!] Chua tim thay build\DSA_Console.exe. Dang bien dich...
    cmake -B build -G "Ninja"
    cmake --build build
)

"%~dp0build\DSA_Console.exe"
pause

@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\msys64\mingw64\bin;%PATH%"

if not exist "build\DSA_QtApp.exe" (
    echo [!] Chua tim thay build\DSA_QtApp.exe. Dang bien dich...
    cmake -B build -G "Ninja"
    cmake --build build
)

echo [*] Dang khoi chay DSA_QtApp.exe...
start "" "%~dp0build\DSA_QtApp.exe"

@echo off
chcp 65001 >nul
title Hệ Thống Quản Lý Điểm Sinh Viên Hệ Tín Chỉ - PTIT Qt6
cd /d "%~dp0"
set "PATH=C:\msys64\mingw64\bin;%PATH%"
if not exist "build\DSA_QtApp.exe" (
    echo [!] Chưa tìm thấy build\DSA_QtApp.exe. Đang tiến hành biên dịch...
    cmake -B build -G "Ninja" -DCMAKE_CXX_COMPILER="C:/msys64/mingw64/bin/c++.exe"
    cmake --build build
)
echo [*] Đang khởi chạy ứng dụng giao diện đồ họa Qt6...
start "" "build\DSA_QtApp.exe"

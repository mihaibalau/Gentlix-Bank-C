@echo off
REM Environment setup script for MSYS2 UCRT64
REM This script sets up the environment variables for CLion

set PATH=C:\msys64\ucrt64\bin;%PATH%
set PKG_CONFIG_PATH=C:\msys64\ucrt64\lib\pkgconfig
set PKG_CONFIG=C:\msys64\ucrt64\bin\pkg-config.exe


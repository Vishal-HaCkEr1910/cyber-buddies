@echo off
echo ================================================
echo    StegoProto - Complete Setup Script
echo ================================================
echo.

REM Step 1: Check if Node.js is installed
echo [1/4] Checking Node.js installation...
where node >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Node.js is not installed!
    echo Please install Node.js from https://nodejs.org/
    pause
    exit /b 1
)
echo    Node.js found: 
node --version
echo.

REM Step 2: Check if g++ is installed
echo [2/4] Checking C++ compiler...
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: g++ compiler not found!
    echo Please install MinGW or similar C++ compiler
    echo Download from: https://winlibs.com/
    pause
    exit /b 1
)
echo    g++ found:
g++ --version | findstr "g++"
echo.

REM Step 3: Install Node.js dependencies
echo [3/4] Installing Node.js dependencies...
call npm install
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to install dependencies
    pause
    exit /b 1
)
echo    Dependencies installed successfully!
echo.

REM Step 4: Compile C++ program
echo [4/4] Compiling C++ steganography program...
g++ -o stego.exe stego_cli.cpp -std=c++11
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile C++ program
    pause
    exit /b 1
)
echo    C++ program compiled successfully!
echo.

echo ================================================
echo    Setup Complete!
echo ================================================
echo.
echo Next steps:
echo   1. Run: npm start
echo   2. Open browser: http://localhost:3000
echo   3. Start encoding/decoding files!
echo.
echo Press any key to exit...
pause >nul

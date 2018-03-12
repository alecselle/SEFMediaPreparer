@echo off

set vcvarsall=%PROGRAMFILES(x86)%\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat
set qmake=%LOCALAPPDATA%\\Qt\\5.10.1\\msvc2017_64\\bin\\qmake.exe
set jom=%LOCALAPPDATA%\\Qt\\Tools\\QtCreator\\bin\\jom.exe

echo.Building SEF Media Preparer in: %~dp0
echo.Setting up environment...
call "%vcvarsall%" amd64
cd /d %~dp0
echo.Running qmake...
call "%qmake%" "%~dp0\\SEFMediaPreparer.pro" -spec win32-msvc
echo.Running jom...
call "%jom%" qmake_all 2> null
call "%jom%" 2> null
echo.Complete...
exit /b 0
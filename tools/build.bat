@echo off

cd "%~dp0"/..

set project=%CD%/SEFMediaPreparer.pro
set source=%CD%
set output=%CD%/build
set qmake=C:/Dev/Qt/Static/5.10.1/bin/qmake.exe
set mingw=C:/Dev/Qt/Tools/mingw530_32/bin/mingw32-make.exe

cd "%source%"

echo.
echo.Project : %project%
echo.Source  : %source%
echo.Output  : %output%
echo.
echo.qmake   : %qmake%
echo.mingw   : %mingw%
echo.

if not exist "%output%" mkdir "%output%"
cd "%output%"

echo.Running `qmake SEFMediaPreparer.pro -spec win32-g++ "CONFIG+=release"`
start /B /W "" "%qmake%" "%project%" -spec win32-g++ "CONFIG+=release" -Wnone
if %errorlevel% NEQ 0 echo.An error occurred while running `qmake SEFMediaPreparer.pro -spec win32-g++ "CONFIG+=release"`
echo.

echo.Running `mingw32-make qmake_all`
start /B /W "" "%mingw%" qmake_all -B
if %errorlevel% NEQ 0 echo.An error occurred while running `mingw32-make qmake_all`
echo.

echo.Running `mingw32-make`
start /B /W "" "%mingw%" -B
if %errorlevel% NEQ 0 echo.An error occurred while running `mingw32-make`
echo.

cd "%source%"
echo.Finished...

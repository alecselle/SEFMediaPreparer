@echo off

cd "%~dp0"/..

set project=%CD%/SEFMediaPreparer.pro
set sourceDir=%CD%
set buildDir=%CD%/build
set varbat=C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat
set qmake=C:/Dev/Qt/Static/5.10.1/bin/qmake.exe
set mingw=C:/Dev/Qt/Tools/mingw530_32/bin/mingw32-make.exe

cd "%sourceDir%"

echo.
echo.Project Path: %project%
echo.Source Dir  : %sourceDir%
echo.Output Dir  : %buildDir%
echo.
echo.qmake Path  : %qmake%
echo.mingw Path  : %mingw%
echo.

if not exist "%buildDir%" mkdir "%buildDir%"
cd "%buildDir%"

echo.Running `qmake SEFMediaPreparer.pro -spec win32-g++ "CONFIG+=release"`
call "%qmake%" "%project%" -spec win32-g++ "CONFIG+=release" -Wnone
echo.

echo.Running `mingw32-make`
call "%mingw%" -B
echo.

if not exist "%sourceDir%/bin" mkdir "%sourceDir%/bin"

echo.Copying build artifacts...
copy "%buildDir%\release\SEFMediaPreparer.exe" "%sourceDir%\bin\"
copy "%sourceDir%\tools\ffmpeg.exe" "%sourceDir%\bin\"
copy "%sourceDir%\tools\ffprobe.exe" "%sourceDir%\bin\"
echo.

cd "%sourceDir%"
echo.Finished...

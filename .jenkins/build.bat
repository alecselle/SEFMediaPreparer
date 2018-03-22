@echo off
setlocal EnableDelayedExpansion
echo.Running qmake...
if not exist "%WORKSPACE%/build" mkdir "%WORKSPACE%/build"
cd "%WORKSPACE%/build"
call "%QMAKE%" "%WORKSPACE%/SEFMediaPreparer.pro" -spec win32-g++ "CONFIG+=release" -Wnone
echo.
echo.Running mingw32-make...
cd "%WORKSPACE%/build"
call "%MINGW%" -s -i -B
echo.
endlocal
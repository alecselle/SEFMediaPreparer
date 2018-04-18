@echo off
set ARCH=32
set MSYS=C:\MSYS\mingw%ARCH%

set USERPROFILE=C:\Users\conta
set APPDATA=C:\Users\conta\AppData\Roaming
set COMSPEC=C:\Windows\System32\cmd.exe

set PATH=%MSYS%\bin;C:\Program Files\Git\cmd;C:\Windows\System32;%~dp0
set PATH_STATIC=%MSYS%\qt5-static\bin;%PATH%
set PATH_SHARED=%PATH%

set LIBS=%MSYS%\lib
set LIBS_STATIC=%MSYS%\qt5-static\lib;%LIBS%
set LIBS_SHARED=%LIBS%

set INCLUDE=%MSYS%\include
set INCLUDE_STATIC=%MSYS%\qt5-static\include;%INCLUDE%
set INCLUDE_SHARED=%INCLUDE%

set QTDIR=%MSYS%\share\qt5
set QTDIR_STATIC=%MSYS%\qt5-static\share\qt5
set QTDIR_SHARED=%QTDIR%

set QMAKE=%MSYS%\bin\qmake.exe
set QMAKE_STATIC=%MSYS%\qt5-static\bin\qmake.exe
set QMAKE_SHARED=%QMAKE%

set MINGW=%MSYS%\bin\mingw32-make.exe
set WINDEPLOY=%MSYS%\bin\windeployqt.exe
set INNOSETUP=C:\Program Files (x86)\Inno Setup 5\ISCC.exe
set ZIP=C:\Program Files\7-ZIP\7z.exe

cd "%~dp0"&& cd ".."
if "!WORKSPACE!" EQU "" set WORKSPACE="%CD%"

set /p VERSION=<"%WORKSPACE%/version.txt"

exit /b 0
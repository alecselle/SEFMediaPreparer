@echo off
setlocal enableDelayedExpansion
set result=1
goto defaults

:defaults
set sourceDir=%~dp0
set buildDir=%~dp0
set vcvarsall=%PROGRAMFILES(x86)%/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat
set qmake=%LOCALAPPDATA%/Qt/5.10.1/msvc2017_64/bin/qmake.exe
set jom=%LOCALAPPDATA%/Qt/Tools/QtCreator/bin/jom.exe
set Makefile=Makefile.release
set project=SEFMediaPreparer
set force=0
goto params

:params
if not "%~1"=="" (
	if "%~1"=="-help" (
		goto help
	)
	if "%~1"=="-h" (
		goto help
	)
	if "%~1"=="/?" (
		goto help
	)
	if "%~1"=="?" (
		goto help
	)
	if "%~1"=="-vcvarsall" (
		set vcvarsall=%~2
		shift
	)
	if "%~1"=="-vc" (
		set vcvarsall=%~2
		shift
	)
	if "%~1"=="-qmake" (
		set qmake=%~2
		shift
	)
	if "%~1"=="-q" (
		set qmake=%~2
		shift
	)
	if "%~1"=="-jom" (
		set jom=%~2
		shift
	)
	if "%~1"=="-j" (
		set jom=%~2
		shift
	)
	if "%~1"=="-project" (
		set project=%~2
		shift
	)
	if "%~1"=="-p" (
		set project=%~2
		shift
	)
	if "%~1"=="-force" (
		set force=1
	)
	if "%~1"=="-f" (
		set force=1
	)
	shift
)
if not "%~1"=="" goto params
goto build

:branding
echo.
echo.SuperEpicFuntime Build Script
echo.
exit /b 0
goto eof

:help
call :branding
echo.        -v ^<path to vcvarsall.bat^>
echo.        -q ^<path to qmake.exe^>
echo.        -j ^<path to jom.exe^>
echo.        -p ^<project ('.pro')^>
echo.        -s ^<source directory^>
echo.        -b ^<build directory^>
echo.        -f (force build)
echo.
exit /b -1
goto eof

:info
call :branding
echo.  Building Project: "%project%"
echo.
echo.  Source Directory: "%sourceDir%"
echo.  Build Directory:  "%buildDir%"
echo.
exit /b 0
goto eof

:validate
if "%project:~-4%" NEQ ".pro" set project=%project%.pro
if not exist "%project%" (
	echo."%project%" not found...
	exit /b 1
)
if not exist "%Makefile%" (
	echo.Makefile not found...
	exit /b 1
)
if not exist "%vcvarsall%" (
	echo."%vcvarsall%" not found...
	exit /b 1
)
if not exist "%qmake%" (
	echo."%qmake%" not found...
	exit /b 1
)
if not exist "%jom%" (
	echo."%jom%" not found...
	exit /b 1
)
if %force% NEQ 0 call :force
exit /b 0
goto eof

:force
echo.    Removing existing build files...
del /Q /S "%sourceDir%/debug" "%sourceDir%/release" "%sourceDir%/Makefile" "%sourceDir%/Makefile.debug" "%sourceDir%/Makefile.release" "%sourceDir%/.qmake.stash" 2> nul
exit /b %errorlevel%
goto eof

:environment
echo.    Setting up environment...
echo.    ^> vcvarsall.bat amd64
if not defined DevEnvDir call "%vcvarsall%" amd64 2> nul
if %errorlevel% NEQ 0 (
	echo.Environment setup failed...
	exit /b 1
)
echo.
exit /b 0
goto eof

:qmake
echo.    Running qmake...
echo.    ^> qmake.exe %project%.pro -spec win32-msvc
call "%qmake%" "%sourceDir%/%project%" -spec win32-msvc 2> nul
if %errorlevel% NEQ 0 (
	echo.qmake failed...
	exit /b 1
)
echo.
exit /b 0
goto eof

:jom
echo.    Runing jom...
echo.    ^> jom.exe -f %Makefile%
call "%jom%" -f %Makefile% 2> nul
if %errorlevel% NEQ 0 (
	echo.jom failed...
	exit /b 1
)
echo.
exit /b 0
goto eof

:build
call :validate
if %errorlevel% NEQ 0 (
	set result=1
	goto end
)
call :info
call :environment
if %errorlevel% NEQ 0 (
	set result=1
	goto end
)
call :qmake
if %errorlevel% NEQ 0 (
	set result=1
	goto end
)
call :jom
if %errorlevel% NEQ 0 (
	set result=1
	goto end
)
set result=0
goto end

:end
if "%result%" EQU "1" echo.Build Failed...
if "%result%" EQU "0" echo.Build Successful...
if "%result%" NEQ "1" if "%result%" NEQ "0" echo.Unknown Error...
exit /b %~1
goto eof
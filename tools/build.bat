@echo off
setlocal enableDelayedExpansion
set result=1
goto build

::Main

:build
call :defaults
call :params
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

::Functions

:defaults
set sourceDir=%~dp0
set buildDir=%~dp0
set buildTool=mingw53_32
set vcvarsall=%PROGRAMFILES(x86)%/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat
set qmake=C:/Dev/Qt/5.10.1/%buildTool%/bin/qmake.exe
set jom=C:/Dev/Qt/Tools/QtCreator/bin/jom.exe
set project=SEFMediaPreparer.pro
set force=0
exit /b 0
goto eof

:params
if not "%~1"=="" (
    if "%~1"=="-help" goto help
	if "%~1"=="-h" goto help
	if "%~1"=="/?" goto help
	if "%~1"=="?" goto help
	if "%~1"=="-force" set force=1
	if "%~1"=="-f" set force=1
	if "%~1"=="-vcvarsall" (
	    set vcvarsall=%~2
		shift
	)
	if "%~1"=="-vc" (
	    set vcvarsall=%~2
		shift
	)
	if "%~1"=="-v" (
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
	shift
)
if not "%~1"=="" goto params
exit /b 0
goto eof

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
echo.  vcvarsall.bat:    "%vcvarsall%"
echo.  qmake.exe:        "%qmake%"
echo.  jom.exe:          "%jom%"
echo.
echo.  Building Project: "%project%"
echo.
echo.  Source Directory: "%sourceDir%"
echo.  Build Directory:  "%buildDir%"
echo.
exit /b 0
goto eof


:dependency
call :dependency_vcvarsall
if %errorlevel% NEQ 0 exit /b 1
call :dependency_qmake_jom
if %errorlevel% NEQ 0 exit /b 1
exit /b 0
goto eof

:dependency_vcvarsall
if exist "%vcvarsall%" (
	exit /b 0
	goto eof
)
set staticPathVcvarsall=Community/VC/Auxiliary/Build/vcvarsall.bat
set staticDirs="%PROGRAMFILES(x86)%/Microsoft Visual Studio/" "%PROGRAMFILES%/Microsoft Visual Studio/"
for %%a in (%staticDirs%) do for /D %%i in ("%%~a/*") do (
	if exist "%%~a/%%~nxi/%staticPathVcvarsall%" set vcvarsall=%%~a/%%~nxi/%staticPathVcvarsall%
)
if not exist "%vcvarsall%" (
	echo.Cannot Find Visual Studio Installation...
	exit /b 1
	goto eof
)
echo.Using Default 'vcvarsall.bat'
exit /b 0
goto eof

:dependency_qmake_jom
if exist "%qmake%" (
	if exist "%jom%" (
		exit /b 0
		goto eof
	)
)
set staticPathQmake=%buildTool%/bin/qmake.exe
set staticPathJom=QtCreator/bin/jom.exe
set staticDirs="C:/Dev/Qt/" "C:/Qt/" "%PROGRAMFILES%/Qt/" "%PROGRAMFILES(x86)%/Qt/"
for %%a in (%staticDirs%) do for /D %%i in ("%%~a/*") do (
	if exist "%%~a/%%~nxj/%staticPathQmake%" set qmake=%%~a/%%~nxj/%staticPathQmake%
	if exist "%%~a/%%~nxj/%staticPathJom%" set jom=%%~a/%%~nxj/%staticPathJom%
)
set dynamicDirsA="C:/Users/"
set dynamicDirsB="AppData/Local/Qt/"
for %%a in (%dynamicDirsA%) do for /D %%i in ("%%~a/*") do for %%b in (%dynamicDirsB%) do for /D %%j in ("%%~a/%%~nxi/%%~b/*") do (
	if exist "%%~a/%%~nxi/%%~b/%%~nxj/%staticPathQmake%" set qmake=%%~a/%%~nxi/%%~b/%%~nxj/%staticPathQmake%
	if exist "%%~a/%%~nxi/%%~b/%%~nxj/%staticPathJom%" set jom=%%~a/%%~nxi/%%~b/%%~nxj/%staticPathJom%
)
if not exist "%jom%" (
	echo.Cannot Find QtCreator Installation...
	exit /b 1
	goto eof
)
echo.Using Default 'jom.exe'
if not exist "%qmake%" (
	echo.Cannot Find QtCreator msvc2017_64 Installation...
	exit /b 1
	goto eof
)
echo.Using Default 'qmake.exe'
exit /b 0
goto eof

:validate
call :dependency
if %errorlevel% NEQ 0 (
	echo.Failed to locate dependencies...
	exit /b 1
)
cd %~dp0
if "%project:~-4%" NEQ ".pro" set project=%project%.pro
if not exist "%project%" (
	echo."%project%" not found...
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
call cd /d "%~dp0"
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
call cd /d "%~dp0"
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
echo.    ^> jom.exe -f Makefile
call cd /d "%~dp0"
call "%jom%" -f Makefile 2> nul
if %errorlevel% NEQ 0 (
	echo.jom failed...
	exit /b 1
)
echo.
exit /b 0
goto eof

:end
if %result% EQU 1 echo.Build Failed... && exit /b 1 && goto eof
if %result% EQU 0 echo.Build Successful... && exit /b 0 && goto eof
if %result% NEQ 1 if "%result%" NEQ "0" echo.Unknown Error... && exit /b 2 && goto eof
goto eof

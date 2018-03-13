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
set project=SEFMediaPreparer.pro
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
if not exist "%PROGRAMFILES(x86)%/Microsoft Visual Studio" (
	echo.Cannot Find Visual Studio Installation...
	exit /b 1
	goto eof
)
cd "%PROGRAMFILES(x86)%/Microsoft Visual Studio"
for /D %%i in (*) do if exist "%%~fi/Community/VC/Auxiliary/Build/vcvarsall.bat" set vcvarsall=%%~fi/Community/VC/Auxiliary/Build/vcvarsall.bat
exit /b 0
goto eof

:dependency_qmake_jom
if exist "%qmake%" (
	if exist "%jom%" (
		exit /b 0
		goto eof
	)
)
if not exist "C:/Qt" (
	if not exist "%LOCALAPPDATA%/Qt" (
		cd "C:/Users"
		for /D %%i in (*) do (
			if exist "%%~fi/AppData/Local/Qt" (
				cd "%%~fi/AppData/Local/Qt"
				for /D %%j in (*) do (
					if exist "%%~fj/msvc2017_64/bin/qmake.exe" set qmake=%%~fj/msvc2017_64/bin/qmake.exe
					if exist "%%~fj/Tools/QtCreator/bin/jom.exe" set jom=%%~fj/Tools/QtCreator/bin/jom.exe
				)
			)
		)
	)
)
if not exist "%jom%" (
	echo.Cannot Find QtCreator Installation...
	exit /b 1
	goto eof
)
if not exist "%qmake%" (
	echo.Cannot Find QtCreator msvc2017_64 Installation...
	exit /b 1
	goto eof
)
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
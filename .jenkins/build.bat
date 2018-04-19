@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set PROJECT_DEFAULT=SEFMediaPreparer.pro
call "%~dp0/env.bat"
:VARIABLES
	if "%~1" NEQ "" (
		if "%~1" EQU "-static" set LINK=static
		if "%~1" EQU "-shared" set LINK=shared
		if exist "!WORKSPACE!/%~1" set PROJECT=%~1
		shift
	)
	if "%~1" NEQ "" goto VARIABLES
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Build] Running Script (0/5)
	
	echo.[Build] Checking Variables (1/5) 
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Build] Running qmake (2/5)
	call :QMAKE
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Build] Running mingw32-make (3/5)
	call :MINGW
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	if "!LINK!" EQU "shared" (
		echo.[Build] Running windependloyqt ^(4/5^)
		call :WINDEPLOY
		if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	)
	
	echo.[Build] Copying Artifacts (5/5)
	call :COPY_ARTIFACTS_EXE
	if "!LINK!" EQU "shared" call :COPY_ARTIFACTS_DLL
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!" EQU "" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if "!PROJECT!" EQU "" set PROJECT=!PROJECT_DEFAULT!
	if not exist "!WORKSPACE!/.jenkins/.data" mkdir "!WORKSPACE!/.jenkins/.data" & attrib +h "!WORKSPACE!/.jenkins/.data" /s /d
	if not exist "!WORKSPACE!/!PROJECT!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
	if not exist "!WORKSPACE!/!PROJECT!" (
		if not exist "!WORKSPACE!/!PROJECT_DEFAULT!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
		set PROJECT=!PROJECT_DEFAULT!
	)
	if "!LINK!" EQU "" set LINK=static
	if "!LINK!" EQU "shared" set QMAKE=!QMAKE_SHARED!
	if "!LINK!" EQU "static" set QMAKE=!QMAKE_STATIC!
	if "!QMAKE!" EQU "" (
		call where /q qmake.exe >nul 2>&1
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "qmake.exe"
		for /f "tokens=*" %%i in ('where qmake.exe 2^> nul') do set QMAKE=%%i
	)
	if not exist "!QMAKE!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "qmake.exe"
	if "!MINGW!" EQU "" (
		call where /q mingw32-make.exe >nul 2>&1
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "mingw32-make.exe"
		for /f "tokens=*" %%i in ('where mingw32-make.exe 2^> nul') do set MINGW=%%i
	)
	if not exist "!MINGW!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "mingw32-make.exe"
	if "!WINDEPLOY!" EQU "" (
		call where /q windependloyqt.exe >nul 2>&1
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "windependloyqt.exe"
		for /f "tokens=*" %%i in ('where windependloyqt.exe 2^> nul') do set WINDEPLOY=%%i
	)
	if not exist "!WINDEPLOY!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "windependloyqt.exe"
	exit /b !ERROR_LEVEL!
	goto EOF
:BUILD_DIR
	if not exist "!WORKSPACE!/build" mkdir "!WORKSPACE!/build"
	cd "!WORKSPACE!/build"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "BUILD_DIR" "!WORKSPACE!/build"
	exit /b !ERROR_LEVEL!
	goto EOF
:QMAKE
	call :BUILD_DIR
	echo.[Build] "!QMAKE!" "!PROJECT!"
	call "!QMAKE!" -spec win32-g++ -nocache "CONFIG+=release" "!WORKSPACE!/!PROJECT!"> "%~dp0/.data/qmake.log" 2>&1
	if !errorlevel! NEQ 0 call :ERROR_BUILD_FAILED "QMAKE" "Qmake returned an error" "Check output for details"
	exit /b !ERROR_LEVEL!
	goto EOF
:MINGW
	call :BUILD_DIR
	echo.[Build] "!MINGW!"
	call "!MINGW!" -s -j 8 -B> "%~dp0/.data/mingw.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "MINGW" "MinGW returned an error" "Check output for details" 
	exit /b !ERROR_LEVEL!
	goto EOF
:WINDEPLOY
	call :BUILD_DIR
	echo.[Build] "!WINDEPLOY!" "SEFMediaPreparer.exe"
	call "!WINDEPLOY!" "!WORKSPACE!/bin/SEFMediaPreparer.exe" --release --force --no-translations --dir "!WORKSPACE!/bin"> "%~dp0/.data/windeploy.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "WINDEPLOY" "WinDeployQt returned an error" "Check output for details" 
	exit /b !ERROR_LEVEL!
	goto EOF
:COPY_ARTIFACTS_EXE
	call xcopy /Y "!WORKSPACE!\lib\*.exe" "!WORKSPACE!\bin\"> "%~dp0/.data/xcopy.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_COPY_FAILED "COPY_ARTIFACTS"
	exit /b !ERROR_LEVEL!
	goto EOF
:COPY_ARTIFACTS_DLL
	call xcopy /Y "!WORKSPACE!\lib\*.dll" "!WORKSPACE!\bin\"> "%~dp0/.data/xcopy.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_COPY_FAILED "COPY_ARTIFACTS"
	exit /b !ERROR_LEVEL!
	goto EOF
:: ~~
::=============================================================================
:: ~~ ERROR DECLARATIONS
:ERROR_FILE_NOT_FOUND
:: call :ERROR_FILE_NOT_FOUND "<IDENTIFIER>" "<FILE>"
	call :ERROR "ERROR_FILE_NOT_FOUND" %~1 "File not found '%~2'"
	exit /b 1
	goto EOF
:ERROR_CREATE_DIR_FAILED
:: call :ERROR_CREATE_DIR_FAILED "<IDENTIFIER>" "<DIRECTORY>"
	call :ERROR "ERROR_CREATE_DIR_FAILED" %~1 "Could not create directory '%~2'"
	exit /b 1
	goto EOF
:ERROR_BUILD_FAILED
:: call :ERROR_BUILD_FAILED "<IDENTIFIER>" "[<MESSAGES>]"
	call :ERROR "ERROR_BUILD_FAILED" %*
	exit /b 1
	goto EOF
:ERROR_COPY_FAILED
	call :ERROR "ERROR_COPY_FAILED" %~1 "Could not copy files"
	exit /b 1
	goto EOF
:: ~~
::=============================================================================
:: ~~ ERROR FRAMEWORK
:ERROR
:: call :ERROR "<ERROR_CODE>" "<IDENTIFIER>" "[<MESSAGES>]"
	set ERROR_LEVEL=1
	if "%~2" EQU "" (
		set ERROR_CODE=ERROR_CRITICAL_INVALID
		set ERROR_IDENTIFIER=INVALID
	) else (
		set ERROR_CODE=%~1
		set ERROR_IDENTIFIER=%~2
	)
	for /f "tokens=1,2,* delims= " %%a in ("%*") do set ERROR_MESSAGE=%%c
	echo.=================================================================
	echo.  !ERROR_CODE! [ %~nx0 :!ERROR_IDENTIFIER! ] 
	echo.
	if !ERROR_MESSAGE! NEQ "" (
		for %%x in (!ERROR_MESSAGE!) do echo.    %%~x
		echo.
	)
	echo.=================================================================
	exit /b 1
	goto EOF
:: ~~
::=============================================================================
:END_SUCCESS
	echo.[Build] Completed Successfully
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	echo.[Build][WARNING] Completed Unsuccessfully
	endlocal
	exit /b 1
	goto EOF
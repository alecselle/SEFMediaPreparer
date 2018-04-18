@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set DEBUG=0
call "%~dp0/env.bat"
:RESET
	if "%~1" EQU "-r" goto :RESET_SETTINGS
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Version] Running Script (0/4)
	
	echo.[Version] Checking Variables (1/4)
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Version] Loading Data (2/4)
	call :LOAD_SETTINGS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Version] Parsing Data (3/4)
	call :PARSE_VERSION
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Version] Saving Data (4/4)
	call :SAVE_SETTINGS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	call :DISPLAY
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE

	goto END_SUCCESS
	goto EOF
:: ~~
::============================================================================= 
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!" EQU "" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if not exist "!WORKSPACE!/version.txt" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "version.txt"
	if not exist "!WORKSPACE!/.jenkins/.data" mkdir "!WORKSPACE!/.jenkins/.data" & attrib +h "!WORKSPACE!/.jenkins/.data" /s /d
	exit /b !ERROR_LEVEL!
	goto EOF
:LOAD_SETTINGS
	if not exist "!WORKSPACE!/version.txt" call :ERROR_FILE_NOT_FOUND "LOAD_SETTINGS" "version.txt"
	set /p VERSION=<"!WORKSPACE!/version.txt"
	exit /b !ERROR_LEVEL!
	goto EOF
:SAVE_SETTINGS
	echo|set /p="!VERSION!" >"!WORKSPACE!/version.txt"
	exit /b !ERROR_LEVEL!
	goto EOF
:PARSE_VERSION
	for /F "tokens=1,2,3 delims=. " %%a in ("!VERSION!") do (
		set MAJOR=%%a
		set MINOR=%%b
		set PATCH=%%c
	)
	if %errorlevel% NEQ 0 call :ERROR_PARSE_FAILED "PARSE_VERSION"
	if !ERROR_LEVEL! NEQ 0 exit /b !ERROR_LEVEL! && goto EOF
	set "VERSION=!MAJOR!.!MINOR!.!PATCH!"
	exit /b !ERROR_LEVEL!
	goto EOF
:DISPLAY
	echo.[Version] Building !VERSION!
	exit /b !ERROR_LEVEL!
	goto EOF
:: ~~
::=============================================================================
:: ~~ ERROR DECLARATIONS
:ERROR_FILE_NOT_FOUND
:: call :ERROR_FILE_NOT_FOUND "<IDENTIFIER>" "<FILE>"
	call :ERROR "ERROR_FILE_NOT_FOUND" "%~1" "File not found '%~2'"
	exit /b 1
	goto EOF
:ERROR_CREATE_DIR_FAILED
:: call :ERROR_CREATE_DIR_FAILED "<IDENTIFIER>" "<DIRECTORY>"
	call :ERROR "ERROR_CREATE_DIR_FAILED" "%~1" "Could not create directory '%~2'"
	exit /b 1
	goto EOF
:ERROR_PARSE_FAILED
:: call :ERROR_PARSE_FAILED "<IDENTIFIER>"
	call :ERROR "ERROR_PARSE_FAILED" "%~1" "Failed to parse version"
	exit /b 1
	goto EOF
:ERROR_SAVE_FAILED
:: call :ERROR_SAVE_FAILED "<IDENTIFIER>"
	call :ERROR "ERROR_SAVE_FAILED" "%~1" "Failed to save version to file"
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
	echo.[Version] Completed Successfully
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	echo.[Version][WARNING] Completed Unsuccessfully
	endlocal
	exit /b 1
	goto EOF
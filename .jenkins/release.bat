@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set DEBUG=0
call "%~dp0/env.bat"
call "%~dp0/version.bat" 2>&1 nul
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Release] Running Script (0/2)
	
	echo.[Release] Checking Variables (1/2) 
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE

	echo.[Release] Creating Installer (2/2)
	call :INNOSETUP
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!" EQU "" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if not exist "!WORKSPACE!/.jenkins/.data" mkdir "!WORKSPACE!/.jenkins/.data" & attrib +h "!WORKSPACE!/.jenkins/.data" /s /d
	if "!INNOSETUP!" EQU "" (
		call where /q ISCC.exe 2>&1nul
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "ISCC.exe"
		for /f "tokens=*" %%i in ('where ISCC.exe') do set INNOSETUP=%%i
	)
	if not exist "!INNOSETUP!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "ISCC.exe"
	exit /b !ERROR_LEVEL!
	goto EOF
:INNOSETUP
	cd "!WORKSPACE!"
	echo.[Release] "!INNOSETUP!"
	call "!INNOSETUP!" "!WORKSPACE!/SEFMediaPreparer.iss"> "%~dp0/.data/iscc.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "INNOSETUP" "ISCC returned an error" "Check output for details" 
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
:: call :ERROR_BUILD_FAILED "<IDENTIFIER>" "<EXECUTABLE>"
	call :ERROR "ERROR_BUILD_FAILED" "%~1" "%~2 returned an error" "Check log for details"
	if exist "!WORKSPACE!/.jenkins/.data/%~2.log" echo>"!WORKSPACE!/.jenkins/.data/%~2.log"
	exit /b 1
	goto EOF
:ERROR_COPY_FAILED
	call :ERROR "ERROR_COPY_FAILED" %~1 "Could not copy files"
	exit /b 1
	goto EOF
:ERROR_SYMLINK_FAILED
	call :ERROR "ERROR_SYMLINK_FAILED" %~1 "Could create directory junction"
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
	echo.[Release] Completed Successfully
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	echo.[Release][WARNING] Completed Unsuccessfully
	endlocal
	exit /b 1
	goto EOF
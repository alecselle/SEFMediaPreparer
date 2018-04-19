@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
call "%~dp0/env.bat"
:VARIABLES
	if "%~1" NEQ "" (
		if exist "%~1" set WORKSPACE=%~1
		shift
	)
	if "%~1" NEQ "" goto VARIABLES
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Pull] Running Script (0/2)
	
	echo.[Pull] Checking Variables (1/2) 
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Pull] Pulling from github (2/2)
	call :PULL
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!" EQU "" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if not exist "!GIT!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "git.exe"
	if "!GIT!" EQU "" (
		call where /q git.exe >nul 2>&1
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "git.exe"
		for /f "tokens=*" %%i in ('where git.exe 2^> nul') do set GIT=%%i
	)
	if not exist "!GIT!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "git.exe"
	exit /b !ERROR_LEVEL!
	goto EOF
:PULL
	cd "!WORKSPACE!"
	echo.[Pull] "!GIT!" pull
	call "!GIT!" pull
	if !errorlevel! NEQ 0 call :ERROR_PULL_FAILED "PULL" "Git returned an error" "Check output for details"
	exit /b !ERROR_LEVEL!
	goto EOF
:: ~~
::=============================================================================
:: ~~ ERROR DECLARATIONS
:ERROR_FILE_NOT_FOUND
:: call :ERROR_FILE_NOT_FOUND "<IDENTIFIER>" "<FILE>"
	call :ERROR "ERROR_FILE_NOT_FOUND" %~1 "File not found '%~2'" "%~3"
	exit /b 1
	goto EOF
:ERROR_PULL_FAILED
:: call :ERROR_PULL_FAILED "<IDENTIFIER>" "[<MESSAGES>]"
	call :ERROR "ERROR_PULL_FAILED" %*
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
	echo.[Pull] Completed Successfully
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	echo.[Pull][WARNING] Completed Unsuccessfully
	endlocal
	exit /b 1
	goto EOF
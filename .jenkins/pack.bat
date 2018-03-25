@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set DEBUG=0
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Pack] Running Script (0/2)
	
	echo.[Pack] Checking Variables (1/2)
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Pack] Packing Artifacts (2/2)
	call :PACK_ARTIFACTS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE

	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!"=="" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if "!ZIP!"=="" (
		call where /q 7z.exe
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "7z.exe"
		for /f %%i in ('where 7z') do set ZIP=%%i
	)
	if not exist "!WORKSPACE!/bin/SEFMediaPreparer.exe" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "SEFMediaPreparer.exe"
	if not exist "!WORKSPACE!/tools/ffmpeg.exe" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "ffmpeg.exe"
	if not exist "!WORKSPACE!/tools/ffprobe.exe" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "ffprobe.exe"
	if !ERROR_LEVEL! NEQ 0 exit /b !ERROR_LEVEL! && goto EOF
	if not exist "!WORKSPACE!/release" mkdir "!WORKSPACE!/release"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "CHECK_VARIABLES" "release"
	exit /b !ERROR_LEVEL!
	goto EOF

:PACK_ARTIFACTS
	call "!ZIP!" a -mx9 -mmt8 "!WORKSPACE!/release/SEF.Media.Preparer.!BUILD_DISPLAY_NAME!.7z" "!WORKSPACE!/bin/SEFMediaPreparer.exe" "!WORKSPACE!/tools/ffmpeg.exe" "!WORKSPACE!/tools/ffprobe.exe"
	if %errorlevel% NEQ 0 call :ERROR_ARCHIVE_FAILED "PACK_ARTIFACTS" "SEF.Media.Preparer.!BUILD_DISPLAY_NAME!"
	if !ERROR_LEVEL! NEQ 0 exit /b !ERROR_LEVEL! && goto EOF
	call "!ZIP!" a -mx9 -mmt8 "!WORKSPACE!/release/SEF.Media.Preparer.!BUILD_DISPLAY_NAME!.NF.7z" "!WORKSPACE!/bin/SEFMediaPreparer.exe"
	if %errorlevel% NEQ 0 call :ERROR_ARCHIVE_FAILED "PACK_ARTIFACTS"
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
:ERROR_COPY_FAILED
	call :ERROR "ERROR_COPY_FAILED" %~1 "Could not copy files"
	exit /b 1
	goto EOF
:ERROR_ARCHIVE_FAILED
	call :ERROR "ERROR_ARCHIVE_FAILED" %~1 "Could not create archive '%~2'"
	exit /b 1
	got EOF
:: ~~
::=============================================================================
:: ~~ ERROR FRAMEWORK
:ERROR
:: call :ERROR "<ERROR_CODE>" "<IDENTIFIER>" "[<MESSAGES>]"
	set ERROR_LEVEL=1
	if "%~2"=="" (
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
	echo.[Pack] Completed Successfully
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	echo.[Pack][WARNING] Completed Unsuccessfully
	endlocal
	exit /b 1
	goto EOF
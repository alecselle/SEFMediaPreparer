@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set DEBUG=0
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Archive] Running Script (0/3?)
	
	echo.[Archive] Checking Variables (1/3?)
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Archive] Verifying Directory (2/3?)
	call :BIN_DIR
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Archive] Copying Artifacts (3/3?)
	call :COPY_ARTIFACTS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	if "!packArtifacts!"=="true" (
	
		echo.[Archive] Verifying Directory (4/5)
		call :RELEASE_DIR
		if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
		
		echo.[Archive] Packing Artifacts (5/5)
		call :PACK_ARTIFACTS
		if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
		
	)
	
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!"=="" (
		echo.[WARNING] Variable not initialized. 'WORKSPACE'
		set WORKSPACE=%CD%
		if not exist "!WORKSPACE!/!PROJECT!" (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
			exit /b 1
			goto EOF
		) else (
			echo.[WARNING] Using current directory. '%CD%'
		)
	)
	if "!ZIP!"=="" (
		echo.[WARNING] Variable not initialized. 'ZIP'
		call where /q 7z.exe
		if %errorlevel% NEQ 0 (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "7z.exe"
			exit /b 1
			goto EOF
		) else (
			for /f %%i in ('where 7z') do set ZIP=%%i
			echo.[WARNING] Using '!ZIP!' from PATH
		)
	)
	exit /b 0
	goto EOF
:BIN_DIR
	if not exist "!WORKSPACE!/bin" mkdir "!WORKSPACE!/bin"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "BIN_DIR" "bin" && exit /b 1
	exit /b 0
	goto EOF
:RELEASE_DIR
	if not exist "!WORKSPACE!/release" mkdir "!WORKSPACE!/release"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "RELEASE_DIR" "release" && exit /b 1
	exit /b 0
	goto EOF
:COPY_ARTIFACTS
	copy /Y "!WORKSPACE!\build\release\SEFMediaPreparer.exe" "!WORKSPACE!\bin\"
	if %errorlevel% NEQ 0 call :ERROR_COPY_FAILED "BIN_DIR" && exit /b 1
	exit /b 0
	goto EOF
:PACK_ARTIFACTS
	call "!ZIP!" a -mx9 -mmt8 "!WORKSPACE!/release/SEF.Media.Preparer.!BUILD_DISPLAY_NAME!.7z" "!WORKSPACE!/bin/SEFMediaPreparer.exe" "!WORKSPACE!/tools/ffmpeg.exe" "!WORKSPACE!/tools/ffprobe.exe"
	if %errorlevel% NEQ 0 call :ERROR_ARCHIVE_FAILED "PACK_ARTIFACTS" "SEF.Media.Preparer.!BUILD_DISPLAY_NAME!" && exit /b 1
	call "!ZIP!" a -mx9 -mmt8 "!WORKSPACE!/release/SEF.Media.Preparer.!BUILD_DISPLAY_NAME%.NF.7z" "!WORKSPACE!/bin/SEFMediaPreparer.exe"
	if %errorlevel% NEQ 0 call :ERROR_ARCHIVE_FAILED "PACK_ARTIFACTS" && exit /b 1
	exit /b 0
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
	echo.[Version] Completed Successfully
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	echo.[Version][WARNING] Completed Unsuccessfully
	endlocal
	exit /b 1
	goto EOF
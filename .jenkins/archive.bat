@echo off
setlocal EnableDelayedExpansion
:START
	goto RUN
	goto EOF
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
:RUN
:: ~~ FUNCTION CALLS
	call :CHECK_VARIABLES
	if %errorlevel% NEQ 0 goto END_FAILURE
	call :BIN_DIR
	if %errorlevel% NEQ 0 goto END_FAILURE
	call :COPY_ARTIFACTS
	if %errorlevel% NEQ 0 goto END_FAILURE
	if "!packArtifacts!"=="true" (
		call :RELEASE_DIR
		if %errorlevel% NEQ 0 goto END_FAILURE
		call :PACK_ARTIFACTS
		if %errorlevel% NEQ 0 goto END_FAILURE
	)
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ ERROR DECLARATIONS
:ERROR_FILE_NOT_FOUND
:: call :ERROR_FILE_NOT_FOUND "<IDENTIFIER>" "<FILE>"
	call :ERROR "ERROR_FILE_NOT_FOUND" %~1 "File not found '%~2'"
	goto END_FAILURE
	goto EOF
:ERROR_CREATE_DIR_FAILED
:: call :ERROR_CREATE_DIR_FAILED "<IDENTIFIER>" "<DIRECTORY>"
	call :ERROR "ERROR_CREATE_DIR_FAILED" %~1 "Could not create directory '%~2'"
	goto END_FAILURE
	goto EOF
:ERROR_COPY_FAILED
	call :ERROR "ERROR_COPY_FAILED" %~1 "Could not copy files"
	goto END_FAILURE
	goto EOF
:ERROR_ARCHIVE_FAILED
	call :ERROR "ERROR_ARCHIVE_FAILED" %~1 "Could not create archive '%~2'"
	goto END_FAILURE
	got EOF
:: ~~
::=============================================================================
:ERROR
:: call :ERROR "<ERROR_CODE>" "<IDENTIFIER>" "[<MESSAGES>]"
	if "%~1"=="" goto ERROR_ERROR
	if "%~2"=="" goto ERROR_ERROR
	for /f "tokens=1,2,* delims= " %%a in ("%*") do set ERROR_MSG=%%c
	echo.=================================================================
	echo.  %~1 [ %~nx0 :%~2 ] 
	echo.
	if !ERROR_MSG!=="" (
		echo.    No message provided
	) else for %%x in (!ERROR_MSG!) do (
		echo.    %%~x
	)
	echo.
	echo.=================================================================
	echo.
	exit /b 1
	goto EOF
	:ERROR_ERROR
	if !DEBUG! GEQ 1 echo.[DEBUG] ':ERROR' calls take at least 2 parameters
	if !DEBUG! EQU 0 echo.Unknown Error
	goto END_FAILURE
	goto EOF
::=============================================================================
:CLEANUP
	endlocal
	exit /b 0
	goto EOF
:END_SUCCESS
	call :CLEANUP
	exit /b 0
	goto EOF
:END_FAILURE
	call :CLEANUP
	exit /b 1
	goto EOF
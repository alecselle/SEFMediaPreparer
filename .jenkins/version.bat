@echo off
setlocal EnableDelayedExpansion
:RESET
	if "%~1"=="-r" call :RESET_SETTINGS && goto EOF
:START
	goto RUN
	goto EOF
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:VERIFY_DIR
	if not exist "%WORKSPACE%/.jenkins" mkdir "%WORKSPACE%/.jenkins" & attrib +h "%WORKSPACE%/.jenkins" /s /d
	if not exist "%WORKSPACE%/.jenkins/build.txt" echo|set /p="0">"%WORKSPACE%/.jenkins/build.txt"
	if not exist "%WORKSPACE%/.jenkins/version.txt" copy "%WORKSPACE%/version.txt" "%WORKSPACE%/.jenkins/"
	exit /b 0
	goto EOF
:RESET_SETTINGS
	if not exist "%WORKSPACE%/version.txt" call :ERROR_FILE_NOT_FOUND "RESET_SETTINGS" "version.txt" && exit /b 1
	set /p VERSION=<"%WORKSPACE%/version.txt"
	set BUILD=0
	echo|set /p="!BUILD!" >"!WORKSPACE!/.jenkins/build.txt"
	if %errorlevel% NEQ 0 exit /b 1
	echo|set /p="!VERSION!" >"!WORKSPACE!/.jenkins/version.txt"
	if %errorlevel% NEQ 0 exit /b 1
	exit /b 0
	goto EOF
:LOAD_SETTINGS
	if not exist "%WORKSPACE%/.jenkins/build.txt" call :ERROR_FILE_NOT_FOUND "LOAD_SETTINGS" ".jenkins/build.txt" && exit /b 1
	if not exist "%WORKSPACE%/.jenkins/version.txt" call :ERROR_FILE_NOT_FOUND "LOAD_SETTINGS" ".jenkins/version.txt" && exit /b 1
	if not exist "%WORKSPACE%/version.txt" call :ERROR_FILE_NOT_FOUND "LOAD_SETTINGS" "version.txt" && exit /b 1
	set /p BUILD_OLD=<"%WORKSPACE%/.jenkins/build.txt"
	set /p VERSION_OLD=<"%WORKSPACE%/.jenkins/version.txt"
	set /p VERSION_NEW=<"%WORKSPACE%/version.txt"
	exit /b 0
	goto EOF
:UPDATE_SETTINGS
	if "!VERSION_OLD!" NEQ "!VERSION_NEW!" (
		set VERSION=!VERSION_NEW!
		set BUILD=1
	) else (
		set VERSION=!VERSION_OLD!
		set /A BUILD=!BUILD_OLD!+1
	)
	if "!VERSION_OLD!" NEQ "!VERSION_NEW!" call :ERROR_PARSE_FAILED "UPDATE_SETTINGS" && exit /b 1
	exit /b 0
	goto EOF
:SAVE_SETTINGS
	echo|set /p="!BUILD!" >"!WORKSPACE!/.jenkins/build.txt"
	echo|set /p="!VERSION!" >"!WORKSPACE!/.jenkins/version.txt"
	set /p BUILD_TEMP=<"%WORKSPACE%/.jenkins/build.txt"
	set /p VERSION_TEMP=<"%WORKSPACE%/.jenkins/version.txt"
	if !BUILD_TEMP! NEQ !BUILD! call :ERROR_SAVE_FAILED "SAVE_SETTINGS" && exit /b 1
	if !VERSION_TEMP! NEQ !VERSION! call :ERROR_SAVE_FAILED "SAVE_SETTINGS" && exit /b 1
	exit /b 0
	goto EOF
:PARSE_VERSION
	call :PARSE_VERSION_OLD
	if %errorlevel% NEQ 0 exit /b 1
	call :PARSE_VERSION_NEW
	if %errorlevel% NEQ 0 exit /b 1
	call :PARSE_VERSION_DIFF
	if %errorlevel% NEQ 0 exit /b 1
	exit /b 0
	goto EOF
:PARSE_VERSION_OLD
	for /F "tokens=1,2,3 delims=." %%a in ("!VERSION_OLD!") do (
		set MAJOR_OLD=%%a
		set MINOR_OLD=%%b
		set PATCH_OLD=%%c
	)
	if %errorlevel% NEQ 0 call :ERROR_PARSE_FAILED "PARSE_VERSION_OLD" && exit /b 1
	set VERSION_OLD=!MAJOR_OLD!.!MINOR_OLD!.!PATCH_OLD!
	exit /b 0
	goto EOF
:PARSE_VERSION_NEW
	for /F "tokens=1,2,3 delims=." %%a in ("!VERSION_NEW!") do (
		set MAJOR_NEW=%%a
		set MINOR_NEW=%%b
		set PATCH_NEW=%%c
	)
	if %errorlevel% NEQ 0 call :ERROR_PARSE_FAILED "PARSE_VERSION_NEW" && exit /b 1
	set VERSION_NEW=!MAJOR_NEW!.!MINOR_NEW!.!PATCH_NEW!
	exit /b 0
	goto EOF
:PARSE_VERSION_DIFF
	if !MAJOR_NEW! GTR !MAJOR_OLD! set MAJOR_DIFF=1
	if !MAJOR_NEW! EQU !MAJOR_OLD! set MAJOR_DIFF=0
	if !MAJOR_NEW! LSS !MAJOR_OLD! set MAJOR_DIFF=-1
	if !PATCH_NEW! GTR !PATCH_OLD! set PATCH_DIFF=1
	if !PATCH_NEW! EQU !PATCH_OLD! set PATCH_DIFF=0
	if !PATCH_NEW! LSS !PATCH_OLD! set PATCH_DIFF=-1
	if !MINOR_NEW! GTR !MINOR_OLD! set MINOR_DIFF=1
	if !MINOR_NEW! EQU !MINOR_OLD! set MINOR_DIFF=0
	if !MINOR_NEW! LSS !MINOR_OLD! set MINOR_DIFF=-1
	if "!MAJOR_DIFF!"=="" call :ERROR_PARSE_FAILED "PARSE_VERSION_DIFF" && exit /b 1
	if "!MINOR_DIFF!"=="" call :ERROR_PARSE_FAILED "PARSE_VERSION_DIFF" && exit /b 1
	if "!PATCH_DIFF!"=="" call :ERROR_PARSE_FAILED "PARSE_VERSION_DIFF" && exit /b 1
	exit /b 0
	goto EOF
:DISPLAY
	call :DISPLAY_VERSION_DIFF
	if %errorlevel% NEQ 0 exit /b 1
	call :DISPLAY_SUMMARY
	exit /b 0
	goto EOF
:DISPLAY_VERSION_DIFF
	if !PATCH_DIFF! EQU 1 if !MINOR_DIFF! GEQ 0 if !MAJOR_DIFF! GEQ 0 echo.Upgrading to !VERSION_NEW! from !VERSION_OLD! && exit /b 0
	if !PATCH_DIFF! GEQ 0 if !MINOR_DIFF! EQU 1 if !MAJOR_DIFF! GEQ 0 echo.Upgrading to !VERSION_NEW! from !VERSION_OLD! && exit /b 0
	if !PATCH_DIFF! GEQ 0 if !MINOR_DIFF! GEQ 0 if !MAJOR_DIFF! EQU 1 echo.Upgrading to !VERSION_NEW! from !VERSION_OLD! && exit /b 0
	if !PATCH_DIFF! EQU 0 if !MINOR_DIFF! EQU 0 if !MAJOR_DIFF! EQU 0 echo.Rebuilding !VERSION_OLD! && exit /b 0
	if !PATCH_DIFF! EQU -1 echo.Downgrading to !VERSION_NEW! from !VERSION_OLD! && exit /b 0
	if !MINOR_DIFF! EQU -1 echo.Downgrading to !VERSION_NEW! from !VERSION_OLD! && exit /b 0
	if !MAJOR_DIFF! EQU -1 echo.Downgrading to !VERSION_NEW! from !VERSION_OLD! && exit /b 0
	call :ERROR_PARSE_FAILED "DISPLAY_VERSION_DIFF"
	exit /b 1
	goto EOF
:DISPLAY_SUMMARY
	echo.
	echo.Previous Version : !VERSION_OLD!
	echo.Current Version  : !VERSION!
	echo.Build Number     : !BUILD!
	echo.
	exit /b 0
	goto EOF
:: ~~
::=============================================================================
:RUN
:: ~~ FUNCTION CALLS
	echo.[Version] Running Script (0/6)
	if not exist "%WORKSPACE%/version.txt" call :ERROR_FILE_NOT_FOUND "RUN" "version.txt"
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Version] Verifying Directory (1/6)
	call :VERIFY_DIR
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Version] Loading Data (2/6)
	call :LOAD_SETTINGS
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Version] Parsing Data (3/6)
	call :PARSE_VERSION
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Version] Checking Versions (4/6)
	call :UPDATE_SETTINGS
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Version] Saving Data (5/6)
	call :SAVE_SETTINGS
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Version] Displaying Summary (6/6)
	call :DISPLAY
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Version] Completed Successfully
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
:ERROR_PARSE_FAILED
:: call :ERROR_PARSE_FAILED "<IDENTIFIER>"
	call :ERROR "ERROR_PARSE_FAILED" %~1 "Failed to parse version"
	goto END_FAILURE
	goto EOF
:ERROR_SAVE_FAILED
:: call :ERROR_SAVE_FAILED "<IDENTIFIER>"
	call :ERROR "ERROR_SAVE_FAILED" %~1 "Failed to save version to file"
	goto END_FAILURE
	goto EOF
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
	if !DEBUG! GEQ 1 echo.[Version][DEBUG] ':ERROR' calls take at least 2 parameters
	if !DEBUG! EQU 0 echo.[Version][WARNING] Unknown Error
	goto END_FAILURE
	goto EOF
::=============================================================================
:END_SUCCESS
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	endlocal
	exit /b 1
	goto EOF
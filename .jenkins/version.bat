@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set DEBUG=0
:RESET
	if "%~1"=="-r" goto :RESET_SETTINGS
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Version] Running Script (0/6)
	
	echo.[Version] Checking Variables (1/5)
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Version] Loading Data (2/5)
	call :LOAD_SETTINGS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Version] Parsing Data (3/5)
	call :PARSE_VERSION
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Version] Checking Versions (4/5)
	call :UPDATE_SETTINGS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Version] Saving Data (5/5)
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
	if "!WORKSPACE!"=="" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if not exist "!WORKSPACE!/version.txt" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "version.txt"
	if not exist "%WORKSPACE%/.jenkins" mkdir "%WORKSPACE%/.jenkins" & attrib +h "%WORKSPACE%/.jenkins" /s /d
	if not exist "%WORKSPACE%/.jenkins/build.txt" echo|set /p="0">"%WORKSPACE%/.jenkins/build.txt"
	if not exist "%WORKSPACE%/.jenkins/version.txt" call xcopy "%WORKSPACE%\version.txt" "%WORKSPACE%\.jenkins\"
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:CHECK_VARIABLES] WORKSPACE=!WORKSPACE!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:CHECK_VARIABLES] ERROR_LEVEL=!ERROR_LEVEL!
	exit /b !ERROR_LEVEL!
	goto EOF
:LOAD_SETTINGS
	if not exist "%WORKSPACE%/.jenkins/build.txt" call :ERROR_FILE_NOT_FOUND "LOAD_SETTINGS" ".jenkins/build.txt"
	if not exist "%WORKSPACE%/.jenkins/version.txt" call :ERROR_FILE_NOT_FOUND "LOAD_SETTINGS" ".jenkins/version.txt"
	if not exist "%WORKSPACE%/version.txt" call :ERROR_FILE_NOT_FOUND "LOAD_SETTINGS" "version.txt"
	set /p BUILD_OLD=<"%WORKSPACE%/.jenkins/build.txt"
	set /p VERSION_OLD=<"%WORKSPACE%/.jenkins/version.txt"
	set /p VERSION_NEW=<"%WORKSPACE%/version.txt"
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:LOAD_SETTINGS] BUILD_OLD=!BUILD_OLD!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:LOAD_SETTINGS] VERSION_OLD=!VERSION_OLD!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:LOAD_SETTINGS] VERSION_NEW=!VERSION_NEW!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:LOAD_SETTINGS] ERROR_LEVEL=!ERROR_LEVEL!
	exit /b !ERROR_LEVEL!
	goto EOF
:UPDATE_SETTINGS
	if "!VERSION_NEW!" EQU "!VERSION_OLD!" set /a BUILD_NEW=!BUILD_OLD! + 1
	if "!VERSION_NEW!" NEQ "!VERSION_OLD!" set BUILD_NEW=1
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:UPDATE_SETTINGS] VERSION_NEW=!VERSION_NEW!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:UPDATE_SETTINGS] BUILD_NEW=!BUILD_NEW!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:UPDATE_SETTINGS] ERROR_LEVEL=!ERROR_LEVEL!
	exit /b !ERROR_LEVEL!
	goto EOF
:SAVE_SETTINGS
	echo|set /p="!BUILD_NEW!" >"!WORKSPACE!/.jenkins/build.txt"
	echo|set /p="!VERSION_NEW!" >"!WORKSPACE!/.jenkins/version.txt"
	set /p BUILD_TEMP=<"%WORKSPACE%/.jenkins/build.txt"
	set /p VERSION_TEMP=<"%WORKSPACE%/.jenkins/version.txt"
	if !BUILD_TEMP! NEQ !BUILD_NEW! call :ERROR_SAVE_FAILED "SAVE_SETTINGS" 
	if !VERSION_TEMP! NEQ !VERSION_NEW! call :ERROR_SAVE_FAILED "SAVE_SETTINGS"
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:SAVE_SETTINGS] BUILD_TEMP=!BUILD_TEMP!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:SAVE_SETTINGS] BUILD_NEW=!BUILD_NEW!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:SAVE_SETTINGS] VERSION_TEMP=!VERSION_TEMP!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:SAVE_SETTINGS] VERSION_NEW=!VERSION_NEW!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:SAVE_SETTINGS] ERROR_LEVEL=!ERROR_LEVEL!
	exit /b !ERROR_LEVEL!
	goto EOF
:PARSE_VERSION
	for /F "tokens=1,2,3 delims=." %%a in ("!VERSION_OLD!") do (
		set MAJOR_OLD=%%a
		set MINOR_OLD=%%b
		set PATCH_OLD=%%c
	)
	if %errorlevel% NEQ 0 call :ERROR_PARSE_FAILED "PARSE_VERSION_OLD"
	if !ERROR_LEVEL! NEQ 0 exit /b !ERROR_LEVEL! && goto EOF
	set VERSION_OLD=!MAJOR_OLD!.!MINOR_OLD!.!PATCH_OLD!
	for /F "tokens=1,2,3 delims=." %%a in ("!VERSION_NEW!") do (
		set MAJOR_NEW=%%a
		set MINOR_NEW=%%b
		set PATCH_NEW=%%c
	)
	if %errorlevel% NEQ 0 call :ERROR_PARSE_FAILED "PARSE_VERSION_NEW"
	if !ERROR_LEVEL! NEQ 0 exit /b !ERROR_LEVEL! && goto EOF
	set VERSION_NEW=!MAJOR_NEW!.!MINOR_NEW!.!PATCH_NEW!
	if !MAJOR_NEW! GTR !MAJOR_OLD! set MAJOR_DIFF=1
	if !MAJOR_NEW! EQU !MAJOR_OLD! set MAJOR_DIFF=0
	if !MAJOR_NEW! LSS !MAJOR_OLD! set MAJOR_DIFF=-1
	if !MINOR_NEW! GTR !MINOR_OLD! set MINOR_DIFF=1
	if !MINOR_NEW! EQU !MINOR_OLD! set MINOR_DIFF=0
	if !MINOR_NEW! LSS !MINOR_OLD! set MINOR_DIFF=-1
	if !PATCH_NEW! GTR !PATCH_OLD! set PATCH_DIFF=1
	if !PATCH_NEW! EQU !PATCH_OLD! set PATCH_DIFF=0
	if !PATCH_NEW! LSS !PATCH_OLD! set PATCH_DIFF=-1
	if "!MAJOR_DIFF!"=="" call :ERROR_PARSE_FAILED "PARSE_VERSION_DIFF"
	if "!MINOR_DIFF!"=="" call :ERROR_PARSE_FAILED "PARSE_VERSION_DIFF"
	if "!PATCH_DIFF!"=="" call :ERROR_PARSE_FAILED "PARSE_VERSION_DIFF"
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:PARSE_VERSION] VERSION_OLD=!VERSION_OLD!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:PARSE_VERSION] VERSION_NEW=!VERSION_NEW!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:PARSE_VERSION] MAJOR_DIFF=!MAJOR_DIFF!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:PARSE_VERSION] MINOR_DIFF=!MINOR_DIFF!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:PARSE_VERSION] PATCH_DIFF=!PATCH_DIFF!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:PARSE_VERSION] ERROR_LEVEL=!ERROR_LEVEL!
	exit /b !ERROR_LEVEL!
	goto EOF
:DISPLAY
	echo.
	if !PATCH_DIFF! EQU 1 if !MINOR_DIFF! GEQ 0 if !MAJOR_DIFF! GEQ 0 echo.[Version] Upgrading to !VERSION_NEW!-!BUILD_NEW! from !VERSION_OLD!-!BUILD_OLD!
	if !PATCH_DIFF! GEQ 0 if !MINOR_DIFF! EQU 1 if !MAJOR_DIFF! GEQ 0 echo.[Version] Upgrading to !VERSION_NEW!-!BUILD_NEW! from !VERSION_OLD!-!BUILD_OLD!
	if !PATCH_DIFF! GEQ 0 if !MINOR_DIFF! GEQ 0 if !MAJOR_DIFF! EQU 1 echo.[Version] Upgrading to !VERSION_NEW!-!BUILD_NEW! from !VERSION_OLD!-!BUILD_OLD!
	if !PATCH_DIFF! EQU 0 if !MINOR_DIFF! EQU 0 if !MAJOR_DIFF! EQU 0 echo.[Version] Rebuilding !VERSION_NEW!-!BUILD_NEW!
	if !PATCH_DIFF! EQU -1 echo.[Version] Downgrading to !VERSION_NEW!-!BUILD_NEW! from !VERSION_OLD!-!BUILD_OLD!
	if !MINOR_DIFF! EQU -1 echo.[Version] Downgrading to !VERSION_NEW!-!BUILD_NEW! from !VERSION_OLD!-!BUILD_OLD!
	if !MAJOR_DIFF! EQU -1 echo.[Version] Downgrading to !VERSION_NEW!-!BUILD_NEW! from !VERSION_OLD!-!BUILD_OLD!
	if !DEBUG! GEQ 1 echo.[Version][DEBUG][:DISPLAY] ERROR_LEVEL=!ERROR_LEVEL!
	echo.
	exit /b !ERROR_LEVEL!
	goto EOF
:RESET_SETTINGS
	if not exist "%WORKSPACE%/version.txt" (
		call :ERROR_FILE_NOT_FOUND "RESET_SETTINGS" "version.txt"
	) else (
		set /p VERSION_NEW=<"%WORKSPACE%/version.txt"
		set BUILD_NEW=0
		echo|set /p="!BUILD_NEW!" >"!WORKSPACE!/.jenkins/build.txt"
		echo|set /p="!VERSION_NEW!" >"!WORKSPACE!/.jenkins/version.txt"
	)
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
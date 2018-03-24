@echo off
setlocal EnableDelayedExpansion
:PROJECT
	set PROJECT_DEFAULT=SEFMediaPreparer.pro
	if "%~1" NEQ "" set PROJECT=%~1 && goto START
	set PROJECT=!PROJECT_DEFAULT! && goto START
:START
	goto RUN
	goto EOF
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!"=="" (
		echo.[Build][WARNING] Variable not initialized. 'WORKSPACE'
		cd "%~dp0/.."
		set WORKSPACE=%CD%
		if not exist "!WORKSPACE!/!PROJECT!" (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
			exit /b 1
			goto EOF
		) else (
			echo.[Build][WARNING] Using directory. '%WORKSPACE%'
		)
	)
	if not exist "!WORKSPACE!/!PROJECT!" (
		if not exist "!WORKSPACE!/!PROJECT_DEFAULT!" (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
			exit /b 1
			goto EOF
		) else (
			echo.[Build][WARNING] Project '!PROJECT!' does not exist
			echo.[Build][WARNING] Project '!PROJECT_DEFAULT!' does
			set PROJECT=!PROJECT_DEFAULT!
			echo.[Build][WARNING] Using '!PROJECT!'
		)
	)
	if "!QMAKE!"=="" (
		echo.[Build][WARNING] Variable not initialized. 'QMAKE'
		call where /q qmake.exe
		if %errorlevel% NEQ 0 (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "qmake.exe"
			exit /b 1
			goto EOF
		) else (
			for /f %%i in ('where qmake.exe') do set QMAKE=%%i
			echo.[Build][WARNING] Using '!QMAKE!' from PATH
		)
	)
	if "!MINGW!"=="" (
		echo.[Build][WARNING] Variable not initialized. 'MINGW'
		call where /q mingw32-make.exe
		if %errorlevel% NEQ 0 (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "mingw32-make.exe"
			exit /b 1
			goto EOF
		) else (
			for /f %%i in ('where mingw32-make.exe') do set MINGW=%%i
			echo.[Build][WARNING] Using '!MINGW!' from PATH
		)
	)
	echo %errorlevel%
	exit /b 0
	goto EOF
:BUILD_DIR
	if not exist "!WORKSPACE!/build" mkdir "!WORKSPACE!/build"
	cd "!WORKSPACE!/build"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "BUILD_DIR" "!WORKSPACE!/build" && exit /b 1
	exit /b 0
	goto EOF
:QMAKE
	call :BUILD_DIR
	echo.[Build] "!QMAKE!" !WORKSPACE!/!PROJECT! -spec win32-g++ "CONFIG+=release" -Wnone
	call "!QMAKE!" !WORKSPACE!/!PROJECT! -spec win32-g++ "CONFIG+=release" -Wnone
	if !errorlevel! NEQ 0 call :ERROR_BUILD_FAILED "QMAKE" "Qmake returned an error" "Check output for details" && exit /b 1\
	exit /b 0
	goto EOF
:MINGW
	call :BUILD_DIR
	echo.[Build] "!MINGW!" -s -i -j 4 -B
	call "!MINGW!" -s -i -j 4 -B
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "MINGW" "MinGW returned an error" "Check output for details" && exit /b 1
	exit /b 0
	goto EOF
:: ~~
::=============================================================================
:RUN
:: ~~ FUNCTION CALLS
	echo.[Build] Running Script (0/3)
	echo.[Build] Checking Variables (1/3) 
	call :CHECK_VARIABLES
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Build] Running qmake (2/3)
	call :QMAKE
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Build] Running mingw32-make (3/3)
	call :MINGW
	if %errorlevel% NEQ 0 goto END_FAILURE
	echo.[Build] Completed Successfully
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ UTILITY FUNCTIONS
:TRIM
	for /f "tokens=* delims= " %%a in ("%~2") do set "%~1=%%a"
	echo.[Build][DEBUG] TRIM : %~1
	echo.[Build][DEBUG] TRIM : %~2
	for /l %%a in (1,1,100) do if "!~1:~-1!"==" " set "%~1=!~1:~0,-1!"
	echo.[Build][DEBUG] TRIM : %~1
	echo.[Build][DEBUG] TRIM : %~2
	exit /b 0
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
:ERROR_BUILD_FAILED
:: call :ERROR_BUILD_FAILED "<IDENTIFIER>" "[<MESSAGES>]"
	call :ERROR "ERROR_BUILD_FAILED" %*
	goto END_FAILURE
	goto EOF
:: ~~
::=============================================================================
:ERROR
:: call :ERROR "<ERROR_CODE>" "<IDENTIFIER>" "[<MESSAGES>]"
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
	if !DEBUG! GEQ 1 echo.[Build][DEBUG] ':ERROR' calls take at least 2 parameters
	if !DEBUG! EQU 0 echo.[Build][WARNING]Unknown Error
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
@echo off
setlocal EnableDelayedExpansion
:PROJECT
	set PROJECT_DEFAULT=SEFMediaPreparer.pro
	if "%~1" NEQ "" ( set PROJECT=%~1 )
	else ( set PROJECT=!PROJECT_DEFAULT! )
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
		) else (
			echo.[WARNING] Using current directory. '%CD%'
		)
	)
	if not exist "!WORKSPACE!/!PROJECT!" (
		if not exist "!WORKSPACE!/!PROJECT_DEFAULT!" (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
			exit /b 1
		) else (
			echo.[WARNING] Project '!PROJECT!' does not exist
			echo.[WARNING] Project '!PROJECT_DEFAULT!' does
			set PROJECT=!PROJECT_DEFAULT!
			echo.[WARNING] Using '!PROJECT!'
		)
	)
	if "!QMAKE!"=="" (
		echo.[WARNING] Variable not initialized. 'QMAKE'
		call where /q qmake.exe
		if %errorlevel% NEQ 0 (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "qmake.exe"
			exit /b 1
		) else (
			for /f %%i in ('where qmake.exe') do set QMAKE=%%i
			echo.[WARNING] Using '!QMAKE!' from PATH
		)
	)
	if "!MINGW!"=="" (
		echo.[WARNING] Variable not initialized. 'MINGW'
		call where /q mingw32-make.exe
		if %errorlevel% NEQ 0 (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "mingw32-make.exe"
			exit /b 1
		) else (
			for /f %%i in ('where mingw32-make.exe') do set MINGW=%%i
			echo.[WARNING] Using '!MINGW!' from PATH
		)
	)
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
	echo.Running qmake...
	call "!QMAKE!" "!WORKSPACE!/%PROJECT%" -spec win32-g++ "CONFIG+=release" -Wnone
	if !errorlevel! NEQ 0 call :ERROR_BUILD_FAILED "QMAKE" "Qmake returned an error" "Check output for details" && exit /b 1\
	exit /b 0
	goto EOF
:MINGW
	call :BUILD_DIR
	echo.Running mingw32-make...
	call "!MINGW!" -std=c++11 -s -i -j 4 -B
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "MINGW" "MinGW returned an error" "Check output for details" && exit /b 1
	exit /b 0
	goto EOF
:: ~~
::=============================================================================
:RUN
:: ~~ FUNCTION CALLS
	call :CHECK_VARIABLES
	if %errorlevel% NEQ 0 goto END_FAILURE
	call :QMAKE
	if %errorlevel% NEQ 0 goto END_FAILURE
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
@echo off
setlocal EnableDelayedExpansion
:PROJECT
	set PROJECT_DEFAULT=SEFMediaPreparer.pro
	if "%~1" NEQ "" set PROJECT=%~1
	if "%~1" EQU "" set PROJECT=!PROJECT_DEFAULT!
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Build] Running Script (0/3)
	
	echo.[Build] Checking Variables (1/3) 
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Build] Running qmake (2/3)
	call :QMAKE
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Build] Running mingw32-make (3/3)
	call :MINGW
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_WORKSPACE
	if "!WORKSPACE!"=="" (
		echo.[Build][WARNING] Variable not initialized. 'WORKSPACE'
		set WORKSPACE=%~dp0/..
		echo.[Build][DEBUG] Using directory. '!WORKSPACE!'
		if not exist "!WORKSPACE!/!PROJECT!" (
			call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
			exit /b 1
			goto EOF
		) else (
			echo.[Build][WARNING] Using directory. '%WORKSPACE%'
		)
	)
	exit /b 0
	goto EOF
:CHECK_PROJECT
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
	exit /b 0
	goto EOF
:CHECK_QMAKE
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
	exit /b 0
	goto EOF
:CHECK_MINGW
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
	exit /b 0
	goto EOF
:CHECK_VARIABLES
	call :CHECK_WORKSPACE
	if %errorlevel% NEQ 0 exit /b 1 && goto EOF
	call :CHECK_PROJECT
	if %errorlevel% NEQ 0 exit /b 1 && goto EOF
	call :CHECK_QMAKE
	if %errorlevel% NEQ 0 exit /b 1 && goto EOF
	call :CHECK_MINGW
	if %errorlevel% NEQ 0 exit /b 1 && goto EOF
	exit /b 0
	goto EOF
:BUILD_DIR
	if not exist "!WORKSPACE!/build" mkdir "!WORKSPACE!/build"
	cd "!WORKSPACE!/build"
	if %errorlevel% NEQ 0 (
		call :ERROR_CREATE_DIR_FAILED "BUILD_DIR" "!WORKSPACE!/build"
		exit /b 1
		goto EOF
	)
	exit /b 0
	goto EOF
:QMAKE
	call :BUILD_DIR
	echo.[Build] "!QMAKE!" -spec win32-g++ "CONFIG+=release" -Wnone !WORKSPACE!/!PROJECT!
	call "!QMAKE!" -spec win32-g++ "CONFIG+=release" -Wnone !WORKSPACE!/!PROJECT!
	if !errorlevel! NEQ 0 (
		call :ERROR_BUILD_FAILED "QMAKE" "Qmake returned an error" "Check output for details"
		exit /b 1
		goto EOF
	)
	exit /b 0
	goto EOF
:MINGW
	call :BUILD_DIR
	echo.[Build] "!MINGW!" -s -i -j 4 -B
	call "!MINGW!" -s -i -j 4 -B
	if %errorlevel% NEQ 0 (
		call :ERROR_BUILD_FAILED "MINGW" "MinGW returned an error" "Check output for details" 
		exit /b 1
		goto EOF
	)
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
:ERROR_BUILD_FAILED
:: call :ERROR_BUILD_FAILED "<IDENTIFIER>" "[<MESSAGES>]"
	call :ERROR "ERROR_BUILD_FAILED" %*
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
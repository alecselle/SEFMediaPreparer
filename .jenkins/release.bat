@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set DEBUG=0
:PROJECT
	set PROJECT_DEFAULT=SEFMediaPreparer.pro
	if "%~1" NEQ "" set PROJECT=%~1
	if "%~1" EQU "" set PROJECT=!PROJECT_DEFAULT!
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Release] Running Script (0/6)
	
	echo.[Release] Checking Variables (1/6) 
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Running qmake (2/6)
	call :QMAKE
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Running mingw32-make (3/6)
	call :MINGW
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Running windeployqt (4/6)
	call :WINDEPLOY
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Copying Artifacts (5/6)
	call :COPY_ARTIFACTS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Creating Installer (6/6)
	call :BINARYCREATOR
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!"=="" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if not exist "!WORKSPACE!/!PROJECT!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
	if not exist "!WORKSPACE!/!PROJECT!" (
		if not exist "!WORKSPACE!/!PROJECT_DEFAULT!" call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "!PROJECT!"
		set PROJECT=!PROJECT_DEFAULT!
	)
	if "!QMAKE!"=="" (
		call where /q qmake.exe 2>&1nul
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "qmake.exe"
		for /f %%i in ('where qmake.exe') do set QMAKE=%%i
	)
	if "!MINGW!"=="" (
		call where /q mingw32-make.exe 2>&1nul
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "mingw32-make.exe"
		for /f %%i in ('where mingw32-make.exe') do set MINGW=%%i
	)
	if "!WINDEPLOY!"=="" (
		call where /q windeployqt.exe 2>&1nul
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "windeployqt.exe"
		for /f %%i in ('where windeployqt.exe') do set WINDEPLOY=%%i
	)
	if "!BINARYCREATOR!"=="" (
		call where /q binarycreator.exe 2>&1nul
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "binarycreator.exe"
		for /f %%i in ('where binarycreator.exe') do set BINARYCREATOR=%%i
	)
	exit /b !ERROR_LEVEL!
	goto EOF
:BUILD_DIR
	if not exist "!WORKSPACE!/bin" mkdir "!WORKSPACE!/bin"
	cd "!WORKSPACE!/bin"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "BUILD_DIR" "!WORKSPACE!/bin"
	if not exist "!WORKSPACE!/build" mkdir "!WORKSPACE!/build"
	cd "!WORKSPACE!/build"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "BUILD_DIR" "!WORKSPACE!/build"
	exit /b !ERROR_LEVEL!
	goto EOF
:QMAKE
	call :BUILD_DIR
	echo.[Release] "!QMAKE!" -spec win32-g++ "CONFIG+=release"
	call "!QMAKE!" -spec win32-g++ "CONFIG+=release" "!WORKSPACE!/!PROJECT!"
	if !errorlevel! NEQ 0 call :ERROR_BUILD_FAILED "QMAKE" "Qmake returned an error" "Check output for details"
	exit /b !ERROR_LEVEL!
	goto EOF
:MINGW
	call :BUILD_DIR
	echo.[Release] "!MINGW!" -B
	call "!MINGW!" -w -s -j 2 -B> "%~dp0/mingw.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "MINGW" "MinGW returned an error" "Check output for details" 
	exit /b !ERROR_LEVEL!
	goto EOF
:WINDEPLOY
	call :BUILD_DIR
	echo.[Release] "!WINDEPLOY!" "SEFMediaPreparer.exe" --release --dir "!WORKSPACE!/bin"
	call "!WINDEPLOY!" "!WORKSPACE!/build/release/SEFMediaPreparer.exe" --release --dir "!WORKSPACE!/bin"> "%~dp0/windeploy.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "WINDEPLOY" "WinDeployQt returned an error" "Check output for details" 
	exit /b !ERROR_LEVEL!
	goto EOF
:BINARYCREATOR
	cd "!WORKSPACE!"
	echo.[Release] "!BINARYCREATOR!" "SEFMediaPreparer.exe" -c config\config.xml -p packages SEFMediaPreparer-Setup.exe
	call "!BINARYCREATOR!" -c "!WORKSPACE!\installer\config\config.xml" -p "!WORKSPACE!\installer\packages" SEFMediaPreparer-Setup.exe> "%~dp0/binarycreator.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "BINARYCREATOR" "BinaryCreator returned an error" "Check output for details" 
	exit /b !ERROR_LEVEL!
	goto EOF
:COPY_ARTIFACTS
	call xcopy /Y "!WORKSPACE!\build\release\SEFMediaPreparer.exe" "!WORKSPACE!\bin\"
	if %errorlevel% NEQ 0 call :ERROR_COPY_FAILED "COPY_ARTIFACTS"
	call xcopy /Y "!WORKSPACE!\tools\ffmpeg.exe" "!WORKSPACE!\bin\"
	if %errorlevel% NEQ 0 call :ERROR_COPY_FAILED "COPY_ARTIFACTS"
	call xcopy /Y "!WORKSPACE!\tools\ffprobe.exe" "!WORKSPACE!\bin\"
	if %errorlevel% NEQ 0 call :ERROR_COPY_FAILED "COPY_ARTIFACTS"
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
:: call :ERROR_BUILD_FAILED "<IDENTIFIER>" "[<MESSAGES>]"
	call :ERROR "ERROR_BUILD_FAILED" %*
	exit /b 1
	goto EOF
:ERROR_COPY_FAILED
	call :ERROR "ERROR_COPY_FAILED" %~1 "Could not copy files"
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
	echo.[Release] Completed Successfully
	endlocal
	exit /b 0
	goto EOF
:END_FAILURE
	echo.[Release][WARNING] Completed Unsuccessfully
	endlocal
	pause
	exit /b 1
	goto EOF
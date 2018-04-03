@echo off
setlocal EnableDelayedExpansion
set ERROR_LEVEL=0
set DEBUG=0
::=============================================================================
:: ~~ FUNCTION CALLS
:RUN
	echo.[Release] Running Script (0/5)
	
	echo.[Release] Checking Variables (1/5) 
	call :CHECK_VARIABLES
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE

	echo.[Release] Copying Artifacts (2/5)
	call :COPY_ARTIFACTS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Creating Installer Directories (2/5)
	call :INSTALLER_DIRS
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Preparing Installer Files (5/5)
	call :INSTALLER_PREPARE
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	echo.[Release] Creating Installer (5/5)
	call :BINARYCREATOR
	if !ERROR_LEVEL! NEQ 0 goto END_FAILURE
	
	goto END_SUCCESS
	goto EOF
:: ~~
::=============================================================================
:: ~~ FUNCTION DECLARATIONS
:CHECK_VARIABLES
	if "!WORKSPACE!"=="" cd "%~dp0"& cd ..& set WORKSPACE=!CD!
	if "!BINARYCREATOR!"=="" (
		call where /q binarycreator.exe 2>&1nul
		if %errorlevel% NEQ 0 call :ERROR_FILE_NOT_FOUND "CHECK_VARIABLES" "binarycreator.exe"
		for /f %%i in ('where binarycreator.exe') do set BINARYCREATOR=%%i
	)
	set /p VERSION=<"%WORKSPACE%/version.txt"
	for /f "tokens=1-4 delims=/ " %%i in ("%date%") do (
		set dow=%%i
		set month=%%j
		set day=%%k
		set year=%%l
	)
	set DATESTR=!year!-!month!-!day!
	exit /b !ERROR_LEVEL!
	goto EOF

:INSTALLER_DIRS
	if exist "!WORKSPACE!/installer" call rmdir /S /Q "!WORKSPACE!/installer"
	if exist "!WORKSPACE!/SEFMediaPreparer-Setup.exe" call del "!WORKSPACE!/SEFMediaPreparer-Setup.exe"
	if not exist "!WORKSPACE!/installer" mkdir "!WORKSPACE!/installer"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "INSTALLER_PREPARE" "installer"
	if not exist "!WORKSPACE!/installer/config" mkdir "!WORKSPACE!/installer/config"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "INSTALLER_PREPARE" "installer/config"
	if not exist "!WORKSPACE!/installer/packages" mkdir "!WORKSPACE!/installer/packages"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "INSTALLER_PREPARE" "installer/packages"
	if not exist "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer" mkdir "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "INSTALLER_PREPARE" "installer/packages/com.superepicfuntime.sefmediapreparer"
	if not exist "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer/meta" mkdir "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer/meta"
	if %errorlevel% NEQ 0 call :ERROR_CREATE_DIR_FAILED "INSTALLER_PREPARE" "installer/packages/com.superepicfuntime.sefmediapreparer/meta"
	exit /b !ERROR_LEVEL!
	goto EOF
:INSTALLER_PREPARE
	echo.^<?xml version="1.0" encoding="UTF-8"?^>^<Installer^>^<Name^>SEFMediaPreparer^</Name^>^<Version^>1.0.0^</Version^>^<Title^>SEFMediaPreparer Installer^</Title^>^<Publisher^>SuperEpicFuntime^</Publisher^>^<StartMenuDir^>SuperEpicFuntime^</StartMenuDir^>^<TargetDir^>@HomeDir@/AppData/Roaming/SuperEpicFuntime/SEFMediaPreparer/bin^</TargetDir^>^</Installer^>> "!WORKSPACE!/installer/config/config.xml" 2>&1
	if not exist "!WORKSPACE!/installer/config/config.xml" call :ERROR_FILE_NOT_FOUND "INSTALLER_PREPARE" "config.xml"
	echo.^<?xml version="1.0" encoding="UTF-8"?^>^<Package^>^<DisplayName^>SEFMediaPreparer^</DisplayName^>^<Description^>Core^</Description^>^<Version^>!VERSION!^</Version^>^<ReleaseDate^>!DATESTR!^</ReleaseDate^>^</Package^>> "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer/meta/package.xml" 2>&1
	if not exist "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer/meta/package.xml" call :ERROR_FILE_NOT_FOUND "INSTALLER_PREPARE" "package.xml"
	if exist "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer/data" rmdir /S /Q "!WORKSPACE!/installer/packages/com.superepicfuntime.sefmediapreparer/data"
	call mklink /J "!WORKSPACE!\installer\packages\com.superepicfuntime.sefmediapreparer\data\" "!WORKSPACE!\bin\" 
	if %errorlevel% NEQ 0 call :ERROR_SYMLINK_FAILED "INSTALLER_PREPARE"
	exit /b !ERROR_LEVEL!
	goto EOF
:BINARYCREATOR
	cd "!WORKSPACE!"
	echo.[Release] "!BINARYCREATOR!" SEFMediaPreparer-Setup.exe
	call "!BINARYCREATOR!" -c "!WORKSPACE!/installer/config/config.xml" -p "!WORKSPACE!/installer/packages" SEFMediaPreparer-Setup.exe> "%~dp0/binarycreator.log" 2>&1
	if %errorlevel% NEQ 0 call :ERROR_BUILD_FAILED "BINARYCREATOR" "BinaryCreator returned an error" "Check output for details" 
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
:: call :ERROR_BUILD_FAILED "<IDENTIFIER>" "<EXECUTABLE>"
	call :ERROR "ERROR_BUILD_FAILED" "%~1" "%~2 returned an error" "Check log for details"
	if exist "!WORKSPACE!/.jenkins/%~2.log" echo>"!WORKSPACE!/.jenkins/%~2.log"
	exit /b 1
	goto EOF
:ERROR_COPY_FAILED
	call :ERROR "ERROR_COPY_FAILED" %~1 "Could not copy files"
	exit /b 1
	goto EOF
:ERROR_SYMLINK_FAILED
	call :ERROR "ERROR_SYMLINK_FAILED" %~1 "Could create directory junction"
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
@echo off
setlocal EnableDelayedExpansion
if exist "%WORKSPACE%/version.txt" (
	if not exist "%WORKSPACE%/.jenkins" mkdir "%WORKSPACE%/.jenkins" & attrib +h "%WORKSPACE%/.jenkins" /s /d
	if not exist "%WORKSPACE%/.jenkins/build.txt" echo|set /p="0">"%WORKSPACE%/.jenkins/build.txt"
	if not exist "%WORKSPACE%/.jenkins/version.txt" copy "%WORKSPACE%/version.txt" "%WORKSPACE%/.jenkins/"

	set /p BUILD_NUMBER=<"%WORKSPACE%/.jenkins/build.txt"
	set /p BUILD_VERSION=<"%WORKSPACE%/.jenkins/version.txt"
	set /p SOURCE_VERSION=<"%WORKSPACE%/version.txt"

	for /F "tokens=1,2,3 delims=." %%a in ("!BUILD_VERSION!") do (
		set BUILD_MAJOR=%%a
		set BUILD_MINOR=%%b
		set BUILD_PATCH=%%c
	)
	set BUILD_VERSION=!BUILD_MAJOR!.!BUILD_MINOR!.!BUILD_PATCH!
	for /F "tokens=1,2,3 delims=." %%a in ("!SOURCE_VERSION!") do (
		set SOURCE_MAJOR=%%a
		set SOURCE_MINOR=%%b
		set SOURCE_PATCH=%%c
	)
	set SOURCE_VERSION=!SOURCE_MAJOR!.!SOURCE_MINOR!.!SOURCE_PATCH!
	
	if !SOURCE_PATCH! GTR !BUILD_PATCH! set PATCH=1
	else if !SOURCE_PATCH! EQU !BUILD_PATCH! set PATCH=0
	else if !SOURCE_PATCH! LES !BUILD_PATCH! set PATCH=-1
	
	if !SOURCE_MINOR! GTR !BUILD_MINOR! set MINOR=1
	else if !SOURCE_MINOR! EQU !BUILD_MINOR! set MINOR=0
	else if !SOURCE_MINOR! LES !BUILD_MINOR! set MINOR=-1
	
	if !SOURCE_MAJOR! GTR !BUILD_MAJOR! set MAJOR=1
	else if !SOURCE_MAJOR! EQU !BUILD_MAJOR! set MAJOR=0
	else if !SOURCE_MAJOR! LES !BUILD_MAJOR! set MAJOR=-1
	
	if !PATCH! EQU 1 ( if !MINOR! GEQ 0 if !MAJOR! GEQ 0 
		echo.Upgrading to %SOURCE_VERSION% from %BUILD_VERSION%
	) else if !PATCH! GEQ 0 if !MINOR! EQU 1 ( if !MAJOR! GEQ 1
		echo.Upgrading to %SOURCE_VERSION% from %BUILD_VERSION%
	) else if !PATCH! GEQ 0 if !MINOR! GEQ 0 if !MAJOR! EQU 1 (
		echo.Upgrading to %SOURCE_VERSION% from %BUILD_VERSION%
	) else if !PATCH! EQU -1 (
		echo.Downgrading to %SOURCE_VERSION% from %BUILD_VERSION%
	) else if !MINOR! EQU -1 (
		echo.Downgrading to %SOURCE_VERSION% from %BUILD_VERSION%
	) else if !MAJOR! EQU -1 (
		echo.Downgrading to %SOURCE_VERSION% from %BUILD_VERSION%
	)
	
	if "!BUILD_VERSION!" NEQ "!SOURCE_VERSION!" (
		set BUILD_VERSION=!SOURCE_VERSION!
		set BUILD_NUMBER=1
	) else (
		set /A BUILD_NUMBER=!BUILD_NUMBER!+1
	)
	echo.Build Version  : !BUILD_VERSION!
	echo.Build Number   : !BUILD_NUMBER!
	echo.Source Version : !SOURCE_VERSION!
	echo|set /p="!BUILD_NUMBER!" >"!WORKSPACE!/.jenkins/build.txt"
	echo|set /p="!BUILD_VERSION!" >"!WORKSPACE!/.jenkins/version.txt"
) else echo.'version.txt' not found...
endlocal
exit /b 0
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
@echo off
setlocal EnableDelayedExpansion
echo.Copying build artifacts...
cd %WORKSPACE%
if not exist "%WORKSPACE%/bin" mkdir "%WORKSPACE%/bin"
copy /Y "%WORKSPACE%\build\release\SEFMediaPreparer.exe" "%WORKSPACE%\bin\"
if "%packArtifacts%" EQU "true" (
	echo.Compressing build artifacts...
	if not exist "%WORKSPACE%/release" mkdir "%WORKSPACE%/release"
	call "%ZIP%" a -mx9 -mmt8 "%WORKSPACE%/release/SEF.Media.Preparer.%BUILD_DISPLAY_NAME%.7z" "%WORKSPACE%/bin/SEFMediaPreparer.exe" "%WORKSPACE%/tools/ffmpeg.exe" "%WORKSPACE%/tools/ffprobe.exe"
	call "%ZIP%" a -mx9 -mmt8 "%WORKSPACE%/release/SEF.Media.Preparer.%BUILD_DISPLAY_NAME%.NF.7z" "%WORKSPACE%/bin/SEFMediaPreparer.exe"
)
echo.
endlocal
@echo off

set vcvarsall=%PROGRAMFILES(x86)%/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat
set qmake=%LOCALAPPDATA%/Qt/5.10.1/msvc2017_64/bin/qmake.exe
set jom=%LOCALAPPDATA%/Qt/Tools/QtCreator/bin/jom.exe
set project=SEFMediaPreparer

:params
if not ["%~1"]==[""] (
	if "%~1"=="-vcvarsall" (
		set vcvarsall=%~2
		shift
	)
	if "%~1"=="-vc" (
		set vcvarsall=%~2
		shift
	)
	if "%~1"=="-qmake" (
		set qmake=%~2
		shift
	)
	if "%~1"=="-q" (
		set qmake=%~2
		shift
	)
	if "%~1"=="-jom" (
		set jom=%~2
		shift
	)
	if "%~1"=="-j" (
		set jom=%~2
		shift
	)
	if "%~1"=="-project" (
		set project=%~2
		shift
	)
	if "%~1"=="-p" (
		set project=%~2
		shift
	)
	shift
)
if not ["%~1"]==[""] goto params

echo.Using vcvarsall.bat: %vcvarsall%
echo.Using qmake.exe: %qmake%
echo.Using jom.exe: %jom%
echo.Using project: %project%
echo.

echo.Building %project% in: %~dp0

echo.Setting up environment...
echo.^> vcvarsall.bat amd64
call "%vcvarsall%" amd64
if %errorlevel% != 0 (
	echo Environment setup failed...
	exit /b 1
)

cd /d %~dp0

echo.Running qmake...
echo.^> qmake.exe %project%.pro -spec win32-msvc
call "%qmake%" "%~dp0/%project%.pro" -spec win32-msvc
if %errorlevel% != 0 (
	echo qmake failed...
	exit /b 1
)

echo.Running jom (1/2)...
echo.^> jom.exe qmake_all
call "%jom%" qmake_all
if %errorlevel% != 0 (
	echo jom qmake_all failed...
	exit /b 1
)

echo.Runing jom (2/2)...
echo.^> jom.exe
call "%jom%"
if %errorlevel% != 0 (
	echo jom failed...
	exit /b 1
)

echo.Complete...
exit /b 0
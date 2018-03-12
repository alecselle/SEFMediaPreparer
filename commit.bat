@echo off

set username=Alec Selle
set email=contact@alecselle.com
set msg=Automatic Commit (dev)

:params
if not ["%~1"]==[""] (
	if "%~1"=="-u" (
		set username=%~2
		shift
	)
	if "%~1"=="-e" (
		set email=%~2
		shift
	)
	if "%~1"=="-m" (
		set msg=%~2
		shift
	)
	shift
)
if not ["%~1"]==[""] goto params

echo.Using username: %username%
call git config user.name "%username%"
echo.Using email: %email%
call git config user.email "%email%"
echo.Using commit message: %msg%
echo.

echo.Staging all files...
@echo on
call git add *
@echo off
echo.Commiting changes...
@echo on
call git commit -m "%msg%"
@echo off
echo.Pushing to branch...
@echo on
call git push
@echo off

exit /b 0
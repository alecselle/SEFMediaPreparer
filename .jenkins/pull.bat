@echo off
setlocal EnableDelayedExpansion
echo.Pulling to local workspace from git...
cd /d "F:\Workspace\cpp\SEFMediaPreparer"
call git pull
echo.
endlocal
@echo off
setlocal EnableDelayedExpansion
echo.Pushing local workspace to git...
cd /d "F:\Workspace\cpp\SEFMediaPreparer"
call git add *
call git commit -m "Automatic Commit (Jenkins)"
call git push
echo.Pulling git changes to build directory...
echo.
endlocal
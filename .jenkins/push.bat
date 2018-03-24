@echo off
setlocal EnableDelayedExpansion
echo.Pushing local workspace to git...
cd /d "F:\Workspace\SEFMediaPreparer"
git add *
git commit -m "Automatic Commit (Jenkins)"
git push
echo.Pulling git changes to build directory...
echo.
endlocal
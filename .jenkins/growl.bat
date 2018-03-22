@echo on
setlocal EnableDelayedExpansion
echo.Sending growl notification...
call "%GROWL%" %*
endlocal
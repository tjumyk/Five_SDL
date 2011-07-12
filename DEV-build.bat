@echo off
mkdir Release 2>nul
echo Please compile this project with Dev.
echo (Only compile, don't run!)
echo And then close Dev! 
echo.
echo The program will run automatically with a proper way 
echo just after Dev is shut down!
echo.
echo Press any key to start Dev... 
pause >nul
Five.dev
cd SDL-1.2.14/lib
copy *.dll "../../Release" /Y
cd ../../Five
start ../Release/Five.exe


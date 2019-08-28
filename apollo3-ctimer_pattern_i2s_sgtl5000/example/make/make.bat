@Echo off
make.exe
if errorlevel 1 (
   echo.
   echo Error while executing make. See the error details above.
   echo.
   echo General Hints to setup make on Windows
   echo ======================================
   echo - Download and install http://gnuwin32.sourceforge.net/
   echo - Download and install MinGW http://www.mingw.org/
   echo - Check in system advanced properties if the PATH variable 
   echo   includes the MinGW binary path
   echo - Install Ambiq Control Center or change the TOOLSDIR variable
   echo   in the makefile to your GNU tool chain
   echo.
   pause
) ELSE (
   echo.
   echo Make was successful done.
   pause
)
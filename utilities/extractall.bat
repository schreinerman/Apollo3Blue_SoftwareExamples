@Echo off
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
cd ..
for /f "delims=|" %%f in ('dir /b *.zip') do %SEVENZIP% x %%f
rem setlocal enabledelayedexpansion
rem for /f "delims=|" %%f in ('dir /b *.zip') do (
rem   set zipfile=%%~nf
rem   echo !zipfile!
rem   set name=!zipfile:~0,-4%!
rem   %SEVENZIP% x %%f -o!name!
rem )
rem endlocal
pause
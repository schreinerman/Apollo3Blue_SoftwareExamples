@Echo off
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
cd ..
del *.zip
SETLOCAL EnableDelayedExpansion
for /f "delims=|" %%f in ('dir /b ama3*') do (
  utilities\ProjectCleaner.exe "%%f"
  set /p Build=<%%f/version.txt
  %SEVENZIP% a %%f-v!Build!.zip %%f/*
)
for /f "delims=|" %%f in ('dir /b apollo3*') do (
  utilities\ProjectCleaner.exe "%%f"
  set /p Build=<%%f/version.txt
  %SEVENZIP% a %%f-v!Build!.zip %%f/*
)
ENDLOCAL
pause


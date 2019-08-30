@Echo off
cd ..
cd ..
cd ..
SETLOCAL EnableDelayedExpansion
for /f "delims=|" %%f in ('dir /b ama3*') do (
  cp utilities\patches\2019-08-30_iar_startup\startup_apollo3.s "%%f\example\iar\startup_apollo3.s"
)
for /f "delims=|" %%f in ('dir /b apollo3*') do (
  cp utilities\patches\2019-08-30_iar_startup\startup_apollo3.s "%%f\example\iar\startup_apollo3.s"
)
ENDLOCAL
pause


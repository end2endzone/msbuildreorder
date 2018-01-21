@echo off

::Delete old output files
del output*.* >NUL 2>&1

..\..\%CONFIGURATION%\msbuildreorder.exe inputbuildoutput.txt outputbuildoutput.txt
echo.
..\..\%CONFIGURATION%\msbuildreorder.exe inputdevenv.txt outputdevenv.txt
echo.
..\..\%CONFIGURATION%\msbuildreorder.exe scd-buildlog-685518060061352598.txt scd-buildlog-685518060061352598.2.txt
echo.

pause

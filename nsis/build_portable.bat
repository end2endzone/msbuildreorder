@ECHO OFF

REM **************************************
REM  Generate ZIP package
REM **************************************

cd /d %~dp0
set PRODUCT_PLATFORM=win32


call ..\version_info.bat

set outfile=".\bin\msbuildreorder v%PRODUCT_VERSION% %PRODUCT_PLATFORM% portable.zip"

set infiles=
set infiles=%infiles% "..\msvc\%PRODUCT_PLATFORM%\Release\msbuildreorder.exe"
set infiles=%infiles% "..\msvc\%PRODUCT_PLATFORM%\Release\msbuildreorder_unittest.exe"
set infiles=%infiles% "..\msvc\%PRODUCT_PLATFORM%\Release\test_files"
set infiles=%infiles% "..\AUTHORS"
set infiles=%infiles% "..\CHANGES"
set infiles=%infiles% "..\LICENSE"
set infiles=%infiles% "..\README.md"

echo Generating portable install %outfile%
echo using the following input files:
echo %infiles%
echo.
call "C:\Program Files\7-Zip\7z.exe" a -bd %outfile% %infiles%

pause

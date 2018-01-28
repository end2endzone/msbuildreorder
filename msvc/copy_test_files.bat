@echo off

echo =======================================================================
echo Copying test files for unit tests...
echo =======================================================================

REM Copy test files to project folder to be able to run tests from Visual Studio IDE
cd /d %~dp0
mkdir test_files >NUL 2>NUL
copy /v "..\tests\test_files\*.*" "%~dp0test_files"


REM Copy test files to outdir for NSIS
cd /d %~dp0
mkdir Win32\Release\test_files >NUL 2>NUL
copy /v "..\tests\test_files\*.*" "%~dp0Win32\Release\test_files"

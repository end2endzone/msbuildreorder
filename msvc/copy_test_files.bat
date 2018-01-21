@echo off
mkdir test_files >NUL 2>NUL
copy /v "..\tests\test_files\*.*" "%~dp0test_files"

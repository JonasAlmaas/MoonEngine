@echo off

pushd %~dp0python
python setup/main.py
popd

echo.
echo Running premake...
call Win-GenProjects.bat

echo.
echo Setup completed!

PAUSE

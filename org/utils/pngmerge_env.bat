@echo off

SET PYTHONPATH_=C:\PyTest


set PYTHONPATH=%PYTHONPATH_%\Python27
set PYTHONHOME=%PYTHONPATH%
set path==%path%;%PYTHONPATH%
python pngmerge.py

pause
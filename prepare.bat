@echo off

call clean.bat

git submodule init
git submodule update

cd 3rdparty/sitl/
call build_min.bat

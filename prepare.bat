@echo off

git submodule init
git submodule update

cd extlibs/sitl/
call build_min.bat

@echo off

call clean.bat

cd 3rdparty/sitl/
call build.bat --without-bindings

@echo off

pushd %CD%
call prepare.bat
popd

pushd %CD%

mkdir cmakefiles_gcc
cd cmakefiles_gcc

mkdir programmer
cd programmer

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../../
cmake --build .

popd

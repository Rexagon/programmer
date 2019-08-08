#!/bin/sh

cwd=`dirname "$0"`

sh $cwd/prepare.sh

mkdir -p $cwd/cmakefiles_gcc/programmer
cd $cwd/cmakefiles_gcc/programmer

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake -DCMAKE_BUILD_TYPE=Release ../../
cmake --build .

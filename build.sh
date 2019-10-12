#!/bin/bash

cwd=$(dirname "$0")

sh "$cwd"/prepare.sh

withoutDeployment=0
for var in "$@"
do
    [ "$var" = "--without-deployment" ] && withoutDeployment=1
done

additionalParameters=""
[ $withoutDeployment -eq 1 ] && additionalParameters="$additionalParameters -DWIN_DEPLOY=OFF"

mkdir -p "$cwd"/cmakefiles_gcc/programmer
cd "$cwd"/cmakefiles_gcc/programmer || exit

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      $additionalParameters \
      ../../

cmake --build .

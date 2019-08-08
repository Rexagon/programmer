#!/bin/sh

cwd=`dirname "$0"`

sh $cwd/clean.sh

git submodule init
git submodule update

sh $cwd/3rdparty/sitl/build_min.sh

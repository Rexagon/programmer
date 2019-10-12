#!/bin/sh

cwd=$(dirname "$0")

sh "$cwd"/clean.sh

sh "$cwd"/3rdparty/sitl/build.sh --without-bindings

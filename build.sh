sh prepare.sh

mkdir -p cmakefiles_gcc/programmer
cd cmakefiles_gcc/programmer

cmake -DCMAKE_TOOLCHAIN_FILE=~/mingw-w64-i686.cmake -DCMAKE_BUILD_TYPE=Release ../../
cmake --build .

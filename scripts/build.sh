# Meant to be run from build/
export MAKEFLAGS=-j8
export CXX=clang++
export CC=clang

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make VERBOSE=1

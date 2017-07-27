#!/bin/bash

echo "rm cmake cache begin..."
#rm -rf `find . -name "cmake_install.cmake"`
#rm -rf `find . -name "CMakeFiles"`
#rm -rf `find . -name "Makefile"`
#rm -rf `find . -name "CMakeCache.txt"`

rm -rf cmake_install.cmake
rm -rf CMakeFiles
rm -rf Makefile
rm -rf CMakeCache.txt
echo "rm cmake cache end..."

#!/bin/bash

echo "rm cmake cache begin..."
rm `find . -name "cmake_install.cmake"`
rm -r `find . -name "CMakeFiles"`
rm -r `find . -name "Makefile"`
rm -r `find . -name "CMakeCache.txt"`
echo "rm cmake cache end..."

#!/bin/bash

rm -rf build_arm
mkdir build_arm
cd build_arm
cmake -DENABLE_NEON=ON -DENABLE_AVX2=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_SHARED_LIBS=ON -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake -DCMAKE_INSTALL_PREFIX=install ..
cmake --build . --config Release
cmake --build . --config Release --target install


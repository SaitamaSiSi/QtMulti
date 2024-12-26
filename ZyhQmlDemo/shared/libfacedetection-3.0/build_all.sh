#!/bin/bash

basedir=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)

rm -rf ${basedir}/output

################## x86_64
outdir_x86=${basedir}/output/x86

rm -rf build_x86
mkdir build_x86
cd build_x86

cmake .. -DCMAKE_INSTALL_PREFIX=${outdir_x86} -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=RELEASE -DDEMO=OFF -DUSE_OPENMP=ON
cmake --build . --config Release
cmake --build . --config Release --target install

# generate static library(lib)
cmake .. -DCMAKE_INSTALL_PREFIX=${outdir_x86} -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=RELEASE -DDEMO=OFF -DUSE_OPENMP=ON
cmake --build . --config Release
cmake --build . --config Release --target install

cd ${basedir}

################## ARM
outdir_arm=${basedir}/output/arm

rm -rf build_arm
mkdir build_arm
cd build_arm

cmake  -DENABLE_NEON=ON -DENABLE_AVX2=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_SHARED_LIBS=ON -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake -DCMAKE_INSTALL_PREFIX=${outdir_arm} ..
cmake --build . --config Release
cmake --build . --config Release --target install

# generate static library(lib)
cmake  -DENABLE_NEON=ON -DENABLE_AVX2=OFF -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_SHARED_LIBS=OFF -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake -DCMAKE_INSTALL_PREFIX=${outdir_arm} ..
cmake --build . --config Release
cmake --build . --config Release --target install

cd ${basedir}
rm -rf build_x86
rm -rf build_arm


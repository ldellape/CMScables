#!/bin/bash

ROOT_VERSION=$(root-config --version)
BUILD="build_CMScables"
EXEC="CMScables"
LIBHARU_VERSION="libharu-RELEASE_2_3_0"

pip install pyPDF2

if [ $ROOT_VERSION==6.* ]; then 
echo "root version :$ROOT_VERSION"
rm -rf $BUILD
rm -rf $EXEC
echo "running make..."
make
else 
echo "root version not found, you have root $ROOT_VERSION , but root version >= 6.x.x is needed"
exit 1
fi

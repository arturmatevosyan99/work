#!/bin/bash

VERSION=master

export BUILD_ARCH=$1
export BUILD_TYPE=$2
export IOS_SDK=$3
export SRC_DIR=$4

git clone http://github.com/TinkoffCreditSystems/tinkoff-asdk-ios $SRC_DIR

xcodebuild build -configuration $BUILD_TYPE -arch $BUILD_ARCH -sdk $IOS_SDK -project $SRC_DIR/ASDKUI.xcodeproj

echo -e "TinkoffASDK framework has been successfully built"

exit 0
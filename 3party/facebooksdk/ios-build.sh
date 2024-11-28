#!/usr/bin/env bash

VERSION=master
XCODEBUILD=`which xcodebuild`
XCODEGEN=`which xcodegen`

if [[ ! -f "$XCODEBUILD" ]]; then
	echo -e "FacebookSDK framework cannot been built: xcodebuild not found"
	exit 1
elif [[ ! -f "$XCODEGEN" ]]; then
	echo -e "FacebookSDK framework cannot been built: xcodegen not found"
	exit 1
fi

export BUILD_ARCH=$1
export BUILD_TYPE=$2
export IOS_SDK=$3
export SRC_DIR=$4
export PATH="/usr/local/bin:$PATH"

#git clone http://github.com/facebook/facebook-ios-sdk $SRC_DIR
git clone -b releases/v12.3.2 --single-branch https://github.com/facebook/facebook-ios-sdk $SRC_DIR

$XCODEGEN generate --spec $SRC_DIR/FBSDKCoreKit_Basics/project.yml
$XCODEGEN generate --spec $SRC_DIR/FBAEMKit/project.yml
$XCODEGEN generate --spec $SRC_DIR/FBSDKCoreKit/project.yml
$XCODEGEN generate --spec $SRC_DIR/TestTools/project.yml
$XCODEGEN generate --spec $SRC_DIR/FBSDKLoginKit/project.yml
$XCODEGEN generate --spec $SRC_DIR/FBSDKShareKit/project.yml
$XCODEGEN generate --spec $SRC_DIR/FBSDKGamingServicesKit/project.yml

$XCODEBUILD build -configuration $BUILD_TYPE -arch $BUILD_ARCH -sdk $IOS_SDK -workspace $SRC_DIR/FacebookSDK.xcworkspace -scheme BuildAllKits-Dynamic -derivedDataPath $SRC_DIR/ CLANG_ENABLE_CODE_COVERAGE=NO > $SRC_DIR/xcodebuild.log

echo -e "FacebookSDK framework has been successfully built"
exit 0

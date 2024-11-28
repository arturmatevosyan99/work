#!/usr/bin/env bash

VERSION=1.1.1m
ANDROID_API=22
export BUILD_ARCH=$1
export BUILD_DIR=$2
export ANDROID_NDK_HOME=$3

mkdir -p $BUILD_DIR
cd $BUILD_DIR

if [ ! -f "openssl-$VERSION.tar.gz" ]; then
    curl -O -L https://www.openssl.org/source/openssl-$VERSION.tar.gz
fi
if [ ! -f "openssl-$VERSION/Configure" ]; then
    tar -zxf openssl-$VERSION.tar.gz
    cd openssl-$VERSION
    chmod +x ./Configure
else
    cd openssl-$VERSION
    make clean
    make distclean
fi

case "$(uname -s)" in
    Darwin) HOST="darwin-x86_64";;
    Linux)  HOST="linux-x86_64";;
    *)      HOST="wrong-kernel-x86_64";;
esac

if [ -d "$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/$HOST/bin" ]; then
    ANDROID_TOOLCHAIN="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/$HOST/bin"
fi
export PATH="$ANDROID_TOOLCHAIN":"$PATH"

for arch in $BUILD_ARCH
do
    case $arch in
        armeabi-v7a)
            PLATFORM_ARCH="arm"
            ANDROID_API=21 #16
            ;;
        x86)
            PLATFORM_ARCH="x86"
            ANDROID_API=21 #16
            ;;
        arm64-v8a)
            PLATFORM_ARCH="arm64"
            ANDROID_API=21
            ;;
        x86_64)
            PLATFORM_ARCH="x86_64"
            ANDROID_API=21
            ;;
    esac

    mkdir -p $BUILD_DIR/${arch}
    ./Configure shared android-${PLATFORM_ARCH} -D__ANDROID_API__=${ANDROID_API} --prefix=$BUILD_DIR/$arch || exit 1
    make depend
    make -j$(sysctl -n hw.logicalcpu) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_libs || exit 1
#   make install
    llvm-strip --strip-all libcrypto_1_1.so
    llvm-strip --strip-all libssl_1_1.so
    mv libcrypto_1_1.so $BUILD_DIR/$arch/
    mv libssl_1_1.so $BUILD_DIR/$arch/
done

exit 0

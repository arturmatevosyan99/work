#!/usr/bin/env bash

VERSION=4.2
ANDROID_API=21
HOST=""
export BUILD_ARCH=$1
export BUILD_DIR=$2
export ANDROID_NDK_HOME=$3

ASM=`which nasm`
if [[ ! -f "$ASM" ]]; then
	ASM=`which yasm`
	if [[ ! -f "$ASM" ]]; then
		echo "FFmpeg framework cannot been built: [y|n]asm not found"
		exit 1
	fi
fi

PKG_CONFIG=`which pkg-config`
if [[ ! -f "$PKG_CONFIG" ]]; then
	echo "FFmpeg framework cannot been built: pkg-config not found"
	exit 1
fi

mkdir -p $BUILD_DIR
cd $BUILD_DIR

if [ ! -f "ffmpeg-$VERSION.tar.gz" ]; then
	curl -O https://www.ffmpeg.org/releases/ffmpeg-$VERSION.tar.gz
fi
if [ ! -f "ffmpeg-$VERSION/configure" ]; then
	tar -zxf ffmpeg-$VERSION.tar.gz
	cd ffmpeg-$VERSION
	chmod +x ./configure
else
	cd ffmpeg-$VERSION
fi

case "$(uname -s)" in
	Darwin)	HOST="darwin-x86_64";;
	Linux)	HOST="linux-x86_64";;
	*)	HOST="wrong-kernel-x86_64";;
esac

LLVM_TOOLCHAIN="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/$HOST"
SYSROOT="$LLVM_TOOLCHAIN/sysroot"

for arch in $BUILD_ARCH # "x86_64" "x86" "arm64" "arm"
do
	ADDI_LDFLAGS=""
	LIB_DIRNAME="lib"
	TOOLCHAIN_DIRNAME=""
	SPECIFIC=""
	case $arch in
		armeabi-v7a)
			PLATOFRM_ARCH="arm"
			ANDROID_ARCH="armv7a"
			ANDROID_API=21 #16
			ADDI_CCFLAGS="-target thumbv7-none-linux-android16 -mfpu=vfpv3-d16 -mfloat-abi=soft -D_FILE_OFFSET_BITS=32" #  -march=armv7-a
			ADDI_LDFLAGS="-Wl,-z,relro -Wl,-z,now -Wl,--fix-cortex-a8"
			TOOLCHAN_MONIKER="androideabi"
			TOOLCHAIN_DIRNAME="arm-linux-${TOOLCHAN_MONIKER}"
			SPECIFIC="--enable-thumb"
			;;
		x86)
			PLATOFRM_ARCH="i686"
			ANDROID_ARCH="i686"
			ANDROID_API=21 #16			
			ADDI_CCFLAGS="-target i686-none-linux-androideabi -mssse3 -mfpmath=sse -m32"
			ADDI_LDFLAGS="-Wl,-z,relro -Wl,-z,now"
			TOOLCHAN_MONIKER="android"
			TOOLCHAIN_DIRNAME="i686-linux-${TOOLCHAN_MONIKER}"
			SPECIFIC="--disable-asm"
			;;
		arm64-v8a)
			PLATOFRM_ARCH="aarch64"
			ANDROID_ARCH="aarch64"
			ANDROID_API=21
			ADDI_CCFLAGS="-target aarch64-none-linux-android -mfpu=neon -mfloat-abi=soft"
			ADDI_LDFLAGS=""
			TOOLCHAN_MONIKER="android"
			TOOLCHAIN_DIRNAME="aarch64-linux-${TOOLCHAN_MONIKER}"
			SPECIFIC=""
			;;
		x86_64)
			PLATOFRM_ARCH="x86_64"
			ANDROID_ARCH="x86_64"
			ANDROID_API=21
			ADDI_CCFLAGS="-target x86_64-none-linux-androideabi -msse4.2 -mpopcnt -m64"
			ADDI_LDFLAGS=""
			TOOLCHAN_MONIKER="android"
			TOOLCHAIN_DIRNAME="x86_64-linux-${TOOLCHAN_MONIKER}"
			SPECIFIC="--x86asmexe=${LLVM_TOOLCHAIN}/bin/yasm"
			;;
	esac
	ANDROID_TOOLCHAIN=$ANDROID_NDK_HOME/toolchains/$TOOLCHAIN_DIRNAME-4.9/prebuilt/$HOST/bin
	export PATH="$ANDROID_TOOLCHAIN":"$PATH"

	mkdir -p $BUILD_DIR/${arch}
	./configure --disable-everything \
		--prefix=$BUILD_DIR/${arch} \
		--enable-decoder=pcm_s16le \
		--enable-encoder=pcm_s16le \
		--enable-decoder=opus \
		--enable-encoder=opus \
		--enable-parser=opus \
		--enable-demuxer=ogg \
		--enable-muxer=ogg \
		--enable-protocol=tls_protocol,http,https,file,concat \
		--enable-pic \
		--enable-shared \
		--disable-static \
		--disable-pthreads \
		--disable-programs \
		--cross-prefix=${LLVM_TOOLCHAIN}/bin/${PLATOFRM_ARCH}-linux-${TOOLCHAN_MONIKER}- \
		--cc=${LLVM_TOOLCHAIN}/bin/${ANDROID_ARCH}-linux-${TOOLCHAN_MONIKER}${ANDROID_API}-clang \
		--cxx=${LLVM_TOOLCHAIN}/bin/${ANDROID_ARCH}-linux-${TOOLCHAN_MONIKER}${ANDROID_API}-clang++ \
		--pkg-config="$PKG_CONFIG" \
		--x86asmexe="$ASM" \
		--enable-cross-compile \
		--strip=${LLVM_TOOLCHAIN}/bin/llvm-strip \
		--nm=${LLVM_TOOLCHAIN}/bin/llvm-nm \
		--sysroot=$SYSROOT \
		--target-os=android \
		--arch=$ANDROID_ARCH \
		--extra-cflags="-O3 -fPIC $ADDI_CCFLAGS" \
		--extra-ldflags="$ADDI_LDFLAGS" \
		$SPECIFIC || exit 1

	$ANDROID_NDK_HOME/prebuilt/$HOST/bin/make clean
	$ANDROID_NDK_HOME/prebuilt/$HOST/bin/make -j2 || exit 1
	$ANDROID_NDK_HOME/prebuilt/$HOST/bin/make install || exit 1
done

exit 0

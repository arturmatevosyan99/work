#!/usr/bin/env bash

VERSION=4.2
export BUILD_DIR=$1
export ARCH=`uname -m`

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

mkdir -p $BUILD_DIR/${ARCH}

./configure \
--disable-everything \
--prefix=$BUILD_DIR/${ARCH} \
--ld="g++" \
--extra-libs="-lpthread -lm" \
--pkg-config="$PKG_CONFIG" \
#--pkg-config-flags="--static" \
--x86asmexe="$ASM" \
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
--disable-programs
|| exit 1

make clean
make -j2 || exit 1

exit 0

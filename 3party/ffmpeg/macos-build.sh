#!/usr/bin/env bash

VERSION=4.2
export BUILD_ARCH=$1
export BUILD_SDK=$2
export BUILD_DIR=$3
export PATH="/usr/local/bin:$PATH"

DEPLOYMENT_TARGET="10.13"
UNIVERSAL_DIR=${BUILD_DIR}/universal
CWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

ASM=`which nasm`
if [[ ! -f "$ASM" ]]; then
	ASM=`which yasm`
	if [[ ! -f "$ASM" ]]; then
		echo "FFmpeg framework cannot been built: [y|n]asm not found"
		exit 1
	fi
fi

mkdir -p $BUILD_DIR
mkdir -p $UNIVERSAL_DIR
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

echo "Building $BUILD_ARCH..."
mkdir -p "$BUILD_DIR/$BUILD_ARCH"

CFLAGS="-arch $BUILD_ARCH"
CFLAGS="$CFLAGS -mmacosx-version-min=$DEPLOYMENT_TARGET -fembed-bitcode"
EXPORT="GASPP_FIX_XCODE5=1"

XCRUN_SDK=`echo $BUILD_SDK | tr '[:upper:]' '[:lower:]'`
CC="xcrun -sdk $XCRUN_SDK clang"

# force "configure" to use "gas-preprocessor.pl" (FFmpeg 3.3)
if [ "$BUILD_ARCH" = "arm64" ]; then
	AS="${CWD}/gas-preprocessor.pl -arch aarch64 -- $CC"
else
	AS="${CWD}/gas-preprocessor.pl -- $CC"
fi

./configure \
	--arch=$BUILD_ARCH \
	--cc="$CC" \
	--as="$AS" \
	--x86asmexe="$ASM" \
	--disable-everything \
	--enable-cross-compile \
	--enable-decoder=pcm_s16le \
	--enable-encoder=pcm_s16le \
	--enable-decoder=opus \
	--enable-encoder=opus \
	--enable-parser=opus \
	--enable-demuxer=ogg \
	--enable-muxer=ogg \
	--enable-protocol=tls_protocol,http,https,file,concat \
	--disable-debug \
	--disable-programs \
	--disable-doc \
	--disable-bzlib \
	--enable-pic \
	--extra-cflags="$CFLAGS" \
	--extra-ldflags="$LDFLAGS" \
	--prefix="$BUILD_DIR/$BUILD_ARCH" \
|| exit 1

make clean
make -j3 install || exit 1

set -e
FFMPEG_LIBS="libavcodec libavdevice libavfilter libavformat libavutil libswresample libswscale"
echo "Building frameworks: ${FFMPEG_LIBS}..."
for FFMPEG_LIB in ${FFMPEG_LIBS}
do
#   xcrun --sdk $PLATFORM-f lipo -create ${BUILD_DIR}/build/${BUILD_ARCH}/lib/${FFMPEG_LIB}.a -output ${FRAMEWORK_DIR}/lib/${FFMPEG_LIB}.a
	FFMPEG_LIB_UPPERCASE=$(echo ${FFMPEG_LIB} | tr '[a-z]' '[A-Z]')
	FFMPEG_LIB_MAJOR=$(grep "#define ${FFMPEG_LIB_UPPERCASE}_VERSION_MAJOR" ${BUILD_DIR}/${BUILD_ARCH}/include/${FFMPEG_LIB}/version.h | sed -e "s/#define ${FFMPEG_LIB_UPPERCASE}_VERSION_MAJOR//g;s/\ //g")
	FFMPEG_LIB_MINOR=$(grep "#define ${FFMPEG_LIB_UPPERCASE}_VERSION_MINOR" ${BUILD_DIR}/${BUILD_ARCH}/include/${FFMPEG_LIB}/version.h | sed -e "s/#define ${FFMPEG_LIB_UPPERCASE}_VERSION_MINOR//g;s/\ //g")
	FFMPEG_LIB_MICRO=$(grep "#define ${FFMPEG_LIB_UPPERCASE}_VERSION_MICRO" ${BUILD_DIR}/${BUILD_ARCH}/include/${FFMPEG_LIB}/version.h | sed "s/#define ${FFMPEG_LIB_UPPERCASE}_VERSION_MICRO//g;s/\ //g")
	FFMPEG_LIB_VERSION="${FFMPEG_LIB_MAJOR}.${FFMPEG_LIB_MINOR}.${FFMPEG_LIB_MICRO}"
	FFMPEG_LIB_SHORT_VERSION="${FFMPEG_LIB_MAJOR}.${FFMPEG_LIB_MINOR}"

	FFMPEG_LIB_FRAMEWORK_PATH=$BUILD_DIR/${FFMPEG_LIB}.framework
	mkdir -p ${FFMPEG_LIB_FRAMEWORK_PATH}/Headers

	cp -r ${BUILD_DIR}/${BUILD_ARCH}/include/${FFMPEG_LIB}/* ${FFMPEG_LIB_FRAMEWORK_PATH}/Headers
	cp ${BUILD_DIR}/${BUILD_ARCH}/lib/${FFMPEG_LIB}.a ${FFMPEG_LIB_FRAMEWORK_PATH}/${FFMPEG_LIB}

	cat > ${FFMPEG_LIB_FRAMEWORK_PATH}/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleExecutable</key>
	<string>${FFMPEG_LIB}</string>
	<key>CFBundleIdentifier</key>
	<string>org.ffmpeg.${FFMPEG_LIB}</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>${FFMPEG_LIB}</string>
	<key>CFBundlePackageType</key>
	<string>FMWK</string>
	<key>CFBundleShortVersionString</key>
	<string>${FFMPEG_LIB_SHORT_VERSION}</string>
	<key>CFBundleVersion</key>
	<string>${FFMPEG_LIB_VERSION}</string>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>CFBundleSupportedPlatforms</key>
	<array>
		<string>MacOSX</string>
	</array>
	<key>NSPrincipalClass</key>
	<string></string>
</dict>
</plist>
EOF
	echo -e "FFmpeg ${FFMPEG_LIB} library has been successfully built"
done

echo -e "FFmpeg framework has been successfully built"
exit 0

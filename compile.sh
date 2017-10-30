#!/bin/sh
if [ -f ext/lib/libjpeg.a ]
then
    echo "libjpeg already built; skipping..."
else
    cd ext/src/libjpeg-turbo
    autoreconf --install
#   TODO: see if we can get away with enabling SIMD
    ./configure --host=powerpc-eabi --prefix=$(dirname $(dirname $(pwd))) --disable-shared --without-simd CFLAGS=-mwup
    make $@
    make install
    cd ../../..
fi

mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$WUT_ROOT/cmake/wut-toolchain.cmake ../
make $@

#!/bin/bash
set -e

if [ -z "$1" ]; then
    ./configure --prefix=/usr/x86_64-w64-mingw32 --host=x86_64-w64-mingw32 && \
        make clean && \
        make && \
        ${STRIP} liftoff.exe && \
        upx liftoff.exe
else
    exec "$@"
fi

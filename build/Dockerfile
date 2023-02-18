FROM debian:bullseye-slim

ENV TARGET="x86_64-w64-mingw32"
ENV PREFIX="/usr/${TARGET}"
ENV PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

RUN echo 'deb-src http://deb.debian.org/debian bullseye main contrib non-free' >>/etc/apt/sources.list

RUN apt-get update && \
    apt-get install -y \
        automake \
        binutils-mingw-w64-x86-64 \
        dpkg-dev \
        g++-mingw-w64-x86-64-win32 \
        pkg-config \
        upx \
        wget

WORKDIR ${PREFIX}/src

# glew
RUN apt-get source -y libglew2.1 && \
    cd glew-2.1.0 && \
    make clean install GLEW_DEST=${PREFIX} HOST=${TARGET} SYSTEM=linux-mingw64 && \
    cd .. && \
    rm -rf glew-2.1.0 glew_*

# glm headers
RUN wget https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz -O - | tar -zxf - && \
    cd glm-0.9.9.8 && \
    cp -ar glm ../../include && \
    cd .. && \
    rm -rf glm-0.9.9.8

# freetype
RUN wget https://download.savannah.gnu.org/releases/freetype/freetype-2.13.0.tar.gz -O - | tar -zxf - && \
    cd freetype-2.13.0 && \
    ./configure \
        --prefix=${PREFIX} \
        --host=${TARGET} \
        --with-zlib=no \
        --with-bzip2=no \
        --with-png=no && \
    make && \
    make install && \
    cd .. && \
    rm -rf freetype-2.13.0

# libogg
RUN wget https://downloads.xiph.org/releases/ogg/libogg-1.3.5.tar.gz -O - | tar -zxf - && \
    cd libogg-1.3.5 && \
    ./configure \
        --prefix=${PREFIX} \
        --host=${TARGET} \
        --enable-static && \
    make && \
    make install && \
    cd .. && \
    rm -rf libogg-1.3.5

# libvorbis
RUN wget https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.tar.gz -O - | tar -zxf - && \
    cd libvorbis-1.3.7 && \
    ./configure \
        --prefix=${PREFIX} \
        --host=${TARGET} \
        --enable-static && \
    make && \
    make install && \
    cd .. && \
    rm -rf libvorbis-1.3.7

# SDL2
RUN wget https://github.com/libsdl-org/SDL/releases/download/release-2.26.3/SDL2-2.26.3.tar.gz -O - | tar -zxf - && \
    cd SDL2-2.26.3 && \
    ./configure \
        --prefix=${PREFIX} \
        --host=${TARGET} \
        --enable-static \
        --disable-power \
        --disable-diskaudio \
        --disable-dummyaudio \
        --disable-video-dummy \
        --disable-video-vulkan \
        --disable-video-opengles \
        --disable-video-opengles1 \
        --disable-video-opengles2 \
        --disable-render-metal && \
    make && \
    make install && \
    cd .. && \
    rm -rf SDL2-2.26.3

# SDL2_mixer
RUN wget https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.6.3/SDL2_mixer-2.6.3.tar.gz -O - | tar -zxf - && \
    cd SDL2_mixer-2.6.3 && \
    ./configure \
        --prefix=${PREFIX} \
        --host=${TARGET} \
        --enable-static \
        --enable-music-ogg \
        --disable-music-cmd \
        --disable-music-wave \
        --disable-music-mod \
        --disable-music-mod-modplug \
        --disable-music-mod-modplug-shared \
        --disable-music-mod-mikmod \
        --disable-music-mod-mikmod-shared \
        --disable-music-midi \
        --disable-music-midi-timidity \
        --disable-music-midi-native \
        --disable-music-midi-fluidsynth \
        --disable-music-midi-fluidsynth-shared \
        --disable-music-ogg-tremor \
        --disable-music-ogg-shared \
        --disable-music-flac \
        --disable-music-flac-shared \
        --disable-music-mp3 \
        --disable-music-mp3-mad-gpl \
        --disable-music-mp3-mad-gpl-dithering \
        --disable-music-mp3-mpg123 \
        --disable-music-mp3-mpg123-shared \
        --disable-music-opus \
        --disable-music-opus-shared && \
    make && \
    make install && \
    cd .. && \
    rm -rf SDL2_mixer-2.6.3

WORKDIR /build

COPY entrypoint.sh /
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

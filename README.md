<img src="https://github.com/poliander/liftoff/blob/master/resources/screenshot.png" width="100%">

This game somehow happened when I looked at the source code of [Xavier Hosxe's "Glaxium"](http://xhosxe.free.fr/glaxium/) and decided to try writing a small screensaver in OpenGL.

[Download 64-bit Windows ZIP](https://github.com/poliander/liftoff/releases/download/2019%2F06%2F10/liftoff-20190610-win32-x64.zip)

## Controls

- JOYSTICK or GAMEPAD is supported
- CURSOR KEYS or W/A/S/D will move your ship
- CTRL will let your ship fire its little guns
- F12 toggles frame rate display
- ESCAPE quits the game at any time

## Build

### Requirements

- libGL
- libGLU
- libGLEW
- libvorbisfile
- libogg
- SDL 2.0
- SDL\_mixer 2.0 (with ogg vorbis support)
- glm.hpp (GL math headers)

### Compilation

```
./configure && make && sudo make install
```
 
Assumed you have a working cross-compiling environment available (congratulations), then creating win32 executables is possible:

```
./configure --prefix=/usr/x86_64-w64-mingw32 --host=x86_64-w64-mingw32 && make
```

## Command line options

```liftoff -d```

Write debug information to "debug.log" (Windows only)

```liftoff -noms```

Disable multisampling

## External resources used

Font "Zekton" from Typodermic Fonts Inc. (c) 2000 Ray Larabie

## License

[GNU GPLv3](https://github.com/poliander/liftoff/blob/master/LICENSE)

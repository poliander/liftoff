<img src="https://github.com/poliander/liftoff/blob/master/dat/screenshot.png" width="100%">

This game somehow happened after I looked at the source code of [Xavier Hosxe's "Glaxium"](http://xhosxe.free.fr/glaxium/) and tried to write a small screensaver in OpenGL.

[Download 64-bit Windows ZIP](https://github.com/poliander/liftoff/releases/download/2019%2F06%2F09/liftoff-20190609-win32-x64.zip)

## Controls

- JOYSTICKS and GAMEPADS are supported
- MOUSE works intentionally only in menu
- CURSOR KEYS or W/A/S/D will move your ship
- CTRL will let your ship fire its little guns
- F12 toggles frame rate display
- ESCAPE quits the game at any time

## Build

### Requirements

- libGL
- libGLU
- SDL 1.2
- SDL\_mixer 1.2 (with ogg vorbis support)
- SDL\_image 1.2

### Compilation

```
./configure && make
```
 
Assumed you have a working cross-compiling environment available (congratulations), then creating win32 executables is possible:

```
./configure --prefix=/usr/x86_64-w64-mingw32 --host=x86_64-w64-mingw32 && make
```

## Command line options

```liftoff -d```

Write debug information to "debug.log"

```liftoff -noms```

Disable multisampling

## License

[GNU GPLv3](https://github.com/poliander/liftoff/blob/master/LICENSE)

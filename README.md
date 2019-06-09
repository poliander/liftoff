<img src="https://github.com/poliander/liftoff/blob/master/dat/screenshot.png" width="100%">

This game somehow happened after I looked at the source code of [Xavier Hosxe's "Glaxium"](http://xhosxe.free.fr/glaxium/) and tried to write a small screensaver in OpenGL.

## Controls

- JOYSTICKS and GAMEPADS are supported
- MOUSE works intentionally only in menu
- CURSOR KEYS or W/A/S/D will move your ship
- CTRL will let your ship fire its little guns
- F12 toggles frame rate display
- ESCAPE quits the game at any time

## Build

### Requirements

- libgl1-mesa-dev
- libglu1-mesa-dev
- libsdl1.2-dev
- libsdl-mixer1.2-dev (with ogg vorbis support)
- libsdl-image1.2-dev

### Compilation

```
./configure && make
```
 
Assuming you have a working cross-compiling environment available (congratulations), then creating win32 executables is possible:

```
./configure --prefix=/usr/x86_64-w64-mingw32 --host=x86_64-w64-mingw32 && make
```

## Command line options

```liftoff -h```

Show command line parameters (Linux only).

```liftoff -d```

Write debug information to "debug.log".

```liftoff -l```

Low quality mode. Half resolution for background texture, disables mip-mapping and multisampling and reduces the number of background stars and particles to 50%.

```liftoff -m[2|4|8|16]```

Enables multisampling at 2, 4, 8 or 16 samples. If not specified, the highest multisampling mode available on your hardware will be used. Note that multisampling is disabled in low quality mode ("-l").

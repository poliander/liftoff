<img src="https://github.com/poliander/liftoff/blob/master/resources/screenshot.png" width="100%">

This game somehow happened when I looked at the source code of [Xavier Hosxe's "Glaxium"](http://xhosxe.free.fr/glaxium/) and decided to try writing a small screensaver in OpenGL.

[Download 64-bit Windows ZIP](https://github.com/poliander/liftoff/releases/download/2019%2F06%2F10/liftoff-20190610-win32-x64.zip)

### Controls

- JOYSTICK or GAMEPAD is supported
- CURSOR KEYS or W/A/S/D will move your ship
- CTRL will let your ship fire its little guns
- F12 toggles frame rate display
- ESCAPE quits the game at any time

### Quality levels

|  Quality  | Recommended for | MSAA  | Framebuffer  | Font res. |
| :------------ | :------------ | :------------ | :------------ | :------------ |
| very low | 720p or lower | off  | 1024x1024 | 4 bit |
| low  | 720p | 2x  | 1024x1024 | 5 bit | 720p |
| medium | 1080p | 2x  | 2048x2048 | 5 bit | 1080p |
|  high | 1080p | 4x  | 2048x2048 | 6 bit | 1080p |
|  very high | 1440p | 4x  | 4096x4096 | 6 bit |
|  ultra | 4K or higher | 8x  | 4096x4096 | 7 bit |

## Building from source

### Prerequisites

- libGL
- libGLU
- libGLEW
- libvorbisfile >= 1.3
- libogg >= 1.3
- FreeType2 >= 2.8 (no zlib/libpng support required)
- SDL 2.0
- SDL\_mixer 2.0 (with ogg vorbis support)
- glm.hpp (GL math headers)

### Compile

```
./configure && make && sudo make install
```
 
Assumed you have a working cross-compiling environment available, then creating a static linked win32 executables is possible:

```
./configure --prefix=/usr/x86_64-w64-mingw32 --host=x86_64-w64-mingw32 && make
```

## External resources

Font "Zekton" from Typodermic Fonts Inc. (c) 2000 Ray Larabie  

Title theme "Unknown Energy" by Dox | https://soundcloud.com/dox-free-music  
License: Attribution-NoDerivs 3.0 Unported ([CC BY-ND 3.0](https://creativecommons.org/licenses/by-nd/3.0/))  

### Helpful links

- [learnopengl.com](https://learnopengl.com/)
- [www.opengl-tutorial.org](https://www.opengl-tutorial.org/)
- [www.songho.ca/opengl/](https://www.songho.ca/opengl/)
- [Visual glBlendFunc/glBlendEquation Tool](https://www.andersriggelsen.dk/glblendfunc.php)
- [GLSL Syntax Highlighting for Vim](https://github.com/tikhomirov/vim-glsl)

## License

[GNU GPLv3](https://github.com/poliander/liftoff/blob/master/LICENSE)

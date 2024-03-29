AC_DEFUN([AC_DEFINE_DIR], [
	prefix_NONE=
	exec_prefix_NONE=
	test "x$prefix" = xNONE && prefix_NONE=yes && prefix=$ac_default_prefix
        test "x$exec_prefix" = xNONE && exec_prefix_NONE=yes && exec_prefix=$prefix
	dnl In Autoconf 2.60, ${datadir} refers to ${datarootdir}, which in turn
	dnl refers to ${prefix}.  Thus we have to use `eval' twice.
	eval ac_define_dir="\"[$]$2\""
	eval ac_define_dir="\"$ac_define_dir\""
	AC_SUBST($1, "$ac_define_dir")
	AC_DEFINE_UNQUOTED($1, "$ac_define_dir", [$3])
	test "$prefix_NONE" && prefix=NONE
	test "$exec_prefix_NONE" && exec_prefix=NONE
])

AC_PREREQ(2.61)
AC_INIT(liftoff, [2019/06/10], poliander@mailbox.org)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_CXX
AC_LANG([C++])
AC_CANONICAL_HOST
AC_HEADER_STDC
AC_C_CONST
AC_CHECK_HEADERS([algorithm cmath cstring map memory string vector])
PKG_PROG_PKG_CONFIG

# GL, GLU, GLEW, GLM ----------------------------------------------------------

AC_CHECK_HEADER([GL/gl.h],,
    [AC_MSG_ERROR([GL/gl.h not found, install libgl1-mesa-dev])])

AC_CHECK_HEADER([GL/glu.h],,
    [AC_MSG_ERROR([GL/glu.h not found, install libglu1-mesa-dev])])

AC_CHECK_HEADER([GL/glew.h],,
    [AC_MSG_ERROR([GL/glew.h not found, install libglew-dev])])

AC_CHECK_HEADER([glm/glm.hpp],,
    [AC_MSG_ERROR([glm/glm.hpp not found, install libglm-dev])])

case $host_os in
    *mingw32*)
        AC_DEFINE_DIR(GAMEDATA, ["resources"], [Game resources directory])
        PKG_CONFIG="$PKG_CONFIG --static"
        CXXFLAGS="$CXXFLAGS -DGLEW_STATIC"
        LIBS="$LIBS -lwinmm -lglew32 -lglu32 -lopengl32 -lgdi32 -ldxguid -static -static-libgcc -static-libstdc++"
    ;;

    *)
        AC_DEFINE_DIR(GAMEDATA, ["{prefix}/share/liftoff"], [Game resources directory])
        AC_CHECK_LIB(GL, glVertex3f,, [AC_MSG_ERROR([OpenGL library missing or not usable.])])
        AC_CHECK_LIB(GLU, gluPerspective,, [AC_MSG_ERROR([libglu1-mesa missing or not usable.])])
        AC_CHECK_LIB(GLEW, glewIsSupported,, [AC_MSG_ERROR([libglew-dev missing or not usable.])])
    ;;
esac

# SDL2 ------------------------------------------------------------------------

PKG_CHECK_MODULES(
    [SDL2],
    [sdl2 >= 2.0.0],
    [
        CPPFLAGS="$CPPFLAGS $SDL2_CFLAGS"
        LIBS="$LIBS $SDL2_LIBS"
    ],
    [AC_MSG_ERROR([SDL2 not located by pkg-config])]
)

# SDL2_mixer ------------------------------------------------------------------

PKG_CHECK_MODULES(
    [SDL2_mixer],
    [SDL2_mixer >= 2.0.0],
    [
        CPPFLAGS="$CPPFLAGS $SDL2_mixer_CFLAGS"
        LIBS="$LIBS $SDL2_mixer_LIBS"
    ],
    [AC_MSG_ERROR([SDL2_mixer not located by pkg-config])]
)

# libvorbisfile ---------------------------------------------------------------

PKG_CHECK_MODULES(
    [libvorbisfile],
    [vorbisfile >= 1.3.0],
    [
        CPPFLAGS="$CPPFLAGS $libvorbisfile_CFLAGS"
        LIBS="$LIBS $libvorbisfile_LIBS"
        AC_CHECK_HEADER([vorbis/vorbisfile.h],,
            [AC_MSG_ERROR([vorbis/vorbisfile.h not found, install libvorbis-dev])])
    ],
    [AC_MSG_ERROR([libvorbisfile not located by pkg-config])]
)

# libogg ----------------------------------------------------------------------

PKG_CHECK_MODULES(
    [libogg],
    [ogg >= 1.3.0],
    [
        CPPFLAGS="$CPPFLAGS $libogg_CFLAGS"
        LIBS="$LIBS $libogg_LIBS"
        AC_CHECK_HEADER([ogg/ogg.h],,
            [AC_MSG_ERROR([ogg/ogg.h not found, install libogg-dev])])
    ],
    [AC_MSG_ERROR([libogg not located by pkg-config])]
)

# FreeType2 -------------------------------------------------------------------

PKG_CHECK_MODULES(
    [freetype2],
    [freetype2 >= 2.8.0],
    [
        CPPFLAGS="$CPPFLAGS $freetype2_CFLAGS"
        LIBS="$LIBS $freetype2_LIBS"
        AC_CHECK_HEADER([ft2build.h],,
            [AC_MSG_ERROR([ft2build.h not found, install libfreetype-dev])])
    ],
    [AC_MSG_NOTICE([FreeType2 not located by pkg-config])]
)

# -----------------------------------------------------------------------------

AC_SUBST(CPPFLAGS)
AC_SUBST(LIBS)

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT

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
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])

# Checks for standard header files, typedefs, structures etc.
AC_HEADER_STDC
AC_CHECK_HEADERS([algorithm cmath cstring map memory vector])
AC_C_CONST
AC_CANONICAL_HOST
AC_DEFINE_DIR(GAMEDATA, ["{prefix}/share/liftoff"], [Game resources])

# SDL -------------------------------------------------------------------------

PKG_CHECK_MODULES(
    [SDL],
    [sdl >= 1.2.0],
    [],
    [SDL_VERSION=1.2.0
     AM_PATH_SDL($SDL_VERSION, :,
        AC_MSG_ERROR([SDL version $SDL_VERSION not found! http://www.libsdl.org]))])

LIBS="$LIBS $SDL_LIBS"

# SDL_mixer -------------------------------------------------------------------

PKG_CHECK_MODULES(
    [SDL_MIXER],
    [SDL_mixer >= 1.2.0],
    [],
    [AC_MSG_NOTICE([SDL_mixer not located by pkg-config, checking with AC CHECK LIB:])
     AC_CHECK_LIB([SDL_mixer],
        [Mix_OpenAudio],
        [],
        [AC_MSG_ERROR([SDL_mixer not found! http://www.libsdl.org/projects/SDL_mixer])])])

LIBS="$LIBS $SDL_MIXER_LIBS"

# GL + GLU --------------------------------------------------------------------

AC_CHECK_HEADER([GL/gl.h],, [AC_MSG_ERROR([GL/gl.h not found, install libgl1-mesa-dev])])
AC_CHECK_HEADER([GL/glu.h],, [AC_MSG_ERROR([GL/glu.h not found, install libglu1-mesa-dev])])

case $host_os in
    *mingw32*)
        LIBS="$LIBS -lvorbisfile -lvorbis -logg -lopengl32 -lglu32 -lwinmm -lgdi32 -ldxguid -static -static-libgcc -static-libstdc++"
    ;;

    *)
        AC_CHECK_LIB(GL, glVertex3f,, [AC_MSG_ERROR([OpenGL library missing or not usable.])])
        AC_CHECK_LIB(GLU, gluPerspective,, [AC_MSG_ERROR([libglu1-mesa missing or not usable.])])
    ;;
esac

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT

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
AC_INIT(liftoff, 0.3, poliander@mailbox.org)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])

# Checks for standard header files, typedefs, structures etc.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])
AC_C_CONST

# Checks for libraries.
AM_PATH_SDL(1.2.10,, AC_MSG_ERROR([SDL $SDL_VERSION not found.]))
LIBS="$LIBS $SDL_LIBS"
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"

AC_CHECK_HEADER([SDL.h],, [AC_MSG_ERROR([SDL.h not found, install libsdl1.2-dev])])
AC_CHECK_HEADER([SDL_image.h],, [AC_MSG_ERROR([SDL_image.h not found, install libsdl-image1.2-dev])])
AC_CHECK_HEADER([SDL_mixer.h],, [AC_MSG_ERROR([SDL_mixer.h not found, install libsdl-mixer1.2-dev])])
AC_CHECK_HEADER([GL/gl.h],, [AC_MSG_ERROR([GL/gl.h not found, install OpenGL header files])])
AC_CHECK_HEADER([GL/glu.h],, [AC_MSG_ERROR([GL/glu.h not found, install libglu1-mesa-dev])])

case "$host" in
    *mingw32)
        AC_SUBST([SDL_LIBS], [-lSDL])
        AC_SUBST([SDL_NET_LIBS], [-lSDL_net])
        AC_SUBST([SDL_MIXER_LIBS], [-lSDL_mixer])
    ;;

    *)
        AC_CHECK_LIB(SDL, SDL_Init,, [AC_MSG_ERROR([libsdl1.2 missing or not usable.])])
        AC_CHECK_LIB(SDL_image, IMG_LoadTGA_RW,, [AC_MSG_ERROR([libsdl-image1.2 missing or not usable.])])
        AC_CHECK_LIB(SDL_mixer, Mix_PlayMusic,, [AC_MSG_ERROR([libsdl-mixer1.2 missing or not usable.])])
        AC_CHECK_LIB(GL, glVertex3f,, [AC_MSG_ERROR([OpenGL library missing or not usable.])])
        AC_CHECK_LIB(GLU, gluPerspective,, [AC_MSG_ERROR([libglu1-mesa missing or not usable.])])
    ;;
esac


# Create output.
AC_DEFINE_DIR(GAMEDATA, [datadir], [Game resources])
AC_OUTPUT(Makefile src/Makefile)

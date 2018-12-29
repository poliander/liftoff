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
AC_INIT(liftoff, 0.2, r.pollesch@gmx.net)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for standard header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for SDL
AM_PATH_SDL(1.2.10, ,AC_MSG_ERROR([SDL $SDL_VERSION not found.]))

LIBS="$LIBS $SDL_LIBS"
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"

AC_CHECK_HEADER(SDL.h, ,echo "SDL header file not found!";exit 12)

AC_CHECK_HEADER(SDL_mixer.h, ,echo "SDL_mixer header file not found!";exit 12)
AC_CHECK_LIB(SDL_mixer,Mix_PlayMusic)

AC_CHECK_HEADER(SDL_image.h, ,echo "SDL_image header file not found!";exit 12)
AC_CHECK_LIB(SDL_image,IMG_LoadTGA_RW)

# Checks for OpenGL
AC_CHECK_HEADER(GL/gl.h, ,echo "GL/gl.h header file not found!";exit 12)
AC_CHECK_LIB(GL,glVertex3f)
AC_CHECK_HEADER(GL/glu.h, ,echo "GL/glu.h header file not found!";exit 12)
AC_CHECK_LIB(GLU,gluPerspective)
AC_CHECK_HEADER(GL/glut.h, ,echo "GL/glut.h header file not found!";exit 12)
AC_CHECK_LIB(glut,glutGet)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.

# config.h
AC_DEFINE_DIR(GAMEDATA, [datadir], [Game resources])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

# Process this file with autoconf to produce a configure script.

GEDIT_MAJOR=2.29.1
GEDIT_GUTACHTER_PLUGIN_MAJOR=0.0.1

AC_INIT(gedit-gutachter-plugin, 0.0.1, http://bugzilla.gnome.org/enter_bug.cgi?product=gedit)

AC_CONFIG_SRCDIR([gutachter])
AC_CONFIG_MACRO_DIR([m4])
AC_PREFIX_PROGRAM([gedit-gutachter-plugin])

AC_PREREQ(2.59)

AM_INIT_AUTOMAKE([1.8 dist-bzip2 no-dist-gzip])

AM_MAINTAINER_MODE
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_CONFIG_HEADERS([config.h])
AC_DISABLE_STATIC

IT_PROG_INTLTOOL([0.35.0])

AC_PROG_LIBTOOL

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# make sure we keep ACLOCAL_FLAGS around for maintainer builds to work
AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS -I m4")

# ================================================================
# Gettext stuff.
# ================================================================

GETTEXT_PACKAGE=gedit-gutachter-plugin
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package])
AM_GLIB_GNU_GETTEXT

# ================================================================
# Start of pkg-config checks
# ================================================================

PKG_CHECK_MODULES(GEDIT_GUTACHTER_PLUGIN, [
	glib-2.0 >= 2.16.0
	gtk+-2.0 >= 2.16.0
	gedit-2.20 >= 2.28.3
	gutachter-0.0.2
])

AC_ARG_ENABLE(deprecations,
              [AC_HELP_STRING([--enable-deprecations],
                              [warn about deprecated usages [default=no]])],,
              [enable_deprecations=no])

if test "x$enable_deprecations" = "xyes"
then
	DISABLE_DEPRECATED_CFLAGS="\
		-DG_DISABLE_DEPRECATED		\
		-DGDK_DISABLE_DEPRECATED	\
		-DGTK_DISABLE_DEPRECATED	\
		-DGDK_PIXBUF_DISABLE_DEPRECATED	\
		-DGNOME_DISABLE_DEPRECATED"
	AC_SUBST(DISABLE_DEPRECATED_CFLAGS)
fi

AC_CONFIG_FILES([
Makefile
gutachter/Makefile
po/Makefile.in])

AC_OUTPUT


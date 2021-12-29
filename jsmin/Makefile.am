#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# library definition
lib_LTLIBRARIES = libjsmin.la

libjsmin_la_SOURCES = \
	jsmin.c \
	stream.c

libjsmin_la_CPPFLAGS = \
	-I$(top_srcdir) -I$(top_builddir)

libjsmin_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

# program definition
bin_PROGRAMS = jsmin

jsmin_SOURCES = \
	main.c

jsmin_CPPFLAGS = \
	-I$(top_srcdir) -I$(top_builddir)

jsmin_LDADD = \
	libjsmin.la

# install development headers
pkgincludedir = $(includedir)/jsmin
pkginclude_HEADERS = \
	jsmin.h \
	macros.h \
	stream.h

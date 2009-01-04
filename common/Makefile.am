AM_CPPFLAGS = -I$(top_srcdir)/include

EXTRA_DIST = \
	common.h \
	config.h \
	config_windows.h \
	endian.h \
	memory.h \
	narrow_string.h \
	types.h \
	wide_string.h

# noinst_LTLIBRARIES = libcommon.la
#
# libcommon_la_SOURCES = \
#	memwatch.c memwatch.h

distclean: clean
	/bin/rm -f common/config.h
	/bin/rm -f Makefile


AM_CPPFLAGS = -I$(top_srcdir)/include

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
	character_string.c character_string.h \
	common.h \
	file_io.h \
	memory.h \
	notify.c notify.h \
	types.h

distclean: clean
	/bin/rm -f Makefile


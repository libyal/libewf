AM_CPPFLAGS = -I$(top_srcdir)/include

MEMWATCH_FILES = memwatch.c memwatch.h

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
	common.h \
	date_time.c date_time.h \
	endian.h \
	memory.h \
	narrow_string.h \
	notify.c notify.h \
	types.h \
	wide_string.h \
	$(MEMWATCH_FILES)

distclean: clean
	/bin/rm -f Makefile


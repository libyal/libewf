AM_CPPFLAGS = -I$(top_srcdir)/include

MEMWATCH_FILES = memwatch.c memwatch.h

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
	character_string.c character_string.h \
	common.h \
	date_time.c date_time.h \
	endian.h \
	error_string.c error_string.h \
	file_io.c file_io.h \
	file_stream_io.h \
	memory.h \
	narrow_string.h \
	notify.c notify.h \
	string_conversion.c string_conversion.h \
	system_string.c system_string.h \
	types.h \
	wide_string.h \
	$(MEMWATCH_FILES)

distclean: clean
	/bin/rm -f Makefile


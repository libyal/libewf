AM_CPPFLAGS = -I$(top_srcdir)/include

noinst_LTLIBRARIES = libcommon.la

libcommon_la_SOURCES = \
	character_string.c character_string.h \
	common.h \
	date_time.c date_time.h \
	date_time_string.c date_time_string.h \
	error_string.c error_string.h \
	file_io.c file_io.h \
	memory.h \
	notify.c notify.h \
	system_string.c system_string.h \
	types.h

distclean: clean
	/bin/rm -f Makefile


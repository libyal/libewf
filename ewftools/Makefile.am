if HAVE_LOCAL_LIBUNA
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common -I$(top_srcdir)/libuna
else
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common
endif

bin_PROGRAMS = ewfacquire ewfacquirestream ewfalter ewfexport ewfinfo ewfverify

ewfacquire_SOURCES = \
	byte_size_string.c byte_size_string.h \
	character_string.c character_string.h \
	error_string.c error_string.h \
	ewfacquire.c \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	process_status.c process_status.h \
	string_conversion.c string_conversion.h \
	system_string.c system_string.h

ewfacquire_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfacquirestream_SOURCES = \
	byte_size_string.c byte_size_string.h \
	character_string.c character_string.h \
	error_string.c error_string.h \
	ewfacquirestream.c \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	process_status.c process_status.h \
	string_conversion.c string_conversion.h \
	system_string.c system_string.h

ewfacquirestream_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfalter_SOURCES = \
	byte_size_string.c byte_size_string.h \
	character_string.c character_string.h \
	error_string.c error_string.h \
	ewfalter.c \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	process_status.c process_status.h \
	string_conversion.c string_conversion.h \
	system_string.c system_string.h

ewfalter_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfexport_SOURCES = \
	byte_size_string.c byte_size_string.h \
	character_string.c character_string.h \
	error_string.c error_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfexport.c \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	file_io.c file_io.h \
	glob.c glob.h \
	process_status.c process_status.h \
	string_conversion.c string_conversion.h \
	system_string.c system_string.h

ewfexport_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	character_string.c character_string.h \
	error_string.c error_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfguid.c ewfguid.h \
	ewfinfo.c \
	ewfmd5.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	file_io.c file_io.h \
	glob.c glob.h \
	process_status.c process_status.h \
	string_conversion.c string_conversion.h \
	system_string.c system_string.h

ewfinfo_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	character_string.c character_string.h \
	error_string.c error_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfmd5.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	ewfverify.c \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	process_status.c process_status.h \
	string_conversion.c string_conversion.h \
	system_string.c system_string.h

ewfverify_LDADD  = ../common/libcommon.la ../libewf/libewf.la

distclean: clean
	/bin/rm -f Makefile


if HAVE_LOCAL_LIBUNA
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common -I$(top_srcdir)/libuna
else
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common
endif

bin_PROGRAMS = ewfacquire ewfacquirestream ewfalter ewfexport ewfinfo ewfverify

ewfacquire_SOURCES = \
	ewfbyte_size_string.c ewfbyte_size_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfglob.c ewfglob.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfprocess_status.c ewfprocess_status.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	ewfacquire.c

ewfacquire_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfacquirestream_SOURCES = \
	ewfbyte_size_string.c ewfbyte_size_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfglob.c ewfglob.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfprocess_status.c ewfprocess_status.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	ewfacquirestream.c

ewfacquirestream_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfalter_SOURCES = \
	ewfbyte_size_string.c ewfbyte_size_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfglob.c ewfglob.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfprocess_status.c ewfprocess_status.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	ewfalter.c

ewfalter_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfexport_SOURCES = \
	ewfbyte_size_string.c ewfbyte_size_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfglob.c ewfglob.h \
	ewfinput.c ewfinput.h \
	ewfmd5.h \
	ewfprocess_status.c ewfprocess_status.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	ewfexport.c

ewfexport_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfinfo_SOURCES = \
	ewfbyte_size_string.c ewfbyte_size_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfglob.c ewfglob.h \
	ewfguid.c ewfguid.h \
	ewfmd5.h \
	ewfprocess_status.c ewfprocess_status.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	ewfinfo.c

ewfinfo_LDADD  = ../common/libcommon.la ../libewf/libewf.la

ewfverify_SOURCES = \
	ewfbyte_size_string.c ewfbyte_size_string.h \
	ewfcommon.c ewfcommon.h \
	ewfdigest_context.c ewfdigest_context.h \
	ewfdigest_hash.c ewfdigest_hash.h \
	ewfgetopt.c ewfgetopt.h \
	ewfglob.c ewfglob.h \
	ewfmd5.h \
	ewfprocess_status.c ewfprocess_status.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfstring.c ewfstring.h \
	ewfsha1.h \
	ewfverify.c

ewfverify_LDADD  = ../common/libcommon.la ../libewf/libewf.la

distclean: clean
	/bin/rm -f Makefile


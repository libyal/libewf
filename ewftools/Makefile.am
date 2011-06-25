AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBMFDATA_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBMDHASHF_CPPFLAGS@ \
	@LIBODRAW_CPPFLAGS@ \
	@LIBSMDEV_CPPFLAGS@ \
	@LIBSMRAW_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBEWF_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	ewfacquire \
	ewfacquirestream \
	ewfdebug \
	ewfexport \
	ewfinfo \
	ewfverify

ewfacquire_SOURCES = \
	byte_size_string.c byte_size_string.h \
	device_handle.c device_handle.h \
	ewfacquire.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libewf.h \
	ewftools_libmdhashf.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h

ewfacquire_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libewf/libewf.la \
	../libsystem/libsystem.la \
	@LIBMDHASHF_LIBADD@ \
	@LIBODRAW_LIBADD@ \
	@LIBSMDEV_LIBADD@ \
	@LIBSMRAW_LIBADD@ \
	@LIBINTL@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBDL_LIBADD@ \
	@MEMWATCH_LIBADD@

ewfacquirestream_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	ewfacquirestream.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libewf.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	log_handle.c log_handle.h \
	md5.h \
	platform.c platform.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h

ewfacquirestream_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libewf/libewf.la \
	../libsystem/libsystem.la \
	@LIBMDHASHF_LIBADD@ \
	@LIBINTL@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBDL_LIBADD@ \
	@MEMWATCH_LIBADD@

ewfdebug_SOURCES = \
	byte_size_string.c byte_size_string.h \
	ewfdebug.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libewf.h

ewfdebug_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libewf/libewf.la \
	../libsystem/libsystem.la \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

ewfexport_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfexport.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libewf.h \
	ewftools_libsmraw.h \
	export_handle.c export_handle.h \
	guid.c guid.h \
	log_handle.c log_handle.h \
	md5.h \
	platform.c platform.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h

ewfexport_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libewf/libewf.la \
	../libsystem/libsystem.la \
	@LIBMDHASHF_LIBADD@ \
	@LIBSMRAW_LIBADD@ \
	@LIBINTL@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBDL_LIBADD@ \
	@MEMWATCH_LIBADD@

ewfinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfinfo.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libewf.h \
	guid.c guid.h \
	info_handle.c info_handle.h \
	platform.c platform.h

ewfinfo_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libewf/libewf.la \
	../libsystem/libsystem.la \
	@LIBINTL@ \
	@LIBUUID_LIBADD@ \
	@MEMWATCH_LIBADD@

ewfverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libewf.h \
	ewfverify.c \
	log_handle.c log_handle.h \
	md5.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h \
	verification_handle.c verification_handle.h

ewfverify_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libewf/libewf.la \
	../libsystem/libsystem.la \
	@LIBMDHASHF_LIBADD@ \
	@LIBINTL@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on ewfacquire ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfacquire_SOURCES) 
	@echo "Running splint on ewfacquirestream ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfacquirestream_SOURCES) 
	@echo "Running splint on ewfdebug ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfdebug_SOURCES) 
	@echo "Running splint on ewfexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfexport_SOURCES) 
	@echo "Running splint on ewfinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfinfo_SOURCES) 
	@echo "Running splint on ewfverify ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfverify_SOURCES) 


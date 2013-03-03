AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBMFDATA_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBODRAW_CPPFLAGS@ \
	@LIBSMDEV_CPPFLAGS@ \
	@LIBSMRAW_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBEWF_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	ewfacquire \
	ewfacquirestream \
	ewfdebug \
	ewfexport \
	ewfinfo \
	ewfmount \
	ewfrecover \
	ewfverify

ewfacquire_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	device_handle.c device_handle.h \
	ewfacquire.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcsplit.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h

ewfacquire_LDADD = \
	@LIBODRAW_LIBADD@ \
	@LIBSMDEV_LIBADD@ \
	@LIBSMRAW_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

ewfacquirestream_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfacquirestream.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcsplit.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h

ewfacquirestream_LDADD = \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

ewfdebug_SOURCES = \
	byte_size_string.c byte_size_string.h \
	ewfdebug.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h

ewfdebug_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

ewfexport_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfexport.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libcfile.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcsplit.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	export_handle.c export_handle.h \
	guid.c guid.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h

ewfexport_LDADD = \
	@LIBSMRAW_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

ewfinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfinfo.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcsplit.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	guid.c guid.h \
	info_handle.c info_handle.h \
	platform.c platform.h

ewfinfo_LDADD = \
	@LIBUUID_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

ewfmount_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfmount.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	guid.c guid.h \
	mount_handle.c mount_handle.h \
	platform.c platform.h

ewfmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

ewfrecover_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfrecover.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libcfile.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	export_handle.c export_handle.h \
	guid.c guid.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h

ewfrecover_LDADD = \
	@LIBSMRAW_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

ewfverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcstring.h \
	ewftools_libcsystem.h \
	ewftools_libewf.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewfverify.c \
	log_handle.c log_handle.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h \
	verification_handle.c verification_handle.h

ewfverify_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

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
	@echo "Running splint on ewfmount ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfmount_SOURCES) 
	@echo "Running splint on ewfrecover ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfrecover_SOURCES) 
	@echo "Running splint on ewfverify ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfverify_SOURCES) 


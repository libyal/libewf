AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCDATETIME_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBODRAW_CPPFLAGS@ \
	@LIBSMDEV_CPPFLAGS@ \
	@LIBSMRAW_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
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
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_i18n.h \
	ewftools_libbfio.h \
	ewftools_libcdata.h \
	ewftools_libcdatetime.h \
	ewftools_libcerror.h \
	ewftools_libcfile.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcsplit.h \
	ewftools_libcthreads.h \
	ewftools_libewf.h \
	ewftools_libfdatetime.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h \
	storage_media_buffer_queue.c storage_media_buffer_queue.h

ewfacquire_LDADD = \
	@LIBODRAW_LIBADD@ \
	@LIBSMDEV_LIBADD@ \
	@LIBSMRAW_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATETIME_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

ewfacquirestream_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfacquirestream.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_i18n.h \
	ewftools_libbfio.h \
	ewftools_libcdata.h \
	ewftools_libcdatetime.h \
	ewftools_libcerror.h \
	ewftools_libcfile.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcsplit.h \
	ewftools_libcthreads.h \
	ewftools_libewf.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h \
	storage_media_buffer_queue.c storage_media_buffer_queue.h

ewfacquirestream_LDADD = \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATETIME_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

ewfdebug_SOURCES = \
	byte_size_string.c byte_size_string.h \
	ewfdebug.c \
	ewfinput.c ewfinput.h \
	ewftools_find.h \
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_glob.c ewftools_glob.h \
	ewftools_i18n.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libewf.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h

ewfdebug_LDADD = \
	@LIBUNA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

ewfexport_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfexport.c \
	ewfinput.c ewfinput.h \
	ewftools_find.h \
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_glob.c ewftools_glob.h \
	ewftools_i18n.h \
	ewftools_libbfio.h \
	ewftools_libcdata.h \
	ewftools_libcdatetime.h \
	ewftools_libcerror.h \
	ewftools_libcfile.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcsplit.h \
	ewftools_libcthreads.h \
	ewftools_libewf.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h \
	export_handle.c export_handle.h \
	guid.c guid.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h \
	storage_media_buffer_queue.c storage_media_buffer_queue.h

ewfexport_LDADD = \
	@LIBSMRAW_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATETIME_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

ewfinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfinfo.c \
	ewfinput.c ewfinput.h \
	ewftools_find.h \
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_glob.c ewftools_glob.h \
	ewftools_i18n.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcsplit.h \
	ewftools_libewf.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h \
	guid.c guid.h \
	info_handle.c info_handle.h \
	platform.c platform.h

ewfinfo_LDADD = \
	@LIBUUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

ewfmount_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfmount.c \
	ewfinput.c ewfinput.h \
	ewftools_find.h \
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_glob.c ewftools_glob.h \
	ewftools_i18n.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcthreads.h \
	ewftools_libewf.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h \
	guid.c guid.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h \
	platform.c platform.h

ewfmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

ewfrecover_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfrecover.c \
	ewfinput.c ewfinput.h \
	ewftools_find.h \
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_glob.c ewftools_glob.h \
	ewftools_i18n.h \
	ewftools_libbfio.h \
	ewftools_libcdata.h \
	ewftools_libcdatetime.h \
	ewftools_libcerror.h \
	ewftools_libcfile.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcpath.h \
	ewftools_libcthreads.h \
	ewftools_libewf.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h \
	export_handle.c export_handle.h \
	guid.c guid.h \
	log_handle.c log_handle.h \
	platform.c platform.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h \
	storage_media_buffer_queue.c storage_media_buffer_queue.h

ewfrecover_LDADD = \
	@LIBSMRAW_LIBADD@ \
	@LIBUUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATETIME_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

ewfverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewftools_find.h \
	ewftools_getopt.c ewftools_getopt.h \
	ewftools_glob.c ewftools_glob.h \
	ewftools_i18n.h \
	ewftools_libbfio.h \
	ewftools_libcdata.h \
	ewftools_libcdatetime.h \
	ewftools_libcerror.h \
	ewftools_libclocale.h \
	ewftools_libcnotify.h \
	ewftools_libcthreads.h \
	ewftools_libewf.h \
	ewftools_libfvalue.h \
	ewftools_libhmac.h \
	ewftools_libodraw.h \
	ewftools_libsmdev.h \
	ewftools_libsmraw.h \
	ewftools_libuna.h \
	ewftools_output.c ewftools_output.h \
	ewftools_signal.c ewftools_signal.h \
	ewftools_system_string.c ewftools_system_string.h \
	ewftools_unused.h \
	ewfverify.c \
	log_handle.c log_handle.h \
	process_status.c process_status.h \
	storage_media_buffer.c storage_media_buffer.h \
	storage_media_buffer_queue.c storage_media_buffer_queue.h \
	verification_handle.c verification_handle.h

ewfverify_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATETIME_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

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


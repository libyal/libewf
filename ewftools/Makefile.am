AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common -I$(top_srcdir)/liberror -I$(top_srcdir)/libsystem @LIBUNA_CPPFLAGS@ @LIBBFIO_CPPFLAGS@ @MEMWATCH_CPPFLAGS@ @LIBEWF_DLL_IMPORT@
AM_LDFLAGS  = @STATIC_LDFLAGS@

bin_PROGRAMS = ewfacquire ewfacquirestream ewfalter ewfdebug ewfexport ewfinfo ewfverify

ewfacquire_SOURCES = \
	byte_size_string.c byte_size_string.h \
	device_handle.c device_handle.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	directory_io.h \
	ewfacquire.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	file_io.c file_io.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	io_ata.c io_ata.h \
	io_bus.h \
	io_optical_disk.c io_optical_disk.h \
	io_scsi.c io_scsi.h \
	io_usb.c io_usb.h \
	md5.h \
	platform.c platform.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h

ewfacquire_LDADD  = ../liberror/liberror.la ../libewf/libewf.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

ewfacquirestream_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	directory_io.h \
	ewfacquirestream.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	file_io.c file_io.h \
	guid.c guid.h \
	imaging_handle.c imaging_handle.h \
	md5.h \
	platform.c platform.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h

ewfacquirestream_LDADD  = ../liberror/liberror.la ../libewf/libewf.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

ewfalter_SOURCES = \
	alteration_handle.c alteration_handle.h \
	byte_size_string.c byte_size_string.h \
	directory_io.h \
	ewfalter.c \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	storage_media_buffer.c storage_media_buffer.h

ewfalter_LDADD  = ../liberror/liberror.la ../libewf/libewf.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

ewfdebug_SOURCES = \
	byte_size_string.c byte_size_string.h \
	ewfdebug.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h

ewfdebug_LDADD  = ../liberror/liberror.la ../libewf/libewf.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

ewfexport_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	directory_io.h \
	ewfcommon.h \
	ewfexport.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	export_handle.c export_handle.h \
	file_io.c file_io.h \
	guid.c guid.h \
	md5.h \
	platform.c platform.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h

ewfexport_LDADD  = ../liberror/liberror.la ../libewf/libewf.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

ewfinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	directory_io.h \
	ewfinfo.c \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	guid.c guid.h \
	info_handle.c info_handle.h

ewfinfo_LDADD  = ../liberror/liberror.la ../libewf/libewf.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

ewfverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	directory_io.h \
	ewfcommon.h \
	ewfinput.c ewfinput.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfverify.c \
	file_io.c file_io.h \
	md5.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h \
	verification_handle.c verification_handle.h

ewfverify_LDADD  = ../liberror/liberror.la ../libewf/libewf.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on ewfacquire ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfacquire_SOURCES) 
	@echo "Running splint on ewfacquirestream ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfacquirestream_SOURCES) 
	@echo "Running splint on ewfalter ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfalter_SOURCES) 
	@echo "Running splint on ewfdebug ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfdebug_SOURCES) 
	@echo "Running splint on ewfexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfexport_SOURCES) 
	@echo "Running splint on ewfinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfinfo_SOURCES) 
	@echo "Running splint on ewfverify ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfverify_SOURCES) 


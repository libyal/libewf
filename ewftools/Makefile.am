AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common -I$(top_srcdir)/liberror @LIBUNA_CPPFLAGS@ @MEMWATCH_CPPFLAGS@

bin_PROGRAMS = ewfacquire ewfacquirestream ewfalter ewfexport ewfinfo ewfverify

ewfacquire_SOURCES = \
	byte_size_string.c byte_size_string.h \
	date_time.c date_time.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	error_string.c error_string.h \
	ewfacquire.c \
	ewfcommon.c ewfcommon.h \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewflibewf.c ewflibewf.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	md5.h \
	notify.c notify.h \
	process_status.c process_status.h \
	sha1.h \
	system_string.c system_string.h

ewfacquire_LDADD  = ../liberror/liberror.la ../libewf/libewf.la @MEMWATCH_LIBADD@

ewfacquirestream_SOURCES = \
	byte_size_string.c byte_size_string.h \
	date_time.c date_time.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	error_string.c error_string.h \
	ewfacquirestream.c \
	ewfcommon.c ewfcommon.h \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewflibewf.c ewflibewf.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	md5.h \
	notify.c notify.h \
	process_status.c process_status.h \
	sha1.h \
	system_string.c system_string.h

ewfacquirestream_LDADD  = ../liberror/liberror.la ../libewf/libewf.la @MEMWATCH_LIBADD@

ewfalter_SOURCES = \
	byte_size_string.c byte_size_string.h \
	date_time.c date_time.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	error_string.c error_string.h \
	ewfalter.c \
	ewfcommon.c ewfcommon.h \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewflibewf.c ewflibewf.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	md5.h \
	notify.c notify.h \
	process_status.c process_status.h \
	sha1.h \
	system_string.c system_string.h

ewfalter_LDADD  = ../liberror/liberror.la ../libewf/libewf.la @MEMWATCH_LIBADD@

ewfexport_SOURCES = \
	byte_size_string.c byte_size_string.h \
	date_time.c date_time.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	error_string.c error_string.h \
	ewfcommon.c ewfcommon.h \
	ewfexport.c \
	ewfgetopt.c ewfgetopt.h \
	ewfinput.c ewfinput.h \
	ewflibewf.c ewflibewf.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	file_io.c file_io.h \
	glob.c glob.h \
	md5.h \
	notify.c notify.h \
	process_status.c process_status.h \
	sha1.h \
	system_string.c system_string.h

ewfexport_LDADD  = ../liberror/liberror.la ../libewf/libewf.la @MEMWATCH_LIBADD@

ewfinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	date_time.c date_time.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	error_string.c error_string.h \
	ewfcommon.c ewfcommon.h \
	ewfgetopt.c ewfgetopt.h \
	ewfinfo.c \
	ewflibewf.c ewflibewf.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	file_io.c file_io.h \
	glob.c glob.h \
	guid.c guid.h \
	md5.h \
	notify.c notify.h \
	process_status.c process_status.h \
	sha1.h \
	system_string.c system_string.h

ewfinfo_LDADD  = ../liberror/liberror.la ../libewf/libewf.la @MEMWATCH_LIBADD@

ewfverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	date_time.c date_time.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	error_string.c error_string.h \
	ewfcommon.c ewfcommon.h \
	ewfgetopt.c ewfgetopt.h \
	ewflibewf.c ewflibewf.h \
	ewfoutput.c ewfoutput.h \
	ewfsignal.c ewfsignal.h \
	ewfverify.c \
	file_io.c file_io.h \
	file_stream_io.h \
	glob.c glob.h \
	md5.h \
	notify.c notify.h \
	process_status.c process_status.h \
	sha1.h \
	storage_media_buffer.c storage_media_buffer.h \
	system_string.c system_string.h \
	verification_handle.c verification_handle.h

ewfverify_LDADD  = ../liberror/liberror.la ../libewf/libewf.la @MEMWATCH_LIBADD@

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on ewfacquire ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfacquire_SOURCES) 
	@echo "Running splint on ewfacquirestream ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfacquirestream_SOURCES) 
	@echo "Running splint on ewfalter ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfalter_SOURCES) 
	@echo "Running splint on ewfexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfexport_SOURCES) 
	@echo "Running splint on ewfinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfinfo_SOURCES) 
	@echo "Running splint on ewfverify ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(ewfverify_SOURCES) 


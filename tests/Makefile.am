AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@BZIP2_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCAES_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBEWF_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYEWF = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_glob.sh \
	test_write_functions.sh \
	test_ewfverify.sh \
	test_ewfverify_logical.sh \
	test_ewfacquire.sh \
	test_ewfacquire_optical.sh \
	test_ewfacquirestream.sh \
	test_ewfacquire_resume.sh \
	test_ewfinfo.sh \
	test_ewfexport.sh \
	test_ewfexport_logical.sh \
	$(TESTS_PYEWF)

check_SCRIPTS = \
	create_ewfacquire_optical_option_sets.sh \
	create_ewfacquire_option_sets.sh \
	create_ewfacquirestream_option_sets.sh \
	create_ewfexport_option_sets.sh \
	pyewf_test_support.py \
	pyewf_test_read.py \
	test_ewfacquire.sh \
	test_ewfacquire_device.sh \
	test_ewfacquire_optical.sh \
	test_ewfacquire_resume.sh \
	test_ewfacquirestream.sh \
	test_ewfinfo.sh \
	test_ewfexport.sh \
	test_ewfexport_logical.sh \
	test_ewfverify.sh \
	test_ewfverify_logical.sh \
	test_glob.sh \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh \
	test_write_functions.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	ewf_test_error \
	ewf_test_glob \
	ewf_test_handle \
	ewf_test_open_close \
	ewf_test_support \
	ewf_test_truncate \
	ewf_test_write \
	ewf_test_write_chunk

ewf_test_error_SOURCES = \
	ewf_test_error.c \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_error_LDADD = \
	../libewf/libewf.la

ewf_test_glob_SOURCES = \
	ewf_test_glob.c \
	ewf_test_libewf.h \
	ewf_test_unused.h

ewf_test_glob_LDADD = \
	../libewf/libewf.la \
	@PTHREAD_LIBADD@

ewf_test_handle_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_handle.c \
	ewf_test_libcerror.h \
	ewf_test_libcclocal.h \
	ewf_test_libewf.h \
	ewf_test_libuna.h \
	ewf_test_memory.c ewf_test_memory.h

ewf_test_handle_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

ewf_test_open_close_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libewf.h \
	ewf_test_libcerror.h \
	ewf_test_libcnotify.h \
	ewf_test_open_close.c \
	ewf_test_unused.h

ewf_test_open_close_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

ewf_test_support_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libcerror.h \
	ewf_test_libcclocal.h \
	ewf_test_libewf.h \
	ewf_test_libuna.h \
	ewf_test_macros.h \
	ewf_test_support.c \
	ewf_test_unused.h

ewf_test_support_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_truncate_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libcerror.h \
	ewf_test_libcfile.h \
	ewf_test_libcnotify.h \
	ewf_test_truncate.c \
	ewf_test_unused.h

ewf_test_truncate_LDADD = \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

ewf_test_write_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_unused.h \
	ewf_test_write.c

ewf_test_write_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

ewf_test_write_chunk_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_unused.h \
	ewf_test_write_chunk.c

ewf_test_write_chunk_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile


AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBEWF_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYEWF = \
	test_python_functions.sh
endif

TESTS = \
	test_api_functions.sh \
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
	pyewf_test_get_version.py \
	pyewf_test_open_close.py \
	pyewf_test_read.py \
	pyewf_test_seek.py \
	test_api_functions.sh \
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
	test_python_functions.sh \
	test_runner.sh \
	test_write_functions.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	ewf_test_get_version \
	ewf_test_glob \
	ewf_test_handle_get_values \
	ewf_test_open_close \
	ewf_test_read \
	ewf_test_read_chunk \
	ewf_test_seek \
	ewf_test_truncate \
	ewf_test_write \
	ewf_test_write_chunk

ewf_test_get_version_SOURCES = \
	ewf_test_get_version.c \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_unused.h

ewf_test_get_version_LDADD = \
	../libewf/libewf.la \
	@LIBCSTRING_LIBADD@

ewf_test_glob_SOURCES = \
	ewf_test_glob.c \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_unused.h

ewf_test_glob_LDADD = \
	../libewf/libewf.la \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_handle_get_values_SOURCES = \
	ewf_test_handle_get_values.c \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_libewf.h \
	ewf_test_unused.h

ewf_test_handle_get_values_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_open_close_SOURCES = \
	ewf_test_libewf.h \
	ewf_test_libcerror.h \
	ewf_test_libcnotify.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_open_close.c \
	ewf_test_unused.h

ewf_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_read_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_libcthreads.h \
	ewf_test_libewf.h \
	ewf_test_read.c \
	ewf_test_unused.h

ewf_test_read_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_read_chunk_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_libewf.h \
	ewf_test_read_chunk.c \
	ewf_test_unused.h

ewf_test_read_chunk_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_seek_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_libewf.h \
	ewf_test_seek.c \
	ewf_test_unused.h

ewf_test_seek_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_truncate_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcfile.h \
	ewf_test_libcnotify.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_truncate.c \
	ewf_test_unused.h

ewf_test_truncate_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_write_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_unused.h \
	ewf_test_write.c

ewf_test_write_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_write_chunk_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_libcsystem.h \
	ewf_test_unused.h \
	ewf_test_write_chunk.c

ewf_test_write_chunk_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile


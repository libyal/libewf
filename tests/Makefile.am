AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@

if HAVE_PYTHON
TESTS_PYEWF  = \
	test_pyewf.sh
endif

TESTS = \
	test_glob.sh \
	test_seek.sh \
	test_read.sh \
	test_write.sh \
	test_read_write.sh \
	test_read_write_delta.sh \
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
	test_ewfacquire.sh \
	test_ewfacquire_optical.sh \
	test_ewfacquire_resume.sh \
	test_ewfacquirestream.sh \
	test_ewfinfo.sh \
	test_ewfexport.sh \
	test_ewfexport_logical.sh \
	test_ewfverify.sh \
	test_ewfverify_logical.sh \
	test_glob.sh \
	test_pyewf.sh \
	test_read.sh \
	test_read_write.sh \
	test_read_write_delta.sh \
	test_seek.sh \
	test_write.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	ewf_test_glob \
	ewf_test_read \
	ewf_test_read_write\
	ewf_test_read_write_delta \
	ewf_test_seek \
	ewf_test_truncate \
	ewf_test_write \
	ewf_test_write_chunk

ewf_test_glob_SOURCES = \
	ewf_test_glob.c \
	ewf_test_libcstring.h \
	ewf_test_libewf.h

ewf_test_glob_LDADD = \
	@LIBCSTRING_LIBADD@ \
	../libewf/libewf.la

ewf_test_read_SOURCES = \
	ewf_test_definitions.h \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_read.c

ewf_test_read_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

ewf_test_read_write_SOURCES = \
	ewf_test_definitions.h \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_libewf.h \
	ewf_test_read_write.c

ewf_test_read_write_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

ewf_test_read_write_delta_SOURCES = \
	ewf_test_definitions.h \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_libewf.h \
	ewf_test_read_write_delta.c

ewf_test_read_write_delta_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

ewf_test_seek_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_seek.c

ewf_test_seek_LDADD = \
	../libewf/libewf.la
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

ewf_test_truncate_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libcfile.h \
	ewf_test_libcnotify.h \
	ewf_test_libcstring.h \
	ewf_test_libcsystem.h \
	ewf_test_truncate.c

ewf_test_truncate_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

ewf_test_write_SOURCES = \
	ewf_test_definitions.h \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_write.c

ewf_test_write_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

ewf_test_write_chunk_SOURCES = \
	ewf_test_definitions.h \
	ewf_test_libcerror.h \
	ewf_test_libcstring.h \
	ewf_test_libewf.h \
	ewf_test_libcsystem.h \
	ewf_test_write_chunk.c

ewf_test_write_chunk_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile


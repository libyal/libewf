if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBEWF_DLL_IMPORT@

pyexec_LTLIBRARIES = pyewf.la

pyewf_la_SOURCES = \
	pyewf.c pyewf.h \
	pyewf_codepage.c pyewf_codepage.h \
	pyewf_compression_methods.c pyewf_compression_methods.h \
	pyewf_datetime.c pyewf_datetime.h \
	pyewf_error.c pyewf_error.h \
	pyewf_file_entries.c pyewf_file_entries.h \
	pyewf_file_entry.c pyewf_file_entry.h \
	pyewf_file_object_io_handle.c pyewf_file_object_io_handle.h \
	pyewf_file_objects_io_pool.c pyewf_file_objects_io_pool.h \
	pyewf_handle.c pyewf_handle.h \
	pyewf_integer.c pyewf_integer.h \
	pyewf_libbfio.h \
	pyewf_libcerror.h \
	pyewf_libclocale.h \
	pyewf_libewf.h \
	pyewf_media_flags.c pyewf_media_flags.h \
	pyewf_media_types.c pyewf_media_types.h \
	pyewf_metadata.c pyewf_metadata.h \
	pyewf_python.h \
	pyewf_unused.h

pyewf_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libewf/libewf.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyewf_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyewf_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile


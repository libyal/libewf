if HAVE_LIB_FUZZING_ENGINE
AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

bin_PROGRAMS = \
	data_chunk_fuzzer \
	file_entry_fuzzer \
	handle_fuzzer \
	write_fuzzer

data_chunk_fuzzer_SOURCES = \
	data_chunk_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libewf.h

data_chunk_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

file_entry_fuzzer_SOURCES = \
	file_entry_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libewf.h

file_entry_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

handle_fuzzer_SOURCES = \
	handle_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libewf.h

handle_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

write_fuzzer_SOURCES = \
	write_fuzzer.cc \
	ossfuzz_libewf.h

write_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@
endif

DISTCLEANFILES = \
	Makefile \
	Makefile.in

splint-local:
	@echo "Running splint on data_chunk_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(data_chunk_fuzzer_SOURCES)
	@echo "Running splint on file_entry_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(file_entry_fuzzer_SOURCES)
	@echo "Running splint on handle_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(handle_fuzzer_SOURCES)
	@echo "Running splint on write_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(write_fuzzer_SOURCES)


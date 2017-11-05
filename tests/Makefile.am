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
	test_ewfinfo.sh \
	test_ewfexport.sh \
	test_ewfexport_logical.sh \
	test_ewfverify.sh \
	test_ewfverify_logical.sh \
	test_ewfacquire.sh \
	test_ewfacquire_optical.sh \
	test_ewfacquirestream.sh \
	test_ewfacquire_resume.sh \
	$(TESTS_PYEWF)

check_SCRIPTS = \
	create_ewfacquire_optical_option_sets.sh \
	create_ewfacquire_option_sets.sh \
	create_ewfacquirestream_option_sets.sh \
	create_ewfexport_option_sets.sh \
	pyewf_test_read.py \
	pyewf_test_support.py \
	test_ewfacquire.sh \
	test_ewfacquire_device.sh \
	test_ewfacquire_optical.sh \
	test_ewfacquire_resume.sh \
	test_ewfacquirestream.sh \
	test_ewfexport.sh \
	test_ewfexport_logical.sh \
	test_ewfinfo.sh \
	test_ewfverify_logical.sh \
	test_ewfverify.sh \
	test_glob.sh \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh \
	test_write_functions.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	ewf_test_analytical_data \
	ewf_test_case_data \
	ewf_test_chunk_data \
	ewf_test_chunk_group \
	ewf_test_chunk_table \
	ewf_test_data_chunk \
	ewf_test_device_information \
	ewf_test_error \
	ewf_test_file_entry \
	ewf_test_glob \
	ewf_test_handle \
	ewf_test_hash_sections \
	ewf_test_hash_values \
	ewf_test_header_sections \
	ewf_test_header_values \
	ewf_test_io_handle \
	ewf_test_media_values \
	ewf_test_notify \
	ewf_test_read_io_handle \
	ewf_test_restart_data \
	ewf_test_section \
	ewf_test_sector_range \
	ewf_test_segment_file \
	ewf_test_segment_table \
	ewf_test_single_file_entry \
	ewf_test_single_files \
	ewf_test_support \
	ewf_test_truncate \
	ewf_test_write \
	ewf_test_write_chunk \
	ewf_test_write_io_handle

ewf_test_analytical_data_SOURCES = \
	ewf_test_analytical_data.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_analytical_data_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_case_data_SOURCES = \
	ewf_test_case_data.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_case_data_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_chunk_data_SOURCES = \
	ewf_test_chunk_data.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_chunk_data_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_chunk_group_SOURCES = \
	ewf_test_chunk_group.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_chunk_group_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_chunk_table_SOURCES = \
	ewf_test_chunk_table.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_chunk_table_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_data_chunk_SOURCES = \
	ewf_test_data_chunk.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_data_chunk_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_device_information_SOURCES = \
	ewf_test_device_information.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_device_information_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_error_SOURCES = \
	ewf_test_error.c \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_error_LDADD = \
	../libewf/libewf.la

ewf_test_file_entry_SOURCES = \
	ewf_test_file_entry.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_file_entry_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_glob_SOURCES = \
	ewf_test_glob.c \
	ewf_test_libewf.h \
	ewf_test_unused.h

ewf_test_glob_LDADD = \
	../libewf/libewf.la

ewf_test_handle_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_handle.c \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libclocale.h \
	ewf_test_libcnotify.h \
	ewf_test_libewf.h \
	ewf_test_libuna.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_handle_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@

ewf_test_hash_sections_SOURCES = \
	ewf_test_hash_sections.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_hash_sections_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_hash_values_SOURCES = \
	ewf_test_hash_values.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_hash_values_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_header_sections_SOURCES = \
	ewf_test_header_sections.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_header_sections_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_header_values_SOURCES = \
	ewf_test_header_values.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_header_values_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_io_handle_SOURCES = \
	ewf_test_io_handle.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_io_handle_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_media_values_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_media_values.c \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_media_values_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_notify_SOURCES = \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_notify.c \
	ewf_test_unused.h

ewf_test_notify_LDADD = \
	../libewf/libewf.la

ewf_test_read_io_handle_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_read_io_handle.c \
	ewf_test_unused.h

ewf_test_read_io_handle_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_restart_data_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_restart_data.c \
	ewf_test_unused.h

ewf_test_restart_data_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_section_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_section.c \
	ewf_test_unused.h

ewf_test_section_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_sector_range_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_sector_range.c \
	ewf_test_unused.h

ewf_test_sector_range_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_segment_file_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_segment_file.c \
	ewf_test_unused.h

ewf_test_segment_file_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_segment_table_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_segment_table.c \
	ewf_test_unused.h

ewf_test_segment_table_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_single_file_entry_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_single_file_entry.c \
	ewf_test_unused.h

ewf_test_single_file_entry_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_single_files_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_single_files.c \
	ewf_test_unused.h

ewf_test_single_files_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_support_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libclocale.h \
	ewf_test_libcnotify.h \
	ewf_test_libewf.h \
	ewf_test_libuna.h \
	ewf_test_macros.h \
	ewf_test_support.c \
	ewf_test_unused.h

ewf_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
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

ewf_test_write_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_unused.h \
	ewf_test_write.c

ewf_test_write_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_write_chunk_SOURCES = \
	ewf_test_getopt.c ewf_test_getopt.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_unused.h \
	ewf_test_write_chunk.c

ewf_test_write_chunk_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_write_io_handle_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h \
	ewf_test_write_io_handle.c

ewf_test_write_io_handle_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile


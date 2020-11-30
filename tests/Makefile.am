AUTOMAKE_OPTIONS = subdir-objects

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
	@ZLIB_CPPFLAGS@ \
	@BZIP2_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCAES_CPPFLAGS@ \
	@LIBODRAW_CPPFLAGS@ \
	@LIBSMDEV_CPPFLAGS@ \
	@LIBSMRAW_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBEWF_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYEWF = \
	test_python_module.sh
endif

if HAVE_WRITE_TESTS
TESTS_WRITE = \
	test_write_functions.sh \
	test_ewfexport.sh \
	test_ewfexport_logical.sh \
	test_ewfacquire.sh \
	test_ewfacquire_optical.sh \
	test_ewfacquirestream.sh \
	test_ewfacquire_resume.sh
endif

TESTS = \
	test_library.sh \
	test_glob.sh \
	test_tools.sh \
	test_ewfinfo.sh \
	test_ewfverify.sh \
	test_ewfverify_logical.sh \
	$(TESTS_WRITE) \
	$(TESTS_PYEWF)

check_SCRIPTS = \
	pyewf_test_support.py \
	test_ewfacquire.sh \
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
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh \
	test_write_functions.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	ewf_test_access_control_entry \
	ewf_test_analytical_data \
	ewf_test_attribute \
	ewf_test_bit_stream \
	ewf_test_case_data \
	ewf_test_checksum \
	ewf_test_chunk_data \
	ewf_test_chunk_descriptor \
	ewf_test_chunk_group \
	ewf_test_chunk_table \
	ewf_test_compression \
	ewf_test_data_chunk \
	ewf_test_date_time \
	ewf_test_date_time_values \
	ewf_test_deflate \
	ewf_test_device_information \
	ewf_test_digest_section \
	ewf_test_error \
	ewf_test_error2_section \
	ewf_test_file_entry \
	ewf_test_filename \
	ewf_test_glob \
	ewf_test_handle \
	ewf_test_hash_sections \
	ewf_test_hash_values \
	ewf_test_header_sections \
	ewf_test_header_values \
	ewf_test_huffman_tree \
	ewf_test_io_handle \
	ewf_test_lef_extended_attribute \
	ewf_test_lef_file_entry \
	ewf_test_lef_permission \
	ewf_test_lef_source \
	ewf_test_lef_subject \
	ewf_test_ltree_section \
	ewf_test_md5_hash_section \
	ewf_test_media_values \
	ewf_test_notify \
	ewf_test_permission_group \
	ewf_test_read_io_handle \
	ewf_test_restart_data \
	ewf_test_section_descriptor \
	ewf_test_sector_range \
	ewf_test_sector_range_list \
	ewf_test_segment_file \
	ewf_test_segment_table \
	ewf_test_serialized_string \
	ewf_test_session_section \
	ewf_test_sha1_hash_section \
	ewf_test_single_file_tree \
	ewf_test_single_files \
	ewf_test_source \
	ewf_test_support \
	ewf_test_table_section \
	ewf_test_tools_byte_size_string \
	ewf_test_tools_device_handle \
	ewf_test_tools_digest_hash \
	ewf_test_tools_export_handle \
	ewf_test_tools_guid \
	ewf_test_tools_imaging_handle \
	ewf_test_tools_info_handle \
	ewf_test_tools_log_handle \
	ewf_test_tools_output \
	ewf_test_tools_platform \
	ewf_test_tools_signal \
	ewf_test_tools_storage_media_buffer \
	ewf_test_tools_system_string \
	ewf_test_tools_verification_handle \
	ewf_test_truncate \
	ewf_test_value_table \
	ewf_test_volume_section \
	ewf_test_write \
	ewf_test_write_chunk \
	ewf_test_write_io_handle

ewf_test_access_control_entry_SOURCES = \
	ewf_test_access_control_entry.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_rwlock.c ewf_test_rwlock.h \
	ewf_test_unused.h

ewf_test_access_control_entry_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_analytical_data_SOURCES = \
	ewf_test_analytical_data.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_analytical_data_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_attribute_SOURCES = \
	ewf_test_attribute.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_rwlock.c ewf_test_rwlock.h \
	ewf_test_unused.h

ewf_test_attribute_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_bit_stream_SOURCES = \
	ewf_test_bit_stream.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_bit_stream_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_case_data_SOURCES = \
	ewf_test_case_data.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_case_data_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_checksum_SOURCES = \
	ewf_test_checksum.c \
	ewf_test_libcerror.h \
	ewf_test_libcnotify.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_checksum_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_chunk_data_SOURCES = \
	ewf_test_chunk_data.c \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_chunk_data_LDADD = \
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

ewf_test_chunk_descriptor_SOURCES = \
	ewf_test_chunk_descriptor.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_chunk_descriptor_LDADD = \
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
	ewf_test_libcdata.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_chunk_table_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_compression_SOURCES = \
	ewf_test_compression.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_compression_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_data_chunk_SOURCES = \
	ewf_test_data_chunk.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_rwlock.c ewf_test_rwlock.h \
	ewf_test_unused.h

ewf_test_data_chunk_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

ewf_test_date_time_SOURCES = \
	ewf_test_date_time.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_date_time_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_date_time_values_SOURCES = \
	ewf_test_date_time_values.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_date_time_values_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_deflate_SOURCES = \
	ewf_test_deflate.c \
	ewf_test_libcerror.h \
	ewf_test_libcnotify.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_deflate_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_device_information_SOURCES = \
	ewf_test_device_information.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_device_information_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_digest_section_SOURCES = \
	ewf_test_digest_section.c \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_digest_section_LDADD = \
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

ewf_test_error_SOURCES = \
	ewf_test_error.c \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_error_LDADD = \
	../libewf/libewf.la

ewf_test_error2_section_SOURCES = \
	ewf_test_error2_section.c \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcdata.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_error2_section_LDADD = \
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

ewf_test_file_entry_SOURCES = \
	ewf_test_file_entry.c \
	ewf_test_libcdata.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_file_entry_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_filename_SOURCES = \
	ewf_test_filename.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_filename_LDADD = \
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
	ewf_test_memory.c ewf_test_memory.h

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
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
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
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_huffman_tree_SOURCES = \
	ewf_test_huffman_tree.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_huffman_tree_LDADD = \
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

ewf_test_lef_extended_attribute_SOURCES = \
	ewf_test_lef_extended_attribute.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_lef_extended_attribute_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_lef_file_entry_SOURCES = \
	ewf_test_lef_file_entry.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_lef_file_entry_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_lef_permission_SOURCES = \
	ewf_test_lef_permission.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_lef_permission_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_lef_source_SOURCES = \
	ewf_test_lef_source.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_lef_source_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_lef_subject_SOURCES = \
	ewf_test_lef_subject.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_lef_subject_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_ltree_section_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_ltree_section.c \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_ltree_section_LDADD = \
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

ewf_test_md5_hash_section_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_md5_hash_section.c \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_md5_hash_section_LDADD = \
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
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_notify.c \
	ewf_test_unused.h

ewf_test_notify_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_permission_group_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_permission_group.c \
	ewf_test_unused.h

ewf_test_permission_group_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

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
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_restart_data.c \
	ewf_test_unused.h

ewf_test_restart_data_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_section_descriptor_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_section_descriptor.c \
	ewf_test_unused.h

ewf_test_section_descriptor_LDADD = \
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

ewf_test_sector_range_list_SOURCES = \
	ewf_test_libcdata.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_sector_range_list.c \
	ewf_test_unused.h

ewf_test_sector_range_list_LDADD = \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_segment_file_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_segment_file.c \
	ewf_test_unused.h

ewf_test_segment_file_LDADD = \
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

ewf_test_serialized_string_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_serialized_string.c \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_serialized_string_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_session_section_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcdata.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_session_section.c \
	ewf_test_unused.h

ewf_test_session_section_LDADD = \
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

ewf_test_sha1_hash_section_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_sha1_hash_section.c \
	ewf_test_unused.h

ewf_test_sha1_hash_section_LDADD = \
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

ewf_test_single_file_tree_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_single_file_tree.c \
	ewf_test_unused.h

ewf_test_single_file_tree_LDADD = \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_single_files_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_single_files.c \
	ewf_test_unused.h

ewf_test_single_files_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_source_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_rwlock.c ewf_test_rwlock.h \
	ewf_test_source.c \
	ewf_test_unused.h

ewf_test_source_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

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
	ewf_test_memory.c ewf_test_memory.h \
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

ewf_test_table_section_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_table_section.c \
	ewf_test_unused.h

ewf_test_table_section_LDADD = \
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

ewf_test_tools_byte_size_string_SOURCES = \
	../ewftools/byte_size_string.c ../ewftools/byte_size_string.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_byte_size_string.c \
	ewf_test_unused.h

ewf_test_tools_byte_size_string_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@

ewf_test_tools_device_handle_SOURCES = \
	../ewftools/byte_size_string.c ../ewftools/byte_size_string.h \
	../ewftools/device_handle.c ../ewftools/device_handle.h \
	../ewftools/ewfinput.c ../ewftools/ewfinput.h \
	../ewftools/ewftools_system_string.c ../ewftools/ewftools_system_string.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_tools_device_handle.c \
	ewf_test_unused.h

ewf_test_tools_device_handle_LDADD = \
	@LIBSMRAW_LIBADD@ \
	@LIBSMDEV_LIBADD@ \
	@LIBODRAW_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@

ewf_test_tools_digest_hash_SOURCES = \
	../ewftools/digest_hash.c ../ewftools/digest_hash.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_digest_hash.c \
	ewf_test_unused.h

ewf_test_tools_digest_hash_LDADD = \
	@LIBCERROR_LIBADD@

ewf_test_tools_export_handle_SOURCES = \
	../ewftools/byte_size_string.c ../ewftools/byte_size_string.h \
	../ewftools/digest_hash.c ../ewftools/digest_hash.h \
	../ewftools/ewfinput.c ../ewftools/ewfinput.h \
	../ewftools/ewftools_system_string.c ../ewftools/ewftools_system_string.h \
	../ewftools/export_handle.c ../ewftools/export_handle.h \
	../ewftools/guid.c ../ewftools/guid.h \
	../ewftools/log_handle.c ../ewftools/log_handle.h \
	../ewftools/process_status.c ../ewftools/process_status.h \
	../ewftools/storage_media_buffer.c ../ewftools/storage_media_buffer.h \
	../ewftools/storage_media_buffer_queue.c ../ewftools/storage_media_buffer_queue.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_tools_export_handle.c \
	ewf_test_unused.h

ewf_test_tools_export_handle_LDADD = \
	@LIBSMRAW_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
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
	@LIBUUID_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

ewf_test_tools_guid_SOURCES = \
	../ewftools/guid.c ../ewftools/guid.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_guid.c \
	ewf_test_unused.h

ewf_test_tools_guid_LDADD = \
	@LIBCERROR_LIBADD@ \
	@LIBUUID_LIBADD@

ewf_test_tools_imaging_handle_SOURCES = \
	../ewftools/byte_size_string.c ../ewftools/byte_size_string.h \
	../ewftools/digest_hash.c ../ewftools/digest_hash.h \
	../ewftools/ewfinput.c ../ewftools/ewfinput.h \
	../ewftools/ewftools_system_string.c ../ewftools/ewftools_system_string.h \
	../ewftools/guid.c ../ewftools/guid.h \
	../ewftools/imaging_handle.c ../ewftools/imaging_handle.h \
	../ewftools/platform.c ../ewftools/platform.h \
	../ewftools/process_status.c ../ewftools/process_status.h \
	../ewftools/storage_media_buffer.c ../ewftools/storage_media_buffer.h \
	../ewftools/storage_media_buffer_queue.c ../ewftools/storage_media_buffer_queue.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_tools_imaging_handle.c \
	ewf_test_unused.h

ewf_test_tools_imaging_handle_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
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
	@LIBUUID_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

ewf_test_tools_info_handle_SOURCES = \
	../ewftools/byte_size_string.c ../ewftools/byte_size_string.h \
	../ewftools/ewfinput.c ../ewftools/ewfinput.h \
	../ewftools/ewftools_system_string.c ../ewftools/ewftools_system_string.h \
	../ewftools/guid.c ../ewftools/guid.h \
	../ewftools/info_handle.c ../ewftools/info_handle.h \
	../ewftools/platform.c ../ewftools/platform.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_tools_info_handle.c \
	ewf_test_unused.h

ewf_test_tools_info_handle_LDADD = \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@ \
	@LIBUUID_LIBADD@

ewf_test_tools_log_handle_SOURCES = \
	../ewftools/log_handle.c ../ewftools/log_handle.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_tools_log_handle.c \
	ewf_test_unused.h

ewf_test_tools_log_handle_LDADD = \
	@LIBCERROR_LIBADD@

ewf_test_tools_output_SOURCES = \
	../ewftools/ewftools_output.c ../ewftools/ewftools_output.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_output.c \
	ewf_test_unused.h

ewf_test_tools_output_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_tools_platform_SOURCES = \
	../ewftools/platform.c ../ewftools/platform.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_platform.c \
	ewf_test_unused.h

ewf_test_tools_platform_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@

ewf_test_tools_signal_SOURCES = \
	../ewftools/ewftools_signal.c ../ewftools/ewftools_signal.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_signal.c \
	ewf_test_unused.h

ewf_test_tools_signal_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_tools_storage_media_buffer_SOURCES = \
	../ewftools/storage_media_buffer.c ../ewftools/storage_media_buffer.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_tools_storage_media_buffer.c \
	ewf_test_unused.h

ewf_test_tools_storage_media_buffer_LDADD = \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_tools_system_string_SOURCES = \
	../ewftools/ewftools_system_string.c ../ewftools/ewftools_system_string.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_system_string.c \
	ewf_test_unused.h

ewf_test_tools_system_string_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@

ewf_test_tools_verification_handle_SOURCES = \
	../ewftools/byte_size_string.c ../ewftools/byte_size_string.h \
	../ewftools/digest_hash.c ../ewftools/digest_hash.h \
	../ewftools/ewfinput.c ../ewftools/ewfinput.h \
	../ewftools/ewftools_system_string.c ../ewftools/ewftools_system_string.h \
	../ewftools/log_handle.c ../ewftools/log_handle.h \
	../ewftools/process_status.c ../ewftools/process_status.h \
	../ewftools/storage_media_buffer.c ../ewftools/storage_media_buffer.h \
	../ewftools/storage_media_buffer_queue.c ../ewftools/storage_media_buffer_queue.h \
	../ewftools/verification_handle.c ../ewftools/verification_handle.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_tools_verification_handle.c \
	ewf_test_unused.h

ewf_test_tools_verification_handle_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
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

ewf_test_value_table_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h \
	ewf_test_value_table.c

ewf_test_value_table_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_volume_section_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h \
	ewf_test_volume_section.c

ewf_test_volume_section_LDADD = \
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
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h \
	ewf_test_write_io_handle.c

ewf_test_write_io_handle_LDADD = \
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

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile


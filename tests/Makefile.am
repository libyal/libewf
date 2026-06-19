AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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

AM_TESTS_ENVIRONMENT = \
	export TESTS_USE_WINAPI="@TESTS_USE_WINAPI@";

BUILT_SOURCES = package.m4

TESTS = \
	test_glob.sh

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_WRITE_TESTS
check_AUTOTESTS += test_library_write
endif
if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif
if HAVE_WRITE_TESTS
check_AUTOTESTS += test_tools_write
endif

check_SCRIPTS = \
	pyewf_test_handle.py \
	pyewf_test_support.py

check_PROGRAMS = \
	ewf_test_access_control_entry \
	ewf_test_analytical_data \
	ewf_test_attribute \
	ewf_test_buffer_data_handle \
	ewf_test_bit_stream \
	ewf_test_case_data \
	ewf_test_case_data_section \
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
	ewf_test_device_information_section \
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
	ewf_test_line_reader \
	ewf_test_ltree_section \
	ewf_test_md5_hash_section \
	ewf_test_media_values \
	ewf_test_notify \
	ewf_test_permission_group \
	ewf_test_read_io_handle \
	ewf_test_restart_data \
	ewf_test_section_data_handle \
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
	ewf_test_tools_bodyfile \
	ewf_test_tools_byte_size_string \
	ewf_test_tools_device_handle \
	ewf_test_tools_digest_hash \
	ewf_test_tools_export_handle \
	ewf_test_tools_guid \
	ewf_test_tools_imaging_handle \
	ewf_test_tools_info_handle \
	ewf_test_tools_log_handle \
	ewf_test_tools_mount_path_string \
	ewf_test_tools_output \
	ewf_test_tools_path_string \
	ewf_test_tools_platform \
	ewf_test_tools_signal \
	ewf_test_tools_storage_media_buffer \
	ewf_test_tools_system_string \
	ewf_test_tools_verification_handle \
	ewf_test_truncate \
	ewf_test_value_reader \
	ewf_test_value_table \
	ewf_test_volume_section \
	ewf_test_write \
	ewf_test_write_chunk \
	ewf_test_write_io_handle

ewf_test_access_control_entry_SOURCES = \
	ewf_test_access_control_entry.c \
	ewf_test_extern.h \
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
	ewf_test_extern.h \
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

ewf_test_buffer_data_handle_SOURCES = \
	ewf_test_buffer_data_handle.c \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_buffer_data_handle_LDADD = \
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

ewf_test_case_data_section_SOURCES = \
	ewf_test_case_data_section.c \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_case_data_section_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
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
	ewf_test_extern.h \
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

ewf_test_device_information_section_SOURCES = \
	ewf_test_device_information_section.c \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_device_information_section_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
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
	ewf_test_libfdata.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_file_entry_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
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
	ewf_test_macros.h \
	ewf_test_unused.h

ewf_test_glob_LDADD = \
	../libewf/libewf.la

ewf_test_handle_SOURCES = \
	ewf_test_extern.h \
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
	ewf_test_rwlock.c ewf_test_rwlock.h

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

ewf_test_line_reader_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfdata.h \
	ewf_test_line_reader.c \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_line_reader_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_ltree_section_SOURCES = \
	ewf_test_functions.c ewf_test_functions.h \
	ewf_test_libbfio.h \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfdata.h \
	ewf_test_ltree_section.c \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h

ewf_test_ltree_section_LDADD = \
	@LIBFDATA_LIBADD@ \
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

ewf_test_section_data_handle_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_section_data_handle.c \
	ewf_test_unused.h

ewf_test_section_data_handle_LDADD = \
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
	ewf_test_libfdata.h \
	ewf_test_macros.h \
	ewf_test_single_file_tree.c \
	ewf_test_unused.h

ewf_test_single_file_tree_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_single_files_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_libfdata.h \
	ewf_test_libfvalue.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_single_files.c \
	ewf_test_unused.h

ewf_test_single_files_LDADD = \
	@LIBFVALUE_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libewf/libewf.la \
	@LIBCERROR_LIBADD@

ewf_test_source_SOURCES = \
	ewf_test_extern.h \
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

ewf_test_tools_bodyfile_SOURCES = \
	../ewftools/bodyfile.c ../ewftools/bodyfile.h \
	ewf_test_libcerror.h \
	ewf_test_libcpath.h \
	ewf_test_macros.h \
	ewf_test_tools_bodyfile.c \
	ewf_test_unused.h

ewf_test_tools_bodyfile_LDADD = \
	@LIBUNA_LIBADD@ \
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
	../ewftools/bodyfile.c ../ewftools/bodyfile.h \
	../ewftools/byte_size_string.c ../ewftools/byte_size_string.h \
	../ewftools/ewfinput.c ../ewftools/ewfinput.h \
	../ewftools/ewftools_system_string.c ../ewftools/ewftools_system_string.h \
	../ewftools/guid.c ../ewftools/guid.h \
	../ewftools/info_handle.c ../ewftools/info_handle.h \
	../ewftools/path_string.c ../ewftools/path_string.h \
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

ewf_test_tools_mount_path_string_SOURCES = \
	../ewftools/mount_path_string.c ../ewftools/mount_path_string.h \
	ewf_test_libcerror.h \
	ewf_test_libcpath.h \
	ewf_test_macros.h \
	ewf_test_tools_mount_path_string.c \
	ewf_test_unused.h

ewf_test_tools_mount_path_string_LDADD = \
	@LIBUNA_LIBADD@ \
	../libewf/libewf.la \
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
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

ewf_test_tools_path_string_SOURCES = \
	../ewftools/path_string.c ../ewftools/path_string.h \
	ewf_test_libcerror.h \
	ewf_test_macros.h \
	ewf_test_tools_path_string.c \
	ewf_test_unused.h

ewf_test_tools_path_string_LDADD = \
	@LIBUNA_LIBADD@ \
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

ewf_test_value_reader_SOURCES = \
	ewf_test_libcerror.h \
	ewf_test_libewf.h \
	ewf_test_macros.h \
	ewf_test_memory.c ewf_test_memory.h \
	ewf_test_unused.h \
	ewf_test_value_reader.c

ewf_test_value_reader_LDADD = \
	../libewf/libewf.la \
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
	@LIBCNOTIFY_LIBADD@ \
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

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	echo "m4_define([HAVE_MULTI_THREAD_SUPPORT], [@HAVE_MULTI_THREAD_SUPPORT@])" | \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp - $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libewf.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libewf

test_inputs_pyewf.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyewf

test_inputs_ewfinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfinfo

test_inputs_ewfinfo_logical.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfinfo_logical

test_inputs_ewfinfo_logical_bodyfile.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfinfo_logical_bodyfile

test_inputs_ewfexport.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfexport

test_inputs_ewfexport_logical.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfexport_logical

test_inputs_ewfexport_write.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfexport_write

test_inputs_ewfverify.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfverify

test_inputs_ewfverify_logical.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfverify_logical

test_inputs_ewfacquire.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfacquire

test_inputs_ewfacquire_optical.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfacquire_optical

test_inputs_ewfacquirestream.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) ewfacquirestream

test_library: \
	package.m4 \
	test_inputs_libewf.at \
	test_library.at \
	test_macros.at

test_library_write: \
	package.m4 \
	test_library_write.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pyewf.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_ewfexport.at \
	test_inputs_ewfexport_logical.at \
	test_inputs_ewfinfo.at \
	test_inputs_ewfinfo_logical.at \
	test_inputs_ewfinfo_logical_bodyfile.at \
	test_inputs_ewfverify.at \
	test_inputs_ewfverify_logical.at \
	test_macros.at \
	test_tools.at

test_tools_write: \
	package.m4 \
	test_inputs_ewfacquire.at \
	test_inputs_ewfacquire_optical.at \
	test_inputs_ewfacquirestream.at \
	test_inputs_ewfexport_write.at \
	test_macros.at \
	test_tools_write.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_glob.sh \
	test_macros.at \
	test_runner.sh

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in


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
	@PTHREAD_CPPFLAGS@

lib_LTLIBRARIES = libewf.la

libewf_la_SOURCES = \
	ewf_data.h \
	ewf_digest.h \
	ewf_error.h \
	ewf_file_header.h \
	ewf_hash.h \
	ewf_ltree.h \
	ewf_section.h \
	ewf_session.h \
	ewf_table.h \
	ewf_volume.h \
	libewf.c \
	libewf_analytical_data.c libewf_analytical_data.h \
	libewf_case_data.c libewf_case_data.h \
	libewf_checksum.c libewf_checksum.h \
	libewf_chunk_data.c libewf_chunk_data.h \
	libewf_chunk_group.c libewf_chunk_group.h \
	libewf_chunk_table.c libewf_chunk_table.h \
	libewf_codepage.h \
	libewf_compression.c libewf_compression.h \
	libewf_data_chunk.c libewf_data_chunk.h \
	libewf_date_time.c libewf_date_time.h \
	libewf_date_time_values.c libewf_date_time_values.h \
	libewf_debug.c libewf_debug.h \
	libewf_definitions.h \
	libewf_deflate.c libewf_deflate.h \
	libewf_device_information.c libewf_device_information.h \
	libewf_error.c libewf_error.h \
	libewf_extern.h \
	libewf_filename.c libewf_filename.h \
	libewf_file_entry.c libewf_file_entry.h \
	libewf_handle.c libewf_handle.h \
	libewf_hash_sections.c libewf_hash_sections.h \
	libewf_hash_values.c libewf_hash_values.h \
	libewf_header_sections.c libewf_header_sections.h \
	libewf_header_values.c libewf_header_values.h \
	libewf_io_handle.c libewf_io_handle.h \
	libewf_libbfio.h \
	libewf_libcaes.h \
	libewf_libcdata.h \
	libewf_libcerror.h \
	libewf_libclocale.h \
	libewf_libcnotify.h \
	libewf_libcthreads.h \
	libewf_libhmac.h \
	libewf_libfcache.h \
	libewf_libfdata.h \
	libewf_libfguid.h \
	libewf_libfvalue.h \
	libewf_libuna.h \
	libewf_media_values.c libewf_media_values.h \
	libewf_notify.c libewf_notify.h \
	libewf_read_io_handle.c libewf_read_io_handle.h \
	libewf_restart_data.c libewf_restart_data.h \
	libewf_section.c libewf_section.h \
	libewf_sector_range.c libewf_sector_range.h \
	libewf_segment_file.c libewf_segment_file.h \
	libewf_segment_table.c libewf_segment_table.h \
	libewf_single_files.c libewf_single_files.h \
	libewf_single_file_entry.c libewf_single_file_entry.h \
	libewf_single_file_tree.c libewf_single_file_tree.h \
	libewf_support.c libewf_support.h \
	libewf_types.h \
	libewf_unused.h \
	libewf_write_io_handle.c libewf_write_io_handle.h

libewf_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@ZLIB_LIBADD@ \
	@BZIP2_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCAES_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@PTHREAD_LIBADD@

libewf_la_LDFLAGS = -no-undefined -version-info 3:0:0

EXTRA_DIST = \
	libewf_definitions.h.in \
	libewf.rc \
	libewf.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libewf_definitions.h
	/bin/rm -f libewf.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libewf ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libewf_la_SOURCES)


if HAVE_LOCAL_LIBUNA
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common -I$(top_srcdir)/libuna
else
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common
endif

lib_LTLIBRARIES = libewf.la

libewf_la_SOURCES = \
	ewf_crc.h \
	ewf_data.h \
	ewf_definitions.h \
	ewf_digest_hash.h \
	ewf_error2.h \
	ewf_file_header.h \
	ewf_hash.h \
	ewf_ltree.h \
	ewf_section.h \
	ewf_session.h \
	ewf_table.h \
	ewf_volume.h \
	ewf_volume_smart.h \
	ewfx_delta_chunk.h \
	libewf.c \
	libewf_chunk_cache.c libewf_chunk_cache.h \
	libewf_chunk_offset.h \
	libewf_compression.c libewf_compression.h \
	libewf_debug.c libewf_debug.h \
	libewf_definitions.h \
	libewf_error.c libewf_error.h \
	libewf_extern.h \
	libewf_file.c libewf_file.h \
	libewf_filename.c libewf_filename.h \
	libewf_file_io.c libewf_file_io.h \
	libewf_file_io_handle.c libewf_file_io_handle.h \
	libewf_file_io_pool.c libewf_file_io_pool.h \
	libewf_handle.c libewf_handle.h \
	libewf_hash_sections.c libewf_hash_sections.h \
	libewf_hash_values.c libewf_hash_values.h \
	libewf_header_sections.c libewf_header_sections.h \
	libewf_header_values.c libewf_header_values.h \
	libewf_interface.c libewf_interface.h \
	libewf_libuna.h \
	libewf_list_type.c libewf_list_type.h \
	libewf_media_values.c libewf_media_values.h \
	libewf_offset_table.c libewf_offset_table.h \
	libewf_read.c libewf_read.h \
	libewf_section.c libewf_section.h \
	libewf_section_list.c libewf_section_list.h \
	libewf_sector_table.c libewf_sector_table.h \
	libewf_segment_file.c libewf_segment_file.h \
	libewf_segment_file_handle.c libewf_segment_file_handle.h \
	libewf_segment_table.c libewf_segment_table.h \
	libewf_string.c libewf_string.h \
	libewf_support.c libewf_support.h \
	libewf_system_string.h \
	libewf_values_table.c libewf_values_table.h \
	libewf_write.c libewf_write.h

if HAVE_LOCAL_LIBUNA
libewf_la_LIBADD  = ../common/libcommon.la ../libuna/libuna.la
else
libewf_la_LIBADD  = ../common/libcommon.la
endif
if HAVE_V2_API
libewf_la_LDFLAGS = -no-undefined -version-info 2:0:0
else
libewf_la_LDFLAGS = -no-undefined -version-info 1:4:0
endif

distclean: clean
	/bin/rm -f Makefile


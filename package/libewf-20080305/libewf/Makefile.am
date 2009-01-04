AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libewf.la

libewf_la_SOURCES = \
	ewf_char.h \
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
	ewf_string.h \
	ewf_table.h \
	ewf_volume.h \
	ewf_volume_smart.h \
	ewfx_delta_chunk.h \
	libewf.c \
	libewf_char.h \
	libewf_chunk_cache.c libewf_chunk_cache.h \
	libewf_chunk_offset.h \
	libewf_common.c libewf_common.h \
	libewf_compression.c libewf_compression.h \
	libewf_debug.c libewf_debug.h \
	libewf_endian.c libewf_endian.h \
	libewf_file.c libewf_file.h \
	libewf_filename.c libewf_filename.h \
	libewf_file_io_handle.c libewf_file_io_handle.h \
	libewf_file_io_pool.c libewf_file_io_pool.h \
	libewf_hash_sections.c libewf_hash_sections.h \
	libewf_hash_values.c libewf_hash_values.h \
	libewf_header_sections.c libewf_header_sections.h \
	libewf_header_values.c libewf_header_values.h \
	libewf_includes.h \
	libewf_internal_handle.c libewf_internal_handle.h \
	libewf_media_values.c libewf_media_values.h \
	libewf_notify.c libewf_notify.h \
	libewf_offset_table.c libewf_offset_table.h \
	libewf_read.c libewf_read.h \
	libewf_section.c libewf_section.h \
	libewf_section_list.c libewf_section_list.h \
	libewf_sector_table.c libewf_sector_table.h \
	libewf_segment_file.c libewf_segment_file.h \
	libewf_segment_file_handle.c libewf_segment_file_handle.h \
	libewf_segment_table.c libewf_segment_table.h \
	libewf_string.c libewf_string.h \
	libewf_values_table.c libewf_values_table.h \
	libewf_write.c libewf_write.h

libewf_la_LDFLAGS = -no-undefined -version-info 1:2:0

distclean: clean
	/bin/rm -f Makefile


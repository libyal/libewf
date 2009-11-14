AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libewf.la

libewf_la_SOURCES = \
	ewf_crc.h \
	ewf_data.h \
	ewf_definitions.h \
	ewf_digest.h \
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
	libewf_codepage.h \
	libewf_compression.c libewf_compression.h \
	libewf_date_time.c libewf_date_time.h \
	libewf_date_time_values.c libewf_date_time_values.h \
	libewf_debug.c libewf_debug.h \
	libewf_definitions.h \
	libewf_empty_block.c libewf_empty_block.h \
	libewf_error.c libewf_error.h \
	libewf_extern.h \
	libewf_filename.c libewf_filename.h \
	libewf_handle.c libewf_handle.h \
	libewf_hash_sections.c libewf_hash_sections.h \
	libewf_hash_values.c libewf_hash_values.h \
	libewf_header_sections.c libewf_header_sections.h \
	libewf_header_values.c libewf_header_values.h \
	libewf_io_handle.c libewf_io_handle.h \
	libewf_legacy.c libewf_legacy.h \
	libewf_libbfio.h \
	libewf_libuna.h \
	libewf_list_type.c libewf_list_type.h \
	libewf_media_values.c libewf_media_values.h \
	libewf_metadata.c libewf_metadata.h \
	libewf_notify.c libewf_notify.h \
	libewf_offset_table.c libewf_offset_table.h \
	libewf_read_io_handle.c libewf_read_io_handle.h \
	libewf_section.c libewf_section.h \
	libewf_section_list.c libewf_section_list.h \
	libewf_sector_table.c libewf_sector_table.h \
	libewf_segment_file.c libewf_segment_file.h \
	libewf_segment_file_handle.c libewf_segment_file_handle.h \
	libewf_segment_table.c libewf_segment_table.h \
	libewf_string.c libewf_string.h \
	libewf_support.c libewf_support.h \
	libewf_system_string.h \
	libewf_types.h \
	libewf_values_table.c libewf_values_table.h \
	libewf_write_io_handle.c libewf_write_io_handle.h

libewf_la_LIBADD = \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@ \
	@LIBBFIO_LIBADD@

if HAVE_V2_API
libewf_la_LDFLAGS = -no-undefined -version-info 2:0:0
else
libewf_la_LDFLAGS = -no-undefined -version-info 1:4:0
endif

EXTRA_DIST = \
	libewf_definitions.h.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libewf ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libewf_la_SOURCES) 


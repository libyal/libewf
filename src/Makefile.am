lib_LIBRARIES = libewf.a
bin_PROGRAMS = ewfacquire ewfexport ewfinfo ewfverify

libewf_a_SOURCES = \
	definitions.h \
	libewf_common.c libewf_common.h \
	libewf_notify.c libewf_notify.h \
	libewf_endian.c libewf_endian.h \
	ewf_compress.c ewf_compress.h \
	ewf_crc.c ewf_crc.h \
	ewf_data.c ewf_data.h \
	ewf_error2.c ewf_error2.h \
	ewf_file_header.c ewf_file_header.h \
	ewf_hash.c ewf_hash.h \
	ewf_header.c ewf_header.h \
	ewf_header2.c ewf_header2.h \
	ewf_ltree.c ewf_ltree.h \
	ewf_md5hash.c ewf_md5hash.h \
	ewf_section.c ewf_section.h \
	ewf_sectors.c ewf_sectors.h \
	ewf_table.c ewf_table.h \
	ewf_volume.c ewf_volume.h \
	ewf_volume_smart.c ewf_volume_smart.h \
	handle.c handle.h \
	header_values.c header_values.h \
	file.c file.h \
	file_read.c file_read.h \
	file_write.c file_write.h \
	md5.h \
	offset_table.c offset_table.h \
	section_list.c section_list.h \
	segment_table.c segment_table.h

#libewf_a_LDADD = -lz -lcrypto

ewfacquire_SOURCES = \
	libewf.h \
	ewfacquire.c

ewfacquire_CFLAGS = -Wall @LIBEWF_PLATFORM@
ewfacquire_LDADD = -lz -lcrypto libewf.a

ewfexport_SOURCES = \
	libewf.h \
	ewfexport.c

ewfexport_CFLAGS = -Wall
ewfexport_LDADD = -lz -lcrypto libewf.a

ewfinfo_SOURCES = \
	libewf.h \
	ewfinfo.c

ewfinfo_CFLAGS = -Wall
ewfinfo_LDADD = -lz -lcrypto libewf.a

ewfverify_SOURCES = \
	libewf.h \
	ewfverify.c

ewfverify_CFLAGS = -Wall
ewfverify_LDADD = -lz -lcrypto libewf.a


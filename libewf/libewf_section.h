/*
 * libewf section
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined( _LIBEWF_SECTION_H )
#define _LIBEWF_SECTION_H

#include "libewf_includes.h"
#include "libewf_error_sector.h"
#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"

#include "ewf_char.h"
#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_digest_hash.h"
#include "ewf_section.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

ssize_t libewf_section_start_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_SECTION *section, uint8_t error_tollerance );
ssize_t libewf_section_start_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_CHAR *section_type, size_t section_type_length, size64_t section_data_size );

ssize_t libewf_section_compressed_string_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t compressed_string_size, EWF_CHAR **uncompressed_string, size_t *uncompressed_string_size );
ssize_t libewf_section_write_compressed_string( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_CHAR *section_type, size_t section_type_length, EWF_CHAR *uncompressed_string, size_t uncompressed_string_size, int8_t compression_level );

ssize_t libewf_section_header_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, EWF_CHAR **cached_header, size_t *cached_header_size );
ssize_t libewf_section_header_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_CHAR *header, size_t header_size, int8_t compression_level );

ssize_t libewf_section_header2_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, EWF_CHAR **cached_header2, size_t *cached_header2_size );
ssize_t libewf_section_header2_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_CHAR *header2, size_t header2_size, int8_t compression_level );

ssize_t libewf_section_volume_s01_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_MEDIA_VALUES *media_values, uint8_t *format, uint8_t error_tollerance );
ssize_t libewf_section_volume_s01_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_MEDIA_VALUES *media_values, uint8_t format, uint8_t no_section_append );
ssize_t libewf_section_volume_e01_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_MEDIA_VALUES *media_values, int8_t *compression_level, uint8_t error_tollerance );
ssize_t libewf_section_volume_e01_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_MEDIA_VALUES *media_values, int8_t compression_level, uint8_t format, uint8_t no_section_append );
ssize_t libewf_section_volume_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, LIBEWF_MEDIA_VALUES *media_values, int8_t *compression_level, uint8_t *format, uint8_t *ewf_format, uint8_t error_tollerance );

ssize_t libewf_section_table_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, uint32_t media_amount_of_chunks, LIBEWF_OFFSET_TABLE *offset_table, uint8_t format, uint8_t ewf_format, uint8_t error_tollerance );
ssize_t libewf_section_table_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, off64_t base_offset, LIBEWF_OFFSET_TABLE *offset_table, uint32_t offset_table_index, uint32_t amount_of_offsets, EWF_CHAR *section_type, size_t section_type_length, size_t additional_size, uint8_t format, uint8_t ewf_format, uint8_t no_section_append );

ssize64_t libewf_section_sectors_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size64_t section_size, uint8_t ewf_format, uint8_t error_tollerance );
ssize_t libewf_section_sectors_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size64_t sectors_data_size, uint8_t no_section_append );

ssize_t libewf_section_ltree_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, uint8_t *ewf_format, uint8_t error_tollerance );

ssize_t libewf_section_session_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, uint8_t ewf_format, uint8_t error_tollerance );

ssize_t libewf_section_data_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, LIBEWF_MEDIA_VALUES *media_values, uint8_t ewf_format, uint8_t error_tollerance );
ssize_t libewf_section_data_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_MEDIA_VALUES *media_values, int8_t compression_level, uint8_t format, EWF_DATA **cached_data_section, uint8_t no_section_append );

ssize_t libewf_section_error2_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_SECTOR_TABLE *acquiry_errors, uint8_t ewf_format, uint8_t error_tollerance );
ssize_t libewf_section_error2_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_SECTOR_TABLE *acquiry_errors );

ssize_t libewf_section_hash_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_DIGEST_HASH *md5_hash, uint8_t error_tollerance );
ssize_t libewf_section_hash_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_DIGEST_HASH *md5_hash );

ssize_t libewf_section_last_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_CHAR *section_type, size_t section_type_length, uint8_t format, uint8_t ewf_format );

ssize_t libewf_section_xheader_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, EWF_CHAR **cached_xheader, size_t *cached_xheader_size );
ssize_t libewf_section_xheader_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_CHAR *xheader, size_t xheader_size, int8_t compression_level );

ssize_t libewf_section_xhash_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, EWF_CHAR **cached_xhash, size_t *cached_xhash_size );
ssize_t libewf_section_xhash_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, EWF_CHAR *xhash, size_t xhash_size, int8_t compression_level );

ssize_t libewf_section_delta_chunk_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size_t section_size, LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_OFFSET_TABLE *secondary_offset_table, uint8_t error_tollerance );
ssize_t libewf_section_delta_chunk_write( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint32_t chunk, EWF_CHAR *chunk_data, size_t chunk_size, EWF_CRC *chunk_crc, uint8_t write_crc, uint8_t no_section_append );

#if defined( HAVE_DEBUG_OUTPUT )
ssize_t libewf_section_debug_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, size64_t section_size );
#endif

int libewf_section_read( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_HEADER_SECTIONS *header_sections, LIBEWF_HASH_SECTIONS *hash_sections, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_OFFSET_TABLE *secondary_offset_table, LIBEWF_SECTOR_TABLE *acquiry_errors, int8_t *compression_level, uint8_t *format, uint8_t *ewf_format, size64_t *segment_file_size, EWF_SECTION *section, off64_t *section_start_offset, uint8_t error_tollerance );

#if defined( __cplusplus )
}
#endif

#endif


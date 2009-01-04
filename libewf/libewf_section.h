/*
 * libewf section
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

ssize_t libewf_section_start_read(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_section_t *section,
         uint8_t error_tollerance );

ssize_t libewf_section_start_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *section_type,
         size_t section_type_length,
         size64_t section_data_size );

ssize_t libewf_section_compressed_string_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t compressed_string_size,
         ewf_char_t **uncompressed_string,
         size_t *uncompressed_string_size );

ssize_t libewf_section_write_compressed_string(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *section_type,
         size_t section_type_length,
         ewf_char_t *uncompressed_string,
         size_t uncompressed_string_size,
         int8_t compression_level );

ssize_t libewf_section_header_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_header,
         size_t *cached_header_size );

ssize_t libewf_section_header_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *header,
         size_t header_size,
         int8_t compression_level );

ssize_t libewf_section_header2_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_header2,
         size_t *cached_header2_size );

ssize_t libewf_section_header2_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *header2,
         size_t header2_size,
         int8_t compression_level );

ssize_t libewf_section_volume_s01_read(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         uint8_t *format,
         uint8_t error_tollerance );

ssize_t libewf_section_volume_s01_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         uint8_t format,
         uint8_t no_section_append );

ssize_t libewf_section_volume_e01_read(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t *compression_level,
         uint8_t error_tollerance );

ssize_t libewf_section_volume_e01_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t format,
         uint8_t no_section_append );

ssize_t libewf_section_volume_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_media_values_t *media_values,
         int8_t *compression_level,
         uint8_t *format,
         uint8_t *ewf_format,
         uint8_t error_tollerance );

ssize_t libewf_section_table_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint32_t media_amount_of_chunks,
         libewf_offset_table_t *offset_table,
         uint8_t format,
         uint8_t ewf_format,
         uint8_t error_tollerance );

ssize_t libewf_section_table_write(
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t base_offset,
         libewf_offset_table_t *offset_table,
         uint32_t offset_table_index,
         uint32_t amount_of_offsets,
         ewf_char_t *section_type,
         size_t section_type_length,
         size_t additional_size,
         uint8_t format,
         uint8_t ewf_format,
         uint8_t no_section_append );

ssize64_t libewf_section_sectors_read(
           libewf_segment_file_handle_t *segment_file_handle,
           size64_t section_size,
           uint8_t ewf_format,
           uint8_t error_tollerance );

ssize_t libewf_section_sectors_write(
         libewf_segment_file_handle_t *segment_file_handle,
         size64_t sectors_data_size,
         uint8_t no_section_append );

ssize_t libewf_section_ltree_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         uint8_t *ewf_format,
         uint8_t error_tollerance );

ssize_t libewf_section_session_read(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *sessions,
         size_t section_size,
         uint8_t ewf_format,
         uint8_t error_tollerance );

ssize_t libewf_section_session_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *sessions );

ssize_t libewf_section_data_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_media_values_t *media_values,
         uint8_t ewf_format,
         uint8_t error_tollerance );

ssize_t libewf_section_data_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_media_values_t *media_values,
         int8_t compression_level,
         uint8_t format,
         ewf_data_t **cached_data_section,
         uint8_t no_section_append );

ssize_t libewf_section_error2_read(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *acquiry_errors,
         size_t section_size,
         uint8_t ewf_format,
         uint8_t error_tollerance );

ssize_t libewf_section_error2_write(
         libewf_segment_file_handle_t *segment_file_handle,
         libewf_sector_table_t *acquiry_errors );

ssize_t libewf_section_hash_read(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_digest_hash_t *md5_hash,
         uint8_t error_tollerance );

ssize_t libewf_section_hash_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_digest_hash_t *md5_hash );

ssize_t libewf_section_last_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *section_type,
         size_t section_type_length,
         uint8_t format,
         uint8_t ewf_format );

ssize_t libewf_section_xheader_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_xheader,
         size_t *cached_xheader_size );

ssize_t libewf_section_xheader_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *xheader,
         size_t xheader_size,
         int8_t compression_level );

ssize_t libewf_section_xhash_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         ewf_char_t **cached_xhash,
         size_t *cached_xhash_size );

ssize_t libewf_section_xhash_write(
         libewf_segment_file_handle_t *segment_file_handle,
         ewf_char_t *xhash,
         size_t xhash_size,
         int8_t compression_level );

ssize_t libewf_section_delta_chunk_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size_t section_size,
         libewf_offset_table_t *offset_table,
         libewf_offset_table_t *secondary_offset_table,
         uint8_t error_tollerance );

ssize_t libewf_section_delta_chunk_write(
         libewf_segment_file_handle_t *segment_file_handle,
         uint32_t chunk,
         ewf_char_t *chunk_data,
         size_t chunk_size,
         ewf_crc_t *chunk_crc,
         uint8_t write_crc,
         uint8_t no_section_append );

#if defined( HAVE_DEBUG_OUTPUT )
ssize_t libewf_section_debug_read(
         libewf_segment_file_handle_t *segment_file_handle,
         size64_t section_size );
#endif

int libewf_section_read(
     libewf_segment_file_handle_t *segment_file_handle,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_offset_table_t *secondary_offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     ewf_section_t *section,
     off64_t *section_start_offset,
     uint8_t error_tollerance );

#if defined( __cplusplus )
}
#endif

#endif


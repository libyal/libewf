/*
 * libewf segment file
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

#if !defined( _LIBEWF_SEGMENT_FILE_H )
#define _LIBEWF_SEGMENT_FILE_H

#include "libewf_includes.h"

#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"

#include "ewf_crc.h"
#include "ewf_data.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_segment_file_check_file_signature( int file_descriptor );

ssize_t libewf_segment_file_read_file_header( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint16_t *segment_number, uint8_t *segment_file_type );

int libewf_segment_file_read_sections( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, int *last_segment_file, LIBEWF_HEADER_SECTIONS *header_sections, LIBEWF_HASH_SECTIONS *hash_sections, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_OFFSET_TABLE *secondary_offset_table, libewf_sector_table_t *acquiry_errors, int8_t *compression_level, uint8_t *format, uint8_t *ewf_format, size64_t *segment_file_size, uint8_t error_tollerance );

ssize_t libewf_segment_file_write_headers( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_HEADER_SECTIONS *header_sections, int8_t compression_level, uint8_t format );
ssize_t libewf_segment_file_write_last_section( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, int last_segment_file, uint8_t format, uint8_t ewf_format );

ssize_t libewf_segment_file_write_start( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint16_t segment_number, uint8_t segment_file_type, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_HEADER_SECTIONS *header_sections, int8_t compression_level, uint8_t format, uint8_t ewf_format, EWF_DATA **cached_data_section );

ssize_t libewf_segment_file_write_chunks_section_start( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_OFFSET_TABLE *offset_table, size32_t chunk_size, uint32_t total_chunk_amount, uint32_t segment_chunk_amount, uint8_t format, uint8_t ewf_format );

ssize_t libewf_segment_file_write_chunks_data( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_OFFSET_TABLE *offset_table, uint32_t chunk, EWF_CHAR *chunk_data, size_t size, int8_t is_compressed, EWF_CRC *chunk_crc, int8_t write_crc );

ssize_t libewf_segment_file_write_chunks_correction( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, LIBEWF_OFFSET_TABLE *offset_table, off64_t chunks_section_offset, size64_t chunks_section_size, uint32_t amount_of_chunks, uint32_t section_amount_of_chunks, uint8_t format, uint8_t ewf_format );

ssize_t libewf_segment_file_write_delta_chunk( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint32_t chunk, EWF_CHAR *chunk_data, size_t chunk_size, EWF_CRC *chunk_crc, uint8_t write_crc );

ssize_t libewf_segment_file_write_close( LIBEWF_SEGMENT_FILE_HANDLE *segment_file_handle, uint16_t segment_number, uint32_t segment_amount_of_chunks, int last_segment_file, LIBEWF_HASH_SECTIONS *hash_sections, LIBEWF_VALUES_TABLE *hash_values, LIBEWF_MEDIA_VALUES *media_values, libewf_sector_table_t *acquiry_errors, int8_t compression_level, uint8_t format, uint8_t ewf_format, EWF_DATA **cached_data_section );

#if defined( __cplusplus )
}
#endif

#endif


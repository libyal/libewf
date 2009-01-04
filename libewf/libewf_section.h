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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#ifndef _LIBEWF_SECTION_H
#define _LIBEWF_SECTION_H

#include "libewf_includes.h"
#include "libewf_error_sector.h"
#include "libewf_internal_handle.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"

#include "ewf_char.h"
#include "ewf_digest_hash.h"
#include "ewf_header.h"
#include "ewf_header2.h"
#include "ewf_section.h"
#include "ewf_table.h"

#ifdef __cplusplus
extern "C" {
#endif

EWF_SECTION *libewf_section_start_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor );
ssize_t libewf_section_start_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, EWF_CHAR *section_type, size_t section_data_size, off_t start_offset );

ssize_t libewf_section_compressed_string_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_CHAR *section_type, EWF_CHAR *uncompressed_string, size_t size, int8_t compression_level );

ssize_t libewf_section_header_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_header_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_HEADER *header, size_t size, int8_t compression_level );

ssize_t libewf_section_header2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_header2_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_HEADER2 *header2, size_t size, int8_t compression_level );

ssize_t libewf_section_volume_s01_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_volume_s01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset );
ssize_t libewf_section_volume_e01_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_volume_e01_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset );
ssize_t libewf_section_volume_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );

LIBEWF_OFFSET_TABLE *libewf_fill_offset_table( LIBEWF_OFFSET_TABLE *offset_table, EWF_TABLE_OFFSET *offsets, uint32_t amount_of_chunks, int file_descriptor, uint16_t segment_number, uint8_t error_tollerance );
int8_t libewf_calculate_last_offset( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, int file_descriptor, uint16_t segment_number, uint8_t error_tollerance );
LIBEWF_OFFSET_TABLE *libewf_offset_table_read( LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_SECTION_LIST *section_list, uint32_t *amount_of_chunks, int file_descriptor, uint16_t segment_number, size_t size, uint8_t ewf_format, uint8_t error_tollerance );
uint8_t libewf_compare_offset_tables( LIBEWF_OFFSET_TABLE *offset_table1, LIBEWF_OFFSET_TABLE *offset_table2 );
ssize_t libewf_section_table_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number );
ssize_t libewf_section_table_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_OFFSET_TABLE *offset_table, uint32_t offset_table_index, uint32_t amount_of_offsets, EWF_CHAR *section_header, size_t additional_size );

ssize_t libewf_section_table2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number );

ssize_t libewf_section_sectors_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );

ssize_t libewf_section_ltree_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );

ssize_t libewf_section_data_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_data_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset );

ssize_t libewf_section_error2_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_error2_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, LIBEWF_ERROR_SECTOR *sectors, uint32_t amount_of_errors );

ssize_t libewf_section_hash_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_hash_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_DIGEST_HASH *md5_hash );

ssize_t libewf_section_last_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, EWF_CHAR *section_type, off_t start_offset );

ssize_t libewf_section_xheader_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
ssize_t libewf_section_xheader_write( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, off_t start_offset, EWF_CHAR *xheader, size_t size, int8_t compression_level );

ssize_t libewf_section_xhash_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, size_t size );
#define libewf_section_xhash_write( handle, file_descriptor, start_offset, xhash, size, compression_level ) \
	libewf_section_compressed_string_write( handle, file_descriptor, start_offset, (EWF_CHAR *) "xhash", xhash, size, compression_level )

EWF_SECTION *libewf_section_read( LIBEWF_INTERNAL_HANDLE *internal_handle, int file_descriptor, LIBEWF_SECTION_LIST *section_list, uint16_t segment_number, off_t *section_start_offset );

#ifdef __cplusplus
}
#endif

#endif


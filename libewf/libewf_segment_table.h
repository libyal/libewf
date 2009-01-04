/*
 * libewf segment table
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

#if !defined( _LIBEWF_SEGMENT_TABLE_H )
#define _LIBEWF_SEGMENT_TABLE_H

#include "libewf_includes.h"

#include "libewf_filename.h"
#include "libewf_hash_sections.h"
#include "libewf_header_sections.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"
#include "libewf_sector_table.h"
#include "libewf_segment_file_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_SEGMENT_TABLE		libewf_segment_table_t
#define LIBEWF_SEGMENT_TABLE_SIZE	sizeof( LIBEWF_SEGMENT_TABLE )

typedef struct libewf_segment_table libewf_segment_table_t;

struct libewf_segment_table
{
	/* The amount of segments in the table
	 */
	uint16_t amount;

	/* A dynamic array containting references to segment file handles
	 */
	LIBEWF_SEGMENT_FILE_HANDLE **segment_file_handle;
};

LIBEWF_SEGMENT_TABLE *libewf_segment_table_alloc( uint16_t amount );
int libewf_segment_table_realloc( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t amount );
void libewf_segment_table_free( LIBEWF_SEGMENT_TABLE *segment_table );

int libewf_segment_table_build( LIBEWF_SEGMENT_TABLE *segment_table, LIBEWF_HEADER_SECTIONS *header_sections, LIBEWF_HASH_SECTIONS *hash_sections, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_OFFSET_TABLE *secondary_offset_table, libewf_sector_table_t *acquiry_errors, int8_t *compression_level, uint8_t *format, uint8_t *ewf_format, size64_t *segment_file_size, uint8_t error_tollerance );

int libewf_segment_table_read_open( LIBEWF_SEGMENT_TABLE *segment_table, LIBEWF_SEGMENT_TABLE *delta_segment_table, LIBEWF_FILENAME * const filenames[], uint16_t file_amount, uint8_t flags, LIBEWF_HEADER_SECTIONS *header_sections, LIBEWF_HASH_SECTIONS *hash_sections, LIBEWF_MEDIA_VALUES *media_values, LIBEWF_OFFSET_TABLE *offset_table, LIBEWF_OFFSET_TABLE *secondary_offset_table, libewf_sector_table_t *acquiry_errors, int8_t *compression_level, uint8_t *format, uint8_t *ewf_format, size64_t *segment_file_size, uint8_t error_tollerance );

int libewf_segment_table_write_open( LIBEWF_SEGMENT_TABLE *segment_table, LIBEWF_FILENAME * const filenames[], uint16_t file_amount );

int libewf_segment_table_close_all( LIBEWF_SEGMENT_TABLE *segment_table );

int libewf_segment_table_create_segment_file( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment_number, int16_t maximum_amount_of_segments, uint8_t segment_file_type, uint8_t format, uint8_t ewf_format );

#if defined( __cplusplus )
}
#endif

#endif


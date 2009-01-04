/*
 * libewf handle
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

#if !defined( _LIBEWF_INTERNAL_HANDLE_H )
#define _LIBEWF_INTERNAL_HANDLE_H

#include "libewf_includes.h"
#include "libewf_char.h"
#include "libewf_chunk_cache.h"
#include "libewf_error_sector.h"
#include "libewf_header_sections.h"
#include "libewf_media_values.h"
#include "libewf_offset_table.h"
#include "libewf_segment_table.h"
#include "libewf_values_table.h"

#include "ewf_char.h"
#include "ewf_data.h"
#include "ewf_digest_hash.h"
#include "ewf_error2.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_INTERNAL_HANDLE libewf_internal_handle_t
#define LIBEWF_INTERNAL_HANDLE_SIZE sizeof( LIBEWF_INTERNAL_HANDLE )

#define LIBEWF_INTERNAL_HANDLE_READ libewf_internal_handle_read_t
#define LIBEWF_INTERNAL_HANDLE_READ_SIZE sizeof( LIBEWF_INTERNAL_HANDLE_READ )

#define LIBEWF_INTERNAL_HANDLE_WRITE libewf_internal_handle_write_t
#define LIBEWF_INTERNAL_HANDLE_WRITE_SIZE sizeof( LIBEWF_INTERNAL_HANDLE_WRITE )

typedef struct libewf_internal_handle libewf_internal_handle_t;
typedef struct libewf_internal_handle_read libewf_internal_handle_read_t;
typedef struct libewf_internal_handle_write libewf_internal_handle_write_t;

struct libewf_internal_handle
{
	/* A specific subhandle for read specific values
	 */
	LIBEWF_INTERNAL_HANDLE_READ *read;

	/* A specific subhandle for write specific values
	 */
	LIBEWF_INTERNAL_HANDLE_WRITE *write;

	/* The media values
	 */
	LIBEWF_MEDIA_VALUES *media_values;

	/* The list of segment files
	 */
	LIBEWF_SEGMENT_TABLE *segment_table;

	/* The list of delta segment files
	 */
	LIBEWF_SEGMENT_TABLE *delta_segment_table;

	/* The list of offsets within the segment files within the table sections
	 */
	LIBEWF_OFFSET_TABLE *offset_table;

	/* The list of offsets within the segment files within the table2 sections
	 */
	LIBEWF_OFFSET_TABLE *secondary_offset_table;

	/* A simple cache is implemented here to avoid having to read and decompress the
	 * same chunk while reading the data.
	 */
	LIBEWF_CHUNK_CACHE *chunk_cache;

	/* The stored header sections
	 */
	LIBEWF_HEADER_SECTIONS *header_sections;

	/* The stored xhash
	 */
	EWF_CHAR *xhash;

	/* The size of the stored xhash
	 */
	size_t xhash_size;

	/* The header values
	 */
	LIBEWF_VALUES_TABLE *header_values;

	/* The hash values
	 */
	LIBEWF_VALUES_TABLE *hash_values;

	/* The MD5 hash of the data
	 */
	EWF_DIGEST_HASH md5_hash[ EWF_DIGEST_HASH_SIZE_MD5 ];

	/* The sectors with acquiry read errors
	 */
	LIBEWF_ERROR_SECTOR *acquiry_error_sectors;

	/* The amount of the acquiry read error sectors
	 */
	uint32_t amount_of_acquiry_errors;

	/* The current chunk
	 */
	uint32_t current_chunk;

	/* The current chunk offset
	 */
	uint32_t current_chunk_offset;

	/* value to indicate the compression level used
	 */
	int8_t compression_level;

	/* value to indicate if the MD5 hash was set
	 */
	int8_t md5_hash_set;

	/* value to indicate which file format is used
	 */
	uint8_t format;

	/* value to indicate which ewf format is used
	 */
	uint8_t ewf_format;

	/* value to indicate the level of error tollerance
	 */
	uint8_t error_tollerance;
};

/* Additional subhandle for read specific parameters
 */
struct libewf_internal_handle_read
{
	/* The sectors with CRC errors
	 */
	LIBEWF_ERROR_SECTOR *crc_error_sectors;

	/* The amount of the CRC error sectors
	 */
	uint32_t crc_amount_of_errors;

	/* Value to indicate the write values were initialized
	 */
	uint8_t values_initialized;

	/* A value to indicate if a chunk should be wiped on error
	 */
	uint8_t wipe_on_error;
};

/* Additional subhandle for write specific parameters
 */
struct libewf_internal_handle_write
{
	/* A cached version of the data section
	 */
	EWF_DATA *data_section;

	/* The amount of bytes of the input written
	 */
	ssize64_t input_write_count;

	/* The total amount of bytes written
	 */
	ssize64_t write_count;

	/* The amount of bytes to write
	 */
	size64_t input_write_size;

	/* The maximum segment file size
	 */
	size64_t maximum_segment_file_size;

	/* The segment file size
	 */
	size64_t segment_file_size;

	/* The maximum amount of segments
	 */
	int16_t maximum_amount_of_segments;

	/* The amount of bytes written to a section containing chunks
	 */
	ssize64_t chunks_section_write_count;

        /* The amount of chunks written
         */
        uint32_t amount_of_chunks;

        /* The determined (estimated) amount of chunks per segment
         */
        int64_t chunks_per_segment;

        /* The determined (estimated) amount of chunks per chunks section
         */
        uint32_t chunks_per_chunks_section;

        /* The amount of chunks written of the current segment file
         */
        uint32_t segment_amount_of_chunks;

        /* The maximum amount of chunks that can be written to a chunks section
         */
        uint32_t maximum_section_amount_of_chunks;

        /* The amount of chunks written of the current chunks section
         */
        uint32_t section_amount_of_chunks;

	/* The offset of the chunks section within the current segment file
	 */
	off64_t chunks_section_offset;

	/* The current chunks section number
	 */
	uint8_t chunks_section_number;

	/* Value to indicate if empty block should be compressed
	 * even if no compression is used
	 */
	uint8_t compress_empty_block;

	/* Value to indicate if the offset table should not be restricted
	 * to the maximum amount of offsets
	 */
	uint8_t unrestrict_offset_amount;

	/* Value to indicate the write values were initialized
	 */
	uint8_t values_initialized;

	/* Value to indicate a new chunks section should be created
	 */
	uint8_t create_chunks_section;

	/* Value to indicate if the write has been finalized
	 */
	uint8_t write_finalized;
};

LIBEWF_INTERNAL_HANDLE *libewf_internal_handle_alloc( uint8_t flags );
void libewf_internal_handle_free( LIBEWF_INTERNAL_HANDLE *internal_handle );

LIBEWF_INTERNAL_HANDLE_READ *libewf_internal_handle_read_alloc( void );
void libewf_internal_handle_read_free( LIBEWF_INTERNAL_HANDLE_READ *handle_read );

LIBEWF_INTERNAL_HANDLE_WRITE *libewf_internal_handle_write_alloc( void );
void libewf_internal_handle_write_free( LIBEWF_INTERNAL_HANDLE_WRITE *handle_write );

int16_t libewf_internal_handle_get_write_maximum_amount_of_segments( LIBEWF_INTERNAL_HANDLE *internal_handle );

int libewf_internal_handle_determine_format( LIBEWF_INTERNAL_HANDLE *internal_handle );

int libewf_internal_handle_create_header_values( LIBEWF_INTERNAL_HANDLE *internal_handle );
int libewf_internal_handle_create_headers( LIBEWF_HEADER_SECTIONS *header_sections, LIBEWF_VALUES_TABLE *header_values, int8_t compression_level, uint8_t format );

int libewf_internal_handle_read_initialize( LIBEWF_INTERNAL_HANDLE *internal_handle );
int libewf_internal_handle_write_initialize( LIBEWF_INTERNAL_HANDLE *internal_handle );

#if defined( __cplusplus )
}
#endif

#endif


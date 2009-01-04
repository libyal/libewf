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

#ifndef _LIBEWF_INTERNAL_HANDLE_H
#define _LIBEWF_INTERNAL_HANDLE_H

#include "libewf_includes.h"
#include "libewf_char.h"
#include "libewf_chunk_cache.h"
#include "libewf_error_sector.h"
#include "libewf_hash_values.h"
#include "libewf_header_values.h"
#include "libewf_md5.h"
#include "libewf_offset_table.h"
#include "libewf_segment_table.h"

#include "ewf_digest_hash.h"
#include "ewf_error2.h"
#include "ewf_header.h"
#include "ewf_header2.h"
#include "ewf_table.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBEWF_INTERNAL_HANDLE libewf_internal_handle_t
#define LIBEWF_INTERNAL_HANDLE_SIZE sizeof( LIBEWF_INTERNAL_HANDLE )

#define LIBEWF_INTERNAL_HANDLE_MEDIA libewf_internal_handle_media_t
#define LIBEWF_INTERNAL_HANDLE_MEDIA_SIZE sizeof( LIBEWF_INTERNAL_HANDLE_MEDIA )

#define LIBEWF_INTERNAL_HANDLE_READ libewf_internal_handle_read_t
#define LIBEWF_INTERNAL_HANDLE_READ_SIZE sizeof( LIBEWF_INTERNAL_HANDLE_READ )

#define LIBEWF_INTERNAL_HANDLE_WRITE libewf_internal_handle_write_t
#define LIBEWF_INTERNAL_HANDLE_WRITE_SIZE sizeof( LIBEWF_INTERNAL_HANDLE_WRITE )

typedef struct libewf_internal_handle libewf_internal_handle_t;
typedef struct libewf_internal_handle_media libewf_internal_handle_media_t;
typedef struct libewf_internal_handle_read libewf_internal_handle_read_t;
typedef struct libewf_internal_handle_write libewf_internal_handle_write_t;

struct libewf_internal_handle
{
	/* A specific subhandle for media specific values
	 */
	libewf_internal_handle_media_t *media;

	/* A specific subhandle for read specific values
	 */
	libewf_internal_handle_read_t *read;

	/* A specific subhandle for write specific values
	 */
	libewf_internal_handle_write_t *write;

	/* The list of segment files
	 */
	LIBEWF_SEGMENT_TABLE *segment_table;

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

	/* The stored header
	 */
	EWF_HEADER *header;

	/* The size of the stored header
	 */
	size_t header_size;

	/* The stored header2
	 */
	EWF_HEADER2 *header2;

	/* The size of the stored header2
	 */
	size_t header2_size;

	/* The stored xheader
	 */
	EWF_HEADER *xheader;

	/* The size of the stored xheader
	 */
	size_t xheader_size;

	/* The stored xhash
	 */
	EWF_HEADER *xhash;

	/* The size of the stored xhash
	 */
	size_t xhash_size;

	/* The header values
	 */
	LIBEWF_HEADER_VALUES *header_values;

	/* The hash values
	 */
	LIBEWF_HASH_VALUES *hash_values;

	/* The MD5 digest context
	 */
	LIBEWF_MD5_CONTEXT md5_context;

	/* The stored MD5 hash of the data
	 */
	EWF_DIGEST_HASH *stored_md5_hash;

	/* The calculated MD5 hash of the data
	 */
	EWF_DIGEST_HASH *calculated_md5_hash;

	/* The sectors with acquiry read errors
	 */
	LIBEWF_ERROR_SECTOR *acquiry_error_sectors;

	/* The amount of the acquiry read error sectors
	 */
	uint32_t acquiry_amount_of_errors;

	/* The current chunk
	 */
	uint32_t current_chunk;

	/* The current chunk offset
	 */
	uint32_t current_chunk_offset;

	/* Value to indicate if a pair of bytes should be swapped
	 * this allows to convert little endian into big endian data and vice versa
	 * this is only applicable to the actual media data within the EWF file
	 */
	uint8_t swap_byte_pairs;

	/* value to indicate the compression level used
	 */
	int8_t compression_level;

	/* value to indicate how much header sections were found
	 */
	uint8_t amount_of_header_sections;

	/* value to indicate which file format is used
	 */
	uint8_t format;

	/* value to indicate which ewf format is used
	 */
	uint8_t ewf_format;

	/* value to indicate if the index has been build
	 */
	uint8_t index_build;

	/* value to indicate the level of error tollerance
	 */
	uint8_t error_tollerance;

	/* The GUID of the acquiry system
	 */
	uint8_t guid[ 16 ];
};

/* Additional subhandle for media specific parameters
 */
struct libewf_internal_handle_media
{
	/* The media size
	 */
	uint64_t media_size;

	/* The size of an individual chunk
	 */
	uint32_t chunk_size;

	/* The amount of sectors per chunk
	 */
	uint32_t sectors_per_chunk;

	/* The amount of bytes per sector
	 */
	uint32_t bytes_per_sector;

        /* The amount of chunks
         */
        uint32_t amount_of_chunks;

        /* The amount of sectors
         */
        uint32_t amount_of_sectors;

        /* The amount of sectors to use as error granularity
         */
        uint32_t error_granularity;

	/* The media type
	 */
	uint8_t media_type;

	/* The media flags
	 */
	uint8_t media_flags;
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
};

/* Additional subhandle for write specific parameters
 */
struct libewf_internal_handle_write
{
	/* The amount of bytes of the input written
	 */
	int64_t input_write_count;

	/* The total amount of bytes written
	 */
	int64_t write_count;

	/* The amount of bytes to write
	 */
	uint64_t input_write_size;

	/* The segment file size
	 */
	uint32_t segment_file_size;

	/* The maximum amount of segments
	 */
	int16_t maximum_amount_of_segments;

	/* The amount of bytes written to a section containing chunks
	 */
	int32_t chunks_section_write_count;

        /* The amount of chunks written
         */
        uint32_t amount_of_chunks;

        /* The determined (estimated) amount of chunks per segment
         */
        uint32_t chunks_per_segment;

        /* The determined (estimated) amount of chunks per chunks section
         */
        uint32_t chunks_per_chunks_section;

        /* The amount of chunks written of the current segment file
         */
        uint32_t segment_amount_of_chunks;

        /* The amount of chunks written of the current chunks section
         */
        uint32_t section_amount_of_chunks;

	/* The offset of the chunks section within the current segment file
	 */
	off_t chunks_section_offset;

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

LIBEWF_INTERNAL_HANDLE *libewf_internal_handle_alloc( uint16_t segment_amount, uint8_t flags );
void libewf_internal_handle_free( LIBEWF_INTERNAL_HANDLE *internal_handle );

LIBEWF_INTERNAL_HANDLE_MEDIA *libewf_internal_handle_media_alloc( void );
#define libewf_internal_handle_media_free( handle_media )	free( handle_media )

LIBEWF_INTERNAL_HANDLE_READ *libewf_internal_handle_read_alloc( void );
void libewf_internal_handle_read_free( LIBEWF_INTERNAL_HANDLE_READ *handle_read );

LIBEWF_INTERNAL_HANDLE_WRITE *libewf_internal_handle_write_alloc( void );
#define libewf_internal_handle_write_free( handle_write )	free( handle_write )

LIBEWF_INTERNAL_HANDLE *libewf_internal_handle_chunk_cache_realloc( LIBEWF_INTERNAL_HANDLE *internal_handle, size_t size );

int8_t libewf_internal_handle_is_set_header( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_is_set_header2( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_is_set_xheader( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_is_set_xhash( LIBEWF_INTERNAL_HANDLE *internal_handle );

int32_t libewf_internal_handle_get_media_sectors_per_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle );
int32_t libewf_internal_handle_get_media_bytes_per_sector( LIBEWF_INTERNAL_HANDLE *internal_handle );
int32_t libewf_internal_handle_get_media_amount_of_sectors( LIBEWF_INTERNAL_HANDLE *internal_handle );
int32_t libewf_internal_handle_get_media_chunk_size( LIBEWF_INTERNAL_HANDLE *internal_handle );
int32_t libewf_internal_handle_get_media_error_granularity( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_get_compression_level( LIBEWF_INTERNAL_HANDLE *internal_handle );
int64_t libewf_internal_handle_get_media_size( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_get_media_type( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_get_media_flags( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_get_volume_type( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_get_format( LIBEWF_INTERNAL_HANDLE *internal_handle );

int8_t libewf_internal_handle_get_guid( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t *guid, size_t size );

int64_t libewf_internal_handle_get_write_amount_of_chunks( LIBEWF_INTERNAL_HANDLE *internal_handle );

int16_t libewf_internal_handle_get_write_maximum_amount_of_segments( LIBEWF_INTERNAL_HANDLE *internal_handle );

int8_t libewf_internal_handle_get_header_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );
#define libewf_internal_handle_get_header_value_case_number( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "case_number" ), value, length )
#define libewf_internal_handle_get_header_value_description( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "description" ), value, length )
#define libewf_internal_handle_get_header_value_examiner_name( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "examiner_name" ), value, length )
#define libewf_internal_handle_get_header_value_evidence_number( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "evidence_number" ), value, length )
#define libewf_internal_handle_get_header_value_notes( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "notes" ), value, length )
#define libewf_internal_handle_get_header_value_acquiry_date( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "acquiry_date" ), value, length )
#define libewf_internal_handle_get_header_value_system_date( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "system_date" ), value, length )
#define libewf_internal_handle_get_header_value_acquiry_operating_system( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "acquiry_operating_system" ), value, length )
#define libewf_internal_handle_get_header_value_acquiry_software_version( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "acquiry_software_version" ), value, length )
#define libewf_internal_handle_get_header_value_password( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "password" ), value, length )
#define libewf_internal_handle_get_header_value_compression_type( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "compression_type" ), value, length )
#define libewf_internal_handle_get_header_value_model( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "model" ), value, length )
#define libewf_internal_handle_get_header_value_serial_number( handle, value, length ) \
        libewf_internal_handle_get_header_value( handle, _S_LIBEWF_CHAR( "serial_number" ), value, length )

int8_t libewf_internal_handle_get_hash_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

int8_t libewf_internal_handle_set_header( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER *header, size_t size );
int8_t libewf_internal_handle_set_header2( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER2 *header2, size_t size );
int8_t libewf_internal_handle_set_xheader( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER *xheader, size_t size );
int8_t libewf_internal_handle_set_xhash( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER *xhash, size_t size );

int8_t libewf_internal_handle_set_stored_md5_hash( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_DIGEST_HASH *md5_hash );

int8_t libewf_internal_handle_set_media_values( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t sectors_per_chunk, uint32_t bytes_per_sector );

int8_t libewf_internal_handle_set_guid( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t *guid, size_t size );

int8_t libewf_internal_handle_set_write_segment_file_size( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t segment_file_size );
int8_t libewf_internal_handle_set_write_error_granularity( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t error_granularity );
int8_t libewf_internal_handle_set_write_compression_values( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t compression_level, uint8_t compress_empty_block );
int8_t libewf_internal_handle_set_write_media_type( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t media_type, uint8_t volume_type );
int8_t libewf_internal_handle_set_write_media_flags( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t media_flags );
int8_t libewf_internal_handle_set_write_format( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t format );
int8_t libewf_internal_handle_set_write_input_write_size( LIBEWF_INTERNAL_HANDLE *internal_handle, uint64_t input_write_size );

int8_t libewf_internal_handle_set_header_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );
#define libewf_internal_handle_set_header_value_case_number( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "case_number" ), value, length )
#define libewf_internal_handle_set_header_value_description( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "description" ), value, length )
#define libewf_internal_handle_set_header_value_examiner_name( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "examiner_name" ), value, length )
#define libewf_internal_handle_set_header_value_evidence_number( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "evidence_number" ), value, length )
#define libewf_internal_handle_set_header_value_notes( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "notes" ), value, length )
#define libewf_internal_handle_set_header_value_acquiry_date( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "acquiry_date" ), value, length )
#define libewf_internal_handle_set_header_value_system_date( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "system_date" ), value, length )
#define libewf_internal_handle_set_header_value_acquiry_operating_system( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "acquiry_operating_system" ), value, length )
#define libewf_internal_handle_set_header_value_acquiry_software_version( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "acquiry_software_version" ), value, length )
#define libewf_internal_handle_set_header_value_password( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "password" ), value, length )
#define libewf_internal_handle_set_header_value_compression_type( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "compression_type" ), value, length )
#define libewf_internal_handle_set_header_value_model( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "model" ), value, length )
#define libewf_internal_handle_set_header_value_serial_number( handle, value, length ) \
        libewf_internal_handle_set_header_value( handle, _S_LIBEWF_CHAR( "serial_number" ), value, length )

int8_t libewf_internal_handle_set_hash_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length );

int8_t libewf_internal_handle_set_swap_byte_pairs( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t swap_byte_pairs );

int8_t libewf_internal_handle_add_acquiry_error_sector( LIBEWF_INTERNAL_HANDLE *internal_handle, uint64_t sector, uint32_t amount_of_sectors );
int8_t libewf_internal_handle_add_crc_error_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk );

int8_t libewf_internal_handle_determine_format( LIBEWF_INTERNAL_HANDLE *internal_handle );

int8_t libewf_internal_handle_create_header_values( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_create_headers( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_HEADER_VALUES *header_values );

int8_t libewf_internal_handle_read_is_initialized( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_read_initialize( LIBEWF_INTERNAL_HANDLE *internal_handle );

int8_t libewf_internal_handle_write_is_initialized( LIBEWF_INTERNAL_HANDLE *internal_handle );
int8_t libewf_internal_handle_write_initialize( LIBEWF_INTERNAL_HANDLE *internal_handle );

#ifdef __cplusplus
}
#endif

#endif


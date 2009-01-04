/*
 * libewf handle
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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

#ifndef _LIBEWF_HANDLE_H
#define _LIBEWF_HANDLE_H

#include <inttypes.h>

#include "ewf_error2.h"
#include "ewf_header.h"
#include "ewf_header2.h"
#include "ewf_md5hash.h"
#include "ewf_sectors.h"

#include "libewf_chunk_cache.h"
#include "libewf_offset_table.h"
#include "libewf_segment_table.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The levels are ordered from zero tollerance to full tollerance of non fatal errors
 * fatal error cannot be tollerated
 */
#define LIBEWF_ERROR_TOLLERANCE_NONE		0x00
#define LIBEWF_ERROR_TOLLERANCE_DATA_ONLY	0x03
#define LIBEWF_ERROR_TOLLERANCE_COMPENSATE	0x0C
#define LIBEWF_ERROR_TOLLERANCE_NON_FATAL	0x0F

#define LIBEWF_HANDLE libewf_handle_t
#define LIBEWF_HANDLE_SIZE sizeof( LIBEWF_HANDLE )

typedef struct libewf_handle libewf_handle_t;

struct libewf_handle
{
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
         * consists of 4 bytes (32 bits)
         */
        uint32_t chunk_count;

        /* The amount of sectors
         * consists of 4 bytes (32 bits)
         */
        uint32_t sector_count;

	/* The maximum ewf file size
	 */
	uint64_t input_file_size;

	/* The maximum ewf file size
	 */
	uint64_t ewf_file_size;

	/* The maximum amount of chunks per file
	 */
	uint32_t chunks_per_file;

	/* The list of segment files
	 */
	LIBEWF_SEGMENT_TABLE *segment_table;

	/* The list of offsets within the segment files within the table sections
	 */
	LIBEWF_OFFSET_TABLE *offset_table;

	/* The list of offsets within the segment files within the table2 sections
	 */
	LIBEWF_OFFSET_TABLE *secondary_offset_table;

	/* The amount of sectors to correct on read error
	 */
	uint32_t error_granularity_sectors;

	/* The amount of the stored error2 sectors
	 */
	uint64_t error2_error_count;

	/* The stored error2 sectors
	 */
	EWF_ERROR2_SECTOR *error2_sectors;

	/* The stored header
	 */
	EWF_HEADER *header;

	/* The stored header2
	 */
	EWF_HEADER *header2;

	/* The stored MD5 hash of the data
	 */
	EWF_MD5HASH *md5hash;

	/* value to indicate if a pair of bytes should be swapped
	 * this allows to convert little endian into big endian data and vice versa
	 * this is only applicable to the actual media data within the EWF file
	 */
	uint8_t swap_byte_pairs;

	/* value to indicate if compression is used
	 */
	uint8_t compression_used;

	/* value to indicate if compression is used
	 */
	int8_t compression_level;

	/* value to indicate if empty block should be compressed
	 * even if no compression is used
	 */
	int8_t compress_empty_block;

	/* value to indicate if the entire block hsould be wiped on a read error
	 */
	uint8_t wipe_block_on_read_error;

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

	/* the amount of retries on read error
	 */
	uint8_t read_error_retry;

	/* A simple cache is implemented here to avoid having to read and decompress the
	 * same chunk while reading the data.
	 */
	LIBEWF_CHUNK_CACHE *chunk_cache;

	/* The media type
	 */
	uint32_t media_type;

	/* The GUID (Encase 5 only)
	 * consists of 16 bytes
	 */
	uint8_t guid[16];
};

LIBEWF_HANDLE *libewf_handle_alloc( uint32_t segment_amount );
void libewf_handle_free( LIBEWF_HANDLE *handle );
uint8_t libewf_handle_is_set_header( LIBEWF_HANDLE *handle );
uint8_t libewf_handle_is_set_header2( LIBEWF_HANDLE *handle );
void libewf_handle_set_header( LIBEWF_HANDLE *handle, EWF_HEADER *header );
void libewf_handle_set_header2( LIBEWF_HANDLE *handle, EWF_HEADER *header2 );
void libewf_handle_set_md5hash( LIBEWF_HANDLE *handle, EWF_MD5HASH *md5hash );

#ifdef __cplusplus
}
#endif

#endif


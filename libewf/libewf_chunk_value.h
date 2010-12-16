/*
 * Chunk value functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEWF_CHUNK_VALUE_H )
#define _LIBEWF_CHUNK_VALUE_H

#include <common.h>
#include <types.h>

#include "libewf_segment_file_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBEWF_CHUNK_VALUE_FLAGS
{
	LIBEWF_CHUNK_VALUE_FLAG_COMPRESSED	= 0x01,
	LIBEWF_CHUNK_VALUE_FLAG_TAINTED		= 0x02,
	LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED	= 0x04,
	LIBEWF_CHUNK_VALUE_FLAG_MISSING		= 0x08,

	LIBEWF_CHUNK_VALUE_FLAG_DELTA_CHUNK	= 0x80
};

typedef struct libewf_chunk_value libewf_chunk_value_t;

struct libewf_chunk_value
{
	/* A reference to the segment file handle
	 */
	libewf_segment_file_handle_t *segment_file_handle;

	/* The index of the segment file in the segment or delta segment table
	 */
	int segment_table_index;

	/* The file offset of the chunk in the segment file
	 */
	off64_t file_offset;

	/* The size of the chunk
	 */
	size_t size;

	/* Flags
	 * 0x01 indicates if the chunk is compressed
	 * 0x02 indicates if the chunk is tainted and possibly corrupted
	 * 0x04 indicates if the chunk is corrupted
	 * 0x08 indicates if the chunk is missing
	 * 0x80 indicates if the chunk is a delta chunk
	 */
	uint8_t flags;
};

int libewf_chunk_value_initialize(
     libewf_chunk_value_t **chunk_value,
     liberror_error_t **error );

int libewf_chunk_value_free(
     intptr_t *chunk_value,
     liberror_error_t **error );

int libewf_chunk_value_clone(
     intptr_t **destination_chunk_value,
     intptr_t *source_chunk_value,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


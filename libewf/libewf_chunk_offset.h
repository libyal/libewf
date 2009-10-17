/*
 * Chunk offset functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBEWF_CHUNK_OFFSET_H )
#define _LIBEWF_CHUNK_OFFSET_H

#include <common.h>
#include <types.h>

#include "libewf_segment_file_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED	0x01
#define LIBEWF_CHUNK_OFFSET_FLAGS_TAINTED 	0x02
#define LIBEWF_CHUNK_OFFSET_FLAGS_CORRUPTED 	0x04
#define LIBEWF_CHUNK_OFFSET_FLAGS_MISSING 	0x08
#define LIBEWF_CHUNK_OFFSET_FLAGS_DELTA_CHUNK 	0x80

typedef struct libewf_chunk_offset libewf_chunk_offset_t;

struct libewf_chunk_offset
{
	/* A reference to the segment file handle
	 */
	libewf_segment_file_handle_t *segment_file_handle;

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

#if defined( __cplusplus )
}
#endif

#endif


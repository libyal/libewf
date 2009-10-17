/*
 * IO handle functions
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

#if !defined( _LIBEWF_IO_HANDLE_H )
#define _LIBEWF_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_io_handle libewf_io_handle_t;

struct libewf_io_handle
{
	/* The flags
	 */
	uint8_t flags;

	/* The file io pool
	 */
	libbfio_pool_t *file_io_pool;

	/* Value to indicate if the pool was created inside the library
	 */
	uint8_t pool_created_in_library;

	/* The current chunk
	 */
	uint32_t current_chunk;

	/* The current chunk offset
	 */
	uint32_t current_chunk_offset;

	/* Value to indicate which file format is used
	 */
	uint8_t format;

	/* Value to indicate which ewf format is used
	 */
	uint8_t ewf_format;

	/* Value to indicate the compression level used
	 */
	int8_t compression_level;

	/* Value to indicate certain compression modes
	 * like empty block compression
	 */
	uint8_t compression_flags;
};

int libewf_io_handle_initialize(
     libewf_io_handle_t **io_handle,
     liberror_error_t **error );

int libewf_io_handle_free(
     libewf_io_handle_t **io_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


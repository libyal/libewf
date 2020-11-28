/*
 * Chunk descriptor functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEWF_CHUNK_DESCRIPTOR_H )
#define _LIBEWF_CHUNK_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_chunk_descriptor libewf_chunk_descriptor_t;

struct libewf_chunk_descriptor
{
	/* The (chunk) data offset
	 */
	off64_t data_offset;

	/* The (chunk) data size
	 */
	size64_t data_size;

	/* The (chunk data) range flags
	 */
	uint32_t range_flags;
};

int libewf_chunk_descriptor_initialize(
     libewf_chunk_descriptor_t **chunk_descriptor,
     libcerror_error_t **error );

int libewf_chunk_descriptor_free(
     libewf_chunk_descriptor_t **chunk_descriptor,
     libcerror_error_t **error );

int libewf_chunk_descriptor_clone(
     libewf_chunk_descriptor_t **destination_chunk_descriptor,
     libewf_chunk_descriptor_t *source_chunk_descriptor,
     libcerror_error_t **error );

int libewf_chunk_descriptor_write_data(
     libewf_chunk_descriptor_t *chunk_descriptor,
     uint8_t *data,
     size_t data_size,
     off64_t base_offset,
     uint8_t format_version,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_CHUNK_DESCRIPTOR_H ) */


/*
 * Data chunk functions
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

#if !defined( _LIBEWF_INTERNAL_DATA_CHUNK_H )
#define _LIBEWF_INTERNAL_DATA_CHUNK_H

#include <common.h>
#include <types.h>

#include "libewf_chunk_data.h"
#include "libewf_extern.h"
#include "libewf_io_handle.h"
#include "libewf_libcerror.h"
#include "libewf_libcthreads.h"
#include "libewf_types.h"
#include "libewf_write_io_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_internal_data_chunk libewf_internal_data_chunk_t;

struct libewf_internal_data_chunk
{
	/* The data size
	 */
	size_t data_size;

	/* The IO handle
	 */
	libewf_io_handle_t *io_handle;

	/* The write IO handle
	 */
	libewf_write_io_handle_t *write_io_handle;

	/* The chunk data
	 */
	libewf_chunk_data_t *chunk_data;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libewf_data_chunk_initialize(
     libewf_data_chunk_t **data_chunk,
     libewf_io_handle_t *io_handle,
     libewf_write_io_handle_t *write_io_handle,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_data_chunk_free(
     libewf_data_chunk_t **data_chunk,
     libcerror_error_t **error );

int libewf_internal_data_chunk_set_chunk_data(
     libewf_internal_data_chunk_t *internal_data_chunk,
     libewf_chunk_data_t *chunk_data,
     libcerror_error_t **error );

LIBEWF_EXTERN \
int libewf_data_chunk_is_corrupted(
     libewf_data_chunk_t *data_chunk,
     libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_data_chunk_read_buffer(
         libewf_data_chunk_t *data_chunk,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBEWF_EXTERN \
ssize_t libewf_data_chunk_write_buffer(
         libewf_data_chunk_t *data_chunk,
         const void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_INTERNAL_DATA_CHUNK_H ) */


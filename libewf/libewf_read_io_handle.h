/*
 * Low level reading functions
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

#if !defined( _LIBEWF_READ_IO_HANDLE_H )
#define _LIBEWF_READ_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libewf_chunk_data.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_media_values.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_read_io_handle libewf_read_io_handle_t;

struct libewf_read_io_handle
{
	/* Cached version of the case data
	 */
	uint8_t *case_data;

	/* Size of the cached version of the case data
	 */
	size_t case_data_size;

	/* Cached version of the device information
	 */
	uint8_t *device_information;

	/* Size of the cached version of the device information
	 */
	size_t device_information_size;

	/* The storage media size read
	 */
	size64_t storage_media_size_read;

        /* The (total) number of chunks read
         */
        uint64_t number_of_chunks_read;
};

int libewf_read_io_handle_initialize(
     libewf_read_io_handle_t **read_io_handle,
     libcerror_error_t **error );

int libewf_read_io_handle_free(
     libewf_read_io_handle_t **read_io_handle,
     libcerror_error_t **error );

int libewf_read_io_handle_clone(
     libewf_read_io_handle_t **destination_read_io_handle,
     libewf_read_io_handle_t *source_read_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_READ_IO_HANDLE_H ) */


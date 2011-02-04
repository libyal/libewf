/*
 * Low level reading functions
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_READ_IO_HANDLE_H )
#define _LIBEWF_READ_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libewf_chunk_data.h"
#include "libewf_libbfio.h"
#include "libewf_libmfdata.h"
#include "libewf_media_values.h"
#include "libewf_sector_list.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_read_io_handle libewf_read_io_handle_t;

struct libewf_read_io_handle
{
	/* The sectors with checksum errors
	 */
	libewf_sector_list_t *checksum_errors;

	/* A value to indicate if the chunk data should be zeroed on error
	 */
	uint8_t zero_on_error;
};

int libewf_read_io_handle_initialize(
     libewf_read_io_handle_t **read_io_handle,
     liberror_error_t **error );

int libewf_read_io_handle_free(
     libewf_read_io_handle_t **read_io_handle,
     liberror_error_t **error );

int libewf_read_io_handle_clone(
     libewf_read_io_handle_t **destination_read_io_handle,
     libewf_read_io_handle_t *source_read_io_handle,
     liberror_error_t **error );

int libewf_read_io_handle_read_chunk_data(
     libewf_read_io_handle_t *read_io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libmfdata_list_t *chunk_table_list,
     libmfdata_cache_t *chunk_table_cache,
     int chunk_index,
     libewf_chunk_data_t **chunk_data,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


/*
 * Chunk group functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <types.h>

#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_unused.h"

/* Reads a chunk group
 * Callback function for the chunk groups list
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_read_element_data(
     intptr_t *data_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t segment_file_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

/* TODO implement */
	return( -1 );
}


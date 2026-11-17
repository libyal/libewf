/*
 * OSS-Fuzz target for libewf data chunk type
 *
 * Copyright (C) 2006-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libewf.h"

#if !defined( LIBEWF_HAVE_BFIO )

/* Opens a set of EWF file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
LIBEWF_EXTERN \
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libewf_error_t **error );

#endif /* !defined( LIBEWF_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t *chunk_buffer            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libewf_handle_t *handle          = NULL;
	libewf_data_chunk_t *data_chunk  = NULL;
	ssize_t read_count               = 0;
	size_t chunk_buffer_size         = 0;
	uint32_t bytes_per_sector        = 0;
	uint32_t sectors_per_chunk       = 0;
	int entry_index                  = 0;
	int read_iterator                = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbfio_pool_append_handle(
	     file_io_pool,
	     &entry_index,
	     file_io_handle,
	     LIBBFIO_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	/* The file IO pool takes over management of the file IO handle
	 */
	file_io_handle = NULL;

	if( libewf_handle_initialize(
	     &handle,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     LIBEWF_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_data_chunk(
	     handle,
	     &data_chunk,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	/* libewf_data_chunk_read_buffer requires a buffer that can hold the
	 * entire uncompressed chunk. Size it from the chunk geometry the handle
	 * reports and bound it so a malformed image cannot force a huge
	 * allocation.
	 */
	if( libewf_handle_get_bytes_per_sector(
	     handle,
	     &bytes_per_sector,
	     NULL ) != 1 || bytes_per_sector == 0 )
	{
		bytes_per_sector = 512;
	}
	if( libewf_handle_get_sectors_per_chunk(
	     handle,
	     &sectors_per_chunk,
	     NULL ) != 1 || sectors_per_chunk == 0 )
	{
		sectors_per_chunk = 64;
	}
	chunk_buffer_size = (size_t) bytes_per_sector * (size_t) sectors_per_chunk;

	if( chunk_buffer_size < 512 )
	{
		chunk_buffer_size = 512;
	}
	/* Upper bound at 64 MiB: covers any realistic chunk while keeping the
	 * allocation bounded for attacker-supplied geometry.
	 */
	if( chunk_buffer_size > ( 64 * 1024 * 1024 ) )
	{
		chunk_buffer_size = (size_t) ( 64 * 1024 * 1024 );
	}
	chunk_buffer = (uint8_t *) malloc( chunk_buffer_size );

	if( chunk_buffer == NULL )
	{
		goto on_error_libewf_data_chunk;
	}
	for( read_iterator = 0;
	     read_iterator < 128;
	     read_iterator++ )
	{
		read_count = libewf_handle_read_data_chunk(
		              handle,
		              data_chunk,
		              NULL );

		if( read_count == -1 )
		{
			break;
		}
		if( read_count == 0 )
		{
			break;
		}
		libewf_data_chunk_is_corrupted(
		 data_chunk,
		 NULL );
		if( libewf_data_chunk_read_buffer(
		     data_chunk,
		     chunk_buffer,
		     chunk_buffer_size,
		     NULL ) == -1 )
		{
			goto on_error_libewf_data_chunk;
		}
	}
on_error_libewf_data_chunk:
	if( chunk_buffer != NULL )
	{
		free( chunk_buffer );
	}
	if( libewf_data_chunk_free(
	     &data_chunk,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	libewf_handle_close(
	 handle,
	 NULL );

on_error_libewf:
	libewf_handle_free(
	 &handle,
	 NULL );

on_error_libbfio:
	/* Note that on error the handle still has a reference to file_io_pool
	 * that will be closed. Therefore the file IO pool and handle need to
	 * be freed after closing or freeing the handle.
	 */
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

} /* extern "C" */

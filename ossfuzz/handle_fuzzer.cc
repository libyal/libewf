/*
 * OSS-Fuzz target for libewf file type
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
	uint8_t buffer[ 512 ];
	uint8_t guid[ 16 ];

	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libewf_handle_t *handle          = NULL;
	off64_t media_offset             = 0;
	size64_t media_size              = 0;
	uint64_t value_64bit             = 0;
	uint32_t value_32bit             = 0;
	uint16_t value_16bit             = 0;
	uint8_t major_version            = 0;
	uint8_t minor_version            = 0;
	uint8_t value_8bit               = 0;
	int8_t compression_level         = 0;
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
	if( libewf_handle_segment_files_corrupted(
	     handle,
	     NULL ) == -1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_segment_files_encrypted(
	     handle,
	     NULL ) == -1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_bytes_per_sector(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_sectors_per_chunk(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_number_of_sectors(
	     handle,
	     &value_64bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_chunk_size(
	     handle,
	     (size32_t *) &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_error_granularity(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_compression_method(
	     handle,
	     &value_16bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_compression_values(
	     handle,
	     &compression_level,
	     &value_8bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_media_type(
	     handle,
	     &value_8bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_media_flags(
	     handle,
	     &value_8bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_format(
	     handle,
	     &value_8bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_segment_file_version(
	     handle,
	     &major_version,
	     &minor_version,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_segment_file_set_identifier(
	     handle,
	     guid,
	     16,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_md5_hash(
	     handle,
	     buffer,
	     16,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_sha1_hash(
	     handle,
	     buffer,
	     20,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_number_of_acquiry_errors(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_number_of_sessions(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_number_of_tracks(
	     handle,
	     &value_32bit,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	if( libewf_handle_get_media_size(
	     handle,
	     &media_size,
	     NULL ) != 1 )
	{
		goto on_error_libewf;
	}
	for( read_iterator = 0;
	     read_iterator < 128;
	     read_iterator++ )
	{
		if( media_offset >= media_size )
		{
			break;
		}
		if( libewf_handle_read_buffer_at_offset(
		     handle,
		     buffer,
		     497,
		     media_offset,
		     NULL ) == -1 )
		{
			goto on_error_libewf;
		}
		media_offset += 497;
	}
	if( libewf_handle_get_number_of_checksum_errors(
	     handle,
	     &value_32bit,
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


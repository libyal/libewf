/*
 * OSS-Fuzz target for libewf file entry type
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

static void get_file_entry_values(
              libewf_file_entry_t *file_entry )
{
	uint8_t buffer[ 512 ];
	uint8_t string[ 64 ];
	libewf_extent_t *extent = NULL;
	int64_t posix_time            = 0;
	size64_t extent_size           = 0;
	size64_t size64_value          = 0;
	size_t string_size             = 0;
	uint64_t identifier            = 0;
	off64_t extent_offset          = 0;
	off64_t duplicate_offset       = 0;
	uint8_t type_8bit              = 0;
	uint32_t extent_flags          = 0;
	uint32_t flags                 = 0;
	int number_of_sub_file_entries = 0;
	int number_of_extents          = 0;
	int number_of_attributes       = 0;
	int number_of_access_control_entries = 0;

	if( file_entry == NULL )
	{
		return;
	}
	libewf_file_entry_get_identifier(
	 file_entry,
	 &identifier,
	 NULL );
	libewf_file_entry_get_type(
	 file_entry,
	 &type_8bit,
	 NULL );
	libewf_file_entry_get_flags(
	 file_entry,
	 &flags,
	 NULL );
	libewf_file_entry_get_duplicate_media_data_offset(
	 file_entry,
	 &duplicate_offset,
	 NULL );
	if( libewf_file_entry_get_utf8_name_size(
	     file_entry,
	     &string_size,
	     NULL ) == 1 )
	{
		if( string_size > 64 )
		{
			string_size = 64;
		}
		libewf_file_entry_get_utf8_name(
		 file_entry,
		 string,
		 string_size,
		 NULL );
	}
	if( libewf_file_entry_get_utf16_name_size(
	     file_entry,
	     &string_size,
	     NULL ) == 1 )
	{
		if( string_size > 64 )
		{
			string_size = 64;
		}
		libewf_file_entry_get_utf16_name(
		 file_entry,
		 (uint16_t *) string,
		 string_size,
		 NULL );
	}
	libewf_file_entry_get_utf8_short_name_size(
	 file_entry,
	 &string_size,
	 NULL );
	libewf_file_entry_get_utf8_short_name(
	 file_entry,
	 string,
	 64,
	 NULL );
	libewf_file_entry_get_utf16_short_name_size(
	 file_entry,
	 &string_size,
	 NULL );
	libewf_file_entry_get_utf16_short_name(
	 file_entry,
	 (uint16_t *) string,
	 64,
	 NULL );
	libewf_file_entry_get_size(
	 file_entry,
	 &size64_value,
	 NULL );
	libewf_file_entry_get_creation_time(
	 file_entry,
	 &posix_time,
	 NULL );
	libewf_file_entry_get_modification_time(
	 file_entry,
	 &posix_time,
	 NULL );
	libewf_file_entry_get_access_time(
	 file_entry,
	 &posix_time,
	 NULL );
	libewf_file_entry_get_entry_modification_time(
	 file_entry,
	 &posix_time,
	 NULL );
	libewf_file_entry_get_deletion_time(
	 file_entry,
	 &posix_time,
	 NULL );
	libewf_file_entry_get_utf8_hash_value_md5(
	 file_entry,
	 string,
	 64,
	 NULL );
	libewf_file_entry_get_utf8_hash_value_sha1(
	 file_entry,
	 string,
	 64,
	 NULL );
	libewf_file_entry_read_buffer(
	 file_entry,
	 buffer,
	 497,
	 NULL );
	if( libewf_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     NULL ) == 1 && number_of_sub_file_entries > 0 )
	{
		libewf_file_entry_t *sub_file_entry = NULL;

		if( libewf_file_entry_get_sub_file_entry(
		     file_entry,
		     0,
		     &sub_file_entry,
		     NULL ) == 1 && sub_file_entry != NULL )
		{
			get_file_entry_values(
			 sub_file_entry );
			libewf_file_entry_free(
			 &sub_file_entry,
			 NULL );
		}
		libewf_file_entry_get_sub_file_entry_by_utf8_name(
		 file_entry,
		 (uint8_t *) "case_data",
		 9,
		 &sub_file_entry,
		 NULL );
	}
	if( libewf_file_entry_get_number_of_extents(
	     file_entry,
	     &number_of_extents,
	     NULL ) == 1 && number_of_extents > 0 )
	{
		if( libewf_file_entry_get_extent(
		     file_entry,
		     0,
		     &extent,
		     NULL ) == 1 && extent != NULL )
		{
			libewf_extent_get_values(
			 extent,
			 &extent_offset,
			 &extent_size,
			 &extent_flags,
			 NULL );
			libewf_extent_free(
			 &extent,
			 NULL );
		}
	}
	libewf_file_entry_get_number_of_attributes(
	 file_entry,
	 &number_of_attributes,
	 NULL );
	libewf_file_entry_get_number_of_access_control_entries(
	 file_entry,
	 &number_of_access_control_entries,
	 NULL );
}

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	libbfio_handle_t *file_io_handle   = NULL;
	libbfio_pool_t *file_io_pool       = NULL;
	libewf_handle_t *handle            = NULL;
	libewf_file_entry_t *file_entry    = NULL;
	size_t file_entry_path_size        = 0;
	int entry_index                    = 0;

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
	if( libewf_handle_get_root_file_entry(
	     handle,
	     &file_entry,
	     NULL ) == 1 && file_entry != NULL )
	{
		get_file_entry_values(
		 file_entry );
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	/* Use a bounded slice of the input as a path to explore the
	 * lookup of the file entry by name.
	 */
	file_entry_path_size = size;

	if( file_entry_path_size > 24 )
	{
		file_entry_path_size = 24;
	}
	if( file_entry_path_size > 0 )
	{
		libewf_handle_get_file_entry_by_utf8_path(
		 handle,
		 data,
		 file_entry_path_size,
		 &file_entry,
		 NULL );

		if( file_entry != NULL )
		{
			get_file_entry_values(
			 file_entry );
			libewf_file_entry_free(
			 &file_entry,
			 NULL );
		}
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

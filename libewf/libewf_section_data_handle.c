/*
 * The section data handle functions
 *
 * Copyright (C) 2006-2024, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_section_data_handle.h"
#include "libewf_unused.h"

/* Creates a section data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_section_data_handle_initialize(
     libewf_section_data_handle_t **data_handle,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_data_handle_initialize";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
		 function );

		return( -1 );
	}
	*data_handle = memory_allocate_structure(
	                libewf_section_data_handle_t );

	if( *data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_handle,
	     0,
	     sizeof( libewf_section_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		goto on_error;
	}
	( *data_handle )->data_offset = data_offset;
	( *data_handle )->data_size   = data_size;

	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees a section data handle
 * Returns 1 if successful or -1 on error
 */
int libewf_section_data_handle_free(
     libewf_section_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_section_data_handle_free";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( 1 );
}

/* Reads data from the current offset into a buffer
 * Callback for the data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_section_data_handle_read_segment_data(
         libewf_section_data_handle_t *data_handle,
         libbfio_pool_t *file_io_pool,
         int segment_index,
         int file_io_pool_entry,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBEWF_ATTRIBUTE_UNUSED,
         uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function = "libewf_section_data_handle_read_segment_data";
	size_t read_size      = 0;
	ssize_t read_count    = 0;
	off64_t file_offset   = 0;

	LIBEWF_UNREFERENCED_PARAMETER( segment_flags )
	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( data_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_index != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data.",
		 function );

		return( -1 );
	}
	if( segment_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_handle->current_offset >= (off64_t) data_handle->data_size )
	{
		return( 0 );
	}
	read_size = data_handle->data_size - (size_t) data_handle->current_offset;

	if( read_size > segment_data_size )
	{
		read_size = segment_data_size;
	}
	file_offset = data_handle->data_offset + data_handle->current_offset;

	read_count = libbfio_pool_read_buffer_at_offset(
	              file_io_pool,
	              file_io_pool_entry,
	              segment_data,
	              segment_data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	data_handle->current_offset += read_size;

	return( (ssize_t) read_size );
}

/* Seeks a certain offset of the data
 * Callback for the data stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_section_data_handle_seek_segment_offset(
         libewf_section_data_handle_t *data_handle,
         intptr_t *file_io_handle LIBEWF_ATTRIBUTE_UNUSED,
         int segment_index,
         int segment_file_index LIBEWF_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libewf_section_data_handle_seek_segment_offset";

	LIBEWF_UNREFERENCED_PARAMETER( file_io_handle )
	LIBEWF_UNREFERENCED_PARAMETER( segment_file_index )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_index != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	data_handle->current_offset = segment_offset;

	return( segment_offset );
}


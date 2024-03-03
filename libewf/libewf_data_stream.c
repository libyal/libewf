/*
 * Data stream functions
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libewf_buffer_data_handle.h"
#include "libewf_data_stream.h"
#include "libewf_libcerror.h"
#include "libewf_libfdata.h"
#include "libewf_section_data_handle.h"

/* Creates data stream from a buffer of data
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_data_stream_initialize_from_buffer(
     libfdata_stream_t **data_stream,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	libewf_buffer_data_handle_t *data_handle = NULL;
	libfdata_stream_t *safe_data_stream      = NULL;
	static char *function                    = "libewf_data_stream_initialize_from_buffer";
	int segment_index                        = 0;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( libewf_buffer_data_handle_initialize(
	     &data_handle,
	     buffer,
	     buffer_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create buffer data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_buffer_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_buffer_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libewf_buffer_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     safe_data_stream,
	     &segment_index,
	     0,
	     0,
	     (size64_t) buffer_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data stream segment.",
		 function );

		goto on_error;
	}
	*data_stream = safe_data_stream;

	return( 1 );

on_error:
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libewf_buffer_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Creates data stream from a section
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_data_stream_initialize_from_section(
     libfdata_stream_t **data_stream,
     int file_io_pool_entry,
     off64_t data_offset,
     size64_t data_size,
     libcerror_error_t **error )
{
	libewf_section_data_handle_t *data_handle = NULL;
	libfdata_stream_t *safe_data_stream       = NULL;
	static char *function                     = "libewf_data_stream_initialize_from_section";
	int segment_index                         = 0;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( libewf_section_data_handle_initialize(
	     &data_handle,
	     data_offset,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_section_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_section_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libewf_section_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     safe_data_stream,
	     &segment_index,
	     file_io_pool_entry,
	     0,
	     data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data stream segment.",
		 function );

		goto on_error;
	}
	*data_stream = safe_data_stream;

	return( 1 );

on_error:
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libewf_section_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}


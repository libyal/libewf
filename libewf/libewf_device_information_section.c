/*
 * Device information section functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_device_information.h"
#include "libewf_device_information_section.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"

/* Reads a device information section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_device_information_section_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_read_io_handle_t *read_io_handle,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         libcerror_error_t **error )
{
	uint8_t *string_data    = NULL;
	static char *function   = "libewf_device_information_section_read_file_io_pool";
	size_t string_data_size = 0;
	ssize_t read_count      = 0;

	if( section_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section descriptor.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( read_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read IO handle.",
		 function );

		return( -1 );
	}
	read_count = libewf_section_compressed_string_read(
		      section_descriptor,
		      io_handle,
		      file_io_pool,
		      file_io_pool_entry,
		      io_handle->compression_method,
		      &string_data,
		      &string_data_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read device information file object string.",
		 function );

		goto on_error;
	}
	else if( read_count != 0 )
	{
		if( read_io_handle->device_information == NULL )
		{
			if( libewf_device_information_parse(
			     string_data,
			     string_data_size,
			     media_values,
			     header_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to parse device information.",
				 function );

				goto on_error;
			}
			read_io_handle->device_information      = string_data;
			read_io_handle->device_information_size = string_data_size;
		}
		else
		{
			if( ( read_io_handle->device_information_size != string_data_size )
			 || ( memory_compare(
			       read_io_handle->device_information,
			       string_data,
			       string_data_size ) != 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_INPUT,
				 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
				 "%s: device information does not match.",
				 function );

				goto on_error;
			}
			memory_free(
			 string_data );
		}
	}
	return( read_count );

on_error:
	if( string_data != NULL )
	{
		memory_free(
		 string_data );
	}
	return( -1 );
}


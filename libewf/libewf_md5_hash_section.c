/*
 * MD5 hash section functions
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
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include "libewf_md5_hash_section.h"
#include "libewf_checksum.h"
#include "libewf_definitions.h"
#include "libewf_hash_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"

#include "ewf_hash.h"

/* Reads a version 1 hash section or a version 2 MD5 hash section
 * Returns 1 if successful or -1 on error
 */
int libewf_md5_hash_section_read_data(
     const uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libewf_hash_sections_t *hash_sections,
     libcerror_error_t **error )
{
	static char *function        = "libewf_md5_hash_section_read_data";
	size_t md5_hash_data_size    = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;
	int result                   = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		md5_hash_data_size = sizeof( ewf_hash_t );
	}
	else if( format_version == 2 )
	{
		md5_hash_data_size = sizeof( ewf_md5_hash_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( data_size != (size_t) md5_hash_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: MD5 hash data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_hash_t *) data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_md5_hash_t *) data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 data,
		 16,
		 0 );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_hash_t *) data )->unknown1,
			 16,
			 0 );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "\n" );
		}
		else if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_md5_hash_t *) data )->padding,
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version == 2 )
	{
		data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		return( -1 );
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		return( -1 );
	}
	result = libewf_section_test_zero(
		  data,
		  16,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MD5 hash is empty.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( memory_copy(
		     hash_sections->md5_hash,
		     data,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash in hash sections.",
			 function );

			return( -1 );
		}
		hash_sections->md5_hash_set = 1;
	}
	else
	{
		hash_sections->md5_hash_set = 0;
	}
	return( 1 );
}

/* Reads a version 1 hash section or a version 2 MD5 hash section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_md5_hash_section_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error )
{
	uint8_t *section_data    = NULL;
	static char *function    = "libewf_md5_hash_section_read_file_io_pool";
	size_t section_data_size = 0;
	ssize_t read_count       = 0;

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
	read_count = libewf_section_read_data(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              file_io_pool_entry,
	              &section_data,
	              &section_data_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	else if( read_count != 0 )
	{
		if( libewf_md5_hash_section_read_data(
		     section_data,
		     section_data_size,
		     format_version,
		     hash_sections,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section data.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 section_data );

	return( read_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

/* Writes a version 1 hash section or a version 2 MD5 hash section
 * Returns 1 if successful or -1 on error
 */
int libewf_md5_hash_section_write_data(
     uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libewf_hash_sections_t *hash_sections,
     libcerror_error_t **error )
{
	static char *function        = "libewf_md5_hash_section_write_data";
	size_t md5_hash_data_size    = 0;
	uint32_t calculated_checksum = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		md5_hash_data_size = sizeof( ewf_hash_t );
	}
	else if( format_version == 2 )
	{
		md5_hash_data_size = sizeof( ewf_md5_hash_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( data_size != (size_t) md5_hash_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     data,
	     0,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data.",
		 function );

		return( -1 );
	}
	if( hash_sections->md5_hash_set != 0 )
	{
		if( memory_copy(
		     data,
		     hash_sections->md5_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 hash.",
			 function );

			return( -1 );
		}
	}
	if( format_version == 2 )
	{
		data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate checksum.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_hash_t *) data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_md5_hash_t *) data )->checksum,
		 calculated_checksum );

		data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash section data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 data,
		 16,
		 0 );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_hash_t *) data )->unknown1,
			 16,
			 0 );
		}
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 calculated_checksum );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "\n" );
		}
		else if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_md5_hash_t *) data )->padding,
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Writes a version 1 or 2 MD5 hash section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_md5_hash_section_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error )
{
	uint8_t *section_data               = NULL;
	static char *function               = "libewf_md5_hash_section_write_file_io_pool";
	size_t md5_hash_data_size           = 0;
	size_t section_descriptor_data_size = 0;
	size_t section_padding_size         = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;

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
	if( format_version == 1 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v1_t );
		md5_hash_data_size           = sizeof( ewf_hash_t );
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		md5_hash_data_size           = sizeof( ewf_md5_hash_t );
		section_padding_size         = 12;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( hash_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash sections.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_set(
	     section_descriptor,
	     LIBEWF_SECTION_TYPE_MD5_HASH,
	     (uint8_t *) "hash",
	     4,
	     section_offset,
	     (size64_t) ( section_descriptor_data_size + md5_hash_data_size ),
	     (size64_t) md5_hash_data_size,
	     (uint32_t) section_padding_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section descriptor.",
		 function );

		goto on_error;
	}
	if( format_version == 1 )
	{
		write_count = libewf_section_descriptor_write_file_io_pool(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
			       error );

		if( write_count != (ssize_t) section_descriptor_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write section descriptor data.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	section_data = (uint8_t *) memory_allocate(
	                            md5_hash_data_size );

	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section data.",
		 function );

		goto on_error;
	}
	if( libewf_md5_hash_section_write_data(
	     section_data,
	     md5_hash_data_size,
	     format_version,
	     hash_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write section data.",
		 function );

		goto on_error;
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               section_data,
	               md5_hash_data_size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		goto on_error;
	}
	total_write_count += write_count;

	memory_free(
	 section_data );

	section_data = NULL;

	if( format_version == 2 )
	{
		write_count = libewf_section_descriptor_write_file_io_pool(
			       section_descriptor,
			       file_io_pool,
			       file_io_pool_entry,
			       format_version,
			       error );

		if( write_count != (ssize_t) section_descriptor_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write section descriptor data.",
			 function );

			goto on_error;
		}
		total_write_count += write_count;
	}
	return( total_write_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}


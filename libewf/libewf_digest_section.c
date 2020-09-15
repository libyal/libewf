/*
 * Digest section functions
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

#include "libewf_digest_section.h"
#include "libewf_checksum.h"
#include "libewf_definitions.h"
#include "libewf_hash_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"

#include "ewf_digest.h"

/* Reads a digest section
 * Returns 1 if successful or -1 on error
 */
int libewf_digest_section_read_data(
     const uint8_t *data,
     size_t data_size,
     libewf_hash_sections_t *hash_sections,
     libcerror_error_t **error )
{
	static char *function        = "libewf_digest_section_read_data";
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
	if( data_size != (size_t) sizeof( ewf_digest_t ) )
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
	 	 "%s: digest data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (ewf_digest_t *) data )->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_digest_t *) data )->md5_hash,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: SHA1 hash:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_digest_t *) data )->sha1_hash,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: padding:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_digest_t *) data )->padding1,
		 40,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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
		  ( (ewf_digest_t *) data )->md5_hash,
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
		     hash_sections->md5_digest,
		     ( (ewf_digest_t *) data )->md5_hash,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set MD5 digest in hash sections.",
			 function );

			return( -1 );
		}
		hash_sections->md5_digest_set = 1;
	}
	else
	{
		hash_sections->md5_digest_set = 0;
	}
	result = libewf_section_test_zero(
		  ( (ewf_digest_t *) data )->sha1_hash,
		  20,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if SHA1 hash is empty.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( memory_copy(
		     hash_sections->sha1_digest,
		     ( (ewf_digest_t *) data )->sha1_hash,
		     20 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set SHA1 digest in hash sections.",
			 function );

			return( -1 );
		}
		hash_sections->sha1_digest_set = 1;
	}
	else
	{
		hash_sections->sha1_digest_set = 0;
	}
	return( 1 );
}

/* Reads a digest section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_digest_section_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error )
{
	uint8_t *section_data    = NULL;
	static char *function    = "libewf_digest_section_read_file_io_pool";
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
		if( libewf_digest_section_read_data(
		     section_data,
		     section_data_size,
		     hash_sections,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read digest section.",
			 function );

			goto on_error;
		}
		memory_free(
		 section_data );
	}
	return( read_count );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	return( -1 );
}

/* Writes a digest section
 * Returns 1 if successful or -1 on error
 */
int libewf_digest_section_write_data(
     uint8_t *data,
     size_t data_size,
     libewf_hash_sections_t *hash_sections,
     libcerror_error_t **error )
{
	static char *function        = "libewf_digest_section_write_data";
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size != (size_t) sizeof( ewf_digest_t ) )
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
	if( hash_sections->md5_digest_set != 0 )
	{
		if( memory_copy(
		     ( (ewf_digest_t *) data )->md5_hash,
		     hash_sections->md5_digest,
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
	if( hash_sections->sha1_digest_set != 0 )
	{
		if( memory_copy(
		     ( (ewf_digest_t *) data )->sha1_hash,
		     hash_sections->sha1_digest,
		     20 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set SHA1 hash.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: digest section data:\n",
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
		 ( (ewf_digest_t *) data )->md5_hash,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: SHA1 hash:\n",
		 function );
		libcnotify_print_data(
		 ( (ewf_digest_t *) data )->sha1_hash,
		 20,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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
	byte_stream_copy_from_uint32_little_endian(
	 ( (ewf_digest_t *) data )->checksum,
	 calculated_checksum );

	return( 1 );
}

/* Writes a digest section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_digest_section_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libcerror_error_t **error )
{
	uint8_t section_data[ sizeof( ewf_digest_t ) ];

	static char *function     = "libewf_digest_section_write_file_io_pool";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

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
	if( libewf_section_descriptor_set(
	     section_descriptor,
	     0,
	     (uint8_t *) "digest",
	     6,
	     section_offset,
	     (size64_t) ( sizeof( ewf_section_descriptor_v1_t ) + sizeof( ewf_digest_t ) ),
	     (size64_t) sizeof( ewf_digest_t ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set section descriptor.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_descriptor_write_file_io_pool(
	               section_descriptor,
	               file_io_pool,
	               file_io_pool_entry,
	               1,
	               error );

	if( write_count != (ssize_t) sizeof( ewf_section_descriptor_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section descriptor.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( libewf_digest_section_write_data(
	     section_data,
	     sizeof( ewf_digest_t ),
	     hash_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to write section data.",
		 function );

		return( -1 );
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               section_data,
	               sizeof( ewf_digest_t ),
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	return( total_write_count );
}


/*
 * Ltree section functions
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

#include "libewf_checksum.h"
#include "libewf_debug.h"
#include "libewf_definitions.h"
#include "libewf_hash_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libhmac.h"
#include "libewf_ltree_section.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"

#include "ewf_ltree.h"

/* Reads a version 1 ltree section or version 2 singles files data section
 * single_files_data will be set to a pointer within the section data
 * Returns 1 if successful or -1 on error
 */
int libewf_ltree_section_read_data(
     const uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     uint8_t **single_files_data,
     size_t *single_files_data_size,
     libcerror_error_t **error )
{
	uint8_t calculated_md5_hash[ 16 ];

	uint8_t ltree_header_data[ sizeof( ewf_ltree_header_t ) ];

	static char *function        = "libewf_ltree_section_read_data";
	size_t data_offset           = 0;
	uint64_t stored_data_size    = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t trailing_data_size    = 0;
#endif

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
	if( ( format_version != 1 )
	 && ( format_version != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( single_files_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files data.",
		 function );

		return( -1 );
	}
	if( *single_files_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single files data value already set.",
		 function );

		return( -1 );
	}
	if( single_files_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files data size.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		if( data_size < sizeof( ewf_ltree_header_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for header.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: ltree header data:\n",
			 function );
			libcnotify_print_data(
			 data,
			 sizeof( ewf_ltree_header_t ),
			 0 );
		}
#endif
		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_ltree_header_t *) data )->data_size,
		 stored_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_ltree_header_t *) data )->checksum,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: integrity hash:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_ltree_header_t *) data )->integrity_hash,
			 16,
			 0 );

			libcnotify_printf(
			 "%s: data size\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 stored_data_size );

			libcnotify_printf(
			 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 stored_checksum );

			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_ltree_header_t *) data )->unknown1,
			 20,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( memory_copy(
		     ltree_header_data,
		     data,
		     sizeof( ewf_ltree_header_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy ltree header data.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_ltree_header_t *) ltree_header_data )->checksum,
		 0 );

		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     ltree_header_data,
		     sizeof( ewf_ltree_header_t ),
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate header checksum.",
			 function );

			return( -1 );
		}
		if( stored_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: header checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
			 function,
			 stored_checksum,
			 calculated_checksum );

			return( -1 );
		}
		data_offset += sizeof( ewf_ltree_header_t );
		data_size   -= sizeof( ewf_ltree_header_t );

		if( stored_data_size > (uint64_t) data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for entries data.",
			 function );

			return( -1 );
		}
		*single_files_data      = (uint8_t *) &( data[ data_offset ] );
		*single_files_data_size = (size_t) stored_data_size;
	}
	else if( format_version == 2 )
	{
		*single_files_data      = (uint8_t *) data;
		*single_files_data_size = data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libewf_debug_utf16_stream_print(
		     "single files data",
		     *single_files_data,
		     *single_files_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print single files data.",
			 function );

			return( -1 );
		}
	}
#endif
	if( format_version == 1 )
	{
		if( libhmac_md5_calculate(
		     *single_files_data,
		     *single_files_data_size,
		     calculated_md5_hash,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate integrity hash.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: calculated MD5 hash:\n",
			 function );
			libcnotify_print_data(
			 calculated_md5_hash,
			 16,
			 0 );
		}
#endif
		if( memory_compare(
		     data,
		     calculated_md5_hash,
		     16 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
			 "%s: mismatch in integrity hash.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT ) || defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( stored_data_size < (uint64_t) data_size )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				data_offset       += stored_data_size;
				trailing_data_size = data_size - stored_data_size;

				libcnotify_printf(
				 "%s: trailing data:\n",
				 function );
				libcnotify_print_data(
				 &( data[ data_offset ] ),
				 trailing_data_size,
				 0 );

#elif defined( HAVE_VERBOSE_OUTPUT )
				libcnotify_printf(
				 "%s: section has trailing data.\n",
				 function );
#endif
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) || defined( HAVE_VERBOSE_OUTPUT ) */
	}
	return( 1 );
}

/* Reads a version 1 ltree section or version 2 singles files data section
 * section_data will be set to a buffer containing the full section data
 * single_files_data will be set to a pointer within the section data
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_ltree_section_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t **section_data,
         size_t *section_data_size,
         uint8_t **single_files_data,
         size_t *single_files_data_size,
         libcerror_error_t **error )
{
	static char *function = "libewf_ltree_section_read_file_io_pool";
	ssize_t read_count    = 0;

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
	              section_data,
	              section_data_size,
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
	else if( read_count > 0 )
	{
		if( section_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing section data.",
			 function );

			goto on_error;
		}
		if( libewf_ltree_section_read_data(
		     *section_data,
		     *section_data_size,
		     format_version,
		     single_files_data,
		     single_files_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read ltree section.",
			 function );

			goto on_error;
		}
	}
	return( read_count );

on_error:
	if( *section_data != NULL )
	{
		memory_free(
		 *section_data );

		*section_data = NULL;
	}
	*section_data_size = 0;

	return( -1 );
}

/* Writes a version 1 ltree section or version 2 singles files data section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_ltree_section_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         uint8_t *section_data,
         size_t section_data_size,
         uint8_t *single_files_data,
         size_t single_files_data_size,
         libcerror_error_t **error )
{
	static char *function               = "libewf_ltree_section_write_file_io_pool";
	size_t ltree_header_data_size       = 0;
	size_t required_section_data_size   = 0;
	size_t section_descriptor_data_size = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint32_t calculated_checksum        = 0;
	uint32_t section_padding_size       = 0;

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
		ltree_header_data_size       = sizeof( ewf_ltree_header_t );
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
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
	if( section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section data.",
		 function );

		return( -1 );
	}
	if( section_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid section data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( single_files_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single files data.",
		 function );

		return( -1 );
	}
	required_section_data_size = ltree_header_data_size
	                           + single_files_data_size;

	if( required_section_data_size > section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid section data value too small.",
		 function );

		return( -1 );
	}
	if( single_files_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid single files data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libewf_section_descriptor_set(
	     section_descriptor,
	     LIBEWF_SECTION_TYPE_SINGLE_FILES_DATA,
	     (uint8_t *) "ltree",
	     5,
	     section_offset,
	     (size64_t) ( section_descriptor_data_size + required_section_data_size ),
	     (size64_t) required_section_data_size,
	     section_padding_size,
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

			return( -1 );
		}
		total_write_count += write_count;
	}
	if( format_version == 1 )
	{
		if( ( section_data + ltree_header_data_size ) != single_files_data )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid single files data value out of bounds.",
			 function );

			return( -1 );
		}
		if( single_files_data_size > (size_t) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid single files data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( section_data_size < ltree_header_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for header.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     section_data,
		     0,
		     ltree_header_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear ltree header data.",
			 function );

			return( -1 );
		}
		if( libhmac_md5_calculate(
		     single_files_data,
		     single_files_data_size,
		     ( (ewf_ltree_header_t *) section_data )->integrity_hash,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate integrity hash.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: calculated MD5 hash:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_ltree_header_t *) section_data )->integrity_hash,
			 16,
			 0 );
		}
#endif
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_ltree_header_t *) section_data )->data_size,
		 single_files_data_size );

		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     section_data,
		     ltree_header_data_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate header checksum.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_ltree_header_t *) section_data )->checksum,
		 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: ltree header data:\n",
			 function );
			libcnotify_print_data(
			 section_data,
			 ltree_header_data_size,
			 0 );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: integrity hash:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_ltree_header_t *) section_data )->integrity_hash,
			 16,
			 0 );

			libcnotify_printf(
			 "%s: data size\t\t\t\t\t: %" PRIzd "\n",
			 function,
			 single_files_data_size );

			libcnotify_printf(
			 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 calculated_checksum );

			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (ewf_ltree_header_t *) section_data )->unknown1,
			 20,
			 0 );
		}
#endif
	}
	else if( format_version == 2 )
	{
		section_descriptor->data_flags |= LIBEWF_SECTION_DATA_FLAGS_HAS_INTEGRITY_HASH;
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               section_data,
	               required_section_data_size,
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

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}


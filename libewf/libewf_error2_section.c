/*
 * Error2 section functions
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
#include "libewf_definitions.h"
#include "libewf_error2_section.h"
#include "libewf_hash_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"

#include "ewf_error.h"

/* Reads a version 1 error2 section or version 2 error table section
 * Returns 1 if successful or -1 on error
 */
int libewf_error2_section_read_data(
     const uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libcdata_range_list_t *acquiry_errors,
     libcerror_error_t **error )
{
	const uint8_t *error_data       = NULL;
	const uint8_t *error_entry_data = NULL;
	static char *function           = "libewf_error2_section_read_data";
	size_t error_entries_data_size  = 0;
	size_t error_entry_data_size    = 0;
	size_t error_footer_data_size   = 0;
	size_t error_header_data_size   = 0;
	uint64_t start_sector           = 0;
	uint32_t calculated_checksum    = 0;
	uint32_t entry_index            = 0;
	uint32_t number_of_entries      = 0;
	uint32_t number_of_sectors      = 0;
	uint32_t stored_checksum        = 0;
	int result                      = 0;

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
	if( format_version == 1 )
	{
		error_header_data_size = sizeof( ewf_error_header_v1_t );
		error_entry_data_size  = sizeof( ewf_error_entry_v1_t );
		error_footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		error_header_data_size = sizeof( ewf_error_header_v2_t );
		error_entry_data_size  = sizeof( ewf_error_entry_v2_t );
		error_footer_data_size = 16;
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
	if( data_size < error_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds - insufficient space for header.",
		 function );

		return( -1 );
	}
	error_data = data;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error header data:\n",
		 function );
		libcnotify_print_data(
		 error_data,
		 error_header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_data )->checksum,
		 stored_checksum);
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_data )->checksum,
		 stored_checksum);
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );

		if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_error_header_v1_t *) error_data )->unknown1,
			 200,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_error_header_v2_t *) error_data )->unknown1,
			 12,
			 0 );
		}
		libcnotify_printf(
	 	 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
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
			 ( (ewf_error_header_v2_t *) error_data )->padding,
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version == 2 )
	{
		error_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     error_data,
	     error_header_data_size - 4,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate header checksum.",
		 function );

		goto on_error;
	}
	if( format_version == 2 )
	{
		error_header_data_size += 12;
	}
	error_data += error_header_data_size;
	data_size  -= error_header_data_size;

	if( stored_checksum!= calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: header checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( number_of_entries > 0 )
	{
		error_entry_data = error_data;

		error_entries_data_size = number_of_entries * error_entry_data_size;

		if( error_entries_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid error entries data size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( data_size < error_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for entries.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: error entries data:\n",
			 function );
			libcnotify_print_data(
			 error_data,
			 error_entries_data_size,
			 0 );
		}
#endif
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     error_data,
		     error_entries_data_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate entries checksum.",
			 function );

			goto on_error;
		}
		error_data += error_entries_data_size;
		data_size  -= error_entries_data_size;

		if( data_size < error_footer_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for footer.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 error_data,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: error entries checksum\t\t\t: 0x%08" PRIx32 "\n",
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
				 &( error_data[ 4 ] ),
				 12,
				 0 );
			}
		}
#endif
		error_data += error_footer_data_size;
		data_size  -= error_footer_data_size;

		if( stored_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: entries checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
			 function,
			 stored_checksum,
			 calculated_checksum );

			goto on_error;
		}
#if defined( HAVE_VERBOSE_OUTPUT ) || defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( data_size > 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				libcnotify_printf(
				 "%s: trailing data:\n",
				 function );
				libcnotify_print_data(
				 error_data,
				 data_size,
				 0 );

#elif defined( HAVE_VERBOSE_OUTPUT )
				libcnotify_printf(
				 "%s: section has trailing data.\n",
				 function );
#endif
			}
		}
#endif
		if( libcdata_range_list_empty(
		     acquiry_errors,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty acquiry errors range list.",
			 function );

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( format_version == 1 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_error_entry_v1_t *) error_entry_data )->start_sector,
				 start_sector );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_error_entry_v1_t *) error_entry_data )->number_of_sectors,
				 number_of_sectors );
			}
			else if( format_version == 2 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (ewf_error_entry_v2_t *) error_entry_data )->start_sector,
				 start_sector );

				byte_stream_copy_to_uint32_little_endian(
				 ( (ewf_error_entry_v2_t *) error_entry_data )->number_of_sectors,
				 number_of_sectors );
			}

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: error entry: %02" PRIu32 " start sector\t\t\t: %" PRIu64 "\n",
				 function,
				 entry_index,
				 start_sector );

				libcnotify_printf(
				 "%s: error entry: %02" PRIu32 " number of sectors\t\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 number_of_sectors );

				if( format_version == 2 )
				{
					libcnotify_printf(
					 "%s: error entry: %02" PRIu32 " padding:\n",
					 function,
					 entry_index );
					libcnotify_print_data(
					 ( (ewf_error_entry_v2_t *) error_entry_data )->padding,
					 12,
					 0 );
				}
			}
#endif
			result = libcdata_range_list_insert_range(
			          acquiry_errors,
			          start_sector,
			          (uint64_t) number_of_sectors,
			          NULL,
			          NULL,
			          NULL,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert acquiry error in range list.",
				 function );

				goto on_error;
			}
			error_entry_data += error_entry_data_size;
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error section contains no entries.\n",
		 function );
	}
#endif
	return( 1 );

on_error:
	libcdata_range_list_empty(
	 acquiry_errors,
	 NULL,
	 NULL );

	return( -1 );
}

/* Reads a version 1 error2 section or version 2 error table section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_error2_section_read_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         libcdata_range_list_t *acquiry_errors,
         libcerror_error_t **error )
{
	uint8_t *section_data    = NULL;
	static char *function    = "libewf_error2_section_read_file_io_pool";
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
		if( libewf_error2_section_read_data(
		     section_data,
		     section_data_size,
		     format_version,
		     acquiry_errors,
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

/* Writes a version 1 error2 section or version 2 error table section
 * Returns 1 if successful or -1 on error
 */
int libewf_error2_section_write_data(
     uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libcdata_range_list_t *acquiry_errors,
     libcerror_error_t **error )
{
	uint8_t *error_data            = NULL;
	uint8_t *error_entry_data      = NULL;
	intptr_t *value                = NULL;
	static char *function          = "libewf_error2_section_write_data";
	size_t error_entries_data_size = 0;
	size_t error_entry_data_size   = 0;
	size_t error_footer_data_size  = 0;
	size_t error_header_data_size  = 0;
	size_t required_data_size      = 0;
	uint64_t number_of_sectors     = 0;
	uint64_t start_sector          = 0;
	uint32_t calculated_checksum   = 0;
	int entry_index                = 0;
	int number_of_entries          = 0;

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
	if( format_version == 1 )
	{
		error_header_data_size = sizeof( ewf_error_header_v1_t );
		error_entry_data_size  = sizeof( ewf_error_entry_v1_t );
		error_footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		error_header_data_size = sizeof( ewf_error_header_v2_t );
		error_entry_data_size  = sizeof( ewf_error_entry_v2_t );
		error_footer_data_size = 16;
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
	if( libcdata_range_list_get_number_of_elements(
	     acquiry_errors,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from acquiry error range list.",
		 function );

		return( -1 );
	}
	if( data_size < error_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of errors value out of bounds.",
		 function );

		return( -1 );
	}
	error_entries_data_size = number_of_entries * error_entry_data_size;

	required_data_size = error_header_data_size
	                   + error_entries_data_size
	                   + error_footer_data_size;

	if( data_size < required_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data value too small.",
		 function );

		return( -1 );
	}
	error_data = data;

	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_data )->number_of_entries,
		 number_of_entries );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_data )->number_of_entries,
		 number_of_entries );

		error_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     error_data,
	     error_header_data_size - 4,
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
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v1_t *) error_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_error_header_v2_t *) error_data )->checksum,
		 calculated_checksum );

		error_header_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error header data:\n",
		 function );
		libcnotify_print_data(
		 error_data,
		 error_header_data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );

		if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_error_header_v1_t *) error_data )->unknown1,
			 200,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else if( format_version == 1 )
		{
			libcnotify_print_data(
			 ( (ewf_error_header_v2_t *) error_data )->unknown1,
			 12,
			 0 );
		}
		libcnotify_printf(
	 	 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
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
			 ( (ewf_error_header_v2_t *) error_data )->padding,
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	error_data += error_header_data_size;

/* TODO EnCase compatible way to handle > 32-bit sector values */
	error_entry_data = error_data;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_range_list_get_range_by_index(
		     acquiry_errors,
		     entry_index,
		     &start_sector,
		     &number_of_sectors,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve acquiry error: %d from range list.",
			 function,
			 entry_index );

			return( -1 );
		}
/* TODO check bounds fo start_sector and number_of_sectors and print warning if truncation happens */
		if( format_version == 1 )
		{
			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_error_entry_v1_t *) error_entry_data )->start_sector,
			 (uint32_t) start_sector );

			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_error_entry_v1_t *) error_entry_data )->number_of_sectors,
			 (uint32_t) number_of_sectors );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_from_uint64_little_endian(
			 ( (ewf_error_entry_v2_t *) error_entry_data )->start_sector,
			 start_sector );

			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_error_entry_v2_t *) error_entry_data )->number_of_sectors,
			 (uint32_t) number_of_sectors );
		}
		error_entry_data += error_entry_data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error entries data:\n",
		 function );
		libcnotify_print_data(
		 error_data,
		 error_entries_data_size,
		 0 );
	}
#endif
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     error_data,
	     error_entries_data_size,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate entries checksum.",
		 function );

		return( -1 );
	}
	error_data += error_entries_data_size;

	byte_stream_copy_from_uint32_little_endian(
	 error_data,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error footer data:\n",
		 function );
		libcnotify_print_data(
		 error_data,
		 error_footer_data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: error entries checksum\t\t\t: 0x%08" PRIx32 "\n",
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
			 &( error_data[ 4 ] ),
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Writes a version 1 error2 section or version 2 error table section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_error2_section_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         off64_t section_offset,
         libcdata_range_list_t *acquiry_errors,
         libcerror_error_t **error )
{
	uint8_t *section_data               = NULL;
	static char *function               = "libewf_error2_section_write_file_io_pool";
	size_t error_entry_data_size        = 0;
	size_t error_entries_data_size      = 0;
	size_t error_footer_data_size       = 0;
	size_t error_header_data_size       = 0;
	size_t section_data_size            = 0;
	size_t section_descriptor_data_size = 0;
	ssize_t total_write_count           = 0;
	ssize_t write_count                 = 0;
	uint32_t section_padding_size       = 0;
	int number_of_entries               = 0;

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
		error_header_data_size       = sizeof( ewf_error_header_v1_t );
		error_entry_data_size        = sizeof( ewf_error_entry_v1_t );
		error_footer_data_size       = 4;
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		error_header_data_size       = sizeof( ewf_error_header_v2_t );
		error_entry_data_size        = sizeof( ewf_error_entry_v2_t );
		error_footer_data_size       = 16;
		section_padding_size         = 24;
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
	if( libcdata_range_list_get_number_of_elements(
	     acquiry_errors,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from acquiry error range list.",
		 function );

		goto on_error;
	}
	if( number_of_entries <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of errors value out of bounds.",
		 function );

		goto on_error;
	}
	error_entries_data_size = number_of_entries * error_entry_data_size;

	section_data_size = error_header_data_size
	                  + error_entries_data_size
	                  + error_footer_data_size;

	if( libewf_section_descriptor_set(
	     section_descriptor,
	     LIBEWF_SECTION_TYPE_ERROR_TABLE,
	     (uint8_t *) "error2",
	     6,
	     section_offset,
	     (size64_t) ( section_descriptor_data_size + section_data_size ),
	     (size64_t) section_data_size,
	     section_padding_size,
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
	if( ( section_data_size == 0 )
	 || ( section_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section data size value out of bounds.",
		 function );

		goto on_error;
	}
	section_data = (uint8_t *) memory_allocate(
	                            section_data_size );

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
	if( libewf_error2_section_write_data(
	     section_data,
	     section_data_size,
	     format_version,
	     acquiry_errors,
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
	               section_data_size,
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


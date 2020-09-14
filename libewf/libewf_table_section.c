/*
 * Table section functions
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

#include "libewf_checksum.h"
#include "libewf_definitions.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_section.h"
#include "libewf_table_section.h"

#include "ewf_section.h"
#include "ewf_table.h"

/* Creates a table section
 * Make sure the value table_section is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_table_section_initialize(
     libewf_table_section_t **table_section,
     libcerror_error_t **error )
{
	static char *function = "libewf_table_section_initialize";

	if( table_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table section.",
		 function );

		return( -1 );
	}
	if( *table_section != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table section value already set.",
		 function );

		return( -1 );
	}
	*table_section = memory_allocate_structure(
	                  libewf_table_section_t );

	if( *table_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table section.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_section,
	     0,
	     sizeof( libewf_table_section_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table section.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *table_section != NULL )
	{
		memory_free(
		 *table_section );

		*table_section = NULL;
	}
	return( -1 );
}

/* Frees a table section
 * Returns 1 if successful or -1 on error
 */
int libewf_table_section_free(
     libewf_table_section_t **table_section,
     libcerror_error_t **error )
{
	static char *function = "libewf_table_section_free";

	if( table_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table section.",
		 function );

		return( -1 );
	}
	if( *table_section != NULL )
	{
		if( ( *table_section )->section_data != NULL )
		{
			memory_free(
			 ( *table_section )->section_data );
		}
		memory_free(
		 *table_section );

		*table_section = NULL;
	}
	return( 1 );
}

/* Reads a version 1 table or table2 section or version 2 sector table section
 * The section data will be set to a buffer containing the relevant (not necessarily full) section data
 * The table entries data will be set to a pointer within the section data
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_table_section_read_file_io_pool(
         libewf_table_section_t *table_section,
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         uint8_t **section_data,
         size_t *section_data_size,
         uint64_t *first_chunk_index,
         uint64_t *base_offset,
         uint8_t **table_entries_data,
         size_t *table_entries_data_size,
         uint32_t *number_of_entries,
         uint8_t *entries_corrupted,
         libcerror_error_t **error )
{
	uint8_t *table_data           = NULL;
	static char *function         = "libewf_table_section_read_file_io_pool";
	size_t table_data_size        = 0;
	size_t table_entry_data_size  = 0;
	size_t table_header_data_size = 0;
	size_t table_footer_data_size = 0;
	ssize_t read_count            = 0;
	uint32_t calculated_checksum  = 0;
	uint32_t stored_checksum      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit          = 0;
#endif

	if( table_section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table section.",
		 function );

		return( -1 );
	}
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
	if( format_version == 1 )
	{
		table_header_data_size = sizeof( ewf_table_header_v1_t );
		table_entry_data_size  = sizeof( ewf_table_entry_v1_t );

		/* The original EWF and SMART (EWF-S01) formats do not contain a table footer
		 */
		if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
		{
			table_footer_data_size = 4;
		}
	}
	else if( format_version == 2 )
	{
		table_header_data_size = sizeof( ewf_table_header_v2_t );
		table_entry_data_size  = sizeof( ewf_table_entry_v2_t );
		table_footer_data_size = 16;
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
	if( *section_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid section data value already set.",
		 function );

		return( -1 );
	}
	if( section_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section data size.",
		 function );

		return( -1 );
	}
	if( first_chunk_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first chunk index.",
		 function );

		return( -1 );
	}
	if( base_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	if( table_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table entries data.",
		 function );

		return( -1 );
	}
	if( *table_entries_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table entries data value already set.",
		 function );

		return( -1 );
	}
	if( table_entries_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table entries data size.",
		 function );

		return( -1 );
	}
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	if( entries_corrupted == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entries corrupted.",
		 function );

		return( -1 );
	}
	/* In original EWF, SMART (EWF-S01) and EnCase1 EWF-E01 the trailing data will be the chunk data
	 */
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		*section_data_size = (size_t) table_header_data_size;
	}
	else
	{
		*section_data_size = (size_t) section_descriptor->data_size;
	}
/* TODO add support for table with chunk data */

	if( ( *section_data_size == 0 )
	 || ( *section_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section data size value out of bounds.",
		 function );

		goto on_error;
	}
	*section_data = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * *section_data_size );

	if( *section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create section data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
	              file_io_pool,
	              file_io_pool_entry,
	              *section_data,
	              *section_data_size,
	              error );

	if( read_count != (ssize_t) *section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	if( ( section_descriptor->data_flags & LIBEWF_SECTION_DATA_FLAGS_IS_ENCRYPTED ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: encrypted data:\n",
			 function );
			libcnotify_print_data(
			 *section_data,
			 *section_data_size,
			 0 );
		}
#endif
/* TODO decrypt */
		memory_free(
		 *section_data );

		*section_data      = NULL;
		*section_data_size = 0;

		return( 0 );
	}
	if( *section_data_size < table_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section size value out of bounds - insufficient space for header.",
		 function );

		goto on_error;
	}
	table_data      = *section_data;
	table_data_size = *section_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 table_data,
		 table_header_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->number_of_entries,
		 *number_of_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->base_offset,
		 *base_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->first_chunk_number,
		 *first_chunk_index );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->number_of_entries,
		 *number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: first chunk number\t\t\t\t: %" PRIu64 "\n",
			 function,
			 *first_chunk_index );
		}
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *number_of_entries );

		if( format_version == 1 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) table_data )->padding1,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding1\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: base offset\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 *base_offset );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) table_data )->padding2,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding2\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v2_t *) table_data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
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
			 ( (ewf_table_header_v2_t *) table_data )->padding,
			 12,
			 0 );
		}
	}
#endif
	if( format_version == 2 )
	{
		table_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     table_data,
	     table_header_data_size - 4,
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
		table_header_data_size += 12;
	}
	table_data      += table_header_data_size;
	table_data_size -= table_header_data_size;

/* TODO flag that header is corrupted and continue */
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

		goto on_error;
	}
	if( *number_of_entries > 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
/* TODO what about linen7 */
		if( format_version == 1 )
		{
			if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
			 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 ) )
			{
				if( *number_of_entries > LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6 )
				{
					libcnotify_printf(
					 "%s: number of entries: %" PRIu32 " exceeds maximum: %" PRIu32 ".\n",
					 function,
					 *number_of_entries,
					 LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6 );
				}
			}
			else
			{
				if( *number_of_entries > LIBEWF_MAXIMUM_TABLE_ENTRIES )
				{
					libcnotify_printf(
					 "%s: number of entries: %" PRIu32 " exceeds maximum: %" PRIu32 ".\n",
					 function,
					 *number_of_entries,
					 LIBEWF_MAXIMUM_TABLE_ENTRIES );
				}
			}
		}
#endif
		*table_entries_data_size = *number_of_entries * table_entry_data_size;

		if( *table_entries_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid table entries data size value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
		{
			memory_free(
			 *section_data );

			*section_data_size += *table_entries_data_size + table_footer_data_size;

			if( ( *section_data_size == 0 )
			 || ( *section_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid section data size value out of bounds.",
				 function );

				goto on_error;
			}
			*section_data = (uint8_t *) memory_allocate(
						     sizeof( uint8_t ) * *section_data_size );

			if( *section_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create section data.",
				 function );

				goto on_error;
			}
			read_count = libbfio_pool_read_buffer(
				      file_io_pool,
				      file_io_pool_entry,
				      *section_data,
				      *section_data_size,
				      error );

			if( read_count != (ssize_t) *section_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section data.",
				 function );

				goto on_error;
			}
			table_data      = *section_data;
			table_data_size = *section_data_size;
		}
/* TODO flag that number of entries is corrupted and continue */
		if( table_data_size < *table_entries_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for entries.",
			 function );

			goto on_error;
		}
		*table_entries_data = table_data;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entries data:\n",
			 function );
			libcnotify_print_data(
			 table_data,
			 *table_entries_data_size,
			 0 );
		}
#endif
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     table_data,
		     *table_entries_data_size,
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
		table_data      += *table_entries_data_size;
		table_data_size -= *table_entries_data_size;

		if( table_footer_data_size > 0 )
		{
			if( table_data_size < table_footer_data_size )
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
			 table_data,
			 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table entries checksum\t\t\t: 0x%08" PRIx32 "\n",
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
					 &( table_data[ 4 ] ),
					 12,
					 0 );
				}
			}
#endif
			table_data      += table_footer_data_size;
			table_data_size -= table_footer_data_size;

			if( stored_checksum != calculated_checksum )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
	                        if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: entries checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").\n",
					 function,
					 stored_checksum,
					 calculated_checksum );
				}
#endif
				/* The table entries cannot be fully trusted therefore mark them as corrupted
				 */
				*entries_corrupted = 1;
			}
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		if( *number_of_entries == 0 )
		{
			libcnotify_printf(
			 "%s: table section contains no entries.\n",
			 function );
		}
	}
#endif
#if defined( HAVE_VERBOSE_OUTPUT ) || defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( table_data_size > 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 table_data,
			 table_data_size,
			 0 );

#elif defined( HAVE_VERBOSE_OUTPUT )
			libcnotify_printf(
			 "%s: section has trailing data.\n",
			 function );
#endif
		}
	}
#endif
	return( read_count );

on_error:
	if( *section_data != NULL )
	{
		memory_free(
		 *section_data );

		*section_data = NULL;
	}
	*section_data_size       = 0;
	*table_entries_data      = NULL;
	*table_entries_data_size = 0;

	return( -1 );
}

/* Writes a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_table_section_write_file_io_pool(
         libewf_section_descriptor_t *section_descriptor,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t *section_data,
         size_t section_data_size,
         uint64_t first_chunk_index,
         uint64_t base_offset,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_entries,
         size64_t chunks_data_size,
         libcerror_error_t **error )
{
	uint8_t *table_data                 = NULL;
	static char *function               = "libewf_table_section_write_file_io_pool";
	size_t required_section_data_size   = 0;
	size_t section_descriptor_data_size = 0;
	size_t table_entry_data_size        = 0;
	size_t table_header_data_size       = 0;
	size_t table_footer_data_size       = 0;
	size_t used_table_entries_data_size = 0;
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
		table_header_data_size       = sizeof( ewf_table_header_v1_t );
		table_entry_data_size        = sizeof( ewf_table_entry_v1_t );
		table_footer_data_size       = 4;
	}
	else if( format_version == 2 )
	{
		section_descriptor_data_size = sizeof( ewf_section_descriptor_v2_t );
		table_header_data_size       = sizeof( ewf_table_header_v2_t );
		table_entry_data_size        = sizeof( ewf_table_entry_v2_t );
		table_footer_data_size       = 16;
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
	if( type_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type string.",
		 function );

		return( -1 );
	}
	if( base_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base offset value exceeds maximum.",
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
	if( table_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table entries data.",
		 function );

		return( -1 );
	}
	if( table_entries_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table entries data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	used_table_entries_data_size = number_of_entries * table_entry_data_size;

	if( used_table_entries_data_size > table_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data size value out of bounds.",
		 function );

		return( -1 );
	}
	required_section_data_size = table_header_data_size
	                           + used_table_entries_data_size;

	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		required_section_data_size += table_footer_data_size;
	}
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
	if( libewf_section_descriptor_set(
	     section_descriptor,
	     LIBEWF_SECTION_TYPE_SECTOR_TABLE,
	     type_string,
	     type_string_length,
	     section_offset,
	     section_descriptor_data_size + required_section_data_size + chunks_data_size,
	     required_section_data_size + chunks_data_size,
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
	table_data = section_data;

	if( memory_set(
	     table_data,
	     0,
	     table_header_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table header data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->number_of_entries,
		 number_of_entries );

		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) table_data )->base_offset,
		 base_offset );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->first_chunk_number,
		 first_chunk_index );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->number_of_entries,
		 number_of_entries );

		table_header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     table_data,
	     table_header_data_size - 4,
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
		 ( (ewf_table_header_v1_t *) table_data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) table_data )->checksum,
		 calculated_checksum );

		table_header_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 table_data,
		 table_header_data_size,
		 0 );
	}
#endif
	table_data += table_header_data_size;

	if( table_data != table_entries_data )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table entries data:\n",
		 function );
		libcnotify_print_data(
		 table_data,
		 used_table_entries_data_size,
		 0 );
	}
#endif
	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     table_data,
		     used_table_entries_data_size,
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
		table_data += used_table_entries_data_size;

		if( memory_set(
		     table_data,
		     0,
		     table_footer_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear table footer data.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint32_little_endian(
		 table_data,
		 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table footer data:\n",
			 function );
			libcnotify_print_data(
			 table_data,
			 table_footer_data_size,
			 0 );
		}
#endif
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


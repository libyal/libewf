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

/* Reads the header of a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes read or -1 on error
 */
int libewf_table_section_read_header_data(
     libewf_table_section_t *table_section,
     libewf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libcerror_error_t **error )
{
	static char *function              = "libewf_table_section_read_header_data";
	size_t header_data_size            = 0;
	uint32_t calculated_checksum       = 0;
	uint32_t stored_checksum           = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint32_t maximum_number_of_entries = 0;
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit               = 0;
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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		header_data_size = sizeof( ewf_table_header_v1_t );
	}
	else if( format_version == 2 )
	{
		header_data_size = sizeof( ewf_table_header_v2_t );
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
	if( ( data_size < header_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 0 );
	}
#endif
	if( format_version == 1 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) data )->number_of_entries,
		 table_section->number_of_entries );

		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) data )->base_offset,
		 table_section->base_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v1_t *) data )->checksum,
		 stored_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_header_v2_t *) data )->first_chunk_number,
		 table_section->first_chunk_index );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) data )->number_of_entries,
		 table_section->number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( format_version == 2 )
		{
			libcnotify_printf(
			 "%s: first chunk number\t\t: %" PRIu64 "\n",
			 function,
			 table_section->first_chunk_index );
		}
		libcnotify_printf(
		 "%s: number of entries\t\t: %" PRIu32 "\n",
		 function,
		 table_section->number_of_entries );

		if( format_version == 1 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) data )->padding1,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding1\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: base offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 table_section->base_offset );

			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v1_t *) data )->padding2,
			 value_32bit );
			libcnotify_printf(
			 "%s: padding2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		else if( format_version == 2 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (ewf_table_header_v2_t *) data )->unknown1,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
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
			 ( (ewf_table_header_v2_t *) data )->padding,
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version == 1 )
	{
		header_data_size -= 4;
	}
	else if( format_version == 2 )
	{
		header_data_size -= 16;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     header_data_size,
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

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
/* TODO what about linen7 */
		if( format_version == 1 )
		{
			if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
			 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 ) )
			{
				maximum_number_of_entries = LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6;
			}
			else
			{
				maximum_number_of_entries = LIBEWF_MAXIMUM_TABLE_ENTRIES;
			}
			if( table_section->number_of_entries > maximum_number_of_entries )
			{
				libcnotify_printf(
				 "%s: number of entries: %" PRIu32 " exceeds maximum: %" PRIu32 ".\n",
				 function,
				 table_section->number_of_entries,
				 maximum_number_of_entries );
			}
		}
	}
#endif /* defined( HAVE_VERBOSE_OUTPUT ) */

	if( table_section->base_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid base offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( table_section->first_chunk_index > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid first chunk index value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the footer of a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes read or -1 on error
 */
int libewf_table_section_read_footer_data(
     libewf_table_section_t *table_section,
     const uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     uint32_t *stored_checksum,
     libcerror_error_t **error )
{
	static char *function         = "libewf_table_section_read_footer_data";
	size_t footer_data_size       = 0;
	uint32_t safe_stored_checksum = 0;

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		footer_data_size = 16;
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
	if( data_size != footer_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( stored_checksum == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stored checksum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table footer data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 footer_data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 data,
	 safe_stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table entries checksum\t\t: 0x%08" PRIx32 "\n",
		 function,
		 safe_stored_checksum );

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
			 &( data[ 4 ] ),
			 12,
			 0 );
		}
	}
#endif /* defined( HAVE_VERBOSE_OUTPUT ) */

	*stored_checksum = safe_stored_checksum;

	return( 1 );
}

/* Reads a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_table_section_read_file_io_pool(
         libewf_table_section_t *table_section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         size64_t section_data_size,
         uint32_t section_data_flags,
         libcerror_error_t **error )
{
	static char *function         = "libewf_table_section_read_file_io_pool";
	size_t data_offset            = 0;
	size_t table_entry_data_size  = 0;
	size_t table_footer_data_size = 0;
	size_t table_header_data_size = 0;
	ssize_t read_count            = 0;
	uint32_t calculated_checksum  = 0;
	uint32_t stored_checksum      = 0;

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
	if( ( section_data_size == 0 )
	 || ( section_data_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section data size value out of bounds.",
		 function );

		return( -1 );
	}
	/* In original EWF, SMART (EWF-S01) and EnCase1 EWF-E01 the trailing data will be the chunk data
	 */
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
	{
/* TODO consider reading max number of chunks for table with chunk data */
		table_section->section_data_size = (size_t) table_header_data_size;
	}
	else
	{
		table_section->section_data_size = (size_t) section_data_size;
	}
	if( ( table_section->section_data_size < table_header_data_size )
	 || ( table_section->section_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid section data size value out of bounds.",
		 function );

		goto on_error;
	}
	table_section->section_data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * table_section->section_data_size );

	if( table_section->section_data == NULL )
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
	              table_section->section_data,
	              table_section->section_data_size,
	              error );

	if( read_count != (ssize_t) table_section->section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section data.",
		 function );

		goto on_error;
	}
	if( ( section_data_flags & LIBEWF_SECTION_DATA_FLAGS_IS_ENCRYPTED ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: encrypted data:\n",
			 function );
			libcnotify_print_data(
			 table_section->section_data,
			 table_section->section_data_size,
			 0 );
		}
#endif
/* TODO decrypt */
		memory_free(
		 table_section->section_data );

		table_section->section_data      = NULL;
		table_section->section_data_size = 0;

		return( 0 );
	}
	if( libewf_table_section_read_header_data(
	     table_section,
	     io_handle,
	     table_section->section_data,
	     table_section->section_data_size,
	     format_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table section header.",
		 function );

		goto on_error;
	}
	data_offset = table_header_data_size;

	if( table_section->number_of_entries > 0 )
	{
		if( (size_t) table_section->number_of_entries > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / table_entry_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid table section - number of entries value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		table_section->entries_size = (size_t) table_section->number_of_entries * table_entry_data_size;

		if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
		 || ( io_handle->format == LIBEWF_FORMAT_ENCASE1 ) )
		{
			if( (size_t) table_section->number_of_entries > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE - table_footer_data_size ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid section data size value out of bounds.",
				 function );

				goto on_error;
			}
			table_section->section_data_size = table_section->entries_size + table_footer_data_size;

			memory_free(
			 table_section->section_data );

/* TODO consider using memory_reallocate */
			table_section->section_data = (uint8_t *) memory_allocate(
			                                           sizeof( uint8_t ) * table_section->section_data_size );

			if( table_section->section_data == NULL )
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
				      table_section->section_data,
				      table_section->section_data_size,
				      error );

			if( read_count != (ssize_t) table_section->section_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section data.",
				 function );

				goto on_error;
			}
			data_offset = 0;
		}
/* TODO flag that number of entries is corrupted and continue */
		if( table_section->entries_size > ( table_section->section_data_size - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid section size value out of bounds - insufficient space for entries.",
			 function );

			goto on_error;
		}
		table_section->entries_offset = data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entries data:\n",
			 function );
			libcnotify_print_data(
			 &( table_section->section_data[ data_offset ] ),
			 table_section->entries_size,
			 0 );
		}
#endif
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     &( table_section->section_data[ data_offset ] ),
		     table_section->entries_size,
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
		data_offset += table_section->entries_size;

		if( table_footer_data_size > 0 )
		{
			if( libewf_table_section_read_footer_data(
			     table_section,
			     &( table_section->section_data[ data_offset ] ),
			     table_section->section_data_size - data_offset,
			     format_version,
			     &stored_checksum,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read table section footer.",
				 function );

				goto on_error;
			}
			data_offset += table_footer_data_size;

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
				table_section->entries_corrupted = 1;
			}
		}
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		if( table_section->number_of_entries == 0 )
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
		if( data_offset < table_section->section_data_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( table_section->section_data[ data_offset ] ),
			 table_section->section_data_size - data_offset,
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
	if( table_section->section_data != NULL )
	{
		memory_free(
		 table_section->section_data );

		table_section->section_data = NULL;
	}
	table_section->section_data_size = 0;

	return( -1 );
}

/* Writes the header of a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes written or -1 on error
 */
int libewf_table_section_write_header_data(
     libewf_table_section_t *table_section,
     uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     libcerror_error_t **error )
{
	static char *function        = "libewf_table_section_write_header_data";
	size_t header_data_size      = 0;
	uint32_t calculated_checksum = 0;

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		header_data_size = sizeof( ewf_table_header_v1_t );
	}
	else if( format_version == 2 )
	{
		header_data_size = sizeof( ewf_table_header_v2_t );
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
	if( ( data_size < header_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     data,
	     0,
	     header_data_size ) == NULL )
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
		 ( (ewf_table_header_v1_t *) data )->number_of_entries,
		 table_section->number_of_entries );

		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_table_header_v1_t *) data )->base_offset,
		 table_section->base_offset );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint64_little_endian(
		 ( (ewf_table_header_v2_t *) data )->first_chunk_number,
		 table_section->first_chunk_index );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) data )->number_of_entries,
		 table_section->number_of_entries );

		header_data_size -= 12;
	}
	if( libewf_checksum_calculate_adler32(
	     &calculated_checksum,
	     data,
	     header_data_size - 4,
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
		 ( (ewf_table_header_v1_t *) data )->checksum,
		 calculated_checksum );
	}
	else if( format_version == 2 )
	{
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_header_v2_t *) data )->checksum,
		 calculated_checksum );

		header_data_size += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 0 );
	}
#endif
	return( 1 );
}

/* Writes the footer of a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes written or -1 on error
 */
int libewf_table_section_write_footer_data(
     libewf_table_section_t *table_section,
     uint8_t *data,
     size_t data_size,
     uint8_t format_version,
     uint32_t calculated_checksum,
     libcerror_error_t **error )
{
	static char *function   = "libewf_table_section_write_footer_data";
	size_t footer_data_size = 0;

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		footer_data_size = 4;
	}
	else if( format_version == 2 )
	{
		footer_data_size = 16;
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
	if( ( data_size < footer_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     data,
	     0,
	     footer_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data.",
		 function );

		return( -1 );
	}
	byte_stream_copy_from_uint32_little_endian(
	 data,
	 calculated_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table footer data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 footer_data_size,
		 0 );
	}
#endif
	return( 1 );
}

/* Writes a version 1 table or table2 section or version 2 sector table section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_table_section_write_file_io_pool(
         libewf_table_section_t *table_section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t format_version,
         uint8_t segment_file_type,
         uint8_t *type_string,
         size_t type_string_length,
         off64_t section_offset,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         size64_t chunks_data_size,
         libcerror_error_t **error )
{
	libewf_section_descriptor_t *section_descriptor = NULL;
	static char *function                           = "libewf_table_section_write_file_io_pool";
	size_t data_offset                              = 0;
	size_t required_section_data_size               = 0;
	size_t section_descriptor_data_size             = 0;
	size_t table_entry_data_size                    = 0;
	size_t table_footer_data_size                   = 0;
	size_t table_header_data_size                   = 0;
	size_t used_table_entries_data_size             = 0;
	ssize_t total_write_count                       = 0;
	ssize_t write_count                             = 0;
	uint32_t calculated_checksum                    = 0;
	uint32_t section_padding_size                   = 0;

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
	if( table_section->section_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table section - missing section data.",
		 function );

		return( -1 );
	}
	if( table_section->section_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table section - section data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_section->base_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table section - base offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_section->number_of_entries == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table section - number of entries values out of bounds.",
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
/* TODO improve bounds check */
	if( ( table_entries_data_size == 0 )
	 || ( table_entries_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data size value out of bounds.",
		 function );

		return( -1 );
	}
	used_table_entries_data_size = table_section->number_of_entries * table_entry_data_size;

	if( used_table_entries_data_size > table_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data size value out of bounds.",
		 function );

		goto on_error;
	}
	required_section_data_size = table_header_data_size
	                           + used_table_entries_data_size;

	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		required_section_data_size += table_footer_data_size;
	}
	if( required_section_data_size > table_section->section_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid section data value too small.",
		 function );

		goto on_error;
	}
	if( libewf_section_descriptor_initialize(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section descriptor.",
		 function );

		goto on_error;
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
	if( libewf_table_section_write_header_data(
	     table_section,
	     table_section->section_data,
	     table_section->section_data_size,
	     format_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write section header data.",
		 function );

		goto on_error;
	}
	data_offset = table_header_data_size;

	if( &( table_section->section_data[ data_offset ] ) != table_entries_data )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table entries data:\n",
		 function );
		libcnotify_print_data(
		 &( table_section->section_data[ data_offset ] ),
		 used_table_entries_data_size,
		 0 );
	}
#endif
	if( segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     &( table_section->section_data[ data_offset ] ),
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

			goto on_error;
		}
		data_offset += used_table_entries_data_size;

		if( libewf_table_section_write_footer_data(
		     table_section,
		     &( table_section->section_data[ data_offset ] ),
		     table_section->section_data_size - data_offset,
		     format_version,
		     calculated_checksum,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write section footer data.",
			 function );

			goto on_error;
		}
	}
	write_count = libewf_section_write_data(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               file_io_pool_entry,
	               table_section->section_data,
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

		goto on_error;
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

			goto on_error;
		}
		total_write_count += write_count;
	}
	if( libewf_section_descriptor_free(
	     &section_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		goto on_error;
	}
	return( total_write_count );

on_error:
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( -1 );
}


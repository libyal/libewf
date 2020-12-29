/*
 * Low level writing functions
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
#include <system_string.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_chunk_data.h"
#include "libewf_chunk_descriptor.h"
#include "libewf_chunk_table.h"
#include "libewf_compression.h"
#include "libewf_definitions.h"
#include "libewf_filename.h"
#include "libewf_header_sections.h"
#include "libewf_header_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_libfvalue.h"
#include "libewf_media_values.h"
#include "libewf_read_io_handle.h"
#include "libewf_section.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_unused.h"
#include "libewf_write_io_handle.h"

#include "ewf_data.h"
#include "ewf_section.h"
#include "ewf_table.h"

/* Creates a write IO handle
 * Make sure the value write_io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize(
     libewf_write_io_handle_t **write_io_handle,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_initialize";

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( *write_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid write IO handle value already set.",
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
	*write_io_handle = memory_allocate_structure(
	                    libewf_write_io_handle_t );

	if( *write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create write IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *write_io_handle,
	     0,
	     sizeof( libewf_write_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear write IO handle.",
		 function );

		memory_free(
		 *write_io_handle );

		*write_io_handle = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *write_io_handle )->chunks_section ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk section.",
		 function );

		goto on_error;
	}
	( *write_io_handle )->pack_flags                  = LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM;
	( *write_io_handle )->section_descriptor_size     = sizeof( ewf_section_descriptor_v1_t );
	( *write_io_handle )->table_header_size           = sizeof( ewf_table_header_v1_t );
	( *write_io_handle )->table_entry_size            = sizeof( ewf_table_entry_v1_t );
	( *write_io_handle )->maximum_segment_file_size   = INT64_MAX;
	( *write_io_handle )->remaining_segment_file_size = LIBEWF_DEFAULT_SEGMENT_FILE_SIZE;
	( *write_io_handle )->maximum_chunks_per_section  = LIBEWF_MAXIMUM_TABLE_ENTRIES_ENCASE6;
	( *write_io_handle )->maximum_number_of_segments  = (uint32_t) 14971;
	( *write_io_handle )->current_file_io_pool_entry  = -1;

	return( 1 );

on_error:
	if( *write_io_handle != NULL )
	{
		memory_free(
		 *write_io_handle );

		*write_io_handle = NULL;
	}
	return( -1 );
}

/* Frees a write IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_free(
     libewf_write_io_handle_t **write_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_free";
	int result            = 1;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( *write_io_handle != NULL )
	{
		if( ( *write_io_handle )->case_data != NULL )
		{
			memory_free(
			 ( *write_io_handle )->case_data );
		}
		if( ( *write_io_handle )->device_information != NULL )
		{
			memory_free(
			 ( *write_io_handle )->device_information );
		}
		if( ( *write_io_handle )->data_section != NULL )
		{
			memory_free(
			 ( *write_io_handle )->data_section );
		}
		if( ( *write_io_handle )->table_section_data != NULL )
		{
			memory_free(
			 ( *write_io_handle )->table_section_data );
		}
		if( ( *write_io_handle )->compressed_zero_byte_empty_block != NULL )
		{
			memory_free(
			 ( *write_io_handle )->compressed_zero_byte_empty_block );
		}
		if( libcdata_array_free(
		     &( ( *write_io_handle )->chunks_section ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk section.",
			 function );

			result = -1;
		}
		memory_free(
		 *write_io_handle );

		*write_io_handle = NULL;
	}
	return( result );
}

/* Clones the write IO handle
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_clone(
     libewf_write_io_handle_t **destination_write_io_handle,
     libewf_write_io_handle_t *source_write_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_clone";

	if( destination_write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination write IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_write_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination write IO handle value already set.",
		 function );

		return( -1 );
	}
	if( source_write_io_handle == NULL )
	{
		*destination_write_io_handle = NULL;

		return( 1 );
	}
	*destination_write_io_handle = memory_allocate_structure(
	                                libewf_write_io_handle_t );

	if( *destination_write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination write IO handle.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_write_io_handle,
	     source_write_io_handle,
	     sizeof( libewf_write_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination write IO handle.",
		 function );

		goto on_error;
	}
	( *destination_write_io_handle )->case_data                  = NULL;
	( *destination_write_io_handle )->case_data_size             = 0;
	( *destination_write_io_handle )->device_information         = NULL;
	( *destination_write_io_handle )->device_information_size    = 0;
	( *destination_write_io_handle )->data_section               = NULL;
	( *destination_write_io_handle )->table_section_data         = NULL;
	( *destination_write_io_handle )->table_section_data_size    = 0;
	( *destination_write_io_handle )->table_entries_data         = NULL;
	( *destination_write_io_handle )->table_entries_data_size    = 0;
	( *destination_write_io_handle )->number_of_table_entries    = 0;
	( *destination_write_io_handle )->chunks_section             = NULL;
	( *destination_write_io_handle )->current_file_io_pool_entry = -1;
	( *destination_write_io_handle )->current_segment_file       = NULL;

	if( source_write_io_handle->case_data != NULL )
	{
		( *destination_write_io_handle )->case_data = (uint8_t *) memory_allocate(
		                                                           source_write_io_handle->case_data_size );

		if( ( *destination_write_io_handle )->case_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination case data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_write_io_handle )->case_data,
		     source_write_io_handle->case_data,
		     sizeof( ewf_data_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination case data.",
			 function );

			goto on_error;
		}
	}
	if( source_write_io_handle->device_information != NULL )
	{
		( *destination_write_io_handle )->device_information = (uint8_t *) memory_allocate(
		                                                                    source_write_io_handle->device_information_size );

		if( ( *destination_write_io_handle )->device_information == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination device information.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_write_io_handle )->device_information,
		     source_write_io_handle->device_information,
		     sizeof( ewf_data_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination device information.",
			 function );

			goto on_error;
		}
	}
	if( source_write_io_handle->data_section != NULL )
	{
		( *destination_write_io_handle )->data_section = memory_allocate_structure(
		                                                  ewf_data_t );

		if( ( *destination_write_io_handle )->data_section == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination data section.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_write_io_handle )->data_section,
		     source_write_io_handle->data_section,
		     sizeof( ewf_data_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination data section.",
			 function );

			goto on_error;
		}
	}
	if( source_write_io_handle->table_section_data != NULL )
	{
		( *destination_write_io_handle )->table_section_data = (uint8_t *) memory_allocate(
		                                                                    source_write_io_handle->table_section_data_size );

		if( ( *destination_write_io_handle )->table_section_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination table section data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_write_io_handle )->table_section_data,
		     source_write_io_handle->table_section_data,
		     source_write_io_handle->table_section_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination table section data.",
			 function );

			goto on_error;
		}
		( *destination_write_io_handle )->table_section_data_size = source_write_io_handle->table_section_data_size;

		( *destination_write_io_handle )->table_entries_data = ( *destination_write_io_handle )->table_section_data
		                                                     + ( *destination_write_io_handle )->table_header_size;

		( *destination_write_io_handle )->table_entries_data_size = source_write_io_handle->table_entries_data_size;
	}
	if( libcdata_array_clone(
	     &( ( *destination_write_io_handle )->chunks_section ),
	     source_write_io_handle->chunks_section,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_descriptor_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libewf_chunk_descriptor_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination chunks section.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_write_io_handle != NULL )
	{
		if( ( *destination_write_io_handle )->table_section_data != NULL )
		{
			memory_free(
			 ( *destination_write_io_handle )->table_section_data );
		}
		if( ( *destination_write_io_handle )->data_section != NULL )
		{
			memory_free(
			 ( *destination_write_io_handle )->data_section );
		}
		if( ( *destination_write_io_handle )->device_information != NULL )
		{
			memory_free(
			 ( *destination_write_io_handle )->device_information );
		}
		if( ( *destination_write_io_handle )->case_data != NULL )
		{
			memory_free(
			 ( *destination_write_io_handle )->case_data );
		}
		memory_free(
		 *destination_write_io_handle );

		*destination_write_io_handle = NULL;
	}
	return( -1 );
}

/* Initializes the write IO handle value to start writing
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize_values(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libcerror_error_t **error )
{
	uint8_t *compressed_zero_byte_empty_block = NULL;
	uint8_t *zero_byte_empty_block            = NULL;
	static char *function                     = "libewf_write_io_handle_initialize_values";
	void *reallocation                        = NULL;
	int64_t required_number_of_segments       = 0;
	int8_t compression_level                  = 0;
	int result                                = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( write_io_handle->values_initialized != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: write values were initialized and cannot be initialized anymore.",
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
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( ( media_values->chunk_size == 0 )
	 || ( media_values->chunk_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid media values - chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	/* Determine the maximum segment size
	 */
	if( segment_table->maximum_segment_size == 0 )
	{
		segment_table->maximum_segment_size = write_io_handle->maximum_segment_file_size;
	}
	write_io_handle->pack_flags = LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM;

	if( ( io_handle->format == LIBEWF_FORMAT_EWF )
	 || ( io_handle->format == LIBEWF_FORMAT_SMART ) )
	{
		write_io_handle->pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION;
	}
	else if( ( io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
	      || ( io_handle->format == LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 ) )
	{
		write_io_handle->pack_flags |= LIBEWF_PACK_FLAG_ADD_ALIGNMENT_PADDING;
	}
	if( io_handle->format == LIBEWF_FORMAT_V2_ENCASE7 )
	{
		write_io_handle->section_descriptor_size = sizeof( ewf_section_descriptor_v2_t );
		write_io_handle->table_header_size       = sizeof( ewf_table_header_v2_t );
		write_io_handle->table_entry_size        = sizeof( ewf_table_entry_v2_t );
	}
	else
	{
		write_io_handle->section_descriptor_size = sizeof( ewf_section_descriptor_v1_t );
		write_io_handle->table_header_size       = sizeof( ewf_table_header_v1_t );
		write_io_handle->table_entry_size        = sizeof( ewf_table_entry_v1_t );
	}
	if( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_UNDEFINED )
	{
		io_handle->segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1;
	}
	if( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		/* Leave space for the a table entry in the table section
		 */
		write_io_handle->chunk_table_entries_reserved_size = sizeof( ewf_table_entry_v1_t );

		/* Leave space for the table section descriptor
		 */
		write_io_handle->chunks_section_reserved_size = sizeof( ewf_section_descriptor_v1_t );
	}
	else if( io_handle->format == LIBEWF_FORMAT_ENCASE1 )
	{
		/* Leave space for the a table entry in the table section
		 */
		write_io_handle->chunk_table_entries_reserved_size = sizeof( ewf_table_entry_v1_t );

		/* Leave space for the table section descriptor and the table footer
		 */
		write_io_handle->chunks_section_reserved_size = sizeof( ewf_section_descriptor_v1_t ) + 4;
	}
	else if( ( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	      || ( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	{
		/* Leave space for the a table entry in the table and table2 sections
		 */
		write_io_handle->chunk_table_entries_reserved_size = 2 * sizeof( ewf_table_entry_v1_t );

		/* Leave space for the sectors, table and table2 section descriptors and the table and table2 footers
		 */
		write_io_handle->chunks_section_reserved_size = ( 3 * sizeof( ewf_section_descriptor_v1_t ) ) + ( 2 * 4 );
	}
	else
	{
		/* Leave space for the a table entry in the sector table section
		 */
		write_io_handle->chunk_table_entries_reserved_size = sizeof( ewf_table_entry_v2_t );

		/* Leave space for the sector data and sector table section descriptor and the sector table footer
		 */
		write_io_handle->chunks_section_reserved_size = sizeof( ewf_section_descriptor_v2_t ) + 16;
	}
	/* If no input write size was provided check if EWF file format allows for streaming
	 */
	if( media_values->media_size == 0 )
	{
		if( ( io_handle->format != LIBEWF_FORMAT_ENCASE2 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE3 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE4 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE5 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE6 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE7 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN5 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN6 )
		 && ( io_handle->format != LIBEWF_FORMAT_LINEN7 )
		 && ( io_handle->format != LIBEWF_FORMAT_LOGICAL_ENCASE5 )
		 && ( io_handle->format != LIBEWF_FORMAT_LOGICAL_ENCASE6 )
		 && ( io_handle->format != LIBEWF_FORMAT_LOGICAL_ENCASE7 )
		 && ( io_handle->format != LIBEWF_FORMAT_V2_ENCASE7 )
		 && ( io_handle->format != LIBEWF_FORMAT_FTK_IMAGER )
		 && ( io_handle->format != LIBEWF_FORMAT_EWFX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: format does not allow for streaming write.",
			 function );

			goto on_error;
		}
	}
	else
	{
		/* Determine the required number of segments allowed to write
		 */
		required_number_of_segments = (int64_t) media_values->media_size
		                            / (int64_t) segment_table->maximum_segment_size;

		if( required_number_of_segments > (int64_t) write_io_handle->maximum_number_of_segments )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: the maximum number of allowed segment files will be exceeded with the segment file size: %" PRIu64 ".",
			 function,
			 segment_table->maximum_segment_size );

			goto on_error;
		}
	}
	if( media_values->media_size > LIBEWF_2_TIB )
	{
/* TODO what about linen 7 */
		if( ( io_handle->format != LIBEWF_FORMAT_ENCASE6 )
		 && ( io_handle->format != LIBEWF_FORMAT_ENCASE7 )
		 && ( io_handle->format != LIBEWF_FORMAT_V2_ENCASE7 )
		 && ( io_handle->format != LIBEWF_FORMAT_EWFX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: format does not allow for a media size greater than 2 TiB.",
			 function );

			goto on_error;
		}
	}
	io_handle->chunk_size = media_values->chunk_size;

	if( ( write_io_handle->pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) == 0 )
	{
		if( write_io_handle->compressed_zero_byte_empty_block == NULL )
		{
			zero_byte_empty_block = (uint8_t *) memory_allocate(
			                                     sizeof( uint8_t ) * (size_t) media_values->chunk_size );

			if( zero_byte_empty_block == NULL )
			{	
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create zero byte empty block.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     zero_byte_empty_block,
			     0,
			     sizeof( uint8_t ) * (size_t) media_values->chunk_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear zero byte empty block.",
				 function );

				goto on_error;
			}
			write_io_handle->compressed_zero_byte_empty_block_size = 1024;

			compressed_zero_byte_empty_block = (uint8_t *) memory_allocate(
			                                                sizeof( uint8_t ) * write_io_handle->compressed_zero_byte_empty_block_size );

			if( compressed_zero_byte_empty_block == NULL )
			{	
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create compressed zero byte empty block.",
				 function );

				goto on_error;
			}
			compression_level = io_handle->compression_level;

			if( compression_level == LIBEWF_COMPRESSION_LEVEL_NONE )
			{
				compression_level = LIBEWF_COMPRESSION_LEVEL_DEFAULT;
			}
			result = libewf_compress_data(
				  compressed_zero_byte_empty_block,
				  &( write_io_handle->compressed_zero_byte_empty_block_size ),
				  io_handle->compression_method,
				  compression_level,
				  zero_byte_empty_block,
				  (size_t) media_values->chunk_size,
				  error );

			/* Check if the compressed buffer was too small
			 * and a new compressed data size buffer was passed back
			 */
			if( result == 0 )
			{
				if( write_io_handle->compressed_zero_byte_empty_block_size <= 1024 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid compressed string size value out of bounds.",
					 function );

					goto on_error;
				}
#if !defined( HAVE_COMPRESS_BOUND ) && !defined( WINAPI )
				/* The some version of zlib require a fairly large buffer
				 * if compressBound() was not used but the factor 2 use the chunk size instead
				 */
				write_io_handle->compressed_zero_byte_empty_block_size = media_values->chunk_size;

				/* For EWF-S01 in a worst case scenario the resulting chunk data is + 16 larger than the chunk size
				 */
				if( ( io_handle->format == LIBEWF_FORMAT_SMART )
				 || ( io_handle->format == LIBEWF_FORMAT_EWF ) )
				{
					write_io_handle->compressed_zero_byte_empty_block_size += 16;
				}
#endif
				reallocation = memory_reallocate(
				                compressed_zero_byte_empty_block,
				                sizeof( uint8_t ) * write_io_handle->compressed_zero_byte_empty_block_size );

				if( reallocation == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to resize compressed zero byte empty block.",
					 function );

					return( -1 );
				}
				compressed_zero_byte_empty_block = (uint8_t *) reallocation;

				result = libewf_compress_data(
					  compressed_zero_byte_empty_block,
					  &( write_io_handle->compressed_zero_byte_empty_block_size ),
					  io_handle->compression_method,
					  compression_level,
					  zero_byte_empty_block,
					  (size_t) media_values->chunk_size,
					  error );
			}
			if( result != 1 )
			{
				libcerror_error_free(
				 error );

				memory_free(
				 compressed_zero_byte_empty_block );

				write_io_handle->compressed_zero_byte_empty_block_size = 0;
			}
			else
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compressed zero byte empty block data:\n",
					 function );
					libcnotify_print_data(
					 compressed_zero_byte_empty_block,
					 write_io_handle->compressed_zero_byte_empty_block_size,
					 0 );
				}
#endif
				write_io_handle->compressed_zero_byte_empty_block = compressed_zero_byte_empty_block;
			}
			compressed_zero_byte_empty_block = NULL;

			memory_free(
			 zero_byte_empty_block );

			zero_byte_empty_block = NULL;
		}
	}
	write_io_handle->timestamp = time( NULL );

	/* Flag that the write values were initialized
	 */
	write_io_handle->values_initialized = 1;

	return( 1 );

on_error:
	if( compressed_zero_byte_empty_block != NULL )
	{
		memory_free(
		 compressed_zero_byte_empty_block );
	}
	if( zero_byte_empty_block != NULL )
	{
		memory_free(
		 zero_byte_empty_block );
	}
	return( -1 );
}

/* Initializes the write IO handle to resume writing
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_initialize_resume(
     libewf_write_io_handle_t *write_io_handle,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libewf_read_io_handle_t *read_io_handle,
     off64_t *current_offset,
     libcerror_error_t **error )
{
	libewf_section_descriptor_t *previous_section = NULL;
	libewf_section_descriptor_t *section          = NULL;
	libewf_segment_file_t *segment_file           = NULL;
	libfcache_cache_t *sections_cache             = NULL;
	static char *function                         = "libewf_write_io_handle_initialize_resume";
	size64_t segment_file_size                    = 0;
	size64_t storage_media_size                   = 0;
	size64_t unusable_storage_media_size          = 0;
	off64_t resume_segment_file_offset            = 0;
	uint64_t unusable_number_of_chunks            = 0;
	uint32_t number_of_segments                   = 0;
	uint32_t segment_number                       = 0;
	uint8_t backtrack_to_last_chunks_section      = 0;
	uint8_t reopen_segment_file                   = 0;
	int file_io_pool_entry                        = 0;
	int number_of_sections                        = 0;
	int previous_section_index                    = 0;
	int result                                    = 0;
	int section_index                             = 0;
	int supported_section                         = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
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
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid media values - missing chunk size.",
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
	if( current_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid current offset.",
		 function );

		return( -1 );
	}
	if( libfcache_cache_initialize(
	     &sections_cache,
	     LIBEWF_MAXIMUM_CACHE_ENTRIES_SECTIONS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sections cache.",
		 function );

		goto on_error;
	}
	if( libewf_segment_table_get_number_of_segments(
	     segment_table,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments.",
		 function );

		goto on_error;
	}
	if( number_of_segments == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of segments value out of bounds.",
		 function );

		goto on_error;
	}
	segment_number = number_of_segments - 1;

	if( libewf_segment_table_get_segment_file_by_index(
	     segment_table,
	     segment_number,
	     file_io_pool,
	     &segment_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file: %" PRIu32 " from segment table.",
		 function,
		 segment_number );

		goto on_error;
	}
	if( libewf_segment_file_get_number_of_sections(
	     segment_file,
	     &number_of_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of sections from segment file: %" PRIu32 ".",
		 function,
		 segment_number );

		goto on_error;
	}
	if( number_of_sections == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of sections value out of bounds.",
		 function );

		goto on_error;
	}
	section_index = number_of_sections - 1;
	
	if( libewf_segment_file_get_section_by_index(
	     segment_file,
	     section_index,
	     file_io_pool,
	     sections_cache,
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve section: %d from segment file: %" PRIu32 ".",
		 function,
		 section_index,
		 segment_number );

		goto on_error;
	}
	if( section == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing section: %d.",
		 function,
		 section_index );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( section->type_string_length > 0 )
		{
			libcnotify_printf(
			 "%s: last read section: %s.\n",
			 function,
			 (char *) section->type_string );
		}
		else if( section->type != 0 )
		{
			libcnotify_printf(
			 "%s: last read section: 0x%08" PRIx32 ".\n",
			 function,
			 section->type );
		}
	}
#endif
	if( section->type_string_length == 4 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "data",
		     4 ) == 0 )
		{
			if( segment_number == 0 )
			{
				backtrack_to_last_chunks_section = 1;
			}
		}
		else if( memory_compare(
			  (void *) section->type_string,
			  (void *) "hash",
			  4 ) == 0 )
		{
			backtrack_to_last_chunks_section = 1;
		}
	}
	else if( section->type_string_length == 5 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "xhash",
		     6 ) == 0 )
		{
			backtrack_to_last_chunks_section = 1;
		}
	}
	else if( section->type_string_length == 5 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "digest",
		     7 ) == 0 )
		{
			backtrack_to_last_chunks_section = 1;
		}
		else if( memory_compare(
			  (void *) section->type_string,
			  (void *) "error2",
			  7 ) == 0 )
		{
			backtrack_to_last_chunks_section = 1;
		}
	}
	else if( section->type_string_length == 7 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "session",
		     8 ) == 0 )
		{
			backtrack_to_last_chunks_section = 1;
		}
	}
	if( backtrack_to_last_chunks_section != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: backtracking to last chunks section.\n",
			 function );
		}
#endif
		while( section_index >= 0 )
		{
			if( libfdata_list_get_element_value_by_index(
			     segment_file->sections_list,
			     (intptr_t *) file_io_pool,
			     (libfdata_cache_t *) sections_cache,
			     section_index,
			     (intptr_t **) &section,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve section: %d from sections list.",
				 function,
				 section_index );

				goto on_error;
			}
			if( section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing section: %d.",
				 function,
				 section_index );

				goto on_error;
			}
			if( memory_compare(
			     (void *) section->type_string,
			     (void *) "table",
			     5 ) == 0 )
			{
				break;
			}
			section_index--;
		}
		if( section_index < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing last chunks section.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( section->type_string_length > 0 )
			{
				libcnotify_printf(
				 "%s: last chunks section: %s.\n",
				 function,
				 (char *) section->type_string );
			}
			else if( section->type != 0 )
			{
				libcnotify_printf(
				 "%s: last chunks section: 0x%08" PRIx32 ".\n",
				 function,
				 section->type );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	if( section->type_string_length == 4 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "data",
		     4 ) == 0 )
		{
			/* The sections containing the chunks and offsets were read entirely
			 * in the previous segment file
			 */
			supported_section                      = 1;
			reopen_segment_file                    = 1;
			resume_segment_file_offset             = section->end_offset;
			write_io_handle->create_chunks_section = 1;
		}
		else if( memory_compare(
			  (void *) section->type_string,
			  (void *) "done",
			  4 ) == 0 )
		{
			/* The segment file was read entirely
			 */
			supported_section = 1;
		}
		else if( memory_compare(
			  (void *) section->type_string,
			  (void *) "next",
			  4 ) == 0 )
		{
			/* The segment file was read entirely
			 */
			supported_section = 1;
		}
	}
	else if( section->type_string_length == 5 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "table",
		     6 ) == 0 )
		{
			/* Determine if the table section also contains chunks
			 */
			previous_section_index = section_index - 1;

			if( libfdata_list_get_element_value_by_index(
			     segment_file->sections_list,
			     (intptr_t *) file_io_pool,
			     (libfdata_cache_t *) sections_cache,
			     previous_section_index,
			     (intptr_t **) &previous_section,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous section: %d from sections list.",
				 function,
				 previous_section_index );

				goto on_error;
			}
			if( previous_section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing previous section: %d.",
				 function,
				 previous_section_index );

				goto on_error;
			}
			if( memory_compare(
			     (void *) previous_section->type_string,
			     (void *) "sectors",
			     8 ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported previous section: %s.",
				 function,
				 previous_section->type_string );

				goto on_error;
			}
			/* The sections containing the chunks and offsets were read partially
			 */
			section_index = previous_section_index;
			section       = previous_section;

			supported_section                      = 1;
			reopen_segment_file                    = 1;
			resume_segment_file_offset             = section->start_offset;
			write_io_handle->create_chunks_section = 1;
		}
	}
	else if( section->type_string_length == 6 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "table2",
		     6 ) == 0 )
		{
			/* Determine if the table section also contains chunks
			 */
			previous_section_index = section_index - 1;

			if( libfdata_list_get_element_value_by_index(
			     segment_file->sections_list,
			     (intptr_t *) file_io_pool,
			     (libfdata_cache_t *) sections_cache,
			     previous_section_index,
			     (intptr_t **) &previous_section,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous section: %d from sections list.",
				 function,
				 previous_section_index );

				goto on_error;
			}
			if( previous_section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing previous section: %d.",
				 function,
				 previous_section_index );

				goto on_error;
			}
			if( memory_compare(
			     (void *) previous_section->type_string,
			     (void *) "table",
			     6 ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported previous section: %s.",
				 function,
				 previous_section->type_string );

				goto on_error;
			}
			previous_section_index -= 1;

			if( libfdata_list_get_element_value_by_index(
			     segment_file->sections_list,
			     (intptr_t *) file_io_pool,
			     (libfdata_cache_t *) sections_cache,
			     previous_section_index,
			     (intptr_t **) &previous_section,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve previous section: %d from sections list.",
				 function,
				 previous_section_index );

				goto on_error;
			}
			if( previous_section == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing previous section: %d.",
				 function,
				 previous_section_index );

				goto on_error;
			}
			if( memory_compare(
			     (void *) previous_section->type_string,
			     (void *) "sectors",
			     8 ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported second previous section: %s.",
				 function,
				 previous_section->type_string );

				goto on_error;
			}
			/* The sections containing the chunks and offsets were read partially
			 */
			section_index = previous_section_index;
			section       = previous_section;

			supported_section                      = 1;
			reopen_segment_file                    = 1;
			resume_segment_file_offset             = section->start_offset;
			write_io_handle->create_chunks_section = 1;
		}
	}
	else if( section->type_string_length == 7 )
	{
		if( memory_compare(
		     (void *) section->type_string,
		     (void *) "sectors",
		     7 ) == 0 )
		{
			/* Uncertain if the sections containing the chunks was read entirely
			 * the offsets to the chunks are missing so the chunks need to be rewritten anyway
			 */
			supported_section                      = 1;
			reopen_segment_file                    = 1;
			resume_segment_file_offset             = section->start_offset;
			write_io_handle->create_chunks_section = 1;
		}
	}
	if( supported_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write resume from section: %s not supported.",
		 function,
		 (char *) section->type_string );

		goto on_error;
	}
	/* Set offset into media data
	 */
	*current_offset = (off64_t) read_io_handle->storage_media_size_read;

	/* Set write IO handle values
	 */
	write_io_handle->input_write_count        = (ssize64_t) read_io_handle->storage_media_size_read;
	write_io_handle->number_of_chunks_written = read_io_handle->number_of_chunks_read;
	write_io_handle->current_segment_number   = segment_number;
	write_io_handle->write_finalized          = 0;

	if( reopen_segment_file == 0 )
	{
		write_io_handle->current_segment_number += 1;
	}
	else
	{
		if( libewf_segment_table_get_segment_by_index(
		     segment_table,
		     segment_number,
		     &file_io_pool_entry,
		     &segment_file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %" PRIu32 " from segment table.",
			 function,
			 segment_number );

			goto on_error;
		}
		result = libewf_segment_table_get_segment_storage_media_size_by_index(
		          segment_table,
		          segment_number,
		          &storage_media_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %" PRIu32 " storage media size from segment table.",
			 function,
			 segment_number );

			goto on_error;
		}
		unusable_storage_media_size = segment_file->storage_media_size;
		unusable_number_of_chunks   = segment_file->number_of_chunks;

		if( libewf_segment_file_reopen(
		     segment_file,
		     section_index - 1,
		     file_io_pool,
		     file_io_pool_entry,
		     sections_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to reopen segment file: %" PRIu32 " for resume write.",
			 function,
			 segment_number );

			goto on_error;
		}
		/* Set segment file to the correct offset if write is resumed
		 */
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     resume_segment_file_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to seek resume segment file offset: %" PRIi64 " (0x%08" PRIx64 ") in segment file: %" PRIu32 ".",
			 function,
			 resume_segment_file_offset,
			 resume_segment_file_offset,
			 segment_number );

			goto on_error;
		}
		if( segment_file->storage_media_size > unusable_storage_media_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: segment file storage media size cannot be greater than unusable storage media size.",
			 function );

			goto on_error;
		}
		unusable_storage_media_size -= segment_file->storage_media_size;

		if( segment_file->number_of_chunks > unusable_number_of_chunks )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: segment file number of chunks cannot be greater than unusable number of chunks.",
			 function );

			goto on_error;
		}
		unusable_number_of_chunks -= segment_file->number_of_chunks;

		if( (size64_t) segment_file->current_offset > segment_table->maximum_segment_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: segment file current offset cannot be greater than maximum segment file size.",
			 function );

			goto on_error;
		}
		if( result != 0 )
		{
			/* The offset into media data need to be corrected if the read of the segment file was considered successful
			 */
			*current_offset -= unusable_storage_media_size;

			/* The write IO handle values need to be corrected if the read of the segment file was considered successful
			 */
			write_io_handle->input_write_count                       -= unusable_storage_media_size;
			write_io_handle->number_of_chunks_written                -= unusable_number_of_chunks;
			write_io_handle->remaining_segment_file_size              = segment_table->maximum_segment_size - segment_file->current_offset;
			write_io_handle->number_of_chunks_written_to_segment_file = segment_file->number_of_chunks;

			if( write_io_handle->input_write_count > 0 )
			{
				write_io_handle->current_segment_file = segment_file;
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: resuming write at offset: %" PRIi64 " (0x%08" PRIx64 ") with chunk: %" PRIu64 ".\n",
		 function,
		 write_io_handle->input_write_count,
		 write_io_handle->input_write_count,
		 write_io_handle->number_of_chunks_written );
	}
#endif
	if( libfcache_cache_free(
	     &sections_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sections cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sections_cache != NULL )
	{
		libfcache_cache_free(
		 &sections_cache,
		 NULL );
	}
	return( -1 );
}

/* Resize the table entries
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_resize_table_entries(
     libewf_write_io_handle_t *write_io_handle,
     uint32_t number_of_entries,
     libcerror_error_t **error )
{
	static char *function          = "libewf_write_io_handle_resize_table_entries";
	void *reallocation             = NULL;
	size_t table_entries_data_size = 0;
	size_t table_section_data_size = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( write_io_handle->table_entry_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid write IO handle - missing table entry size.",
		 function );

		return( -1 );
	}
	if( ( number_of_entries < write_io_handle->number_of_table_entries )
	 || ( (size_t) number_of_entries > ( (size_t) SSIZE_MAX / write_io_handle->table_entry_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		return( -1 );
	}
	table_entries_data_size = number_of_entries * write_io_handle->table_entry_size;

	if( table_entries_data_size > ( MEMORY_MAXIMUM_ALLOCATION_SIZE - write_io_handle->table_header_size - 16 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data size value out of bounds.",
		 function );

		return( -1 );
	}
	/* Reserve space for the header, entries and footer
	 */
	table_section_data_size = write_io_handle->table_header_size + table_entries_data_size + 16;

	reallocation = memory_reallocate(
			write_io_handle->table_section_data,
			table_section_data_size );

	if( reallocation == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to resize table section data.",
		 function );

		return( -1 );
	}
	write_io_handle->table_section_data      = (uint8_t *) reallocation;
	write_io_handle->table_section_data_size = table_section_data_size;
	write_io_handle->table_entries_data      = &( write_io_handle->table_section_data[ write_io_handle->table_header_size ] );
	write_io_handle->table_entries_data_size = table_entries_data_size;
	write_io_handle->number_of_table_entries = number_of_entries;

	return( 1 );
}

/* Calculates an estimate of the number of chunks that fit within a segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_calculate_chunks_per_segment_file(
     libewf_write_io_handle_t *write_io_handle,
     libewf_media_values_t *media_values,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error )
{
	static char *function                      = "libewf_write_io_handle_calculate_chunks_per_segment_file";
	int64_t calculated_chunks_per_segment_file = 0;
	int64_t maximum_chunks_per_segment_file    = 0;
	int64_t remaining_number_of_chunks         = 0;
	int64_t required_chunk_sections            = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( write_io_handle->maximum_chunks_per_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid write IO handle - missing maximum chunks per section.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
/* TODO check if media values -> number of chunks is in bounds */
	/* Calculate the maximum number of chunks within this segment file
	 */
	maximum_chunks_per_segment_file = write_io_handle->remaining_segment_file_size;

	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		maximum_chunks_per_segment_file /= media_values->chunk_size + 16;
	}
	else
	{
		maximum_chunks_per_segment_file /= media_values->chunk_size + 4;
	}
	/* Determine the number of required chunk sections
	 */
	required_chunk_sections = maximum_chunks_per_segment_file
	                        % write_io_handle->maximum_chunks_per_section;

	calculated_chunks_per_segment_file = write_io_handle->remaining_segment_file_size;

	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		/* Leave space for the chunk section descriptors
		 */
		calculated_chunks_per_segment_file -= required_chunk_sections
		                                    * sizeof( ewf_section_descriptor_v1_t );

		/* Leave space for the table entries data
		 */
		calculated_chunks_per_segment_file -= maximum_chunks_per_segment_file
		                                    * sizeof( ewf_table_entry_v1_t );
	}
	else if( format == LIBEWF_FORMAT_ENCASE1 )
	{
		/* Leave space for the chunk section descriptors and the offset table checksum
		 */
		calculated_chunks_per_segment_file -= required_chunk_sections
		                                    * ( sizeof( ewf_section_descriptor_v1_t ) + 4 );

		/* Leave space for the table entries data
		 */
		calculated_chunks_per_segment_file -= maximum_chunks_per_segment_file
		                                    * sizeof( ewf_table_entry_v1_t );
	}
	else if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	      || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	{
		/* Leave space for the chunk, table and table2 section descriptors and the table and table2 offset table checksums
		 */
		calculated_chunks_per_segment_file -= required_chunk_sections
		                                    * ( ( 3 * sizeof( ewf_section_descriptor_v1_t ) ) + ( 2 * 4 ) );

		/* Leave space for the table and table2 offsets
		 */
		calculated_chunks_per_segment_file -= 2 * maximum_chunks_per_segment_file
		                                    * sizeof( ewf_table_entry_v1_t );
	}
	else
	{
/* TODO EWF2 */
	}
	/* Calculate the number of chunks within this segment file
	 */
	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		/* The EWF-S01 format uses compression this will add 16 bytes on average
		 */
		calculated_chunks_per_segment_file /= media_values->chunk_size + 16;
	}
	else if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	      || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL ) )
	{
		/* The EWF-E01 format will used 4 bytes for an uncompressed chunk
		 * when the chunk cannot be compressed
		 */
		calculated_chunks_per_segment_file /= media_values->chunk_size + 4;
	}
	else
	{
/* TODO EWF2 */
	}
	/* If the input size is known determine the remaining number of chunks
	 */
	if( media_values->media_size > 0 )
	{
		remaining_number_of_chunks = (int64_t) media_values->number_of_chunks
		                           - (int64_t) write_io_handle->number_of_chunks_written;

		/* Check if less chunks remain than the number of chunks calculated
		 */
		if( remaining_number_of_chunks < calculated_chunks_per_segment_file )
		{
			calculated_chunks_per_segment_file = remaining_number_of_chunks;
		}
	}
	/* Make sure to return the total number of chunks per segment file
	 */
	calculated_chunks_per_segment_file += (int64_t) write_io_handle->number_of_chunks_written_to_segment_file;

	/* Fail safe segment should contain at least 1 chunk
	 */
	if( calculated_chunks_per_segment_file <= 0 )
	{
		calculated_chunks_per_segment_file = 1;
	}
	/* Fail safe no more than 2^32 values are allowed
	 */
	else if( calculated_chunks_per_segment_file > (int64_t) UINT32_MAX )
	{
		calculated_chunks_per_segment_file = UINT32_MAX;
	}
	write_io_handle->chunks_per_segment_file = (uint64_t) calculated_chunks_per_segment_file;

	return( 1 );
}

/* Calculates the number of chunks that fit within a (chunks) section
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_calculate_chunks_per_section(
     libewf_write_io_handle_t *write_io_handle,
     libcerror_error_t **error )
{
	static char *function              = "libewf_write_io_handle_calculate_chunks_per_section";
	int64_t remaining_number_of_chunks = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( write_io_handle->maximum_chunks_per_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid write IO handle - missing maximum chunks per section.",
		 function );

		return( -1 );
	}
	if( write_io_handle->number_of_chunks_written_to_segment_file > write_io_handle->chunks_per_segment_file )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of chunks written to segment exceeds number of chunks per segment file.",
		 function );

		return( -1 );
	}
        remaining_number_of_chunks = (int64_t) write_io_handle->chunks_per_segment_file
	                           - (int64_t) write_io_handle->number_of_chunks_written_to_segment_file;

	if( remaining_number_of_chunks > (int64_t) write_io_handle->maximum_chunks_per_section )
	{
		remaining_number_of_chunks = (int64_t) write_io_handle->maximum_chunks_per_section;
	}
	/* Fail safe no more than 2^31 values are allowed
	 */
	if( remaining_number_of_chunks > (int64_t) INT32_MAX )
	{
		remaining_number_of_chunks = INT32_MAX;
	}
	write_io_handle->chunks_per_section = (uint32_t) remaining_number_of_chunks;

	return( 1 );
}

/* Tests if the current segment file is full
 * Returns 1 if full, 0 if not or -1 on error
 */
int libewf_write_io_handle_test_segment_file_full(
     libewf_write_io_handle_t *write_io_handle,
     libewf_media_values_t *media_values,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_test_segment_file_full";

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
	/* Check if the maximum number of chunks has been reached
	 */
	if( ( media_values->number_of_chunks != 0 )
	 && ( media_values->number_of_chunks == write_io_handle->number_of_chunks_written ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required chunks have been written.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( media_values->media_size != 0 )
	 && ( write_io_handle->input_write_count >= (ssize64_t) media_values->media_size ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required data has been written.\n",
			 function );
		}
#endif

		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format should use the pre calculated size
	 */
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( write_io_handle->number_of_chunks_written_to_segment_file >= write_io_handle->chunks_per_segment_file )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
			 	"%s: no space left for additional chunk.\n",
				 function );
			}
#endif
			return( 1 );
		}
	}
	/* Determine if a chunk would fit in the segment file
	 */
	else if( write_io_handle->remaining_segment_file_size < (ssize64_t) ( media_values->chunk_size + 4 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - file size exceeded.\n",
			 function );
		}
#endif

		return( 1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: space left for additional chunk.\n",
		 function );
	}
#endif

	return( 0 );
}

/* Tests if the current chunks section is full
 * Returns 1 if full, 0 if not or -1 on error
 */
int libewf_write_io_handle_test_chunks_section_full(
     libewf_write_io_handle_t *write_io_handle,
     libewf_media_values_t *media_values,
     off64_t segment_file_offset,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_test_chunks_section_full";

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( media_values->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid media values - missing chunk size.",
		 function );

		return( -1 );
	}
	if( write_io_handle->maximum_chunks_per_section == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid write IO handle - missing maximum chunks per section.",
		 function );

		return( -1 );
	}
	if( segment_file_offset > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment file offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Check if a chunks section has been opened
	 */
	if( write_io_handle->chunks_section_offset == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no chunks section has been created.\n",
			 function );
		}
#endif
		return( 0 );
	}
	/* Check if the maximum number of chunks has been reached
	 */
	if( ( media_values->number_of_chunks != 0 )
	 && ( media_values->number_of_chunks == write_io_handle->number_of_chunks_written ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required chunks have been written.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Check if the end of the input has been reached
	*/
	if( ( media_values->media_size != 0 )
	 && ( write_io_handle->input_write_count >= (ssize64_t) media_values->media_size ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: all required data has been written.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( write_io_handle->number_of_chunks_written_to_section >= write_io_handle->maximum_chunks_per_section )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - maximum reached.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* No more than ( 2^31 / 4 ) chunks are allowed
	 */
	if( write_io_handle->number_of_chunks_written_to_section > (uint32_t) ( INT32_MAX / 4 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - preventing chunk overflow.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Prevent offset overflow
	 */
	if( ( segment_file_offset - write_io_handle->chunks_section_offset ) > (off64_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: no space left for additional chunk - preventing offset overflow.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* The EWF-S01 and EnCase1 format do not allow for a growth of the offset table
	 */
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 || ( format == LIBEWF_FORMAT_ENCASE1 ) )
	{
		if( write_io_handle->number_of_chunks_written_to_section >= write_io_handle->chunks_per_section )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: no space left for additional chunk.\n",
				 function );
			}
#endif
			return( 1 );
		}
	}
	/* Determine if a chunk would fit in the segment file
	 */
	else if( write_io_handle->remaining_segment_file_size < (ssize64_t) ( media_values->chunk_size + 4 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
		 	"%s: no space left for additional chunk - file size exceeded.\n",
			 function );
		}
#endif
		return( 1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: space left for additional chunk.\n",
		 function );
	}
#endif
	return( 0 );
}

/* Creates a new segment file and opens it for writing
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_create_segment_file(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_segment_table_t *segment_table,
     uint8_t segment_file_type,
     uint32_t segment_number,
     uint32_t maximum_number_of_segments,
     const uint8_t *set_identifier,
     int *file_io_pool_entry,
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle         = NULL;
	libewf_segment_file_t *safe_segment_file = NULL;
	system_character_t *filename             = NULL;
	static char *function                    = "libewf_write_io_handle_create_segment_file";
	size_t filename_size                     = 0;
	int bfio_access_flags                    = 0;
	int safe_file_io_pool_entry              = 0;

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
	if( file_io_pool_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool entry.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( *segment_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment file - value already set.",
		 function );

		return( -1 );
	}
	if( libewf_filename_create(
	     &filename,
	     &filename_size,
	     segment_table->basename,
	     segment_table->basename_size - 1,
	     segment_number + 1,
	     maximum_number_of_segments,
	     segment_file_type,
	     io_handle->format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file: %" PRIu32 " filename.",
		 function,
		 segment_number );

		goto on_error;
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing filename.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: creating segment file: %" PRIu32 " with filename: %" PRIs_SYSTEM ".\n",
		 function,
		 segment_number,
		 filename );
	}
#endif
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
#else
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	memory_free(
	 filename );

	filename = NULL;

	bfio_access_flags = LIBBFIO_OPEN_WRITE_TRUNCATE;

	if( libbfio_pool_append_handle(
	     file_io_pool,
	     &safe_file_io_pool_entry,
	     file_io_handle,
	     bfio_access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append file IO handle to pool.",
		 function );

		goto on_error;
	}
	file_io_handle = NULL;

	if( libbfio_pool_open(
	     file_io_pool,
	     safe_file_io_pool_entry,
	     bfio_access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file IO pool entry: %d.",
		 function,
		 safe_file_io_pool_entry );

		goto on_error;
	}
	if( libewf_segment_file_initialize(
	     &safe_segment_file,
	     io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file.",
		 function );

		goto on_error;
	}
	safe_segment_file->type           = segment_file_type;
	safe_segment_file->segment_number = segment_number + 1;
	safe_segment_file->flags         |= LIBEWF_SEGMENT_FILE_FLAG_WRITE_OPEN;

	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART ) )
	{
		safe_segment_file->major_version      = 1;
		safe_segment_file->minor_version      = 0;
		safe_segment_file->compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
	}
	else if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	      || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		safe_segment_file->major_version      = io_handle->major_version;
		safe_segment_file->minor_version      = io_handle->minor_version;
		safe_segment_file->compression_method = io_handle->compression_method;

		if( memory_copy(
		     safe_segment_file->set_identifier,
		     set_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy set identifier.",
			 function );

			goto on_error;
		}
	}
	if( libewf_segment_table_append_segment_by_segment_file(
	     segment_table,
	     safe_segment_file,
	     safe_file_io_pool_entry,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment: %" PRIu32 " to segment table.",
		 function,
		 segment_number );

		goto on_error;
	}
	*file_io_pool_entry = safe_file_io_pool_entry;
	*segment_file       = safe_segment_file;

	return( 1 );

on_error:
	if( safe_segment_file != NULL )
	{
		libewf_segment_file_free(
		 &safe_segment_file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( filename != NULL )
	{
		memory_free(
		 filename );
	}
	return( -1 );
}

/* Generates the table entries data from the chunks list
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_generate_table_entries_data(
     libewf_write_io_handle_t *write_io_handle,
     uint64_t chunk_index,
     uint8_t format_version,
     uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint32_t number_of_entries,
     off64_t base_offset,
     libcerror_error_t **error )
{
	libewf_chunk_descriptor_t *chunk_descriptor = NULL;
	static char *function                       = "libewf_write_io_handle_generate_table_entries_data";
	size_t data_offset                          = 0;
	size_t table_entry_data_size                = 0;
	uint32_t table_entry_index                  = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( format_version == 1 )
	{
		table_entry_data_size = sizeof( ewf_table_entry_v1_t );
	}
	else if( format_version == 2 )
	{
		table_entry_data_size = sizeof( ewf_table_entry_v2_t );
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
	if( number_of_entries > ( table_entries_data_size / table_entry_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data size value out of bounds.",
		 function );

		return( -1 );
	}
	for( table_entry_index = 0;
	     table_entry_index < number_of_entries;
	     table_entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     write_io_handle->chunks_section,
		     table_entry_index,
		     (intptr_t **) &chunk_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk descriptor: %" PRIu32 " from array.",
			 function,
			 table_entry_index );

			return( -1 );
		}
		if( libewf_chunk_descriptor_write_data(
		     chunk_descriptor,
		     &( table_entries_data[ data_offset ] ),
		     table_entry_data_size,
		     base_offset,
		     format_version,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write chunk descriptor: %" PRIu32 ".",
			 function,
			 table_entry_index );

			return( -1 );
		}
		data_offset += table_entry_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk\t\t\t: %" PRIu64 "\n",
			 function,
			 table_entry_index,
			 chunk_index );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " base offset\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 table_entry_index,
			 base_offset,
			 base_offset );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data offset\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 table_entry_index,
			 chunk_descriptor->data_offset,
			 chunk_descriptor->data_offset );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data size\t\t: %" PRIu32 "\n",
			 function,
			 table_entry_index,
			 chunk_descriptor->data_size );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data flags:\n",
			 function,
			 table_entry_index );

			if( ( chunk_descriptor->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0  )
			{
				libcnotify_printf(
				 "\tIs compressed\n" );
			}
			if( ( chunk_descriptor->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
			{
				libcnotify_printf(
				 "\tHas checksum\n" );
			}
			libcnotify_printf(
			 "\n" );
		}
		chunk_index++;

#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	return( 1 );
}

/* Writes the start of the chunks section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_write_io_handle_write_chunks_section_start(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_write_chunks_section_start";
	ssize_t write_count   = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
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
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	if( ( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	 || ( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	 || ( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART ) )
	{
		if( write_io_handle->number_of_table_entries < write_io_handle->chunks_per_section )
		{
			if( libewf_write_io_handle_resize_table_entries(
			     write_io_handle,
			     write_io_handle->chunks_per_section,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to resize table entries.",
				 function );

				return( -1 );
			}
		}
		if( memory_set(
		     write_io_handle->table_section_data,
		     0,
		     write_io_handle->table_section_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear table section data.",
			 function );

			return( -1 );
		}
		/* Write the section descriptor of the chunks section
		 */
		write_count = libewf_segment_file_write_chunks_section_start(
			       segment_file,
			       file_io_pool,
			       file_io_pool_entry,
			       write_io_handle->table_section_data,
			       write_io_handle->table_section_data_size,
			       write_io_handle->table_entries_data,
			       write_io_handle->table_entries_data_size,
			       write_io_handle->chunks_per_section,
			       write_io_handle->number_of_chunks_written,
			       error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write chunks section start.",
			 function );

			return( -1 );
		}
	}
	return( write_count );
}

/* Writes the end of the chunks section
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_write_io_handle_write_chunks_section_end(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         libcerror_error_t **error )
{
	static char *function      = "libewf_write_io_handle_write_chunks_section_end";
	off64_t base_offset        = 0;
	ssize_t write_count        = 0;
	uint64_t first_chunk_index = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
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
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: closing chunks section number of bytes written: %" PRIi64 " (padding: %" PRIu32 ").\n",
		 function,
		 write_io_handle->chunks_section_write_count,
		 write_io_handle->chunks_section_padding_size );
	}
#endif
	if( write_io_handle->number_of_table_entries < write_io_handle->number_of_chunks_written_to_section )
	{
		if( libewf_write_io_handle_resize_table_entries(
		     write_io_handle,
		     write_io_handle->number_of_chunks_written_to_section,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize table entries.",
			 function );

			return( -1 );
		}
	}
/* TODO what about linen 7 */
	if( ( io_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( io_handle->format == LIBEWF_FORMAT_ENCASE7 ) )
	{
		base_offset = write_io_handle->chunks_section_offset;
	}
	first_chunk_index = write_io_handle->number_of_chunks_written
	                  - write_io_handle->number_of_chunks_written_to_section;

	if( libewf_write_io_handle_generate_table_entries_data(
	     write_io_handle,
	     first_chunk_index,
	     segment_file->major_version,
	     write_io_handle->table_entries_data,
	     write_io_handle->table_entries_data_size,
	     write_io_handle->number_of_chunks_written_to_section,
	     base_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set table entries data.",
		 function );

		return( -1 );
	}
	write_count = libewf_segment_file_write_chunks_section_end(
		       segment_file,
		       file_io_pool,
		       file_io_pool_entry,
		       write_io_handle->table_section_data,
		       write_io_handle->table_section_data_size,
		       write_io_handle->table_entries_data,
		       write_io_handle->table_entries_data_size,
		       write_io_handle->number_of_chunks_written_to_section,
		       write_io_handle->chunks_section_offset,
		       (size64_t) write_io_handle->chunks_section_write_count,
		       write_io_handle->chunks_section_padding_size,
		       first_chunk_index,
		       base_offset,
		       error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunks section end.",
		 function );

		return( -1 );
	}
	if( libcdata_array_empty(
	     write_io_handle->chunks_section,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to empty chunks section.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Creates a new segment file and opens it for writing
 * The necessary sections at the start of the segment file are written
 * Returns the number of bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_io_handle_write_new_chunk_create_segment_file(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libewf_segment_table_t *segment_table,
         libfvalue_table_t *header_values,
         uint32_t segment_number,
         int *file_io_pool_entry,
         libewf_segment_file_t **segment_file,
         libcerror_error_t **error )
{
	libewf_segment_file_t *safe_segment_file = NULL;
	static char *function                    = "libewf_write_io_handle_write_new_chunk_create_segment_file";
	ssize_t write_count                      = 0;
	int safe_file_io_pool_entry              = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
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
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( file_io_pool_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool entry.",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	write_io_handle->create_chunks_section                    = 1;
	write_io_handle->chunks_per_section                       = 0;
	write_io_handle->number_of_chunks_written_to_segment_file = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: creating segment file: %" PRIu32 ".\n",
		 function,
		 segment_number );
	}
#endif
	if( libewf_write_io_handle_create_segment_file(
	     io_handle,
	     file_io_pool,
	     segment_table,
	     io_handle->segment_file_type,
	     segment_number,
	     write_io_handle->maximum_number_of_segments,
	     media_values->set_identifier,
	     &safe_file_io_pool_entry,
	     &safe_segment_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to create segment file: %" PRIu32 ".",
		 function,
		 segment_number );

		goto on_error;
	}
	write_io_handle->remaining_segment_file_size = segment_table->maximum_segment_size;

	/* Reserve space for the done or next section
	 */
	write_io_handle->remaining_segment_file_size -= write_io_handle->section_descriptor_size;

	/* Write the start of the segment file
	 * like the file header, the header, volume and/or data section, etc.
	 */
	write_count = libewf_segment_file_write_start(
	               safe_segment_file,
	               file_io_pool,
	               safe_file_io_pool_entry,
	               &( write_io_handle->case_data ),
	               &( write_io_handle->case_data_size ),
	               &( write_io_handle->device_information ),
	               &( write_io_handle->device_information_size ),
	               &( write_io_handle->data_section ),
	               media_values,
	               header_values,
	               write_io_handle->timestamp,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write segment file start.",
		 function );

		goto on_error;
	}
	write_io_handle->remaining_segment_file_size -= write_count;

	/* Determine the number of chunks per segment file
	 */
	if( safe_segment_file->number_of_chunks == 0 )
	{
		if( libewf_write_io_handle_calculate_chunks_per_segment_file(
		     write_io_handle,
		     media_values,
		     io_handle->segment_file_type,
		     io_handle->format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of chunks per segment file.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: calculated number of chunks per segment file: %" PRIu64 ".\n",
			 function,
			 write_io_handle->chunks_per_segment_file );
		}
#endif
	}
	else
	{
		write_io_handle->chunks_per_segment_file = safe_segment_file->number_of_chunks;
	}
	*file_io_pool_entry = safe_file_io_pool_entry;
	*segment_file       = safe_segment_file;

	return( write_count );

on_error:
	if( safe_segment_file != NULL )
	{
		libewf_segment_file_free(
		 &safe_segment_file,
		 NULL );
	}
	return( -1 );
}

/* Creates a new chunks section
 * Returns the number of bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_io_handle_write_new_chunk_create_chunks_section(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         libcerror_error_t **error )
{
	static char *function = "libewf_write_io_handle_write_new_chunk_create_chunks_section";
	ssize_t write_count   = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
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
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		"%s: creating chunks section.\n",
		 function );
	}
#endif
	write_io_handle->create_chunks_section               = 0;
	write_io_handle->number_of_chunks_written_to_section = 0;
	write_io_handle->chunks_section_write_count          = 0;
	write_io_handle->chunks_section_padding_size         = 0;

	/* Reserve space in the segment file for the end of the chunks section
	 */
	write_io_handle->remaining_segment_file_size -= write_io_handle->chunks_section_reserved_size;

	if( libbfio_pool_get_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     &( write_io_handle->chunks_section_offset ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in segment file.",
		 function );

		return( -1 );
	}
	/* Recalculate the number of chunks per segment file for a better fill when compression is used
	 */
	if( segment_file->number_of_chunks == 0 )
	{
		if( libewf_write_io_handle_calculate_chunks_per_segment_file(
		     write_io_handle,
		     media_values,
		     io_handle->segment_file_type,
		     io_handle->format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of chunks per segment file.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: calculated number of chunks per segment file: %" PRIu64 ".\n",
			 function,
			 write_io_handle->chunks_per_segment_file );
		}
#endif
	}
	else
	{
		write_io_handle->chunks_per_segment_file = segment_file->number_of_chunks;
	}
	if( libewf_write_io_handle_calculate_chunks_per_section(
	     write_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine the number of chunks per chunks section.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		"%s: calculated number of chunks per section: %" PRIu32 ".\n",
		 function,
		 write_io_handle->chunks_per_section );
	}
#endif
	write_count = libewf_write_io_handle_write_chunks_section_start(
		       write_io_handle,
		       io_handle,
		       file_io_pool,
		       file_io_pool_entry,
		       segment_file,
		       error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunks section start.",
		 function );

		return( -1 );
	}
	write_io_handle->remaining_segment_file_size -= write_count;

	return( write_count );
}

/* Creates a new chunks section
 * Returns the number of bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_io_handle_write_new_chunk_create_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_segment_file_t *segment_file,
         uint64_t chunk_index,
         libewf_chunk_data_t *chunk_data,
         size_t input_data_size,
         libcerror_error_t **error )
{
	libewf_chunk_descriptor_t *chunk_descriptor = NULL;
	static char *function                       = "libewf_write_io_handle_write_new_chunk_create_chunk";
	ssize_t write_count                         = 0;
	off64_t chunk_offset                        = 0;
	int entry_index                             = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( segment_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	chunk_offset = write_io_handle->current_segment_file->current_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: writing chunk: %" PRIu64 " data of size: %" PRIzd " to segment file: %" PRIu32 " at offset: 0x%08" PRIx64 ".\n",
		 function,
		 chunk_index,
		 chunk_data->data_size,
		 segment_file->segment_number,
		 chunk_offset );
	}
#endif
	write_count = libewf_segment_file_write_chunk_data(
		       segment_file,
		       file_io_pool,
		       file_io_pool_entry,
		       chunk_index,
		       chunk_data,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		goto on_error;
	}
	if( libewf_chunk_descriptor_initialize(
	     &chunk_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk descriptor.",
		 function );

		goto on_error;
	}
	chunk_descriptor->data_offset = chunk_offset;
	chunk_descriptor->data_size   = (size64_t) write_count - chunk_data->padding_size;
	chunk_descriptor->range_flags = chunk_data->range_flags;

	if( libcdata_array_append_entry(
	     write_io_handle->chunks_section,
	     &entry_index,
	     (intptr_t *) chunk_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append chunk: %" PRIu64 " descriptor to chunks section.",
		 function,
		 chunk_index );

		goto on_error;
	}
	chunk_descriptor = NULL;

	write_io_handle->input_write_count                        += input_data_size;
	write_io_handle->chunks_section_write_count               += write_count;
	write_io_handle->chunks_section_padding_size              += (uint32_t) chunk_data->padding_size;
	write_io_handle->remaining_segment_file_size              -= write_count;
	write_io_handle->number_of_chunks_written_to_segment_file += 1;
	write_io_handle->number_of_chunks_written_to_section      += 1;
	write_io_handle->number_of_chunks_written                 += 1;

	return( write_count );

on_error:
	if( chunk_descriptor != NULL )
	{
		libewf_chunk_descriptor_free(
		 &chunk_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Writes a new chunk of data in EWF format at the current offset
 * The necessary settings of the write values must have been made
 * Returns the number of bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_write_io_handle_write_new_chunk(
         libewf_write_io_handle_t *write_io_handle,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         libewf_media_values_t *media_values,
         libewf_segment_table_t *segment_table,
         libfvalue_table_t *header_values,
         libfvalue_table_t *hash_values,
         libewf_hash_sections_t *hash_sections,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcdata_range_list_t *acquiry_errors,
         uint64_t chunk_index,
         libewf_chunk_data_t *chunk_data,
         size_t input_data_size,
         libcerror_error_t **error )
{
	static char *function     = "libewf_write_io_handle_write_new_chunk";
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;
	int result                = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
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
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( ( input_data_size == 0 )
	 && ( input_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid input data size value out of bounds.",
		 function );

		return( -1 );
	}
	/* Check if the write was already finalized
	 */
	if( write_io_handle->write_finalized != 0 )
	{
		return( 0 );
	}
	/* Check if the number of bytes as specified have been written
	 */
	if( ( media_values->media_size != 0 )
	 && ( write_io_handle->input_write_count >= (ssize64_t) media_values->media_size ) )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: writing chunk: %" PRIu64 " of size: %" PRIzd " (data size: %" PRIzd ").\n",
		 function,
		 chunk_index,
		 chunk_data->data_size,
		 input_data_size );
	}
#endif
	if( write_io_handle->current_segment_file == NULL )
	{
		write_count = libewf_write_io_handle_write_new_chunk_create_segment_file(
			       write_io_handle,
			       io_handle,
			       file_io_pool,
			       media_values,
			       segment_table,
			       header_values,
			       write_io_handle->current_segment_number,
			       &( write_io_handle->current_file_io_pool_entry ),
			       &( write_io_handle->current_segment_file ),
			       error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to create segment file: %" PRIu32 ".",
			 function,
			 write_io_handle->current_segment_number );

			return( -1 );
		}
		total_write_count += write_count;
	}
	/* Check if a chunks section should be created
	 */
	if( write_io_handle->create_chunks_section == 1 )
	{
		write_count = libewf_write_io_handle_write_new_chunk_create_chunks_section(
		               write_io_handle,
		               io_handle,
		               file_io_pool,
		               media_values,
		               write_io_handle->current_file_io_pool_entry,
		               write_io_handle->current_segment_file,
		               error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to create chunks section in segment file: %" PRIu32 ".",
			 function,
			 write_io_handle->current_segment_number );

			return( -1 );
		}
		total_write_count += write_count;
	}
	/* Write the chunk data
	 */
	write_count = libewf_write_io_handle_write_new_chunk_create_chunk(
	               write_io_handle,
	               file_io_pool,
	               write_io_handle->current_file_io_pool_entry,
	               write_io_handle->current_segment_file,
	               chunk_index,
	               chunk_data,
	               input_data_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to create chunk: %" PRIu64 " in segment file: %" PRIu32 ".",
		 function,
		 chunk_index,
		 write_io_handle->current_segment_number );

		return( -1 );
	}
	total_write_count += write_count;

	/* Reserve space in the segment file for the chunk table entries
	 */
	write_io_handle->remaining_segment_file_size -= write_io_handle->chunk_table_entries_reserved_size;

	/* Check if the current chunks section is full, if so close the current section
	 */
	result = libewf_write_io_handle_test_chunks_section_full(
		  write_io_handle,
		  media_values,
		  write_io_handle->current_segment_file->current_offset,
		  io_handle->segment_file_type,
		  io_handle->format,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if chunks section is full.",
		 function );

		return( -1 );
	}
	if( result == 1 )
	{
		write_count = libewf_write_io_handle_write_chunks_section_end(
			       write_io_handle,
			       io_handle,
			       file_io_pool,
			       write_io_handle->current_file_io_pool_entry,
			       write_io_handle->current_segment_file,
			       error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write chunks section end.",
			 function );

			return( -1 );
		}
		total_write_count += write_count;

		write_io_handle->create_chunks_section = 1;
		write_io_handle->chunks_section_offset = 0;

		/* Check if the current segment file is full, if so close the current segment file
		 */
		result = libewf_write_io_handle_test_segment_file_full(
			  write_io_handle,
			  media_values,
			  io_handle->segment_file_type,
			  io_handle->format,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if segment file is full.",
			 function );

			return( -1 );
		}
		else if( result == 1 )
		{
			/* Check if this is not the last segment file
			 */
			if( ( media_values->media_size == 0 )
			 || ( write_io_handle->input_write_count < (ssize64_t) media_values->media_size ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
				 	"%s: closing segment file: %" PRIu32 ".\n",
					 function,
					 write_io_handle->current_segment_number );
				}
#endif
				/* Finalize and close the segment file
				 */
				write_count = libewf_segment_file_write_close(
					       write_io_handle->current_segment_file,
					       file_io_pool,
					       write_io_handle->current_file_io_pool_entry,
					       write_io_handle->number_of_chunks_written_to_segment_file,
					       0,
					       hash_sections,
					       hash_values,
					       media_values,
					       sessions,
					       tracks,
					       acquiry_errors,
					       &( write_io_handle->data_section ),
				               error );

				if( write_count < 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to close segment file: %" PRIu32 ".",
					 function,
					 write_io_handle->current_segment_number );

					return( -1 );
				}
				total_write_count += write_count;

				write_io_handle->current_file_io_pool_entry = -1;
				write_io_handle->current_segment_file       = NULL;
				write_io_handle->current_segment_number    += 1;
			}
		}
	}
	return( total_write_count );
}

/* Corrects sections after streamed write
 * Returns 1 if successful or -1 on error
 */
int libewf_write_io_handle_finalize_write_sections_corrections(
     libewf_write_io_handle_t *write_io_handle,
     libbfio_pool_t *file_io_pool,
     libewf_media_values_t *media_values,
     libewf_segment_table_t *segment_table,
     libfvalue_table_t *header_values,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     libcdata_range_list_t *acquiry_errors,
     libcerror_error_t **error )
{
	libewf_segment_file_t *segment_file = NULL;
	static char *function               = "libewf_write_io_handle_finalize_write_sections_corrections";
	size64_t segment_file_size          = 0;
	uint32_t number_of_segments         = 0;
	uint32_t segment_number             = 0;
	int file_io_pool_entry              = 0;
	int last_segment_file               = 0;

	if( write_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write IO handle.",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_get_number_of_segments(
	     segment_table,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments from segment table.",
		 function );

		return( -1 );
	}
	for( segment_number = 0;
	     segment_number < number_of_segments;
	     segment_number++ )
	{
		if( segment_number == ( number_of_segments - 1 ) )
		{
			last_segment_file = 1;
		}
		if( libewf_segment_table_get_segment_by_index(
		     segment_table,
		     segment_number,
		     &file_io_pool_entry,
		     &segment_file_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %" PRIu32 " from segment table.",
			 function,
			 segment_number );

			return( -1 );
		}
		if( libbfio_pool_reopen(
		     file_io_pool,
		     file_io_pool_entry,
		     LIBBFIO_OPEN_READ_WRITE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to reopen segment file: %" PRIu32 " for write corrections.",
			 function,
			 segment_number );

			return( -1 );
		}
		segment_file = NULL;

		if( libewf_segment_table_get_segment_file_by_index(
		     segment_table,
		     segment_number,
		     file_io_pool,
		     &segment_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file: %" PRIu32 " from segment table.",
			 function,
			 segment_number );

			return( -1 );
		}
		if( libewf_segment_file_write_sections_correction(
		     segment_file,
		     file_io_pool,
		     file_io_pool_entry,
		     write_io_handle->number_of_chunks_written_to_segment_file,
		     last_segment_file,
		     media_values,
		     header_values,
		     write_io_handle->timestamp,
		     hash_values,
		     hash_sections,
		     sessions,
		     tracks,
		     acquiry_errors,
		     &( write_io_handle->case_data ),
		     &( write_io_handle->case_data_size ),
		     &( write_io_handle->device_information ),
		     &( write_io_handle->device_information_size ),
		     &( write_io_handle->data_section ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write sections correction to segment file: %" PRIu32 ".",
			 function,
			 segment_number );

			return( -1 );
		}
	}
	return( 1 );
}


/*
 * Chunk table functions
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libmfdata.h"
#include "libewf_list_type.h"
#include "libewf_section.h"
#include "libewf_unused.h"

#include "ewf_checksum.h"
#include "ewf_definitions.h"
#include "ewf_table.h"

/* Initialize the chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_initialize(
     libewf_chunk_table_t **chunk_table,
     libewf_io_handle_t *io_handle,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_table_initialize";

	if( chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *chunk_table == NULL )
	{
		*chunk_table = memory_allocate_structure(
		                libewf_chunk_table_t );

		if( *chunk_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create chunk table.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *chunk_table,
		     0,
		     sizeof( libewf_chunk_table_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear chunk table.",
			 function );

			goto on_error;
		}
		( *chunk_table )->io_handle = io_handle;
	}
	return( 1 );

on_error:
	if( *chunk_table != NULL )
	{
		memory_free(
		 *chunk_table );

		*chunk_table = NULL;
	}
	return( -1 );
}

/* Frees the chunk table including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_free(
     intptr_t *chunk_table,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_table_free";

	if( chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	memory_free(
	 chunk_table );

	return( 1 );
}

/* Clones the chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_clone(
     intptr_t **destination_chunk_table,
     intptr_t *source_chunk_table,
     liberror_error_t **error )
{
	static char *function = "libewf_chunk_table_clone";

	if( destination_chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination chunk table.",
		 function );

		return( -1 );
	}
	if( *destination_chunk_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination chunk table value already set.",
		 function );

		return( -1 );
	}
	if( source_chunk_table == NULL )
	{
		*destination_chunk_table = NULL;

		return( 1 );
	}
	*destination_chunk_table = memory_allocate_structure_as_value(
	                            libewf_chunk_table_t );

	if( *destination_chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination chunk table.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_chunk_table,
	     source_chunk_table,
	     sizeof( libewf_chunk_table_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination chunk table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_chunk_table != NULL )
	{
		memory_free(
		 *destination_chunk_table );

		*destination_chunk_table = NULL;
	}
	return( -1 );
}

/* Reads a chunk
 * Callback function for the chunk table list
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_read_chunk(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_list_element_t *list_element,
     libmfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     liberror_error_t **error )
{
	libewf_chunk_data_t *chunk_data   = NULL;
	libewf_chunk_table_t *chunk_table = NULL;
	static char *function             = "libewf_chunk_table_read_chunk";
	ssize_t read_count                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	int element_index                 = 0;
#endif

	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	chunk_table = (libewf_chunk_table_t *) io_handle;

	if( element_data_size > (size64_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( ( element_data_flags & LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_GROUP ) != 0 )
	 || ( ( element_data_flags & LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_SPARSE ) != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported element data flags.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libmfdata_list_element_get_element_index(
		     list_element,
		     &element_index,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve element index from list element.",
			 function );

			goto on_error;
		}
		if( ( element_data_flags & LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			libnotify_printf(
			 "%s: reading compressed chunk: %d from file IO pool entry: %d at offset: %" PRIi64 " of size: %" PRIu64 "\n",
			 function,
			 element_index,
			 file_io_pool_entry,
			 element_data_offset,
			 element_data_size );
		}
		else
		{
			libnotify_printf(
			 "%s: reading uncompressed chunk: %d from file IO pool entry: %d at offset: %" PRIi64 " of size: %" PRIu64 "\n",
			 function,
			 element_index,
			 file_io_pool_entry,
			 element_data_offset,
			 element_data_size );
		}
	}
#endif
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     element_data_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek chunk offset: %" PRIi64 " in file IO pool entry: %d.",
		 function,
		 element_data_offset,
		 file_io_pool_entry );

		goto on_error;
	}
	if( libewf_chunk_data_initialize(
	     &chunk_data,
	     (size_t) element_data_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk data.",
		 function );

		goto on_error;
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_pool_read(
		      file_io_pool,
		      file_io_pool_entry,
		      chunk_data->data,
		      (size_t) element_data_size,
		      error );

	if( read_count != (ssize_t) element_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data.",
		 function );

		goto on_error;
	}
	chunk_data->data_size = (size_t) read_count;

	if( ( element_data_flags & LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		chunk_data->is_compressed = 1;
	}
	chunk_data->is_packed = 1;

	if( libmfdata_list_element_set_element_value(
	     list_element,
	     cache,
	     (intptr_t *) chunk_data,
	     &libewf_chunk_data_free,
	     LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk data as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 (intptr_t *) chunk_data,
		 NULL );
	}
	return( -1 );
}

/* Reads a table section for the chunk offsets
 * Callback function for the chunk table list
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_read_offsets(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_list_t *list,
     int element_index,
     int number_of_elements LIBEWF_ATTRIBUTE_UNUSED,
     libmfdata_cache_t *cache LIBEWF_ATTRIBUTE_UNUSED,
     int file_io_pool_entry,
     off64_t element_group_offset,
     size64_t element_group_size,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     liberror_error_t **error )
{
	uint8_t table_offsets_checksum[ 4 ];

	libewf_chunk_table_t *chunk_table = NULL;
	libewf_section_t *section         = NULL;
	uint8_t *table_offsets_data       = NULL;
	static char *function             = "libewf_chunk_table_read_offsets";
	size_t table_offsets_data_size    = 0;
	ssize_t read_count                = 0;
	uint64_t base_offset              = 0;
	uint32_t calculated_checksum      = 0;
	uint32_t number_of_offsets        = 0;
	uint32_t stored_checksum          = 0;
	uint8_t table_offsets_corrupted   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *trailing_data            = NULL;
#endif

	LIBEWF_UNREFERENCED_PARAMETER( number_of_elements )
	LIBEWF_UNREFERENCED_PARAMETER( cache )
	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	chunk_table = (libewf_chunk_table_t *) io_handle;

	if( chunk_table->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	read_count = libewf_section_start_read(
		      section,
		      file_io_pool,
		      file_io_pool_entry,
		      element_group_offset,
		      error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read section start.",
		 function );

		goto on_error;
	}
	if( element_group_size != section->size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element group size value out of bounds.",
		 function );

		goto on_error;
	}
	element_group_size -= read_count;

	read_count = libewf_section_table_header_read(
	              section,
	              file_io_pool,
	              file_io_pool_entry,
	              &number_of_offsets,
	              &base_offset,
	              error );
	
	if( read_count < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table section header.",
		 function );

		goto on_error;
	}
	element_group_size -= read_count;

	if( number_of_offsets == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_INVALID_DATA,
		 "%s: invalid number of offsets.",
		 function );

		goto on_error;
	}
	table_offsets_data_size = sizeof( ewf_table_offset_t ) * number_of_offsets;

	if( table_offsets_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table offsets data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	table_offsets_data = (uint8_t *) memory_allocate(
	                                  table_offsets_data_size );

	if( table_offsets_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table offsets data.",
		 function );

		goto on_error;
	}
/* TODO handle corruption element_group_size < table_offsets_data_size ? */
	if( element_group_size < (size64_t) table_offsets_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid element group size value too small.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read(
		      file_io_pool,
		      file_io_pool_entry,
		      table_offsets_data,
		      table_offsets_data_size,
		      error );

	if( read_count != (ssize_t) table_offsets_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table offsets data.",
		 function );

		goto on_error;
	}
	element_group_size -= read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
	 	 "%s: table offsets data:\n",
		 function );
		libnotify_print_data(
		 table_offsets_data,
		 table_offsets_data_size );
	}
#endif
	/* The EWF-S01 format does not contain a checksum after the table offsets
	 */
	if( chunk_table->io_handle->ewf_format != EWF_FORMAT_S01 )
	{
		if( element_group_size < (size64_t) sizeof( uint32_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid element group size value too small.",
			 function );

			return( -1 );
		}
		read_count = libbfio_pool_read(
			      file_io_pool,
			      file_io_pool_entry,
			      table_offsets_checksum,
			      sizeof( uint32_t ),
			      error );

		if( read_count != (ssize_t) sizeof( uint32_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read checksum from file descriptor.",
			 function );

			goto on_error;
		}
		element_group_size -= read_count;

		byte_stream_copy_to_uint32_little_endian(
		 table_offsets_checksum,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
	 		 "%s: table offsets checksum\t\t\t: 0x%" PRIx32 "\n",
			 function,
			 table_offsets_checksum );

			libnotify_printf(
	 		 "\n" );
		}
#endif
		calculated_checksum = ewf_checksum_calculate(
				       table_offsets_data,
				       table_offsets_data_size,
				       1 );

		if( stored_checksum != calculated_checksum )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: checksum does not match (stored: 0x%08" PRIx32 " calculated: 0x%08" PRIx32 ").\n",
				 function,
				 stored_checksum,
				 calculated_checksum );
			}
#endif
			/* The table offsets cannot be fully trusted therefore mark them as corrupted
			 */
			table_offsets_corrupted = 1;
		}
	}
	if( libewf_chunk_table_fill(
	     chunk_table,
	     list,
	     element_index,
	     file_io_pool_entry,
	     section,
	     (off64_t) base_offset,
	     (ewf_table_offset_t *) table_offsets_data,
	     number_of_offsets,
	     table_offsets_corrupted,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to fill chunk table.",
		 function );

		goto on_error;
	}
	memory_free(
	 table_offsets_data );

	table_offsets_data = NULL;

	if( libewf_section_free(
	     (intptr_t *) section,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		section = NULL;

		goto on_error;
	}
	section = NULL;

#if defined( HAVE_VERBOSE_OUTPUT ) || defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( ( element_group_size > (size64_t) 0 )
		 && ( chunk_table->io_handle->ewf_format != EWF_FORMAT_S01 )
		 && ( chunk_table->io_handle->format != LIBEWF_FORMAT_ENCASE1 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			libnotify_printf(
		 	 "%s: trailing data:\n",
			 function );

			trailing_data = (uint8_t *) memory_allocate(
			                             sizeof( uint8_t ) * (size_t) element_group_size );

			if( trailing_data == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create trailing data.",
				 function );

				goto on_error;
			}
			read_count = libbfio_pool_read(
				      file_io_pool,
				      file_io_pool_entry,
				      trailing_data,
				      element_group_size,
				      error );

			if( read_count != (ssize_t) element_group_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read trailing data.",
				 function );

				goto on_error;
			}
			libnotify_printf(
		 	 "%s: trailing data:\n",
			 function );
			libnotify_print_data(
			 trailing_data,
			 (size_t) element_group_size );

			memory_free(
			 trailing_data );

			trailing_data = NULL;

#elif defined( HAVE_VERBOSE_OUTPUT )
			libnotify_printf(
		 	 "%s: trailing data after table section offsets.\n",
			 function );
#endif
		}
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( trailing_data != NULL )
	{
		memory_free(
		 trailing_data );
	}
#endif
	if( table_offsets_data != NULL )
	{
		memory_free(
		 table_offsets_data );
	}
	if( section != NULL )
	{
		libewf_section_free(
		 (intptr_t *) section,
		 NULL );
	}
	return( -1 );
}

/* Fills the chunk table from the offsets
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_fill(
     libewf_chunk_table_t *chunk_table,
     libmfdata_list_t *list,
     int chunk_index,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     off64_t base_offset,
     ewf_table_offset_t *table_offsets,
     uint32_t number_of_offsets,
     uint8_t tainted,
     liberror_error_t **error )
{
	static char *function           = "libewf_chunk_table_fill";
	off64_t last_chunk_offset       = 0;
	off64_t last_chunk_size         = 0;
	off64_t previous_chunk_offset   = 0;
	size64_t previous_chunk_size    = 0;
	uint32_t chunk_flags            = 0;
	uint32_t chunk_size             = 0;
	uint32_t current_offset         = 0;
	uint32_t next_offset            = 0;
	uint32_t previous_chunk_flags   = 0;
	uint32_t stored_offset          = 0;
	uint32_t table_offset_index     = 0;
	uint8_t corrupted               = 0;
	uint8_t is_compressed           = 0;
	uint8_t overflow                = 0;
	int previous_file_io_pool_entry = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *chunk_type                = NULL;
	char *remarks                   = NULL;
#endif

	if( chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( table_section == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table section.",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	if( table_offsets == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table offsets.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 table_offsets[ table_offset_index ].offset,
	 stored_offset );

	while( table_offset_index < ( number_of_offsets - 1 ) )
	{
		if( overflow == 0 )
		{
			is_compressed  = (uint8_t) ( stored_offset >> 31 );
			current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			current_offset = stored_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 table_offsets[ table_offset_index + 1 ].offset,
		 stored_offset );

		if( overflow == 0 )
		{
			next_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			next_offset = stored_offset;
		}
		corrupted = 0;

		/* This is to compensate for the crappy > 2 GiB segment file solution in EnCase 6.7
		 */
		if( next_offset < current_offset )
		{
			if( stored_offset < current_offset )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: chunk offset: %" PRIu32 " larger than stored chunk offset: %" PRIu32 ".\n",
					 function,
					 current_offset,
					 stored_offset );
				}
#endif
				corrupted = 1;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: chunk offset: %" PRIu32 " larger than next chunk offset: %" PRIu32 ".\n",
				 function,
				 current_offset,
				 next_offset );
			}
#endif
			chunk_size = stored_offset - current_offset;
		}
		else
		{
			chunk_size = next_offset - current_offset;
		}
		if( chunk_size == 0 )
		{
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: invalid chunk size value is zero.\n",
				 function );
			}
			corrupted = 1;
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: invalid chunk size value exceeds maximum.\n",
				 function );
			}
			corrupted = 1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( is_compressed == 0 )
		{
			chunk_type = "uncompressed";
		}
		else
		{
			chunk_type = "compressed";
		}
		if( corrupted != 0 )
		{
			remarks = " corrupted";
		}
		else if( tainted != 0 )
		{
			remarks = " tainted";
		}
		else
		{
			remarks = "";
		}
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: %s chunk %d read with: base %" PRIi64 ", offset %" PRIu32 " and size %" PRIu32 "%s.\n",
			 function,
			 chunk_type,
			 chunk_index,
			 base_offset,
			 current_offset,
			 chunk_size,
			 remarks );
		}
#endif
		if( libmfdata_list_get_element_by_index(
		     list,
		     chunk_index,
		     &previous_file_io_pool_entry,
		     &previous_chunk_offset,
		     &previous_chunk_size,
		     &previous_chunk_flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk: %d from chunk table list.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( ( previous_chunk_flags & LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_GROUP ) != 0 )
		{
			chunk_flags = 0;

			if( is_compressed != 0 )
			{
				chunk_flags |= LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_COMPRESSED;
			}
			if( corrupted != 0 )
			{
				chunk_flags |= LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_CORRUPTED;
			}
			if( tainted != 0 )
			{
				chunk_flags |= LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_TAINTED;
			}
			if( libmfdata_list_set_element_by_index(
			     list,
			     chunk_index,
			     file_io_pool_entry,
			     base_offset + current_offset,
			     (size64_t) chunk_size,
			     chunk_flags,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set chunk: %d in chunk table list.",
				 function,
				 chunk_index );

				return( -1 );
			}
			if( libmfdata_list_set_element_value_size_by_index(
			     list,
			     chunk_index,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set value size of chunk: %d in chunk table list.",
				 function,
				 chunk_index );

				return( -1 );
			}
		}
		/* This is to compensate for the crappy > 2 GiB segment file solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: chunk offset overflow at: %" PRIu32 ".\n",
				 function,
				 current_offset );
			}
#endif
			overflow      = 1;
			is_compressed = 0;
		}
		chunk_index++;

		table_offset_index++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 table_offsets[ table_offset_index ].offset,
	 stored_offset );

	if( overflow == 0 )
	{
		is_compressed  = (uint8_t) ( stored_offset >> 31 );
		current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = stored_offset;
	}
	corrupted = 0;

	/* There is no indication how large the last chunk is.
	 * The only thing known is where it starts.
	 * However it can be determined using the offset of the next section.
	 * The size of the last chunk is determined by subtracting the last offset from the offset of the next section.
	 *
	 * The offset of the next section is either table_section->end_offset for original EWF and EWF-S01
	 * or table_section->start_offset for other types of EWF
	 */
	last_chunk_offset = (off64_t) base_offset + current_offset;

	if( last_chunk_offset > (off64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid last chunk offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( last_chunk_offset < table_section->start_offset )
	{
		last_chunk_size = table_section->start_offset - last_chunk_offset;
	}
	else if( last_chunk_offset < table_section->end_offset )
	{
		last_chunk_size = table_section->end_offset - last_chunk_offset;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: invalid last chunk offset value exceeds table section end offset.\n",
		 function );
	}
#endif
	if( last_chunk_size <= 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: invalid last chunk size value is zero or less.\n",
			 function );
		}
#endif
		corrupted = 1;
	}
	if( last_chunk_size > (off64_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: invalid last chunk size value exceeds maximum.\n",
			 function );
		}
#endif
		corrupted = 1;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( is_compressed == 0 )
	{
		chunk_type = "uncompressed";
	}
	else
	{
		chunk_type = "compressed";
	}
	if( corrupted != 0 )
	{
		remarks = " corrupted";
	}
	else if( tainted != 0 )
	{
		remarks = " tainted";
	}
	else
	{
		remarks = "";
	}
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: %s last chunk %d read with: base %" PRIi64 ", offset %" PRIu32 " and calculated size %" PRIi64 "%s.\n",
		 function,
		 chunk_type,
		 chunk_index,
		 base_offset,
		 current_offset,
		 last_chunk_size,
		 remarks );
	}
#endif
	if( libmfdata_list_get_element_by_index(
	     list,
	     chunk_index,
	     &previous_file_io_pool_entry,
	     &previous_chunk_offset,
	     &previous_chunk_size,
	     &previous_chunk_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk: %d from chunk table list.",
		 function,
		 chunk_index );

		return( -1 );
	}
	if( ( previous_chunk_flags & LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_GROUP ) != 0 )
	{
		chunk_flags = 0;

		if( is_compressed != 0 )
		{
			chunk_flags |= LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_COMPRESSED;
		}
		if( corrupted != 0 )
		{
			chunk_flags |= LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_CORRUPTED;
		}
		if( tainted != 0 )
		{
			chunk_flags |= LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_TAINTED;
		}
		if( libmfdata_list_set_element_by_index(
		     list,
		     chunk_index,
		     file_io_pool_entry,
		     last_chunk_offset,
		     (size64_t) last_chunk_size,
		     chunk_flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set chunk: %d in chunk table list.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( libmfdata_list_set_element_value_size_by_index(
		     list,
		     chunk_index,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value size of chunk: %d in chunk table list.",
			 function,
			 chunk_index );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

#ifdef TODO

/* Compares the offsets with the ones in the chunk table and makes corrections if necessary
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_compare(
     libmfdata_list_t *chunk_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t number_of_offsets,
     int segment_table_index,
     uint8_t tainted,
     liberror_error_t **error )
{
	static char *function       = "libewf_chunk_table_compare";
	uint32_t chunk_size         = 0;
	uint32_t current_offset     = 0;
	uint32_t next_offset        = 0;
	uint32_t table_offset_index = 0;
	uint32_t stored_offset      = 0;
	uint8_t corrupted           = 0;
	uint8_t is_compressed       = 0;
	uint8_t mismatch            = 0;
	uint8_t overflow            = 0;
	int number_of_chunks        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *chunk_type            = NULL;
	char *remarks               = NULL;
#endif

	if( chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	if( offsets == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets.",
		 function );

		return( -1 );
	}
	if( libmfdata_list_get_number_of_elements(
	     chunk_table,
	     &number_of_chunks,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of chunks in the chunk table.",
		 function );

		return( -1 );
	}
	/* Allocate additional entries in the chunk table if needed
	 * - a single reallocation saves processing time
	 */
	if( (uint32_t) number_of_chunks < ( chunk_table->last_chunk_value_compared + number_of_offsets ) )
	{
		if( libmfdata_list_resize_elements(
		     chunk_table,
		     (int) ( chunk_table->last_chunk_value_compared + number_of_offsets ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk table.",
			 function );

			return( -1 );
		}
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ table_offset_index ].offset,
	 stored_offset );

	/* The size of the last chunk must be determined differently
	 */
	while( table_offset_index < ( number_of_offsets - 1 ) )
	{
		if( overflow == 0 )
		{
			is_compressed  = (uint8_t) ( stored_offset >> 31 );
			current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			current_offset = stored_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 offsets[ table_offset_index + 1 ].offset,
		 stored_offset );

		if( overflow == 0 )
		{
			next_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			next_offset = stored_offset;
		}
		corrupted = 0;

		/* This is to compensate for the crappy > 2 GiB segment file
		 * solution in EnCase 6.7
		 */
		if( next_offset < current_offset )
		{
			if( stored_offset < current_offset )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: chunk offset: %" PRIu32 " larger than stored chunk offset: %" PRIu32 ".\n",
					 function,
					 current_offset,
					 stored_offset );
				}
#endif
				corrupted = 1;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: chunk offset: %" PRIu32 " larger than next chunk offset: %" PRIu32 ".\n",
				 function,
				 current_offset,
				 next_offset );
			}
#endif
			chunk_size = stored_offset - current_offset;
		}
		else
		{
			chunk_size = next_offset - current_offset;
		}

		if( chunk_size == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: invalid chunk size - size is zero.\n",
				 function );
			}
#endif
			corrupted = 1;
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: invalid chunk size value exceeds maximum.\n",
				 function );
			}
#endif
			corrupted = 1;
		}
		if( libewf_array_get_entry_by_index(
		     chunk_table->chunk_values,
		     (int) chunk_table->last_chunk_value_compared,
		     (intptr_t **) &chunk_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
			 function,
			 chunk_table->last_chunk_value_compared );

			return( -1 );
		}
		if( chunk_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk value.",
			 function );

			return( -1 );
		}
		if( chunk_value->file_offset != (off64_t) ( base_offset + current_offset ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: file offset mismatch for chunk value: %" PRIu32 ".\n",
				 function,
				 chunk_table->last_chunk_value_compared );
			}
#endif
			mismatch = 1;
		}
		else if( chunk_value->size != (size_t) chunk_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: chunk size mismatch for chunk value: %" PRIu32 ".\n",
				 function,
				 chunk_table->last_chunk_value_compared );
			}
#endif

			mismatch = 1;
		}
		else if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAG_COMPRESSED ) != is_compressed )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: compressed mismatch for chunk value: %" PRIu32 ".\n",
				 function,
				 chunk_table->last_chunk_value_compared );
			}
#endif
			mismatch = 1;
		}
		else
		{
			mismatch = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( is_compressed == 0 )
		{
			chunk_type = "uncompressed";
		}
		else
		{
			chunk_type = "compressed";
		}
		if( corrupted != 0 )
		{
			remarks = " corrupted";
		}
		else if( tainted != 0 )
		{
			remarks = " tainted";
		}
		else if( mismatch == 1 )
		{
			remarks = " corrected";
		}
		else
		{
			remarks = "";
		}
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: %s chunk %" PRIu32 " read with: base %" PRIi64 ", offset %" PRIu32 " and size %" PRIu32 "%s.\n",
			 function,
			 chunk_type,
			 chunk_table->last_chunk_value_compared,
			 base_offset,
			 current_offset,
			 chunk_size,
			 remarks );
		}
#endif
		if( ( corrupted == 0 )
		 && ( tainted == 0 )
		 && ( mismatch == 1 ) )
		{
			chunk_value->segment_table_index = segment_table_index;
			chunk_value->file_offset         = (off64_t) ( base_offset + current_offset );
			chunk_value->size                = (size_t) chunk_size;
			chunk_value->flags              |= is_compressed;
			chunk_value->flags              &= ~( LIBEWF_CHUNK_VALUE_FLAG_TAINTED | LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED ) ;
		}
		chunk_table->last_chunk_value_compared++;

		/* This is to compensate for the crappy > 2 GiB segment file
		 * solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: chunk offset overflow at: %" PRIu32 ".\n",
				 function,
				 current_offset );
			}
#endif
			overflow      = 1;
			is_compressed = 0;
		}
		table_offset_index++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ table_offset_index ].offset,
	 stored_offset );

	if( overflow == 0 )
	{
		is_compressed  = (uint8_t) ( stored_offset >> 31 );
		current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = stored_offset;
	}
	corrupted = 0;

	if( libewf_array_get_entry_by_index(
	     chunk_table->chunk_values,
	     (int) chunk_table->last_chunk_value_compared,
	     (intptr_t **) &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
		 function,
		 chunk_table->last_chunk_value_compared );

		return( -1 );
	}
	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk value.",
		 function );

		return( -1 );
	}
	if( chunk_value->file_offset != (off64_t) ( base_offset + current_offset ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: file offset mismatch for chunk value: %" PRIu32 ".\n",
			 function,
			 chunk_table->last_chunk_value_compared );
		}
#endif

		mismatch = 1;
	}
	else if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAG_COMPRESSED ) != is_compressed )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: compressed mismatch for chunk value: %" PRIu32 ".\n",
			 function,
			 chunk_table->last_chunk_value_compared );
		}
#endif
		mismatch = 1;
	}
	else
	{
		mismatch = 0;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( is_compressed == 0 )
	{
		chunk_type = "uncompressed";
	}
	else
	{
		chunk_type = "compressed";
	}
	if( corrupted != 0 )
	{
		remarks = " corrupted";
	}
	else if( tainted != 0 )
	{
		remarks = " tainted";
	}
	else if( mismatch == 1 )
	{
		remarks = " corrected";
	}
	else
	{
		remarks = "";
	}
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: %s last chunk %" PRIu32 " read with: base %" PRIi64 " and offset %" PRIu32 "%s.\n",
		 function,
		 chunk_type,
		 chunk_table->last_chunk_value_compared,
		 base_offset,
		 current_offset,
		 remarks );
	}
#endif
	if( ( corrupted == 0 )
	 && ( tainted == 0 )
	 && ( mismatch == 1 ) )
	{
		chunk_value->segment_table_index = segment_table_index;
		chunk_value->file_offset         = (off64_t) ( base_offset + current_offset );
		chunk_value->flags              |= is_compressed;
		chunk_value->flags              &= ~( LIBEWF_CHUNK_VALUE_FLAG_TAINTED | LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED ) ;
	}
	return( 1 );
}

/* Calculate the last offset and compares it with the one in the chunk table and makes corrections if necessary
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_compare_last_offset(
     libmfdata_list_t *chunk_table,
     libewf_list_t *section_list,
     uint8_t tainted,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value   = NULL;
	libewf_list_element_t *list_element = NULL;
	libewf_section_t *section           = NULL;
	static char *function               = "libewf_chunk_table_fill_last_offset";
	off64_t last_offset                 = 0;
	off64_t chunk_size                  = 0;
	uint8_t corrupted                   = 0;
	uint8_t mismatch                    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *remarks                       = NULL;
#endif

	if( chunk_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( section_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid section list.",
		 function );

		return( -1 );
	}
	if( libewf_array_get_entry_by_index(
	     chunk_table->chunk_values,
	     (int) chunk_table->last_chunk_value_compared,
	     (intptr_t **) &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
		 function,
		 chunk_table->last_chunk_value_compared );

		return( -1 );
	}
	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk value.",
		 function );

		return( -1 );
	}
	/* There is no indication how large the last chunk is. The only thing known is where it starts.
	 * However it can be determined where the next section starts within the file.
	 * The size of the last chunk is determined by subtracting the last offset from the offset of the section that follows.
	 */
	list_element = section_list->first_element;
	last_offset  = chunk_value->file_offset;

	while( list_element != NULL )
	{
		section = (libewf_section_t *) list_element->value;

		if( section == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid section.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: section: %s start offset: %" PRIi64 " last offset: %" PRIi64 " \n",
			 function,
			 (char *) section->type,
			 section->start_offset,
			 last_offset );
		}
#endif
		if( ( section->start_offset < last_offset )
		 && ( last_offset < section->end_offset ) )
		{
			chunk_size = section->end_offset - last_offset;

			if( last_offset > (off64_t) INT64_MAX )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid last chunk offset value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( chunk_size == 0 )
			{
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: invalid chunk size - size is zero.\n",
					 function );
				}
				corrupted = 1;
			}
			if( chunk_size > (off64_t) INT32_MAX )
			{
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
				 	"%s: invalid chunk size value exceeds maximum.\n",
					 function );
				}
				corrupted = 1;
			}
			if( chunk_value->size != (size_t) chunk_size )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: chunk size mismatch for chunk value: %" PRIu32 ".\n",
					 function,
					 chunk_table->last_chunk_value_compared );
				}
#endif
				mismatch = 1;
			}
			else
			{
				mismatch = 0;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( corrupted != 0 )
			{
				remarks = " corrupted";
			}
			else if( tainted != 0 )
			{
				remarks = " tainted";
			}
			else if( mismatch == 1 )
			{
				remarks = " corrected";
			}
			else
			{
				remarks = "";
			}
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: last chunk %" PRIu32 " calculated with offset: %" PRIi64 " and size %" PRIzu "%s.\n",
				 function,
				 chunk_table->last_chunk_value_compared,
				 last_offset,
				 (size_t) chunk_size,
				 remarks );
			}
#endif
			if( ( corrupted == 0 )
			 && ( tainted == 0 )
			 && ( mismatch == 1 ) )
			{
				chunk_value->size   = (size_t) chunk_size;
				chunk_value->flags &= ~( LIBEWF_CHUNK_VALUE_FLAG_TAINTED | LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED ) ;
			}
			chunk_table->last_chunk_value_compared++;

			break;
		}
		list_element = list_element->next_element;
	}
	return( 1 );
}

#endif /* TODO */

/* Fills the offsets from the chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_fill_offsets(
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     off64_t base_offset,
     ewf_table_offset_t *table_offsets,
     uint32_t number_of_offsets,
     liberror_error_t **error )
{
	static char *function       = "libewf_chunk_table_fill_offsets";
	off64_t chunk_offset        = 0;
	size64_t chunk_size         = 0;
	uint32_t chunk_flags        = 0;
	uint32_t table_offset       = 0;
	uint32_t table_offset_index = 0;
	int file_io_pool_entry      = 0;

	if( chunk_table_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table list.",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	if( table_offsets == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table offsets.",
		 function );

		return( -1 );
	}
	for( table_offset_index = 0;
	     table_offset_index < number_of_offsets;
	     table_offset_index++ )
	{
		if( libmfdata_list_get_element_by_index(
		     chunk_table_list,
		     chunk_index,
		     &file_io_pool_entry,
		     &chunk_offset,
		     &chunk_size,
		     &chunk_flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of chunk: %d.",
			 function,
			 chunk_index );

			return( -1 );
		}
		chunk_offset -= base_offset;

		if( ( chunk_offset < 0 )
		 || ( chunk_offset > (off64_t) INT32_MAX ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk: %d offset value out of bounds.",
			 function,
			 chunk_index );

			return( -1 );
		}
		table_offset = (uint32_t) chunk_offset;

		if( ( chunk_flags & LIBMFDATA_LIST_ELEMENT_DATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			table_offset |= EWF_OFFSET_COMPRESSED_WRITE_MASK;
		}
		byte_stream_copy_from_uint32_little_endian(
		 table_offsets[ table_offset_index ].offset,
		 table_offset );

		chunk_index++;
	}
	return( 1 );
}


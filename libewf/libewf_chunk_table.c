/*
 * Chunk table functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_checksum.h"
#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_section.h"
#include "libewf_unused.h"

#include "ewf_definitions.h"
#include "ewf_table.h"

/* Initialize the chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_initialize(
     libewf_chunk_table_t **chunk_table,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_initialize";

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( *chunk_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk table value already set.",
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
	*chunk_table = memory_allocate_structure(
	                libewf_chunk_table_t );

	if( *chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk_table,
	     0,
	     sizeof( libewf_chunk_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk table.",
		 function );

		goto on_error;
	}
	( *chunk_table )->io_handle = io_handle;

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
     libewf_chunk_table_t **chunk_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_free";

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
	if( *chunk_table != NULL )
	{
		memory_free(
		 *chunk_table );

		*chunk_table = NULL;
	}
	return( 1 );
}

/* Clones the chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_clone(
     libewf_chunk_table_t **destination_chunk_table,
     libewf_chunk_table_t *source_chunk_table,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_table_clone";

	if( destination_chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination chunk table.",
		 function );

		return( -1 );
	}
	if( *destination_chunk_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination chunk table value already set.",
		 function );

		return( -1 );
	}
	if( source_chunk_table == NULL )
	{
		*destination_chunk_table = NULL;

		return( 1 );
	}
	*destination_chunk_table = memory_allocate_structure(
	                            libewf_chunk_table_t );

	if( *destination_chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination chunk table.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_chunk_table,
	     source_chunk_table,
	     sizeof( libewf_chunk_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
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
     libfcache_cache_t *cache,
     int file_io_pool_entry,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data = NULL;
	static char *function           = "libewf_chunk_table_read_chunk";
	ssize_t read_count              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	int element_index               = 0;
	uint32_t chunk_checksum         = 0;
#endif

	LIBEWF_UNREFERENCED_PARAMETER( read_flags )

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
	if( ( element_data_size == (size64_t) 0 )
	 || ( element_data_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( element_data_flags & LIBEWF_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported element data flags.",
		 function );

		return( -1 );
	}
	if( libewf_chunk_data_initialize(
	     &chunk_data,
	     (size_t) element_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk data.",
		 function );

		goto on_error;
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk data.",
		 function );

		goto on_error;
	}
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     file_io_pool_entry,
	     element_data_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek chunk offset: %" PRIi64 " in file IO pool entry: %d.",
		 function,
		 element_data_offset,
		 file_io_pool_entry );

		goto on_error;
	}
	read_count = libbfio_pool_read_buffer(
		      file_io_pool,
		      file_io_pool_entry,
		      chunk_data->data,
		      (size_t) element_data_size,
		      error );

	if( read_count != (ssize_t) element_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data.",
		 function );

		goto on_error;
	}
	chunk_data->data_size = (size_t) read_count;

	chunk_data->range_flags = ( element_data_flags | LIBEWF_RANGE_FLAG_IS_PACKED )
	                        & ~( LIBEWF_RANGE_FLAG_IS_TAINTED | LIBEWF_RANGE_FLAG_IS_CORRUPTED );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libmfdata_list_element_get_element_index(
		     list_element,
		     &element_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve element index from list element.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: chunk: % 8d file IO pool entry\t: %d\n",
		 function,
		 element_index,
		 file_io_pool_entry );

		libcnotify_printf(
		 "%s: chunk: % 8d offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 element_index,
		 element_data_offset,
		 element_data_offset );

		libcnotify_printf(
		 "%s: chunk: % 8d size\t\t\t: %" PRIu64 "\n",
		 function,
		 element_index,
		 element_data_size );

		if( ( ( element_data_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		 && ( chunk_data->data_size >= 4 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( ( chunk_data->data )[ chunk_data->data_size - 4 ] ),
			 chunk_checksum );
		}
		libcnotify_printf(
		 "%s: chunk: % 8d checksum\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 element_index,
		 chunk_checksum );

		libcnotify_printf(
		 "%s: chunk: % 8d flags:\n",
		 function,
		 element_index );

		if( ( element_data_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			libcnotify_printf(
			 "Is compressed\n" );
		}
		if( ( element_data_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		{
			libcnotify_printf(
			 "Has checksum\n" );
		}
		if( ( element_data_flags & LIBEWF_RANGE_FLAG_IS_DELTA ) != 0 )
		{
			libcnotify_printf(
			 "Is delta\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libmfdata_list_element_set_element_value(
	     list_element,
	     cache,
	     (intptr_t *) chunk_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
	     LIBMFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk data as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &chunk_data,
		 NULL );
	}
	return( -1 );
}

/* Reads a table section for the chunk offsets
 * Callback function for the chunk table list
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_chunk_table_read_offsets(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libmfdata_list_t *chunk_table_list,
     int element_index,
     int number_of_elements,
     libfcache_cache_t *cache LIBEWF_ATTRIBUTE_UNUSED,
     int file_io_pool_entry,
     off64_t element_group_offset,
     size64_t element_group_size,
     uint32_t element_group_flags LIBEWF_ATTRIBUTE_UNUSED,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libewf_chunk_table_t *chunk_table = NULL;
	libewf_section_t *section         = NULL;
	uint8_t *section_data             = NULL;
	uint8_t *table_entries_data       = NULL;
	static char *function             = "libewf_chunk_table_read_offsets";
	size_t section_data_size          = 0;
	size_t table_entries_data_size    = 0;
	ssize_t read_count                = 0;
	uint64_t base_offset              = 0;
	uint64_t first_chunk_index        = 0;
	uint32_t number_of_entries        = 0;
	uint8_t entries_corrupted         = 0;
	int result                        = 0;

	LIBEWF_UNREFERENCED_PARAMETER( cache )
	LIBEWF_UNREFERENCED_PARAMETER( element_group_flags )

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
	chunk_table = (libewf_chunk_table_t *) io_handle;

	if( chunk_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( element_group_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element group size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( libewf_section_initialize(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create section.",
		 function );

		goto on_error;
	}
	if( chunk_table->io_handle->major_version == 1 )
	{
		read_count = libewf_section_descriptor_read(
			      section,
			      file_io_pool,
			      file_io_pool_entry,
			      element_group_offset,
			      chunk_table->io_handle->major_version,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section descriptor.",
			 function );

			goto on_error;
		}
		if( element_group_size != section->size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid element group size value out of bounds.",
			 function );

			goto on_error;
		}
		element_group_size -= read_count;
	}
	else if( chunk_table->io_handle->major_version == 2 )
	{
		if( libbfio_pool_seek_offset(
		     file_io_pool,
		     file_io_pool_entry,
		     element_group_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk table offset: %" PRIi64 " in file IO pool entry: %d.",
			 function,
			 element_group_offset,
			 file_io_pool_entry );

			goto on_error;
		}
/* TODO check bounds */
		section->start_offset = element_group_offset;
		section->data_size    = (uint32_t) element_group_size;
	}
	read_count = libewf_section_table_read(
		      section,
		      chunk_table->io_handle,
		      file_io_pool,
		      file_io_pool_entry,
		      chunk_table->io_handle->major_version,
		      &section_data,
		      &section_data_size,
		      &first_chunk_index,
		      &base_offset,
		      &table_entries_data,
		      &table_entries_data_size,
		      &number_of_entries,
		      &entries_corrupted,
		      error );
	
	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table section.",
		 function );

		goto on_error;
	}
	element_group_size -= read_count;

/* TODO check bounds of first_chunk_index ? */

	if( number_of_entries == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_INVALID_DATA,
		 "%s: invalid number of entries.",
		 function );

		goto on_error;
	}
	if( ( read_flags & LIBMFDATA_READ_FLAG_IS_BACKUP_RANGE ) == 0 )
	{
		if( chunk_table->io_handle->major_version == 1 )
		{
			result = libewf_chunk_table_fill_v1(
			          chunk_table,
			          chunk_table_list,
			          element_index,
			          file_io_pool_entry,
			          section,
			          (off64_t) base_offset,
			          number_of_entries,
			          table_entries_data,
			          table_entries_data_size,
			          entries_corrupted,
			          error );
		}
		else if( chunk_table->io_handle->major_version == 2 )
		{
			result = libewf_chunk_table_fill_v2(
			          chunk_table,
			          chunk_table_list,
			          element_index,
			          file_io_pool_entry,
			          section,
			          number_of_entries,
			          table_entries_data,
			          table_entries_data_size,
			          entries_corrupted,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to fill chunk table.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( chunk_table->io_handle->major_version == 1 )
		{
			result = libewf_chunk_table_correct_v1(
				  chunk_table,
				  chunk_table_list,
				  element_index,
				  file_io_pool_entry,
				  section,
				  (off64_t) base_offset,
				  number_of_entries,
				  table_entries_data,
				  table_entries_data_size,
				  entries_corrupted,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to correct chunk table.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 section_data );

	section_data = NULL;

	if( libewf_section_free(
	     &section,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free section.",
		 function );

		section = NULL;

		goto on_error;
	}
	if( entries_corrupted != 0 )
	{
		return( 0 );
	}
	return( 1 );

on_error:
	if( section_data != NULL )
	{
		memory_free(
		 section_data );
	}
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( -1 );
}

/* Fills the chunk table from the EWF version 1 sector table entries
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_fill_v1(
     libewf_chunk_table_t *chunk_table,
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     off64_t base_offset,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error )
{
	static char *function           = "libewf_chunk_table_fill_v1";
	off64_t last_chunk_offset       = 0;
	off64_t last_chunk_size         = 0;
	off64_t previous_chunk_offset   = 0;
	size64_t previous_chunk_size    = 0;
	uint32_t chunk_size             = 0;
	uint32_t current_offset         = 0;
	uint32_t next_offset            = 0;
	uint32_t previous_range_flags   = 0;
	uint32_t range_flags            = 0;
	uint32_t stored_offset          = 0;
	uint32_t table_entry_index      = 0;
	uint8_t corrupted               = 0;
	uint8_t is_compressed           = 0;
	uint8_t overflow                = 0;
	int previous_file_io_pool_entry = 0;
	int result                      = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
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
	if( base_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
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
	byte_stream_copy_to_uint32_little_endian(
	 ( ( (ewf_table_entry_v1_t *) table_entries_data )[ table_entry_index ] ).chunk_data_offset,
	 stored_offset );

	while( table_entry_index < ( number_of_entries - 1 ) )
	{
		if( overflow == 0 )
		{
			is_compressed  = (uint8_t) ( stored_offset >> 31 );
			current_offset = stored_offset & 0x7fffffffUL;
		}
		else
		{
			current_offset = stored_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( ( (ewf_table_entry_v1_t *) table_entries_data )[ table_entry_index + 1 ] ).chunk_data_offset,
		 stored_offset );

		if( overflow == 0 )
		{
			next_offset = stored_offset & 0x7fffffffUL;
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
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: chunk offset: %" PRIu32 " larger than stored chunk offset: %" PRIu32 ".\n",
					 function,
					 current_offset,
					 stored_offset );
				}
#endif
				corrupted = 1;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
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
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid chunk size value is zero.\n",
				 function );
			}
			corrupted = 1;
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid chunk size value exceeds maximum.\n",
				 function );
			}
			corrupted = 1;
		}
		if( is_compressed != 0 )
		{
			range_flags = LIBEWF_RANGE_FLAG_IS_COMPRESSED;
		}
		else
		{
			range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;
		}
		if( corrupted != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
		}
		if( tainted != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_IS_TAINTED;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk index\t\t: %d\n",
			 function,
			 table_entry_index,
			 chunk_index );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " base offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 table_entry_index,
			 base_offset );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data offset\t: 0x%08" PRIx32 "\n",
			 function,
			 table_entry_index,
			 current_offset );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data size\t: %" PRIu32 "\n",
			 function,
			 table_entry_index,
			 chunk_size );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data flags:\n",
			 function,
			 table_entry_index );

			if( is_compressed != 0 )
			{
				libcnotify_printf(
				 "Is compressed\n" );
			}
			else
			{
				libcnotify_printf(
				 "Has checksum\n" );
			}
			if( corrupted != 0 )
			{
				libcnotify_printf(
				 "Is corrupted\n" );
			}
			else if( tainted != 0 )
			{
				libcnotify_printf(
				 "Is tainted\n" );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif
		result = libmfdata_list_is_group(
		          chunk_table_list,
		          chunk_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunk: %d is a group.",
			 function,
			 chunk_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libmfdata_list_set_element_by_index(
			     chunk_table_list,
			     chunk_index,
			     file_io_pool_entry,
			     base_offset + current_offset,
			     (size64_t) chunk_size,
			     range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set chunk: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
		}
		else
		{
			if( libmfdata_list_get_data_range_by_index(
			     chunk_table_list,
			     chunk_index,
			     &previous_file_io_pool_entry,
			     &previous_chunk_offset,
			     &previous_chunk_size,
			     &previous_range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data range of chunk: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
			if( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_DELTA ) == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: unable to set data range of chunk: %d value already set.",
				 function,
				 chunk_index );

				return( -1 );
			}
			/* No need to overwrite the data range of a delta chunk */
		}
		/* This is to compensate for the crappy > 2 GiB segment file solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: chunk offset overflow at: %" PRIu32 ".\n",
				 function,
				 current_offset );
			}
#endif
			overflow      = 1;
			is_compressed = 0;
		}
		chunk_index++;

		table_entry_index++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( ( (ewf_table_entry_v1_t *) table_entries_data )[ table_entry_index ] ).chunk_data_offset,
	 stored_offset );

	if( overflow == 0 )
	{
		is_compressed  = (uint8_t) ( stored_offset >> 31 );
		current_offset = stored_offset & 0x7fffffffUL;
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
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
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: invalid last chunk offset value exceeds table section end offset.\n",
		 function );
	}
#endif
	if( last_chunk_size <= 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last chunk size value is zero or less.\n",
			 function );
		}
#endif
		corrupted = 1;
	}
	if( last_chunk_size > (off64_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last chunk size value exceeds maximum.\n",
			 function );
		}
#endif
		corrupted = 1;
	}
	if( is_compressed != 0 )
	{
		range_flags = LIBEWF_RANGE_FLAG_IS_COMPRESSED;
	}
	else
	{
		range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;
	}
	if( corrupted != 0 )
	{
		range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
	}
	if( tainted != 0 )
	{
		range_flags |= LIBEWF_RANGE_FLAG_IS_TAINTED;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk index\t\t: %d\n",
		 function,
		 table_entry_index,
		 chunk_index );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " base offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 table_entry_index,
		 base_offset );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk data offset\t: 0x%08" PRIx32 "\n",
		 function,
		 table_entry_index,
		 current_offset );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk data size\t: %" PRIu32 " (calculated)\n",
		 function,
		 table_entry_index,
		 last_chunk_size );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk data flags:\n",
		 function,
		 table_entry_index );

		if( is_compressed != 0 )
		{
			libcnotify_printf(
			 "Is compressed\n" );
		}
		else
		{
			libcnotify_printf(
			 "Has checksum\n" );
		}
		if( corrupted != 0 )
		{
			libcnotify_printf(
			 "Is corrupted\n" );
		}
		else if( tainted != 0 )
		{
			libcnotify_printf(
			 "Is tainted\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	result = libmfdata_list_is_group(
		  chunk_table_list,
		  chunk_index,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if chunk: %d is a group.",
		 function,
		 chunk_index );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libmfdata_list_set_element_by_index(
		     chunk_table_list,
		     chunk_index,
		     file_io_pool_entry,
		     last_chunk_offset,
		     (size64_t) last_chunk_size,
		     range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set chunk: %d.",
			 function,
			 chunk_index );

			return( -1 );
		}
	}
	else
	{
		if( libmfdata_list_get_data_range_by_index(
		     chunk_table_list,
		     chunk_index,
		     &previous_file_io_pool_entry,
		     &previous_chunk_offset,
		     &previous_chunk_size,
		     &previous_range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of chunk: %d.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_DELTA ) == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: unable to set data range of chunk: %d value already set.",
			 function,
			 chunk_index );

			return( -1 );
		}
		/* No need to overwrite the data range of a delta chunk */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Fills the chunk table from the EWF version 2 sector table entries
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_fill_v2(
     libewf_chunk_table_t *chunk_table,
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     uint32_t number_of_offsets,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error )
{
	static char *function           = "libewf_chunk_table_fill_v2";
	off64_t previous_chunk_offset   = 0;
	off64_t table_entry_offset      = 0;
	size64_t previous_chunk_size    = 0;
	size_t table_entry_index        = 0;
	uint64_t chunk_offset           = 0;
	uint32_t chunk_data_flags       = 0;
	uint32_t chunk_size             = 0;
	uint32_t previous_range_flags   = 0;
	uint32_t range_flags            = 0;
	int previous_file_io_pool_entry = 0;
	int result                      = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
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
	table_entry_offset = table_section->start_offset + sizeof( ewf_table_header_v2_t );

	while( table_entries_data_size >= sizeof( ewf_table_entry_v2_t ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_entry_v2_t *) table_entries_data )->chunk_data_offset,
		 chunk_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_entry_v2_t *) table_entries_data )->chunk_data_size,
		 chunk_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_entry_v2_t *) table_entries_data )->chunk_data_flags,
		 chunk_data_flags );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk index\t\t: %d\n",
			 function,
			 table_entry_index,
			 chunk_index );

			if( ( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_IS_COMPRESSED ) != 0 )
			 && ( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_USES_PATTERN_FILL ) != 0 ) )
			{
				libcnotify_printf(
				 "%s: table entry: % 8" PRIzd " chunk pattern fill\t: 0x%08" PRIx64 "\n",
				 function,
				 table_entry_index,
				 chunk_offset );
			}
			else
			{
				libcnotify_printf(
				 "%s: table entry: % 8" PRIzd " chunk data offset\t: 0x%08" PRIx64 "\n",
				 function,
				 table_entry_index,
				 chunk_offset );
			}
			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data size\t: %" PRIu32 "\n",
			 function,
			 table_entry_index,
			 chunk_size );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data flags\t: 0x%08" PRIx32 "\n",
			 function,
			 table_entry_index,
			 chunk_data_flags );
		}
#endif
		table_entries_data      += sizeof( ewf_table_entry_v2_t );
		table_entries_data_size -= sizeof( ewf_table_entry_v2_t );

		table_entry_index++;

		range_flags = 0;

		if( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_IS_COMPRESSED ) != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_IS_COMPRESSED;

			if( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_USES_PATTERN_FILL ) != 0 )
			{
				range_flags |= LIBEWF_RANGE_FLAG_USES_PATTERN_FILL;
			}
		}
		if( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_HAS_CHECKSUM ) != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_HAS_CHECKSUM;
		}
/* TODO handle corruption e.g. check for zero data
		if( ( ( chunk_data_flags & 0x00000007UL ) != 1 )
		 || ( ( chunk_data_flags & 0x00000007UL ) != 2 )
		 || ( ( chunk_data_flags & 0x00000007UL ) != 5 ) )
		{
		}
*/
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( chunk_data_flags & ~( 0x00000007UL ) ) != 0 )
			{
				libcnotify_printf(
				 "%s: unsupported chunk data flags: 0x%08" PRIx32 " in table entry: %" PRIzd "\n",
				 function,
				 chunk_data_flags,
				 table_entry_index );
			}
		}
#endif
		if( tainted != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_IS_TAINTED;
		}
		if( ( range_flags & LIBEWF_RANGE_FLAG_USES_PATTERN_FILL ) != 0 )
		{
			chunk_offset = table_entry_offset;
			chunk_size   = 8;
		}
		table_entry_offset += sizeof( ewf_table_entry_v2_t );

		result = libmfdata_list_is_group(
		          chunk_table_list,
		          chunk_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunk: %d is a group.",
			 function,
			 chunk_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libmfdata_list_set_element_by_index(
			     chunk_table_list,
			     chunk_index,
			     file_io_pool_entry,
			     chunk_offset,
			     (size64_t) chunk_size,
			     range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set chunk: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
		}
		else
		{
			if( libmfdata_list_get_data_range_by_index(
			     chunk_table_list,
			     chunk_index,
			     &previous_file_io_pool_entry,
			     &previous_chunk_offset,
			     &previous_chunk_size,
			     &previous_range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data range of chunk: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
			if( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_DELTA ) == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: unable to set data range of chunk: %d value already set.",
				 function,
				 chunk_index );

				return( -1 );
			}
			/* No need to overwrite the data range of a delta chunk */
		}
		chunk_index++;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	return( 1 );
}

/* Corrects the chunk table from the offsets
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_correct_v1(
     libewf_chunk_table_t *chunk_table,
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     int file_io_pool_entry,
     libewf_section_t *table_section,
     off64_t base_offset,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error )
{
	static char *function           = "libewf_chunk_table_correct_v1";
	off64_t last_chunk_offset       = 0;
	off64_t last_chunk_size         = 0;
	off64_t previous_chunk_offset   = 0;
	size64_t previous_chunk_size    = 0;
	uint32_t chunk_size             = 0;
	uint32_t current_offset         = 0;
	uint32_t next_offset            = 0;
	uint32_t previous_range_flags   = 0;
	uint32_t range_flags            = 0;
	uint32_t stored_offset          = 0;
	uint32_t table_entry_index      = 0;
	uint8_t corrupted               = 0;
	uint8_t is_compressed           = 0;
	uint8_t mismatch                = 0;
	uint8_t overflow                = 0;
	uint8_t update_data_range       = 0;
	int previous_file_io_pool_entry = 0;
	int result                      = 0;

	if( chunk_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table.",
		 function );

		return( -1 );
	}
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
	if( base_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
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
	byte_stream_copy_to_uint32_little_endian(
	 ( ( (ewf_table_entry_v1_t *) table_entries_data )[ table_entry_index ] ).chunk_data_offset,
	 stored_offset );

	while( table_entry_index < ( number_of_entries - 1 ) )
	{
		if( overflow == 0 )
		{
			is_compressed  = (uint8_t) ( stored_offset >> 31 );
			current_offset = stored_offset & 0x7fffffffUL;
		}
		else
		{
			current_offset = stored_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( ( (ewf_table_entry_v1_t *) table_entries_data )[ table_entry_index + 1 ] ).chunk_data_offset,
		 stored_offset );

		if( overflow == 0 )
		{
			next_offset = stored_offset & 0x7fffffffUL;
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
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: chunk offset: %" PRIu32 " larger than stored chunk offset: %" PRIu32 ".\n",
					 function,
					 current_offset,
					 stored_offset );
				}
#endif
				corrupted = 1;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
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
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid chunk size value is zero.\n",
				 function );
			}
			corrupted = 1;
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid chunk size value exceeds maximum.\n",
				 function );
			}
			corrupted = 1;
		}
		range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;

		if( is_compressed != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_IS_COMPRESSED;
		}
		if( corrupted != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
		}
		if( tainted != 0 )
		{
			range_flags |= LIBEWF_RANGE_FLAG_IS_TAINTED;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk index\t\t: %d\n",
			 function,
			 table_entry_index,
			 chunk_index );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " base offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 table_entry_index,
			 base_offset );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data offset\t: 0x%08" PRIx32 "\n",
			 function,
			 table_entry_index,
			 current_offset );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data size\t: %" PRIu32 "\n",
			 function,
			 table_entry_index,
			 chunk_size );

			libcnotify_printf(
			 "%s: table entry: % 8" PRIzd " chunk data flags:\n",
			 function,
			 table_entry_index );

			if( is_compressed != 0 )
			{
				libcnotify_printf(
				 "Is compressed\n" );
			}
			else
			{
				libcnotify_printf(
				 "Has checksum\n" );
			}
			if( corrupted != 0 )
			{
				libcnotify_printf(
				 "Is corrupted\n" );
			}
			else if( tainted != 0 )
			{
				libcnotify_printf(
				 "Is tainted\n" );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif
		result = libmfdata_list_is_group(
		          chunk_table_list,
		          chunk_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunk: %d is a group.",
			 function,
			 chunk_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libmfdata_list_set_element_by_index(
			     chunk_table_list,
			     chunk_index,
			     file_io_pool_entry,
			     base_offset + current_offset,
			     (size64_t) chunk_size,
			     range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set chunk: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
		}
		else
		{
			if( libmfdata_list_get_data_range_by_index(
			     chunk_table_list,
			     chunk_index,
			     &previous_file_io_pool_entry,
			     &previous_chunk_offset,
			     &previous_chunk_size,
			     &previous_range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data range of chunk: %d.",
				 function,
				 chunk_index );

				return( -1 );
			}
			if( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_DELTA ) == 0 )
			{
				if( (off64_t) ( base_offset + current_offset ) != previous_chunk_offset )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: chunk: %d offset mismatch.\n",
						 function,
						 chunk_index );
					}
#endif
					mismatch = 1;
				}
				else if( (size64_t) chunk_size != previous_chunk_size )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: chunk: %d size mismatch.\n",
						 function,
						 chunk_index );
					}
#endif
					mismatch = 1;
				}
				else if( ( range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED )
				      != ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: chunk: %d compression flag mismatch.\n",
						 function,
						 chunk_index );
					}
#endif
					mismatch = 1;
				}
				else
				{
					mismatch = 0;
				}
				update_data_range = 0;

				if( mismatch != 0 )
				{
					if( ( corrupted == 0 )
					 && ( tainted == 0 ) )
					{
						update_data_range = 1;
					}
					else if( ( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
					      && ( corrupted == 0 ) )
					{
						update_data_range = 1;
					}
				}
				else if( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_TAINTED ) != 0 )
				{
					update_data_range = 1;
				}
				if( update_data_range != 0 )
				{
					if( libmfdata_list_set_data_range_by_index(
					     chunk_table_list,
					     chunk_index,
					     file_io_pool_entry,
					     base_offset + current_offset,
					     (size64_t) chunk_size,
					     range_flags,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set data range of chunk: %d.",
						 function,
						 chunk_index );

						return( -1 );
					}
				}
			}
		}
		/* This is to compensate for the crappy > 2 GiB segment file solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: chunk offset overflow at: %" PRIu32 ".\n",
				 function,
				 current_offset );
			}
#endif
			overflow      = 1;
			is_compressed = 0;
		}
		chunk_index++;

		table_entry_index++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( ( (ewf_table_entry_v1_t *) table_entries_data )[ table_entry_index ] ).chunk_data_offset,
	 stored_offset );

	if( overflow == 0 )
	{
		is_compressed  = (uint8_t) ( stored_offset >> 31 );
		current_offset = stored_offset & 0x7fffffffUL;
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
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
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: invalid last chunk offset value exceeds table section end offset.\n",
		 function );
	}
#endif
	last_chunk_size -= table_section->size;

	if( last_chunk_size <= 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last chunk size value is zero or less.\n",
			 function );
		}
#endif
		corrupted = 1;
	}
	if( last_chunk_size > (off64_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last chunk size value exceeds maximum.\n",
			 function );
		}
#endif
		corrupted = 1;
	}
	range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;

	if( is_compressed != 0 )
	{
		range_flags |= LIBEWF_RANGE_FLAG_IS_COMPRESSED;
	}
	if( corrupted != 0 )
	{
		range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
	}
	if( tainted != 0 )
	{
		range_flags |= LIBEWF_RANGE_FLAG_IS_TAINTED;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk index\t\t: %d\n",
		 function,
		 table_entry_index,
		 chunk_index );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " base offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 table_entry_index,
		 base_offset );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk data offset\t: 0x%08" PRIx32 "\n",
		 function,
		 table_entry_index,
		 current_offset );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk data size\t: %" PRIu32 " (calculated)\n",
		 function,
		 table_entry_index,
		 last_chunk_size );

		libcnotify_printf(
		 "%s: table entry: % 8" PRIzd " chunk data flags:\n",
		 function,
		 table_entry_index );

		if( is_compressed != 0 )
		{
			libcnotify_printf(
			 "Is compressed\n" );
		}
		else
		{
			libcnotify_printf(
			 "Has checksum\n" );
		}
		if( corrupted != 0 )
		{
			libcnotify_printf(
			 "Is corrupted\n" );
		}
		else if( tainted != 0 )
		{
			libcnotify_printf(
			 "Is tainted\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	result = libmfdata_list_is_group(
		  chunk_table_list,
		  chunk_index,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if chunk: %d is a group.",
		 function,
		 chunk_index );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libmfdata_list_set_element_by_index(
		     chunk_table_list,
		     chunk_index,
		     file_io_pool_entry,
		     last_chunk_offset,
		     (size64_t) last_chunk_size,
		     range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set chunk: %d.",
			 function,
			 chunk_index );

			return( -1 );
		}
	}
	else
	{
		if( libmfdata_list_get_data_range_by_index(
		     chunk_table_list,
		     chunk_index,
		     &previous_file_io_pool_entry,
		     &previous_chunk_offset,
		     &previous_chunk_size,
		     &previous_range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of chunk: %d.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_DELTA ) == 0 )
		{
				if( (off64_t) ( base_offset + current_offset ) != previous_chunk_offset )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: chunk: %d offset mismatch.\n",
						 function,
						 chunk_index );
					}
#endif
					mismatch = 1;
			}
			else if( (size64_t) chunk_size != previous_chunk_size )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: chunk: %d size mismatch.\n",
					 function,
					 chunk_index );
				}
#endif
				mismatch = 1;
			}
			else if( ( range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED )
			      != ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: chunk: %d compression flag mismatch.\n",
					 function,
					 chunk_index );
				}
#endif
				mismatch = 1;
			}
			else
			{
				mismatch = 0;
			}
			update_data_range = 0;

			if( mismatch != 0 )
			{
				if( ( corrupted == 0 )
				 && ( tainted == 0 ) )
				{
					update_data_range = 1;
				}
				else if( ( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
				      && ( corrupted == 0 ) )
				{
					update_data_range = 1;
				}
			}
			else if( ( previous_range_flags & LIBEWF_RANGE_FLAG_IS_TAINTED ) != 0 )
			{
				update_data_range = 1;
			}
			if( update_data_range != 0 )
			{
				if( libmfdata_list_set_data_range_by_index(
				     chunk_table_list,
				     chunk_index,
				     file_io_pool_entry,
				     base_offset + current_offset,
				     (size64_t) chunk_size,
				     range_flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set data range of chunk: %d.",
					 function,
					 chunk_index );

					return( -1 );
				}
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Generates the table entries data from the chunk table
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_table_generate_table_entries_data(
     libmfdata_list_t *chunk_table_list,
     int chunk_index,
     uint8_t format_version,
     uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint32_t number_of_entries,
     off64_t base_offset,
     libcerror_error_t **error )
{
	static char *function        = "libewf_chunk_table_generate_table_entries_data";
	off64_t chunk_offset         = 0;
	size64_t chunk_size          = 0;
	size_t table_entry_data_size = 0;
	uint32_t chunk_data_flags    = 0;
	uint32_t range_flags         = 0;
	uint32_t table_offset        = 0;
	uint32_t table_entry_index   = 0;
	int file_io_pool_entry       = 0;

	if( chunk_table_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk table list.",
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
	if( ( number_of_entries * table_entry_data_size ) > table_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table entries data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid base offset.",
		 function );

		return( -1 );
	}
	for( table_entry_index = 0;
	     table_entry_index < number_of_entries;
	     table_entry_index++ )
	{
		if( libmfdata_list_get_data_range_by_index(
		     chunk_table_list,
		     chunk_index,
		     &file_io_pool_entry,
		     &chunk_offset,
		     &chunk_size,
		     &range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data range of chunk: %d.",
			 function,
			 chunk_index );

			return( -1 );
		}
		if( format_version == 1 )
		{
			chunk_offset -= base_offset;

			if( ( chunk_offset < 0 )
			 || ( chunk_offset > (off64_t) INT32_MAX ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid chunk: %d offset value out of bounds.",
				 function,
				 chunk_index );

				return( -1 );
			}
			table_offset = (uint32_t) chunk_offset;

			if( ( range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
			{
				table_offset |= 0x80000000UL;
			}
			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_table_entry_v1_t *) table_entries_data )->chunk_data_offset,
			 table_offset );
		}
		else if( format_version == 2 )
		{
			if( chunk_size > (size64_t) UINT32_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid chunk: %d size value out of bounds.",
				 function,
				 chunk_index );

				return( -1 );
			}
			chunk_data_flags = 0;

			if( ( range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
			{
				chunk_data_flags |= LIBEWF_CHUNK_DATA_FLAG_IS_COMPRESSED;
			}
			if( ( range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
			{
				chunk_data_flags |= LIBEWF_CHUNK_DATA_FLAG_HAS_CHECKSUM;
			}
			if( ( range_flags & LIBEWF_RANGE_FLAG_USES_PATTERN_FILL ) != 0 )
			{
				chunk_data_flags |= LIBEWF_CHUNK_DATA_FLAG_USES_PATTERN_FILL;
			}
			byte_stream_copy_from_uint64_little_endian(
			 ( (ewf_table_entry_v2_t *) table_entries_data )->chunk_data_offset,
			 chunk_offset );

			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_table_entry_v2_t *) table_entries_data )->chunk_data_size,
			 (uint32_t) chunk_size );

			byte_stream_copy_from_uint32_little_endian(
			 ( (ewf_table_entry_v2_t *) table_entries_data )->chunk_data_flags,
			 chunk_data_flags );
		}
		table_entries_data += table_entry_data_size;

		chunk_index++;
	}
	return( 1 );
}


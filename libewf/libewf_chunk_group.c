/*
 * Chunk group functions
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
#include <types.h>

#include "libewf_chunk_data.h"
#include "libewf_chunk_group.h"
#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfcache.h"
#include "libewf_libfdata.h"
#include "libewf_section.h"
#include "libewf_section_descriptor.h"

#include "ewf_table.h"

/* Creates a chunk group
 * Make sure the value chunk_group is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_initialize(
     libewf_chunk_group_t **chunk_group,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_group_initialize";

	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group.",
		 function );

		return( -1 );
	}
	if( *chunk_group != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk group value already set.",
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
	*chunk_group = memory_allocate_structure(
	                libewf_chunk_group_t );

	if( *chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk group.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk_group,
	     0,
	     sizeof( libewf_chunk_group_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk group.",
		 function );

		memory_free(
		 *chunk_group );

		*chunk_group = NULL;

		return( -1 );
	}
	if( libfdata_list_initialize(
	     &( ( *chunk_group )->chunks_list ),
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libewf_chunk_data_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunks list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *chunk_group != NULL )
	{
		memory_free(
		 *chunk_group );

		*chunk_group = NULL;
	}
	return( -1 );
}

/* Frees a chunk group
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_free(
     libewf_chunk_group_t **chunk_group,
     libcerror_error_t **error )
{
        static char *function = "libewf_chunk_group_free";
	int result            = 1;

	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group.",
		 function );

		return( -1 );
	}
	if( *chunk_group != NULL )
	{
		if( libfdata_list_free(
		     &( ( *chunk_group )->chunks_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunks list.",
			 function );

			result = -1;
		}
		memory_free(
		 *chunk_group );

		*chunk_group = NULL;
	}
	return( result );
}

/* Clones the chunk group
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_clone(
     libewf_chunk_group_t **destination_chunk_group,
     libewf_chunk_group_t *source_chunk_group,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_group_clone";

	if( destination_chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination chunk group.",
		 function );

		return( -1 );
	}
	if( *destination_chunk_group != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination chunk group already set.",
		 function );

		return( -1 );
	}
	if( source_chunk_group == NULL )
	{
		*destination_chunk_group = NULL;

		return( 1 );
	}
	*destination_chunk_group = memory_allocate_structure(
		                    libewf_chunk_group_t );

	if( *destination_chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination chunk group.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_chunk_group,
	     source_chunk_group,
	     sizeof( libewf_chunk_group_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination chunk group.",
		 function );

		memory_free(
		 *destination_chunk_group );

		*destination_chunk_group = NULL;

		return( -1 );
	}
	( *destination_chunk_group )->chunks_list = NULL;

/* TODO clone chunks_list */

	return( 1 );

on_error:
	if( *destination_chunk_group != NULL )
	{
		memory_free(
		 *destination_chunk_group );

		*destination_chunk_group = NULL;
	}
	return( -1 );
}

/* Empties a chunk group
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_empty(
     libewf_chunk_group_t *chunk_group,
     libcerror_error_t **error )
{
        static char *function = "libewf_chunk_group_empty";

	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group.",
		 function );

		return( -1 );
	}
	if( libfdata_list_empty(
	     chunk_group->chunks_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to empty chunks list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Fills the chunks list from the EWF version 1 sector table entries
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_fill_v1(
     libewf_chunk_group_t *chunk_group,
     uint64_t chunk_index,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_descriptor_t *table_section,
     off64_t base_offset,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error )
{
	static char *function          = "libewf_chunk_group_fill_v1";
	size_t data_offset             = 0;
	off64_t chunk_data_end_offset  = 0;
	off64_t last_chunk_data_offset = 0;
	off64_t last_chunk_data_size   = 0;
	uint32_t chunk_data_size       = 0;
	uint32_t current_offset        = 0;
	uint32_t next_offset           = 0;
	uint32_t range_flags           = 0;
	uint32_t stored_offset         = 0;
	uint32_t table_entry_index     = 0;
	uint8_t corrupted              = 0;
	uint8_t is_compressed          = 0;
	uint8_t overflow               = 0;
	int element_index              = 0;

	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group.",
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
	 &( table_entries_data[ data_offset ] ),
	 stored_offset );

	data_offset += sizeof( ewf_table_entry_v1_t );

	for( table_entry_index = 0;
	     table_entry_index < ( number_of_entries - 1 );
	     table_entry_index++ )
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
		 &( table_entries_data[ data_offset ] ),
		 stored_offset );

		data_offset += sizeof( ewf_table_entry_v1_t );

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
					 "%s: entry: %" PRIu32 " offset: %" PRIu32 " larger than stored offset: %" PRIu32 ".\n",
					 function,
					 table_entry_index,
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
				 "%s: entry: %" PRIu32 " offset: %" PRIu32 " larger than next offset: %" PRIu32 ".\n",
				 function,
				 table_entry_index,
				 current_offset,
				 next_offset );
			}
#endif
			chunk_data_size = stored_offset - current_offset;
		}
		else
		{
			chunk_data_size = next_offset - current_offset;
		}
		if( chunk_data_size == 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid entry: %" PRIu32 " size value is zero.\n",
				 function,
				 table_entry_index );
			}
			corrupted = 1;
		}
		if( chunk_data_size > (uint32_t) INT32_MAX )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid entry: %" PRIu32 " size value exceeds maximum.\n",
				 function,
				 table_entry_index );
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
			 "%s: table entry: %05" PRIu32 " chunk\t\t\t: %" PRIu64 "\n",
			 function,
			 table_entry_index,
			 chunk_index );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " base offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 table_entry_index,
			 base_offset );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data offset\t: %" PRIu32 " (0x%08" PRIx32 ") (file offset: 0x%08" PRIx64 ")\n",
			 function,
			 table_entry_index,
			 current_offset,
			 current_offset,
			 base_offset + current_offset );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data size\t\t: %" PRIu32 "\n",
			 function,
			 table_entry_index,
			 chunk_data_size );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data flags:\n",
			 function,
			 table_entry_index );

			if( is_compressed != 0 )
			{
				libcnotify_printf(
				 "\tIs compressed\n" );
			}
			else
			{
				libcnotify_printf(
				 "\tHas checksum\n" );
			}
			if( corrupted != 0 )
			{
				libcnotify_printf(
				 "\tIs corrupted\n" );
			}
			else if( tainted != 0 )
			{
				libcnotify_printf(
				 "\tIs tainted\n" );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		chunk_index++;

		if( libfdata_list_append_element_with_mapped_size(
		     chunk_group->chunks_list,
		     &element_index,
		     file_io_pool_entry,
		     base_offset + current_offset,
		     (size64_t) chunk_data_size,
		     range_flags,
		     chunk_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element: %" PRIu32 " with mapped size to chunks list.",
			 function,
			 table_entry_index );

			return( -1 );
		}
		/* This is to compensate for the crappy > 2 GiB segment file solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_data_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: entry: %" PRIu64 " offset overflow at: %" PRIu32 ".\n",
				 function,
				 table_entry_index,
				 current_offset );
			}
#endif
			overflow      = 1;
			is_compressed = 0;
		}
	}
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
	last_chunk_data_offset = (off64_t) base_offset + current_offset;

	if( last_chunk_data_offset > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid last entry: %" PRIu32 " offset value exceeds maximum.",
		 function,
		 table_entry_index );

		return( -1 );
	}
	/* A table2 section where the chunk data is stored 2 sections before
	 */
	if( ( table_section->type_string_length == 6 )
	 && ( memory_compare(
	       (void *) table_section->type_string,
	       (void *) "table2",
	       6 ) == 0 ) )
	{
		chunk_data_end_offset = table_section->start_offset - table_section->size;
	}
	/* A table2 section where the chunk data is stored 1 section before
	 */
	else if( last_chunk_data_offset < table_section->start_offset )
	{
		chunk_data_end_offset = table_section->start_offset;
	}
	/* A table section that contains the chunk data
	 */
	else if( last_chunk_data_offset < table_section->end_offset )
	{
		chunk_data_end_offset = table_section->end_offset;
	}
	if( last_chunk_data_offset < chunk_data_end_offset )
	{
		last_chunk_data_size = chunk_data_end_offset - last_chunk_data_offset;
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last entry: %" PRIu32 " offset value exceeds table section end offset.\n",
			 function,
			 table_entry_index );
		}
#endif
		corrupted = 1;
	}
	if( last_chunk_data_size <= 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last entry: %" PRIu32 " size value is zero or less.\n",
			 function,
			 table_entry_index );
		}
#endif
		corrupted = 1;
	}
	if( last_chunk_data_size > (off64_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last entry: %" PRIu32 " size value exceeds maximum.\n",
			 function,
			 table_entry_index );
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
		 "%s: table entry: %05" PRIu32 " chunk\t\t\t: %" PRIu64 "\n",
		 function,
		 table_entry_index,
		 chunk_index );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " base offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 table_entry_index,
		 base_offset );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " chunk data offset\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 table_entry_index,
		 current_offset,
		 current_offset );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " chunk data size\t\t: %" PRIu32 " (calculated)\n",
		 function,
		 table_entry_index,
		 last_chunk_data_size );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " chunk data flags:\n",
		 function,
		 table_entry_index );

		if( is_compressed != 0 )
		{
			libcnotify_printf(
			 "\tIs compressed\n" );
		}
		else
		{
			libcnotify_printf(
			 "\tHas checksum\n" );
		}
		if( corrupted != 0 )
		{
			libcnotify_printf(
			 "\tIs corrupted\n" );
		}
		else if( tainted != 0 )
		{
			libcnotify_printf(
			 "\tIs tainted\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfdata_list_append_element_with_mapped_size(
	     chunk_group->chunks_list,
	     &element_index,
	     file_io_pool_entry,
	     last_chunk_data_offset,
	     (size64_t) last_chunk_data_size,
	     range_flags,
	     chunk_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element: %" PRIu32 " with mapped size to chunks list.",
		 function,
		 table_entry_index );

		return( -1 );
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

/* Fills the chunks list from the EWF version 2 sector table entries
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_fill_v2(
     libewf_chunk_group_t *chunk_group,
     uint64_t chunk_index,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_descriptor_t *table_section,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error )
{
	static char *function      = "libewf_chunk_group_fill_v2";
	size_t data_offset         = 0;
	uint64_t chunk_data_offset = 0;
	uint32_t chunk_data_flags  = 0;
	uint32_t chunk_data_size   = 0;
	uint32_t range_flags       = 0;
	uint32_t table_entry_index = 0;
	int element_index          = 0;

	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group.",
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
	for( table_entry_index = 0;
	     table_entry_index < number_of_entries;
	     table_entry_index++ )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (ewf_table_entry_v2_t *) &( table_entries_data[ data_offset ] ) )->chunk_data_offset,
		 chunk_data_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_entry_v2_t *) &( table_entries_data[ data_offset ] ) )->chunk_data_size,
		 chunk_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (ewf_table_entry_v2_t *) &( table_entries_data[ data_offset ] ) )->chunk_data_flags,
		 chunk_data_flags );

		data_offset += sizeof( ewf_table_entry_v2_t );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk\t\t\t: %" PRIu64 "\n",
			 function,
			 table_entry_index,
			 chunk_index );

			if( ( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_IS_COMPRESSED ) != 0 )
			 && ( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_USES_PATTERN_FILL ) != 0 ) )
			{
				libcnotify_printf(
				 "%s: table entry: %05" PRIu32 " chunk pattern fill\t: 0x%08" PRIx64 "\n",
				 function,
				 table_entry_index,
				 chunk_data_offset );
			}
			else
			{
				libcnotify_printf(
				 "%s: table entry: %05" PRIu32 " chunk data offset\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
				 function,
				 table_entry_index,
				 chunk_data_offset,
				 chunk_data_offset );
			}
			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data size\t\t: %" PRIu32 "\n",
			 function,
			 table_entry_index,
			 chunk_data_size );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data flags\t\t: 0x%08" PRIx32 "\n",
			 function,
			 table_entry_index,
			 chunk_data_flags );

			if( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_IS_COMPRESSED ) != 0 )
			{
				libcnotify_printf(
				 "\tIs compressed\n" );
			}
			if( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_HAS_CHECKSUM ) != 0 )
			{
				libcnotify_printf(
				 "\tHas checksum\n" );
			}
			if( ( chunk_data_flags & LIBEWF_CHUNK_DATA_FLAG_USES_PATTERN_FILL ) != 0 )
			{
				libcnotify_printf(
				 "\tUses pattern fill\n" );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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
				 "%s: unsupported chunk data flags: 0x%08" PRIx32 " in table entry: %" PRIu32 "\n",
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
			chunk_data_offset = table_section->start_offset + sizeof( ewf_table_header_v2_t ) + data_offset - sizeof( ewf_table_entry_v2_t );
			chunk_data_size   = 8;
		}
		if( libfdata_list_append_element_with_mapped_size(
		     chunk_group->chunks_list,
		     &element_index,
		     file_io_pool_entry,
		     chunk_data_offset,
		     (size64_t) chunk_data_size,
		     range_flags,
		     chunk_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append element: %" PRIu32 " with mapped size to chunks list.",
			 function,
			 table_entry_index );

			return( -1 );
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

/* Corrects the chunks list from the offsets
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_group_correct_v1(
     libewf_chunk_group_t *chunk_group,
     uint64_t chunk_index,
     size32_t chunk_size,
     int file_io_pool_entry,
     libewf_section_descriptor_t *table_section,
     off64_t base_offset,
     uint32_t number_of_entries,
     const uint8_t *table_entries_data,
     size_t table_entries_data_size,
     uint8_t tainted,
     libcerror_error_t **error )
{
	static char *function              = "libewf_chunk_group_correct_v1";
	off64_t last_chunk_data_offset     = 0;
	off64_t last_chunk_data_size       = 0;
	off64_t previous_chunk_data_offset = 0;
	size64_t previous_chunk_data_size  = 0;
	uint32_t chunk_data_size           = 0;
	uint32_t current_offset            = 0;
	uint32_t next_offset               = 0;
	uint32_t previous_range_flags      = 0;
	uint32_t range_flags               = 0;
	uint32_t stored_offset             = 0;
	uint32_t table_entry_index         = 0;
	uint8_t corrupted                  = 0;
	uint8_t is_compressed              = 0;
	uint8_t mismatch                   = 0;
	uint8_t overflow                   = 0;
	uint8_t update_data_range          = 0;
	int previous_file_io_pool_entry    = 0;

	if( chunk_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk group.",
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
					 "%s: chunk: %" PRIu64 " offset: %" PRIu32 " larger than stored offset: %" PRIu32 ".\n",
					 function,
					 chunk_index,
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
				 "%s: chunk: %" PRIu64 " offset: %" PRIu32 " larger than next offset: %" PRIu32 ".\n",
				 function,
				 chunk_index,
				 current_offset,
				 next_offset );
			}
#endif
			chunk_data_size = stored_offset - current_offset;
		}
		else
		{
			chunk_data_size = next_offset - current_offset;
		}
		if( chunk_data_size == 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid chunk: %" PRIu64 " size value is zero.\n",
				 function,
				 chunk_index );
			}
			corrupted = 1;
		}
		if( chunk_data_size > (uint32_t) INT32_MAX )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid chunk: %" PRIu64 " size value exceeds maximum.\n",
				 function,
				 chunk_index );
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
			 "%s: table entry: %05" PRIu32 " chunk\t\t\t: %" PRIu64 "\n",
			 function,
			 table_entry_index,
			 chunk_index );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " base offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 table_entry_index,
			 base_offset );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data offset\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
			 function,
			 table_entry_index,
			 current_offset,
			 current_offset );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data size\t: %" PRIu32 "\n",
			 function,
			 table_entry_index,
			 chunk_data_size );

			libcnotify_printf(
			 "%s: table entry: %05" PRIu32 " chunk data flags:\n",
			 function,
			 table_entry_index );

			if( is_compressed != 0 )
			{
				libcnotify_printf(
				 "\tIs compressed\n" );
			}
			else
			{
				libcnotify_printf(
				 "\tHas checksum\n" );
			}
			if( corrupted != 0 )
			{
				libcnotify_printf(
				 "\tIs corrupted\n" );
			}
			else if( tainted != 0 )
			{
				libcnotify_printf(
				 "\tIs tainted\n" );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfdata_list_get_element_by_index(
		     chunk_group->chunks_list,
		     table_entry_index,
		     &previous_file_io_pool_entry,
		     &previous_chunk_data_offset,
		     &previous_chunk_data_size,
		     &previous_range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve element: %" PRIu32 " from chunks list.",
			 function,
			 table_entry_index );

			return( -1 );
		}
		if( (off64_t) ( base_offset + current_offset ) != previous_chunk_data_offset )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: chunk: %" PRIu64 " offset mismatch.\n",
				 function,
				 chunk_index );
			}
#endif
			mismatch = 1;
		}
		else if( (size64_t) chunk_data_size != previous_chunk_data_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: chunk: %" PRIu64 " size mismatch.\n",
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
				 "%s: chunk: %" PRIu64 " compression flag mismatch.\n",
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
			if( libfdata_list_set_element_by_index(
			     chunk_group->chunks_list,
			     table_entry_index,
			     file_io_pool_entry,
			     base_offset + current_offset,
			     (size64_t) chunk_data_size,
			     range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set element: %" PRIu32 " in chunks list.",
				 function,
				 table_entry_index );

				return( -1 );
			}
		}
		/* This is to compensate for the crappy > 2 GiB segment file solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_data_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: chunk: %" PRIu64 " offset overflow at: %" PRIu32 ".\n",
				 function,
				 chunk_index,
				 current_offset );
			}
#endif
			overflow      = 1;
			is_compressed = 0;
		}
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
	last_chunk_data_offset = (off64_t) base_offset + current_offset;

	if( last_chunk_data_offset > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid last chunk: %" PRIu64 " offset value exceeds maximum.",
		 function,
		 chunk_index );

		return( -1 );
	}
/* TODO fix for table2 */
	if( last_chunk_data_offset < table_section->start_offset )
	{
		last_chunk_data_size = table_section->start_offset - last_chunk_data_offset;
	}
	else if( last_chunk_data_offset < table_section->end_offset )
	{
		last_chunk_data_size = table_section->end_offset - last_chunk_data_offset;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: invalid last chunk: %" PRIu64 " offset value exceeds table section end offset.\n",
		 function,
		 chunk_index );
	}
#endif
	last_chunk_data_size -= table_section->size;

	if( last_chunk_data_size <= 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last chunk: %" PRIu64 " size value is zero or less.\n",
			 function,
			 chunk_index );
		}
#endif
		corrupted = 1;
	}
	if( last_chunk_data_size > (off64_t) INT32_MAX )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid last chunk: %" PRIu64 " size value exceeds maximum.\n",
			 function,
			 chunk_index );
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
		 "%s: table entry: %05" PRIu32 " chunk\t\t\t: %" PRIu64 "\n",
		 function,
		 table_entry_index,
		 chunk_index );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " base offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 table_entry_index,
		 base_offset );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " chunk data offset\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 table_entry_index,
		 current_offset,
		 current_offset );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " chunk data size\t: %" PRIu32 " (calculated)\n",
		 function,
		 table_entry_index,
		 last_chunk_data_size );

		libcnotify_printf(
		 "%s: table entry: %05" PRIu32 " chunk data flags:\n",
		 function,
		 table_entry_index );

		if( is_compressed != 0 )
		{
			libcnotify_printf(
			 "\tIs compressed\n" );
		}
		else
		{
			libcnotify_printf(
			 "\tHas checksum\n" );
		}
		if( corrupted != 0 )
		{
			libcnotify_printf(
			 "\tIs corrupted\n" );
		}
		else if( tainted != 0 )
		{
			libcnotify_printf(
			 "\tIs tainted\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfdata_list_get_element_by_index(
	     chunk_group->chunks_list,
	     table_entry_index,
	     &previous_file_io_pool_entry,
	     &previous_chunk_data_offset,
	     &previous_chunk_data_size,
	     &previous_range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %" PRIu32 " from chunks list.",
		 function,
		 table_entry_index );

		return( -1 );
	}
	if( (off64_t) ( base_offset + current_offset ) != previous_chunk_data_offset )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: chunk: %" PRIu64 " offset mismatch.\n",
			 function,
			 chunk_index );
		}
#endif
		mismatch = 1;
	}
	else if( (size64_t) chunk_data_size != previous_chunk_data_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: chunk: %" PRIu64 " size mismatch.\n",
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
			 "%s: chunk: %" PRIu64 " compression flag mismatch.\n",
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
		if( libfdata_list_set_element_by_index(
		     chunk_group->chunks_list,
		     table_entry_index,
		     file_io_pool_entry,
		     base_offset + current_offset,
		     (size64_t) chunk_data_size,
		     range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set element: %" PRIu32 " in chunks list.",
			 function,
			 table_entry_index );

			return( -1 );
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


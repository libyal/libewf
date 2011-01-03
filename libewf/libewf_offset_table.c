/*
 * Offset table functions
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

#include "libewf_chunk_value.h"
#include "libewf_definitions.h"
#include "libewf_libbfio.h"
#include "libewf_list_type.h"
#include "libewf_offset_table.h"
#include "libewf_section_list.h"

#include "ewf_definitions.h"
#include "ewf_table.h"

/* Initialize the offset table
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_initialize(
     libewf_offset_table_t **offset_table,
     uint32_t number_of_chunk_values,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_initialize";

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( *offset_table == NULL )
	{
		*offset_table = memory_allocate_structure(
		                 libewf_offset_table_t );

		if( *offset_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create offset table.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *offset_table,
		     0,
		     sizeof( libewf_offset_table_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear offset table.",
			 function );

			goto on_error;
		}
		if( libewf_array_initialize(
		     &( ( *offset_table )->chunk_values ),
		     number_of_chunk_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create chunk values array.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *offset_table != NULL )
	{
		memory_free(
		 *offset_table );

		*offset_table = NULL;
	}
	return( -1 );
}

/* Frees the offset table including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_free(
     libewf_offset_table_t **offset_table,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_free";
	int result            = 1;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( *offset_table != NULL )
	{
		if( libewf_array_free(
		     &( ( *offset_table )->chunk_values ),
		     &libewf_chunk_value_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the chunk values array.",
			 function );

			result = -1;
		}
		memory_free(
		 *offset_table );

		*offset_table = NULL;
	}
	return( result );
}

/* Clones the offset table
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_clone(
     libewf_offset_table_t **destination_offset_table,
     libewf_offset_table_t *source_offset_table,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_clone";

	if( destination_offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination offset table.",
		 function );

		return( -1 );
	}
	if( *destination_offset_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination offset table value already set.",
		 function );

		return( -1 );
	}
	if( source_offset_table == NULL )
	{
		*destination_offset_table = NULL;

		return( 1 );
	}
	*destination_offset_table = memory_allocate_structure(
	                             libewf_offset_table_t );

	if( *destination_offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination offset table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *destination_offset_table,
	     0,
	     sizeof( libewf_offset_table_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear source to destination offset table.",
		 function );

		goto on_error;
	}
	if( libewf_array_clone(
	     &( ( *destination_offset_table )->chunk_values ),
	     source_offset_table->chunk_values,
	     &libewf_chunk_value_free,
	     &libewf_chunk_value_clone,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination chunk values.",
		 function );

		goto on_error;
	}
	( *destination_offset_table )->last_chunk_value_filled   = source_offset_table->last_chunk_value_filled;
	( *destination_offset_table )->last_chunk_value_compared = source_offset_table->last_chunk_value_compared;

	return( 1 );

on_error:
	if( *destination_offset_table != NULL )
	{
		memory_free(
		 *destination_offset_table );

		*destination_offset_table = NULL;
	}
	return( -1 );
}

/* Resizes the offset table
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_resize(
     libewf_offset_table_t *offset_table,
     uint32_t number_of_chunk_values,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_resize";

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( libewf_array_resize(
	     offset_table->chunk_values,
	     number_of_chunk_values,
	     &libewf_chunk_value_free,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize chunk values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of chunk values in the offset table
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_get_number_of_chunk_values(
     libewf_offset_table_t *offset_table,
     uint32_t *number_of_chunk_values,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_get_number_of_chunk_values";
	int number_of_entries = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( number_of_chunk_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of chunk values.",
		 function );

		return( -1 );
	}
	if( libewf_array_get_number_of_entries(
	     offset_table->chunk_values,
	     &number_of_entries,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in the chunk values array.",
		 function );

		return( -1 );
	}
	*number_of_chunk_values = (uint32_t) number_of_entries;

	return( 1 );
}

/* Determines if the chunk exists
 * Returns 1 if the chunk exists, 0 if not or -1 on error
 */
int libewf_offset_table_chunk_exists(
     libewf_offset_table_t *offset_table,
     uint32_t chunk,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value = NULL;
	static char *function             = "libewf_offset_table_get_number_of_chunk_values";
	int number_of_entries             = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( libewf_array_get_number_of_entries(
	     offset_table->chunk_values,
	     &number_of_entries,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in the chunk values array.",
		 function );

		return( -1 );
	}
	if( chunk >= (uint32_t) number_of_entries )
	{
		return( 0 );
	}
	if( libewf_array_get_entry_by_index(
	     offset_table->chunk_values,
	     (int) chunk,
	     (intptr_t **) &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
		 function,
		 chunk );

		return( -1 );
	}
	if( chunk_value == NULL )
	{
		return( 0 );
	}
	if( chunk_value->segment_table_index < 0 )
	{
		return( 0 );
	}
	return( 1 );
}

/* Retrieves the chunk value of a specific chunk
 * Returns 1 if the succesful or -1 on error
 */
int libewf_offset_table_get_chunk_value(
     libewf_offset_table_t *offset_table,
     uint32_t chunk,
     libewf_chunk_value_t **chunk_value,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_get_chunk_value";

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( libewf_array_get_entry_by_index(
	     offset_table->chunk_values,
	     (int) chunk,
	     (intptr_t **) chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
		 function,
		 chunk );

		return( -1 );
	}
	return( 1 );
}

/* Sets the chunk value of a specific chunk
 * Returns 1 if the succesful or -1 on error
 */
int libewf_offset_table_set_chunk_value(
     libewf_offset_table_t *offset_table,
     uint32_t chunk,
     libewf_chunk_value_t *chunk_value,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_set_chunk_value";

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
		 function );

		return( -1 );
	}
	if( libewf_array_set_entry_by_index(
	     offset_table->chunk_values,
	     (int) chunk,
	     (intptr_t *) chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set entry: %" PRIu32 " in chunk values array.",
		 function,
		 chunk );

		return( -1 );
	}
	return( 1 );
}

/* Fills the offset table from the offsets
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_fill(
     libewf_offset_table_t *offset_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t number_of_offsets,
     int segment_table_index,
     uint8_t tainted,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value = NULL;
	static char *function             = "libewf_offset_table_fill";
	uint32_t chunk_size               = 0;
	uint32_t current_offset           = 0;
	uint32_t next_offset              = 0;
	uint32_t offset_iterator          = 0;
	uint32_t stored_offset            = 0;
	uint8_t compressed                = 0;
	uint8_t corrupted                 = 0;
	uint8_t overflow                  = 0;
	int number_of_chunk_values        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *chunk_type                  = NULL;
	char *remarks                     = NULL;
#endif

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
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
	if( libewf_array_get_number_of_entries(
	     offset_table->chunk_values,
	     &number_of_chunk_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of chunk values in the chunk values array.",
		 function );

		return( -1 );
	}
	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( (uint32_t) number_of_chunk_values < ( offset_table->last_chunk_value_filled + number_of_offsets ) )
	{
		if( libewf_array_resize(
		     offset_table->chunk_values,
		     offset_table->last_chunk_value_filled + number_of_offsets,
		     NULL,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk values array.",
			 function );

			return( -1 );
		}
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 stored_offset );

	/* The size of the last chunk must be determined differently
	 */
	while( offset_iterator < ( number_of_offsets - 1 ) )
	{
		if( overflow == 0 )
		{
			compressed     = (uint8_t) ( stored_offset >> 31 );
			current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			current_offset = stored_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 offsets[ offset_iterator + 1 ].offset,
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
		if( compressed == 0 )
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
			 "%s: %s chunk %" PRIu32 " read with: base %" PRIi64 ", offset %" PRIu32 " and size %" PRIu32 "%s.\n",
			 function,
			 chunk_type,
			 offset_table->last_chunk_value_filled,
			 base_offset,
			 current_offset,
			 chunk_size,
			 remarks );
		}
#endif
		if( libewf_chunk_value_initialize(
		     &chunk_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to creacte chunk value: %" PRIu32 "",
			 function,
			 offset_table->last_chunk_value_filled );

			return( -1 );
		}
		if( chunk_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing chunk value: %" PRIu32 ".",
			 function,
			 offset_table->last_chunk_value_filled );

			return( -1 );
		}
		chunk_value->segment_table_index = segment_table_index;
		chunk_value->file_offset         = (off64_t) ( base_offset + current_offset );
		chunk_value->size                = (size_t) chunk_size;
		chunk_value->flags              |= compressed;

		if( corrupted != 0 )
		{
			chunk_value->flags |= LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED;
		}
		else if( tainted != 0 )
		{
			chunk_value->flags |= LIBEWF_CHUNK_VALUE_FLAG_TAINTED;
		}
		if( libewf_array_set_entry_by_index(
		     offset_table->chunk_values,
		     (int) offset_table->last_chunk_value_filled,
		     (intptr_t *) chunk_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set entry: %" PRIu32 " in chunk values array.",
			 function,
			 offset_table->last_chunk_value_filled );

			return( -1 );
		}
		chunk_value = NULL;

		offset_table->last_chunk_value_filled++;

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
			overflow   = 1;
			compressed = 0;
		}
		offset_iterator++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 stored_offset );

	if( overflow == 0 )
	{
		compressed     = (uint8_t) ( stored_offset >> 31 );
		current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = stored_offset;
	}
	corrupted = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( compressed == 0 )
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
		 "%s: %s last chunk %" PRIu32 " read with: base %" PRIi64 " and offset %" PRIu32 "%s.\n",
		 function,
		 chunk_type,
		 offset_table->last_chunk_value_filled,
		 base_offset,
		 current_offset,
		 remarks );
	}
#endif
	if( libewf_chunk_value_initialize(
	     &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk value: %" PRIu32 "",
		 function,
		 offset_table->last_chunk_value_filled );

		return( -1 );
	}
	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk value: %" PRIu32 ".",
		 function,
		 offset_table->last_chunk_value_filled );

		return( -1 );
	}
	chunk_value->segment_table_index = segment_table_index;
	chunk_value->file_offset         = (off64_t) ( base_offset + current_offset );
	chunk_value->flags              |= compressed;

	if( corrupted != 0 )
	{
		chunk_value->flags |= LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED;
	}
	else if( tainted != 0 )
	{
		chunk_value->flags |= LIBEWF_CHUNK_VALUE_FLAG_TAINTED;
	}
	if( libewf_array_set_entry_by_index(
	     offset_table->chunk_values,
	     (int) offset_table->last_chunk_value_filled,
	     (intptr_t *) chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set entry: %" PRIu32 " in chunk values array.",
		 function,
		 offset_table->last_chunk_value_filled );

		return( -1 );
	}
	chunk_value = NULL;

	return( 1 );
}

/* Calculate the last offset and fills the offset table
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_fill_last_offset(
     libewf_offset_table_t *offset_table,
     libewf_list_t *section_list,
     uint8_t tainted,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value                 = NULL;
	libewf_list_element_t *list_element               = NULL;
	libewf_section_list_values_t *section_list_values = NULL;
	static char *function                             = "libewf_offset_table_fill_last_offset";
	off64_t last_offset                               = 0;
	off64_t chunk_size                                = 0;
	uint8_t corrupted                                 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *remarks                                     = NULL;
#endif

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
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
	     offset_table->chunk_values,
	     (int) offset_table->last_chunk_value_filled,
	     (intptr_t **) &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
		 function,
		 offset_table->last_chunk_value_filled );

		return( -1 );
	}
	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk value: %" PRIu32 ".",
		 function,
		 offset_table->last_chunk_value_filled );

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
		section_list_values = (libewf_section_list_values_t *) list_element->value;

		if( section_list_values == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid section list values.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: start offset: %" PRIi64 " last offset: %" PRIi64 " \n",
			 function,
			 section_list_values->start_offset,
			 last_offset );
		}
#endif
		if( ( section_list_values->start_offset < last_offset )
		 && ( last_offset < section_list_values->end_offset ) )
		{
			chunk_size = section_list_values->end_offset - last_offset;

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
			corrupted = 0;

			if( chunk_size == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: invalid chunk size value is zero.\n",
					 function );
				}
#endif
				corrupted = 1;
			}
			if( chunk_size > (off64_t) INT32_MAX )
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
#if defined( HAVE_DEBUG_OUTPUT )
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
				 "%s: last chunk %" PRIu32 " calculated with offset: %" PRIi64 " and size %" PRIzu "%s.\n",
				 function,
				 offset_table->last_chunk_value_filled,
				 last_offset,
				 (size_t) chunk_size,
				 remarks );
			}
#endif
			chunk_value->size = (size_t) chunk_size;

			if( corrupted != 0 )
			{
				chunk_value->flags |= LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED;
			}
			else if( tainted != 0 )
			{
				chunk_value->flags |= LIBEWF_CHUNK_VALUE_FLAG_TAINTED;
			}
			offset_table->last_chunk_value_filled++;

			break;
		}
		list_element = list_element->next_element;
	}
	return( 1 );
}

/* Fills the offsets from the offset table
 * number_of_chunk_values contains the number of chunk values to fill
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_fill_offsets(
     libewf_offset_table_t *offset_table,
     uint32_t offset_table_index,
     uint32_t number_of_chunk_values,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t number_of_offsets,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value = NULL;
	static char *function             = "libewf_offset_table_fill_offsets";
	off64_t offset64_value            = 0;
	uint32_t offset32_value           = 0;
	uint32_t offset_iterator          = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
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
	if( number_of_offsets < number_of_chunk_values )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: number of offsets too small.",
		 function );

		return( -1 );
	}
	for( offset_iterator = 0;
	     offset_iterator < number_of_chunk_values;
	     offset_iterator++ )
	{
		if( libewf_array_get_entry_by_index(
		     offset_table->chunk_values,
		     (int) ( offset_table_index + offset_iterator ),
		     (intptr_t **) &chunk_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
			 function,
			 offset_table_index + offset_iterator );

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
		offset64_value = chunk_value->file_offset - base_offset;

		if( ( offset64_value < 0 )
		 || ( offset64_value > (off64_t) INT32_MAX ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk offset value out of bounds.",
			 function );

			return( -1 );
		}
		offset32_value = (uint32_t) offset64_value;

		if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAG_COMPRESSED ) != 0 )
		{
			offset32_value |= EWF_OFFSET_COMPRESSED_WRITE_MASK;
		}
		byte_stream_copy_from_uint32_little_endian(
		 offsets[ offset_iterator ].offset,
		 offset32_value );
	}
	return( 1 );
}

/* Compares the offsets with the ones in the offset table and makes corrections if necessary
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_compare(
     libewf_offset_table_t *offset_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t number_of_offsets,
     int segment_table_index,
     uint8_t tainted,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value = NULL;
	static char *function             = "libewf_offset_table_compare";
	uint32_t chunk_size               = 0;
	uint32_t current_offset           = 0;
	uint32_t next_offset              = 0;
	uint32_t offset_iterator          = 0;
	uint32_t stored_offset            = 0;
	uint8_t compressed                = 0;
	uint8_t corrupted                 = 0;
	uint8_t mismatch                  = 0;
	uint8_t overflow                  = 0;
	int number_of_chunk_values        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *chunk_type                  = NULL;
	char *remarks                     = NULL;
#endif

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
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
	if( libewf_array_get_number_of_entries(
	     offset_table->chunk_values,
	     &number_of_chunk_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of chunk values in the chunk values array.",
		 function );

		return( -1 );
	}
	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( (uint32_t) number_of_chunk_values < ( offset_table->last_chunk_value_compared + number_of_offsets ) )
	{
		if( libewf_array_resize(
		     offset_table->chunk_values,
		     offset_table->last_chunk_value_compared + number_of_offsets,
		     NULL,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk values array.",
			 function );

			return( -1 );
		}
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 stored_offset );

	/* The size of the last chunk must be determined differently
	 */
	while( offset_iterator < ( number_of_offsets - 1 ) )
	{
		if( overflow == 0 )
		{
			compressed     = (uint8_t) ( stored_offset >> 31 );
			current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			current_offset = stored_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 offsets[ offset_iterator + 1 ].offset,
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
		     offset_table->chunk_values,
		     (int) offset_table->last_chunk_value_compared,
		     (intptr_t **) &chunk_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
			 function,
			 offset_table->last_chunk_value_compared );

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
				 offset_table->last_chunk_value_compared );
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
				 offset_table->last_chunk_value_compared );
			}
#endif

			mismatch = 1;
		}
		else if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAG_COMPRESSED ) != compressed )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: compressed mismatch for chunk value: %" PRIu32 ".\n",
				 function,
				 offset_table->last_chunk_value_compared );
			}
#endif
			mismatch = 1;
		}
		else
		{
			mismatch = 0;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( compressed == 0 )
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
			 offset_table->last_chunk_value_compared,
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
			chunk_value->flags              |= compressed;
			chunk_value->flags              &= ~( LIBEWF_CHUNK_VALUE_FLAG_TAINTED | LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED ) ;
		}
		offset_table->last_chunk_value_compared++;

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
			overflow   = 1;
			compressed = 0;
		}
		offset_iterator++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 stored_offset );

	if( overflow == 0 )
	{
		compressed     = (uint8_t) ( stored_offset >> 31 );
		current_offset = stored_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = stored_offset;
	}
	corrupted = 0;

	if( libewf_array_get_entry_by_index(
	     offset_table->chunk_values,
	     (int) offset_table->last_chunk_value_compared,
	     (intptr_t **) &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
		 function,
		 offset_table->last_chunk_value_compared );

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
			 offset_table->last_chunk_value_compared );
		}
#endif

		mismatch = 1;
	}
	else if( ( chunk_value->flags & LIBEWF_CHUNK_VALUE_FLAG_COMPRESSED ) != compressed )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: compressed mismatch for chunk value: %" PRIu32 ".\n",
			 function,
			 offset_table->last_chunk_value_compared );
		}
#endif
		mismatch = 1;
	}
	else
	{
		mismatch = 0;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( compressed == 0 )
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
		 offset_table->last_chunk_value_compared,
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
		chunk_value->flags              |= compressed;
		chunk_value->flags              &= ~( LIBEWF_CHUNK_VALUE_FLAG_TAINTED | LIBEWF_CHUNK_VALUE_FLAG_CORRUPTED ) ;
	}
	return( 1 );
}

/* Calculate the last offset and compares it with the one in the offset table and makes corrections if necessary
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_compare_last_offset(
     libewf_offset_table_t *offset_table,
     libewf_list_t *section_list,
     uint8_t tainted,
     liberror_error_t **error )
{
	libewf_chunk_value_t *chunk_value                 = NULL;
	libewf_list_element_t *list_element               = NULL;
	libewf_section_list_values_t *section_list_values = NULL;
	static char *function                             = "libewf_offset_table_fill_last_offset";
	off64_t last_offset                               = 0;
	off64_t chunk_size                                = 0;
	uint8_t corrupted                                 = 0;
	uint8_t mismatch                                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *remarks                                     = NULL;
#endif

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset table.",
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
	     offset_table->chunk_values,
	     (int) offset_table->last_chunk_value_compared,
	     (intptr_t **) &chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu32 " from chunk values array.",
		 function,
		 offset_table->last_chunk_value_compared );

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
		section_list_values = (libewf_section_list_values_t *) list_element->value;

		if( section_list_values == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid section list values.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: start offset: %" PRIi64 " last offset: %" PRIi64 " \n",
			 function,
			 section_list_values->start_offset,
			 last_offset );
		}
#endif
		if( ( section_list_values->start_offset < last_offset )
		 && ( last_offset < section_list_values->end_offset ) )
		{
			chunk_size = section_list_values->end_offset - last_offset;

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
					 offset_table->last_chunk_value_compared );
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
				 offset_table->last_chunk_value_compared,
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
			offset_table->last_chunk_value_compared++;

			break;
		}
		list_element = list_element->next_element;
	}
	return( 1 );
}


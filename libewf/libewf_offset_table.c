/*
 * Offset table functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#include "libewf_chunk_offset.h"
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
     uint32_t amount_of_chunk_offsets,
     liberror_error_t **error )
{
	static char *function    = "libewf_offset_table_initialize";
	size_t chunk_offset_size = 0;

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
		chunk_offset_size = sizeof( libewf_chunk_offset_t ) * amount_of_chunk_offsets;

		if( chunk_offset_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk offset size value exceeds maximum.",
			 function );

			return( -1 );
		}
		*offset_table = (libewf_offset_table_t *) memory_allocate(
		                                           sizeof( libewf_offset_table_t ) );

		if( *offset_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create offset table.",
			 function );

			return( -1 );
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

			memory_free(
			 *offset_table );

			*offset_table = NULL;

			return( -1 );
		}
		if( amount_of_chunk_offsets > 0 )
		{
			( *offset_table )->chunk_offset = (libewf_chunk_offset_t *) memory_allocate(
			                                                             chunk_offset_size );

			if( ( *offset_table )->chunk_offset == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create chunk offsets.",
				 function );

				memory_free(
				 *offset_table );

				*offset_table = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *offset_table )->chunk_offset,
			     0,
			     chunk_offset_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear chunk offsets.",
				 function );

				memory_free(
				 ( *offset_table )->chunk_offset );
				memory_free(
				 *offset_table );

				*offset_table = NULL;

				return( -1 );
			}
		}
		( *offset_table )->amount_of_chunk_offsets = amount_of_chunk_offsets;
	}
	return( 1 );
}

/* Frees the offset table including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_free(
     libewf_offset_table_t **offset_table,
     liberror_error_t **error )
{
	static char *function = "libewf_offset_table_free";

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
		/* The segment file references are freed in the segment table
		 */
		if( ( *offset_table )->chunk_offset != NULL )
		{
			memory_free(
			 ( *offset_table )->chunk_offset );
		}
		memory_free(
		 *offset_table );

		*offset_table = NULL;
	}
	return( 1 );
}

/* Resizes the offset table
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_resize(
     libewf_offset_table_t *offset_table,
     uint32_t amount_of_chunk_offsets,
     liberror_error_t **error )
{
	void *reallocation       = NULL;
	static char *function    = "libewf_offset_table_resize";
	size_t chunk_offset_size = 0;

	if( offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset_table.",
		 function );

		return( -1 );
	}
	if( offset_table->amount_of_chunk_offsets < amount_of_chunk_offsets )
	{
		chunk_offset_size = sizeof( libewf_chunk_offset_t ) * amount_of_chunk_offsets;

		if( chunk_offset_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk offset size value exceeds maximum.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
				offset_table->chunk_offset,
				chunk_offset_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize chunk offsets.",
			 function );

			return( -1 );
		}
		offset_table->chunk_offset = (libewf_chunk_offset_t *) reallocation;

		if( memory_set(
		     &( offset_table->chunk_offset[ offset_table->amount_of_chunk_offsets ] ),
		     0,
		     ( sizeof( libewf_chunk_offset_t ) * ( amount_of_chunk_offsets - offset_table->amount_of_chunk_offsets ) ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear chunk offsets.",
			 function );

			return( -1 );
		}
		offset_table->amount_of_chunk_offsets = amount_of_chunk_offsets;
	}
	return( 1 );
}

/* Fills the offset table from the table offsets
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_fill(
     libewf_offset_table_t *offset_table,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t amount_of_chunks,
     libewf_segment_file_handle_t *segment_file_handle,
     uint8_t tainted,
     liberror_error_t **error )
{
#if defined( HAVE_VERBOSE_OUTPUT )
	char *chunk_type                    = NULL;
	char *remarks                       = NULL;
#endif

	libewf_chunk_offset_t *chunk_offset = NULL;
	static char *function               = "libewf_offset_table_fill";
	uint32_t chunk_size                 = 0;
	uint32_t current_offset             = 0;
	uint32_t next_offset                = 0;
	uint32_t raw_offset                 = 0;
	uint32_t offset_iterator            = 0;
	uint8_t compressed                  = 0;
	uint8_t corrupted                   = 0;
	uint8_t overflow                    = 0;

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
	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( offset_table->amount_of_chunk_offsets < ( offset_table->last_chunk_offset_filled + amount_of_chunks ) )
	{
		if( libewf_offset_table_resize(
		     offset_table,
		     offset_table->last_chunk_offset_filled + amount_of_chunks,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize offset table.",
			 function );

			return( -1 );
		}
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 raw_offset );

	/* The size of the last chunk must be determined differently
	 */
	while( offset_iterator < ( amount_of_chunks - 1 ) )
	{
		if( overflow == 0 )
		{
			compressed     = (uint8_t) ( raw_offset >> 31 );
			current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			current_offset = raw_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 offsets[ offset_iterator + 1 ].offset,
		 raw_offset );

		if( overflow == 0 )
		{
			next_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			next_offset = raw_offset;
		}
		corrupted = 0;

		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6.7
		 */
		if( next_offset < current_offset )
		{
			if( raw_offset < current_offset )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				libnotify_verbose_printf(
				 "%s: chunk offset %" PRIu32 " larger than raw %" PRIu32 ".\n",
				 function,
				 current_offset,
				 raw_offset );
#endif

				corrupted = 1;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			else
			{
				libnotify_verbose_printf(
				 "%s: chunk offset %" PRIu32 " larger than next %" PRIu32 ".\n",
				 function,
				 current_offset,
				 next_offset );
			}
#endif
			chunk_size = raw_offset - current_offset;
		}
		else
		{
			chunk_size = next_offset - current_offset;
		}
		if( chunk_size == 0 )
		{
			libnotify_verbose_printf(
			 "%s: invalid chunk size value is zero.\n",
			 function );

			corrupted = 1;
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
			libnotify_verbose_printf(
			 "%s: invalid chunk size value exceeds maximum.\n",
			 function );

			corrupted = 1;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
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
		libnotify_verbose_printf(
		 "%s: %s chunk %" PRIu32 " read with: base %" PRIu64 ", offset %" PRIu32 " and size %" PRIu32 "%s.\n",
		 function,
		 chunk_type,
		 offset_table->last_chunk_offset_filled,
		 base_offset,
		 current_offset,
		 chunk_size,
		 remarks );
#endif

		chunk_offset = &( offset_table->chunk_offset[ offset_table->last_chunk_offset_filled ] );

		chunk_offset->segment_file_handle = segment_file_handle;
		chunk_offset->file_offset         = (off64_t) ( base_offset + current_offset );
		chunk_offset->size                = (size_t) chunk_size;
		chunk_offset->flags              |= compressed;

		if( corrupted != 0 )
		{
			chunk_offset->flags |= LIBEWF_CHUNK_OFFSET_FLAGS_CORRUPTED;
		}
		else if( tainted != 0 )
		{
			chunk_offset->flags |= LIBEWF_CHUNK_OFFSET_FLAGS_TAINTED;
		}
		offset_table->last_chunk_offset_filled++;

		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: chunk offset overflow at: %" PRIu32 ".\n",
			 function,
			 current_offset );
#endif

			overflow   = 1;
			compressed = 0;
		}
		offset_iterator++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 raw_offset );

	if( overflow == 0 )
	{
		compressed     = (uint8_t) ( raw_offset >> 31 );
		current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = raw_offset;
	}
	corrupted = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
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
	libnotify_verbose_printf(
	 "%s: %s last chunk %" PRIu32 " read with: base %" PRIu64 " and offset %" PRIu32 "%s.\n",
	 function,
	 chunk_type,
	 offset_table->last_chunk_offset_filled,
	 base_offset,
	 current_offset,
	 remarks );
#endif

	chunk_offset = &( offset_table->chunk_offset[ offset_table->last_chunk_offset_filled ] );

	chunk_offset->segment_file_handle = segment_file_handle;
	chunk_offset->file_offset         = (off64_t) ( base_offset + current_offset );
	chunk_offset->flags              |= compressed;

	if( corrupted != 0 )
	{
		chunk_offset->flags |= LIBEWF_CHUNK_OFFSET_FLAGS_CORRUPTED;
	}
	else if( tainted != 0 )
	{
		chunk_offset->flags |= LIBEWF_CHUNK_OFFSET_FLAGS_TAINTED;
	}
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
#if defined( HAVE_VERBOSE_OUTPUT )
	char *remarks                                     = NULL;
#endif

	libewf_chunk_offset_t *chunk_offset               = NULL;
	libewf_list_element_t *list_element               = NULL;
	libewf_section_list_values_t *section_list_values = NULL;
	static char *function                             = "libewf_offset_table_fill_last_offset";
	off64_t last_offset                               = 0;
	off64_t chunk_size                                = 0;
	uint8_t corrupted                                 = 0;

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
	if( offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset table - missing chunk offsets.",
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
	/* There is no indication how large the last chunk is. The only thing known is where it starts.
	 * However it can be determined where the next section starts within the file.
	 * The size of the last chunk is determined by subtracting the last offset from the offset of the section that follows.
	 */
	list_element = section_list->first;
	chunk_offset = &( offset_table->chunk_offset[ offset_table->last_chunk_offset_filled ] );
	last_offset  = chunk_offset->file_offset;

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
		libnotify_verbose_printf(
		 "%s: start offset: %" PRIi64 " last offset: %" PRIi64 " \n",
		 function,
		 section_list_values->start_offset,
		 last_offset );
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
#if defined( HAVE_VERBOSE_OUTPUT )
				libnotify_verbose_printf(
				 "%s: invalid chunk size value is zero.\n",
				 function );
#endif

				corrupted = 1;
			}
			if( chunk_size > (off64_t) INT32_MAX )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				libnotify_verbose_printf(
				 "%s: invalid chunk size value exceeds maximum.\n",
				 function );
#endif

				corrupted = 1;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
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
			libnotify_verbose_printf(
			 "%s: last chunk %" PRIu32 " calculated with offset: %" PRIu64 " and size %" PRIzu "%s.\n",
			 function,
			 offset_table->last_chunk_offset_filled,
			 last_offset,
			 (size_t) chunk_size,
			 remarks );
#endif

			chunk_offset->size = (size_t) chunk_size;

			if( corrupted != 0 )
			{
				chunk_offset->flags |= LIBEWF_CHUNK_OFFSET_FLAGS_CORRUPTED;
			}
			else if( tainted != 0 )
			{
				chunk_offset->flags |= LIBEWF_CHUNK_OFFSET_FLAGS_TAINTED;
			}
			offset_table->last_chunk_offset_filled++;

			break;
		}
		list_element = list_element->next;
	}
	return( 1 );
}

/* Fills the offsets from the offset table
 * amount_of_chunk_offsets contains the amount of chunk offsets to fill
 * Returns 1 if successful or -1 on error
 */
int libewf_offset_table_fill_offsets(
     libewf_offset_table_t *offset_table,
     uint32_t offset_table_index,
     uint32_t amount_of_chunk_offsets,
     off64_t base_offset,
     ewf_table_offset_t *offsets,
     uint32_t amount_of_offsets,
     liberror_error_t **error )
{
	libewf_chunk_offset_t *chunk_offset = NULL;
	static char *function               = "libewf_offset_table_fill_offsets";
	off64_t offset64_value              = 0;
	uint32_t offset32_value             = 0;
	uint32_t offset_iterator            = 0;

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
	if( offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset table - missing chunk offsets.",
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
		 "%s: invalid table offsets.",
		 function );

		return( -1 );
	}
	if( amount_of_offsets < amount_of_chunk_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: amount of offsets too small.",
		 function );

		return( -1 );
	}
	for( offset_iterator = 0;
	     offset_iterator < amount_of_chunk_offsets;
	     offset_iterator++ )
	{
		chunk_offset   = &( offset_table->chunk_offset[ offset_table_index + offset_iterator ] );
		offset64_value = chunk_offset->file_offset - base_offset;

		if( ( offset64_value < 0 )
		 || ( offset64_value > (off64_t) INT32_MAX ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk offset value.",
			 function );

			return( -1 );
		}
		offset32_value = (uint32_t) offset64_value;

		if( ( chunk_offset->flags & LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED ) == LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED )
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
     uint32_t amount_of_chunks,
     libewf_segment_file_handle_t *segment_file_handle,
     uint8_t tainted,
     liberror_error_t **error )
{
#if defined( HAVE_VERBOSE_OUTPUT )
	char *chunk_type                    = NULL;
	char *remarks                       = NULL;
#endif

	libewf_chunk_offset_t *chunk_offset = NULL;
	static char *function               = "libewf_offset_table_compare";
	uint32_t chunk_size                 = 0;
	uint32_t current_offset             = 0;
	uint32_t next_offset                = 0;
	uint32_t raw_offset                 = 0;
	uint32_t offset_iterator            = 0;
	uint8_t compressed                  = 0;
	uint8_t corrupted                   = 0;
	uint8_t mismatch                    = 0;
	uint8_t overflow                    = 0;

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
	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file.",
		 function );

		return( -1 );
	}
	/* Allocate additional entries in the offset table if needed
	 * - a single reallocation saves processing time
	 */
	if( ( offset_table->amount_of_chunk_offsets < ( offset_table->last_chunk_offset_compared + amount_of_chunks ) )
	 && ( libewf_offset_table_resize(
	       offset_table,
	       offset_table->last_chunk_offset_compared + amount_of_chunks,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize offset table.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 raw_offset );

	/* The size of the last chunk must be determined differently
	 */
	while( offset_iterator < ( amount_of_chunks - 1 ) )
	{
		if( overflow == 0 )
		{
			compressed     = (uint8_t) ( raw_offset >> 31 );
			current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			current_offset = raw_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 offsets[ offset_iterator + 1 ].offset,
		 raw_offset );

		if( overflow == 0 )
		{
			next_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
		}
		else
		{
			next_offset = raw_offset;
		}
		corrupted = 0;

		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6.7
		 */
		if( next_offset < current_offset )
		{
			if( raw_offset < current_offset )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				libnotify_verbose_printf(
				 "%s: chunk offset %" PRIu32 " larger than raw %" PRIu32 ".\n",
				 function,
				 current_offset,
				 raw_offset );
#endif

				corrupted = 1;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			else
			{
				libnotify_verbose_printf(
				 "%s: chunk offset %" PRIu32 " larger than next %" PRIu32 ".\n",
				 function,
				 current_offset,
				 next_offset );
			}
#endif

			chunk_size = raw_offset - current_offset;
		}
		else
		{
			chunk_size = next_offset - current_offset;
		}

		if( chunk_size == 0 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: invalid chunk size - size is zero.\n",
			 function );
#endif

			corrupted = 1;
		}
		if( chunk_size > (uint32_t) INT32_MAX )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: invalid chunk size value exceeds maximum.\n",
			 function );
#endif

			corrupted = 1;
		}
		chunk_offset = &( offset_table->chunk_offset[ offset_table->last_chunk_offset_compared ] );

		if( chunk_offset->file_offset != (off64_t) ( base_offset + current_offset ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: file offset mismatch for chunk offset: %" PRIu32 ".\n",
			 function,
			 offset_table->last_chunk_offset_compared );
#endif

			mismatch = 1;
		}
		else if( chunk_offset->size != (size_t) chunk_size )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: chunk size mismatch for chunk offset: %" PRIu32 ".\n",
			 function,
			 offset_table->last_chunk_offset_compared );
#endif

			mismatch = 1;
		}
		else if( ( chunk_offset->flags & LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED ) != compressed )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: compressed mismatch for chunk offset: %" PRIu32 ".\n",
			 function,
			 offset_table->last_chunk_offset_compared );
#endif

			mismatch = 1;
		}
		else
		{
			mismatch = 0;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
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
		libnotify_verbose_printf(
		 "%s: %s chunk %" PRIu32 " read with: base %" PRIu64 ", offset %" PRIu32 " and size %" PRIu32 "%s.\n",
		 function,
		 chunk_type,
		 offset_table->last_chunk_offset_compared,
		 base_offset,
		 current_offset,
		 chunk_size,
		 remarks );
#endif

		if( ( corrupted == 0 )
		 && ( tainted == 0 )
		 && ( mismatch == 1 ) )
		{
			chunk_offset->segment_file_handle = segment_file_handle;
			chunk_offset->file_offset         = (off64_t) ( base_offset + current_offset );
			chunk_offset->size                = (size_t) chunk_size;
			chunk_offset->flags              |= compressed;
			chunk_offset->flags              &= ~ ( LIBEWF_CHUNK_OFFSET_FLAGS_TAINTED | LIBEWF_CHUNK_OFFSET_FLAGS_CORRUPTED ) ;
		}
		offset_table->last_chunk_offset_compared++;

		/* This is to compensate for the crappy >2Gb segment file
		 * solution in EnCase 6.7
		 */
		if( ( overflow == 0 )
		 && ( ( current_offset + chunk_size ) > (uint32_t) INT32_MAX ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: chunk offset overflow at: %" PRIu32 ".\n",
			 function,
			 current_offset );
#endif

			overflow   = 1;
			compressed = 0;
		}
		offset_iterator++;
	}
	byte_stream_copy_to_uint32_little_endian(
	 offsets[ offset_iterator ].offset,
	 raw_offset );

	if( overflow == 0 )
	{
		compressed     = (uint8_t) ( raw_offset >> 31 );
		current_offset = raw_offset & EWF_OFFSET_COMPRESSED_READ_MASK;
	}
	else
	{
		current_offset = raw_offset;
	}
	corrupted = 0;

	chunk_offset = &( offset_table->chunk_offset[ offset_table->last_chunk_offset_compared ] );

	if( chunk_offset->file_offset != (off64_t) ( base_offset + current_offset ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libnotify_verbose_printf(
		 "%s: file offset mismatch for chunk offset: %" PRIu32 ".\n",
		 function,
		 offset_table->last_chunk_offset_compared );
#endif

		mismatch = 1;
	}
	else if( ( chunk_offset->flags & LIBEWF_CHUNK_OFFSET_FLAGS_COMPRESSED ) != compressed )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libnotify_verbose_printf(
		 "%s: compressed mismatch for chunk offset: %" PRIu32 ".\n",
		 function,
		 offset_table->last_chunk_offset_compared );
#endif

		mismatch = 1;
	}
	else
	{
		mismatch = 0;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
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
	libnotify_verbose_printf(
	 "%s: %s last chunk %" PRIu32 " read with: base %" PRIu64 " and offset %" PRIu32 "%s.\n",
	 function,
	 chunk_type,
	 offset_table->last_chunk_offset_compared,
	 base_offset,
	 current_offset,
	 remarks );
#endif

	if( ( corrupted == 0 )
	 && ( tainted == 0 )
	 && ( mismatch == 1 ) )
	{
		chunk_offset->segment_file_handle = segment_file_handle;
		chunk_offset->file_offset         = (off64_t) ( base_offset + current_offset );
		chunk_offset->flags              |= compressed;
		chunk_offset->flags              &= ~ ( LIBEWF_CHUNK_OFFSET_FLAGS_TAINTED | LIBEWF_CHUNK_OFFSET_FLAGS_CORRUPTED ) ;
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
#if defined( HAVE_VERBOSE_OUTPUT )
	char *remarks                                     = NULL;
#endif

	libewf_chunk_offset_t *chunk_offset               = NULL;
	libewf_list_element_t *list_element               = NULL;
	libewf_section_list_values_t *section_list_values = NULL;
	static char *function                             = "libewf_offset_table_fill_last_offset";
	off64_t last_offset                               = 0;
	off64_t chunk_size                                = 0;
	uint8_t corrupted                                 = 0;
	uint8_t mismatch                                  = 0;

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
	if( offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset table - missing chunk offsets.",
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
	/* There is no indication how large the last chunk is. The only thing known is where it starts.
	 * However it can be determined where the next section starts within the file.
	 * The size of the last chunk is determined by subtracting the last offset from the offset of the section that follows.
	 */
	list_element = section_list->first;
	chunk_offset = &( offset_table->chunk_offset[ offset_table->last_chunk_offset_compared ] );
	last_offset  = chunk_offset->file_offset;

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
		libnotify_verbose_printf(
		 "%s: start offset: %" PRIi64 " last offset: %" PRIi64 " \n",
		 function,
		 section_list_values->start_offset,
		 last_offset );
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
				libnotify_verbose_printf(
				 "%s: invalid chunk size - size is zero.\n",
				 function );

				corrupted = 1;
			}
			if( chunk_size > (off64_t) INT32_MAX )
			{
				libnotify_verbose_printf(
				 "%s: invalid chunk size value exceeds maximum.\n",
				 function );

				corrupted = 1;
			}
			if( chunk_offset->size != (size_t) chunk_size )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				libnotify_verbose_printf(
				 "%s: chunk size mismatch for chunk offset: %" PRIu32 ".\n",
				 function,
				 offset_table->last_chunk_offset_compared );
#endif

				mismatch = 1;
			}
			else
			{
				mismatch = 0;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
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
			libnotify_verbose_printf(
			 "%s: last chunk %" PRIu32 " calculated with offset: %" PRIu64 " and size %" PRIzu "%s.\n",
			 function,
			 offset_table->last_chunk_offset_compared,
			 last_offset,
			 (size_t) chunk_size,
			 remarks );
#endif

			if( ( corrupted == 0 )
			 && ( tainted == 0 )
			 && ( mismatch == 1 ) )
			{
				chunk_offset->size   = (size_t) chunk_size;
				chunk_offset->flags &= ~ ( LIBEWF_CHUNK_OFFSET_FLAGS_TAINTED | LIBEWF_CHUNK_OFFSET_FLAGS_CORRUPTED ) ;
			}
			offset_table->last_chunk_offset_compared++;

			break;
		}
		list_element = list_element->next;
	}
	return( 1 );
}

/* Seeks a certain chunk offset within the offset table
 * Returns the chunk segment file offset if the seek is successful or -1 on error
 */
off64_t libewf_offset_table_seek_chunk_offset(
         libewf_offset_table_t *offset_table,
         uint32_t chunk,
         libbfio_pool_t *file_io_pool,
         liberror_error_t **error )
{
	static char *function = "libewf_segment_table_seek_chunk_offset";

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
	if( offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset table - missing chunk offsets.",
		 function );

		return( -1 );
	}
	if( chunk >= offset_table->amount_of_chunk_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: chunk: %" PRIu32 " out of range [0,%" PRIu32 "].",
		 function,
		 chunk,
		 offset_table->amount_of_chunk_offsets - 1 );

		return( -1 );
	}
	if( offset_table->chunk_offset[ chunk ].segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment file handle for chunk: %" PRIu32 ".",
		 function,
		 chunk );

		return( -1 );
	}
	if( libbfio_pool_seek_offset(
	     file_io_pool,
	     offset_table->chunk_offset[ chunk ].segment_file_handle->file_io_pool_entry,
	     offset_table->chunk_offset[ chunk ].file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to find chunk offset: %" PRIi64 ".",
		 function,
		 offset_table->chunk_offset[ chunk ].file_offset );

		return( -1 );
	}
	return( offset_table->chunk_offset[ chunk ].file_offset );
}


/*
 * Chunk descriptor functions
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

#include "libewf_chunk_descriptor.h"
#include "libewf_definitions.h"
#include "libewf_libcerror.h"

#include "ewf_table.h"

/* Creates a chunk descriptor
 * Make sure the value chunk_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_descriptor_initialize(
     libewf_chunk_descriptor_t **chunk_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_descriptor_initialize";

	if( chunk_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk descriptor.",
		 function );

		return( -1 );
	}
	if( *chunk_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk descriptor value already set.",
		 function );

		return( -1 );
	}
	*chunk_descriptor = memory_allocate_structure(
	                     libewf_chunk_descriptor_t );

	if( *chunk_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk_descriptor,
	     0,
	     sizeof( libewf_chunk_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *chunk_descriptor != NULL )
	{
		memory_free(
		 *chunk_descriptor );

		*chunk_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a chunk descriptor
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_descriptor_free(
     libewf_chunk_descriptor_t **chunk_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_descriptor_free";

	if( chunk_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk descriptor.",
		 function );

		return( -1 );
	}
	if( *chunk_descriptor != NULL )
	{
		memory_free(
		 *chunk_descriptor );

		*chunk_descriptor = NULL;
	}
	return( 1 );
}

/* Clones the chunk descriptor
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_descriptor_clone(
     libewf_chunk_descriptor_t **destination_chunk_descriptor,
     libewf_chunk_descriptor_t *source_chunk_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_descriptor_clone";

	if( destination_chunk_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination chunk descriptor.",
		 function );

		return( -1 );
	}
	if( *destination_chunk_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination chunk descriptor value already set.",
		 function );

		return( -1 );
	}
	if( source_chunk_descriptor == NULL )
	{
		*destination_chunk_descriptor = NULL;

		return( 1 );
	}
	*destination_chunk_descriptor = memory_allocate_structure(
	                                 libewf_chunk_descriptor_t );

	if( *destination_chunk_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination chunk descriptor.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_chunk_descriptor,
	     source_chunk_descriptor,
	     sizeof( libewf_chunk_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination chunk descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_chunk_descriptor != NULL )
	{
		memory_free(
		 *destination_chunk_descriptor );

		*destination_chunk_descriptor = NULL;
	}
	return( -1 );
}

/* Writes a chunk descriptor
 * Returns the number of bytes written or -1 on error
 */
int libewf_chunk_descriptor_write_data(
     libewf_chunk_descriptor_t *chunk_descriptor,
     uint8_t *data,
     size_t data_size,
     off64_t base_offset,
     uint8_t format_version,
     libcerror_error_t **error )
{
	static char *function             = "libewf_chunk_descriptor_write_data";
	size64_t chunk_data_size          = 0;
	size_t chunk_descriptor_data_size = 0;
	off64_t chunk_data_offset         = 0;
	uint32_t chunk_data_flags         = 0;
	uint32_t range_flags              = 0;
	uint32_t table_offset             = 0;

	if( chunk_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk descriptor.",
		 function );

		return( -1 );
	}
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
	if( format_version == 1 )
	{
		chunk_descriptor_data_size = sizeof( ewf_table_entry_v1_t );
	}
	else if( format_version == 2 )
	{
		chunk_descriptor_data_size = sizeof( ewf_table_entry_v2_t );
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
	if( data_size != chunk_descriptor_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( base_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid base offset value out of bounds.",
		 function );

		return( -1 );
	}
	chunk_data_offset  = chunk_descriptor->data_offset;
	chunk_data_size    = chunk_descriptor->data_size;
	range_flags        = chunk_descriptor->range_flags;

	if( format_version == 1 )
	{
		chunk_data_offset -= base_offset;

		if( ( chunk_data_offset < 0 )
		 || ( chunk_data_offset > (off64_t) INT32_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk offset value out of bounds.",
			 function );

			return( -1 );
		}
		table_offset = (uint32_t) chunk_data_offset;

		if( ( range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			table_offset |= 0x80000000UL;
		}
		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_entry_v1_t *) data )->chunk_data_offset,
		 table_offset );
	}
	else if( format_version == 2 )
	{
		if( chunk_data_offset < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( chunk_data_size > (size64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk size value out of bounds.",
			 function );

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
		 ( (ewf_table_entry_v2_t *) data )->chunk_data_offset,
		 chunk_data_offset );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_entry_v2_t *) data )->chunk_data_size,
		 (uint32_t) chunk_data_size );

		byte_stream_copy_from_uint32_little_endian(
		 ( (ewf_table_entry_v2_t *) data )->chunk_data_flags,
		 chunk_data_flags );
	}
	return( 1 );
}


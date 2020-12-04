/*
 * Chunk data functions
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

#include "libewf_checksum.h"
#include "libewf_chunk_data.h"
#include "libewf_compression.h"
#include "libewf_definitions.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfdata.h"
#include "libewf_types.h"
#include "libewf_unused.h"

#if !defined( LIBEWF_ATTRIBUTE_FALLTHROUGH )
#if defined( __GNUC__ ) && __GNUC__ >= 7
#define LIBEWF_ATTRIBUTE_FALLTHROUGH	__attribute__ ((fallthrough))
#else
#define LIBEWF_ATTRIBUTE_FALLTHROUGH
#endif
#endif

/* Creates chunk data
 * Make sure the value chunk_data is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_initialize(
     libewf_chunk_data_t **chunk_data,
     size32_t chunk_size,
     uint8_t clear_data,
     libcerror_error_t **error )
{
	static char *function      = "libewf_chunk_data_initialize";
	size_t allocated_data_size = 0;

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
	if( *chunk_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk data value already set.",
		 function );

		return( -1 );
	}
	if( ( chunk_size == 0 )
	 || ( chunk_size > (size32_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE - 16 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	*chunk_data = memory_allocate_structure(
	               libewf_chunk_data_t );

	if( *chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk_data,
	     0,
	     sizeof( libewf_chunk_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk data.",
		 function );

		memory_free(
		 *chunk_data );

		*chunk_data = NULL;

		return( -1 );
	}
	/* Reserve 4 bytes for the chunk checksum
	 */
	allocated_data_size = (size_t) chunk_size + 4;

	/* The allocated data size should be rounded to the next 16-byte increment
	 */
	if( ( allocated_data_size % 16 ) != 0 )
	{
		allocated_data_size += 16;
	}
	allocated_data_size = ( allocated_data_size / 16 ) * 16;

	( *chunk_data )->data = (uint8_t *) memory_allocate(
					     sizeof( uint8_t ) * allocated_data_size );

	if( ( *chunk_data )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	if( clear_data != 0 )
	{
		if( memory_set(
		     ( *chunk_data )->data,
		     0,
		     sizeof( uint8_t ) * allocated_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			goto on_error;
		}
	}
	( *chunk_data )->chunk_size          = chunk_size;
	( *chunk_data )->allocated_data_size = allocated_data_size;
	( *chunk_data )->flags               = LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA;

	return( 1 );

on_error:
	if( *chunk_data != NULL )
	{
		if( ( *chunk_data )->data != NULL )
		{
			memory_free(
			 ( *chunk_data )->data );
		}
		memory_free(
		 *chunk_data );

		*chunk_data = NULL;
	}
	return( -1 );
}

/* Frees chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_free(
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_free";

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
	if( *chunk_data != NULL )
	{
		if( ( ( *chunk_data )->flags & LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA ) != 0 )
		{
			if( ( *chunk_data )->data != NULL )
			{
				memory_free(
				 ( *chunk_data )->data );
			}
		}
		if( ( *chunk_data )->compressed_data != NULL )
		{
			memory_free(
			 ( *chunk_data )->compressed_data );
		}
		memory_free(
		 *chunk_data );

		*chunk_data = NULL;
	}
	return( 1 );
}

/* Reads chunk data into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_chunk_data_read_buffer(
         libewf_chunk_data_t *chunk_data,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_read_buffer";

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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( buffer_size < (size_t) chunk_data->chunk_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid buffer size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     buffer,
	     chunk_data->data,
	     chunk_data->data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy chunk data to buffer.",
		 function );

		return( -1 );
	}
	return( (ssize_t) chunk_data->data_size );
}

/* Writes a buffer to the chunk data
 * Returns the number of bytes written or -1 on error
 */
ssize_t libewf_chunk_data_write_buffer(
         libewf_chunk_data_t *chunk_data,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_write_buffer";

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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) chunk_data->chunk_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
		 "%s: invalid buffer size value too large.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     chunk_data->data,
	     buffer,
	     buffer_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy buffer to chunk data.",
		 function );

		return( -1 );
	}
	chunk_data->data_size = buffer_size;

	return( (ssize_t) buffer_size );
}

/* Determines the pack flags as part of packing the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_pack_determine_pack_flags(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     uint8_t *pack_flags,
     libcerror_error_t **error )
{
	static char *function   = "libewf_chunk_data_pack_determine_pack_flags";
	uint64_t fill_pattern   = 0;
	uint8_t safe_pack_flags = 0;
	int result              = 0;

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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
		 function );

		return( -1 );
	}
	if( chunk_data->chunk_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: chunk size value out of bounds.",
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
	if( pack_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pack flags.",
		 function );

		return( -1 );
	}
	safe_pack_flags = *pack_flags;

	if( ( ( io_handle->compression_flags & LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 )
	 && ( ( chunk_data->data_size % 8 ) == 0 ) )
	{
		result = libewf_chunk_data_check_for_64_bit_pattern_fill(
			  chunk_data->data,
			  chunk_data->data_size,
			  &fill_pattern,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunk data contains a fill pattern.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			safe_pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM | LIBEWF_PACK_FLAG_ADD_ALIGNMENT_PADDING );
			safe_pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION | LIBEWF_PACK_FLAG_USE_PATTERN_FILL_COMPRESSION;
		}
	}
	else if( ( ( io_handle->compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
	      || ( io_handle->compression_level != LIBEWF_COMPRESSION_LEVEL_NONE ) )
	{
		result = libewf_chunk_data_check_for_empty_block(
			  chunk_data->data,
			  chunk_data->data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if chunk data is an empty block.",
			 function );

			return( -1 );
		}
		else if( ( result != 0 )
		      && ( chunk_data->data[ 0 ] == 0 ) )
		{
			safe_pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM );
			safe_pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION | LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
		}
	}
	*pack_flags = safe_pack_flags;

	return( 1 );
}

/* Packs the chunk data using 64-bit pattern fill
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_pack_with_64_bit_pattern_fill(
     libewf_chunk_data_t *chunk_data,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_pack_with_64_bit_pattern_fill";

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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
		 function );

		return( -1 );
	}
	if( chunk_data->compressed_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk data - compressed data value already set.",
		 function );

		return( -1 );
	}
	chunk_data->compressed_data_size = 8;

	chunk_data->compressed_data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * chunk_data->compressed_data_size );

	if( chunk_data->compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     chunk_data->compressed_data,
	     chunk_data->data,
	     chunk_data->compressed_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy fill pattern to compressed chunk data.",
		 function );

		goto on_error;
	}
	chunk_data->range_flags = LIBEWF_RANGE_FLAG_IS_COMPRESSED | LIBEWF_RANGE_FLAG_USES_PATTERN_FILL;

	return( 1 );

on_error:
	if( chunk_data->compressed_data != NULL )
	{
		memory_free(
		 chunk_data->compressed_data );

		chunk_data->compressed_data = NULL;
	}
	chunk_data->compressed_data_size = 0;

	return( -1 );
}

/* Packs the chunk data using empty block compression
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_pack_with_empty_block_compression(
     libewf_chunk_data_t *chunk_data,
     const uint8_t *compressed_zero_byte_empty_block,
     size_t compressed_zero_byte_empty_block_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_pack_with_empty_block_compression";

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
	if( chunk_data->compressed_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk data - compressed data value already set.",
		 function );

		return( -1 );
	}
	if( compressed_zero_byte_empty_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed zero byte empty block.",
		 function );

		return( -1 );
	}
	if( ( compressed_zero_byte_empty_block_size < 4 )
	 || ( compressed_zero_byte_empty_block_size > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE - 16 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compressed zero byte empty block size value out of bounds.",
		 function );

		return( -1 );
	}
	chunk_data->compressed_data_size = compressed_zero_byte_empty_block_size;

	if( ( compressed_zero_byte_empty_block_size % 16 ) != 0 )
	{
		chunk_data->compressed_data_size += 16 - ( compressed_zero_byte_empty_block_size % 16 );
	}
	chunk_data->compressed_data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * chunk_data->compressed_data_size );

	if( chunk_data->compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     chunk_data->compressed_data,
	     compressed_zero_byte_empty_block,
	     compressed_zero_byte_empty_block_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy compressed 0-byte empty block to compressed chunk data.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( compressed_zero_byte_empty_block[ compressed_zero_byte_empty_block_size - 4 ] ),
	 chunk_data->checksum );

	chunk_data->range_flags = LIBEWF_RANGE_FLAG_IS_COMPRESSED;

	return( 1 );

on_error:
	if( chunk_data->compressed_data != NULL )
	{
		memory_free(
		 chunk_data->compressed_data );

		chunk_data->compressed_data = NULL;
	}
	chunk_data->compressed_data_size = 0;

	return( -1 );
}

/* Packs the chunk data using compression
 * Returns 1 on success, 0 if buffer is too small or -1 on error
 */
int libewf_chunk_data_pack_with_compression(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function            = "libewf_chunk_data_pack_with_compression";
	size_t safe_compressed_data_size = 0;
	int8_t compression_level         = 0;
	int result                       = 0;

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
	if( ( chunk_data->chunk_size == 0 )
	 || ( chunk_data->chunk_size > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / 2 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data - chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	if( chunk_data->compressed_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk data - compressed data value already set.",
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
/* TODO add a light weight entropy test to assess if compression makes sense */

	chunk_data->compressed_data_size = chunk_data->chunk_size;

	/* EWF-S01 allows to have compressed chunks larger than the chunk size, a factor 2 should suffice
	 */
	if( io_handle->segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		chunk_data->compressed_data_size *= 2;
	}
	chunk_data->compressed_data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * chunk_data->compressed_data_size );

	if( chunk_data->compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed data.",
		 function );

		goto on_error;
	}
	safe_compressed_data_size = chunk_data->compressed_data_size;

	/* If compression was forced but no compression level provided use the default
	 */
	compression_level = io_handle->compression_level;

	if( ( io_handle->segment_file_type != LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	 && ( compression_level == LIBEWF_COMPRESSION_LEVEL_NONE ) )
	{
		compression_level = LIBEWF_COMPRESSION_LEVEL_DEFAULT;
	}
	result = libewf_compress_data(
		  chunk_data->compressed_data,
		  &safe_compressed_data_size,
		  io_handle->compression_method,
		  compression_level,
		  chunk_data->data,
		  chunk_data->data_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
		 "%s: unable to compress chunk data.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( ( safe_compressed_data_size < 4 )
		 || ( safe_compressed_data_size > chunk_data->compressed_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: compressed data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( io_handle->compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
		{
			/* Deflate has its own checksum
			 */
			byte_stream_copy_to_uint32_little_endian(
			 &( ( chunk_data->compressed_data )[ safe_compressed_data_size - 4 ] ),
			 chunk_data->checksum );
		}
/* TODO bzip2 support */
#ifdef IGNORE
		else if( io_handle->compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
		{
			/* Some parts of the bzip2 compressed block are not stored
			 */
			chunk_data->compressed_data_offset = 4;
			safe_compressed_data_size         -= 4;
		}
#endif
		chunk_data->compressed_data_size = safe_compressed_data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: required compressed data size: %zd.\n",
		 function,
		 safe_compressed_data_size );
	}
#endif
	return( result );

on_error:
	if( chunk_data->compressed_data != NULL )
	{
		memory_free(
		 chunk_data->compressed_data );

		chunk_data->compressed_data = NULL;
	}
	chunk_data->compressed_data_size = 0;

	return( -1 );
}

/* Packs the chunk data
 * This function either adds the checksum or compresses the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_pack(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     const uint8_t *compressed_zero_byte_empty_block,
     size_t compressed_zero_byte_empty_block_size,
     uint8_t pack_flags,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_pack";
	int result            = 0;

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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
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
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) != 0 )
	{
		return( 1 );
	}
	if( libewf_chunk_data_pack_determine_pack_flags(
	     chunk_data,
	     io_handle,
	     &pack_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine pack flags.",
		 function );

		goto on_error;
	}
	/* Make sure range flags are cleared before usage.
	 */
	chunk_data->range_flags = 0;

	if( ( io_handle->compression_level != LIBEWF_COMPRESSION_LEVEL_NONE )
	 || ( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) != 0 ) )
	{
		if( ( pack_flags & LIBEWF_PACK_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 )
		{
			if( libewf_chunk_data_pack_with_64_bit_pattern_fill(
			     chunk_data,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
				 "%s: unable to compress chunk data using 64-bit pattern fill.",
				 function );

				goto on_error;
			}
		}
		else if( ( ( pack_flags & LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
		      && ( chunk_data->data_size == (size_t) chunk_data->chunk_size )
		      && ( compressed_zero_byte_empty_block != NULL ) )
		{
			if( libewf_chunk_data_pack_with_empty_block_compression(
			     chunk_data,
			     compressed_zero_byte_empty_block,
			     compressed_zero_byte_empty_block_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
				 "%s: unable to compress chunk data using empty block compression.",
				 function );

				goto on_error;
			}
		}
		else
		{
			result = libewf_chunk_data_pack_with_compression(
			          chunk_data,
			          io_handle,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
				 "%s: unable to compress chunk data using compression.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				/* Use the compressed data if it is smaller than the uncompressed data or when compression is forced
				 */
				if( ( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) != 0 )
				 || ( chunk_data->compressed_data_size < chunk_data->data_size ) )
				{
					chunk_data->range_flags = LIBEWF_RANGE_FLAG_IS_COMPRESSED;
				}
			}
			else if( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
				 "%s: unable to compress chunk data - compression was forced but compressed data is too small.",
				 function );

				goto on_error;
			}
		}
	}
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		if( ( chunk_data->flags & LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA ) != 0 )
		{
			memory_free(
			 chunk_data->data );
		}
		chunk_data->data      = chunk_data->compressed_data;
		chunk_data->data_size = chunk_data->compressed_data_size;
		chunk_data->flags     = LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA;

		chunk_data->compressed_data      = NULL;
		chunk_data->compressed_data_size = 0;
	}
	else if( ( pack_flags & LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM ) != 0 )
	{
		if( libewf_checksum_calculate_adler32(
		     &( chunk_data->checksum ),
		     chunk_data->data,
		     chunk_data->data_size,
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
		if( ( chunk_data->data_size + 4 ) <= chunk_data->allocated_data_size )
		{
			byte_stream_copy_from_uint32_little_endian(
			 &( ( chunk_data->data )[ chunk_data->data_size ] ),
			 chunk_data->checksum );

			chunk_data->data_size += 4;
		}
		else
		{
			chunk_data->chunk_io_flags = LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET;
		}
		chunk_data->range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;
	}
	if( ( pack_flags & LIBEWF_PACK_FLAG_ADD_ALIGNMENT_PADDING ) != 0 )
	{
		chunk_data->padding_size = chunk_data->data_size % 16;

		if( chunk_data->padding_size != 0 )
		{
			chunk_data->padding_size = 16 - chunk_data->padding_size;
		}
		if( ( chunk_data->padding_size > chunk_data->allocated_data_size )
		 || ( chunk_data->data_size > ( chunk_data->allocated_data_size - chunk_data->padding_size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk data - allocated data size value too small.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     &( ( chunk_data->data )[ chunk_data->data_size ] ),
		     0,
		     chunk_data->padding_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear alignment padding.",
			 function );

			goto on_error;
		}
	}
	chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_PACKED;

	return( 1 );

on_error:
	if( chunk_data->compressed_data != NULL )
	{
		memory_free(
		 chunk_data->compressed_data );

		chunk_data->compressed_data = NULL;
	}
	chunk_data->compressed_data_size = 0;

	return( -1 );
}

/* Unpacks the chunk data
 * This function either validates the checksum or decompresses the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_unpack(
     libewf_chunk_data_t *chunk_data,
     libewf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function        = "libewf_chunk_data_unpack";
	size_t remaining_chunk_size  = 0;
	uint32_t calculated_checksum = 0;

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
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
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
	if( ( chunk_data->chunk_size == 0 )
	 || ( chunk_data->chunk_size > (size32_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE - 16 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) == 0 )
	{
		return( 1 );
	}
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		if( chunk_data->compressed_data != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid chunk data - compressed data value already set.",
			 function );

			return( -1 );
		}
		chunk_data->compressed_data      = chunk_data->data;
		chunk_data->compressed_data_size = chunk_data->data_size;

		/* Reserve 4 bytes for the checksum
		 */
		chunk_data->allocated_data_size = (size_t) ( chunk_data->chunk_size + 4 );

		/* The allocated data size should be rounded to the next 16-byte increment
		 */
		if( ( chunk_data->allocated_data_size % 16 ) != 0 )
		{
			chunk_data->allocated_data_size += 16;
		}
		chunk_data->allocated_data_size = ( chunk_data->allocated_data_size / 16 ) * 16;

		chunk_data->data = (uint8_t *) memory_allocate(
		                                sizeof( uint8_t ) * chunk_data->allocated_data_size );

		if( chunk_data->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     chunk_data->data,
		     0,
		     sizeof( uint8_t ) * chunk_data->allocated_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			goto on_error;
		}
		chunk_data->data_size = (size_t) chunk_data->chunk_size;

		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_USES_PATTERN_FILL ) != 0 )
		{
/* TODO move into libewf_chunk_data_unpack_with_64_bit_pattern_fill */
			if( chunk_data->compressed_data_size < (size_t) 8 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid chunk data - compressed data size value out of bounds.",
				 function );

				goto on_error;
			}
			remaining_chunk_size = (size_t) chunk_data->chunk_size;

			switch( remaining_chunk_size % 8 )
			{
				case 7:
					( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 7 ];

				LIBEWF_ATTRIBUTE_FALLTHROUGH;
				case 6:
					( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 6 ];

				LIBEWF_ATTRIBUTE_FALLTHROUGH;
				case 5:
					( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 5 ];

				LIBEWF_ATTRIBUTE_FALLTHROUGH;
				case 4:
					( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 4 ];

				LIBEWF_ATTRIBUTE_FALLTHROUGH;
				case 3:
					( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 3 ];

				LIBEWF_ATTRIBUTE_FALLTHROUGH;
				case 2:
					( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 2 ];

				LIBEWF_ATTRIBUTE_FALLTHROUGH;
				case 1:
					( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 1 ];
			}
			while( remaining_chunk_size > 0 )
			{
/* TODO make this memory aligned ? */
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 7 ];
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 6 ];
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 5 ];
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 4 ];
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 3 ];
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 2 ];
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 1 ];
				( chunk_data->data )[ --remaining_chunk_size ] = ( chunk_data->compressed_data )[ 0 ];
			}
		}
		else
		{
			if( libewf_decompress_data(
			     chunk_data->compressed_data,
			     chunk_data->compressed_data_size,
			     io_handle->compression_method,
			     chunk_data->data,
			     &( chunk_data->data_size ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
				 "%s: unable to decompress chunk data.",
				 function );

#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );

				chunk_data->data_size    = (size_t) chunk_data->chunk_size;
				chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
			}
		}
	}
	else if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
	{
		if( chunk_data->data_size < 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk data - data size value out of bounds.",
			 function );

			goto on_error;
		}
		chunk_data->data_size -= 4;

		if( ( chunk_data->chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) == 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( ( chunk_data->data )[ chunk_data->data_size ] ),
			 chunk_data->checksum );
		}
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     chunk_data->data,
		     chunk_data->data_size,
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
		if( chunk_data->checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: chunk data checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
			 function,
			 chunk_data->checksum,
			 calculated_checksum );

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif /* defined( HAVE_VERBOSE_OUTPUT ) */

			libcerror_error_free(
			 error );

			chunk_data->data_size    = (size_t) chunk_data->chunk_size;
			chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
		}
	}
	if( ( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_CORRUPTED ) != 0 )
	 && ( io_handle->zero_on_error != 0 ) )
	{
		if( memory_set(
		     chunk_data->data,
		     0,
		     chunk_data->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to zero chunk data.",
			 function );

			goto on_error;
		}
	}
	chunk_data->range_flags &= ~( LIBEWF_RANGE_FLAG_IS_PACKED );

	return( 1 );

on_error:
	if( chunk_data->compressed_data != NULL )
	{
		if( chunk_data->data != NULL )
		{
			memory_free(
			 chunk_data->data );
		}
		chunk_data->data      = chunk_data->compressed_data;
		chunk_data->data_size = chunk_data->compressed_data_size;

		chunk_data->compressed_data      = NULL;
		chunk_data->compressed_data_size = 0;
	}
	return( -1 );
}

/* Checks if a buffer containing the chunk data is filled with same value bytes (empty-block)
 * Returns 1 if a pattern was found, 0 if not or -1 on error
 */
int libewf_chunk_data_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_check_for_empty_block";

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
	if( data_size == 0 )
	{
		return( 0 );
	}
	else if( data_size == 1 )
	{
		return( 1 );
	}
	if( memory_compare(
	     data,
	     &( data[ 1 ] ),
	     data_size - 1 ) != 0 )
	{
		return( 0 );
	}
	return( 1 );
}

/* Checks if a buffer containing the chunk data is filled with a 64-bit pattern
 * Returns 1 if a pattern was found, 0 if not or -1 on error
 */
int libewf_chunk_data_check_for_64_bit_pattern_fill(
     const uint8_t *data,
     size_t data_size,
     uint64_t *pattern,
     libcerror_error_t **error )
{
	libewf_aligned_t *aligned_data_index = NULL;
	libewf_aligned_t *aligned_data_start = NULL;
	uint8_t *data_index                  = NULL;
	uint8_t *data_start                  = NULL;
	static char *function                = "libewf_chunk_data_check_for_64_bit_pattern_fill";

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
	if( pattern == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pattern.",
		 function );

		return( -1 );
	}
	if( ( data_size <= 8 )
	 || ( ( data_size % 8 ) != 0 ) )
	{
		return( 0 );
	}
	data_start = (uint8_t *) data;
	data_index = (uint8_t *) data + 8;
	data_size -= 8;

	/* Only optimize for data larger than the alignment
	 */
	if( data_size > ( sizeof( libewf_aligned_t ) + sizeof( libewf_aligned_t ) ) )
	{
		/* Align the data start
		 */
		while( ( (intptr_t) data_start % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_start++;
			data_index++;
			data_size--;
		}
		/* Align the data index
		 */
		while( ( (intptr_t) data_index % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_index++;
			data_size--;
		}
		aligned_data_start = (libewf_aligned_t *) data_start;
		aligned_data_index = (libewf_aligned_t *) data_index;

		while( data_size > sizeof( libewf_aligned_t ) )
		{
			if( *aligned_data_start != *aligned_data_index )
			{
				return( 0 );
			}
			aligned_data_start++;
			aligned_data_index++;

			data_size -= sizeof( libewf_aligned_t );
		}
		data_start = (uint8_t *) aligned_data_start;
		data_index = (uint8_t *) aligned_data_index;
	}
	while( data_size != 0 )
	{
		if( *data_start != *data_index )
		{
			return( 0 );
		}
		data_start++;
		data_index++;
		data_size--;
	}
	byte_stream_copy_to_uint64_little_endian(
	 data,
	 *pattern );

	return( 1 );
}

/* Writes a chunk
 * Returns 1 if successful or -1 on error
 */
ssize_t libewf_chunk_data_write(
         libewf_chunk_data_t *chunk_data,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error )
{
	uint8_t checksum_buffer[ 4 ];

	static char *function     = "libewf_chunk_data_write";
	size_t write_size         = 0;
	ssize_t total_write_count = 0;
	ssize_t write_count       = 0;

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
	write_size = chunk_data->data_size + chunk_data->padding_size;

	/* Write the chunk data to the segment file
	 */
	write_count = libbfio_pool_write_buffer(
	               file_io_pool,
	               file_io_pool_entry,
	               chunk_data->data,
	               write_size,
	               error );

	if( write_count != (ssize_t) write_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write chunk data.",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	if( ( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) == 0 )
	 && ( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 ) )
	{
		/* Check if the chunk and checksum buffers are aligned
		 * if not the checksum needs to be written separately
		 */
		if( ( chunk_data->chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) != 0 )
		{
			byte_stream_copy_from_uint32_little_endian(
			 checksum_buffer,
			 chunk_data->checksum );

			write_count = libbfio_pool_write_buffer(
				       file_io_pool,
				       file_io_pool_entry,
				       checksum_buffer,
				       4,
				       error );

			if( write_count != (ssize_t) 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write chunk checksum.",
				 function );

				return( -1 );
			}
			total_write_count += write_count;
		}
	}
	return( total_write_count );
}

/* Retrieves the write size of the chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_get_write_size(
     libewf_chunk_data_t *chunk_data,
     uint32_t *write_size,
     libcerror_error_t **error )
{
	static char *function  = "libewf_chunk_data_get_write_size";
	size_t safe_write_size = 0;

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
#if SIZEOF_SIZE_T <= 4
	if( chunk_data->data_size > (size_t) SSIZE_MAX )
#else
	if( chunk_data->data_size > (size_t) UINT32_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data - data size value out of bounds.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T <= 4
	if( chunk_data->padding_size > (size_t) SSIZE_MAX )
#else
	if( chunk_data->padding_size > (size_t) UINT32_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data - padding size value out of bounds.",
		 function );

		return( -1 );
	}
	if( write_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid write size.",
		 function );

		return( -1 );
	}
	safe_write_size = chunk_data->data_size + chunk_data->padding_size;

	if( ( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) == 0 )
	 && ( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 ) )
	{
		if( ( chunk_data->chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) != 0 )
		{
			safe_write_size += 4;
		}
	}
#if SIZEOF_SIZE_T <= 4
	if( safe_write_size > (size_t) SSIZE_MAX )
#else
	if( safe_write_size > (size_t) UINT32_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid write size value out of bounds.",
		 function );

		return( -1 );
	}
	*write_size = (uint32_t) safe_write_size;

	return( 1 );
}

/* Retrieves the (stored) checksum
 * Returns 1 if successful, 0 if no checksum or -1 on error
 */
int libewf_chunk_data_get_checksum(
     libewf_chunk_data_t *chunk_data,
     uint16_t compression_method,
     uint32_t *checksum,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_get_checksum";
	int result            = 0;

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
	if( checksum == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum.",
		 function );

		return( -1 );
	}
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		if( chunk_data->data_size < 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid chunk data size value out of bounds.",
			 function );

			return( -1 );
		}
		if( compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( ( chunk_data->data )[ chunk_data->data_size - 4 ] ),
			 *checksum );

			result = 1;
		}
		else if( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
		{
/* TODO bzip2 support */
		}
	}
	else if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
	{
/* TODO chunk data rewrite:
* Set LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET chunk_data->chunk_io_flags if the checksum is not provided within the chunk data
*/
		if( ( chunk_data->chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) != 0 )
		{
			*checksum = chunk_data->checksum;

			result = 1;
		}
/* TODO chunk data rewrite */
		else if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) == 0 )
		{
			if( ( chunk_data->data_size < 4 )
			 || ( ( chunk_data->data_size + 4 ) > chunk_data->allocated_data_size ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid chunk data size value out of bounds.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint32_little_endian(
			 &( ( chunk_data->data )[ chunk_data->data_size ] ),
			 *checksum );

			result = 1;
		}
		else
		{
			if( ( chunk_data->data_size < 4 )
			 || ( chunk_data->data_size > chunk_data->allocated_data_size ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid chunk data size value out of bounds.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint32_little_endian(
			 &( ( chunk_data->data )[ chunk_data->data_size - 4 ] ),
			 *checksum );

			result = 1;
		}
	}
	return( result );
}

/* Reads chunk data from the file IO pool
 * Returns the number of bytes read, 0 when no longer data can be read or -1 on error
 */
ssize_t libewf_chunk_data_read_from_file_io_pool(
         libewf_chunk_data_t *chunk_data,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t chunk_data_offset,
         size64_t chunk_data_size,
         uint32_t chunk_data_flags,
         libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_read_from_file_io_pool";
	ssize_t read_count    = 0;

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
	if( ( chunk_data_size == (size64_t) 0 )
	 || ( chunk_data_size > (size64_t) chunk_data->allocated_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data size value out of bounds.",
		 function );

		return( -1 );
	}
	read_count = libbfio_pool_read_buffer_at_offset(
		      file_io_pool,
		      file_io_pool_entry,
		      chunk_data->data,
		      (size_t) chunk_data_size,
	              chunk_data_offset,
		      error );

	if( read_count != (ssize_t) chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data at offset: %" PRIi64 " (0x%08" PRIx64 ") in file IO pool entry: %d.",
		 function,
		 chunk_data_offset,
		 chunk_data_offset,
		 file_io_pool_entry );

		return( -1 );
	}
	chunk_data->data_size = (size_t) read_count;

	chunk_data->range_flags = ( chunk_data_flags | LIBEWF_RANGE_FLAG_IS_PACKED )
	                        & ~( LIBEWF_RANGE_FLAG_IS_TAINTED | LIBEWF_RANGE_FLAG_IS_CORRUPTED );

	return( read_count );
}

/* Reads chunk data
 * Callback function for the chunks list
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_read_element_data(
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int file_io_pool_entry,
     off64_t chunk_data_offset,
     size64_t chunk_data_size,
     uint32_t chunk_data_flags,
     uint8_t read_flags LIBEWF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libewf_chunk_data_t *chunk_data = NULL;
	static char *function           = "libewf_chunk_data_read_element_data";
	ssize_t read_count              = 0;

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
	if( io_handle->chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing chunk size.",
		 function );

		return( -1 );
	}
	if( ( chunk_data_flags & LIBEWF_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported chunk data flags.",
		 function );

		return( -1 );
	}
	if( libewf_chunk_data_initialize(
	     &chunk_data,
	     io_handle->chunk_size,
	     0,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading chunk at offset: 0x%08" PRIx64 " with size: %" PRIu64 " in file IO pool entry: %d.\n",
		 function,
		 chunk_data_offset,
		 chunk_data_size,
		 file_io_pool_entry );
	}
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: chunk file IO pool entry\t\t: %d\n",
		 function,
		 file_io_pool_entry );

		libcnotify_printf(
		 "%s: chunk offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 chunk_data_offset,
		 chunk_data_offset );

		libcnotify_printf(
		 "%s: chunk size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 chunk_data_size );

		libcnotify_printf(
		 "%s: chunk flags:\n",
		 function );

		if( ( chunk_data_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			libcnotify_printf(
			 "\tIs compressed\n" );
		}
		if( ( chunk_data_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		{
			libcnotify_printf(
			 "\tHas checksum\n" );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	read_count = libewf_chunk_data_read_from_file_io_pool(
		      chunk_data,
		      file_io_pool,
		      file_io_pool_entry,
	              chunk_data_offset,
		      chunk_data_size,
		      chunk_data_flags,
		      error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
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
		if( ( ( chunk_data_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
		 && ( chunk_data->data_size >= 4 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( ( chunk_data->data )[ chunk_data->data_size - 4 ] ),
			 chunk_data->checksum );

			libcnotify_printf(
			 "%s: chunk checksum\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 chunk_data->checksum );

			libcnotify_printf(
			 "\n" );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_pool,
	     cache,
	     (intptr_t *) chunk_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libewf_chunk_data_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
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


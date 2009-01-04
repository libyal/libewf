/*
 * libewf handle
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "libewf_includes.h"

#include <time.h>

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_internal_handle.h"
#include "libewf_string.h"

#include "ewf_crc.h"
#include "ewf_compress.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"

/* Allocates memory for a new handle struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_INTERNAL_HANDLE *libewf_internal_handle_alloc( uint8_t flags )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_internal_handle_alloc";

	internal_handle = (LIBEWF_INTERNAL_HANDLE *) libewf_common_alloc( LIBEWF_INTERNAL_HANDLE_SIZE );

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate handle.\n",
		 function );

		return( NULL );
	}
	internal_handle->media                     = NULL;
	internal_handle->read                      = NULL;
	internal_handle->write                     = NULL;
	internal_handle->segment_table             = NULL;
	internal_handle->delta_segment_table       = NULL;
	internal_handle->offset_table              = NULL;
	internal_handle->secondary_offset_table    = NULL;
	internal_handle->chunk_cache               = NULL;
	internal_handle->header                    = NULL;
	internal_handle->header_size               = 0;
	internal_handle->header2                   = NULL;
	internal_handle->header2_size              = 0;
	internal_handle->xheader                   = NULL;
	internal_handle->xheader_size              = 0;
	internal_handle->xhash                     = NULL;
	internal_handle->xhash_size                = 0;
	internal_handle->header_values             = NULL;
	internal_handle->hash_values               = NULL;
	internal_handle->stored_md5_hash           = NULL;
	internal_handle->calculated_md5_hash       = NULL;
	internal_handle->acquiry_error_sectors     = NULL;
	internal_handle->acquiry_amount_of_errors  = 0;
	internal_handle->current_chunk             = 0;
	internal_handle->current_chunk_offset      = 0;
	internal_handle->swap_byte_pairs           = 0;
	internal_handle->compression_level         = EWF_COMPRESSION_UNKNOWN;
	internal_handle->amount_of_header_sections = 0;
	internal_handle->format                    = LIBEWF_FORMAT_UNKNOWN;
	internal_handle->ewf_format                = EWF_FORMAT_UNKNOWN;
	internal_handle->error_tollerance          = LIBEWF_ERROR_TOLLERANCE_COMPENSATE;

	/* The segment table is initially filled with a single entry
	 */
	internal_handle->segment_table = libewf_segment_table_alloc( 1 );

	if( internal_handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create segment table.\n",
		 function );

		libewf_common_free( internal_handle );

		return( NULL );
	}
	/* The delta segment table is initially filled with a single entry
	 */
	internal_handle->delta_segment_table = libewf_segment_table_alloc( 1 );

	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create delta segment table.\n",
		 function );

		libewf_segment_table_free( internal_handle->segment_table );
		libewf_common_free( internal_handle );

		return( NULL );
	}
	internal_handle->chunk_cache = libewf_chunk_cache_alloc( EWF_MINIMUM_CHUNK_SIZE + EWF_CRC_SIZE );

	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create chunk cache.\n",
		 function );

		libewf_segment_table_free( internal_handle->segment_table );
		libewf_segment_table_free( internal_handle->delta_segment_table );
		libewf_common_free( internal_handle );

		return( NULL );
	}
	internal_handle->media = libewf_internal_handle_media_alloc();

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create media sub handle.\n",
		 function );

		libewf_chunk_cache_free( internal_handle->chunk_cache );
		libewf_segment_table_free( internal_handle->segment_table );
		libewf_segment_table_free( internal_handle->delta_segment_table );
		libewf_common_free( internal_handle );

		return( NULL );
	}
	/* Initialize the MD5 context
	 */
	if( libewf_md5_initialize( &internal_handle->md5_context ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize MD5 context.\n",
		 function );

		libewf_internal_handle_media_free( internal_handle->media );
		libewf_chunk_cache_free( internal_handle->chunk_cache );
		libewf_segment_table_free( internal_handle->segment_table );
		libewf_segment_table_free( internal_handle->delta_segment_table );
		libewf_common_free( internal_handle );

		return( NULL );
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		internal_handle->read = libewf_internal_handle_read_alloc();

		if( internal_handle->read == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create read sub handle.\n",
			 function );

			libewf_internal_handle_media_free( internal_handle->media );
			libewf_chunk_cache_free( internal_handle->chunk_cache );
			libewf_segment_table_free( internal_handle->segment_table );
			libewf_segment_table_free( internal_handle->delta_segment_table );
			libewf_common_free( internal_handle );

			return( NULL );
		}
	}
	if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		internal_handle->write = libewf_internal_handle_write_alloc();

		if( internal_handle->write == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create write sub handle.\n",
			 function );

			if( internal_handle->read != NULL )
			{
				libewf_internal_handle_read_free( internal_handle->read );
			}
			libewf_internal_handle_media_free( internal_handle->media );
			libewf_chunk_cache_free( internal_handle->chunk_cache );
			libewf_segment_table_free( internal_handle->segment_table );
			libewf_segment_table_free( internal_handle->delta_segment_table );
			libewf_common_free( internal_handle );

			return( NULL );
		}
	}
	return( internal_handle );
}

/* Frees memory of a handle struct including elements
 */
void libewf_internal_handle_free( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_free";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return;
	}
	libewf_internal_handle_media_free( internal_handle->media );

	if( internal_handle->read != NULL )
	{
		libewf_internal_handle_read_free( internal_handle->read );
	}
	libewf_internal_handle_write_free( internal_handle->write );

	if( internal_handle->segment_table != NULL )
	{
		libewf_segment_table_free( internal_handle->segment_table );
	}
	if( internal_handle->delta_segment_table != NULL )
	{
		libewf_segment_table_free( internal_handle->delta_segment_table );
	}
	if( internal_handle->offset_table != NULL )
	{
		libewf_offset_table_free( internal_handle->offset_table );
	}
	if( internal_handle->secondary_offset_table != NULL )
	{
		libewf_offset_table_free( internal_handle->secondary_offset_table );
	}
	libewf_common_free( internal_handle->acquiry_error_sectors );
	libewf_common_free( internal_handle->header );
	libewf_common_free( internal_handle->header2 );
	libewf_common_free( internal_handle->xheader );
	libewf_common_free( internal_handle->xhash );

	if( internal_handle->header_values != NULL )
	{
		libewf_header_values_free( internal_handle->header_values );
	}
	if( internal_handle->hash_values != NULL )
	{
		libewf_hash_values_free( internal_handle->hash_values );
	}
	libewf_common_free( internal_handle->stored_md5_hash );
	libewf_common_free( internal_handle->calculated_md5_hash );

	if( internal_handle->chunk_cache != NULL )
	{
		libewf_chunk_cache_free( internal_handle->chunk_cache );
	}
	libewf_common_free( internal_handle );
}

/* Allocates memory for a new handle media struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_INTERNAL_HANDLE_MEDIA *libewf_internal_handle_media_alloc( void )
{
	LIBEWF_INTERNAL_HANDLE_MEDIA *handle_media = NULL;
	static char *function                      = "libewf_internal_handle_media_alloc";

	handle_media = (LIBEWF_INTERNAL_HANDLE_MEDIA *) libewf_common_alloc( LIBEWF_INTERNAL_HANDLE_MEDIA_SIZE );

	if( handle_media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate handle media.\n",
		 function );

		return( NULL );
	}
	handle_media->media_size        = 0;
	handle_media->chunk_size        = EWF_MINIMUM_CHUNK_SIZE;
	handle_media->sectors_per_chunk = 0;
	handle_media->bytes_per_sector  = 0;
	handle_media->amount_of_chunks  = 0;
	handle_media->amount_of_sectors = 0;
	handle_media->error_granularity = 0;
	handle_media->media_type        = 0;
	handle_media->media_flags       = 0x01;

	return( handle_media );
}

/* Allocates memory for a new handle read struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_INTERNAL_HANDLE_READ *libewf_internal_handle_read_alloc( void )
{
	LIBEWF_INTERNAL_HANDLE_READ *handle_read = NULL;
	static char *function                    = "libewf_internal_handle_read_alloc";

	handle_read = (LIBEWF_INTERNAL_HANDLE_READ *) libewf_common_alloc( LIBEWF_INTERNAL_HANDLE_READ_SIZE );

	if( handle_read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate handle read.\n",
		 function );

		return( NULL );
	}
	handle_read->crc_error_sectors    = NULL;
	handle_read->crc_amount_of_errors = 0;
	handle_read->values_initialized   = 0;

	return( handle_read );
}

/* Frees memory of a handle read struct including elements
 */
void libewf_internal_handle_read_free( LIBEWF_INTERNAL_HANDLE_READ *handle_read )
{
	static char *function = "libewf_internal_handle_read_free";

	if( handle_read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle read.\n",
		 function );

		return;
	}
	libewf_common_free( handle_read->crc_error_sectors );
	libewf_common_free( handle_read );
}

/* Allocates memory for a new handle write struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_INTERNAL_HANDLE_WRITE *libewf_internal_handle_write_alloc( void )
{
	LIBEWF_INTERNAL_HANDLE_WRITE *handle_write = NULL;
	static char *function                      = "libewf_internal_handle_write_alloc";

	handle_write = (LIBEWF_INTERNAL_HANDLE_WRITE *) libewf_common_alloc( LIBEWF_INTERNAL_HANDLE_WRITE_SIZE );

	if( handle_write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate handle write.\n",
		 function );

		return( NULL );
	}
	handle_write->data_section               = NULL;
	handle_write->input_write_count          = 0;
	handle_write->write_count                = 0;
	handle_write->input_write_size           = 0;
	handle_write->segment_file_size          = 0;
	handle_write->maximum_amount_of_segments = 0;
	handle_write->chunks_section_write_count = 0;
	handle_write->amount_of_chunks           = 0;
	handle_write->chunks_per_segment         = 0;
	handle_write->chunks_per_chunks_section  = 0;
	handle_write->segment_amount_of_chunks   = 0;
	handle_write->section_amount_of_chunks   = 0;
	handle_write->chunks_section_offset      = 0;
	handle_write->chunks_section_number      = 0;
	handle_write->compress_empty_block       = 0;
	handle_write->unrestrict_offset_amount   = 0;
	handle_write->values_initialized         = 0;
	handle_write->create_chunks_section      = 0;
	handle_write->write_finalized            = 0;

	return( handle_write );
}

/* Frees memory of a handle write struct including elements
 */
void libewf_internal_handle_write_free( LIBEWF_INTERNAL_HANDLE_WRITE *handle_write )
{
	static char *function = "libewf_internal_handle_write_free";

	if( handle_write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle write.\n",
		 function );

		return;
	}
	libewf_common_free( handle_write->data_section );
	libewf_common_free( handle_write );
}

/* Reallocates the handle chunk cache
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_INTERNAL_HANDLE *libewf_internal_handle_chunk_cache_realloc( LIBEWF_INTERNAL_HANDLE *internal_handle, size_t size )
{
	LIBEWF_CHUNK_CACHE *chunk_cache = NULL;
	static char *function           = "libewf_internal_handle_chunk_cache_realloc";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( NULL );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( NULL );
	}
	chunk_cache = libewf_chunk_cache_realloc( internal_handle->chunk_cache, size );

	if( chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate chunk cache.\n",
		 function );

		return( NULL );
	}
	internal_handle->chunk_cache = chunk_cache;

	return( internal_handle );
}

/* Updates the internal MD5 for raw access mode
 * Returns 1 if successful, -1 on error
 */
int libewf_internal_handle_raw_update_md5( LIBEWF_INTERNAL_HANDLE *internal_handle, void *buffer, size_t size )
{
	static char *function = "libewf_internal_handle_raw_update_md5";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
        if( libewf_md5_update( &internal_handle->md5_context, buffer, size ) != 1 )
        {
                LIBEWF_WARNING_PRINT( "%s: unable to update MD5 context.\n",
                 function );

                return( -1 );
        }
	return( 1 );
}

/* Check if the header value is set
 * Returns 0 if not set, 1 if set, or -1 on error
 */
int8_t libewf_internal_handle_is_set_header( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_is_set_header";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( (uint8_t) ( internal_handle->header != NULL ) );
}

/* Check if the header2 value is set
 * Returns 0 if not set, 1 if set, or -1 on error
 */
int8_t libewf_internal_handle_is_set_header2( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_is_set_header2";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( (uint8_t) ( internal_handle->header2 != NULL ) );
}

/* Check if the xheader value is set
 * Returns 0 if not set, 1 if set, or -1 on error
 */
int8_t libewf_internal_handle_is_set_xheader( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_is_set_xheader";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( (uint8_t) ( internal_handle->xheader != NULL ) );
}

/* Check if the xhash value is set
 * Returns 0 if not set, 1 if set, or -1 on error
 */
int8_t libewf_internal_handle_is_set_xhash( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_is_set_xhash";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( (uint8_t) ( internal_handle->xhash != NULL ) );
}

/* Returns the amount of sectors per chunk from the media information, 0 if not set, -1 on error
 */
int32_t libewf_internal_handle_get_media_sectors_per_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_sectors_per_chunk";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sector per chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( internal_handle->media->sectors_per_chunk );
}

/* Returns the amount of bytes per sector from the media information, 0 if not set, -1 on error
 */
int32_t libewf_internal_handle_get_media_bytes_per_sector( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_bytes_per_sector";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( internal_handle->media->bytes_per_sector );
}

/* Returns the amount of sectors from the media information, 0 if not set, -1 on error
 */
int32_t libewf_internal_handle_get_media_amount_of_sectors( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_amount_of_sectors";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->amount_of_sectors > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sectors value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( internal_handle->media->amount_of_sectors );
}

/* Returns the chunk size from the media information, 0 if not set, -1 on error
 */
ssize32_t libewf_internal_handle_get_media_chunk_size( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_chunk_size";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->chunk_size > (size32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( internal_handle->media->chunk_size );
}

/* Returns the error granularity from the media information, 0 if not set, -1 on error
 */
int32_t libewf_internal_handle_get_media_error_granularity( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_error_granularity";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->error_granularity > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid error granularity value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( internal_handle->media->error_granularity );
}

/* Returns the compression level value, or -1 on error
 */
int8_t libewf_internal_handle_get_compression_level( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_compression_level";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->compression_level <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compression level only positive values are supported.\n",
		 function );

		return( -1 );
	}
	return( (int8_t) internal_handle->compression_level );
}

/* Returns the size of the contained media data, 0 if not set, -1 on error
 */
ssize64_t libewf_internal_handle_get_media_size( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_size";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->media_size == 0 )
	{
		internal_handle->media->media_size = (size64_t) internal_handle->media->amount_of_sectors
		                                   * (size64_t) internal_handle->media->bytes_per_sector;
	}
	if( internal_handle->media->media_size > (ssize64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( (ssize64_t) internal_handle->media->media_size );
}

/* Returns the media type value, or -1 on error
 */
int8_t libewf_internal_handle_get_media_type( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_type";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->media_type > (uint8_t) INT8_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media type value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( (int8_t) internal_handle->media->media_type );
}

/* Returns the media flags value, or -1 on error
 */
int8_t libewf_internal_handle_get_media_flags( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_media_flags";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->media_flags > (uint8_t) INT8_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media flags value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( (int8_t) internal_handle->media->media_flags );
}

/* Returns the volume type value, or -1 on error
 */
int8_t libewf_internal_handle_get_volume_type( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_volume_type";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->media->media_flags & 0x02 ) == 0 )
	{
		return( (int8_t) LIBEWF_VOLUME_TYPE_LOGICAL );
	}
	return( (int8_t) LIBEWF_VOLUME_TYPE_PHYSICAL );
}

/* Returns the format value, or -1 on error
 */
int8_t libewf_internal_handle_get_format( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_format";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->format > (uint8_t) INT8_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid format value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	return( (int8_t) internal_handle->format );
}

/* Returns 1 if the GUID is set, or -1 on error
 */
int8_t libewf_internal_handle_get_guid( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t *guid, size_t size )
{
	static char *function = "libewf_internal_handle_get_guid";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid guid.\n",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: guid too small.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy( guid, internal_handle->guid, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set guid.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Returns the amount of chunks written, 0 if no chunks have been written, or -1 on error
 */
int64_t libewf_internal_handle_get_write_amount_of_chunks( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_write_amount_of_chunks";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle write.\n",
		 function );

		return( -1 );
	}
	return( (int64_t) internal_handle->write->amount_of_chunks );
}

/* Returns the maximum amount of supported segment files to write, or -1 on error
 */
int16_t libewf_internal_handle_get_write_maximum_amount_of_segments( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_get_write_maximum_amount_of_segments";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		/* = 4831
		 */
		return( (int16_t) ( ( (int) ( 'z' - 's' ) * 26 * 26 ) + 99 ) );
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		/* = 14295
		 */
		return( (int16_t) ( ( (int) ( 'Z' - 'E' ) * 26 * 26 ) + 99 ) );
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported EWF format.\n",
		 function );

		return( -1 );
	}
}

/* Retrieves the header value specified by identifier
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int8_t libewf_internal_handle_get_header_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	static char *function = "libewf_internal_handle_get_header_value";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid indentifier.\n",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->header_values == NULL )
	{
		return( 0 );
	}
	return( libewf_header_values_get_value( internal_handle->header_values, identifier, value, length ) );
}

/* Retrieves the hash value specified by identifier
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int8_t libewf_internal_handle_get_hash_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	static char *function = "libewf_internal_handle_get_hash_value";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid indentifier.\n",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_values == NULL )
	{
		return( 0 );
	}
	return( libewf_hash_values_get_value( internal_handle->hash_values, identifier, value, length ) );
}

/* Sets the header and its byte size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_header( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER *header, size_t size )
{
	static char *function = "libewf_internal_handle_set_header";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle->header      = header;
	internal_handle->header_size = size;

	return( 1 );
}

/* Sets the header2 and its byte size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_header2( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER2 *header2, size_t size )
{
	static char *function = "libewf_internal_handle_set_header2";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle->header2      = header2;
	internal_handle->header2_size = size;

	return( 1 );
}

/* Sets the xheader and its byte size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_xheader( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER2 *xheader, size_t size )
{
	static char *function = "libewf_internal_handle_set_xheader";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle->xheader      = xheader;
	internal_handle->xheader_size = size;

	return( 1 );
}

/* Sets the xhash and its byte size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_xhash( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_HEADER2 *xhash, size_t size )
{
	static char *function = "libewf_internal_handle_set_xhash";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle->xhash      = xhash;
	internal_handle->xhash_size = size;

	return( 1 );
}

/* Sets the MD5 hash value
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_stored_md5_hash( LIBEWF_INTERNAL_HANDLE *internal_handle, EWF_DIGEST_HASH *md5_hash )
{
	static char *function = "libewf_internal_handle_set_stored_md5_hash";
	size_t size           = EWF_DIGEST_HASH_SIZE_MD5;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle->stored_md5_hash = (EWF_DIGEST_HASH *) libewf_common_alloc( size );

	if( internal_handle->stored_md5_hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy( internal_handle->stored_md5_hash, md5_hash, size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash.\n",
		 function );

		libewf_common_free( internal_handle->stored_md5_hash );

		internal_handle->stored_md5_hash = NULL;

		return( -1 );
	}
	return( 1 );
}

/* Sets the media values
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_media_values( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t sectors_per_chunk, uint32_t bytes_per_sector )
{
	static char *function = "libewf_internal_handle_set_media_values";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: write values were initialized, therefore media values cannot be changed anymore.\n",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sectors.\n",
		 function );

		return( -1 );
	}
	internal_handle->media->sectors_per_chunk = sectors_per_chunk;
	internal_handle->media->bytes_per_sector  = bytes_per_sector;
	internal_handle->media->chunk_size        = sectors_per_chunk * bytes_per_sector;

	return( 1 );
}

/* Returns 1 if the GUID is set, or -1 on error
 */
int8_t libewf_internal_handle_set_guid( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t *guid, size_t size )
{
	static char *function = "libewf_internal_handle_set_guid";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid guid.\n",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: guid too small.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: write values were initialized, therefore media values cannot be changed anymore.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy( internal_handle->guid, guid, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set guid.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the write segment file size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_write_segment_file_size( LIBEWF_INTERNAL_HANDLE *internal_handle, size32_t segment_file_size )
{
	static char *function = "libewf_internal_handle_set_write_segment_file_size";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle write.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->values_initialized != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: write values were initialized and cannot be changed anymore.\n",
		 function );

		return( -1 );
	}
	if( ( segment_file_size == 0 )
	 || ( segment_file_size > (size32_t) INT32_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid value segment file value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	internal_handle->write->segment_file_size = segment_file_size;

	return( 1 );
}

/* Sets the write error granularity
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_write_error_granularity( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t error_granularity )
{
	static char *function = "libewf_internal_handle_set_write_error_granularity";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: write values were initialized, therefore media values cannot be changed anymore.\n",
		 function );

		return( -1 );
	}
	internal_handle->media->error_granularity = error_granularity;

	return( 1 );
}

/* Sets the write compression values
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_write_compression_values( LIBEWF_INTERNAL_HANDLE *internal_handle, int8_t compression_level, uint8_t compress_empty_block )
{
	static char *function = "libewf_internal_handle_set_write_compression_values";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle write.\n",
		 function );

		return( -1 );
	}
	internal_handle->compression_level = compression_level;

	/* Compress empty block is only useful when no compression is used
	 */
	if( compression_level == EWF_COMPRESSION_NONE )
	{
		internal_handle->write->compress_empty_block = compress_empty_block;
	}
	return( 1 );
}

/* Sets the write output media type
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_write_media_type( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t media_type, uint8_t volume_type )
{
	static char *function = "libewf_internal_handle_set_write_media_type";
	uint8_t media_flags   = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	internal_handle->media->media_type = media_type;

	media_flags = internal_handle->media->media_flags;

	if( volume_type == LIBEWF_VOLUME_TYPE_LOGICAL )
	{
		/* Uses 1-complement of EWF_MEDIA_FLAGS_IS_PHYSICAL
		 */
		media_flags &= ~EWF_MEDIA_FLAGS_IS_PHYSICAL;
	}
	else if( volume_type == LIBEWF_VOLUME_TYPE_PHYSICAL )
	{
		media_flags |= EWF_MEDIA_FLAGS_IS_PHYSICAL;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported volume type.\n",
		 function );

		return( -1 );
	}
	return( libewf_internal_handle_set_write_media_flags( internal_handle, media_flags ) );
}

/* Sets the write output media flags
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_write_media_flags( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t media_flags )
{
	static char *function = "libewf_internal_handle_set_write_media_flags";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	internal_handle->media->media_flags = media_flags;

	return( 1 );
}

/* Sets the write output format
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_write_format( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t format )
{
	static char *function = "libewf_internal_handle_set_write_format";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle->format = format;

	return( 1 );
}

/* Sets the write input write size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_write_input_write_size( LIBEWF_INTERNAL_HANDLE *internal_handle, size64_t input_write_size )
{
	static char *function = "libewf_internal_handle_set_write_input_write_size";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle write.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->values_initialized != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: write values were initialized and cannot be changed anymore.\n",
		 function );

		return( -1 );
	}
	if( input_write_size > (size64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	internal_handle->write->input_write_size = input_write_size;

	return( 1 );
}

/* Sets the header value specified by the identifier
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_header_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	static char *function = "libewf_internal_handle_set_header_value";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->header_values == NULL )
	{
		internal_handle->header_values = libewf_header_values_alloc();

		if( internal_handle->header_values == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
	}
	return( libewf_header_values_set_value( internal_handle->header_values, identifier, value, length ) );
}

/* Sets the hash value specified by the identifier
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_hash_value( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	static char *function = "libewf_internal_handle_set_hash_value";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_values == NULL )
	{
		internal_handle->hash_values = libewf_hash_values_alloc();

		if( internal_handle->hash_values == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create hash values.\n",
			 function );

			return( -1 );
		}
	}
	return( libewf_hash_values_set_value( internal_handle->hash_values, identifier, value, length ) );
}

/* Sets the swap byte pairs, used by both read and write
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_set_swap_byte_pairs( LIBEWF_INTERNAL_HANDLE *internal_handle, uint8_t swap_byte_pairs )
{
	static char *function = "libewf_internal_handle_set_swap_byte_pairs";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle->swap_byte_pairs = swap_byte_pairs;

	return( 1 );
}

/* Add a acquiry read error sector to the list
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_add_acquiry_error_sector( LIBEWF_INTERNAL_HANDLE *internal_handle, off64_t sector, uint32_t amount_of_sectors )
{
	LIBEWF_ERROR_SECTOR *acquiry_error_sectors = NULL;
	static char *function                      = "libewf_internal_handle_set_acquiry_error_sector";
	uint32_t iterator                          = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( sector <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sector.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->acquiry_error_sectors == NULL )
	{
		acquiry_error_sectors = (LIBEWF_ERROR_SECTOR *) libewf_common_alloc( LIBEWF_ERROR_SECTOR_SIZE );
	}
	else
	{
		/* Check if acquiry read error sector is already in list
		 */
		for( iterator = 0; iterator < internal_handle->acquiry_amount_of_errors; iterator++ )
		{
			if( internal_handle->acquiry_error_sectors[ iterator ].sector == sector )
			{
				return( 1 );
			}
		}
		acquiry_error_sectors = (LIBEWF_ERROR_SECTOR *) libewf_common_realloc(
		                         internal_handle->acquiry_error_sectors,
		                         ( LIBEWF_ERROR_SECTOR_SIZE * ( internal_handle->acquiry_amount_of_errors + 1 ) ) );
	}
	if( acquiry_error_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create acquiry read error sectors.\n",
		 function );

		return( -1 );
	}
	internal_handle->acquiry_error_sectors = acquiry_error_sectors;

	internal_handle->acquiry_error_sectors[ internal_handle->acquiry_amount_of_errors ].sector            = sector;
	internal_handle->acquiry_error_sectors[ internal_handle->acquiry_amount_of_errors ].amount_of_sectors = amount_of_sectors;

	internal_handle->acquiry_amount_of_errors++;

	return( 1 );
}

/* Add a CRC error sector to the list
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_add_crc_error_chunk( LIBEWF_INTERNAL_HANDLE *internal_handle, uint32_t chunk ) 
{
	LIBEWF_ERROR_SECTOR *crc_error_sectors = NULL;
	static char *function                  = "libewf_internal_handle_add_crc_error_chunk";
	off64_t sector                         = 0;
	uint32_t iterator                      = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle read.\n",
		 function );

		return( -1 );
	}
	sector = (off64_t) chunk * (off64_t) internal_handle->media->sectors_per_chunk;

	if( internal_handle->read->crc_error_sectors == NULL )
	{
		crc_error_sectors = (LIBEWF_ERROR_SECTOR *) libewf_common_alloc( LIBEWF_ERROR_SECTOR_SIZE );
	}
	else
	{
		/* Check if CRC error is already in list
		 */
		for( iterator = 0; iterator < internal_handle->read->crc_amount_of_errors; iterator++ )
		{
			if( internal_handle->read->crc_error_sectors[ iterator ].sector == sector )
			{
				return( 1 );
			}
		}
		crc_error_sectors = (LIBEWF_ERROR_SECTOR *) libewf_common_realloc(
		                     internal_handle->read->crc_error_sectors,
		                     ( LIBEWF_ERROR_SECTOR_SIZE * ( internal_handle->read->crc_amount_of_errors + 1 ) ) );
	}
	if( crc_error_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create CRC error sectors.\n",
		 function );

		return( -1 );
	}
	internal_handle->read->crc_error_sectors = crc_error_sectors;

	internal_handle->read->crc_error_sectors[ internal_handle->read->crc_amount_of_errors ].sector            = sector;
	internal_handle->read->crc_error_sectors[ internal_handle->read->crc_amount_of_errors ].amount_of_sectors = internal_handle->media->sectors_per_chunk;

	internal_handle->read->crc_amount_of_errors++;

	return( 1 );
}

/* Determines the EWF file format based on known characteristics
 * Returns 1 if the format was determined, -1 on errror
 */
int8_t libewf_internal_handle_determine_format( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_determine_format";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->ewf_format == EWF_FORMAT_S01 )
	{
		/* The format identifier for the EWF-S01 format was already set
		 * while reading the volume section
		 */
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_E01 )
	{
		if( internal_handle->xheader != NULL )
		{
			internal_handle->format = LIBEWF_FORMAT_EWFX;
		}
		/* The header2 in raw format starts with 0xff 0xfe <number>
		 */
		else if( internal_handle->header2 != NULL )
		{
			if( internal_handle->header2[ 2 ] == (EWF_CHAR) '3' )
			{
				/* The EnCase5 header2 contains av on the 6th position (0x36 ... 0x38 ...)
				 * the header2 is an UTF16 string
				 */
				if( ( internal_handle->header2[ 36 ] == (EWF_CHAR) 'a' )
				 && ( internal_handle->header2[ 38 ] == (EWF_CHAR) 'v' ) )
				{
					internal_handle->format = LIBEWF_FORMAT_ENCASE5;
				}
				else if( ( internal_handle->header2[ 36 ] == (EWF_CHAR) 'm' )
				 && ( internal_handle->header2[ 38 ] == (EWF_CHAR) 'd' ) )
				{
					internal_handle->format = LIBEWF_FORMAT_ENCASE6;
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unsupported header2 format: %c%c.\n",
					 function, (char) internal_handle->header2[ 36 ], (char) internal_handle->header2[ 38 ] );

					return( -1 );
				}
			}
			else if( internal_handle->header2[ 2 ] == (EWF_CHAR) '1' )
			{
				internal_handle->format = LIBEWF_FORMAT_ENCASE4;
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: unsupported header2 version: %c.\n",
				 function, (char) internal_handle->header2[ 2 ] );

				return( -1 );
			}
		}
		else if( internal_handle->header != NULL )
		{
			if( internal_handle->header[ 0 ] == (EWF_CHAR) '3' )
			{
				/* The linen5 header2 contains av on the 6th position (0x17 0x18)
				 * the header2 is an UTF16 string
				 */
				if( ( internal_handle->header[ 17 ] == (EWF_CHAR) 'a' )
				 && ( internal_handle->header[ 18 ] == (EWF_CHAR) 'v' ) )
				{
					internal_handle->format = LIBEWF_FORMAT_LINEN5;
				}
				else if( ( internal_handle->header[ 17 ] == (EWF_CHAR) 'm' )
				 && ( internal_handle->header[ 18 ] == (EWF_CHAR) 'd' ) )
				{
					internal_handle->format = LIBEWF_FORMAT_LINEN6;
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unsupported header format: %c%c.\n",
					 function, (char) internal_handle->header[ 17 ], (char) internal_handle->header[ 18 ] );

					return( -1 );
				}
			}
			else if( internal_handle->header[ 0 ] == (EWF_CHAR) '1' )
			{
				/* EnCase uses \r\n
				 */
				if( internal_handle->header[ 1 ] == (EWF_CHAR) '\r' )
				{
					if( internal_handle->header[ 25 ] == (EWF_CHAR) 'r' )
					{
						internal_handle->format = LIBEWF_FORMAT_ENCASE1;
					}
					else if( internal_handle->header[ 31 ] == (EWF_CHAR) 'r' )
					{
						internal_handle->format = LIBEWF_FORMAT_ENCASE2;
					}
					else
					{
						LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
						 function );

						return( -1 );
					}
				}
				/* FTK Imager uses \n
				 */
				else if( internal_handle->header[ 1 ] == (EWF_CHAR) '\n' )
				{
					if( internal_handle->header[ 29 ] == (EWF_CHAR) 'r' )
					{
						internal_handle->format = LIBEWF_FORMAT_FTK;
					}
					else
					{
						LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
						 function );

						return( -1 );
					}
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
					 function );

					return( -1 );
				}
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
				 function );

				return( -1 );
			}
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: missing header information.\n",
			 function );

			return( -1 );
		}
	}
	else if( internal_handle->ewf_format == EWF_FORMAT_L01 )
	{
		internal_handle->format = LIBEWF_FORMAT_LVF;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported EWF file format.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Create the default header values
 * Returns 1 on success, -1 on error
 */
int8_t libewf_internal_handle_create_header_values( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	LIBEWF_CHAR *case_number              = _S_LIBEWF_CHAR( "Case Number" );
	LIBEWF_CHAR *description              = _S_LIBEWF_CHAR( "Description" );
	LIBEWF_CHAR *evidence_number          = _S_LIBEWF_CHAR( "Evidence Number" );
	LIBEWF_CHAR *examiner_name            = _S_LIBEWF_CHAR( "Examiner Name" );
	LIBEWF_CHAR *notes                    = _S_LIBEWF_CHAR( "Notes" );
	LIBEWF_CHAR *acquiry_operating_system = _S_LIBEWF_CHAR( "Undetermined" );
	LIBEWF_CHAR *acquiry_software_version = LIBEWF_VERSION;
	static char *function                 = "libewf_internal_handle_create_header_values";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->header_values != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: header values already created - cleaning up previous header values.\n",
		 function );

		libewf_header_values_free( internal_handle->header_values );
	}
	internal_handle->header_values = libewf_header_values_alloc();

	if( internal_handle->header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value(
	     internal_handle->header_values,
	     _S_LIBEWF_CHAR( "case_number" ),
	     case_number,
	     libewf_string_length( case_number ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set case number.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value(
	     internal_handle->header_values,
	     _S_LIBEWF_CHAR( "description" ),
	     description,
	     libewf_string_length( description ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set description.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value(
	     internal_handle->header_values,
	     _S_LIBEWF_CHAR( "evidence_number" ),
	     evidence_number,
	     libewf_string_length( evidence_number ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set evidence number.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value(
	     internal_handle->header_values,
	     _S_LIBEWF_CHAR( "examiner_name" ),
	     examiner_name,
	     libewf_string_length( examiner_name ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set examiner name.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value(
	     internal_handle->header_values,
	     _S_LIBEWF_CHAR( "notes" ),
	     notes,
	     libewf_string_length( notes ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set notes.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value(
	     internal_handle->header_values,
	     _S_LIBEWF_CHAR( "acquiry_operating_system" ),
	     acquiry_operating_system,
	     libewf_string_length( acquiry_operating_system ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to acquiry operating system.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value(
	     internal_handle->header_values,
	     _S_LIBEWF_CHAR( "acquiry_software_version" ),
	     acquiry_software_version,
	     libewf_string_length( acquiry_software_version ) ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to acquiry software version.\n",
		 function );

		return( -1 );
	}
        /* The acquiry date, system date values and compression type
	 * will be generated automatically when set to NULL
         */
	return( 1 );
}

/* Create the header strings from the header values
 * Returns 1 on success, -1 on error
 */
int8_t libewf_internal_handle_create_headers( LIBEWF_INTERNAL_HANDLE *internal_handle, LIBEWF_HEADER_VALUES *header_values )
{
	static char *function = "libewf_internal_handle_create_headers";
	time_t timestamp      = time( NULL );

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->format == LIBEWF_FORMAT_EWF )
	{
		internal_handle->header = libewf_header_values_generate_header_string_ewf(
		                           header_values,
		                           timestamp,
		                           internal_handle->compression_level,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
	}
	else if( internal_handle->format == LIBEWF_FORMAT_ENCASE1 )
	{
		internal_handle->header = libewf_header_values_generate_header_string_encase1(
		                           header_values,
		                           timestamp,
		                           internal_handle->compression_level,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
	}
	else if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE2 )
	 || ( internal_handle->format == LIBEWF_FORMAT_ENCASE3 ) )
	{
		internal_handle->header = libewf_header_values_generate_header_string_encase2(
		                           header_values,
		                           timestamp,
		                           internal_handle->compression_level,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
	}
	else if( ( internal_handle->format == LIBEWF_FORMAT_FTK )
	 || ( internal_handle->format == LIBEWF_FORMAT_SMART ) )
	{
		internal_handle->header = libewf_header_values_generate_header_string_ftk(
		                           header_values,
		                           timestamp,
		                           internal_handle->compression_level,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
	}
	else if( internal_handle->format == LIBEWF_FORMAT_ENCASE4 )
	{
		internal_handle->header = libewf_header_values_generate_header_string_encase4(
		                           header_values,
		                           timestamp,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
		internal_handle->header2 = libewf_header_values_generate_header2_string_encase4(
		                            header_values,
		                            timestamp,
		                            &internal_handle->header2_size );

		if( internal_handle->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 values.\n",
			 function );

			libewf_common_free( internal_handle->header );

			internal_handle->header      = NULL;
			internal_handle->header_size = 0;

			return( -1 );
		}
	}
	else if( internal_handle->format == LIBEWF_FORMAT_ENCASE5 )
	{
		internal_handle->header = libewf_header_values_generate_header_string_encase4(
		                           header_values,
		                           timestamp,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
		internal_handle->header2 = libewf_header_values_generate_header2_string_encase5(
		                            header_values,
		                            timestamp,
		                            &internal_handle->header2_size );

		if( internal_handle->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 values.\n",
			 function );

			libewf_common_free( internal_handle->header );

			internal_handle->header      = NULL;
			internal_handle->header_size = 0;

			return( -1 );
		}
	}
	else if( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
	{
		internal_handle->header = libewf_header_values_generate_header_string_encase4(
		                           header_values,
		                           timestamp,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
		internal_handle->header2 = libewf_header_values_generate_header2_string_encase6(
		                            header_values,
		                            timestamp,
		                            &internal_handle->header2_size );

		if( internal_handle->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 values.\n",
			 function );

			libewf_common_free( internal_handle->header );

			internal_handle->header      = NULL;
			internal_handle->header_size = 0;

			return( -1 );
		}
	}
	else if( ( internal_handle->format == LIBEWF_FORMAT_LINEN5 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 ) )
	{
		internal_handle->header = libewf_header_values_generate_header_string_encase5_linen(
		                           header_values,
		                           timestamp,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
	}
	else if( internal_handle->format == LIBEWF_FORMAT_EWFX )
	{
		internal_handle->header = libewf_header_values_generate_header_string_ewfx(
		                           header_values,
		                           timestamp,
		                           &internal_handle->header_size );

		if( internal_handle->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
		internal_handle->header2 = libewf_header_values_generate_header2_string_ewfx(
		                            header_values,
		                            timestamp,
		                            &internal_handle->header2_size );

		if( internal_handle->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 values.\n",
			 function );

			libewf_common_free( internal_handle->header );

			internal_handle->header      = NULL;
			internal_handle->header_size = 0;

			return( -1 );
		}
		internal_handle->xheader = libewf_header_values_generate_xheader_string_ewfx(
		                            header_values,
		                            timestamp,
		                            &internal_handle->xheader_size );

		if( internal_handle->xheader == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create xheader values.\n",
			 function );

			libewf_common_free( internal_handle->header );
			libewf_common_free( internal_handle->header2 );

			internal_handle->header       = NULL;
			internal_handle->header_size  = 0;
			internal_handle->header2      = NULL;
			internal_handle->header2_size = 0;

			return( -1 );
		}
	}
	return( 1 );
}

/* Check if the read values have been initialized
 * Returns 0 if not initialized, 1 if set, or -1 on error
 */
int8_t libewf_internal_handle_read_is_initialized( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_read_is_initialized";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle read.\n",
		 function );

		return( -1 );
	}
	return( internal_handle->read->values_initialized );
}

/* Initializes the read values
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_read_initialize( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_read_initialize";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle read.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Check if the write values have been initialized
 * Returns 0 if not initialized, 1 if set, or -1 on error
 */
int8_t libewf_internal_handle_write_is_initialized( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function = "libewf_internal_handle_write_is_initialized";

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle write.\n",
		 function );

		return( -1 );
	}
	return( internal_handle->write->values_initialized );
}

/* Initializes the write values
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_internal_handle_write_initialize( LIBEWF_INTERNAL_HANDLE *internal_handle )
{
	static char *function               = "libewf_internal_handle_write_initialize";
	int64_t required_amount_of_segments = 0;
	int64_t amount_of_chunks            = 0;
	int64_t amount_of_sectors           = 0;
	uint64_t maximum_input_file_size    = 0;

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing chunk cache.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle write.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->values_initialized != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: write values were initialized and cannot be initialized anymore.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->chunk_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: the media chunk size cannot be zero - using default media values.\n",
		 function );

		internal_handle->media->sectors_per_chunk = 64;
		internal_handle->media->bytes_per_sector  = 512;
		internal_handle->media->chunk_size        = EWF_MINIMUM_CHUNK_SIZE;
	}
	if( internal_handle->media->chunk_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media chunk size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->segment_file_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: the segment file size cannot be zero - using default value.\n",
		 function );

		internal_handle->write->segment_file_size = LIBEWF_DEFAULT_SEGMENT_FILE_SIZE;
	}
	if( internal_handle->write->segment_file_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->compression_level != EWF_COMPRESSION_NONE ) && ( internal_handle->compression_level != EWF_COMPRESSION_FAST )
	 && ( internal_handle->compression_level != EWF_COMPRESSION_BEST ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported compression level - using default.\n",
		 function );

		internal_handle->compression_level           = EWF_COMPRESSION_NONE;
		internal_handle->write->compress_empty_block = 1;
	}
	/* Check if the input file size does not exceed the maximum input file size
	 */
	maximum_input_file_size = (uint64_t) internal_handle->media->chunk_size * (uint64_t) UINT32_MAX;

	if( internal_handle->write->input_write_size > maximum_input_file_size )
	{
		LIBEWF_WARNING_PRINT( "%s: input write size cannot be larger than size: %" PRIu64 " with a chunk size of: %" PRIu32 ".\n",
		 function, maximum_input_file_size, internal_handle->media->chunk_size );

		return( -1 );
	}
	if( internal_handle->write->input_write_size > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid input write size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Determine the EWF file format
	 */
	if( internal_handle->format == LIBEWF_FORMAT_LVF )
	{
		LIBEWF_WARNING_PRINT( "%s: writing format LVF currently not supported.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->format != LIBEWF_FORMAT_ENCASE1 )
	 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE2 )
	 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE3 )
	 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE4 )
	 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE5 )
	 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE6 )
	 && ( internal_handle->format != LIBEWF_FORMAT_LINEN5 )
	 && ( internal_handle->format != LIBEWF_FORMAT_LINEN6 )
	 && ( internal_handle->format != LIBEWF_FORMAT_SMART )
	 && ( internal_handle->format != LIBEWF_FORMAT_FTK )
	 && ( internal_handle->format != LIBEWF_FORMAT_LVF )
	 && ( internal_handle->format != LIBEWF_FORMAT_EWF )
	 && ( internal_handle->format != LIBEWF_FORMAT_EWFX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported format - using default.\n",
		 function );

		internal_handle->format = LIBEWF_FORMAT_ENCASE5;
	}
	if( ( internal_handle->format == LIBEWF_FORMAT_EWF )
	 || ( internal_handle->format == LIBEWF_FORMAT_SMART ) )
	{
		internal_handle->ewf_format = EWF_FORMAT_S01;
	}
	else if( internal_handle->format == LIBEWF_FORMAT_LVF )
	{
		internal_handle->ewf_format = EWF_FORMAT_L01;
	}
	else
	{
		internal_handle->ewf_format = EWF_FORMAT_E01;
	}
	if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_LINEN6 )
	 || ( internal_handle->format == LIBEWF_FORMAT_EWFX ) )
	{
		internal_handle->write->unrestrict_offset_amount = 1;
	}
	/* Determine the maximum amount of segments allowed to write
	 */
	internal_handle->write->maximum_amount_of_segments = libewf_internal_handle_get_write_maximum_amount_of_segments( internal_handle );

	if( internal_handle->write->maximum_amount_of_segments == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to determine the maximum amount of allowed segment files.\n",
		 function );

		return( -1 );
	}
	/* If no input write size was provided check if EWF file format allows for streaming
	 */
	if( internal_handle->write->input_write_size == 0 )
	{
		if( ( internal_handle->format != LIBEWF_FORMAT_ENCASE2 )
		 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE3 )
		 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE4 )
		 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE5 )
		 && ( internal_handle->format != LIBEWF_FORMAT_ENCASE6 )
		 && ( internal_handle->format != LIBEWF_FORMAT_LINEN5 )
		 && ( internal_handle->format != LIBEWF_FORMAT_LINEN6 )
		 && ( internal_handle->format != LIBEWF_FORMAT_FTK )
		 && ( internal_handle->format != LIBEWF_FORMAT_EWFX ) )
		{
			LIBEWF_WARNING_PRINT( "%s: EWF file format does not allow for streaming write.\n",
			 function );

			return( -1 );
		}
	}
	/* If an input write size was provided
	 */
	else if( internal_handle->write->input_write_size > 0 )
	{
		/* Determine the required amount of segments allowed to write
		 */
		required_amount_of_segments = (int64_t) internal_handle->write->input_write_size / (int64_t) internal_handle->write->segment_file_size;

		if( required_amount_of_segments > (int64_t) internal_handle->write->maximum_amount_of_segments )
		{
			LIBEWF_WARNING_PRINT( "%s: the settings exceed the maximum amount of allowed segment files.\n",
			 function );

			return( -1 );
		}
		/* Determine the amount of chunks to write
		 */
		amount_of_chunks = (int64_t) internal_handle->write->input_write_size / (int64_t) internal_handle->media->chunk_size;

		if( ( internal_handle->write->input_write_size % internal_handle->media->chunk_size ) != 0 )
		{
			amount_of_chunks += 1;
		}
		if( amount_of_chunks > (int64_t) UINT32_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: the settings exceed the maximum amount of allowed chunks.\n",
			 function );

			return( -1 );
		}
		internal_handle->media->amount_of_chunks = (uint32_t) amount_of_chunks;

		/* Determine the amount of sectors to write
		 */
		amount_of_sectors = (int64_t) internal_handle->write->input_write_size / (int64_t) internal_handle->media->bytes_per_sector;

		if( amount_of_chunks > (int64_t) UINT32_MAX )
		{
			LIBEWF_WARNING_PRINT( "%s: the settings exceed the maximum amount of allowed sectors.\n",
			 function );

			return( -1 );
		}
		internal_handle->media->amount_of_sectors = (uint32_t) amount_of_sectors;
	}
	/* Allocate the offset table if necessary
	 */
	if( internal_handle->offset_table == NULL )
	{
		/* Make sure at least one offset table entry is allocated
		 */
		if( amount_of_chunks ==  0 )
		{
			amount_of_chunks = 1;
		}
		internal_handle->offset_table = libewf_offset_table_alloc( (uint32_t) amount_of_chunks );

		if( internal_handle->offset_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate offset table.\n",
			 function );

			return( -1 );
		}
	}
	/* Make sure the chuck cache is large enough
	 */
	if( ( internal_handle->media->chunk_size + EWF_CRC_SIZE ) > internal_handle->chunk_cache->allocated_size )
	{
		if( libewf_internal_handle_chunk_cache_realloc( internal_handle, ( internal_handle->media->chunk_size + EWF_CRC_SIZE ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate chunk cache.\n",
			 function );

			return( -1 );
		}
	}
	/* Create the headers if required
	 */
	if( ( internal_handle->header == NULL )
	 && ( internal_handle->header2 == NULL )
	 && ( internal_handle->xheader == NULL ) )
	{
		if( internal_handle->header_values == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: empty header values - using default.\n",
			 function );

			if( libewf_internal_handle_create_header_values( internal_handle ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
				 function );

				return( -1 );
			}
		}
		if( libewf_internal_handle_create_headers( internal_handle, internal_handle->header_values ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header(s).\n",
			 function );

			return( -1 );
		}
	}
	/* Flag that the write values were initialized
	 */
	internal_handle->write->values_initialized = 1;

	return( 1 );
}


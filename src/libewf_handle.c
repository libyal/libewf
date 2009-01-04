/*
 * libewf handle
 *
 * Copyright (c) 2006, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include <stdlib.h>

#include "ewf_compress.h"
#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_file_header.h"
#include "ewf_md5hash.h"
#include "ewf_sectors.h"

#include "libewf_common.h"
#include "libewf_definitions.h"
#include "libewf_header_values.h"
#include "libewf_notify.h"
#include "libewf_handle.h"

/* Allocates memory for a new handle struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HANDLE *libewf_handle_alloc( uint32_t segment_amount )
{
	LIBEWF_HANDLE *handle = NULL;

	handle = (LIBEWF_HANDLE *) libewf_alloc( LIBEWF_HANDLE_SIZE );

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_handle_alloc: unable to allocate handle.\n" );

		return( NULL );
	}
	handle->chunk_size                = EWF_MINIMUM_CHUNK_SIZE;
	handle->sectors_per_chunk         = 0;
	handle->bytes_per_sector          = 0;
	handle->chunk_count               = 0;
	handle->sector_count              = 0;
	handle->input_file_size           = 0;
	handle->ewf_file_size             = 0;
	handle->chunks_per_file           = 0;
	handle->segment_table             = NULL;
	handle->offset_table              = NULL;
	handle->secondary_offset_table    = NULL;
	handle->error_granularity_sectors = 0;
	handle->error2_error_count        = 0;
	handle->error2_sectors            = NULL;
	handle->header                    = NULL;
	handle->header2                   = NULL;
	handle->md5hash                   = NULL;
	handle->swap_byte_pairs           = 0;
	handle->compression_used          = 0;
	handle->wipe_block_on_read_error  = 0;
	handle->compression_level         = EWF_COMPRESSION_NONE;
	handle->compress_empty_block      = 0;
	handle->format                    = LIBEWF_FORMAT_UNKNOWN;
	handle->ewf_format                = EWF_FORMAT_E01;
	handle->index_build               = 0;
	handle->error_tollerance          = LIBEWF_ERROR_TOLLERANCE_COMPENSATE;
	handle->read_error_retry          = 0;
	handle->media_type                = 0;

	handle->segment_table = libewf_segment_table_alloc( segment_amount );

	if( handle->segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_handle_alloc: unable to create segment table.\n" );

		libewf_free( handle );

		return( NULL );
	}
	handle->chunk_cache = libewf_chunk_cache_alloc( EWF_MINIMUM_CHUNK_SIZE + EWF_CRC_SIZE );

	if( handle->chunk_cache == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewf_handle_alloc: unable to create chunk cache.\n" );

		libewf_segment_table_free( handle->segment_table );
		libewf_free( handle );

		return( NULL );
	}
	return( handle );
}

/* Frees memory of a handle struct including elements
 */
void libewf_handle_free( LIBEWF_HANDLE *handle )
{
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_free: invalid handle.\n" );

		return;
	}
	if( handle->segment_table != NULL )
	{
		libewf_segment_table_free( handle->segment_table );
	}
	if( handle->offset_table != NULL )
	{
		libewf_offset_table_free( handle->offset_table );
	}
	if( handle->secondary_offset_table != NULL )
	{
		libewf_offset_table_free( handle->secondary_offset_table );
	}
	if( handle->error2_sectors != NULL )
	{
		ewf_error2_sectors_free( handle->error2_sectors );
	}
	if( handle->header != NULL )
	{
		ewf_header_free( handle->header );
	}
	if( handle->header2 != NULL )
	{
		ewf_header_free( handle->header2 );
	}
	if( handle->md5hash != NULL )
	{
		ewf_md5hash_free( handle->md5hash );
	}
	if( handle->chunk_cache != NULL )
	{
		libewf_chunk_cache_free( handle->chunk_cache );
	}
	libewf_free( handle );
}

/* Check if the header value is set
 * Return 0 if not set, 1 if set
 */
uint8_t libewf_handle_is_set_header( LIBEWF_HANDLE *handle )
{
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_is_set_header: invalid handle.\n" );

		return( 0 );
	}
	return( handle->header != NULL );
}

/* Check if the header2 value is set
 * Return 0 if not set, 1 if set
 */
uint8_t libewf_handle_is_set_header2( LIBEWF_HANDLE *handle )
{
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_is_set_header2: invalid handle.\n" );

		return( 0 );
	}
	return( handle->header2 != NULL );
}

/* Sets the header
 */
void libewf_handle_set_header( LIBEWF_HANDLE *handle, EWF_HEADER *header )
{
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_set_header: invalid handle.\n" );

		return;
	}
	handle->header = header;
}

/* Sets the header2
 */
void libewf_handle_set_header2( LIBEWF_HANDLE *handle, EWF_HEADER *header2 )
{
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_set_header2: invalid handle.\n" );

		return;
	}
	handle->header2 = header2;
}

/* Sets the MD5 hash value
 */
void libewf_handle_set_md5hash( LIBEWF_HANDLE *handle, EWF_MD5HASH *md5hash )
{
	EWF_MD5HASH *data_set = NULL;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_set_md5hash: invalid handle.\n" );

		return;
	}
	handle->md5hash = ewf_md5hash_alloc();

	if( handle->md5hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_set_md5hash: unable to create MD5 hash.\n" );

		return;
	}
	data_set = (EWF_MD5HASH *) libewf_memcpy( (void *) handle->md5hash, (void *) md5hash, EWF_MD5HASH_SIZE );

	if( data_set == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_handle_set_md5hash: unable to set MD5 hash.\n" );

		ewf_md5hash_free( data_set );

		return;
	}
}


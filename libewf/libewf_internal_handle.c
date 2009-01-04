/*
 * libewf handle
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <common.h>
#include <character_string.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_header_values.h"
#include "libewf_internal_handle.h"
#include "libewf_string.h"

#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"

/* Allocates memory for a new handle struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_internal_handle_t *libewf_internal_handle_alloc(
                           uint8_t flags )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_internal_handle_alloc";

	internal_handle = (libewf_internal_handle_t *) memory_allocate(
	                                                sizeof( libewf_internal_handle_t ) );

	if( internal_handle == NULL )
	{
		notify_warning_printf( "%s: unable to allocate handle.\n",
		 function );

		return( NULL );
	}
	internal_handle->read                     = NULL;
	internal_handle->write                    = NULL;
	internal_handle->media_values             = NULL;
	internal_handle->segment_table            = NULL;
	internal_handle->delta_segment_table      = NULL;
	internal_handle->offset_table             = NULL;
	internal_handle->secondary_offset_table   = NULL;
	internal_handle->chunk_cache              = NULL;
	internal_handle->header_sections          = NULL;
	internal_handle->hash_sections            = NULL;
	internal_handle->header_values            = NULL;
	internal_handle->hash_values              = NULL;
	internal_handle->sessions                 = NULL;
	internal_handle->acquiry_errors           = NULL;
	internal_handle->current_chunk            = 0;
	internal_handle->current_chunk_offset     = 0;
	internal_handle->compression_level        = EWF_COMPRESSION_UNKNOWN;
	internal_handle->compress_empty_block     = 0;
	internal_handle->format                   = LIBEWF_FORMAT_UNKNOWN;
	internal_handle->ewf_format               = EWF_FORMAT_UNKNOWN;
	internal_handle->error_tollerance         = LIBEWF_ERROR_TOLLERANCE_COMPENSATE;
	internal_handle->abort                    = 0;

	/* The segment table is initially filled with a single entry
	 */
	internal_handle->segment_table = libewf_segment_table_alloc( 1 );

	if( internal_handle->segment_table == NULL )
	{
		notify_warning_printf( "%s: unable to create segment table.\n",
		 function );

		memory_free(
		 internal_handle );

		return( NULL );
	}
	/* The delta segment table is initially filled with a single entry
	 */
	internal_handle->delta_segment_table = libewf_segment_table_alloc( 1 );

	if( internal_handle->delta_segment_table == NULL )
	{
		notify_warning_printf( "%s: unable to create delta segment table.\n",
		 function );

		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->offset_table = libewf_offset_table_alloc( 0 );

	if( internal_handle->offset_table == NULL )
	{
		notify_warning_printf( "%s: unable to create offset table.\n",
		 function );

		libewf_segment_table_free(
		 internal_handle->segment_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->secondary_offset_table = libewf_offset_table_alloc( 0 );

	if( internal_handle->secondary_offset_table == NULL )
	{
		notify_warning_printf( "%s: unable to create secondary offset table.\n",
		 function );

		libewf_offset_table_free(
		 internal_handle->offset_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->chunk_cache = libewf_chunk_cache_alloc(
	                                EWF_MINIMUM_CHUNK_SIZE + sizeof( ewf_crc_t ) );

	if( internal_handle->chunk_cache == NULL )
	{
		notify_warning_printf( "%s: unable to create chunk cache.\n",
		 function );

		libewf_offset_table_free(
		 internal_handle->secondary_offset_table );
		libewf_offset_table_free(
		 internal_handle->offset_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->media_values = libewf_media_values_alloc();

	if( internal_handle->media_values == NULL )
	{
		notify_warning_printf( "%s: unable to create media values.\n",
		 function );

		libewf_chunk_cache_free(
		 internal_handle->chunk_cache );
		libewf_offset_table_free(
		 internal_handle->secondary_offset_table );
		libewf_offset_table_free(
		 internal_handle->offset_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->header_sections = libewf_header_sections_alloc();

	if( internal_handle->header_sections == NULL )
	{
		notify_warning_printf( "%s: unable to create header sections.\n",
		 function );

		libewf_media_values_free(
		 internal_handle->media_values );
		libewf_chunk_cache_free(
		 internal_handle->chunk_cache );
		libewf_offset_table_free(
		 internal_handle->secondary_offset_table );
		libewf_offset_table_free(
		 internal_handle->offset_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->hash_sections = libewf_hash_sections_alloc();

	if( internal_handle->hash_sections == NULL )
	{
		notify_warning_printf( "%s: unable to create hash sections.\n",
		 function );

		libewf_header_sections_free(
		 internal_handle->header_sections );
		libewf_media_values_free(
		 internal_handle->media_values );
		libewf_chunk_cache_free(
		 internal_handle->chunk_cache );
		libewf_offset_table_free(
		 internal_handle->secondary_offset_table );
		libewf_offset_table_free(
		 internal_handle->offset_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->sessions = libewf_sector_table_alloc( 0 );

	if( internal_handle->sessions == NULL )
	{
		notify_warning_printf( "%s: unable to create sessions.\n",
		 function );

		libewf_hash_sections_free(
		 internal_handle->hash_sections );
		libewf_header_sections_free(
		 internal_handle->header_sections );
		libewf_media_values_free(
		 internal_handle->media_values );
		libewf_chunk_cache_free(
		 internal_handle->chunk_cache );
		libewf_offset_table_free(
		 internal_handle->secondary_offset_table );
		libewf_offset_table_free(
		 internal_handle->offset_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	internal_handle->acquiry_errors = libewf_sector_table_alloc( 0 );

	if( internal_handle->acquiry_errors == NULL )
	{
		notify_warning_printf( "%s: unable to create acquiry errors.\n",
		 function );

		libewf_sector_table_free(
		 internal_handle->sessions );
		libewf_hash_sections_free(
		 internal_handle->hash_sections );
		libewf_header_sections_free(
		 internal_handle->header_sections );
		libewf_media_values_free(
		 internal_handle->media_values );
		libewf_chunk_cache_free(
		 internal_handle->chunk_cache );
		libewf_offset_table_free(
		 internal_handle->secondary_offset_table );
		libewf_offset_table_free(
		 internal_handle->offset_table );
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
		libewf_segment_table_free(
		 internal_handle->segment_table );
		memory_free(
		 internal_handle );

		return( NULL );
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		internal_handle->read = libewf_internal_handle_read_alloc();

		if( internal_handle->read == NULL )
		{
			notify_warning_printf( "%s: unable to create subhandle read.\n",
			 function );

			libewf_sector_table_free(
			 internal_handle->acquiry_errors );
			libewf_sector_table_free(
			 internal_handle->sessions );
			libewf_hash_sections_free(
			 internal_handle->hash_sections );
			libewf_header_sections_free(
			 internal_handle->header_sections );
			libewf_media_values_free(
			 internal_handle->media_values );
			libewf_chunk_cache_free(
			 internal_handle->chunk_cache );
			libewf_offset_table_free(
			 internal_handle->secondary_offset_table );
			libewf_offset_table_free(
			 internal_handle->offset_table );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( NULL );
		}
	}
	if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		internal_handle->write = libewf_internal_handle_write_alloc();

		if( internal_handle->write == NULL )
		{
			notify_warning_printf( "%s: unable to create subhandle write.\n",
			 function );

			if( internal_handle->read != NULL )
			{
				libewf_internal_handle_read_free(
				 internal_handle->read );
			}
			libewf_sector_table_free(
			 internal_handle->acquiry_errors );
			libewf_sector_table_free(
			 internal_handle->sessions );
			libewf_hash_sections_free(
			 internal_handle->hash_sections );
			libewf_header_sections_free(
			 internal_handle->header_sections );
			libewf_media_values_free(
			 internal_handle->media_values );
			libewf_chunk_cache_free(
			 internal_handle->chunk_cache );
			libewf_offset_table_free(
			 internal_handle->secondary_offset_table );
			libewf_offset_table_free(
			 internal_handle->offset_table );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( NULL );
		}
	}
	return( internal_handle );
}

/* Frees memory of a handle struct including elements
 */
void libewf_internal_handle_free(
      libewf_internal_handle_t *internal_handle )
{
	static char *function = "libewf_internal_handle_free";

	if( internal_handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( internal_handle->media_values != NULL )
	{
		libewf_media_values_free(
		 internal_handle->media_values );
	}
	if( internal_handle->read != NULL )
	{
		libewf_internal_handle_read_free(
		 internal_handle->read );
	}
	if( internal_handle->write != NULL )
	{
		libewf_internal_handle_write_free(
		 internal_handle->write );
	}
	if( internal_handle->segment_table != NULL )
	{
		libewf_segment_table_free(
		 internal_handle->segment_table );
	}
	if( internal_handle->delta_segment_table != NULL )
	{
		libewf_segment_table_free(
		 internal_handle->delta_segment_table );
	}
	if( internal_handle->offset_table != NULL )
	{
		libewf_offset_table_free(
		 internal_handle->offset_table );
	}
	if( internal_handle->secondary_offset_table != NULL )
	{
		libewf_offset_table_free(
		 internal_handle->secondary_offset_table );
	}
	if( internal_handle->sessions != NULL )
	{
		libewf_sector_table_free(
		 internal_handle->sessions );
	}
	if( internal_handle->acquiry_errors != NULL )
	{
		libewf_sector_table_free(
		 internal_handle->acquiry_errors );
	}
	if( internal_handle->header_sections != NULL )
	{
		libewf_header_sections_free(
		 internal_handle->header_sections );
	}
	if( internal_handle->hash_sections != NULL )
	{
		libewf_hash_sections_free(
		 internal_handle->hash_sections );
	}
	if( internal_handle->header_values != NULL )
	{
		libewf_values_table_free(
		 internal_handle->header_values );
	}
	if( internal_handle->hash_values != NULL )
	{
		libewf_values_table_free(
		 internal_handle->hash_values );
	}
	if( internal_handle->chunk_cache != NULL )
	{
		libewf_chunk_cache_free(
		 internal_handle->chunk_cache );
	}
	memory_free(
	 internal_handle );
}

/* Allocates memory for a new handle read struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_internal_handle_read_t *libewf_internal_handle_read_alloc(
                                void )
{
	libewf_internal_handle_read_t *handle_read = NULL;
	static char *function                      = "libewf_internal_handle_read_alloc";

	handle_read = (libewf_internal_handle_read_t *) memory_allocate(
	                                                 sizeof( libewf_internal_handle_read_t ) );

	if( handle_read == NULL )
	{
		notify_warning_printf( "%s: unable to allocate handle read.\n",
		 function );

		return( NULL );
	}
	handle_read->crc_errors = libewf_sector_table_alloc( 0 );

	if( handle_read->crc_errors == NULL )
	{
		notify_warning_printf( "%s: unable to allocate crc errors.\n",
		 function );

		memory_free(
		 handle_read );

		return( NULL );
	}
	handle_read->wipe_on_error = 1;

	return( handle_read );
}

/* Frees memory of a handle read struct including elements
 */
void libewf_internal_handle_read_free(
      libewf_internal_handle_read_t *handle_read )
{
	static char *function = "libewf_internal_handle_read_free";

	if( handle_read == NULL )
	{
		notify_warning_printf( "%s: invalid handle read.\n",
		 function );

		return;
	}
	if( handle_read->crc_errors != NULL )
	{
		libewf_sector_table_free( handle_read->crc_errors );
	}
	memory_free(
	 handle_read );
}

/* Allocates memory for a new handle write struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_internal_handle_write_t *libewf_internal_handle_write_alloc( void )
{
	libewf_internal_handle_write_t *handle_write = NULL;
	static char *function                        = "libewf_internal_handle_write_alloc";

	handle_write = (libewf_internal_handle_write_t *) memory_allocate(
	                                                   sizeof( libewf_internal_handle_write_t ) );

	if( handle_write == NULL )
	{
		notify_warning_printf( "%s: unable to allocate handle write.\n",
		 function );

		return( NULL );
	}
	handle_write->data_section                     = NULL;
	handle_write->input_write_count                = 0;
	handle_write->write_count                      = 0;
	handle_write->maximum_segment_file_size        = INT32_MAX;
	handle_write->segment_file_size                = LIBEWF_DEFAULT_SEGMENT_FILE_SIZE;
	handle_write->delta_segment_file_size          = INT64_MAX;
	handle_write->maximum_amount_of_segments       = 0;
	handle_write->chunks_section_write_count       = 0;
	handle_write->amount_of_chunks                 = 0;
	handle_write->chunks_per_segment               = 0;
	handle_write->chunks_per_chunks_section        = 0;
	handle_write->segment_amount_of_chunks         = 0;
	handle_write->maximum_section_amount_of_chunks = EWF_MAXIMUM_OFFSETS_IN_TABLE;
	handle_write->section_amount_of_chunks         = 0;
	handle_write->chunks_section_offset            = 0;
	handle_write->chunks_section_number            = 0;
	handle_write->unrestrict_offset_amount         = 0;
	handle_write->values_initialized               = 0;
	handle_write->create_chunks_section            = 0;
	handle_write->write_finalized                  = 0;

	return( handle_write );
}

/* Frees memory of a handle write struct including elements
 */
void libewf_internal_handle_write_free(
      libewf_internal_handle_write_t *handle_write )
{
	static char *function = "libewf_internal_handle_write_free";

	if( handle_write == NULL )
	{
		notify_warning_printf( "%s: invalid handle write.\n",
		 function );

		return;
	}
	if( handle_write->data_section != NULL )
	{
		memory_free(
		 handle_write->data_section );
	}
	memory_free(
	 handle_write );
}

/* Retrieves the maximum amount of supported segment files to write
 * Returns 1 if successful, or -1 on error
 */
int libewf_internal_handle_get_write_maximum_amount_of_segments(
     uint8_t ewf_format,
     uint16_t *maximum_amount_of_segments )
{
	static char *function = "libewf_internal_handle_get_write_maximum_amount_of_segments";

	if( maximum_amount_of_segments == NULL )
	{
		notify_warning_printf( "%s: invalid maximum amount of segments.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* = 4831
		 */
		*maximum_amount_of_segments = (uint16_t) ( ( (int) ( 'z' - 's' ) * 26 * 26 ) + 99 );
	}
	else if( ewf_format == EWF_FORMAT_E01 )
	{
		/* = 14295
		 */
		*maximum_amount_of_segments = (uint16_t) ( ( (int) ( 'Z' - 'E' ) * 26 * 26 ) + 99 );
	}
	else
	{
		notify_warning_printf( "%s: unsupported EWF format.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Inializes the media values
 * Returns 1 if the values were successfully initialized, -1 on errror
 */
int libewf_internal_handle_initialize_media_values(
     libewf_internal_handle_t *internal_handle, 
     uint32_t sectors_per_chunk, 
     uint32_t bytes_per_sector, 
     size64_t media_size )
{
	static char *function            = "libewf_internal_handle_initialize_format";
	size32_t chunk_size              = 0;
	size64_t maximum_input_file_size = 0;
	int64_t amount_of_chunks         = 0;
	int64_t amount_of_sectors        = 0;

	if( internal_handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		notify_warning_printf( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( ( sectors_per_chunk == 0 )
	 || ( sectors_per_chunk > (uint32_t) INT32_MAX ) )
	{
		notify_warning_printf( "%s: invalid sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( ( bytes_per_sector == 0 )
	 || ( bytes_per_sector > (uint32_t) INT32_MAX ) )
	{
		notify_warning_printf( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		notify_warning_printf( "%s: invalid media size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* Determine the chunk size
	 */
	chunk_size = sectors_per_chunk * bytes_per_sector;

	if( ( chunk_size == 0 )
	 || ( chunk_size > (size32_t) INT32_MAX ) )
	{
		notify_warning_printf( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	/* Check if the input file size does not exceed the maximum possible input file size
	 * for the chunk size
	 */
	maximum_input_file_size = (size64_t) chunk_size * (size64_t) UINT32_MAX;

	if( media_size > maximum_input_file_size )
	{
		notify_warning_printf( "%s: media size cannot be larger than size: %" PRIu64 " with a chunk size of: %" PRIu32 ".\n",
		 function, maximum_input_file_size, chunk_size );

		return( -1 );
	}
	internal_handle->media_values->sectors_per_chunk = sectors_per_chunk;
	internal_handle->media_values->bytes_per_sector  = bytes_per_sector;
	internal_handle->media_values->chunk_size        = chunk_size;
	internal_handle->media_values->media_size        = media_size;

	/* If a media size was provided
	 */
	if( media_size > 0 )
	{
		/* Determine the amount of chunks to write
		 */
		amount_of_chunks = (int64_t) media_size / (int64_t) chunk_size;

		if( ( media_size % chunk_size ) != 0 )
		{
			amount_of_chunks += 1;
		}
		if( amount_of_chunks > (int64_t) UINT32_MAX )
		{
			notify_warning_printf( "%s: invalid amount of chunks value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		internal_handle->media_values->amount_of_chunks = (uint32_t) amount_of_chunks;

		/* Determine the amount of sectors to write
		 */
		amount_of_sectors = (int64_t) media_size / (int64_t) bytes_per_sector;

		if( amount_of_chunks > (int64_t) UINT32_MAX )
		{
			notify_warning_printf( "%s: invalid amount of sectors value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		internal_handle->media_values->amount_of_sectors = (uint32_t) amount_of_sectors;
	}
	return( 1 );
}

/* Inializes internal values based on the EWF file format
 * Returns 1 if the values were successfully initialized, -1 on errror
 */
int libewf_internal_handle_initialize_format(
     libewf_internal_handle_t *internal_handle )
{
	static char *function = "libewf_internal_handle_initialize_format";

	if( internal_handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
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
	if( internal_handle->write != NULL )
	{
		if( internal_handle->format == LIBEWF_FORMAT_ENCASE6 )
		{
			internal_handle->write->maximum_segment_file_size        = INT64_MAX;
			internal_handle->write->maximum_section_amount_of_chunks = EWF_MAXIMUM_OFFSETS_IN_TABLE_ENCASE6;
		}
		else if( internal_handle->format == LIBEWF_FORMAT_EWFX )
		{
			internal_handle->write->unrestrict_offset_amount         = 1;
			internal_handle->write->maximum_segment_file_size        = INT32_MAX;
			internal_handle->write->maximum_section_amount_of_chunks = INT32_MAX;
		}
		else
		{
			internal_handle->write->maximum_segment_file_size        = INT32_MAX;
			internal_handle->write->maximum_section_amount_of_chunks = EWF_MAXIMUM_OFFSETS_IN_TABLE;
		}
		/* Determine the maximum amount of segments allowed to write
		 */
		if( libewf_internal_handle_get_write_maximum_amount_of_segments(
		     internal_handle->ewf_format,
		     &( internal_handle->write->maximum_amount_of_segments ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to determine the maximum amount of allowed segment files.\n",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Create the default header values
 * Returns 1 on success, -1 on error
 */
int libewf_internal_handle_create_header_values(
     libewf_internal_handle_t *internal_handle )
{
	character_t *case_number              = _CHARACTER_T_STRING( "Case Number" );
	character_t *description              = _CHARACTER_T_STRING( "Description" );
	character_t *evidence_number          = _CHARACTER_T_STRING( "Evidence Number" );
	character_t *examiner_name            = _CHARACTER_T_STRING( "Examiner Name" );
	character_t *notes                    = _CHARACTER_T_STRING( "Notes" );
	character_t *acquiry_operating_system = _CHARACTER_T_STRING( "Undetermined" );
	character_t *acquiry_software_version = LIBEWF_VERSION_STRING;
	static char *function                 = "libewf_internal_handle_create_header_values";

	if( internal_handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->header_values != NULL )
	{
		notify_warning_printf( "%s: header values already created - cleaning up previous header values.\n",
		 function );

		libewf_values_table_free(
		 internal_handle->header_values );
	}
	internal_handle->header_values = libewf_values_table_alloc(
	                                  LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT );

	if( internal_handle->header_values == NULL )
	{
		notify_warning_printf( "%s: unable to create header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_initialize(
	     internal_handle->header_values ) != 1 )
	{
		notify_warning_printf( "%s: unable to initialize the header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_value(
	     internal_handle->header_values,
	     _CHARACTER_T_STRING( "case_number" ),
	     case_number,
	     string_length(
	      case_number ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set case number.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_value(
	     internal_handle->header_values,
	     _CHARACTER_T_STRING( "description" ),
	     description,
	     string_length(
	      description ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set description.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_value(
	     internal_handle->header_values,
	     _CHARACTER_T_STRING( "evidence_number" ),
	     evidence_number,
	     string_length(
	      evidence_number ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set evidence number.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_value(
	     internal_handle->header_values,
	     _CHARACTER_T_STRING( "examiner_name" ),
	     examiner_name,
	     string_length(
	      examiner_name ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set examiner name.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_value(
	     internal_handle->header_values,
	     _CHARACTER_T_STRING( "notes" ),
	     notes,
	     string_length(
	      notes ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set notes.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_value(
	     internal_handle->header_values,
	     _CHARACTER_T_STRING( "acquiry_operating_system" ),
	     acquiry_operating_system,
	     string_length(
	      acquiry_operating_system ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to acquiry operating system.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_value(
	     internal_handle->header_values,
	     _CHARACTER_T_STRING( "acquiry_software_version" ),
	     acquiry_software_version,
	     string_length(
	      acquiry_software_version ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to acquiry software version.\n",
		 function );

		return( -1 );
	}
        /* The acquiry date, system date values and compression type
	 * will be generated automatically when set to NULL
         */
	return( 1 );
}

/* Initializes the write values
 * Returns 1 if successful, -1 on error
 */
int libewf_internal_handle_write_initialize(
     libewf_internal_handle_t *internal_handle )
{
	static char *function               = "libewf_internal_handle_write_initialize";
	int64_t required_amount_of_segments = 0;

	if( internal_handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		notify_warning_printf( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write == NULL )
	{
		notify_warning_printf( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->values_initialized != 0 )
	{
		notify_warning_printf( "%s: write values were initialized and cannot be initialized anymore.\n",
		 function );

		return( -1 );
	}
	/* Determine the EWF file format
	 */
	if( internal_handle->format == LIBEWF_FORMAT_LVF )
	{
		notify_warning_printf( "%s: writing format LVF currently not supported.\n",
		 function );

		return( -1 );
	}
	/* If no input write size was provided check if EWF file format allows for streaming
	 */
	if( internal_handle->media_values->media_size == 0 )
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
			notify_warning_printf( "%s: EWF file format does not allow for streaming write.\n",
			 function );

			return( -1 );
		}
	}
	/* If an input write size was provided
	 */
	else if( internal_handle->media_values->media_size > 0 )
	{
		/* Determine the required amount of segments allowed to write
		 */
		required_amount_of_segments = (int64_t) internal_handle->media_values->media_size
		                            / (int64_t) internal_handle->write->segment_file_size;

		if( required_amount_of_segments > (int64_t) internal_handle->write->maximum_amount_of_segments )
		{
			notify_warning_printf( "%s: the settings exceed the maximum amount of allowed segment files.\n",
			 function );

			return( -1 );
		}
	}
	/* Flag that the write values were initialized
	 */
	internal_handle->write->values_initialized = 1;

	return( 1 );
}


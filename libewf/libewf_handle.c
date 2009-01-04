/*
 * libewf main handle
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <character_string.h>
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_handle.h"
#include "libewf_header_values.h"
#include "libewf_string.h"

#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"

/* Initialize the handle
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_initialize(
     libewf_handle_t **handle,
     uint8_t flags )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_initialize";

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( *handle == NULL )
	{
		internal_handle = (libewf_internal_handle_t *) memory_allocate(
		                                                sizeof( libewf_internal_handle_t ) );

		if( internal_handle == NULL )
		{
			notify_warning_printf( "%s: unable to create internal handle.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_handle,
		     0,
		     sizeof( libewf_internal_handle_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear internal handle.\n",
			 function );

			memory_free(
			 internal_handle );

			return( -1 );
		}
		internal_handle->compression_level = EWF_COMPRESSION_UNKNOWN;
		internal_handle->format            = LIBEWF_FORMAT_UNKNOWN;
		internal_handle->ewf_format        = EWF_FORMAT_UNKNOWN;
		internal_handle->error_tollerance  = LIBEWF_ERROR_TOLLERANCE_COMPENSATE;

		/* The segment table is initially filled with a single entry
		 */
		internal_handle->segment_table = libewf_segment_table_alloc( 1 );

		if( internal_handle->segment_table == NULL )
		{
			notify_warning_printf( "%s: unable to create segment table.\n",
			 function );

			memory_free(
			 internal_handle );

			return( -1 );
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

			return( -1 );
		}
		if( libewf_offset_table_initialize(
		     &( internal_handle->offset_table ),
		     0 ) != 1 )
		{
			notify_warning_printf( "%s: unable to create offset table.\n",
			 function );

			libewf_segment_table_free(
			 internal_handle->segment_table );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_chunk_cache_initialize(
		     &( internal_handle->chunk_cache ),
		     EWF_MINIMUM_CHUNK_SIZE + sizeof( ewf_crc_t ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create chunk cache.\n",
			 function );

			libewf_offset_table_free(
			 &( internal_handle->offset_table ) );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_media_values_initialize(
		     &( internal_handle->media_values ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create media values.\n",
			 function );

			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ) );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ) );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_header_sections_initialize(
		     &( internal_handle->header_sections ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header sections.\n",
			 function );

			libewf_media_values_free(
			 &( internal_handle->media_values ) );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ) );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ) );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_hash_sections_initialize(
		     &( internal_handle->hash_sections ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create hash sections.\n",
			 function );

			libewf_header_sections_free(
			 &( internal_handle->header_sections ) );
			libewf_media_values_free(
			 &( internal_handle->media_values ) );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ) );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ) );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		internal_handle->sessions = libewf_sector_table_alloc( 0 );

		if( internal_handle->sessions == NULL )
		{
			notify_warning_printf( "%s: unable to create sessions.\n",
			 function );

			libewf_hash_sections_free(
			 &( internal_handle->hash_sections ) );
			libewf_header_sections_free(
			 &( internal_handle->header_sections ) );
			libewf_media_values_free(
			 &( internal_handle->media_values ) );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ) );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ) );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		internal_handle->acquiry_errors = libewf_sector_table_alloc( 0 );

		if( internal_handle->acquiry_errors == NULL )
		{
			notify_warning_printf( "%s: unable to create acquiry errors.\n",
			 function );

			libewf_sector_table_free(
			 internal_handle->sessions );
			libewf_hash_sections_free(
			 &( internal_handle->hash_sections ) );
			libewf_header_sections_free(
			 &( internal_handle->header_sections ) );
			libewf_media_values_free(
			 &( internal_handle->media_values ) );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ) );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ) );
			libewf_segment_table_free(
			 internal_handle->delta_segment_table );
			libewf_segment_table_free(
			 internal_handle->segment_table );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
		{
			if( libewf_internal_handle_subhandle_read_initialize(
			     &( internal_handle->read ) ) != 1 )
			{
				notify_warning_printf( "%s: unable to create subhandle read.\n",
				 function );

				libewf_sector_table_free(
				 internal_handle->acquiry_errors );
				libewf_sector_table_free(
				 internal_handle->sessions );
				libewf_hash_sections_free(
				 &( internal_handle->hash_sections ) );
				libewf_header_sections_free(
				 &( internal_handle->header_sections ) );
				libewf_media_values_free(
				 &( internal_handle->media_values ) );
				libewf_chunk_cache_free(
				 &( internal_handle->chunk_cache ) );
				libewf_offset_table_free(
				 &( internal_handle->offset_table ) );
				libewf_segment_table_free(
				 internal_handle->delta_segment_table );
				libewf_segment_table_free(
				 internal_handle->segment_table );
				memory_free(
				 internal_handle );

				return( -1 );
			}
		}
		if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
		{
			if( libewf_internal_handle_subhandle_write_initialize(
			     &( internal_handle->write ) ) != 1 )
			{
				notify_warning_printf( "%s: unable to create subhandle write.\n",
				 function );

				libewf_internal_handle_subhandle_read_free(
				 &( internal_handle->read ) );
				libewf_sector_table_free(
				 internal_handle->acquiry_errors );
				libewf_sector_table_free(
				 internal_handle->sessions );
				libewf_hash_sections_free(
				 &( internal_handle->hash_sections ) );
				libewf_header_sections_free(
				 &( internal_handle->header_sections ) );
				libewf_media_values_free(
				 &( internal_handle->media_values ) );
				libewf_chunk_cache_free(
				 &( internal_handle->chunk_cache ) );
				libewf_offset_table_free(
				 &( internal_handle->offset_table ) );
				libewf_segment_table_free(
				 internal_handle->delta_segment_table );
				libewf_segment_table_free(
				 internal_handle->segment_table );
				memory_free(
				 internal_handle );

				return( -1 );
			}
		}
		*handle = (libewf_handle_t *) internal_handle;
	}
	return( 1 );
}

/* Frees the handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_free(
     libewf_handle_t **handle )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_internal_handle_free";

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libewf_internal_handle_t *) *handle;

		if( libewf_media_values_free(
		     &( internal_handle->media_values ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free media values.\n",
			 function );
		}
		if( libewf_internal_handle_subhandle_read_free(
		     &( internal_handle->read ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free subhandle read.\n",
			 function );
		}
		if( libewf_internal_handle_subhandle_write_free(
		     &( internal_handle->write ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free subhandle write.\n",
			 function );
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
		if( libewf_offset_table_free(
		     &( internal_handle->offset_table ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free offset table.\n",
			 function );
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
		if( libewf_header_sections_free(
		     &( internal_handle->header_sections ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free header sections.\n",
			 function );
		}
		if( libewf_hash_sections_free(
		     &( internal_handle->hash_sections ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free hash sections.\n",
			 function );
		}
		if( libewf_values_table_free(
		     &( internal_handle->header_values ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free header values.\n",
			 function );
		}
		if( libewf_values_table_free(
		     &( internal_handle->hash_values ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free hash values.\n",
			 function );
		}
		if( libewf_chunk_cache_free(
		     &( internal_handle->chunk_cache ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to free chunk cache.\n",
			 function );
		}
		memory_free(
		 internal_handle );

		*handle = NULL;
	}
	return( 1 );
}

/* Initialize the subhandle read
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_subhandle_read_initialize(
     libewf_internal_handle_read_t **subhandle_read )
{
	static char *function = "libewf_internal_handle_subhandle_read_initialize";

	if( subhandle_read == NULL )
	{
		notify_warning_printf( "%s: invalid subhandle read.\n",
		 function );

		return( -1 );
	}
	if( *subhandle_read == NULL )
	{
		*subhandle_read = (libewf_internal_handle_read_t *) memory_allocate(
		                                                     sizeof( libewf_internal_handle_read_t ) );

		if( *subhandle_read == NULL )
		{
			notify_warning_printf( "%s: unable to create subhandle read.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *subhandle_read,
		     0,
		     sizeof( libewf_internal_handle_read_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear subhandle read.\n",
			 function );

			memory_free(
			 *subhandle_read );

			*subhandle_read = NULL;

			return( -1 );
		}
		( *subhandle_read )->crc_errors = libewf_sector_table_alloc( 0 );

		if( ( *subhandle_read )->crc_errors == NULL )
		{
			notify_warning_printf( "%s: unable to create crc errors.\n",
			 function );

			memory_free(
			 *subhandle_read );

			*subhandle_read = NULL;

			return( -1 );
		}
		( *subhandle_read )->wipe_on_error = 1;
	}
	return( 1 );
}

/* Frees the subhandle read including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_subhandle_read_free(
     libewf_internal_handle_read_t **subhandle_read )
{
	static char *function = "libewf_internal_subhandle_read_free";

	if( subhandle_read == NULL )
	{
		notify_warning_printf( "%s: invalid subhandle read.\n",
		 function );

		return( 1 );
	}
	if( *subhandle_read != NULL )
	{
		if( ( *subhandle_read )->crc_errors != NULL )
		{
			libewf_sector_table_free(
			 ( *subhandle_read )->crc_errors );
		}
		memory_free(
		 *subhandle_read );

		*subhandle_read = NULL;
	}
	return( 1 );
}

/* Initialize the subhandle write
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_subhandle_write_initialize(
     libewf_internal_handle_write_t **subhandle_write )
{
	static char *function = "libewf_internal_handle_subhandle_write_alloc";

	if( subhandle_write == NULL )
	{
		notify_warning_printf( "%s: invalid subhandle write.\n",
		 function );

		return( -1 );
	}
	if( *subhandle_write == NULL )
	{
		*subhandle_write = (libewf_internal_handle_write_t *) memory_allocate(
		                                                       sizeof( libewf_internal_handle_write_t ) );

		if( subhandle_write == NULL )
		{
			notify_warning_printf( "%s: unable to create subhandle write.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *subhandle_write,
		     0,
		     sizeof( libewf_internal_handle_write_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear subhandle write.\n",
			 function );

			memory_free(
			 *subhandle_write );

			*subhandle_write = NULL;

			return( -1 );
		}
		( *subhandle_write )->maximum_segment_file_size        = INT32_MAX;
		( *subhandle_write )->segment_file_size                = LIBEWF_DEFAULT_SEGMENT_FILE_SIZE;
		( *subhandle_write )->remaining_segment_file_size      = LIBEWF_DEFAULT_SEGMENT_FILE_SIZE;
		( *subhandle_write )->delta_segment_file_size          = INT64_MAX;
		( *subhandle_write )->maximum_section_amount_of_chunks = EWF_MAXIMUM_OFFSETS_IN_TABLE;
	}
	return( 1 );
}

/* Frees the subhandle write including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_subhandle_write_free(
     libewf_internal_handle_write_t **subhandle_write )
{
	static char *function = "libewf_internal_handle_subhandle_write_free";

	if( subhandle_write == NULL )
	{
		notify_warning_printf( "%s: invalid subhandle write.\n",
		 function );

		return( 1 );
	}
	if( *subhandle_write != NULL )
	{
		if( ( *subhandle_write )->data_section != NULL )
		{
			memory_free(
			 ( *subhandle_write )->data_section );
		}
		if( ( *subhandle_write )->table_offsets != NULL )
		{
			memory_free(
			 ( *subhandle_write )->table_offsets );
		}
		memory_free(
		 *subhandle_write );

		*subhandle_write = NULL;
	}
	return( 1 );
}

/* Retrieves the maximum amount of supported segment files to write
 * Returns 1 if successful or -1 on error
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
 * Returns 1 on success or -1 on error
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( internal_handle->header_values != NULL )
	{
		notify_warning_printf( "%s: header values already created - cleaning up previous header values.\n",
		 function );
	}
#endif
	if( libewf_values_table_free(
	     &( internal_handle->header_values ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to free header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_initialize(
	     &( internal_handle->header_values ),
	     LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT ) != 1 )
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
 * Returns 1 if successful or -1 on error
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


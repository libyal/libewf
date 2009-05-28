/*
 * libewf main handle
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libewf_definitions.h"
#include "libewf_handle.h"
#include "libewf_header_values.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_notify.h"
#include "libewf_segment_file.h"
#include "libewf_segment_file_handle.h"
#include "libewf_string.h"
#include "libewf_types.h"
#include "libewf_write_io_handle.h"

#include "ewf_crc.h"
#include "ewf_data.h"
#include "ewf_definitions.h"
#include "ewf_file_header.h"

/* Initialize the handle
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_initialize(
     libewf_handle_t **handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_initialize";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle == NULL )
	{
		internal_handle = (libewf_internal_handle_t *) memory_allocate(
		                                                sizeof( libewf_internal_handle_t ) );

		if( internal_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create internal handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_handle,
		     0,
		     sizeof( libewf_internal_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear internal handle.",
			 function );

			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_io_handle_initialize(
		     &( internal_handle->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create io handle.",
			 function );

			memory_free(
			 internal_handle );

			return( -1 );
		}
		internal_handle->date_format = LIBEWF_DATE_FORMAT_CTIME;

		/* The segment table is initially filled with a single entry
		 */
		if( libewf_segment_table_initialize(
		     &( internal_handle->segment_table ),
		     1,
		     LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create segment table.",
			 function );

			memory_free(
			 internal_handle );

			return( -1 );
		}
		/* The delta segment table is initially filled with a single entry
		 */
		if( libewf_segment_table_initialize(
		     &( internal_handle->delta_segment_table ),
		     1,
		     INT64_MAX,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create delta segment table.",
			 function );

			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_offset_table_initialize(
		     &( internal_handle->offset_table ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create offset table.",
			 function );

			libewf_segment_table_free(
			 &( internal_handle->delta_segment_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_chunk_cache_initialize(
		     &( internal_handle->chunk_cache ),
		     EWF_MINIMUM_CHUNK_SIZE + sizeof( ewf_crc_t ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create chunk cache.",
			 function );

			libewf_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->delta_segment_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_media_values_initialize(
		     &( internal_handle->media_values ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create media values.",
			 function );

			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ),
			 NULL );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->delta_segment_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_header_sections_initialize(
		     &( internal_handle->header_sections ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header sections.",
			 function );

			libewf_media_values_free(
			 &( internal_handle->media_values ),
			 NULL );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ),
			 NULL );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->delta_segment_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_hash_sections_initialize(
		     &( internal_handle->hash_sections ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hash sections.",
			 function );

			libewf_header_sections_free(
			 &( internal_handle->header_sections ),
			 NULL );
			libewf_media_values_free(
			 &( internal_handle->media_values ),
			 NULL );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ),
			 NULL );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->delta_segment_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_sector_table_initialize(
		     &( internal_handle->sessions ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sessions.",
			 function );

			libewf_hash_sections_free(
			 &( internal_handle->hash_sections ),
			 NULL );
			libewf_header_sections_free(
			 &( internal_handle->header_sections ),
			 NULL );
			libewf_media_values_free(
			 &( internal_handle->media_values ),
			 NULL );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ),
			 NULL );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->delta_segment_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		if( libewf_sector_table_initialize(
		     &( internal_handle->acquiry_errors ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create acquiry errors.",
			 function );

			libewf_sector_table_free(
			 &( internal_handle->sessions ),
			 NULL );
			libewf_hash_sections_free(
			 &( internal_handle->hash_sections ),
			 NULL );
			libewf_header_sections_free(
			 &( internal_handle->header_sections ),
			 NULL );
			libewf_media_values_free(
			 &( internal_handle->media_values ),
			 NULL );
			libewf_chunk_cache_free(
			 &( internal_handle->chunk_cache ),
			 NULL );
			libewf_offset_table_free(
			 &( internal_handle->offset_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->delta_segment_table ),
			 NULL );
			libewf_segment_table_free(
			 &( internal_handle->segment_table ),
			 NULL );
			memory_free(
			 internal_handle );

			return( -1 );
		}
		*handle = (libewf_handle_t *) internal_handle;
	}
	return( 1 );
}

/* Frees the handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_free(
     libewf_handle_t **handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_internal_handle_free";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libewf_internal_handle_t *) *handle;

		if( libewf_io_handle_free(
		     &( internal_handle->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free io handle.",
			 function );
		}
		if( libewf_read_io_handle_free(
		     &( internal_handle->read_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read io handle.",
			 function );
		}
		if( libewf_write_io_handle_free(
		     &( internal_handle->write_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free write io handle.",
			 function );
		}
		if( libewf_segment_table_free(
		     &( internal_handle->segment_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment table.",
			 function );
		}
		if( libewf_segment_table_free(
		     &( internal_handle->delta_segment_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free delta segment table.",
			 function );
		}
		if( libewf_offset_table_free(
		     &( internal_handle->offset_table ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free offset table.",
			 function );
		}
		if( libewf_chunk_cache_free(
		     &( internal_handle->chunk_cache ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk cache.",
			 function );
		}
		if( libewf_media_values_free(
		     &( internal_handle->media_values ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free media values.",
			 function );
		}
		if( libewf_header_sections_free(
		     &( internal_handle->header_sections ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header sections.",
			 function );
		}
		if( libewf_hash_sections_free(
		     &( internal_handle->hash_sections ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free hash sections.",
			 function );
		}
		if( libewf_values_table_free(
		     &( internal_handle->header_values ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header values.",
			 function );
		}
		if( libewf_values_table_free(
		     &( internal_handle->hash_values ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free hash values.",
			 function );
		}
		if( libewf_sector_table_free(
		     &( internal_handle->sessions ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sessions.",
			 function );
		}
		if( libewf_sector_table_free(
		     &( internal_handle->acquiry_errors ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free acquiry errors.",
			 function );
		}
		memory_free(
		 internal_handle );

		*handle = NULL;
	}
	return( 1 );
}

/* Signals the libewf handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_signal_abort(
     libewf_handle_t *handle,
     liberror_error_t **error )
{
	static char *function = "libewf_handle_signal_abort";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	( (libewf_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open(
     libewf_handle_t *handle,
     char * const filenames[],
     int amount_of_filenames,
     uint8_t flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbfio_pool_t *file_io_pool              = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	char *first_delta_segment_filename        = NULL;
	char *first_segment_filename              = NULL;
	static char *function                     = "libewf_handle_open";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_iterator                     = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing io handle.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of files zero or less.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     LIBBFIO_POOL_UNLIMITED_AMOUNT_OF_OPEN_HANDLES,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io pool.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	 || ( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME ) )
	{
		for( filename_iterator = 0; filename_iterator < amount_of_filenames; filename_iterator++ )
		{
			filename_length = narrow_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %s is too small.",
				 function,
				 filenames[ filename_iterator ] );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			file_io_handle = NULL;

			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file io handle.",
				 function );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                liberror_error_set(
		                 error,
		                 LIBERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file io handle.",
		                 function );

		                return( -1 );
			}
#endif
			if( libbfio_file_set_name(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file io handle.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_add_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add file io handle to pool.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'e' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'E' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'l' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'L' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 's' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'S' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_segment_filename = filenames[ filename_iterator ];

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_iterator ];
					}
				}
			}
			else if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'd' )
			      || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'D' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_delta_segment_filename = filenames[ filename_iterator ];
				}
			}
		}
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = narrow_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4 + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				return( -1 );
			}
		}
		/* Get the basename of the first delta segment file
		 */
		if( first_delta_segment_filename != NULL )
		{
			filename_length = narrow_string_length(
					   first_delta_segment_filename );

			/* Set delta segment table basename
			 */
			if( libewf_segment_table_set_basename(
			     internal_handle->delta_segment_table,
			     first_delta_segment_filename,
			     filename_length - 4 + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in delta segment table.",
				 function );

				return( -1 );
			}
		}
	}
	else if( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = narrow_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4 + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				return( -1 );
			}
		}
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = narrow_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			return( -1 );
		}
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file io pool.",
		 function );

		libbfio_pool_free(
		 &file_io_pool,
		 NULL );

		return( -1 );
	}
	internal_handle->io_handle->pool_created_in_library = 1;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_wide(
     libewf_handle_t *handle,
     wchar_t * const filenames[],
     int amount_of_filenames,
     uint8_t flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbfio_pool_t *file_io_pool              = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	wchar_t *first_delta_segment_filename     = NULL;
	wchar_t *first_segment_filename           = NULL;
	static char *function                     = "libewf_handle_open_wide";
	size_t filename_length                    = 0;
	int file_io_pool_entry                    = 0;
	int filename_iterator                     = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing io handle.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of files zero or less.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     LIBBFIO_POOL_UNLIMITED_AMOUNT_OF_OPEN_HANDLES,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io pool.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	 || ( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME ) )
	{
		for( filename_iterator = 0; filename_iterator < amount_of_filenames; filename_iterator++ )
		{
			filename_length = wide_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %ls is too small.",
				 function,
				 filenames[ filename_iterator ] );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			file_io_handle = NULL;

			if( libbfio_file_initialize(
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create file io handle.",
				 function );

				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbfio_handle_set_track_offsets_read(
			     file_io_handle,
			     1,
			     error ) != 1 )
			{
		                liberror_error_set(
		                 error,
		                 LIBERROR_ERROR_DOMAIN_RUNTIME,
		                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		                 "%s: unable to set track offsets read in file io handle.",
		                 function );

		                return( -1 );
			}
#endif
			if( libbfio_file_set_name_wide(
			     file_io_handle,
			     filenames[ filename_iterator ],
			     filename_length + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name in file io handle.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( libbfio_pool_add_handle(
			     file_io_pool,
			     &file_io_pool_entry,
			     file_io_handle,
			     LIBBFIO_OPEN_READ,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add file io handle to pool.",
				 function );

				libbfio_handle_free(
				 &file_io_handle,
				 NULL );
				libbfio_pool_free(
				 &file_io_pool,
				 NULL );

				return( -1 );
			}
			if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'e' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'E' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'l' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'L' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 's' )
			 || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'S' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_segment_filename = filenames[ filename_iterator ];

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_iterator ];
					}
				}
			}
			else if( ( filenames[ filename_iterator ][ filename_length - 3 ] == 'd' )
			      || ( filenames[ filename_iterator ][ filename_length - 3 ] == 'D' ) )
			{
				if( ( filenames[ filename_iterator ][ filename_length - 2 ] == '0' )
				 && ( filenames[ filename_iterator ][ filename_length - 1 ] == '1' ) )
				{
					first_delta_segment_filename = filenames[ filename_iterator ];
				}
			}
		}
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = wide_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename_wide(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4 + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				return( -1 );
			}
		}
		/* Get the basename of the first delta segment file
		 */
		if( first_delta_segment_filename != NULL )
		{
			filename_length = wide_string_length(
					   first_delta_segment_filename );

			/* Set delta segment table basename
			 */
			if( libewf_segment_table_set_basename_wide(
			     internal_handle->delta_segment_table,
			     first_delta_segment_filename,
			     filename_length - 4 + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in delta segment table.",
				 function );

				return( -1 );
			}
		}
	}
	else if( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME )
	{
		/* Get the basename of the first segment file
		 */
		if( first_segment_filename != NULL )
		{
			filename_length = wide_string_length(
					   first_segment_filename );

			/* Set segment table basename
			 */
			if( libewf_segment_table_set_basename_wide(
			     internal_handle->segment_table,
			     first_segment_filename,
			     filename_length - 4 + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set basename in segment table.",
				 function );

				return( -1 );
			}
		}
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = wide_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename_wide(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.",
			 function );

			return( -1 );
		}
	}
	if( libewf_handle_open_file_io_pool(
	     handle,
	     file_io_pool,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle using a file io pool.",
		 function );

		libbfio_pool_free(
		 &file_io_pool,
		 NULL );

		return( -1 );
	}
	internal_handle->io_handle->pool_created_in_library = 1;

	return( 1 );
}
#endif

/* Opens a set of EWF file(s) using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     uint8_t flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_open_file_io_pool";
	uint16_t segment_number                   = 0;
	int amount_of_handles                     = 0;
	int file_io_handle_iterator               = 0;
	int result                                = 0;
	uint8_t segment_file_type                 = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing io handle.",
		 function );

		return( -1 );
	}
	if( file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io pool.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_get_amount_of_handles(
	     file_io_pool,
	     &amount_of_handles,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of handles in the file io pool.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) != LIBEWF_FLAG_READ )
	 && ( ( flags & LIBEWF_FLAG_WRITE ) != LIBEWF_FLAG_WRITE )
	 && ( ( flags & LIBEWF_FLAG_RESUME ) != LIBEWF_FLAG_RESUME ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	 || ( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME ) )
	{
		if( libewf_read_io_handle_initialize(
		     &( internal_handle->read_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create read io handle.",
			 function );

			return( -1 );
		}
	}
	if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		if( libewf_write_io_handle_initialize(
		     &( internal_handle->write_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create write io handle.",
			 function );

			return( -1 );
		}
	}
	internal_handle->io_handle->flags        = flags;
	internal_handle->io_handle->file_io_pool = file_io_pool;

	if( ( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	 || ( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME ) )
	{
		for( file_io_handle_iterator = 0; file_io_handle_iterator < amount_of_handles; file_io_handle_iterator++ )
		{
			file_io_handle = NULL;

			if( libbfio_pool_get_handle(
			     file_io_pool,
			     file_io_handle_iterator,
			     &file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to retrieve file io handle from pool.",
				 function );

				internal_handle->io_handle->file_io_pool = NULL;

				return( -1 );
			}
			result = libewf_internal_handle_add_segment_file(
			          internal_handle,
			          file_io_handle_iterator,
			          flags,
			          &segment_number,
			          &segment_file_type,
			          error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add segment file.",
				 function );

				internal_handle->io_handle->file_io_pool = NULL;

				return( -1 );
			}
			else if( result == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: segment file: %" PRIu16 " already exists.",
				 function,
				 segment_number );

				return( -1 );
			}
			if( (int) segment_number > amount_of_handles )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_INPUT,
				 LIBERROR_INPUT_ERROR_INVALID_DATA,
				 "%s: invalid segment number: %" PRIu16 " - value out of range or missing segment files.",
				 function,
				 segment_number );

				internal_handle->io_handle->file_io_pool = NULL;

				return( -1 );
			}
		}
		result = libewf_segment_table_build(
		          internal_handle->segment_table,
		          internal_handle->io_handle,
		          internal_handle->header_sections,
		          internal_handle->hash_sections,
		          internal_handle->media_values,
		          internal_handle->offset_table,
		          internal_handle->sessions,
		          internal_handle->acquiry_errors,
		          &( internal_handle->abort ),
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to build segment table.",
			 function );

			if( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME )
			{
				libewf_notify_error_backtrace(
				 *error );
				liberror_error_free(
				 error );
			}
			else
			{
				internal_handle->io_handle->file_io_pool = NULL;

				return( -1 );
			}
		}
		if( ( flags & LIBEWF_FLAG_RESUME ) == 0 )
		{
			if( ( internal_handle->delta_segment_table->amount > 1 )
			 && ( libewf_segment_table_build(
			       internal_handle->delta_segment_table,
			       internal_handle->io_handle,
			       internal_handle->header_sections,
			       internal_handle->hash_sections,
			       internal_handle->media_values,
			       internal_handle->offset_table,
			       internal_handle->sessions,
			       internal_handle->acquiry_errors,
			       &( internal_handle->abort ),
			       error ) != 1 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to build delta segment table.",
				 function );

				internal_handle->io_handle->file_io_pool = NULL;

				return( -1 );
			}
		}
		/* Determine the EWF format
		 */
		if( libewf_header_sections_determine_format(
		     internal_handle->header_sections,
		     internal_handle->io_handle->ewf_format,
		     &( internal_handle->io_handle->format ),
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine format.",
			 function );

			internal_handle->io_handle->file_io_pool = NULL;

			return( -1 );
		}
		/* Calculate the media size
		 */
		internal_handle->media_values->media_size = (size64_t) internal_handle->media_values->amount_of_sectors
		                                          * (size64_t) internal_handle->media_values->bytes_per_sector;
	}
	/* Make sure format specific values are set
	 */
	if( ( internal_handle->io_handle->format != 0 )
	 && ( libewf_internal_handle_set_format(
	       internal_handle,
	       internal_handle->io_handle->format,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format.",
		 function );

		internal_handle->io_handle->file_io_pool = NULL;

		return( -1 );
	}
	if( ( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	 && ( ( flags & LIBEWF_FLAG_RESUME ) == LIBEWF_FLAG_RESUME ) )
	{
		if( ( internal_handle->write_io_handle->values_initialized == 0 )
		 && ( libewf_write_io_handle_initialize_values(
		       internal_handle->write_io_handle,
		       internal_handle->io_handle,
		       internal_handle->media_values,
		       internal_handle->segment_table,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write io handle values.",
			 function );

			return( -1 );
		}
		if( libewf_write_io_handle_initialize_resume(
		     internal_handle->write_io_handle,
		     internal_handle->io_handle,
		     internal_handle->media_values,
		     internal_handle->offset_table,
		     internal_handle->segment_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize write io handle to resume.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful or -1 on error
 */
int libewf_handle_close(
     libewf_handle_t *handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_close";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write_io_handle != NULL )
	 && ( internal_handle->write_io_handle->write_finalized == 0 )
	 && ( libewf_write_io_handle_finalize(
	       internal_handle->write_io_handle,
	       internal_handle->io_handle,
	       internal_handle->media_values,
	       internal_handle->offset_table,
	       internal_handle->segment_table,
	       &( internal_handle->header_values ),
	       internal_handle->hash_values,
	       internal_handle->header_sections,
	       internal_handle->hash_sections,
	       internal_handle->sessions,
	       internal_handle->acquiry_errors,
	       internal_handle->chunk_cache,
	       error ) == -1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to finalize write.",
		 function );

		return( -1 );
	}
	if( libbfio_pool_close_all(
	     internal_handle->io_handle->file_io_pool,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close all segment files.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Seeks a certain offset of the media data within the EWF file(s)
 * It will set the related file offset to the specific chunk offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_handle_seek_offset(
         libewf_handle_t *handle,
         off64_t offset,
         int whence,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_seek_offset";
	uint64_t chunk                            = 0;
	uint64_t chunk_offset                     = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{	
		offset += (off64_t) ( ( internal_handle->media_values->chunk_size * internal_handle->io_handle->current_chunk )
		                      + internal_handle->io_handle->current_chunk_offset );
	}
	else if( whence == SEEK_END )
	{	
		offset += (off64_t) internal_handle->media_values->media_size;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: seeking offset: %" PRIjd ".\n",
	 function,
	 offset );
#endif

	if( ( offset < 0 )
	 || ( offset > (off64_t) internal_handle->media_values->media_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid offset value out of range.",
		 function );

		return( -1 );
	}
	if( offset < (off64_t) internal_handle->media_values->media_size )
	{
		/* Determine the chunk that is requested
		 */
		chunk = offset / internal_handle->media_values->chunk_size;

		if( chunk >= (uint64_t) INT32_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( libewf_offset_table_seek_chunk_offset(
		     internal_handle->offset_table,
		     (uint32_t) chunk,
		     internal_handle->io_handle->file_io_pool,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk offset.",
			 function );

			return( -1 );
		}
		internal_handle->io_handle->current_chunk = (uint32_t) chunk;

		/* Determine the offset within the decompressed chunk that is requested
		 */
		chunk_offset = offset % internal_handle->media_values->chunk_size;

		if( chunk_offset >= (uint64_t) INT32_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk offset value exceeds maximum.",
			 function );

			return( -1 );
		}
		internal_handle->io_handle->current_chunk_offset = (uint32_t) chunk_offset;
	}
	else
	{
		internal_handle->io_handle->current_chunk        = internal_handle->offset_table->amount_of_chunk_offsets;
		internal_handle->io_handle->current_chunk_offset = 0;
	}
	return( offset );
}

/* Prepares chunk data after reading it according to the handle settings
 * This function should be used after libewf_handle_read_chunk
 * The chunk buffer size should contain the actual chunk size
 * Returns the resulting chunk size or -1 on error
 */
ssize_t libewf_handle_prepare_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *uncompressed_buffer,
         size_t *uncompressed_buffer_size,
         int8_t is_compressed,
         uint32_t chunk_crc,
         int8_t read_crc,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_prepare_read_chunk";
	ssize_t chunk_data_size                   = 0;
	uint8_t crc_mismatch                      = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( ( uncompressed_buffer == internal_handle->chunk_cache->data )
	 || ( uncompressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	chunk_data_size = libewf_read_io_handle_process_chunk(
	                   (uint8_t *) chunk_buffer,
	                   chunk_buffer_size,
	                   (uint8_t *) uncompressed_buffer,
	                   uncompressed_buffer_size,
	                   is_compressed,
	                   (ewf_crc_t) chunk_crc,
	                   read_crc,
	                   &crc_mismatch,
	                   error );

	if( chunk_data_size <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_INPUT_FAILED,
		 "%s: unable to prepare chunk data.",
		 function );

		return( -1 );
	}
	if( crc_mismatch != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_CRC_MISMATCH,
		 "%s: CRC mismatch for chunk data.",
		 function );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Reads a chunk from the curent offset into a buffer
 * size contains the size of the chunk buffer
 * The function sets the chunk crc, is compressed and read crc values
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_handle_read_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         int8_t *is_compressed,
         void *crc_buffer,
         uint32_t *chunk_crc,
         int8_t *read_crc,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_chunk";
	ssize_t read_count                        = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	read_count = libewf_read_io_handle_read_chunk(
	              internal_handle->io_handle,
	              internal_handle->offset_table,
	              internal_handle->io_handle->current_chunk,
	              (uint8_t *) chunk_buffer,
	              chunk_buffer_size,
	              is_compressed,
	              (uint8_t *) crc_buffer,
	              (ewf_crc_t *) chunk_crc,
	              read_crc,
	              error );

	if( read_count > 0 )
	{
		internal_handle->io_handle->current_chunk += 1;
	}
	else if( read_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk.",
		 function );
	}
	return( read_count );
}

/* Reads media data from the last current into a buffer
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_handle_read_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_read_buffer";
	ssize_t chunk_read_count                  = 0;
	ssize_t total_read_count                  = 0;
	size_t chunk_data_size                    = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: reading size: %" PRIzu ".\n",
	 function,
	 size );
#endif

	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents some reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media_values->chunk_size + sizeof( ewf_crc_t );

	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: reallocating chunk data size: %" PRIzu ".\n",
		 function,
		 chunk_data_size );
#endif

		if( libewf_chunk_cache_resize(
		     internal_handle->chunk_cache,
		     chunk_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk cache.",
			 function );

			return( -1 );
		}
	}
	while( size > 0 )
	{
		chunk_read_count = libewf_read_io_handle_read_chunk_data(
		                    internal_handle->read_io_handle,
		                    internal_handle->io_handle,
		                    internal_handle->media_values,
		                    internal_handle->offset_table,
		                    internal_handle->chunk_cache,
		                    internal_handle->io_handle->current_chunk,
		                    internal_handle->io_handle->current_chunk_offset,
		                    (uint8_t *) &( (uint8_t *) buffer )[ total_read_count ],
		                    size,
		                    error );

		if( chunk_read_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from chunk.",
			 function );

			return( -1 );
		}
		else if( chunk_read_count == 0 )
		{
			break;
		}
		size             -= chunk_read_count;
		total_read_count += chunk_read_count;

		internal_handle->io_handle->current_chunk_offset += (uint32_t) chunk_read_count;

		if( internal_handle->io_handle->current_chunk_offset == internal_handle->media_values->chunk_size )
		{
			internal_handle->io_handle->current_chunk_offset = 0;
			internal_handle->io_handle->current_chunk       += 1;
		}
		else if( internal_handle->io_handle->current_chunk_offset > internal_handle->media_values->chunk_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid current chunk offset.",
			 function );

			return( -1 );
		}
		if( internal_handle->abort == 1 )
		{
			break;
		}
	}
	return( total_read_count );
}

/* Reads media data from an offset into a buffer
 * Returns the amount of bytes read or -1 on error
 */
ssize_t libewf_handle_read_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "libewf_handle_read_random";
	ssize_t read_count    = 0;

	if( libewf_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libewf_handle_read_buffer(
	              handle,
	              buffer,
	              size,
	              error );

	if( read_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes a chunk in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * size contains the size of the data within the buffer while
 * data size contains the size of the actual input data
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_prepare_write_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         void *compressed_buffer,
         size_t *compressed_buffer_size,
         int8_t *is_compressed,
         uint32_t *chunk_crc,
         int8_t *write_crc,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_prepare_write_chunk";
	ssize_t chunk_data_size                   = 0;
	uint8_t chunk_exists                      = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->chunk_cache == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing chunk cache.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( ( compressed_buffer == internal_handle->chunk_cache->data )
	 || ( compressed_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( internal_handle->read_io_handle != NULL )
	{
		/* Check if chunk has already been created within a segment file
		 */
		if( ( internal_handle->io_handle->current_chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
		 || ( internal_handle->offset_table->chunk_offset == NULL )
		 || ( internal_handle->offset_table->chunk_offset[ internal_handle->io_handle->current_chunk ].segment_file_handle == NULL ) )
		{
			if( ( ( internal_handle->io_handle->flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
			 && ( ( internal_handle->io_handle->flags & LIBEWF_FLAG_RESUME ) == 0 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: chunk: %d does not exist.",
				 function,
				 ( internal_handle->io_handle->current_chunk + 1 ) );

				return( -1 );
			}
		}
		else
		{
			chunk_exists = 1;
		}
	}
	chunk_data_size = libewf_write_io_handle_process_chunk(
	                   internal_handle->chunk_cache,
	                   internal_handle->media_values,
	                   internal_handle->io_handle->compression_level,
	                   internal_handle->io_handle->compression_flags,
	                   internal_handle->io_handle->ewf_format,
	                   (uint8_t *) chunk_buffer,
	                   chunk_buffer_size,
	                   (uint8_t *) compressed_buffer,
	                   compressed_buffer_size,
	                   is_compressed,
	                   chunk_exists,
	                   (ewf_crc_t *) chunk_crc,
	                   write_crc,
	                   error );

	if( chunk_data_size <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
		 "%s: unable to prepare chunk.",
		 function );

		return( -1 );
	}
	return( chunk_data_size );
}

/* Writes a chunk in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * size contains the size of the data within the buffer while
 * data size contains the size of the actual input data
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_chunk(
         libewf_handle_t *handle,
         void *chunk_buffer,
         size_t chunk_buffer_size,
         size_t data_size,
         int8_t is_compressed,
         void *crc_buffer,
         uint32_t chunk_crc,
         int8_t write_crc,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_chunk";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write_io_handle->values_initialized == 0 )
	 && ( libewf_write_io_handle_initialize_values(
	       internal_handle->write_io_handle,
	       internal_handle->io_handle,
	       internal_handle->media_values,
	       internal_handle->segment_table,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize write io handle values.",
		 function );

		return( -1 );
	}
	if( chunk_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer.",
		 function );

		return( -1 );
	}
	if( ( chunk_buffer == internal_handle->chunk_cache->data )
	 || ( chunk_buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.",
		 function );

		return( -1 );
	}
	if( data_size > internal_handle->media_values->chunk_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: data size cannot be larger than maximum chunk size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	libewf_notify_verbose_printf(
	 "%s: writing chunk: %d of total: %d.\n",
	 function,
	 ( internal_handle->io_handle->current_chunk + 1 ),
	 internal_handle->offset_table->amount_of_chunk_offsets );
	libewf_notify_verbose_printf(
	 "%s: writing chunk buffer of size: %" PRIzd " with data of size: %" PRIzd ".\n",
	 function,
	 chunk_buffer_size,
	 data_size );
#endif

	if( ( ( internal_handle->io_handle->flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	 && ( ( internal_handle->io_handle->flags & LIBEWF_FLAG_RESUME ) == 0 ) )
	{
		/* Check if chunk has already been created within a segment file
		 */
		if( ( internal_handle->io_handle->current_chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
		 || ( internal_handle->offset_table->chunk_offset == NULL )
		 || ( internal_handle->offset_table->chunk_offset[ internal_handle->io_handle->current_chunk ].segment_file_handle == NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: chunk: %d does not exist.",
			 function,
			 internal_handle->io_handle->current_chunk + 1 );

			return( -1 );
		}
		write_count = libewf_write_io_handle_write_existing_chunk(
		               internal_handle->write_io_handle,
		               internal_handle->io_handle,
		               internal_handle->media_values,
		               internal_handle->offset_table,
		               internal_handle->delta_segment_table,
		               internal_handle->header_sections,
		               internal_handle->io_handle->current_chunk,
		               (uint8_t *) chunk_buffer,
		               chunk_buffer_size,
		               data_size,
		               is_compressed,
		               (uint8_t *) crc_buffer,
		               (ewf_crc_t) chunk_crc,
		               write_crc,
		               error );
	}
	else
	{
		write_count = libewf_write_io_handle_write_new_chunk(
		               internal_handle->write_io_handle,
		               internal_handle->io_handle,
		               internal_handle->media_values,
		               internal_handle->offset_table,
		               internal_handle->segment_table,
		               &( internal_handle->header_values ),
		               internal_handle->hash_values,
		               internal_handle->header_sections,
		               internal_handle->hash_sections,
		               internal_handle->sessions,
		               internal_handle->acquiry_errors,
		               internal_handle->io_handle->current_chunk,
		               (uint8_t *) chunk_buffer,
		               chunk_buffer_size,
		               data_size,
		               is_compressed,
		               (uint8_t *) crc_buffer,
		               (ewf_crc_t) chunk_crc,
		               write_crc,
		               error );
	}
	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write raw chunk data.",
		 function );

		return( -1 );
	}
	internal_handle->io_handle->current_chunk += 1;

	return( (ssize_t) chunk_buffer_size );
}

/* Writes data in EWF format from a buffer at the current offset
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_buffer(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_buffer";
	ssize_t total_write_count                 = 0;
	ssize_t write_count                       = 0;
	size_t chunk_data_size                    = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing subhandle write.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write_io_handle->values_initialized == 0 )
	 && ( libewf_write_io_handle_initialize_values(
	       internal_handle->write_io_handle,
	       internal_handle->io_handle,
	       internal_handle->media_values,
	       internal_handle->segment_table,
	       error ) != 1 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize write io handle values.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( ( buffer == internal_handle->chunk_cache->data )
	 || ( buffer == internal_handle->chunk_cache->compressed ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer - same as chunk cache.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Reallocate the chunk cache if the chunk size is not the default chunk size
	 * this prevents multiple reallocations of the chunk cache
	 */
	chunk_data_size = internal_handle->media_values->chunk_size + sizeof( ewf_crc_t );

	if( buffer == internal_handle->chunk_cache->compressed )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: chunk cache compressed cannot be used as buffer.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > internal_handle->chunk_cache->allocated_size )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		libewf_notify_verbose_printf(
		 "%s: reallocating chunk data size: %" PRIzu ".\n",
		 function,
		 chunk_data_size );
#endif

		if( libewf_chunk_cache_resize(
		     internal_handle->chunk_cache,
		     chunk_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize chunk cache.",
			 function );

			return( -1 );
		}
	}
	while( size > 0 )
	{
		if( ( ( internal_handle->io_handle->flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
		 && ( ( internal_handle->io_handle->flags & LIBEWF_FLAG_RESUME ) == 0 ) )
		{
			/* Check if chunk has already been created within a segment file
			 */
			if( ( internal_handle->io_handle->current_chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
			 || ( internal_handle->offset_table->chunk_offset == NULL )
			 || ( internal_handle->offset_table->chunk_offset[ internal_handle->io_handle->current_chunk ].segment_file_handle == NULL ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: chunk: %d does not exist.",
				 function,
				 ( internal_handle->io_handle->current_chunk + 1 ) );

				return( -1 );
			}
			write_count = libewf_write_io_handle_write_existing_chunk_data(
			               internal_handle->write_io_handle,
			               internal_handle->read_io_handle,
			               internal_handle->io_handle,
			               internal_handle->media_values,
			               internal_handle->offset_table,
			               internal_handle->delta_segment_table,
			               internal_handle->header_sections,
			               internal_handle->chunk_cache,
			               internal_handle->io_handle->current_chunk,
			               internal_handle->io_handle->current_chunk_offset,
			               (void *) &( (uint8_t *) buffer )[ total_write_count ],
			               size,
			               size,
			               error );
		}
		else
		{
			write_count = libewf_write_io_handle_write_new_chunk_data(
			               internal_handle->write_io_handle,
			               internal_handle->io_handle,
			               internal_handle->media_values,
			               internal_handle->offset_table,
			               internal_handle->segment_table,
			               &( internal_handle->header_values ),
			               internal_handle->hash_values,
			               internal_handle->header_sections,
			               internal_handle->hash_sections,
			               internal_handle->sessions,
			               internal_handle->acquiry_errors,
			               internal_handle->chunk_cache,
			               internal_handle->io_handle->current_chunk,
			               (void *) &( (uint8_t *) buffer )[ total_write_count ],
			               size,
			               size,
			               0,
			               error );
		}
		if( write_count <= -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write data from buffer.",
			 function );

			return( -1 );
		}
		else if( write_count == 0 )
		{
			break;
		}
		total_write_count += write_count;
		size              -= write_count;

		internal_handle->io_handle->current_chunk_offset += (uint32_t) write_count;

		if( internal_handle->io_handle->current_chunk_offset == internal_handle->media_values->chunk_size )
		{
			internal_handle->io_handle->current_chunk_offset = 0;
			internal_handle->io_handle->current_chunk       += 1;
		}
		else if( internal_handle->io_handle->current_chunk_offset > internal_handle->media_values->chunk_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: invalid current chunk offset: %" PRIu32 " larger than chunk size: %" PRIu32 ".",
			 function,
			 internal_handle->io_handle->current_chunk_offset,
			 internal_handle->media_values->chunk_size );

			return( -1 );
		}
		if( internal_handle->abort == 1 )
		{
			break;
		}
	}
	return( total_write_count );
}

/* Writes data in EWF format from a buffer at an specific offset,
 * the necessary settings of the write values must have been made
 * Will initialize write if necessary
 * Returns the amount of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libewf_handle_write_random(
         libewf_handle_t *handle,
         void *buffer,
         size_t size,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "libewf_handle_write_random";
	ssize_t write_count   = 0;

	if( libewf_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libewf_handle_write_buffer(
	               handle,
	               buffer,
	               size,
	               error );

	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Finalizes the write by correcting the EWF the meta data in the segment files
 * This function is required after write from stream
 * Returns the amount of bytes written or -1 on error
 */
ssize_t libewf_handle_write_finalize(
         libewf_handle_t *handle,
         liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_write_finalize";
	ssize_t write_count                       = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	write_count = libewf_write_io_handle_finalize(
	               internal_handle->write_io_handle,
	               internal_handle->io_handle,
	               internal_handle->media_values,
	               internal_handle->offset_table,
	               internal_handle->segment_table,
	               &( internal_handle->header_values ),
	               internal_handle->hash_values,
	               internal_handle->header_sections,
	               internal_handle->hash_sections,
	               internal_handle->sessions,
	               internal_handle->acquiry_errors,
	               internal_handle->chunk_cache,
	               error );

	if( write_count <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write finalize.",
		 function );

		return( -1 );
	}
	return( write_count );
}

/* Retrieves the current offset of the media data within the EWF file(s)
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_offset(
     libewf_handle_t *handle,
     off64_t *offset,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_offset";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = ( internal_handle->io_handle->current_chunk * internal_handle->media_values->chunk_size )
	        + internal_handle->io_handle->current_chunk_offset;

	return( 1 );
}

/* Retrieves the segment filename size
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_size";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size(
	          internal_handle->segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename(
	          internal_handle->segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( filename[ filename_length - 1 ] != 0 )
	{
		filename_length += 1;
	}
	if( libewf_segment_table_set_basename(
	     internal_handle->segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_size_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size_wide(
	          internal_handle->segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_filename_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_wide(
	          internal_handle->segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_filename_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( filename[ filename_length - 1 ] != 0 )
	{
		filename_length += 1;
	}
	if( libewf_segment_table_set_basename_wide(
	     internal_handle->segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_segment_file_size(
     libewf_handle_t *handle,
     size64_t *segment_file_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_segment_file_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( segment_file_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file size.",
		 function );

		return( -1 );
	}
	*segment_file_size = internal_handle->segment_table->maximum_segment_size;

	return( 1 );
}

/* Sets the segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_segment_file_size(
     libewf_handle_t *handle,
     size64_t segment_file_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_segment_file_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing segment table.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read_io_handle != NULL )
	 || ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: segment file size cannot be changed.",
		 function );

		return( -1 );
	}
	if( segment_file_size > (size64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment file size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( segment_file_size == 0 )
	 || ( segment_file_size > internal_handle->write_io_handle->maximum_segment_file_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid segment file size value out of range.",
		 function );

		return( -1 );
	}
	internal_handle->segment_table->maximum_segment_size = segment_file_size;

	return( 1 );
}

/* Retrieves the delta segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_size";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size(
	          internal_handle->delta_segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename(
	          internal_handle->delta_segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the delta segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_delta_segment_filename(
     libewf_handle_t *handle,
     const char *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_delta_segment_filename";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: delta segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( filename[ filename_length - 1 ] != 0 )
	{
		filename_length += 1;
	}
	if( libewf_segment_table_set_basename(
	     internal_handle->delta_segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the delta segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_size_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_size_wide(
	          internal_handle->delta_segment_table,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the delta segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_handle_get_delta_segment_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_filename_wide";
	int result                                = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	result = libewf_segment_table_get_basename_wide(
	          internal_handle->delta_segment_table,
	          filename,
	          filename_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment table basename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the delta segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_delta_segment_filename_wide(
     libewf_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_delta_segment_filename_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: delta segment filename cannot be changed.",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( filename[ filename_length - 1 ] != 0 )
	{
		filename_length += 1;
	}
	if( libewf_segment_table_set_basename_wide(
	     internal_handle->delta_segment_table,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set segment table basename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t *delta_segment_file_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_get_delta_segment_file_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( delta_segment_file_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid delta segment file size.",
		 function );

		return( -1 );
	}
	*delta_segment_file_size = internal_handle->delta_segment_table->maximum_segment_size;

	return( 1 );
}

/* Sets the delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_set_delta_segment_file_size(
     libewf_handle_t *handle,
     size64_t delta_segment_file_size,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_handle_set_delta_segment_file_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing delta segment table.",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write_io_handle == NULL )
	 || ( internal_handle->write_io_handle->values_initialized != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: delta segment file size cannot be changed.",
		 function );

		return( -1 );
	}
	if( delta_segment_file_size > (size64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid delta segment file size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( delta_segment_file_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid delta segment file size value out of range.",
		 function );

		return( -1 );
	}
	internal_handle->delta_segment_table->maximum_segment_size = delta_segment_file_size;

	return( 1 );
}

/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename_size(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_size";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file io handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename(
     libewf_handle_t *handle,
     char *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file io handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the (delta) segment file of the current chunk
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename_size_wide(
     libewf_handle_t *handle,
     size_t *filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_size_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file io handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_size_wide(
	     file_io_handle,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the filename of the (delta) segment file of the current chunk
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_filename_wide(
     libewf_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_handle_get_filename_wide";

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_file_io_handle(
	     handle,
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file io handle for current chunk.",
		 function );

		return( -1 );
	}
	if( libbfio_file_get_name_wide(
	     file_io_handle,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve filename.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

/* Retrieves the file io handle of the (delta) segment file of the current chunk
 * Returns 1 if successful or -1 on error
 */
int libewf_handle_get_file_io_handle(
     libewf_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     liberror_error_t **error )
{
	libewf_internal_handle_t *internal_handle         = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_handle_get_file_io_handle";
	int file_io_pool_entry                            = 0;

	if( handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->file_io_pool == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid IO handle - missing file IO pool.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing offset table.",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table->chunk_offset == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - invalid offset table - missing chunk offset.",
		 function );

		return( -1 );
	}
	if( internal_handle->io_handle->current_chunk >= internal_handle->offset_table->amount_of_chunk_offsets )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid current chunk value out of range.",
		 function );

		return( -1 );
	}
	segment_file_handle = internal_handle->offset_table->chunk_offset[ internal_handle->io_handle->current_chunk ].segment_file_handle;

	if( segment_file_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid segment file handle for chunk: %" PRIu32 ".",
		 function,
		 internal_handle->io_handle->current_chunk + 1 );

		return( -1 );
	}
	file_io_pool_entry = segment_file_handle->file_io_pool_entry;

	if( libbfio_pool_get_handle(
	     internal_handle->io_handle->file_io_pool,
	     file_io_pool_entry,
	     file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file io handle for pool entry: %d (chunk: %" PRIu32 ").",
		 function,
		 file_io_pool_entry,
		 internal_handle->io_handle->current_chunk + 1 );

		return( -1 );
	}
	return( 1 );
}

/* Appends a segment file
 * Returns 1 if successful, 0 if the segment file already exists or -1 on error
 */
int libewf_internal_handle_add_segment_file(
     libewf_internal_handle_t *internal_handle,
     int file_io_pool_entry,
     int flags,
     uint16_t *segment_number,
     uint8_t *segment_file_type,
     liberror_error_t **error )
{
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_internal_handle_add_segment_file";

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( file_io_pool_entry < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid file io pool entry value is less than zero.",
		 function );

		return( -1 );
	}
	if( segment_number == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment number.",
		 function );

		return( -1 );
	}
	if( segment_file_type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment file type.",
		 function );

		return( -1 );
	}
	if( libewf_segment_file_handle_initialize(
	     &segment_file_handle,
	     file_io_pool_entry,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file handle.",
		 function );

		return( -1 );
	}
	if( libewf_segment_file_read_file_header(
	     segment_file_handle,
	     segment_number,
	     internal_handle->io_handle->file_io_pool,
	     error ) <= -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment file header.",
		 function );

		libewf_segment_file_handle_free(
		 &segment_file_handle,
		 NULL );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_INVALID_DATA,
		 "%s: invalid segment number: 0.",
		 function );

		libewf_segment_file_handle_free(
		 &segment_file_handle,
		 NULL );

		return( -1 );
	}
	if( ( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	 || ( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF ) )
	{
		if( ( *segment_number >= internal_handle->segment_table->amount )
		 && ( libewf_segment_table_resize(
		       internal_handle->segment_table,
		       *segment_number + 1,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize the segment table.",
			 function );

			libewf_segment_file_handle_free(
			 &segment_file_handle,
			 NULL );

			return( -1 );
		}
		if( internal_handle->segment_table->segment_file_handle[ *segment_number ] != NULL )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: segment file: %" PRIu16 " already exists.\n",
			 function,
			 *segment_number );
#endif

			libewf_segment_file_handle_free(
			 &segment_file_handle,
			 NULL );

			return( 0 );
		}
		internal_handle->segment_table->segment_file_handle[ *segment_number ] = segment_file_handle;
	}
	else if( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		if( ( *segment_number >= internal_handle->delta_segment_table->amount )
		 && ( libewf_segment_table_resize(
		       internal_handle->delta_segment_table,
		       *segment_number + 1,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize the delta segment table.",
			 function );

			return( -1 );
		}
		if( internal_handle->delta_segment_table->segment_file_handle[ *segment_number ] != NULL )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libewf_notify_verbose_printf(
			 "%s: delta segment file: %" PRIu16 " already exists.\n",
			 function,
			 *segment_number );
#endif

			libewf_segment_file_handle_free(
			 &segment_file_handle,
			 NULL );

			return( 0 );
		}
		internal_handle->delta_segment_table->segment_file_handle[ *segment_number ] = segment_file_handle;

		/* Re-open the delta segment file with write access
		 */
		if( ( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
		 && ( libbfio_pool_reopen(
		       internal_handle->io_handle->file_io_pool,
		       file_io_pool_entry,
		       LIBBFIO_OPEN_READ_WRITE,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to reopen delta segment file: %" PRIu16 ".",
			 function,
			 *segment_number );

			return( -1 );
		}
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		libewf_segment_file_handle_free(
		 &segment_file_handle,
		 NULL );

		return( -1 );
	}
	*segment_file_type = segment_file_handle->file_type;

	return( 1 );
}

/* Retrieves the maximum amount of supported segment files to write
 * Returns 1 if successful or -1 on error
 */
int libewf_internal_handle_get_write_maximum_amount_of_segments(
     uint8_t ewf_format,
     uint16_t *maximum_amount_of_segments,
     liberror_error_t **error )
{
	static char *function = "libewf_internal_handle_get_write_maximum_amount_of_segments";

	if( maximum_amount_of_segments == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid maximum amount of segments.",
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported EWF format.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Set the media values
 * Returns 1 if successful or -1 on errror
 */
int libewf_internal_handle_set_media_values(
     libewf_internal_handle_t *internal_handle,
     uint32_t sectors_per_chunk,
     uint32_t bytes_per_sector,
     size64_t media_size,
     liberror_error_t **error )
{
	static char *function            = "libewf_internal_handle_set_media_values";
	size32_t chunk_size              = 0;
	size64_t maximum_input_file_size = 0;
	uint64_t amount_of_sectors       = 0;
	int64_t amount_of_chunks         = 0;

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.",
		 function );

		return( -1 );
	}
	if( ( sectors_per_chunk == 0 )
	 || ( sectors_per_chunk > (uint32_t) INT32_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid sectors per chunk.",
		 function );

		return( -1 );
	}
	if( ( bytes_per_sector == 0 )
	 || ( bytes_per_sector > (uint32_t) INT32_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid media size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Determine the chunk size
	 */
	chunk_size = sectors_per_chunk * bytes_per_sector;

	if( ( chunk_size == 0 )
	 || ( chunk_size > (size32_t) INT32_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid chunk size.",
		 function );

		return( -1 );
	}
	/* Check if the input file size does not exceed the maximum possible input file size
	 * for the chunk size
	 */
	maximum_input_file_size = (size64_t) chunk_size * (size64_t) UINT32_MAX;

	if( media_size > maximum_input_file_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: media size cannot be larger than size: %" PRIu64 " with a chunk size of: %" PRIu32 ".",
		 function,
		 maximum_input_file_size,
		 chunk_size );

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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid amount of chunks value exceeds maximum.",
			 function );

			return( -1 );
		}
		internal_handle->media_values->amount_of_chunks = (uint32_t) amount_of_chunks;

		/* Determine the amount of sectors to write
		 */
		amount_of_sectors = (uint64_t) media_size / (uint64_t) bytes_per_sector;

		if( amount_of_sectors > (uint64_t) INT64_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid amount of sectors value exceeds maximum.",
			 function );

			return( -1 );
		}
		internal_handle->media_values->amount_of_sectors = (uint64_t) amount_of_sectors;
	}
	return( 1 );
}

/* Sets internal values based on the EWF file format
 * Returns 1 if successful or -1 on errror
 */
int libewf_internal_handle_set_format(
     libewf_internal_handle_t *internal_handle,
     uint8_t format,
     liberror_error_t **error )
{
	static char *function = "libewf_internal_handle_set_format";

	if( internal_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK )
	 && ( format != LIBEWF_FORMAT_LVF )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format: %d.",
		 function,
	         format );

		return( -1 );
	}
	internal_handle->io_handle->format = format;

	if( ( format == LIBEWF_FORMAT_EWF )
	 || ( format == LIBEWF_FORMAT_SMART ) )
	{
		internal_handle->io_handle->ewf_format = EWF_FORMAT_S01;
	}
	else if( format == LIBEWF_FORMAT_LVF )
	{
		internal_handle->io_handle->ewf_format = EWF_FORMAT_L01;
	}
	else
	{
		internal_handle->io_handle->ewf_format = EWF_FORMAT_E01;
	}
	if( internal_handle->write_io_handle != NULL )
	{
		if( format == LIBEWF_FORMAT_ENCASE6 )
		{
			internal_handle->write_io_handle->maximum_segment_file_size        = INT64_MAX;
			internal_handle->write_io_handle->maximum_section_amount_of_chunks = EWF_MAXIMUM_OFFSETS_IN_TABLE_ENCASE6;
		}
		else if( format == LIBEWF_FORMAT_EWFX )
		{
			internal_handle->write_io_handle->unrestrict_offset_amount         = 1;
			internal_handle->write_io_handle->maximum_segment_file_size        = INT32_MAX;
			internal_handle->write_io_handle->maximum_section_amount_of_chunks = INT32_MAX;
		}
		else
		{
			internal_handle->write_io_handle->maximum_segment_file_size        = INT32_MAX;
			internal_handle->write_io_handle->maximum_section_amount_of_chunks = EWF_MAXIMUM_OFFSETS_IN_TABLE;
		}
		/* Determine the maximum amount of segments allowed to write
		 */
		if( libewf_internal_handle_get_write_maximum_amount_of_segments(
		     internal_handle->io_handle->ewf_format,
		     &( internal_handle->write_io_handle->maximum_amount_of_segments ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the maximum amount of allowed segment files.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}


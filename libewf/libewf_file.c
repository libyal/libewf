/*
 * libewf file handling
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
#include <types.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <libewf/definitions.h>

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_file.h"
#include "libewf_filename.h"
#include "libewf_hash_values.h"
#include "libewf_header_values.h"
#include "libewf_offset_table.h"
#include "libewf_read.h"
#include "libewf_section_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_file_handle.h"
#include "libewf_segment_table.h"
#include "libewf_string.h"
#include "libewf_write.h"

#include "ewf_crc.h"
#include "ewf_definitions.h"
#include "ewf_digest_hash.h"
#include "ewf_file_header.h"
#include "ewf_section.h"
#include "ewf_volume.h"
#include "ewf_table.h"

/* Returns the library version
 */
const character_t *libewf_get_version(
                      void )
{
	return( (const character_t *) LIBEWF_VERSION_STRING );
}

/* Returns the flags for reading
 */
uint8_t libewf_get_flags_read(
         void )
{
	return( (uint8_t) LIBEWF_FLAG_READ );
}

/* Returns the flags for reading and writing
 */
uint8_t libewf_get_flags_read_write(
         void )
{
	return( (uint8_t) ( LIBEWF_FLAG_READ | LIBEWF_FLAG_WRITE ) );
}

/* Returns the flags for writing
 */
uint8_t libewf_get_flags_write(
         void )
{
	return( (uint8_t) LIBEWF_FLAG_WRITE );
}

/* Detects if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
int libewf_check_file_signature(
     const libewf_filename_t *filename )
{
	static char *function = "libewf_check_file_signature";
	int file_descriptor   = 0;
	int result            = 0;

	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	file_descriptor = libewf_filename_open(
	                   filename,
	                   LIBEWF_OPEN_READ );

	if( file_descriptor < 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to open file: %" PRIs_EWF_filename ".\n",
		 function, filename );

		return( -1 );
	}
	result = libewf_segment_file_check_file_signature(
	          file_descriptor );

	if( libewf_common_close(
	     file_descriptor ) != 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to close file: %" PRIs_EWF_filename ".\n",
		 function, filename );

		return( -1 );
	}
	if( result <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to read signature from file: %" PRIs_EWF_filename ".\n",
		 function, filename );

		return( -1 );
	}
	return( result );
}

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns the amount of filenames if successful or -1 on error
 */
int libewf_glob(
     const libewf_filename_t *filename,
     size_t length,
     uint8_t format,
     libewf_filename_t **filenames[] )
{
	libewf_filename_t *segment_filename = NULL;
	void *reallocation                  = NULL;
	static char *function               = "libewf_glob";
	size_t additional_length            = 4;
	int amount_of_files                 = 0;
	int file_descriptor                 = 0;
	uint8_t segment_file_type           = 0;
	uint8_t ewf_format                  = 0;

	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( ( length == 0 )
	 || ( length > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename length.\n",
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
		format = LIBEWF_FORMAT_UNKNOWN;
	}
	if( filenames == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filenames.\n",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( filename[ length - 4 ] != (libewf_filename_t) '.' )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid filename - missing extension.\n",
			 function );

			return( -1 );
		}
		additional_length = 0;

		if( filename[ length - 3 ] != (libewf_filename_t) 'E' )
		{
			format = LIBEWF_FORMAT_ENCASE5;
		}
		else if( filename[ length - 3 ] != (libewf_filename_t) 'e' )
		{
			format = LIBEWF_FORMAT_EWF;
		}
		else if( filename[ length - 3 ] != (libewf_filename_t) 'L' )
		{
			format = LIBEWF_FORMAT_LVF;
		}
		else if( filename[ length - 3 ] != (libewf_filename_t) 's' )
		{
			format = LIBEWF_FORMAT_SMART;
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: invalid filename - unsupported extension: .\n",
			 function, &( filename[ length - 4 ] ) );

			return( -1 );
		}
	}
	if( format == LIBEWF_FORMAT_LVF )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
		ewf_format        = EWF_FORMAT_L01;
	}
	else if( format == LIBEWF_FORMAT_SMART )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_S01;
	}
	else
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_E01;
	}
	while( amount_of_files < (int) UINT16_MAX )
	{
		segment_filename = memory_allocate(
			            sizeof( libewf_filename_t ) * ( length + additional_length + 1 ) );

		if( segment_filename == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create segment filename.\n",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     segment_filename,
		     filename,
		     length ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to copy filename.\n",
			 function );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		if( additional_length > 0 )
		{
			segment_filename[ length ] = (character_t) '.';
		}
		if( libewf_filename_set_extension(
		     &( segment_filename[ length + additional_length - 3 ] ),
		     ( amount_of_files + 1 ),
		     UINT16_MAX,
		     segment_file_type,
		     format,
		     ewf_format ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set extension.\n",
			 function );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		segment_filename[ length + additional_length ] = (character_t) '\0';

		file_descriptor = libewf_common_open(
		                   segment_filename,
		                   LIBEWF_OPEN_READ );

		if( file_descriptor == -1 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		amount_of_files++;

		libewf_common_close(
		 file_descriptor );

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( libewf_filename_t * ) * amount_of_files );

		if( reallocation == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate filenames.\n",
			 function );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		*filenames = (libewf_filename_t **) reallocation;

		( *filenames )[ amount_of_files - 1 ] = segment_filename;
	}
	return( amount_of_files );
}

/* Signals the libewf handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libewf_signal_abort(
     LIBEWF_HANDLE *handle )
{
	static char *function = "libewf_signal_abort";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	( (libewf_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 */
LIBEWF_HANDLE *libewf_open(
                libewf_filename_t * const filenames[],
                uint16_t amount_of_files,
                uint8_t flags )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_open";
	size64_t *segment_file_size               = NULL;

	if( filenames == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filenames.\n",
		 function );

		return( NULL );
	}
	if( amount_of_files < 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( NULL );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) != LIBEWF_FLAG_READ )
	 && ( ( flags & LIBEWF_FLAG_WRITE ) != LIBEWF_FLAG_WRITE ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported flags.\n",
		 function );

		return( NULL );
	}
	internal_handle = libewf_internal_handle_alloc( flags );

	if( internal_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create handle.\n",
		 function );

		return( NULL );
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		if( internal_handle->write != NULL )
		{
			segment_file_size = &( internal_handle->write->segment_file_size );
		}
		if( libewf_segment_table_read_open(
		     internal_handle->segment_table, 
		     internal_handle->delta_segment_table, 
		     filenames, 
		     amount_of_files,
		     flags,
		     internal_handle->header_sections,
		     internal_handle->hash_sections,
		     internal_handle->media_values,
		     internal_handle->offset_table,
		     internal_handle->secondary_offset_table,
		     internal_handle->sessions,
		     internal_handle->acquiry_errors,
		     &( internal_handle->compression_level ),
		     &( internal_handle->format ),
		     &( internal_handle->ewf_format ),
		     segment_file_size,
		     internal_handle->error_tollerance,
		     &( internal_handle->abort ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to open segment file(s).\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
		/* Determine the EWF format
		 */
		if( libewf_header_sections_determine_format(
		     internal_handle->header_sections,
		     internal_handle->ewf_format,
		     &( internal_handle->format ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine format.\n",
			 function );
		}
		/* Calculate the media size
		 */
		internal_handle->media_values->media_size = (size64_t) internal_handle->media_values->amount_of_sectors
		                                          * (size64_t) internal_handle->media_values->bytes_per_sector;
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		if( libewf_segment_table_write_open(
		     internal_handle->segment_table, 
		     filenames, 
		     amount_of_files ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to open segment file(s).\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
	}
	/* Make sure format specific values are set
	 */
	if( libewf_internal_handle_initialize_format(
	     internal_handle ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize format specific values.\n",
		 function );
	}
	LIBEWF_VERBOSE_PRINT( "%s: open successful.\n",
	 function );

	return( (LIBEWF_HANDLE *) internal_handle );
}

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful or -1 on error
 */
int libewf_close(
     LIBEWF_HANDLE *handle )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_close";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->write_finalized == 0 ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: finalizing write.\n",
		 function );

		libewf_write_finalize( handle );
	}
	if( libewf_segment_table_close_all(
	     internal_handle->segment_table ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to close all segment files in segment table.\n",
		 function );

		return( -1 );
	}
	if( libewf_segment_table_close_all(
	     internal_handle->delta_segment_table ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to close all segment files in delta segment table.\n",
		 function );

		return( -1 );
	}
	libewf_internal_handle_free( internal_handle );

	return( 0 );
}

/* Seeks a certain offset of the media data within the EWF file(s)
 * It will set the related file offset to the specific chunk offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_seek_offset(
     LIBEWF_HANDLE *handle,
     off64_t offset )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_seek_offset";
	uint64_t chunk                            = 0;
	uint64_t chunk_offset                     = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( offset <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset value cannot be negative.\n",
		 function );

		return( -1 );
	}
	if( offset >= (off64_t) internal_handle->media_values->media_size )
	{
		LIBEWF_WARNING_PRINT( "%s: attempting to read past the end of the file.\n",
		 function );

		return( -1 );
	}
	/* Determine the chunk that is requested
	 */
	chunk = offset / internal_handle->media_values->chunk_size;

	if( chunk >= (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_offset_table_seek_chunk_offset(
	     internal_handle->offset_table,
	     (uint32_t) chunk ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to seek chunk offset.\n",
		 function );

		return( -1 );
	}
	internal_handle->current_chunk = (uint32_t) chunk;

	/* Determine the offset within the decompressed chunk that is requested
	 */
	chunk_offset = offset % internal_handle->media_values->chunk_size;

	if( chunk_offset >= (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	internal_handle->current_chunk_offset = (uint32_t) chunk_offset;

	return( offset );
}

/* Retrieves the amount of sectors per chunk from the media information
 * Returns 1 if successful or -1 on error
 */
int libewf_get_sectors_per_chunk(
     LIBEWF_HANDLE *handle,
     uint32_t *sectors_per_chunk )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_sectors_per_chunk";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*sectors_per_chunk = internal_handle->media_values->sectors_per_chunk;

	return( 1 );
}

/* Retrieves the amount of bytes per sector from the media information
 * Returns 1 if successful or -1 on error
 */
int libewf_get_bytes_per_sector(
     LIBEWF_HANDLE *handle,
     uint32_t *bytes_per_sector )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_bytes_per_sector";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*bytes_per_sector = internal_handle->media_values->bytes_per_sector;

	return( 1 );
}

/* Retrieves the amount of sectors from the media information
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_sectors(
     LIBEWF_HANDLE *handle,
     uint32_t *amount_of_sectors )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_amount_of_sectors";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->amount_of_sectors > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sectors value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*amount_of_sectors = internal_handle->media_values->amount_of_sectors;

	return( 1 );
}

/* Retrieves the chunk size from the media information
 * Returns 1 if successful or -1 on error
 */
int libewf_get_chunk_size(
     LIBEWF_HANDLE *handle,
     size32_t *chunk_size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_chunk_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->chunk_size > (size32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*chunk_size = internal_handle->media_values->chunk_size;

	return( 1 );
}

/* Retrieves the error granularity from the media information
 * Returns 1 if successful or -1 on error
 */
int libewf_get_error_granularity(
     LIBEWF_HANDLE *handle,
     uint32_t *error_granularity )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_error_granularity";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( error_granularity == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid error granularity.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->error_granularity > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid error granularity value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*error_granularity = internal_handle->media_values->error_granularity;

	return( 1 );
}

/* Retrieves the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_get_compression_values(
     LIBEWF_HANDLE *handle,
     int8_t *compression_level,
     uint8_t *compress_empty_block )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_compression_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( compression_level == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compression level.\n",
		 function );

		return( -1 );
	}
	if( compress_empty_block == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compress empty block.\n",
		 function );

		return( -1 );
	}
	*compression_level    = internal_handle->compression_level;
	*compress_empty_block = internal_handle->compress_empty_block;

	return( 1 );
}

/* Retrieves the size of the contained media data
 * Returns 1 if successful or -1 on error
 */
int libewf_get_media_size(
     LIBEWF_HANDLE *handle,
     size64_t *media_size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_media_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media size.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->media_size == 0 )
	{
		internal_handle->media_values->media_size = (size64_t) internal_handle->media_values->amount_of_sectors
		                                          * (size64_t) internal_handle->media_values->bytes_per_sector;
	}
	if( internal_handle->media_values->media_size > (size64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*media_size = internal_handle->media_values->media_size;

	return( 1 );
}

/* Retrieves the media type value
 * Returns 1 if successful or -1 on error
 */
int libewf_get_media_type(
     LIBEWF_HANDLE *handle,
     int8_t *media_type )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_media_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->media_type > (uint8_t) INT8_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media type value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media type.\n",
		 function );

		return( -1 );
	}
	*media_type = internal_handle->media_values->media_type;

	return( 1 );
}

/* Retrieves the media flags
 * Returns 1 if successful or -1 on error
 */
int libewf_get_media_flags(
     LIBEWF_HANDLE *handle,
     int8_t *media_flags )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_media_flags";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media_values->media_flags > (uint8_t) INT8_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media flags value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media flags.\n",
		 function );

		return( -1 );
	}
	*media_flags = internal_handle->media_values->media_flags;

	return( 1 );
}

/* Retrieves the volume type value
 * Returns 1 if successful or -1 on error
 */
int libewf_get_volume_type(
     LIBEWF_HANDLE *handle,
     int8_t *volume_type )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_volume_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( volume_type == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid volume type.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->media_values->media_flags & 0x02 ) == 0 )
	{
		*volume_type = (int8_t) LIBEWF_VOLUME_TYPE_LOGICAL;
	}
	else
	{
		*volume_type = (int8_t) LIBEWF_VOLUME_TYPE_PHYSICAL;
	}
	return( 1 );
}

/* Retrieves the format type value
 * Returns 1 if successful or -1 on error
 */
int libewf_get_format(
     LIBEWF_HANDLE *handle,
     uint8_t *format )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_volume_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->format > (uint8_t) INT8_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid format value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	*format = internal_handle->format;

	return( 1 );
}

/* Retrieves the GUID
 * Returns 1 if successful or -1 on error
 */
int libewf_get_guid(
     LIBEWF_HANDLE *handle,
     uint8_t *guid,
     size_t size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_guid";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid GUID.\n",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: GUID too small.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     guid,
	     internal_handle->media_values->guid,
	     16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the MD5 hash
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_md5_hash(
     LIBEWF_HANDLE *handle,
     uint8_t *md5_hash,
     size_t size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_md5_hash";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing hash sections.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_sections->md5_hash_set == 0 )
	{
		return( 0 );
	}
	if( md5_hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( size < EWF_DIGEST_HASH_SIZE_MD5 )
	{
		LIBEWF_WARNING_PRINT( "%s: MD5 hash too small.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     md5_hash,
	     internal_handle->hash_sections->md5_hash,
	     EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the delta segment filename
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_delta_segment_filename(
     LIBEWF_HANDLE *handle,
     libewf_filename_t *filename,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_delta_segment_filename";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta_segment_table.\n",
		 function );

		return( -1 );
	}
	return( libewf_segment_file_handle_get_filename(
	         internal_handle->delta_segment_table->segment_file_handle[ 0 ],
	         filename,
	         length ) );
}

/* Retrieves the amount of acquiry errors
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_get_amount_of_acquiry_errors(
     LIBEWF_HANDLE *handle,
     uint32_t *amount_of_errors )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_amount_of_acquiry_errors";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->acquiry_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing acquiry errors.\n",
		 function );

		return( -1 );
	}
	if( amount_of_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of errors.\n",
		 function );

		return( -1 );
	}
	*amount_of_errors = internal_handle->acquiry_errors->amount;

	return( 1 );
}

/* Retrieves the information of an acquiry error
 * Returns 1 if successful, 0 if no acquiry error could be found or -1 on error
 */
int libewf_get_acquiry_error(
     LIBEWF_HANDLE *handle,
     uint32_t index,
     off64_t *first_sector,
     uint32_t *amount_of_sectors )
{
	static char *function = "libewf_get_acquiry_error";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( libewf_sector_table_get_sector(
	         ( (libewf_internal_handle_t *) handle )->acquiry_errors,
	         index,
	         first_sector,
	         amount_of_sectors ) );
}

/* Retrieves the amount of CRC errors
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_crc_errors(
     LIBEWF_HANDLE *handle,
     uint32_t *amount_of_errors )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_amount_of_crc_errors";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle read.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->read->crc_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid subhandle read - missing crc errors.\n",
		 function );

		return( -1 );
	}
	if( amount_of_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of errors.\n",
		 function );

		return( -1 );
	}
	*amount_of_errors = internal_handle->read->crc_errors->amount;

	return( 1 );
}

/* Retrieves the information of a CRC error
 * Returns 1 if successful, 0 if no CRC error could be found or -1 on error
 */
int libewf_get_crc_error(
     LIBEWF_HANDLE *handle,
     uint32_t index,
     off64_t *first_sector,
     uint32_t *amount_of_sectors )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_crc_error";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle read.\n",
		 function );

		return( -1 );
	}
	return( libewf_sector_table_get_sector(
	         internal_handle->read->crc_errors,
	         index,
	         first_sector,
	         amount_of_sectors ) );
}

/* Retrieves the amount of sessions
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_sessions(
     LIBEWF_HANDLE *handle,
     uint32_t *amount_of_sessions )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_amount_of_sessions";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->sessions == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sessions.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sessions == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sessions.\n",
		 function );

		return( -1 );
	}
	*amount_of_sessions = internal_handle->sessions->amount;

	return( 1 );
}

/* Retrieves the information of a session
 * Returns 1 if successful, 0 if no session could be found or -1 on error
 */
int libewf_get_session(
     LIBEWF_HANDLE *handle,
     uint32_t index,
     off64_t *first_sector,
     uint32_t *amount_of_sectors )
{
	static char *function = "libewf_get_session";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( libewf_sector_table_get_sector(
	         ( (libewf_internal_handle_t *) handle )->sessions,
	         index,
	         first_sector,
	         amount_of_sectors ) );
}

/* Retrieves the amount of chunks written
 * Returns 1 if successful or -1 on error
 */
int libewf_get_write_amount_of_chunks(
     LIBEWF_HANDLE *handle,
     uint32_t *amount_of_chunks )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_write_amount_of_chunks";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	if( amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of chunks.\n",
		 function );

		return( -1 );
	}
	*amount_of_chunks = internal_handle->write->amount_of_chunks;

	return( 1 );
}

/* Retrieves the amount of header values
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_get_amount_of_header_values(
     LIBEWF_HANDLE *handle,
     uint32_t *amount_of_values )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_amount_of_header_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( amount_of_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->header_values == NULL )
	{
		return( 0 );
	}
	*amount_of_values = internal_handle->header_values->amount;

	return( 1 );
}

/* Retrieves the header value identifier specified by its index
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_header_value_identifier(
     LIBEWF_HANDLE *handle,
     uint32_t index,
     character_t *value,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_header_value_identifier";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->header_values == NULL )
	{
		return( 0 );
	}
	return( libewf_values_table_get_identifier(
	         internal_handle->header_values,
	         index,
	         value,
	         length ) );
}

/* Retrieves the header value specified by the identifier
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_header_value(
     LIBEWF_HANDLE *handle,
     character_t *identifier,
     character_t *value,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_header_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

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
	return( libewf_values_table_get_value(
	         internal_handle->header_values,
	         identifier,
	         value,
	         length ) );
}

/* Retrieves the amount of hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_get_amount_of_hash_values(
     LIBEWF_HANDLE *handle,
     uint32_t *amount_of_values )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_amount_of_hash_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( amount_of_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_values == NULL )
	{
		return( 0 );
	}
	*amount_of_values = internal_handle->hash_values->amount;

	return( 1 );
}

/* Retrieves the hash value identifier specified by its index
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_hash_value_identifier(
     LIBEWF_HANDLE *handle,
     uint32_t index,
     character_t *value,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_hash_value_identifier";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_values == NULL )
	{
		return( 0 );
	}
	return( libewf_values_table_get_identifier(
	         internal_handle->hash_values,
	         index,
	         value,
	         length ) );
}

/* Retrieves the hash value specified by the identifier
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_get_hash_value(
     LIBEWF_HANDLE *handle,
     character_t *identifier,
     character_t *value,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_get_hash_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

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
	return( libewf_values_table_get_value(
                 internal_handle->hash_values,
                 identifier,
                 value,
                 length ) );
}

/* Sets the amount of sectors per chunk in the media information
 * Returns 1 if successful or -1 on error
 */
int libewf_set_sectors_per_chunk(
     LIBEWF_HANDLE *handle,
     uint32_t sectors_per_chunk )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_sectors_per_chunk";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: sectors per chunk cannot be changed.\n",
		 function );

		return( -1 );
	}
	return( libewf_internal_handle_initialize_media_values(
	         internal_handle,
	         sectors_per_chunk,
	         internal_handle->media_values->bytes_per_sector,
	         internal_handle->media_values->media_size ) );
}

/* Sets the amount of bytes per sector in the media information
 * Returns 1 if successful or -1 on error
 */
int libewf_set_bytes_per_sector(
     LIBEWF_HANDLE *handle,
     uint32_t bytes_per_sector )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_bytes_per_sector";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: bytes per sector cannot be changed.\n",
		 function );

		return( -1 );
	}
	return( libewf_internal_handle_initialize_media_values(
	         internal_handle,
	         internal_handle->media_values->sectors_per_chunk,
	         bytes_per_sector,
	         internal_handle->media_values->media_size ) );
}

/* Sets the error granularity
 * Returns 1 if successful or -1 on error
 */
int libewf_set_error_granularity(
     LIBEWF_HANDLE *handle,
     uint32_t error_granularity )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_error_granularity";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: error granularity cannot be changed.\n",
		 function );

		return( -1 );
	}
	internal_handle->media_values->error_granularity = error_granularity;

	return( 1 );
}

/* Sets the compression values
 * Returns 1 if successful or -1 on error
 */
int libewf_set_compression_values(
     LIBEWF_HANDLE *handle,
     int8_t compression_level,
     uint8_t compress_empty_block )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_compression_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: compression values cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported compression level.\n",
		 function );

		return( -1 );
	}
	internal_handle->compression_level    = compression_level;
	internal_handle->compress_empty_block = 0;

	/* Compress empty block is only useful when no compression is used
	 */
	if( compression_level == EWF_COMPRESSION_NONE )
	{
		internal_handle->compress_empty_block = compress_empty_block;
	}
	return( 1 );
}

/* Sets the media size
 * Returns 1 if successful or -1 on error
 */
int libewf_set_media_size(
     LIBEWF_HANDLE *handle,
     size64_t media_size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_media_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: media size cannot be changed.\n",
		 function );

		return( -1 );
	}
	return( libewf_internal_handle_initialize_media_values(
	         internal_handle,
	         internal_handle->media_values->sectors_per_chunk,
	         internal_handle->media_values->bytes_per_sector,
	         media_size ) );
}

/* Sets the segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_set_segment_file_size(
     LIBEWF_HANDLE *handle,
     size64_t segment_file_size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_segment_file_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: segment file size cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( ( segment_file_size == 0 )
	 || ( segment_file_size > (size64_t) INT64_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file size value.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->segment_file_size > internal_handle->write->maximum_segment_file_size )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment file size value exceeds maximum segment file size.\n",
		 function );

		return( -1 );
	}
	internal_handle->write->segment_file_size = segment_file_size;

	return( 1 );
}

/* Sets the delta segment file size
 * Returns 1 if successful or -1 on error
 */
int libewf_set_delta_segment_file_size(
     LIBEWF_HANDLE *handle,
     size64_t delta_segment_file_size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_delta_segment_file_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: delta segment file size cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( ( delta_segment_file_size == 0 )
	 || ( delta_segment_file_size > (size64_t) INT64_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid delta segment file size value.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->delta_segment_file_size > (size64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid delta segment file size value exceeds maximum segment file size.\n",
		 function );

		return( -1 );
	}
	internal_handle->write->delta_segment_file_size = delta_segment_file_size;

	return( 1 );
}

/* Sets the media type
 * Returns 1 if successful or -1 on error
 */
int libewf_set_media_type(
     LIBEWF_HANDLE *handle,
     uint8_t media_type )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_media_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: media type cannot be changed.\n",
		 function );

		return( -1 );
	}
	internal_handle->media_values->media_type = media_type;

	return( 1 );
}

/* Sets the volume type
 * Returns 1 if successful or -1 on error
 */
int libewf_set_volume_type(
     LIBEWF_HANDLE *handle,
     uint8_t volume_type )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_volume_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: volume type cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( volume_type == LIBEWF_VOLUME_TYPE_LOGICAL )
	{
		/* Uses 1-complement of EWF_MEDIA_FLAGS_IS_PHYSICAL
		 */
		internal_handle->media_values->media_flags &= ~EWF_MEDIA_FLAGS_IS_PHYSICAL;
	}
	else if( volume_type == LIBEWF_VOLUME_TYPE_PHYSICAL )
	{
		internal_handle->media_values->media_flags |= EWF_MEDIA_FLAGS_IS_PHYSICAL;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported volume type.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the output format
 * Returns 1 if successful or -1 on error
 */
int libewf_set_format(
     LIBEWF_HANDLE *handle,
     uint8_t format )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_format";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: format cannot be changed.\n",
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
		LIBEWF_WARNING_PRINT( "%s: unsupported format.\n",
		 function );

		return( -1 );
	}
	internal_handle->format = format;

	if( libewf_internal_handle_initialize_format(
	     internal_handle ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize format specific values.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the GUID
 * Returns 1 if successful or -1 on error
 */
int libewf_set_guid(
     LIBEWF_HANDLE *handle,
     uint8_t *guid,
     size_t size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_guid";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( guid == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid GUID.\n",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		LIBEWF_WARNING_PRINT( "%s: GUID too small.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: GUID cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     internal_handle->media_values->guid,
	     guid,
	     16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the MD5 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_set_md5_hash(
     LIBEWF_HANDLE *handle,
     uint8_t *md5_hash,
     size_t size )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_md5_hash";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing hash sections.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->read != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: md5 hash cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( size < EWF_DIGEST_HASH_SIZE_MD5 )
	{
		LIBEWF_WARNING_PRINT( "%s: MD5 hash too small.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_sections->md5_hash_set )
	{
		LIBEWF_WARNING_PRINT( "%s: MD5 hash cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     internal_handle->hash_sections->md5_hash,
	     md5_hash,
	     EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash.\n",
		 function );

		return( -1 );
	}
	internal_handle->hash_sections->md5_hash_set = 1;

	return( 1 );
}

/* Sets the delta segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_set_delta_segment_filename(
     LIBEWF_HANDLE *handle,
     libewf_filename_t *filename,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_delta_segment_filename";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write == NULL )
	 && ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: delta segment filename cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->segment_file_handle[ 0 ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing first segment file.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->segment_file_handle[ 0 ]->filename != NULL )
	{
		memory_free(
		 internal_handle->delta_segment_table->segment_file_handle[ 0 ]->filename );

		internal_handle->delta_segment_table->segment_file_handle[ 0 ]->filename        = NULL;
		internal_handle->delta_segment_table->segment_file_handle[ 0 ]->length_filename = 0;
	}
	return( libewf_segment_file_handle_set_filename(
	         internal_handle->delta_segment_table->segment_file_handle[ 0 ],
	         filename,
	         length ) );
}

/* Sets the read wipe chunk on error
 * The chunk is not wiped if read raw is used
 * Returns 1 if successful or -1 on error
 */
int libewf_set_read_wipe_chunk_on_error(
     LIBEWF_HANDLE *handle,
     uint8_t wipe_on_error )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_read_wipe_chunk_on_error";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle read.\n",
		 function );

		return( -1 );
	}
	internal_handle->read->wipe_on_error = wipe_on_error;

	return( 1 );
}

/* Sets the header value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_set_header_value(
     LIBEWF_HANDLE *handle,
     character_t *identifier,
     character_t *value,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_header_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->read != NULL )
	 || ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: header value cannot be changed.\n",
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
		internal_handle->header_values = libewf_values_table_alloc(
		                                  LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT );

		if( internal_handle->header_values == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
			 function );

			return( -1 );
		}
		if( libewf_header_values_initialize(
		     internal_handle->header_values ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to initialize header values.\n",
			 function );

			return( -1 );
		}
	}
	return( libewf_values_table_set_value(
	         internal_handle->header_values,
	         identifier,
	         value,
	         length ) );
}

/* Sets the hash value specified by the identifier
 * Returns 1 if successful or -1 on error
 */
int libewf_set_hash_value(
     LIBEWF_HANDLE *handle,
     character_t *identifier,
     character_t *value,
     size_t length )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_set_hash_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->read != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: hash value cannot be changed.\n",
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
		internal_handle->hash_values = libewf_values_table_alloc(
		                                LIBEWF_HASH_VALUES_DEFAULT_AMOUNT );

		if( internal_handle->hash_values == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create hash values.\n",
			 function );

			return( -1 );
		}
		if( libewf_hash_values_initialize(
		     internal_handle->hash_values ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to initialize hash values.\n",
			 function );

			return( -1 );
		}
	}
	return( libewf_values_table_set_value(
	         internal_handle->hash_values,
	         identifier,
	         value,
	         length ) );
}

/* Parses the header values from the xheader, header2 or header section
 * Will parse the first available header in order mentioned above
 * Returns 1 if successful, 0 if already parsed or -1 on error
 */
int libewf_parse_header_values(
     LIBEWF_HANDLE *handle,
     uint8_t date_format )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_parse_header_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing header sections.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->header_values != NULL )
	{
		return( 0 );
	}
	if( ( internal_handle->header_sections->xheader != NULL )
	 && ( libewf_header_values_parse_xheader(
	       &( internal_handle->header_values ),
	       internal_handle->header_sections->xheader,
	       internal_handle->header_sections->xheader_size,
	       date_format ) != 1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse xheader.\n",
		 function );
	}
	if( ( internal_handle->header_values == NULL )
	 && ( internal_handle->header_sections->header2 != NULL )
	 && ( libewf_header_values_parse_header2(
	       &( internal_handle->header_values ),
	       internal_handle->header_sections->header2,
	       internal_handle->header_sections->header2_size,
	       date_format ) != 1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse header2.\n",
		 function );
	}
	if( ( internal_handle->header_values == NULL )
	 && ( internal_handle->header_sections->header != NULL )
	 && ( libewf_header_values_parse_header(
	       &( internal_handle->header_values ),
	       internal_handle->header_sections->header,
	       internal_handle->header_sections->header_size,
	       date_format ) != 1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse header.\n",
		 function );
	}
	if( internal_handle->header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse header(s) for values.\n",
		 function );

		return( -1 );
	}
	/* The EnCase2 and EnCase3 format are the same
	 * only the acquiry software version provides insight in which version of EnCase was used
	 */
	if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE2 )
	 && ( internal_handle->header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( internal_handle->header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ][ 0 ] == '3' ) )
 	{
		internal_handle->format = LIBEWF_FORMAT_ENCASE3;
	}
	return( 1 );
}

/* Parses the hash values from the xhash section
 * Returns 1 if successful, 0 if already parsed or -1 on error
 */
int libewf_parse_hash_values(
     LIBEWF_HANDLE *handle )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_parse_hash_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->hash_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing hash sections.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_values != NULL )
	{
		return( 0 );
	}
	if( ( internal_handle->hash_sections->xhash != NULL )
	 && ( libewf_hash_values_parse_xhash(
	       &( internal_handle->hash_values ),
	       internal_handle->hash_sections->xhash,
	       internal_handle->hash_sections->xhash_size ) != 1 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse xhash for values.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Add an acquiry error
 * Returns 1 if successful or -1 on error
 */
int libewf_add_acquiry_error(
     LIBEWF_HANDLE *handle,
     off64_t first_sector,
     uint32_t amount_of_sectors )
{
	static char *function = "libewf_add_acquiry_error";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( libewf_sector_table_add_sector(
	         ( (libewf_internal_handle_t *) handle )->acquiry_errors,
	         first_sector,
	         amount_of_sectors,
	         1 ) );
}

/* Add a CRC error
 * Returns 1 if successful or -1 on error
 */
int libewf_add_crc_error(
     LIBEWF_HANDLE *handle,
     off64_t first_sector,
     uint32_t amount_of_sectors )
{
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_add_crc_error";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle read.\n",
		 function );

		return( -1 );
	}
	return( libewf_sector_table_add_sector(
	         internal_handle->read->crc_errors,
	         first_sector,
	         amount_of_sectors,
	         1 ) );
}

/* Add a session
 * Returns 1 if successful or -1 on error
 */
int libewf_add_session(
     LIBEWF_HANDLE *handle,
     off64_t first_sector,
     uint32_t amount_of_sectors )
{
	static char *function = "libewf_add_session";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	return( libewf_sector_table_add_sector(
	         ( (libewf_internal_handle_t *) handle )->sessions,
	         first_sector,
	         amount_of_sectors,
	         0 ) );
}

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_copy_header_values(
     LIBEWF_HANDLE *destination_handle,
     LIBEWF_HANDLE *source_handle )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_copy_header_values";

	if( destination_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid destination handle.\n",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source handle.\n",
		 function );

		return( -1 );
	}
	internal_destination_handle = (libewf_internal_handle_t *) destination_handle;
	internal_source_handle      = (libewf_internal_handle_t *) source_handle;

	if( internal_source_handle->header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source handle - missing header values.\n",
		 function );

		return( -1 );
	}
	if( internal_destination_handle->header_values == NULL )
	{
		internal_destination_handle->header_values = libewf_values_table_alloc(
		                                              LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT );

		if( internal_destination_handle->header_values == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values in destination handle.\n",
			 function );

			return( -1 );
		}
		if( libewf_header_values_initialize(
		     internal_destination_handle->header_values ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to initialize header values.\n",
			 function );

			return( -1 );
		}
	}
	return( libewf_header_values_copy(
	         internal_destination_handle->header_values,
	         internal_source_handle->header_values ) );
}

/* Copies the media values from the source to the destination handle
 * Returns 1 if successful or -1 on error
 */
int libewf_copy_media_values(
     LIBEWF_HANDLE *destination_handle,
     LIBEWF_HANDLE *source_handle )
{
	libewf_internal_handle_t *internal_destination_handle = NULL;
	libewf_internal_handle_t *internal_source_handle      = NULL;
	static char *function                                 = "libewf_copy_media_values";

	if( destination_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid destination handle.\n",
		 function );

		return( -1 );
	}
	if( source_handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source handle.\n",
		 function );

		return( -1 );
	}
	internal_destination_handle = (libewf_internal_handle_t *) destination_handle;
	internal_source_handle      = (libewf_internal_handle_t *) source_handle;

	if( internal_source_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( internal_destination_handle->media_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid destination handle - missing media values.\n",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     internal_destination_handle->media_values,
	     internal_source_handle->media_values,
	     sizeof( libewf_media_values_t ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy media values.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Set the notify values
 */
void libewf_set_notify_values(
      FILE *stream,
      uint8_t verbose )
{
	libewf_notify_set_values( stream, verbose );
}


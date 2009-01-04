/*
 * libewf file handling
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

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <errno.h>

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_endian.h"
#include "libewf_notify.h"
#include "libewf_file.h"
#include "libewf_offset_table.h"
#include "libewf_read.h"
#include "libewf_section_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"
#include "libewf_string.h"
#include "libewf_write.h"

#include "ewf_compress.h"
#include "ewf_crc.h"
#include "ewf_definitions.h"
#include "ewf_digest_hash.h"
#include "ewf_file_header.h"
#include "ewf_section.h"
#include "ewf_volume.h"
#include "ewf_table.h"

/* Returns the library version
 */
const LIBEWF_CHAR *libewf_get_version( void )
{
	return( (const LIBEWF_CHAR *) LIBEWF_VERSION );
}

/* Returns the flags for reading
 */
uint8_t libewf_get_flags_read( void )
{
	return( (uint8_t) LIBEWF_FLAG_READ );
}

/* Returns the flags for reading and writing
 */
uint8_t libewf_get_flags_read_write( void )
{
	return( (uint8_t) ( LIBEWF_FLAG_READ | LIBEWF_FLAG_WRITE ) );
}

/* Returns the flags for writing
 */
uint8_t libewf_get_flags_write( void )
{
	return( (uint8_t) LIBEWF_FLAG_WRITE );
}

/* Detects if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not, or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_check_file_signature( const wchar_t *filename )
#else
int libewf_check_file_signature( const char *filename )
#endif
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
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	file_descriptor = libewf_common_wide_open( filename, LIBEWF_OPEN_READ );
#else
	file_descriptor = libewf_common_open( filename, LIBEWF_OPEN_READ );
#endif

	if( file_descriptor < 0 )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		LIBEWF_WARNING_PRINT( "%s: unable to open file: %ls.\n",
		 function, filename );
#else
		LIBEWF_WARNING_PRINT( "%s: unable to open file: %s.\n",
		 function, filename );
#endif
		return( -1 );
	}
	result = libewf_segment_file_check_file_signature( file_descriptor );

	if( libewf_common_close( file_descriptor ) != 0 )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		LIBEWF_WARNING_PRINT( "%s: unable to close file: %ls.\n",
		 function, filename );
#else
		LIBEWF_WARNING_PRINT( "%s: unable to close file: %s.\n",
		 function, filename );
#endif

		return( -1 );
	}
	if( result <= -1 )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		LIBEWF_WARNING_PRINT( "%s: unable to read signature from file: %ls.\n",
		 function, filename );
#else
		LIBEWF_WARNING_PRINT( "%s: unable to read signature from file: %s.\n",
		 function, filename );
#endif

		return( -1 );
	}
	return( result );
}

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
LIBEWF_HANDLE *libewf_open( wchar_t * const filenames[], uint16_t file_amount, uint8_t flags )
#else
LIBEWF_HANDLE *libewf_open( char * const filenames[], uint16_t file_amount, uint8_t flags )
#endif
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_open";
	uint32_t iterator                       = 0;

	if( filenames == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filenames.\n",
		 function );

		return( NULL );
	}
	if( file_amount < 1 )
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
		/* Initialize the internal handle for reading
		 */
		if( libewf_internal_handle_read_initialize( internal_handle ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to initialize read values in handle.\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		if( libewf_segment_file_read_wide_open(
		     internal_handle, 
		     filenames, 
		     file_amount,
		     flags ) != 1 )
#else
		if( libewf_segment_file_read_open(
		     internal_handle, 
		     filenames, 
		     file_amount,
		     flags ) != 1 )
#endif
		{
			LIBEWF_WARNING_PRINT( "%s: unable to open segment file(s).\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
		/* Determine the EWF file format
		 */
		if( libewf_internal_handle_determine_format( internal_handle ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine file format.\n",
			 function );
		}
		/* Calculate the media size
		 */
		internal_handle->media->media_size = (size64_t) internal_handle->media->amount_of_sectors
						   * (size64_t) internal_handle->media->bytes_per_sector;
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		if( internal_handle->segment_table == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid handle - missing segment table.\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		if( libewf_segment_table_set_wide_filename(
		     internal_handle->segment_table,
		     0,
		     filenames[ iterator ],
		     libewf_common_string_length( filenames[ iterator ] ) ) != 1 )
#else
		if( libewf_segment_table_set_filename(
		     internal_handle->segment_table,
		     0,
		     filenames[ iterator ],
		     libewf_common_string_length( filenames[ iterator ] ) ) != 1 )
#endif
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set filename in segment table.\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
	}
	LIBEWF_VERBOSE_PRINT( "%s: open successful.\n",
	 function );

	return( (LIBEWF_HANDLE *) internal_handle );
}

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful, or -1 on error
 */
int libewf_close( LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_close";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->write_finalized == 0 ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: finalizing write.\n",
		 function );

		libewf_write_finalize( handle );
	}
	if( libewf_segment_file_close_all( internal_handle ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to close all segment files.\n",
		 function );

		return( -1 );
	}
	libewf_internal_handle_free( internal_handle );

	return( 0 );
}

/* Seeks a certain offset of the media data within the EWF file(s)
 * It will set the related file offset to the specific chunk offset
 * Returns the offset if seek is successful, or -1 on error
 */
off64_t libewf_seek_offset( LIBEWF_HANDLE *handle, off64_t offset )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_seek_offset";
	uint64_t chunk                          = 0;
	uint64_t chunk_offset                   = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( offset <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid offset value cannot be negative.\n",
		 function );

		return( -1 );
	}
	if( offset >= (off64_t) internal_handle->media->media_size )
	{
		LIBEWF_WARNING_PRINT( "%s: attempting to read past the end of the file.\n",
		 function );

		return( -1 );
	}
	/* Determine the chunk that is requested
	 */
	chunk = offset / internal_handle->media->chunk_size;

	if( chunk >= (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_segment_file_seek_chunk_offset( internal_handle, (uint32_t) chunk ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to seek chunk offset.\n",
		 function );

		return( -1 );
	}
	/* Determine the offset within the decompressed chunk that is requested
	 */
	chunk_offset = offset % internal_handle->media->chunk_size;

	if( chunk_offset >= (uint64_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	internal_handle->current_chunk_offset = (uint32_t) chunk_offset;

	return( offset );
}

/* Returns the amount of sectors per chunk from the media information
 * Returns 1 if successful, -1 on error
 */
int libewf_get_sectors_per_chunk( LIBEWF_HANDLE *handle, uint32_t *sectors_per_chunk )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_sectors_per_chunk";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->sectors_per_chunk > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*sectors_per_chunk = internal_handle->media->sectors_per_chunk;

	return( 1 );
}

/* Returns the amount of bytes per sector from the media information
 * Returns 1 if successful, -1 on error
 */
int libewf_get_bytes_per_sector( LIBEWF_HANDLE *handle, uint32_t *bytes_per_sector )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_bytes_per_sector";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->bytes_per_sector > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*bytes_per_sector = internal_handle->media->bytes_per_sector;

	return( 1 );
}

/* Returns the amount of sectors from the media information
 * Returns 1 if successful, -1 on error
 */
int libewf_get_amount_of_sectors( LIBEWF_HANDLE *handle, uint32_t *amount_of_sectors )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_amount_of_sectors";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( amount_of_sectors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->amount_of_sectors > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sectors value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*amount_of_sectors = internal_handle->media->amount_of_sectors;

	return( 1 );
}

/* Returns the chunk size from the media information
 * Returns 1 if successful, -1 on error
 */
int libewf_get_chunk_size( LIBEWF_HANDLE *handle, size32_t *chunk_size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_chunk_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->chunk_size > (size32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*chunk_size = internal_handle->media->chunk_size;

	return( 1 );
}

/* Returns the error granularity from the media information
 * Returns 1 if successful, -1 on error
 */
int libewf_get_error_granularity( LIBEWF_HANDLE *handle, uint32_t *error_granularity )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_error_granularity";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( error_granularity == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid error granularity.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->error_granularity > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid error granularity value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*error_granularity = internal_handle->media->error_granularity;

	return( 1 );
}

/* Returns the compression level value
 * Returns 1 if successful, -1 on error
 */
int libewf_get_compression_level( LIBEWF_HANDLE *handle, int8_t *compression_level )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_compression_level";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( compression_level == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compression level.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->compression_level <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid compression level only positive values are supported.\n",
		 function );

		return( -1 );
	}
	*compression_level = internal_handle->compression_level;

	return( 1 );
}

/* Returns the size of the contained media data
 * Returns 1 if successful, -1 on error
 */
int libewf_get_media_size( LIBEWF_HANDLE *handle, size64_t *media_size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_media_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing media sub handle.\n",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media size.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->media->media_size == 0 )
	{
		internal_handle->media->media_size = (size64_t) internal_handle->media->amount_of_sectors
		                                   * (size64_t) internal_handle->media->bytes_per_sector;
	}
	if( internal_handle->media->media_size > (size64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid media size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	*media_size = internal_handle->media->media_size;

	return( 1 );
}

/* Returns the media type value, or -1 on error
 */
int8_t libewf_get_media_type( LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_media_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
int8_t libewf_get_media_flags( LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_media_flags";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
int8_t libewf_get_volume_type( LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_volume_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
int8_t libewf_get_format( LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_volume_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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

/* Returns the GUID
 * Returns 1 if successful, -1 on error
 */
int libewf_get_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_guid";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
	if( libewf_common_memcpy( guid, internal_handle->guid, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Returns the MD5 hash
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int libewf_get_md5_hash( LIBEWF_HANDLE *handle, uint8_t *md5_hash, size_t size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_md5_hash";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->md5_hash_set == 0 )
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
	if( libewf_common_memcpy( md5_hash, internal_handle->md5_hash, EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}


/* Returns the delta segment filename
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int libewf_get_delta_segment_filename( LIBEWF_HANDLE *handle, LIBEWF_CHAR *filename, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_delta_segment_filename";
	size_t filename_length                  = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->delta_segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing delta segment table.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing filenames.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->filename[ 0 ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing filename.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	filename_length = libewf_string_length( internal_handle->delta_segment_table->filename[ 0 ] );

	if( length < filename_length )
	{
		LIBEWF_WARNING_PRINT( "%s: filename too small.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy(
	     filename,
	     internal_handle->delta_segment_table->filename[ 0 ],
	     filename_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Returns the amount of chunks written
 * Returns 1 if successful, -1 on error
 */
int libewf_get_write_amount_of_chunks( LIBEWF_HANDLE *handle, uint32_t *amount_of_chunks )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_write_amount_of_chunks";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing write sub handle.\n",
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

/* Retrieves the header value specified by the identifier
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int libewf_get_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_header_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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

/* Retrieves the hash value specified by the identifier
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int libewf_get_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_hash_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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

/* Sets the amount of sectors per chunk in the media information
 *  * Returns 1 if successful, -1 on error
 *   */
int libewf_set_sectors_per_chunk( LIBEWF_HANDLE *handle, uint32_t sectors_per_chunk )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_sectors_per_chunk";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( ( sectors_per_chunk == 0 )
	 || ( sectors_per_chunk > (uint32_t) INT32_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid sectors per chunk.\n",
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
	internal_handle->media->sectors_per_chunk = sectors_per_chunk;

	return( 1 );
}

/* Sets the amount of bytes per sector in the media information
 *  * Returns 1 if successful, -1 on error
 *   */
int libewf_set_bytes_per_sector( LIBEWF_HANDLE *handle, uint32_t bytes_per_sector )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_bytes_per_sector";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	if( ( bytes_per_sector == 0 )
	 || ( bytes_per_sector > (uint32_t) INT32_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid bytes per sector.\n",
		 function );

		return( -1 );
	}
	if( ( internal_handle->write == NULL )
	 || ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: bytes per sector cannot be changed.\n",
		 function );

		return( -1 );
	}
	internal_handle->media->bytes_per_sector = bytes_per_sector;

	return( 1 );
}

/* Sets the GUID
 * Returns 1 if successful, or -1 on error
 */
int libewf_set_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_guid";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->values_initialized != 0 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: GUID cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy( internal_handle->guid, guid, 16 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set GUID.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the MD5 hash
 * Returns 1 if successful, or -1 on error
 */
int libewf_set_md5_hash( LIBEWF_HANDLE *handle, uint8_t *md5_hash, size_t size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_md5_hash";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
	if( internal_handle->md5_hash_set )
	{
		LIBEWF_WARNING_PRINT( "%s: MD5 hash cannot be changed.\n",
		 function );

		return( -1 );
	}
	if( libewf_common_memcpy( internal_handle->md5_hash, md5_hash, EWF_DIGEST_HASH_SIZE_MD5 ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash.\n",
		 function );

		return( -1 );
	}
	internal_handle->md5_hash_set = 1;

	return( 1 );
}

/* Sets the delta segment file
 * Returns 1 if successful, or -1 on error
 */
int libewf_set_delta_segment_filename( LIBEWF_HANDLE *handle, LIBEWF_CHAR *filename, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_delta_segment_filename";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->write == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle write.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->write->values_initialized != 0 )
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
	if( internal_handle->delta_segment_table->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - invalid delta segment table - missing filenames.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->delta_segment_table->filename[ 0 ] != NULL )
	{
		libewf_common_free( internal_handle->delta_segment_table->filename[ 0 ] );
	}
	internal_handle->delta_segment_table->filename[ 0 ] = libewf_string_duplicate( filename, length );

	if( internal_handle->delta_segment_table->filename[ 0 ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create delta segment file.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the write segment file size
 * Returns 1 if successful, -1 on error
 */
int libewf_set_write_segment_file_size( LIBEWF_HANDLE *handle, size64_t segment_file_size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_write_segment_file_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
	 || ( segment_file_size > (size64_t) INT64_MAX ) )
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
int libewf_set_write_error_granularity( LIBEWF_HANDLE *handle, uint32_t error_granularity )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_write_error_granularity";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
int libewf_set_write_compression_values( LIBEWF_HANDLE *handle, int8_t compression_level, uint8_t compress_empty_block )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_write_compression_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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

/* Sets the media type
 * Returns 1 if successful, -1 on error
 */
int libewf_set_write_media_type( LIBEWF_HANDLE *handle, uint8_t media_type, uint8_t volume_type )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_write_media_type";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing sub handle media.\n",
		 function );

		return( -1 );
	}
	internal_handle->media->media_type = media_type;

	if( volume_type == LIBEWF_VOLUME_TYPE_LOGICAL )
	{
		/* Uses 1-complement of EWF_MEDIA_FLAGS_IS_PHYSICAL
		 */
		internal_handle->media->media_flags &= ~EWF_MEDIA_FLAGS_IS_PHYSICAL;
	}
	else if( volume_type == LIBEWF_VOLUME_TYPE_PHYSICAL )
	{
		internal_handle->media->media_flags |= EWF_MEDIA_FLAGS_IS_PHYSICAL;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported volume type.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the write output format
 * Returns 1 if successful, -1 on error
 */
int libewf_set_write_format( LIBEWF_HANDLE *handle, uint8_t format )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_write_format";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	internal_handle->format = format;

	return( 1 );
}

/* Sets the write input size
 * Returns 1 if successful, -1 on error
 */
int libewf_set_write_input_size( LIBEWF_HANDLE *handle, size64_t input_write_size )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_write_input_size";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
int libewf_set_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_header_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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
int libewf_set_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_set_hash_value";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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

/* Parses the header values from the xheader, header2 or header section
 * Will parse the first available header in order mentioned above
 * Returns 1 if successful, -1 on error
 */
int libewf_parse_header_values( LIBEWF_HANDLE *handle, uint8_t date_format )
{
	LIBEWF_HEADER_VALUES *header_values     = NULL;
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_parse_header_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->xheader != NULL )
	{
		header_values = libewf_header_values_parse_xheader(
		                 internal_handle->xheader,
		                 internal_handle->xheader_size,
		                 date_format );
	}
	if( ( header_values == NULL )
	 && internal_handle->header2 != NULL )
	{
		header_values = libewf_header_values_parse_header2(
		                 internal_handle->header2,
		                 internal_handle->header2_size,
		                 date_format );
	}
	if( ( header_values == NULL )
	 && ( internal_handle->header != NULL ) )
	{
		header_values = libewf_header_values_parse_header(
		                 internal_handle->header,
		                 internal_handle->header_size,
		                 date_format );
	}
	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse header(s) for values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->header_values != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: header values already set in handle - cleaning up previous ones.\n",
		 function );

		libewf_header_values_free( internal_handle->header_values );
	}
	internal_handle->header_values = header_values;

	/* The EnCase2 and EnCase3 format are the same
	 * only the acquiry software version provides insight in which version of EnCase was used
	 */
	if( ( internal_handle->format == LIBEWF_FORMAT_ENCASE2 )
	 && ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ][ 0 ] == '3' ) )
 	{
		internal_handle->format = LIBEWF_FORMAT_ENCASE3;
	}
	return( 1 );
}

/* Parses the hash values from the xhash section
 * Returns 1 if successful, -1 on error
 */
int libewf_parse_hash_values( LIBEWF_HANDLE *handle )
{
	LIBEWF_HASH_VALUES *hash_values         = NULL;
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_parse_hash_values";

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->xhash != NULL )
	{
		hash_values = libewf_hash_values_parse_xhash(
		               internal_handle->xhash,
		               internal_handle->xhash_size );
	}
	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse xhash for values.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->hash_values != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: hash values already set in handle - cleaning up previous ones.\n",
		 function );

		libewf_hash_values_free( internal_handle->hash_values );
	}
	internal_handle->hash_values = hash_values;

	return( 1 );
}

/* Add an acquiry error
 * Returns 1 if successful, -1 on error
 */
int libewf_add_acquiry_error( LIBEWF_HANDLE *handle, off64_t sector, uint32_t amount_of_sectors )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle    = NULL;
	LIBEWF_ERROR_SECTOR *acquiry_error_sectors = NULL;
	static char *function                      = "libewf_add_acquiry_error";
	uint32_t iterator                          = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

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

/* Copies the header values from the source to the destination handle
 * Returns 1 if successful, -1 on error
 */
int libewf_copy_header_values( LIBEWF_HANDLE *destination_handle, LIBEWF_HANDLE *source_handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_destination_handle = NULL;
	LIBEWF_INTERNAL_HANDLE *internal_source_handle      = NULL;
	static char *function                               = "libewf_copy_header_values";

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
	internal_destination_handle = (LIBEWF_INTERNAL_HANDLE *) destination_handle;
	internal_source_handle      = (LIBEWF_INTERNAL_HANDLE *) source_handle;

	if( internal_source_handle->header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source handle - missing header values.\n",
		 function );

		return( -1 );
	}
	if( internal_destination_handle->header_values == NULL )
	{
		internal_destination_handle->header_values = libewf_header_values_alloc();

		if( internal_destination_handle->header_values == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header values in destination handle.\n",
			 function );

			return( -1 );
		}
	}
	return( libewf_header_values_copy(
	         internal_destination_handle->header_values,
	         internal_source_handle->header_values ) );
}

/* Set the notify values
 */
void libewf_set_notify_values( FILE *stream, uint8_t verbose )
{
	libewf_notify_set_values( stream, verbose );
}


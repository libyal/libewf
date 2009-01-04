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
#include "ewf_digest_hash.h"
#include "ewf_file_header.h"
#include "ewf_header.h"
#include "ewf_section.h"
#include "ewf_volume.h"
#include "ewf_table.h"

/* Return the library version
 */
const LIBEWF_CHAR *libewf_get_version( void )
{
	return( (const LIBEWF_CHAR *) LIBEWF_VERSION );
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
	int result                              = 0;

	if( file_amount < 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( NULL );
	}
	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		/* 1 additional entry required because
		 * entry [ 0 ] is not used for reading
		 */
		internal_handle = libewf_internal_handle_alloc( ( file_amount + 1 ), flags );

		if( internal_handle == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create handle.\n",
			 function );

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
		/* TODO: Get the basename of the first segment file and store it in
		 * the 0'th entry
		 */

		/* Initialize the internal handle for reading
		 */
		if( libewf_internal_handle_read_initialize( internal_handle ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to initialize read values in handle.\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
		/* Read the segment table from the segment files
		 */
		result = libewf_segment_file_read_segment_table( internal_handle );

		if( result == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: error while trying to read the segment table.\n",
			 function );

			libewf_internal_handle_free( internal_handle );

			return( NULL );
		}
		else if( result != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to read segment table.\n",
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

		/* Flag that the segment table was build
		 */
		internal_handle->segment_table_build = 1;
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		/* Allocate 2 entries
		 * entry [ 0 ] is used for the base filename
		 */
		internal_handle = libewf_internal_handle_alloc( 1, flags );

		if( internal_handle == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create handle.\n",
			 function );

			return( NULL );
		}
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
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported flags.\n",
		 function );

		return( NULL );
	}
	LIBEWF_VERBOSE_PRINT( "%s: open successful.\n",
	 function );

	return( (LIBEWF_HANDLE *) internal_handle );
}

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful, or -1 on error
 */
int8_t libewf_close( LIBEWF_HANDLE *handle )
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
/*
	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->write_finalized == 0 ) )
	{
		LIBEWF_VERBOSE_PRINT( "%s: write has not been finalized.\n",
		 function );

		libewf_write_finalize( handle );
	}
*/
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
	if( internal_handle->segment_table_build == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: segment table was not build.\n",
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

/* Updates the internal MD5 for raw access mode
 * Returns 1 if successful, -1 on error
 */
int libewf_raw_update_md5( LIBEWF_HANDLE *handle, void *buffer, size_t size )
{
	return( libewf_internal_handle_raw_update_md5(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         buffer,
	         size ) );
}

/* Returns the amount of bytes per sector from the media information, 0 if not set, -1 on error
 */
int32_t libewf_get_bytes_per_sector( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_media_bytes_per_sector(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the amount of sectors from the media information, 0 if not set, -1 on error
 */
int32_t libewf_get_amount_of_sectors( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_media_amount_of_sectors(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the chunk size from the media information, 0 if not set, -1 on error
 */
ssize32_t libewf_get_chunk_size( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_media_chunk_size(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the error granularity from the media information, 0 if not set, -1 on error
 */
int32_t libewf_get_error_granularity( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_media_error_granularity(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the compression level value, or -1 on error
 */
int8_t libewf_get_compression_level( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_compression_level(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the size of the contained media data, 0 if not set, -1 on error
 */
ssize64_t libewf_get_media_size( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_media_size(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the media type value, or -1 on error
 */
int8_t libewf_get_media_type( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_media_type(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the media flags value, or -1 on error
 */
int8_t libewf_get_media_flags( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_media_flags(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the volume type value, or -1 on error
 */
int8_t libewf_get_volume_type( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_volume_type(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns the format value, or -1 on error
 */
int8_t libewf_get_format( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_format(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Returns 1 if the GUID is set, or -1 on error
 */
int8_t libewf_get_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size )
{
	return( libewf_internal_handle_get_guid(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         guid,
	         size ) );
}

/* Returns the amount of chunks written, 0 if no chunks have been written, or -1 on error
 */
int64_t libewf_get_write_amount_of_chunks( LIBEWF_HANDLE *handle )
{
	return( libewf_internal_handle_get_write_amount_of_chunks(
	         (LIBEWF_INTERNAL_HANDLE *) handle ) );
}

/* Retrieves the header value specified by the identifier
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int8_t libewf_get_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	return( libewf_internal_handle_get_header_value(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         identifier,
	         value,
	         length ) );
}

/* Retrieves the hash value specified by the identifier
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int8_t libewf_get_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	return( libewf_internal_handle_get_hash_value(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         identifier,
	         value,
	         length ) );
}

/* Sets the media values
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_media_values( LIBEWF_HANDLE *handle, uint32_t sectors_per_chunk, uint32_t bytes_per_sector )
{
	return( libewf_internal_handle_set_media_values(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         sectors_per_chunk,
	         bytes_per_sector ) );
}

/* Returns 1 if the GUID is set, or -1 on error
 */
int8_t libewf_set_guid( LIBEWF_HANDLE *handle, uint8_t *guid, size_t size )
{
	return( libewf_internal_handle_set_guid(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         guid,
	         size ) );
}

/* Sets the write segment file size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_write_segment_file_size( LIBEWF_HANDLE *handle, size32_t segment_file_size )
{
	return( libewf_internal_handle_set_write_segment_file_size(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         segment_file_size ) );
}

/* Sets the write error granularity
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_write_error_granularity( LIBEWF_HANDLE *handle, uint32_t error_granularity )
{
	return( libewf_internal_handle_set_write_error_granularity(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         error_granularity ) );
}

/* Sets the write compression values
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_write_compression_values( LIBEWF_HANDLE *handle, int8_t compression_level, uint8_t compress_empty_block )
{
	return( libewf_internal_handle_set_write_compression_values(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         compression_level,
	         compress_empty_block ) );
}

/* Sets the media type
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_write_media_type( LIBEWF_HANDLE *handle, uint8_t media_type, uint8_t volume_type )
{
	return( libewf_internal_handle_set_write_media_type(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         media_type,
	         volume_type ) );
}

/* Sets the write output format
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_write_format( LIBEWF_HANDLE *handle, uint8_t format )
{
	return( libewf_internal_handle_set_write_format(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         format ) );
}

/* Sets the write input size
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_write_input_size( LIBEWF_HANDLE *handle, size64_t input_write_size )
{
	return( libewf_internal_handle_set_write_input_write_size(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         input_write_size ) );
}

/* Sets the header value specified by the identifier
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_header_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	return( libewf_internal_handle_set_header_value(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         identifier,
	         value,
	         length ) );
}

/* Sets the hash value specified by the identifier
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_hash_value( LIBEWF_HANDLE *handle, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	return( libewf_internal_handle_set_hash_value(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         identifier,
	         value,
	         length ) );
}

/* Sets the swap byte pairs, used by both read and write
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_set_swap_byte_pairs( LIBEWF_HANDLE *handle, uint8_t swap_byte_pairs )
{
	return( libewf_internal_handle_set_swap_byte_pairs(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         swap_byte_pairs ) );
}

/* Calculates the MD5 hash and creates a printable string of the calculated md5 hash
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_calculate_md5_hash( LIBEWF_HANDLE *handle, LIBEWF_CHAR *string, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	uint8_t *data                           = NULL;
	static char *function                   = "libewf_calculate_md5_hash";
	off_t offset                            = 0;
	ssize_t count                           = 0;
	uint32_t iterator                       = 0;
	int result                              = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%S: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) internal_handle;

	if( internal_handle->media == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing subhandle media.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->offset_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle - missing offset table.\n",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "%s: invalid string.\n",
		 function );

		return( -1 );
	}
	if( length < LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 )
	{
		LIBEWF_VERBOSE_PRINT( "%s: string too small.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->segment_table_build == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: segment table was not build.\n",
		 function );

		return( -1 );
	}
	if( internal_handle->calculated_md5_hash == NULL )
	{
		data = (uint8_t *) libewf_common_alloc( internal_handle->media->chunk_size * sizeof( uint8_t ) );

		if( data == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to allocate data.\n",
			 function );

			return( -1 );
		}
		for( iterator = 0; iterator <= internal_handle->offset_table->last; iterator++ )
		{
			offset = iterator * internal_handle->media->chunk_size;
			count  = libewf_read_random(
			          handle,
			          data,
			          internal_handle->media->chunk_size,
			          offset );

			if( count == -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to read chunk.\n",
				 function );

				libewf_common_free( data );

				return( -1 );
			}
			if( count > (ssize_t) SSIZE_MAX )
			{
				LIBEWF_WARNING_PRINT( "%s: invalid count value exceeds maximum.\n",
				 function );

				libewf_common_free( data );

				return( -1 );
			}
		}
		libewf_common_free( data ) ;
	}
	else
	{
		LIBEWF_VERBOSE_PRINT( "%s: MD5 hash already calculated.\n",
		 function );
	}
	result = libewf_string_copy_from_digest_hash(
	          string,
	          length,
	          internal_handle->calculated_md5_hash,
	          EWF_DIGEST_HASH_SIZE_MD5 );

	if( result != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set MD5 hash string.\n",
		 function );
	}
	return( (int8_t) result );
}

/* Creates a printable string of the stored md5 hash
 * Returns 1 if successful, 0 if no md5 hash is stored, -1 on error
 */
int8_t libewf_get_stored_md5_hash( LIBEWF_HANDLE *handle, LIBEWF_CHAR *string, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_stored_md5_hash";
	int8_t result                           = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->stored_md5_hash == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "%s: MD5 hash was not set.\n",
		 function );

		return( 0 );
	}
	result = libewf_string_copy_from_digest_hash(
	          string,
	          length,
	          internal_handle->stored_md5_hash,
	          EWF_DIGEST_HASH_SIZE_MD5 );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create MD5 hash string.\n",
		 function );
	}
	return( result );
}

/* Creates a printable string of the calculated md5 hash
 * Returns 1 if successful, 0 if no md5 hash is calculated, -1 on error
 */
int8_t libewf_get_calculated_md5_hash( LIBEWF_HANDLE *handle, LIBEWF_CHAR *string, size_t length )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	static char *function                   = "libewf_get_calculated_md5_hash";
	int8_t result                           = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->calculated_md5_hash == NULL )
	{
		LIBEWF_VERBOSE_PRINT( "%s: MD5 hash was not set.\n",
		 function );

		return( 0 );
	}
	result = libewf_string_copy_from_digest_hash(
	          string,
	          length,
	          ( (LIBEWF_INTERNAL_HANDLE *) handle )->calculated_md5_hash,
	          EWF_DIGEST_HASH_SIZE_MD5 );

	if( result == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create MD5 hash string.\n",
		 function );
	}
	return( result );
}

/* Parses the header values from the xheader, header2 or header section
 * Will parse the first available header in order mentioned above
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_parse_header_values( LIBEWF_HANDLE *handle, uint8_t date_format )
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
int8_t libewf_parse_hash_values( LIBEWF_HANDLE *handle )
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
int8_t libewf_add_acquiry_error( LIBEWF_HANDLE *handle, off64_t sector, uint32_t amount_of_sectors )
{
	return( libewf_internal_handle_add_acquiry_error_sector(
	         (LIBEWF_INTERNAL_HANDLE *) handle,
	         sector,
	         amount_of_sectors ) );
}

/* Set the notify values
 */
void libewf_set_notify_values( FILE *stream, uint8_t verbose )
{
	libewf_notify_set_values( stream, verbose );
}


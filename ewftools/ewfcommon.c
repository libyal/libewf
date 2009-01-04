/*
 * ewfcommon
 * Common functions for the ewf tools
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
#include <file_io.h>
#include <memory.h>
#include <notify.h>
#include <system_string.h>
#include <types.h>

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

#if defined( HAVE_ZLIB_H ) && defined( HAVE_LIBZ )
#include <zlib.h>
#endif

#if defined( HAVE_OPENSSL_OPENSSLV_H ) && defined( HAVE_LIBCRYPTO )
#include <openssl/opensslv.h>
#endif

#if defined( HAVE_UUID_UUID_H ) && defined( HAVE_LIBUUID )
#include <uuid/uuid.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "../libewf/ewf_digest_hash.h"

#include "ewfcommon.h"
#include "ewfstring.h"

/* EWFCOMMON_BUFFER_SIZE definition is intended for testing purposes
 */
#if !defined( EWFCOMMON_BUFFER_SIZE )
#define EWFCOMMON_BUFFER_SIZE	chunk_size
#endif

#if !defined( LIBEWF_OPERATING_SYSTEM )
#define LIBEWF_OPERATING_SYSTEM "Unknown"
#endif

int ewfcommon_abort                    = 0;
LIBEWF_HANDLE *ewfcommon_libewf_handle = NULL;

/* Signal handler for ewftools
 */
void ewfcommon_signal_handler(
      ewfsignal_t signal )
{
	static char *function = "ewfcommon_signal_handler";

	ewfcommon_abort = 1;

	if( ( ewfcommon_libewf_handle != NULL )
	 && ( libewf_signal_abort(
	       ewfcommon_libewf_handle ) != 1 ) )
	{
		notify_warning_printf( "%s: unable to signal libewf to abort.\n",
		 function );
	}
}

/* Swaps the byte order of byte pairs within a buffer of a certain size
 * Returns 1 if successful, -1 on error
 */
int ewfcommon_swap_byte_pairs(
     uint8_t *buffer,
     size_t size )
{
	static char *function = "ewfcommon_swap_byte_pairs";
	uint8_t byte          = 0;
	size_t iterator       = 0;

	if( buffer == NULL )
	{
		notify_warning_printf( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < size; iterator += 2 )
	{
		byte                   = buffer[ iterator ];
		buffer[ iterator ]     = buffer[ iterator + 1 ];
		buffer[ iterator + 1 ] = byte;
	}
	return( 1 );
}

/* Determines the current platform, or NULL on error
 */
character_t *ewfcommon_determine_operating_system(
              void )
{
	character_t *string    = NULL;
	char *operating_system = NULL;
	size_t length          = 0;

#if defined( HAVE_SYS_UTSNAME_H )
	struct utsname utsname_buffer;

	/* Determine the operating system
	 */
	if( uname(
	     &utsname_buffer ) == 0 )
	{
		operating_system = utsname_buffer.sysname;
	}
	else
	{
		operating_system = "Undetermined";
	}
#else
	operating_system = LIBEWF_OPERATING_SYSTEM;
#endif
	length = 1 + strlen(
	              operating_system );

	string = (character_t *) memory_allocate(
	                          sizeof( character_t ) * length );

	if( ( string != NULL )
	 && ( string_copy_from_char(
	       string,
	       operating_system,
	       length ) != 1 ) )
	{
		memory_free(
	         string );
	
		return( NULL );
	}
	return( string );
}

/* Determines the GUID
 * Returns 1 if successful, or -1 on error
 */
int8_t ewfcommon_determine_guid(
        uint8_t *guid,
        uint8_t libewf_format )
{
	static char *function = "ewfcommon_determine_guid";

	if( guid == NULL )
	{
		notify_warning_printf( "%s: invalid GUID.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_UUID_UUID_H ) && defined( HAVE_LIBUUID )
#if defined( HAVE_UUID_GENERATE_RANDOM )
	if( ( libewf_format == LIBEWF_FORMAT_ENCASE5 )
	 || ( libewf_format == LIBEWF_FORMAT_ENCASE6 )
	 || ( libewf_format == LIBEWF_FORMAT_EWFX ) )
	{
		uuid_generate_random(
		 guid );
	}
#endif
#if defined( HAVE_UUID_GENERATE_TIME )
	if( ( libewf_format == LIBEWF_FORMAT_LINEN5 )
	 || ( libewf_format == LIBEWF_FORMAT_LINEN6 ) )
	{
		uuid_generate_time(
		 guid );
	}
#endif
#endif
	return( 1 );
}

/* Initialize the libewf handle for writing
 * Returns 1 if successful, or -1 on error
 */
int ewfcommon_initialize_write(
     LIBEWF_HANDLE *handle,
     character_t *case_number,
     character_t *description,
     character_t *evidence_number,
     character_t *examiner_name,
     character_t *notes,
     character_t *acquiry_operating_system,
     character_t *acquiry_software,
     character_t *acquiry_software_version,
     uint8_t media_type,
     uint8_t volume_type,
     int8_t compression_level,
     uint8_t compress_empty_block,
     uint8_t libewf_format,
     size64_t segment_file_size,
     uint32_t sector_error_granularity )
{
#if defined( HAVE_UUID_UUID_H ) && defined( HAVE_LIBUUID )
	uint8_t guid[ 16 ];
#endif
	static char *function = "ewfcommon_ewfcommon_initialize_write";
	size_t string_length  = 0;

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	/* Set case number
	 */
	if( case_number == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = string_length(
		                 case_number );
	}
	if( libewf_set_header_value_case_number(
	     handle,
	     case_number,
	     string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header value case number in handle.\n",
		 function );

		return( -1 );
	}
	/* Set description
	 */
	if( description == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = string_length(
		                 description );
	}
	if( libewf_set_header_value_description(
	     handle,
	     description,
	     string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header value description in handle.\n",
		 function );

		return( -1 );
	}
	/* Set evidence number
	 */
	if( evidence_number == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = string_length(
		                 evidence_number );
	}
	if( libewf_set_header_value_evidence_number(
	     handle,
	     evidence_number,
	     string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header value evidence number in handle.\n",
		 function );

		return( -1 );
	}
	/* Set examiner name
	 */
	if( examiner_name == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = string_length(
		                 examiner_name );
	}
	if( libewf_set_header_value_examiner_name(
	     handle,
	     examiner_name,
	     string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header value examiner name in handle.\n",
		 function );

		return( -1 );
	}
	/* Set notes
	 */
	if( notes == NULL )
	{
		string_length = 0;
	}
	else
	{
		string_length = string_length(
		                 notes );
	}
	if( libewf_set_header_value_notes(
	     handle,
	     notes,
	     string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header value notes in handle.\n",
		 function );

		return( -1 );
	}
	/* Password is not used within libewf
	 */

	/* Acquiry date, system date and compression type will be generated automatically when set to NULL
	 */

	if( ( acquiry_operating_system != NULL )
	 && ( libewf_set_header_value_acquiry_operating_system(
	       handle,
	       acquiry_operating_system,
	       string_length(
	        acquiry_operating_system ) ) != 1 ) )
	{
		notify_warning_printf( "%s: unable to set header value acquiry operating system in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_header_value(
	     handle,
	     _CHARACTER_T_STRING( "acquiry_software" ),
	     acquiry_software,
	     10 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header value acquiry software in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_header_value_acquiry_software_version(
	     handle,
	     acquiry_software_version,
	     string_length(
	      acquiry_software_version ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header value acquiry software version number in handle.\n",
		 function );

		return( -1 );
	}
	/* Format needs to be set before segment file size
	 */
	if( libewf_set_format(
	     handle,
	     libewf_format ) != 1 )
	{
		notify_warning_printf( "%s: unable to set format in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_segment_file_size(
	     handle,
	     segment_file_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set segment file size in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_error_granularity(
	     handle,
	     sector_error_granularity ) != 1 )
	{
		notify_warning_printf( "%s: unable to set error granularity in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_media_type(
	     handle,
	     media_type ) != 1 )
	{
		notify_warning_printf( "%s: unable to set media type in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_volume_type(
	     handle,
	     volume_type ) != 1 )
	{
		notify_warning_printf( "%s: unable to set volume type in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_compression_values(
	     handle,
	     compression_level,
	     compress_empty_block ) != 1 )
	{
		notify_warning_printf( "%s: unable to set compression values in handle.\n",
		 function );

		return( -1 );
	}
#if defined(HAVE_UUID_UUID_H) && defined(HAVE_LIBUUID)
	/* Add a system GUID if necessary
	 */
	if( ewfcommon_determine_guid(
	     guid,
	     libewf_format ) != 1 )
	{
		notify_warning_printf( "%s: unable to create GUID.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_guid(
	     handle,
	     guid,
	     16 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set GUID in handle.\n",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Reads data from a file descriptor into the chunk cache
 * Returns the amount of bytes read, 0 if at end of input, or -1 on error
 */
ssize32_t ewfcommon_read_input(
           LIBEWF_HANDLE *handle,
           int file_descriptor,
           uint8_t *buffer,
           size_t buffer_size,
           size32_t chunk_size,
           uint32_t bytes_per_sector,
           ssize64_t total_read_count,
           size64_t total_input_size,
           uint8_t read_error_retry,
           uint32_t sector_error_granularity,
           uint8_t wipe_chunk_on_error,
           uint8_t seek_on_error )
{
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
	system_character_t *error_string  = NULL;
#endif
	static char *function             = "ewfcommon_read_input";
	off64_t current_read_offset       = 0;
	off64_t current_calculated_offset = 0;
	off64_t error2_sector             = 0;
	ssize_t read_count                = 0;
	ssize_t buffer_offset             = 0;
	size_t read_size                  = 0;
	size_t bytes_to_read              = 0;
	size_t read_remaining_bytes       = 0;
	size_t error_remaining_bytes      = 0;
	int32_t read_amount_of_errors     = 0;
	uint32_t chunk_amount             = 0;
	uint32_t read_error_offset        = 0;
	uint32_t error_skip_bytes         = 0;
	uint32_t error_granularity_offset = 0;
	uint32_t error2_amount_of_sectors = 0;
	uint32_t acquiry_amount_of_errors = 0;
	uint32_t byte_error_granularity   = 0;

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		notify_warning_printf( "%s: invalid read buffer.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		notify_warning_printf( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid buffer size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( total_read_count <= -1 )
	{
		notify_warning_printf( "%s: invalid total read count.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_write_amount_of_chunks(
	     handle,
	     &chunk_amount ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine amount of chunks written.\n",
		 function );

		return( -1 );
	}
	while( buffer_size > 0 )
	{
		/* Determine the amount of bytes to read from the input
		 * Read as much as possible in chunk sizes
		 */
		if( buffer_size < (size_t) chunk_size )
		{
			read_size = buffer_size;
		}
		else
		{
			read_size = chunk_size;
		}
		bytes_to_read = read_size;

		while( read_amount_of_errors <= read_error_retry )
		{
			read_count = file_io_read(
			              file_descriptor,
			              &( buffer[ buffer_offset + read_error_offset ] ),
			              bytes_to_read );

			notify_verbose_printf( "%s: read chunk: %" PRIi32 " with size: %" PRIzd ".\n",
			 function, ( chunk_amount + 1 ), read_count );

			current_calculated_offset = (off64_t) ( total_read_count + buffer_offset + read_error_offset );

			if( read_count <= -1 )
			{
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
				if( ( errno == ESPIPE )
				 || ( errno == EPERM )
				 || ( errno == ENXIO )
				 || ( errno == ENODEV ) )
				{
					error_string = ewfcommon_strerror(
					                errno );

					if( error_string != NULL )
					{
						notify_warning_printf( "%s: error reading data: %s.\n",
						 function, error_string );

						memory_free(
						 error_string );
					}
					return( -1 );
				}
#else
				if( errno == ESPIPE )
				{
					notify_warning_printf( "%s: error reading data: invalid seek.\n",
					 function );

					return( -1 );
				}
				else if( errno == EPERM )
				{
					notify_warning_printf( "%s: error reading data: operation not permitted.\n",
					 function );

					return( -1 );
				}
				else if( errno == ENXIO )
				{
					notify_warning_printf( "%s: error reading data: no such device or address.\n",
					 function );

					return( -1 );
				}
				else if( errno == ENODEV )
				{
					notify_warning_printf( "%s: error reading data: no such device.\n",
					 function );

					return( -1 );
				}
#endif
				if( seek_on_error == 1 )
				{
					current_read_offset = file_io_lseek(
					                       file_descriptor,
					                       0,
					                       SEEK_CUR );

					if( current_read_offset != current_calculated_offset )
					{
						notify_verbose_printf( "%s: correcting offset drift current: %" PRIjd ", calculated: %" PRIjd ".\n",
						 function, current_read_offset, current_calculated_offset );

						if( current_read_offset < current_calculated_offset )
						{
							notify_warning_printf( "%s: unable to correct offset drift.\n",
							 function );

							return( -1 );
						}
						read_count         = (ssize_t) ( current_read_offset - current_calculated_offset );
						read_error_offset += read_count;
						bytes_to_read     -= read_count;
					}
				}
			}
			else
			{
				/* The last read is OK, correct read_count
				 */
				if( read_count == (ssize_t) bytes_to_read )
				{
					read_count = read_error_offset + bytes_to_read;
				}
				/* The entire read is OK
				 */
				if( read_count == (ssize_t) read_size )
				{
					break;
				}
				/* If no end of input can be determined
				 */
				if( total_input_size == 0 )
				{
					/* If some bytes were read it is possible that the end of the input reached
					 */
					if( read_count > 0 )
					{
						return( (ssize32_t) ( buffer_offset + read_count ) );
					}
				}
				else
				{
					/* Check if the end of the input was reached
					 */
					if( ( total_read_count + buffer_offset + read_count ) >= (ssize64_t) total_input_size )
					{
						break;
					}
				}
				/* No bytes were read
				 */
				if( read_count == 0 )
				{
					return( 0 );
				}
				notify_verbose_printf( "%s: read error at offset %" PRIjd " after reading %" PRIzd " bytes.\n",
				 function, current_calculated_offset, read_count );

				/* There was a read error at a certain offset
				 */
				read_error_offset += read_count;
				bytes_to_read     -= read_count;
			}
			read_amount_of_errors++;

			if( read_amount_of_errors > read_error_retry )
			{
				if( seek_on_error == 0 )
				{
					notify_verbose_printf( "%s: unable to handle more input.\n",
					 function );

					return( 0 );
				}
				current_calculated_offset = total_read_count + buffer_offset;

				/* Check if last chunk is smaller than the chunk size and take corrective measures
				 */
				if( ( total_input_size != 0 )
				 && ( ( current_calculated_offset + chunk_size ) > (int64_t) total_input_size ) )
				{
					read_remaining_bytes = (size_t) ( total_input_size - current_calculated_offset );
				}
				else
				{
					read_remaining_bytes = (size_t) chunk_size;
				}
				if( read_remaining_bytes > (size_t) SSIZE_MAX )
				{
					notify_verbose_printf( "%s: invalid remaining bytes value exceeds maximum.\n",
					 function );

					return( -1 );
				}
				byte_error_granularity   = sector_error_granularity * bytes_per_sector;
				error_remaining_bytes    = read_remaining_bytes - read_error_offset;
				error2_sector            = current_calculated_offset;
				error_granularity_offset = ( read_error_offset / byte_error_granularity ) * byte_error_granularity;
				error_skip_bytes         = ( error_granularity_offset + byte_error_granularity ) - read_error_offset;

				if( wipe_chunk_on_error == 1 )
				{
					notify_verbose_printf( "%s: wiping block of %" PRIu32 " bytes at offset %" PRIu32 ".\n",
					 function, byte_error_granularity, error_granularity_offset );

					if( memory_set(
					     &buffer[ error_granularity_offset ],
					     0,
					     byte_error_granularity ) == NULL )
					{
						notify_warning_printf( "%s: unable to wipe data in chunk on error.\n",
						 function );

						return( -1 );
					}
					error2_sector            += error_granularity_offset;
					error2_amount_of_sectors  = byte_error_granularity;
				}
				else
				{
					notify_verbose_printf( "%s: wiping remainder of chunk at offset %" PRIu32 ".\n",
					 function, read_error_offset );

					if( memory_set(
					     &buffer[ read_error_offset ],
					     0,
					     error_skip_bytes ) == NULL )
					{
						notify_warning_printf( "%s: unable to wipe data in chunk on error.\n",
						 function );

						return( -1 );
					}
					error2_sector            += read_error_offset;
					error2_amount_of_sectors  = error_skip_bytes;
				}
				error2_sector            /= bytes_per_sector;
				error2_amount_of_sectors /= bytes_per_sector;

				if( libewf_add_acquiry_error( handle, error2_sector, error2_amount_of_sectors ) != 1 )
				{
					notify_warning_printf( "%s: unable to add acquiry read errror sectors.\n",
					 function );

					return( -1 );
				}
				acquiry_amount_of_errors++;

				notify_verbose_printf( "%s: adding error2: %" PRIu32 " sector: %" PRIu64 ", count: %" PRIu32 ".\n",
				 function, acquiry_amount_of_errors, error2_sector, error2_amount_of_sectors );

				notify_verbose_printf( "%s: skipping %" PRIu32 " bytes.\n",
				 function, error_skip_bytes );

				/* At the end of the input
				 */
				if( ( total_input_size != 0 )
				 && ( ( current_calculated_offset + (int64_t) read_remaining_bytes ) >= (int64_t) total_input_size ) )
				{
					notify_verbose_printf( "%s: at end of input no remaining bytes to read from chunk.\n",
					 function );

					read_count = (ssize_t) read_remaining_bytes;

					break;
				}
				if( file_io_lseek(
				     file_descriptor,
				     error_skip_bytes,
				     SEEK_CUR ) == -1 )
				{
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
					error_string = ewfcommon_strerror(
					                errno );

					if( error_string != NULL )
					{
						notify_warning_printf( "%s: unable skip %" PRIu32 " bytes after sector with error - %s.\n",
						 function, error_skip_bytes, error_string );

						memory_free(
						 error_string );
					}
#else
					notify_warning_printf( "%s: unable skip %" PRIu32 " bytes after sector with error.\n",
					 function, error_skip_bytes );
#endif
					return( -1 );
				}
				/* If error granularity skip is still within the chunk
				 */
				if( error_remaining_bytes > byte_error_granularity )
				{
					bytes_to_read          = error_remaining_bytes - error_skip_bytes;
					read_error_offset     += error_skip_bytes;
					read_amount_of_errors  = 0;

					notify_verbose_printf( "%s: remaining to read from chunk %" PRIzd " bytes.\n",
					 function, bytes_to_read );
				}
				else
				{
					read_count = (ssize_t) read_remaining_bytes;

					notify_verbose_printf( "%s: no remaining bytes to read from chunk.\n",
					 function );

					break;
				}
			}
		}
		buffer_size   -= read_count;
		buffer_offset += read_count;

		/* At the end of the input
		 */
		if( ( total_input_size != 0 )
		 && ( ( total_read_count + buffer_offset ) >= (int64_t) total_input_size ) )
		{
			break;
		}
		if( ewfcommon_abort != 0 )
		{
			break;
		}
	}
	return( (int32_t) buffer_offset );
}

#if defined( HAVE_RAW_ACCESS )

/* Reads the data from an EWF file
 * using the raw access functions
 * buffer will be set to the buffer containing the uncompressed data
 * Returns the amount of bytes read, 0 if no more data can be read, or -1 on error
 */
ssize_t ewfcommon_raw_read_ewf(
         LIBEWF_HANDLE *handle,
         uint8_t *raw_buffer,
         size_t raw_buffer_size,
         uint8_t **buffer,
         size_t buffer_size,
         size_t read_size,
         off64_t read_offset,
         size64_t media_size,
         uint32_t sectors_per_chunk,
         uint32_t bytes_per_sector,
         uint8_t wipe_chunk_on_error )
{
	static char *function      = "ewfcommon_raw_read_ewf";
	ssize_t raw_read_count     = 0;
	ssize_t read_count         = 0;
	off64_t sector             = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t chunk_crc         = 0;
	int8_t is_compressed       = 0;
	int8_t read_crc            = 0;

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( raw_buffer == NULL )
	{
		notify_warning_printf( "%s: invalid raw buffer.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		notify_warning_printf( "%s: invalid buffer pointer.\n",
		 function );

		return( -1 );
	}
	if( *buffer == NULL )
	{
		notify_warning_printf( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( read_size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid read size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( read_size > buffer_size )
	{
		notify_warning_printf( "%s: invalid read size value exceeds buffer size.\n",
		 function );

		return( -1 );
	}
	raw_read_count = libewf_raw_read_buffer(
			  handle,
			  (void *) raw_buffer,
			  raw_buffer_size,
			  &is_compressed,
			  &chunk_crc,
			  &read_crc );

	if( raw_read_count <= -1 )
	{
		notify_warning_printf( "%s: unable to read chunk from file.\n",
		 function );

		return( -1 );
	}
	read_count = libewf_raw_read_prepare_buffer(
		      handle,
		      (void *) raw_buffer,
		      (size_t) raw_read_count,
		      (void *) *buffer,
		      &buffer_size,
		      is_compressed,
		      chunk_crc,
		      read_crc );

	if( read_count <= -1 )
	{
		notify_verbose_printf( "%s: unable to prepare buffer after raw read.\n",
		 function );

		/* Wipe the chunk if nescessary
		 */
		if( wipe_chunk_on_error != 0 )
		{
			if( memory_set(
			     buffer,
			     0,
			     read_size ) == NULL )
			{
				notify_warning_printf( "%s: unable to wipe buffer.\n",
				 function );

				return( -1 );
			}
		}
		/* Add a CRC error
		 */
		sector            = read_offset / bytes_per_sector;
		amount_of_sectors = sectors_per_chunk;

		if( ( sector + amount_of_sectors ) > (off64_t) ( media_size / bytes_per_sector ) )
		{
			amount_of_sectors = (uint32_t) ( ( media_size / bytes_per_sector ) - sector );
		}
		if( libewf_add_crc_error(
		     handle,
		     sector,
		     amount_of_sectors ) != 1 )
		{
			notify_warning_printf( "%s: unable to set CRC error chunk.\n",
			 function );

			return( -1 );
		}
		return( (ssize_t) read_size );
	}
	if( is_compressed == 0 )
	{
		*buffer = raw_buffer;
	}
	if( read_size != buffer_size )
	{
		notify_warning_printf( "%s: mismatch in read and buffer size.\n",
		 function );

		return( -1 );
	}
	return( (ssize_t) read_size );
}

/* Writes the data to an EWF file
 * using the raw access functions
 * Returns the amount of bytes written, 0 if no more data can be written, or -1 on error
 */
ssize_t ewfcommon_raw_write_ewf(
         LIBEWF_HANDLE *handle,
         uint8_t *raw_buffer,
         size_t raw_buffer_size,
         uint8_t *buffer,
         size_t buffer_size,
         size_t write_size )
{
	static char *function     = "ewfcommon_raw_write_ewf";
	uint8_t *raw_write_buffer = NULL;
	ssize_t raw_write_count   = 0;
	ssize_t write_count       = 0;
	uint32_t chunk_crc        = 0;
	int8_t is_compressed      = 0;
	int8_t write_crc          = 0;

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( raw_buffer == NULL )
	{
		notify_warning_printf( "%s: invalid raw buffer.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		notify_warning_printf( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( write_size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid write size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( write_size > buffer_size )
	{
		notify_warning_printf( "%s: invalid write size value exceeds buffer size.\n",
		 function );

		return( -1 );
	}
	raw_write_count = libewf_raw_write_prepare_buffer(
			   handle,
			   (void *) buffer,
			   write_size,
			   (void *) raw_buffer,
			   &raw_buffer_size,
			   &is_compressed,
			   &chunk_crc,
			   &write_crc );

	if( raw_write_count <= -1 )
	{
		notify_warning_printf( "%s: unable to prepare buffer before raw write.\n",
		 function );

		return( -1 );
	}
	if( is_compressed == 0 )
	{
		raw_write_buffer = buffer;
	}
	else
	{
		raw_write_buffer = raw_buffer;
	}
	write_count = libewf_raw_write_buffer(
		       handle,
		       (void *) raw_write_buffer,
		       (size_t) raw_write_count,
		       write_size,
		       is_compressed,
		       chunk_crc,
		       write_crc );

	if( write_count != raw_write_count )
	{
		notify_warning_printf( "%s: unable to write chunk to file.\n",
		 function );

		return( -1 );
	}
	return( (ssize_t) write_size );
}

#endif

/* Reads the data to calculate the MD5 and SHA1 integrity hashes
 * Returns the amount of bytes read if successful, or -1 on error
 */
ssize64_t ewfcommon_read_verify(
           LIBEWF_HANDLE *handle,
           uint8_t calculate_md5,
           character_t *md5_hash_string,
           size_t md5_hash_string_length,
           uint8_t calculate_sha1,
           character_t *sha1_hash_string,
           size_t sha1_hash_string_length,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( size64_t bytes_read, size64_t bytes_total ) )
{
	EWFMD5_CONTEXT md5_context;
	EWFSHA1_CONTEXT sha1_context;

	ewfdigest_hash_t md5_hash[ EWFDIGEST_HASH_SIZE_MD5 ];
	ewfdigest_hash_t sha1_hash[ EWFDIGEST_HASH_SIZE_SHA1 ];

	uint8_t *data               = NULL;
	uint8_t *uncompressed_data  = NULL;
	static char *function       = "ewfcommon_read_verify";
	off64_t read_offset         = 0;
	size64_t media_size         = 0;
	size32_t chunk_size         = 0;
	size_t buffer_size          = 0;
	size_t read_size            = 0;
	size_t md5_hash_size        = EWFDIGEST_HASH_SIZE_MD5;
	size_t sha1_hash_size       = EWFDIGEST_HASH_SIZE_SHA1;
	ssize64_t total_read_count  = 0;
	ssize_t read_count          = 0;
#if defined( HAVE_RAW_ACCESS )
	uint8_t *raw_read_data      = NULL;
	size_t raw_read_buffer_size = 0;
	uint32_t sectors_per_chunk  = 0;
	uint32_t bytes_per_sector   = 0;
#endif

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( calculate_md5 == 1 )
	{
		if( md5_hash_string == NULL )
		{
			notify_warning_printf( "%s: invalid MD5 hash string.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( sha1_hash_string == NULL )
		{
			notify_warning_printf( "%s: invalid SHA1 hash string.\n",
			 function );

			return( -1 );
		}
	}
	if( libewf_get_media_size(
	     handle,
	     &media_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine media size.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_chunk_size(
	     handle,
	     &chunk_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine chunk size.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		notify_warning_printf( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( chunk_size > (uint32_t) INT32_MAX )
	{
		notify_warning_printf( "%s: invalid chunk size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_read_wipe_chunk_on_error(
	     handle,
	     wipe_chunk_on_error ) != 1 )
	{
		notify_warning_printf( "%s: unable to set wipe chunk on error.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_RAW_ACCESS )
	if( libewf_get_sectors_per_chunk(
	     handle,
	     &sectors_per_chunk ) != 1 )
	{
		notify_warning_printf( "%s: unable to get sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_bytes_per_sector(
	     handle,
	     &bytes_per_sector ) != 1 )
	{
		notify_warning_printf( "%s: unable to get bytes per sectors.\n",
		 function );

		return( -1 );
	}
#endif
	if( calculate_md5 == 1 )
	{
		if( ewfmd5_initialize(
		     &md5_context ) != 1 )
		{
			notify_warning_printf( "%s: unable to initialize MD5 digest context.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_initialize(
		     &sha1_context ) != 1 )
		{
			notify_warning_printf( "%s: unable to initialize SHA1 digest context.\n",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_RAW_ACCESS )
	buffer_size = chunk_size;
#else
	buffer_size = EWFCOMMON_BUFFER_SIZE;
#endif
	data = (uint8_t *) memory_allocate(
	                    sizeof( uint8_t ) * buffer_size );

	if( data == NULL )
	{
		notify_warning_printf( "%s: unable to allocate data.\n",
		 function );

		return( -1 );
	} 
#if defined( HAVE_RAW_ACCESS )
	/* The EWF-S01 format uses compression this will add bytes
	 */
	raw_read_buffer_size = buffer_size * 2;
	raw_read_data        = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * raw_read_buffer_size );

	if( raw_read_data == NULL )
	{
		notify_warning_printf( "%s: unable to allocate raw read data.\n",
		 function );

		memory_free(
		 data );

		return( -1 );
	}
#endif

	while( total_read_count < (ssize64_t) media_size )
	{
		read_size = buffer_size;

		if( ( media_size - total_read_count ) < read_size )
		{
			read_size = (size_t) ( media_size - total_read_count );
		}
		uncompressed_data = data;

#if defined( HAVE_RAW_ACCESS )
		read_count = ewfcommon_raw_read_ewf(
		              handle,
		              raw_read_data,
		              raw_read_buffer_size,
		              &uncompressed_data,
		              buffer_size,
		              read_size,
		              read_offset,
		              media_size,
		              sectors_per_chunk,
		              bytes_per_sector,
		              wipe_chunk_on_error );
#else
		read_count = libewf_read_random(
		              handle,
		              (void *) uncompressed_data,
		              read_size,
		              read_offset );
#endif
		if( read_count <= -1 )
		{
			notify_warning_printf( "%s: unable to read data from file.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		if( read_count == 0 )
		{
			notify_warning_printf( "%s: unexpected end of data.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		if( read_count > (ssize_t) read_size )
		{
			notify_warning_printf( "%s: more bytes read than requested.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		/* Digest hashes are calcultated before swap
		 */
		if( calculate_md5 == 1 )
		{
			ewfmd5_update(
			 &md5_context,
			 uncompressed_data,
			 read_count );
		}
		if( calculate_sha1 == 1 )
		{
			ewfsha1_update(
			 &sha1_context,
			 uncompressed_data,
			 read_count );
		}
		/* Swap byte pairs
		 */
		if( ( swap_byte_pairs == 1 )
		 && ( ewfcommon_swap_byte_pairs(
		       uncompressed_data,
		       read_count ) != 1 ) )
		{
			notify_warning_printf( "%s: unable to swap byte pairs.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif
			return( -1 );
		}
		read_offset      += (off64_t) read_size;
		total_read_count += (ssize64_t) read_count;

		if( callback != NULL )
		{
			callback(
			 (size64_t) total_read_count,
			 media_size );
		}
		if( ewfcommon_abort != 0 )
		{
			break;
		}
  	}
	memory_free(
	 data );
#if defined( HAVE_RAW_ACCESS )
	memory_free(
	 raw_read_data );
#endif

	if( calculate_md5 == 1 )
	{
		if( ewfmd5_finalize( &md5_context, md5_hash, &md5_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash.\n",
			 function );

			return( -1 );
		}
		if( ewfdigest_copy_to_string(
		     md5_hash,
		     md5_hash_size,
		     md5_hash_string,
		     md5_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash string.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_finalize( &sha1_context, sha1_hash, &sha1_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set SHA1 hash.\n",
			 function );

			return( -1 );
		}
		if( ewfdigest_copy_to_string(
		     sha1_hash,
		     sha1_hash_size,
		     sha1_hash_string,
		     sha1_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set SHA1 hash string.\n",
			 function );

			return( -1 );
		}
	}
	return( total_read_count );
}

/* Writes data in EWF format from a file descriptor
 * Returns the amount of bytes written, or -1 on error
 */
ssize64_t ewfcommon_write_from_file_descriptor(
           LIBEWF_HANDLE *handle,
           int input_file_descriptor,
           size64_t write_size,
           off64_t write_offset,
           uint32_t sectors_per_chunk,
           uint32_t bytes_per_sector,
           uint8_t read_error_retry,
           uint32_t sector_error_granularity,
           uint8_t wipe_chunk_on_error,
           uint8_t seek_on_error,
           uint8_t calculate_md5,
           character_t *md5_hash_string,
           size_t md5_hash_string_length,
           uint8_t calculate_sha1,
           character_t *sha1_hash_string,
           size_t sha1_hash_string_length,
           uint8_t swap_byte_pairs,
           void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) )
{
	EWFMD5_CONTEXT md5_context;
	EWFSHA1_CONTEXT sha1_context;

	ewfdigest_hash_t md5_hash[ EWFDIGEST_HASH_SIZE_MD5 ];
	ewfdigest_hash_t sha1_hash[ EWFDIGEST_HASH_SIZE_SHA1 ];

	uint8_t *data_buffer        = NULL;
	static char *function       = "ewfcommon_write_from_file_descriptor";
	size32_t chunk_size         = 0;
	size_t data_buffer_size     = 0;
	size_t md5_hash_size        = EWFDIGEST_HASH_SIZE_MD5;
	size_t sha1_hash_size       = EWFDIGEST_HASH_SIZE_SHA1;
	ssize64_t total_write_count = 0;
	ssize64_t write_count       = 0;
	ssize32_t read_count        = 0;
#if defined( HAVE_RAW_ACCESS )
	uint8_t *raw_data_buffer    = NULL;
	size_t raw_data_buffer_size = 0;
#endif

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( calculate_md5 == 1 )
	{
		if( md5_hash_string == NULL )
		{
			notify_warning_printf( "%s: invalid MD5 hash string.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( sha1_hash_string == NULL )
		{
			notify_warning_printf( "%s: invalid SHA1 hash string.\n",
			 function );

			return( -1 );
		}
	}
	if( input_file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_sectors_per_chunk(
	     handle,
	     sectors_per_chunk ) == -1 )
	{
		notify_warning_printf( "%s: unable to set sectors per chunk in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_bytes_per_sector(
	     handle,
	     bytes_per_sector ) == -1 )
	{
		notify_warning_printf( "%s: unable to set bytes per sector in handle.\n",
		 function );

		return( -1 );
	}
	if( write_size > 0 )
	{
		if( libewf_set_media_size(
		     handle,
		     write_size ) == -1 )
		{
			notify_warning_printf( "%s: unable to set media size in handle.\n",
			 function );

			return( -1 );
		}
		if( write_offset > 0 )
		{
			if( write_offset >= (off64_t) write_size )
			{
				notify_warning_printf( "%s: invalid offset to write.\n",
				 function );

				return( -1 );
			}
			if( file_io_lseek(
			     input_file_descriptor,
			     write_offset,
			     SEEK_SET ) != (off64_t) write_offset )
			{
				notify_warning_printf( "%s: unable to find write offset.\n",
				 function );

				return( -1 );
			}
		}
	}
	else if( write_offset > 0 )
	{
		notify_warning_printf( "%s: ignoring write offset in a stream mode.\n",
		 function );
	}
	chunk_size = sectors_per_chunk * bytes_per_sector;

	if( ( chunk_size == 0 )
	 || ( chunk_size > (size32_t) INT32_MAX ) )
	{
		notify_warning_printf( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( calculate_md5 == 1 )
	{
		if( ewfmd5_initialize(
		     &md5_context ) != 1 )
		{
			notify_warning_printf( "%s: unable to initialize MD5 digest context.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_initialize(
		     &sha1_context ) != 1 )
		{
			notify_warning_printf( "%s: unable to initialize SHA1 digest context.\n",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_RAW_ACCESS )
	data_buffer_size = chunk_size;
#else
	data_buffer_size = EWFCOMMON_BUFFER_SIZE;
#endif
	data_buffer = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * data_buffer_size );

	if( data_buffer == NULL )
	{
		notify_warning_printf( "%s: unable to allocate data buffer.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_RAW_ACCESS )
	raw_data_buffer_size = data_buffer_size * 2;
	raw_data_buffer      = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * raw_data_buffer_size );

	if( raw_data_buffer == NULL )
	{
		notify_warning_printf( "%s: unable to allocate compressed raw data buffer.\n",
		 function );

		memory_free(
		 data_buffer );

		return( -1 );
	}
#endif
	while( ( write_size == 0 )
	 || ( total_write_count < (int64_t) write_size ) )
	{
		/* Read a chunk from the file descriptor
		 */
		read_count = ewfcommon_read_input(
		              handle,
		              input_file_descriptor,
		              data_buffer,
		              data_buffer_size,
		              chunk_size,
		              bytes_per_sector,
		              total_write_count,
		              write_size,
		              read_error_retry,
		              sector_error_granularity,
		              wipe_chunk_on_error,
		              seek_on_error );

		if( read_count <= -1 )
		{
			notify_warning_printf( "%s: error reading data from input.\n",
			 function );

			memory_free(
			 data_buffer );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_data_buffer );
#endif

			return( -1 );
		}
		if( read_count == 0 )
		{
			if( write_size != 0 )
			{
				notify_warning_printf( "%s: unexpected end of input.\n",
				 function );

				memory_free(
				 data_buffer );
#if defined( HAVE_RAW_ACCESS )
				memory_free(
				 raw_data_buffer );
#endif
				return( -1 );
			}
			break;
		}
		/* Swap byte pairs
		 */
		if( ( swap_byte_pairs == 1 )
		 && ( ewfcommon_swap_byte_pairs( data_buffer, read_count ) != 1 ) )
		{
			notify_warning_printf( "%s: unable to swap byte pairs.\n",
			 function );

			return( -1 );
		}
		/* Digest hashes are calcultated after swap
		 */
		if( calculate_md5 == 1 )
		{
			ewfmd5_update(
			 &md5_context,
			 data_buffer,
			 read_count );
		}
		if( calculate_sha1 == 1 )
		{
			ewfsha1_update(
			 &sha1_context,
			 data_buffer,
			 read_count );
		}
#if defined( HAVE_RAW_ACCESS )
		write_count = ewfcommon_raw_write_ewf(
		               handle,
		               raw_data_buffer,
		               raw_data_buffer_size,
		               data_buffer,
		               data_buffer_size,
		               read_count );
#else
		write_count = libewf_write_buffer(
		               handle,
		               (void *) data_buffer,
		               read_count );
#endif

		if( write_count != read_count )
		{
			notify_warning_printf( "%s: unable to write data to file.\n",
			 function );

			memory_free(
			 data_buffer );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_data_buffer );
#endif

			return( -1 );
		}
		total_write_count += read_count;

		/* Callback for status update
		 */
		if( callback != NULL )
		{
			callback(
		         (size64_t) total_write_count,
		         write_size );
		}
		if( ewfcommon_abort != 0 )
		{
			break;
		}
	}
	memory_free(
	 data_buffer );
#if defined( HAVE_RAW_ACCESS )
	memory_free(
	 raw_data_buffer );
#endif

	if( calculate_md5 == 1 )
	{
		if( ewfmd5_finalize(
		     &md5_context,
		     md5_hash,
		     &md5_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash.\n",
			 function );

			return( -1 );
		}
		if( ewfdigest_copy_to_string(
		     md5_hash,
		     md5_hash_size,
		     md5_hash_string,
		     md5_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash string.\n",
			 function );

			return( -1 );
		}
		/* The MD5 hash must be set before write finalized is used
		 */
		if( libewf_set_md5_hash(
		     handle,
		     md5_hash,
		     md5_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash in handle.\n",
			 function );

			return( -1 );
		}
		/* The MD5 hash string must be set before write finalized is used
		 */
		if( libewf_set_hash_value_md5(
		     handle,
		     md5_hash_string,
		     md5_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash string in handle.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_finalize(
		     &sha1_context,
		     sha1_hash,
		     &sha1_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set SHA1 hash.\n",
			 function );

			return( -1 );
		}
		if( ewfdigest_copy_to_string(
		     sha1_hash,
		     sha1_hash_size,
		     sha1_hash_string,
		     sha1_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set SHA1 hash string.\n",
			 function );

			return( -1 );
		}
		/* The SHA1 hash string must be set before write finalized is used
		 */
		if( libewf_set_hash_value_sha1(
		     handle,
		     sha1_hash_string,
		     sha1_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set SHA1 hash string in handle.\n",
			 function );

			return( -1 );
		}
	}
	write_count = libewf_write_finalize( handle );

	if( write_count == -1 )
	{
		notify_warning_printf( "%s: unable to finalize EWF file(s).\n",
		 function );

		return( -1 );
	}
	total_write_count += write_count;

	return( total_write_count );
}

/* Reads the media data and exports it in raw format
 * Returns a -1 on error, the amount of bytes read on success
 */
ssize64_t ewfcommon_export_raw(
           LIBEWF_HANDLE *handle,
           system_character_t *target_filename,
           size64_t export_size,
           off64_t read_offset,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( size64_t bytes_read, size64_t bytes_total ) )
{
	uint8_t *data               = NULL;
	uint8_t *uncompressed_data  = NULL;
	static char *function       = "ewfcommon_export_raw";
	size64_t media_size         = 0;
	size32_t chunk_size         = 0;
	size_t read_size            = 0;
	size_t buffer_size          = 0;
	ssize64_t total_read_count  = 0;
	ssize_t read_count          = 0;
	ssize_t write_count         = 0;
	uint8_t read_all            = 0;
	int file_descriptor         = -1;
#if defined( HAVE_RAW_ACCESS )
	uint8_t *raw_read_data      = NULL;
	size_t raw_read_buffer_size = 0;
	uint32_t sectors_per_chunk  = 0;
	uint32_t bytes_per_sector   = 0;
#endif

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( target_filename == NULL )
	{
		notify_warning_printf( "%s: invalid target filename.\n",
		 function );

		return( -1 );
	}
	if( system_string_compare(
	     target_filename,
	     _SYSTEM_CHARACTER_T_STRING( "-" ),
	     1 ) == 0 )
	{
		file_descriptor = 1;
	}
	else
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
		file_descriptor = file_io_wopen(
		                   target_filename,
		                   FILE_IO_O_CREAT | FILE_IO_O_WRONLY | FILE_IO_O_TRUNC );
#else
		file_descriptor = file_io_open(
		                   target_filename,
		                   FILE_IO_O_CREAT | FILE_IO_O_WRONLY | FILE_IO_O_TRUNC );
#endif

		if( file_descriptor == -1 )
		{
			notify_warning_printf( "%s: unable to open file: %" PRIs_SYSTEM ".\n",
			 function, target_filename );

			return( -1 );
		}
	}
	if( libewf_get_media_size(
	     handle,
	     &media_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine media size.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_chunk_size(
	     handle,
	     &chunk_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine chunk size.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		notify_warning_printf( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( ( export_size == 0 )
	 || ( export_size > media_size )
	 || ( export_size > (ssize64_t) INT64_MAX ) )
	{
		notify_warning_printf( "%s: invalid size.\n",
		 function );

		return( -1 );
	}
	if( read_offset >= (off64_t) media_size )
	{
		notify_warning_printf( "%s: invalid offset.\n",
		 function );

		return( -1 );
	}
	if( ( export_size + read_offset ) > media_size )
	{
		notify_warning_printf( "%s: unable to export beyond size of media.\n",
		 function );

		return( -1 );
	}
	read_all = (uint8_t) ( ( export_size == media_size ) && ( read_offset == 0 ) );

	if( libewf_set_read_wipe_chunk_on_error(
	     handle,
	     wipe_chunk_on_error ) != 1 )
	{
		notify_warning_printf( "%s: unable to set wipe chunk on error.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_RAW_ACCESS )
	if( libewf_get_sectors_per_chunk(
	     handle,
	     &sectors_per_chunk ) != 1 )
	{
		notify_warning_printf( "%s: unable to get sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_bytes_per_sector(
	     handle,
	     &bytes_per_sector ) != 1 )
	{
		notify_warning_printf( "%s: unable to get bytes per sectors.\n",
		 function );

		return( -1 );
	}
	buffer_size = chunk_size;
#else
	buffer_size = EWFCOMMON_BUFFER_SIZE;
#endif
	data = (uint8_t *) memory_allocate(
	                    sizeof( uint8_t ) * buffer_size );

	if( data == NULL )
	{
		notify_warning_printf( "%s: unable to allocate data.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_RAW_ACCESS )
	/* The EWF-S01 format uses compression this will add bytes
	 */
	raw_read_buffer_size = buffer_size * 2;
	raw_read_data        = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * raw_read_buffer_size );

	if( raw_read_data == NULL )
	{
		notify_warning_printf( "%s: unable to allocate raw read data.\n",
		 function );

		memory_free(
		 data );

		return( -1 );
	}
#endif
	while( total_read_count < (int64_t) export_size )
	{
		read_size = buffer_size;

		if( ( media_size - total_read_count ) < read_size )
		{
			read_size = (size_t) ( media_size - total_read_count );
		}
		uncompressed_data = data;

#if defined( HAVE_RAW_ACCESS )
		read_count = ewfcommon_raw_read_ewf(
		              handle,
		              raw_read_data,
		              raw_read_buffer_size,
		              &uncompressed_data,
		              buffer_size,
		              read_size,
		              read_offset,
		              media_size,
		              sectors_per_chunk,
		              bytes_per_sector,
		              wipe_chunk_on_error );
#else
		read_count = libewf_read_random(
		              handle,
		              (void *) uncompressed_data,
		              read_size,
		              read_offset );
#endif

		if( read_count <= -1 )
		{
			notify_warning_printf( "%s: unable to read data from file.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		if( read_count == 0 )
		{
			notify_warning_printf( "%s: unexpected end of data.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		if( read_count > (ssize_t) read_size )
		{
			notify_warning_printf( "%s: more bytes read than requested.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		read_offset += read_size;

		/* Swap byte pairs
		 */
		if( ( swap_byte_pairs == 1 )
		 && ( ewfcommon_swap_byte_pairs(
		       uncompressed_data,
		       read_count ) != 1 ) )
		{
			notify_warning_printf( "%s: unable to swap byte pairs.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		write_count = file_io_write(
		               file_descriptor,
		               uncompressed_data,
		               (size_t) read_count );

		if( write_count < read_count )
		{
			notify_warning_printf( "%s: error writing data.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
#endif

			return( -1 );
		}
		total_read_count += read_count;

		if( callback != NULL )
		{
			callback( total_read_count, export_size );
		}
		if( ewfcommon_abort != 0 )
		{
			break;
		}
  	}
	memory_free(
	 data );
#if defined( HAVE_RAW_ACCESS )
	memory_free(
	 raw_read_data );
#endif
	return( total_read_count );
}

/* Reads the media data and exports it in EWF format
 * Returns a -1 on error, the amount of bytes read on success
 */
ssize64_t ewfcommon_export_ewf(
           LIBEWF_HANDLE *handle,
           LIBEWF_HANDLE *export_handle,
           int8_t compression_level,
           uint8_t compress_empty_block,
           uint8_t libewf_format,
           size64_t segment_file_size,
           size64_t export_size,
           off64_t read_offset,
           uint32_t export_sectors_per_chunk,
           uint8_t calculate_md5,
           uint8_t calculate_sha1,
           uint8_t swap_byte_pairs,
           uint8_t wipe_chunk_on_error,
           void (*callback)( size64_t bytes_read, size64_t bytes_total ) )
{
#if defined( HAVE_UUID_UUID_H ) && defined( HAVE_LIBUUID )
	uint8_t guid[ 16 ];
#endif
	EWFMD5_CONTEXT md5_context;
	EWFSHA1_CONTEXT sha1_context;

	ewfdigest_hash_t md5_hash[ EWFDIGEST_HASH_SIZE_MD5 ];
	ewfdigest_hash_t sha1_hash[ EWFDIGEST_HASH_SIZE_SHA1 ];

	character_t md5_hash_string[ EWFSTRING_DIGEST_HASH_LENGTH_MD5 ];
	character_t sha1_hash_string[ EWFSTRING_DIGEST_HASH_LENGTH_SHA1 ];

	uint8_t *data                  = NULL;
	uint8_t *uncompressed_data     = NULL;
	static char *function          = "ewfcommon_export_ewf";
	size64_t media_size            = 0;
	size32_t chunk_size            = 0;
	size_t read_size               = 0;
	size_t buffer_size             = 0;
	ssize64_t total_read_count     = 0;
	size_t md5_hash_size           = EWFDIGEST_HASH_SIZE_MD5;
	size_t md5_hash_string_length  = EWFSTRING_DIGEST_HASH_LENGTH_MD5;
	size_t sha1_hash_size          = EWFDIGEST_HASH_SIZE_SHA1;
	size_t sha1_hash_string_length = EWFSTRING_DIGEST_HASH_LENGTH_SHA1;
	ssize_t read_count             = 0;
	ssize_t write_count            = 0;
	uint8_t read_all               = 0;
#if defined( HAVE_RAW_ACCESS )
	uint8_t *raw_read_data         = NULL;
	uint8_t *raw_write_data        = NULL;
	size_t raw_read_buffer_size    = 0;
	size_t raw_write_buffer_size   = 0;
	uint32_t sectors_per_chunk     = 0;
	uint32_t bytes_per_sector      = 0;
#endif

	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return( -1 );
	}
	if( export_handle == NULL )
	{
		notify_warning_printf( "%s: invalid export handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_media_size(
	     handle,
	     &media_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine media size.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_chunk_size(
	     handle,
	     &chunk_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine chunk size.\n",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		notify_warning_printf( "%s: invalid chunk size.\n",
		 function );

		return( -1 );
	}
	if( ( export_size == 0 )
	 || ( export_size > media_size )
	 || ( export_size > (ssize64_t) INT64_MAX ) )
	{
		notify_warning_printf( "%s: invalid size.\n",
		 function );

		return( -1 );
	}
	if( read_offset >= (off64_t) media_size )
	{
		notify_warning_printf( "%s: invalid offset.\n",
		 function );

		return( -1 );
	}
	if( ( export_size + read_offset ) > media_size )
	{
		notify_warning_printf( "%s: unable to export beyond size of media.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_media_size(
	     export_handle,
	     export_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set media size in export handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_parse_header_values(
	     handle,
	     LIBEWF_DATE_FORMAT_ISO8601 ) != 1 )
	{
		notify_warning_printf( "%s: unable to parse header values in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_copy_header_values(
	     export_handle,
	     handle ) != 1 )
	{
		notify_warning_printf( "%s: unable to set copy header values to export handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_copy_media_values(
	     export_handle,
	     handle ) != 1 )
	{
		notify_warning_printf( "%s: unable to set copy media values to export handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_segment_file_size(
	     export_handle,
	     segment_file_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set segment file size in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_compression_values(
	     export_handle,
	     compression_level,
	     (uint8_t) compress_empty_block ) != 1 )
	{
		notify_warning_printf( "%s: unable to set compression values in handle.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_format(
	     export_handle,
	     libewf_format ) != 1 )
	{
		notify_warning_printf( "%s: unable to set format in handle.\n",
		 function );

		return( -1 );
	}
#if defined(HAVE_UUID_UUID_H) && defined(HAVE_LIBUUID)
	/* Add a system GUID if necessary
	 */
	if( ewfcommon_determine_guid(
	     guid,
	     libewf_format ) != 1 )
	{
		notify_warning_printf( "%s: unable to create GUID.\n",
		 function );

		return( -1 );
	}
	if( libewf_set_guid(
	     export_handle,
	     guid,
	     16 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set GUID in handle.\n",
		 function );

		return( -1 );
	}
#endif
	if( libewf_set_sectors_per_chunk(
	     export_handle,
	     (uint32_t) export_sectors_per_chunk ) != 1 )
	{
		notify_warning_printf( "%s: unable to set sectors per chunk in handle.\n",
		 function );

		return( -1 );
	}
	read_all = (uint8_t) ( ( export_size == media_size ) && ( read_offset == 0 ) );

	if( libewf_set_read_wipe_chunk_on_error(
	     handle,
	     wipe_chunk_on_error ) != 1 )
	{
		notify_warning_printf( "%s: unable to set wipe chunk on error.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_RAW_ACCESS )
	if( libewf_get_sectors_per_chunk(
	     handle,
	     &sectors_per_chunk ) != 1 )
	{
		notify_warning_printf( "%s: unable to get sectors per chunk.\n",
		 function );

		return( -1 );
	}
	if( libewf_get_bytes_per_sector(
	     handle,
	     &bytes_per_sector ) != 1 )
	{
		notify_warning_printf( "%s: unable to get bytes per sectors.\n",
		 function );

		return( -1 );
	}
#endif
	if( calculate_md5 == 1 )
	{
		if( ewfmd5_initialize(
		     &md5_context ) != 1 )
		{
			notify_warning_printf( "%s: unable to initialize MD5 digest context.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_initialize(
		     &sha1_context ) != 1 )
		{
			notify_warning_printf( "%s: unable to initialize SHA1 digest context.\n",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_RAW_ACCESS )
	buffer_size = chunk_size;
#else
	buffer_size = EWFCOMMON_BUFFER_SIZE;
#endif
	data = (uint8_t *) memory_allocate(
	                    sizeof( uint8_t ) * buffer_size );

	if( data == NULL )
	{
		notify_warning_printf( "%s: unable to allocate data.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_RAW_ACCESS )
	/* The EWF-S01 format uses compression this will add bytes
	 */
	raw_read_buffer_size = buffer_size * 2;
	raw_read_data        = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * raw_read_buffer_size );

	if( raw_read_data == NULL )
	{
		notify_warning_printf( "%s: unable to allocate raw read data.\n",
		 function );

		memory_free(
		 data );

		return( -1 );
	}
	raw_write_buffer_size = buffer_size * 2;
	raw_write_data        = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * raw_read_buffer_size );

	if( raw_read_data == NULL )
	{
		notify_warning_printf( "%s: unable to allocate raw write data.\n",
		 function );

		memory_free(
		 raw_read_data );
		memory_free(
		 data );

		return( -1 );
	}
#endif
	while( total_read_count < (int64_t) export_size )
	{
		read_size = buffer_size;

		if( ( media_size - total_read_count ) < read_size )
		{
			read_size = (size_t) ( media_size - total_read_count );
		}
		uncompressed_data = data;

#if defined( HAVE_RAW_ACCESS )
		read_count = ewfcommon_raw_read_ewf(
		              handle,
		              raw_read_data,
		              raw_read_buffer_size,
		              &uncompressed_data,
		              buffer_size,
		              read_size,
		              read_offset,
		              media_size,
		              sectors_per_chunk,
		              bytes_per_sector,
		              wipe_chunk_on_error );
#else
		read_count = libewf_read_random(
		              handle,
		              (void *) uncompressed_data,
		              read_size,
		              read_offset );
#endif

		if( read_count <= -1 )
		{
			notify_warning_printf( "%s: unable to read data from file.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
			memory_free(
			 raw_write_data );
#endif

			return( -1 );
		}
		if( read_count == 0 )
		{
			notify_warning_printf( "%s: unexpected end of data.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
			memory_free(
			 raw_write_data );
#endif

			return( -1 );
		}
		if( read_count > (ssize_t) read_size )
		{
			notify_warning_printf( "%s: more bytes read than requested.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
			memory_free(
			 raw_write_data );
#endif

			return( -1 );
		}
		read_offset += read_size;

		/* Swap byte pairs
		 */
		if( ( swap_byte_pairs == 1 )
		 && ( ewfcommon_swap_byte_pairs(
		       uncompressed_data,
		       read_count ) != 1 ) )
		{
			notify_warning_printf( "%s: unable to swap byte pairs.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
			memory_free(
			 raw_write_data );
#endif
			return( -1 );
		}
		/* Digest hashes are calcultated after swap
		 */
		if( calculate_md5 == 1 )
		{
			ewfmd5_update(
			 &md5_context,
			 uncompressed_data,
			 read_count );
		}
		if( calculate_sha1 == 1 )
		{
			ewfsha1_update(
			 &sha1_context,
			 uncompressed_data,
			 read_count );
		}
#if defined( HAVE_RAW_ACCESS )
		write_count = ewfcommon_raw_write_ewf(
		               export_handle,
		               raw_write_data,
		               raw_write_buffer_size,
		               uncompressed_data,
		               buffer_size,
		               read_count );
#else
		write_count = libewf_write_buffer(
		               export_handle,
		               (void *) uncompressed_data,
		               read_count );
#endif

		if( write_count != read_count )
		{
			notify_warning_printf( "%s: unable to write data to file.\n",
			 function );

			memory_free(
			 data );
#if defined( HAVE_RAW_ACCESS )
			memory_free(
			 raw_read_data );
			memory_free(
			 raw_write_data );
#endif

			return( -1 );
		}
		total_read_count += read_count;

		if( callback != NULL )
		{
			callback(
			 total_read_count,
			 export_size );
		}
		if( ewfcommon_abort != 0 )
		{
			break;
		}
  	}
	memory_free(
	 data );
#if defined( HAVE_RAW_ACCESS )
	memory_free(
	 raw_read_data );
	memory_free(
	 raw_write_data );
#endif

	if( calculate_md5 == 1 )
	{
		if( ewfmd5_finalize(
		     &md5_context,
		     md5_hash,
		     &md5_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash.\n",
			 function );

			return( -1 );
		}
		if( ewfdigest_copy_to_string(
		     md5_hash,
		     md5_hash_size,
		     md5_hash_string,
		     md5_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash string.\n",
			 function );

			return( -1 );
		}
		/* The MD5 hash must be set before write finalized is used
		 */
		if( libewf_set_md5_hash(
		     export_handle,
		     md5_hash,
		     md5_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash in handle.\n",
			 function );

			return( -1 );
		}
		/* The MD5 hash string must be set before write finalized is used
		 */
		if( libewf_set_hash_value_md5(
		     export_handle,
		     md5_hash_string,
		     md5_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set MD5 hash string in handle.\n",
			 function );

			return( -1 );
		}
	}
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_finalize(
		     &sha1_context,
		     sha1_hash,
		     &sha1_hash_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set SHA1 hash.\n",
			 function );

			return( -1 );
		}
		if( ewfdigest_copy_to_string(
		     sha1_hash,
		     sha1_hash_size,
		     sha1_hash_string,
		     sha1_hash_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to set SHA1 hash string.\n",
			 function );

			return( -1 );
		}
	}
	write_count = libewf_write_finalize(
	               export_handle );

	if( write_count == -1 )
	{
		notify_warning_printf( "%s: unable to finalize EWF file(s).\n",
		 function );

		return( -1 );
	}
	return( total_read_count );
}


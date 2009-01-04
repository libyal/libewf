/*
 * ewfoutput
 * User output functions for the ewf tools
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
#include <memory.h>

#include "../libewf/libewf_includes.h"

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

#include "../libewf/libewf_common.h"
#include "../libewf/libewf_hash_values.h"
#include "../libewf/libewf_header_values.h"
#include "../libewf/libewf_notify.h"
#include "../libewf/libewf_string.h"

#include "ewfbyte_size_string.h"
#include "ewfdigest_context.h"
#include "ewfmd5.h"
#include "ewfsha1.h"
#include "ewfoutput.h"

#if defined( HAVE_WINDOWS_API )
#define ewfoutput_gmtime_r( timestamp, time_elements ) \
	gmtime_s( time_elements, timestamp )

#elif defined( HAVE_GMTIME_R )
#define ewfoutput_gmtime_r( timestamp, time_elements ) \
	gmtime_r( timestamp, time_elements )

#endif

/* Returns a structured representation of a time using UTC (GMT), or NULL on error
 */
struct tm *ewfoutput_gmtime(
            const time_t *timestamp )
{
#if !defined( ewfoutput_gmtime_r ) && defined( HAVE_GMTIME )
	struct tm *static_time_elements = NULL;
#endif
	struct tm *time_elements        = NULL;
	static char *function           = "ewfoutput_gmtime";

	if( timestamp == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid time stamp.\n",
		 function );

		return( NULL );
	}
	time_elements = (struct tm *) memory_allocate(
	                               sizeof( struct tm ) );

	if( time_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create time elements.\n",
		 function );

		return( NULL );
	}
#if defined( ewfoutput_gmtime_r )
#if defined( HAVE_WINDOWS_API )
	if( ewfoutput_gmtime_r( timestamp, time_elements ) != 0 )
#else
	if( ewfoutput_gmtime_r( timestamp, time_elements ) == NULL )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set time elements.\n",
		 function );

		libewf_common_free( time_elements );

		return( NULL );
	}
	return( time_elements );
#elif defined( HAVE_GMTIME )
	static_time_elements = gmtime( timestamp );

	if( static_time_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create static time elements.\n",
		 function );

		libewf_common_free( time_elements );

		return( NULL );
	}
	if( libewf_common_memcpy( time_elements, static_time_elements, sizeof( struct tm ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set time elements.\n",
		 function );

		libewf_common_free( time_elements );

		return( NULL );
	}
#else
#error Missing equivalent of function gmtime
#endif
}

/* Print the version information to a stream
 */
void ewfoutput_version_fprint(
      FILE *stream,
      libewf_char_t *program )
{
	static char *function = "ewfoutput_version_fprint";

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( program == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid program name.\n",
		 function );

		return;
	}
	fprintf( stream, "%" PRIs_EWF " %" PRIs_EWF " (libewf %" PRIs_EWF ", zlib %s",
	 program, LIBEWF_VERSION_STRING, LIBEWF_VERSION_STRING, ZLIB_VERSION );

#if defined( HAVE_LIBCRYPTO )
	fprintf( stream, ", libcrypto %s", SHLIB_VERSION_NUMBER );
#endif

#if defined( HAVE_LIBUUID )
	fprintf( stream, ", libuuid" );
#endif

	fprintf( stream, ")\n\n" );
}

/* Prints the executable version information
 */
void ewfoutput_copyright_fprint(
      FILE *stream )
{
	static char *function = "ewfoutput_copyright_fprint";

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	fprintf( stream, "Copyright (c) 2006-2008, Joachim Metz, Hoffmann Investigations <%s> and contributors.\n",
	 PACKAGE_BUGREPORT );
	fprintf( stream, "This is free software; see the source for copying conditions. There is NO\n" );
	fprintf( stream, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" );
}

/* Prints an overview of the aquiry parameters
 */
void ewfoutput_acquiry_parameters_fprint(
      FILE *stream,
      CHAR_T *filename,
      libewf_char_t *case_number,
      libewf_char_t *description,
      libewf_char_t *evidence_number,
      libewf_char_t *examiner_name,
      libewf_char_t *notes,
      uint8_t media_type,
      uint8_t volume_type,
      int8_t compression_level,
      uint8_t compress_empty_block,
      uint8_t libewf_format,
      off64_t acquiry_offset,
      size64_t acquiry_size,
      size64_t segment_file_size,
      uint32_t sectors_per_chunk,
      uint32_t sector_error_granularity,
      uint8_t read_error_retry,
      uint8_t wipe_block_on_read_error )
{
	libewf_char_t acquiry_size_string[ 16 ];
	libewf_char_t segment_file_size_string[ 16 ];

	static char *function = "ewfoutput_acquiry_parameters_fprint";
	int result            = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	fprintf( stream, "Image path and filename:\t%" PRIs ".",
	 filename );

	if( libewf_format == LIBEWF_FORMAT_SMART )
	{
		fprintf( stream, "s01\n" );
	}
	else
	{
		fprintf( stream, "E01\n" );
	}
	fprintf( stream, "Case number:\t\t\t" );

	if( case_number != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "",
		 case_number );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Description:\t\t\t" );

	if( description != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "",
		 description );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Evidence number:\t\t" );

	if( evidence_number != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "",
		 evidence_number );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Examiner name:\t\t\t" );

	if( examiner_name != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "",
		 examiner_name );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Notes:\t\t\t\t" );

	if( notes != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "",
		 notes );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Media type:\t\t\t" );

	if( media_type == LIBEWF_MEDIA_TYPE_FIXED )
	{
		fprintf( stream, "fixed\n" );
	}
	else if( media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
	{
		fprintf( stream, "removable\n" );
	}
	fprintf( stream, "Volume type:\t\t\t" );

	if( volume_type == LIBEWF_VOLUME_TYPE_LOGICAL )
	{
		fprintf( stream, "logical\n" );
	}
	else if( volume_type == LIBEWF_VOLUME_TYPE_PHYSICAL )
	{
		fprintf( stream, "physical\n" );
	}
	fprintf( stream, "Compression used:\t\t" );

	if( compression_level == LIBEWF_COMPRESSION_FAST )
	{
		fprintf( stream, "fast\n" );
	}
	else if( compression_level == LIBEWF_COMPRESSION_BEST )
	{
		fprintf( stream, "best\n" );
	}
	else if( compression_level == LIBEWF_COMPRESSION_NONE )
	{
		fprintf( stream, "none\n" );

		fprintf( stream, "Compress empty blocks:\t\t" );

		if( compress_empty_block == 0 )
		{
			fprintf( stream, "no\n" );
		}
		else
		{
			fprintf( stream, "yes\n" );
		}
	}
	fprintf( stream, "EWF file format:\t\t" );

	if( libewf_format == LIBEWF_FORMAT_EWF )
	{
		fprintf( stream, "original EWF\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_SMART )
	{
		fprintf( stream, "SMART\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_FTK )
	{
		fprintf( stream, "FTK Imager\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_ENCASE1 )
	{
		fprintf( stream, "EnCase 1\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_ENCASE2 )
	{
		fprintf( stream, "EnCase 2\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_ENCASE3 )
	{
		fprintf( stream, "EnCase 3\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_ENCASE4 )
	{
		fprintf( stream, "EnCase 4\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_ENCASE5 )
	{
		fprintf( stream, "EnCase 5\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_ENCASE6 )
	{
		fprintf( stream, "EnCase 6\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_LINEN5 )
	{
		fprintf( stream, "linen 5\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_LINEN6 )
	{
		fprintf( stream, "linen 6\n" );
	}
	else if( libewf_format == LIBEWF_FORMAT_EWFX )
	{
		fprintf( stream, "extended EWF (libewf)\n" );
	}
	else
	{
		fprintf( stream, "\n" );
	}
	fprintf( stream, "Acquiry start offet:\t\t%" PRIi64 "\n",
	 acquiry_offset );

	result = ewfbyte_size_string_create(
	          acquiry_size_string,
	          16,
	          acquiry_size,
	          EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );

	fprintf( stream, "Amount of bytes to acquire:\t" );

	if( acquiry_size == 0 )
	{
		fprintf( stream, "%" PRIu64 " (until end of input)",
		 acquiry_size );
	}
	else if( result == 1 )
	{
		fprintf( stream, "%" PRIs_EWF " (%" PRIu64 " bytes)",
		 acquiry_size_string, acquiry_size );
	}
	else
	{
		fprintf( stream, "%" PRIu64 " bytes",
		 acquiry_size );
	}
	fprintf( stream, "\n" );

	result = ewfbyte_size_string_create(
	          segment_file_size_string,
	          16,
	          segment_file_size,
	          EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );

	fprintf( stream, "Evidence segment file size:\t" );

	if( result == 1 )
	{
		fprintf( stream, "%" PRIs_EWF " (%" PRIu64 " bytes)",
		 segment_file_size_string, segment_file_size );
	}
	else
	{
		fprintf( stream, "%" PRIu64 " bytes",
		 segment_file_size );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Block size:\t\t\t%" PRIu32 " sectors\n",
	 sectors_per_chunk );
	fprintf( stream, "Error granularity:\t\t%" PRIu32 " sectors\n",
	 sector_error_granularity );
	fprintf( stream, "Retries on read error:\t\t%" PRIu8 "\n",
	 read_error_retry );

	fprintf( stream, "Wipe sectors on read error:\t" );

	if( wipe_block_on_read_error == 0 )
	{
		fprintf( stream, "no\n" );
	}
	else
	{
		fprintf( stream, "yes\n" );
	}
	fprintf( stream, "\n" );
}

/* Print the acquiry read errors (error2) to a stream
 */
void ewfoutput_acquiry_errors_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle,
      uint32_t *amount_of_errors )
{
	static char *function      = "ewfoutput_acquiry_errors_fprint";
	off64_t first_sector       = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t iterator          = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( amount_of_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of errors.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_acquiry_errors(
	     handle,
	     amount_of_errors ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to retrieve the amount of acquiry errors.\n",
		 function );

		return;
	}
	if( *amount_of_errors > 0 )
	{
		fprintf( stream, "Read errors during acquiry:\n" );
		fprintf( stream, "\ttotal amount: %" PRIu32 "\n",
		 *amount_of_errors );
		
		for( iterator = 0; iterator < *amount_of_errors; iterator++ )
		{
			if( libewf_get_acquiry_error(
			     handle,
			     iterator,
			     &first_sector,
			     &amount_of_sectors ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to retrieve the acquiry error: %" PRIu32 ".\n",
				 function, iterator );

				first_sector      = 0;
				amount_of_sectors = 0;
			}
			fprintf( stream, "\tin sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n",
			 (uint64_t) first_sector, (uint64_t) ( first_sector + amount_of_sectors ), amount_of_sectors );
		}
		fprintf( stream, "\n" );
	}
}

/* Print the read (CRC) errors to a stream
 */
void ewfoutput_crc_errors_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle,
      uint32_t *amount_of_errors )
{
	static char *function      = "ewfoutput_crc_errors_fprint";
	off64_t first_sector       = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t iterator          = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( amount_of_errors == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of errors.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_crc_errors(
	     handle,
	     amount_of_errors ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to retrieve the amount of CRC errors.\n",
		 function );

		return;
	}
	if( *amount_of_errors > 0 )
	{
		fprintf( stream, "Sector validation errors:\n" );
		fprintf( stream, "\ttotal amount: %" PRIu32 "\n", *amount_of_errors );

		for( iterator = 0; iterator < *amount_of_errors; iterator++ )
		{
			if( libewf_get_crc_error(
			     handle,
			     iterator,
			     &first_sector,
			     &amount_of_sectors ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to retrieve the CRC error: %" PRIu32 ".\n",
				 function, iterator );

				first_sector      = 0;
				amount_of_sectors = 0;
			}
			fprintf( stream, "\tin sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n",
			 (uint64_t) first_sector, (uint64_t) ( first_sector + amount_of_sectors ), amount_of_sectors );
		}
		fprintf( stream, "\n" );
	}
}

/* Print the sessions to a stream
 */
void ewfoutput_sessions_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle,
      uint32_t *amount_of_sessions )
{
	static char *function      = "ewfoutput_sessions_fprint";
	off64_t first_sector       = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t iterator          = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( amount_of_sessions == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount of sessions.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_sessions(
	     handle,
	     amount_of_sessions ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to retrieve the amount of sessions.\n",
		 function );

		return;
	}
	if( *amount_of_sessions > 0 )
	{
		fprintf( stream, "Sessions:\n" );
		fprintf( stream, "\ttotal amount: %" PRIu32 "\n", *amount_of_sessions );

		for( iterator = 0; iterator < *amount_of_sessions; iterator++ )
		{
			if( libewf_get_session(
			     handle,
			     iterator,
			     &first_sector,
			     &amount_of_sectors ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to retrieve the CRC error: %" PRIu32 ".\n",
				 function, iterator );

				first_sector      = 0;
				amount_of_sectors = 0;
			}
			fprintf( stream, "\tin sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n",
			 (uint64_t) first_sector, (uint64_t) ( first_sector + amount_of_sectors ), amount_of_sectors );
		}
		fprintf( stream, "\n" );
	}
}

/* Print the header values to a stream
 */
void ewfoutput_header_values_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle )
{
	libewf_char_t header_identifier[ 64 ];
	libewf_char_t header_value[ 128 ];

	static char *function           = "ewfoutput_header_values_fprint";
	size_t header_identifier_length = 64;
	size_t header_value_length      = 128;
	uint32_t amount_of_values       = 0;
	uint32_t iterator               = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_header_values( handle, &amount_of_values ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to retrieve the amount of header values.\n",
		 function );

		return;
	}
	if( amount_of_values == 0 )
	{
		fprintf( stream, "\tNo information found in file.\n" );

		return;
	}
	if( libewf_get_header_value_case_number( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tCase number:\t\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_description( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tDescription:\t\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_examiner_name( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tExaminer name:\t\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_evidence_number( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tEvidence number:\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_notes( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tNotes:\t\t\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_acquiry_date( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tAcquiry date:\t\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_system_date( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tSystem date:\t\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_acquiry_operating_system( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tOperating system used:\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_acquiry_software_version( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tSoftware version used:\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_password( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tPassword:\t\t(hash: %" PRIs_EWF ")\n", header_value );
	}
	else
	{
		fprintf( stream, "\tPassword:\t\tN/A\n" );
	}
	if( libewf_get_header_value_compression_type( handle, header_value, header_value_length ) == 1 )
	{
		if( libewf_string_compare( header_value, LIBEWF_COMPRESSION_TYPE_NONE, 1 ) == 0 )
		{
			fprintf( stream, "\tCompression type:\tno compression\n" );
		}
		else if( libewf_string_compare( header_value, LIBEWF_COMPRESSION_TYPE_FAST, 1 ) == 0 )
		{
			fprintf( stream, "\tCompression type:\tgood (fast) compression\n" );
		}
		else if( libewf_string_compare( header_value, LIBEWF_COMPRESSION_TYPE_BEST, 1 ) == 0 )
		{
			fprintf( stream, "\tCompression type:\tbest compression\n" );
		}
		else
		{
			fprintf( stream, "\tCompression type:\tunknown compression\n" );
		}
	}
	if( libewf_get_header_value_model( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tModel:\t\t\t%" PRIs_EWF "\n", header_value );
	}
	if( libewf_get_header_value_serial_number( handle, header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tSerial number:\t\t%" PRIs_EWF "\n", header_value );
	}
	/* TODO figure out what this value represents and add get & set API functions to libewf
	 */
	if( libewf_get_header_value( handle, _S_LIBEWF_CHAR( "unknown_dc" ), header_value, header_value_length ) == 1 )
	{
		fprintf( stream, "\tUnknown value dc:\t%" PRIs_EWF "\n", header_value );
	}
	if( amount_of_values > LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		fprintf( stream, "\n\tAdditional values:\n" );

		for( iterator = LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT; iterator < amount_of_values; iterator++ )
		{
			if( libewf_get_header_value_identifier( handle, iterator, header_identifier, header_identifier_length ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to retrieve the header identifier for index: %" PRIu32 ".\n",
				 function, iterator );
			}
			else if( libewf_get_header_value( handle, header_identifier, header_value, header_value_length ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to retrieve the header value for identifier: %" PRIs_EWF ".\n",
				 function, header_identifier );
			}
			else
			{
				fprintf( stream, "\t%" PRIs_EWF ": %" PRIs_EWF "\n",
				 header_identifier, header_value );
			}
		}
	}
}

/* Print the hash values to a stream
 */
void ewfoutput_hash_values_fprint(
      FILE *stream,
      LIBEWF_HANDLE *handle )
{
	libewf_char_t hash_identifier[ 32 ];
	libewf_char_t hash_value[ 128 ];
	ewfdigest_hash_t md5_hash[ EWFDIGEST_HASH_SIZE_MD5 ];

	libewf_char_t *stored_md5_hash_string = NULL;
	static char *function                 = "ewfoutput_hash_values_fprint";
	uint32_t hash_identifier_length       = 32;
	uint32_t hash_value_length            = 128;
	uint32_t amount_of_values             = 0;
	uint32_t iterator                     = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( libewf_get_md5_hash( handle, md5_hash, EWFDIGEST_HASH_SIZE_MD5 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to retrieve MD5 hash.\n",
		 function );

		return;
	}
	stored_md5_hash_string = (libewf_char_t *) memory_allocate(
	                                            sizeof( libewf_char_t ) * EWFSTRING_DIGEST_HASH_LENGTH_MD5 );

	if( ( stored_md5_hash_string != NULL )
	 && ( ewfdigest_copy_to_string(
	       md5_hash,
	       EWFDIGEST_HASH_SIZE_MD5,
	       stored_md5_hash_string,
	       EWFSTRING_DIGEST_HASH_LENGTH_MD5 ) == 1 ) )
	{
		fprintf( stream, "\tMD5 hash in file:\t%" PRIs_EWF "\n",
		 stored_md5_hash_string );

		libewf_common_free( stored_md5_hash_string );
	}
	else
	{
		fprintf( stream, "\tMD5 hash in file:\tN/A\n" );
	}
	if( libewf_parse_hash_values( handle ) == 1 )
	{
		if( libewf_get_amount_of_hash_values( handle, &amount_of_values ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to retrieve amount of hash values.\n",
			 function );

			return;
		}
		if( amount_of_values > 0 )
		{
			if( amount_of_values > LIBEWF_HASH_VALUES_DEFAULT_AMOUNT )
			{
				fprintf( stream, "\n\tAdditional hash values:\n" );

				for( iterator = LIBEWF_HASH_VALUES_DEFAULT_AMOUNT; iterator < amount_of_values; iterator++ )
				{
					if( libewf_get_hash_value_identifier( handle, iterator, hash_identifier, hash_identifier_length ) != 1 )
					{
						LIBEWF_WARNING_PRINT( "%s: unable to retrieve the hash identifier for index: %" PRIu32 ".\n",
						 function, iterator );
					}
					else if( libewf_get_hash_value( handle, hash_identifier, hash_value, hash_value_length ) != 1 )
					{
						LIBEWF_WARNING_PRINT( "%s: unable to retrieve the hash value for identifier: %" PRIs_EWF ".\n",
						 function, hash_identifier );
					}
					else
					{
						fprintf( stream, "\t%" PRIs_EWF ":\t%" PRIs_EWF "\n",
						 hash_identifier, hash_value );
					}
				}
			}
		}
	}
}

/* Prints a time stamp (with a leading space) to a stream
 */
void ewfoutput_timestamp_fprint(
      FILE *stream,
      time_t timestamp )
{
	struct tm *time_elements = NULL;

	if( stream == NULL )
	{
		return;
	}
	time_elements = ewfoutput_gmtime(
	                 &timestamp );

	if( time_elements != NULL )
	{
		fprintf( stream, " in" );

		if( time_elements->tm_isdst != 0 )
		{
			time_elements->tm_hour -= 1;
			time_elements->tm_isdst = 0;	
		}
		if( time_elements->tm_yday > 0 )
		{
			fprintf(
			 stream,
			 " %i day(s), %i hour(s), %i minute(s) and",
			 time_elements->tm_yday,
			 time_elements->tm_hour,
			 time_elements->tm_min );
		}
		else if( time_elements->tm_hour > 0 )
		{
			fprintf(
			 stream,
			 " %i hour(s), %i minute(s) and",
			 time_elements->tm_hour,
			 time_elements->tm_min );
		}
		else if( time_elements->tm_min > 0 )
		{
			fprintf(
			 stream,
			 " %i minute(s) and",
			 time_elements->tm_min );
		}
		fprintf(
		 stream,
		 " %i second(s)",
		 time_elements->tm_sec );

		libewf_common_free(
		 time_elements );
	}
}

/* Prints the amount of bytes per second (with a leading space) to a stream
 */
void ewfoutput_bytes_per_second_fprint(
      FILE *stream,
      size64_t bytes,
      time_t seconds )
{
	libewf_char_t bytes_per_second_string[ 16 ];

	size64_t bytes_per_second = 0;
	int result                = 0;

	if( stream == NULL )
	{
		return;
	}
	if( seconds > 0 )
	{
		bytes_per_second = bytes / seconds;

		if( bytes_per_second > 1024 )
		{
			result = ewfbyte_size_string_create(
			          bytes_per_second_string,
			          10,
			          bytes_per_second,
			          EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );
		}
		fprintf(
		 stream,
		 " with" );

		if( result == 1 )
		{
			fprintf(
			 stream,
			 " %" PRIs_EWF "/s (%" PRIu64 " bytes/second)",
			 bytes_per_second_string, bytes_per_second );
		}
		else
		{
			fprintf(
			 stream,
			 " %" PRIu64 " bytes/second",
			 bytes_per_second );
		}
	}
}

/* Prints the amount of bytes (with a leading space) to a stream
 * Creates a human readable version of the amount of bytes if possible
 */
void ewfoutput_bytes_fprint(
      FILE *stream,
      size64_t bytes )
{
	libewf_char_t bytes_string[ 16 ];

	int result = 0;

	if( stream == NULL )
	{
		return;
	}
	if( bytes > 1024 )
	{
		result = ewfbyte_size_string_create(
		          bytes_string,
		          10,
		          bytes,
		          EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE );
	}
	if( result == 1 )
	{
		fprintf(
		 stream,
		 " %" PRIs_EWF " (%" PRIi64 " bytes)",
		 bytes_string, bytes );
	}
	else
	{
		fprintf(
		 stream,
		 " %" PRIi64 " bytes",
		 bytes );
	}
}

/* Static values for status information of the process
 */
FILE* ewfoutput_process_status_stream              = NULL;
libewf_char_t *ewfoutput_process_status_string     = NULL;
time_t ewfoutput_process_status_timestamp_start    = 0;
time_t ewfoutput_process_status_timestamp_last     = 0;
int8_t ewfoutput_process_status_last_percentage    = -1;
uint64_t ewfoutput_process_status_last_bytes_total = 0;

/* Initializes the status information of the process
 */
void ewfoutput_process_status_initialize(
      FILE *stream,
      libewf_char_t *string,
      time_t timestamp_start )
{
	ewfoutput_process_status_stream          = stream;
	ewfoutput_process_status_string          = string;
	ewfoutput_process_status_timestamp_start = timestamp_start;
	ewfoutput_process_status_timestamp_last  = timestamp_start;
}

/* Prints status information of the process
 */
void ewfoutput_process_status_fprint(
      size64_t bytes_read,
      size64_t bytes_total )
{
	time_t seconds_current   = 0;
	time_t seconds_total     = 0;
	time_t seconds_remaining = 0;
	time_t timestamp_current = 0;
	int8_t new_percentage    = 0;

	if( ewfoutput_process_status_stream == NULL )
	{
		return;
	}
	if( ewfoutput_process_status_string == NULL )
	{
		return;
	}
	if( ( bytes_total > 0 )
	 && ( bytes_read > 0 ) )
	{
		new_percentage = (int8_t) ( ( bytes_read * 100 ) / bytes_total );
	}
	/* Estimate the remaining acquiry time
	 */
	timestamp_current = time( NULL );

	if( ( new_percentage > ewfoutput_process_status_last_percentage )
	 && ( timestamp_current > ewfoutput_process_status_timestamp_last ) )
	{
		ewfoutput_process_status_last_percentage = new_percentage;

		fprintf(
		 ewfoutput_process_status_stream,
		 "Status: at %" PRIu8 "%%.\n",
		 new_percentage );

		fprintf(
		 ewfoutput_process_status_stream,
		 "        %" PRIs_EWF "",
		 ewfoutput_process_status_string );

		ewfoutput_bytes_fprint(
		 ewfoutput_process_status_stream,
		 bytes_read );

		fprintf(
		 ewfoutput_process_status_stream,
		 " of total" );

		ewfoutput_bytes_fprint(
		 ewfoutput_process_status_stream,
		 bytes_total );

		fprintf(
		 ewfoutput_process_status_stream,
		 ".\n" );

		if( ( timestamp_current > ewfoutput_process_status_timestamp_start )
		 && ( new_percentage > 0 ) )
		{
			ewfoutput_process_status_timestamp_last = timestamp_current;

			seconds_current   = timestamp_current - ewfoutput_process_status_timestamp_start;
			seconds_total     = ( ( seconds_current * 100 ) / new_percentage );
			seconds_remaining = seconds_total - seconds_current;

			/* Negative time means nearly finished
			 */
			if( seconds_remaining < 0 )
			{
				seconds_remaining = 0;
			}
			fprintf(
			 ewfoutput_process_status_stream,
			 "        completion" );

			ewfoutput_timestamp_fprint(
			 ewfoutput_process_status_stream,
			 seconds_remaining );

			ewfoutput_bytes_per_second_fprint(
			 ewfoutput_process_status_stream,
			 bytes_total,
			 seconds_total );

			fprintf(
			 ewfoutput_process_status_stream,
			 ".\n" );
		}
		fprintf(
		 ewfoutput_process_status_stream,
		 "\n" );
	}
}

/* Prints status information of the stream process
 */
void ewfoutput_stream_process_status_fprint(
      size64_t bytes_read,
      size64_t bytes_total )
{
	time_t seconds_current   = 0;
	time_t timestamp_current = 0;

	if( ewfoutput_process_status_stream == NULL )
	{
		return;
	}
	if( ewfoutput_process_status_string == NULL )
	{
		return;
	}
	timestamp_current = time( NULL );

	if( timestamp_current > ewfoutput_process_status_timestamp_last )
	{
		/* Update state
		 * - if no status was printed before
		 * - or input has grown > 10 Mb
		 * - or the last update was 30 seconds ago
		 */
		if( ( ewfoutput_process_status_last_bytes_total == 0 )
		 || ( bytes_read > ( ewfoutput_process_status_last_bytes_total + ( 10 * 1024 * 1024 ) ) )
		 || ( ( timestamp_current - ewfoutput_process_status_timestamp_last ) > 30 ) )
		{
			ewfoutput_process_status_timestamp_last   = timestamp_current;
			ewfoutput_process_status_last_bytes_total = bytes_read;

			fprintf(
			 ewfoutput_process_status_stream,
			 "Status: %" PRIs_EWF "",
			 ewfoutput_process_status_string );

			ewfoutput_bytes_fprint(
			 ewfoutput_process_status_stream,
			 bytes_read );

			fprintf(
			 ewfoutput_process_status_stream,
			 "\n" );

			seconds_current = timestamp_current - ewfoutput_process_status_timestamp_start;

			fprintf(
			 ewfoutput_process_status_stream,
			 "       " );

			ewfoutput_timestamp_fprint(
			 ewfoutput_process_status_stream,
			 seconds_current );

			ewfoutput_bytes_per_second_fprint(
			 ewfoutput_process_status_stream,
			 bytes_read,
			 seconds_current );

			fprintf(
			 ewfoutput_process_status_stream,
			 ".\n\n" );
		}
	}
}

/* Prints summary information of the process
 */
void ewfoutput_process_summary_fprint(
      FILE *stream,
      libewf_char_t *string,
      ssize64_t byte_count,
      time_t timestamp_start,
      time_t timestamp_end )
{
	time_t timestamp_acquiry = 0;

	if( stream == NULL )
	{
		return;
	}
	if( string == NULL )
	{
		return;
	}
	timestamp_acquiry = timestamp_end - timestamp_start;

	fprintf(
	 stream,
	 "%" PRIs_EWF ":",
	 string );

	ewfoutput_bytes_fprint(
	 stream,
	 byte_count );

	ewfoutput_timestamp_fprint(
	 stream,
	 timestamp_acquiry );

	ewfoutput_bytes_per_second_fprint(
	 stream,
	 byte_count,
	 timestamp_acquiry );

	fprintf(
	 stream,
	 ".\n" );
}


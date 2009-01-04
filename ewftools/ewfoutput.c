/*
 * User output functions for the ewftools
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
#include <date_time.h>
#include <error_string.h>
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

#if defined( HAVE_STDARG_H )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error No variable argument support available
#endif

#if defined( HAVE_LIBUCA_H )
#include <libuca.h>
#elif defined( HAVE_LOCAL_LIBUCA )
#include "../libuca/libuca_definitions.h"
#endif

#if defined( HAVE_ZLIB_H )
#include <zlib.h>
#endif

#if defined( HAVE_OPENSSL_OPENSSLV_H )
#include <openssl/opensslv.h>
#endif

#if defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "../libewf/libewf_hash_values.h"
#include "../libewf/libewf_header_values.h"

#include "ewfbyte_size_string.h"
#include "ewfdigest_context.h"
#include "ewfmd5.h"
#include "ewfsha1.h"
#include "ewfstring.h"
#include "ewfoutput.h"

/* Print the version information to a stream
 */
void ewfoutput_version_fprint(
      FILE *stream,
      character_t *program )
{
	static char *function = "ewfoutput_version_fprint";

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( program == NULL )
	{
		notify_warning_printf( "%s: invalid program name.\n",
		 function );

		return;
	}
	fprintf( stream, "%" PRIs " %" PRIs " (libewf %" PRIs ", libuca %" PRIs "",
	 program, LIBEWF_VERSION_STRING, LIBEWF_VERSION_STRING, LIBUCA_VERSION_STRING );

#if defined( HAVE_LIBZ )
	fprintf( stream, ", zlib %s", ZLIB_VERSION );
#endif

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
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	fprintf( stream, "Copyright (c) 2006-2008, Joachim Metz, Hoffmann Investigations <%s> and contributors.\n",
	 PACKAGE_BUGREPORT );
	fprintf( stream, "This is free software; see the source for copying conditions. There is NO\n" );
	fprintf( stream, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" );
}

#if defined( HAVE_STDARG_H )
#define VARIABLE_ARGUMENTS_FUNCTION( function, type, argument ) \
        function( FILE *stream, type argument, ... )
#define VARIABLE_ARGUMENTS_START( argument_list, type, name ) \
        va_start( argument_list, name )
#define VARIABLE_ARGUMENTS_END( argument_list ) \
        va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARIABLE_ARGUMENTS_FUNCTION( function, type, argument ) \
        function( FILE *stream, va_alist ) va_dcl
#define VARIABLE_ARGUMENTS_START( argument_list, type, name ) \
        { type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VARIABLE_ARGUMENTS_END( argument_list ) \
        va_end( argument_list ); }

#endif

void VARIABLE_ARGUMENTS_FUNCTION(
      ewfoutput_error_fprint,
      char *,
      format )
{
	va_list argument_list;

#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
        system_character_t *error_string = NULL;
#endif

	if( stream == NULL )
	{
		return;
	}
	VARIABLE_ARGUMENTS_START(
	 argument_list,
	 char *,
       	 format );

	vfprintf(
       	 stream,
	 format,
       	 argument_list );

	VARIABLE_ARGUMENTS_END(
       	 argument_list );

#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
	if( errno != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
		error_string = error_string_wcserror(
		                errno );
#else
		error_string = error_string_strerror(
		                errno );
#endif

		if( error_string != NULL )
		{
			fprintf( stream, " with error: %" PRIs_SYSTEM "",
			 error_string );

			memory_free(
			 error_string );
		}
	}
#endif
	fprintf( stream, ".\n" );
}

#undef VARIABLE_ARGUMENTS_FUNCTION
#undef VARIABLE_ARGUMENTS_START
#undef VARIABLE_ARGUMENTS_END

/* Prints an overview of the aquiry parameters
 */
void ewfoutput_acquiry_parameters_fprint(
      FILE *stream,
      system_character_t *filename,
      character_t *case_number,
      character_t *description,
      character_t *evidence_number,
      character_t *examiner_name,
      character_t *notes,
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
	character_t acquiry_size_string[ 16 ];
	character_t segment_file_size_string[ 16 ];

	static char *function = "ewfoutput_acquiry_parameters_fprint";
	int result            = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	fprintf( stream, "Image path and filename:\t%" PRIs_SYSTEM ".",
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
		fprintf( stream, "%" PRIs "",
		 case_number );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Description:\t\t\t" );

	if( description != NULL )
	{
		fprintf( stream, "%" PRIs "",
		 description );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Evidence number:\t\t" );

	if( evidence_number != NULL )
	{
		fprintf( stream, "%" PRIs "",
		 evidence_number );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Examiner name:\t\t\t" );

	if( examiner_name != NULL )
	{
		fprintf( stream, "%" PRIs "",
		 examiner_name );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Notes:\t\t\t\t" );

	if( notes != NULL )
	{
		fprintf( stream, "%" PRIs "",
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
#if defined( LIBEWF_CD_SUPPORT )
	else if( media_type == LIBEWF_MEDIA_TYPE_CD )
	{
		fprintf( stream, "cd/dvd\n" );
	}
#endif
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
		fprintf( stream, "%" PRIs " (%" PRIu64 " bytes)",
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
		fprintf( stream, "%" PRIs " (%" PRIu64 " bytes)",
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
      libewf_handle_t *handle,
      uint32_t *amount_of_errors )
{
	static char *function      = "ewfoutput_acquiry_errors_fprint";
	off64_t first_sector       = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t iterator          = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( amount_of_errors == NULL )
	{
		notify_warning_printf( "%s: invalid amount of errors.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_acquiry_errors(
	     handle,
	     amount_of_errors ) == -1 )
	{
		notify_warning_printf( "%s: unable to retrieve the amount of acquiry errors.\n",
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
				notify_warning_printf( "%s: unable to retrieve the acquiry error: %" PRIu32 ".\n",
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
      libewf_handle_t *handle,
      uint32_t *amount_of_errors )
{
	static char *function      = "ewfoutput_crc_errors_fprint";
	off64_t first_sector       = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t iterator          = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( amount_of_errors == NULL )
	{
		notify_warning_printf( "%s: invalid amount of errors.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_crc_errors(
	     handle,
	     amount_of_errors ) == -1 )
	{
		notify_warning_printf( "%s: unable to retrieve the amount of CRC errors.\n",
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
				notify_warning_printf( "%s: unable to retrieve the CRC error: %" PRIu32 ".\n",
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
      libewf_handle_t *handle,
      uint32_t *amount_of_sessions )
{
	static char *function      = "ewfoutput_sessions_fprint";
	off64_t first_sector       = 0;
	uint32_t amount_of_sectors = 0;
	uint32_t iterator          = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( amount_of_sessions == NULL )
	{
		notify_warning_printf( "%s: invalid amount of sessions.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_sessions(
	     handle,
	     amount_of_sessions ) == -1 )
	{
		notify_warning_printf( "%s: unable to retrieve the amount of sessions.\n",
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
				notify_warning_printf( "%s: unable to retrieve the CRC error: %" PRIu32 ".\n",
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
      libewf_handle_t *handle )
{
	character_t header_identifier[ 64 ];
	character_t header_value[ 128 ];

	static char *function           = "ewfoutput_header_values_fprint";
	size_t header_identifier_length = 64;
	size_t header_value_length      = 128;
	uint32_t amount_of_values       = 0;
	uint32_t iterator               = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( libewf_get_amount_of_header_values(
	     handle,
	     &amount_of_values ) == -1 )
	{
		notify_warning_printf( "%s: unable to retrieve the amount of header values.\n",
		 function );

		return;
	}
	if( amount_of_values == 0 )
	{
		fprintf( stream, "\tNo information found in file.\n" );

		return;
	}
	if( libewf_get_header_value_case_number(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tCase number:\t\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_description(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tDescription:\t\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_examiner_name(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tExaminer name:\t\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_evidence_number(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tEvidence number:\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_notes(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tNotes:\t\t\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_acquiry_date(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tAcquiry date:\t\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_system_date(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tSystem date:\t\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_acquiry_operating_system(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tOperating system used:\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_acquiry_software_version(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tSoftware version used:\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_password(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tPassword:\t\t(hash: %" PRIs ")\n",
		 header_value );
	}
	else
	{
		fprintf( stream, "\tPassword:\t\tN/A\n" );
	}
	if( libewf_get_header_value_compression_type(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		if( string_compare(
		     header_value,
		     LIBEWF_COMPRESSION_TYPE_NONE, 1 ) == 0 )
		{
			fprintf( stream, "\tCompression type:\tno compression\n" );
		}
		else if( string_compare(
		          header_value,
		          LIBEWF_COMPRESSION_TYPE_FAST, 1 ) == 0 )
		{
			fprintf( stream, "\tCompression type:\tgood (fast) compression\n" );
		}
		else if( string_compare(
		          header_value,
		          LIBEWF_COMPRESSION_TYPE_BEST, 1 ) == 0 )
		{
			fprintf( stream, "\tCompression type:\tbest compression\n" );
		}
		else
		{
			fprintf( stream, "\tCompression type:\tunknown compression\n" );
		}
	}
	if( libewf_get_header_value_model(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tModel:\t\t\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value_serial_number(
	     handle,
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tSerial number:\t\t%" PRIs "\n",
		 header_value );
	}
	/* TODO figure out what this value represents and add get & set API functions to libewf
	 */
	if( libewf_get_header_value(
	     handle,
	     _CHARACTER_T_STRING( "unknown_pid" ),
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tUnknown value pid:\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value(
	     handle,
	     _CHARACTER_T_STRING( "unknown_dc" ),
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tUnknown value dc:\t%" PRIs "\n",
		 header_value );
	}
	if( libewf_get_header_value(
	     handle,
	     _CHARACTER_T_STRING( "unknown_ext" ),
	     header_value,
	     header_value_length ) == 1 )
	{
		fprintf( stream, "\tUnknown value ext:\t%" PRIs "\n",
		 header_value );
	}
	if( amount_of_values > LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		fprintf( stream, "\n\tAdditional values:\n" );

		for( iterator = LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT; iterator < amount_of_values; iterator++ )
		{
			if( libewf_get_header_value_identifier(
			     handle,
			     iterator,
			     header_identifier,
			     header_identifier_length ) != 1 )
			{
				notify_warning_printf( "%s: unable to retrieve the header identifier for index: %" PRIu32 ".\n",
				 function, iterator );
			}
			else if( libewf_get_header_value(
			          handle,
			          header_identifier,
			          header_value,
			          header_value_length ) != 1 )
			{
				notify_warning_printf( "%s: unable to retrieve the header value for identifier: %" PRIs ".\n",
				 function, header_identifier );
			}
			else
			{
				fprintf( stream, "\t%" PRIs ": %" PRIs "\n",
				 header_identifier, header_value );
			}
		}
	}
}

/* Print the hash values to a stream
 */
void ewfoutput_hash_values_fprint(
      FILE *stream,
      libewf_handle_t *handle )
{
	character_t hash_identifier[ 32 ];
	character_t hash_value[ 128 ];
	ewfdigest_hash_t md5_hash[ EWFDIGEST_HASH_SIZE_MD5 ];

	character_t *stored_md5_hash_string = NULL;
	static char *function               = "ewfoutput_hash_values_fprint";
	uint32_t hash_identifier_length     = 32;
	uint32_t hash_value_length          = 128;
	uint32_t amount_of_values           = 0;
	uint32_t iterator                   = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid stream.\n",
		 function );

		return;
	}
	if( handle == NULL )
	{
		notify_warning_printf( "%s: invalid handle.\n",
		 function );

		return;
	}
	if( libewf_get_md5_hash( handle, md5_hash, EWFDIGEST_HASH_SIZE_MD5 ) != 1 )
	{
		notify_warning_printf( "%s: unable to retrieve MD5 hash.\n",
		 function );

		return;
	}
	stored_md5_hash_string = (character_t *) memory_allocate(
	                                          sizeof( character_t ) * EWFSTRING_DIGEST_HASH_LENGTH_MD5 );

	if( ( stored_md5_hash_string != NULL )
	 && ( ewfdigest_copy_to_string(
	       md5_hash,
	       EWFDIGEST_HASH_SIZE_MD5,
	       stored_md5_hash_string,
	       EWFSTRING_DIGEST_HASH_LENGTH_MD5 ) == 1 ) )
	{
		fprintf( stream, "\tMD5 hash in file:\t%" PRIs "\n",
		 stored_md5_hash_string );

		memory_free(
		 stored_md5_hash_string );
	}
	else
	{
		fprintf( stream, "\tMD5 hash in file:\tN/A\n" );
	}
	if( libewf_parse_hash_values( handle ) == 1 )
	{
		if( libewf_get_amount_of_hash_values( handle, &amount_of_values ) == -1 )
		{
			notify_warning_printf( "%s: unable to retrieve amount of hash values.\n",
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
						notify_warning_printf( "%s: unable to retrieve the hash identifier for index: %" PRIu32 ".\n",
						 function, iterator );
					}
					else if( libewf_get_hash_value( handle, hash_identifier, hash_value, hash_value_length ) != 1 )
					{
						notify_warning_printf( "%s: unable to retrieve the hash value for identifier: %" PRIs ".\n",
						 function, hash_identifier );
					}
					else
					{
						fprintf( stream, "\t%" PRIs ":\t%" PRIs "\n",
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
	time_elements = date_time_gmtime(
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

		memory_free(
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
	character_t bytes_per_second_string[ 16 ];

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
			 " %" PRIs "/s (%" PRIu64 " bytes/second)",
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
	character_t bytes_string[ 16 ];

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
		 " %" PRIs " (%" PRIi64 " bytes)",
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


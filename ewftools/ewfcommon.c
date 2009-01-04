/*
 * ewfcommon
 * Common functions for the ewf tools
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

#include "../libewf/libewf_includes.h"

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

#include <libewf.h>

#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"
#include "../libewf/libewf_string.h"

#include "../libewf/ewf_digest_hash.h"

#if defined( HAVE_CHUNK_CACHE_PASSTHROUGH )
#include "../libewf/libewf_internal_handle.h"
#include "../libewf/ewf_crc.h"
#endif

#include "ewfcommon.h"

/* EWFCOMMON_BUFFER_SIZE definition is intended for testing purposes
 */
#ifndef EWFCOMMON_BUFFER_SIZE
#define EWFCOMMON_BUFFER_SIZE	chunk_size
#endif

#ifndef LIBEWF_OPERATING_SYSTEM
#define LIBEWF_OPERATING_SYSTEM "Unknown"
#endif

/* Determines the current platform, or NULL on error
 */
LIBEWF_CHAR *ewfcommon_determine_operating_system( void )
{
	LIBEWF_CHAR *string    = NULL;
	char *operating_system = NULL;
	uint32_t length        = 0;

#ifdef HAVE_SYS_UTSNAME_H
	struct utsname utsname_buffer;

	/* Determine the operating system
	 */
	if( uname( &utsname_buffer ) == 0 )
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
	length = (uint32_t) strlen( operating_system ) + 1;
	string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * length );

	if( ( string != NULL ) && ( ewfcommon_copy_libewf_char_from_char_t( string, operating_system, length ) != 1 ) )
	{
		libewf_common_free( string );
	
		return( NULL );
	}
	return( string );
}

/* Determines the GUID
 * Returns 1 if successful, or -1 on error
 */
int8_t ewfcommon_determine_guid( uint8_t *guid, uint8_t libewf_format )
{
	if( guid == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_determine_guid: invalid GUID.\n" );

		return( -1 );
	}
#if defined(HAVE_UUID_UUID_H) && defined(HAVE_LIBUUID)
#ifdef HAVE_UUID_GENERATE_RANDOM
	if( ( libewf_format == LIBEWF_FORMAT_ENCASE5 )
	 || ( libewf_format == LIBEWF_FORMAT_ENCASE6 )
	 || ( libewf_format == LIBEWF_FORMAT_EWFX ) )
	{
		uuid_generate_random( guid );
	}
#endif
#ifdef HAVE_UUID_GENERATE_TIME
	if( ( libewf_format == LIBEWF_FORMAT_LINEN5 )
	 || ( libewf_format == LIBEWF_FORMAT_LINEN6 ) )
	{
		uuid_generate_time( guid );
	}
#endif
#endif
	return( 1 );
}

/* Determines the units strings of a certain factor value
 */
LIBEWF_CHAR *ewfcommon_determine_units_string( int factor )
{
	switch( factor )
	{
		case 0:
			return( _S_LIBEWF_CHAR( "B" ) );
		case 1:
			return( _S_LIBEWF_CHAR( "kB" ) );
		case 2:
			return( _S_LIBEWF_CHAR( "MB" ) );
		case 3:
			return( _S_LIBEWF_CHAR( "GB" ) );
		case 4:
			return( _S_LIBEWF_CHAR( "TB" ) );
		case 5:
			return( _S_LIBEWF_CHAR( "PB" ) );
		case 6:
			return( _S_LIBEWF_CHAR( "EB" ) );
		case 7:
			return( _S_LIBEWF_CHAR( "ZB" ) );
		default :
			break;
	}
	return( _S_LIBEWF_CHAR( "?B" ) );
}

/* Determines the human readable size as a string
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *ewfcommon_determine_human_readable_size_string( uint64_t size )
{
	LIBEWF_CHAR *size_string  = NULL;
	LIBEWF_CHAR *units_string = NULL;
	int8_t remainder          = -1;
	uint8_t factor            = 0;
	uint64_t new_size         = 0;

	while( size >= 1024 )
	{
		factor++;

		new_size = size / 1024;

		if( new_size < 10 )
		{
			remainder = (uint8_t) ( ( size % 1024 ) / 100 );
		}
		size = new_size;
	}
	if( factor > 7 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_determine_human_readable_size_string: a size with a factor larger than 7 currently not supported.\n" );

		return( NULL );
	}
	/* The string has a maximum of 7 characters + end of string '\0'
	 */
	size_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * 8 );

	if( size_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_determine_human_readable_size_string: unable to create size string.\n" );

		return( NULL );
	}
	units_string = ewfcommon_determine_units_string( (int) factor );

	if( remainder > 9 )
	{
		remainder = 9;
	}
	if( remainder >= 0 )
	{
		libewf_string_snprintf( size_string, 8, _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIu64 ) _S_LIBEWF_CHAR( ".%" ) _S_LIBEWF_CHAR( PRIu8 ) _S_LIBEWF_CHAR( " %" ) _S_LIBEWF_CHAR( PRIs_EWF ), size, remainder, units_string );
	}
	else
	{
		libewf_string_snprintf( size_string, 8, _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIu64 ) _S_LIBEWF_CHAR( " %" ) _S_LIBEWF_CHAR( PRIs_EWF ), size, units_string );
	}
	return( size_string );
}

/* Copies the source string (of CHAR_T) into the destination string for a certain length
 * Terminates the destination string with \0 at ( length - 1 )
 * Returns 1 if successful, -1 on error
 */
int8_t ewfcommon_copy_libewf_char_from_char_t( LIBEWF_CHAR *destination, const CHAR_T *source, size_t length )
{
	ssize_t conversion = (ssize_t) ( sizeof( LIBEWF_CHAR ) - sizeof( CHAR_T ) );
	size_t iterator    = 0;

	if( source == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_copy_libewf_char_from_char_t: invalid source.\n" );

		return( -1 );
	}
	if( destination == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_copy_libewf_char_from_char_t: invalid destination.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < length; iterator++ )
	{
		if( conversion == 0 )
		{
			destination[ iterator ] = (LIBEWF_CHAR) source[ iterator ];
		}
#ifdef HAVE_WIDE_CHARACTER_TYPE
		else if( conversion > 0 )
		{
			destination[ iterator ] = (LIBEWF_CHAR) btowc( (int) source[ iterator ] );
		}
		else if( conversion < 0 )
		{
			destination[ iterator ] = (LIBEWF_CHAR) wctob( (wint_t) source[ iterator ] );

			/* If character is out of the basic ASCII range use '_' as a place holder
			 */
			if( destination[ iterator ] == EOF )
			{
				destination[ iterator ] = '_';
			}
		}
#endif
		else
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_copy_libewf_char_from_char_t: unsupported converstion.\n" );

			return( -1 );
		}
	}
	destination[ length - 1 ] = (LIBEWF_CHAR) '\0';

	return( 1 );
}

/* Copies the source string into the destination string (of CHAR_T) for a certain length
 * Terminates the destination string with \0 at ( length - 1 )
 * Returns 1 if successful, -1 on error
 */
int8_t ewcommon_copy_libewf_char_to_char_t( const LIBEWF_CHAR *source, CHAR_T *destination, size_t length )
{
	ssize_t conversion = (ssize_t) ( sizeof( LIBEWF_CHAR ) - sizeof( CHAR_T ) );
	size_t iterator    = 0;

	if( source == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewcommon_copy_libewf_char_to_char_t: invalid source.\n" );

		return( -1 );
	}
	if( destination == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewcommon_copy_libewf_char_to_char_t: invalid destination.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < length; iterator++ )
	{
		if( conversion == 0 )
		{
			destination[ iterator ] = (CHAR_T) source[ iterator ];
		}
#ifdef HAVE_WIDE_CHARACTER_TYPE
		else if( conversion > 0 )
		{
			destination[ iterator ] = (CHAR_T) wctob( (wint_t) source[ iterator ] );

			/* If character is out of the basic ASCII range use '_' as a place holder
			 */
			if( destination[ iterator ] == EOF )
			{
				destination[ iterator ] = '_';
			}
		}
		else if( conversion < 0 )
		{
			destination[ iterator ] = (CHAR_T) btowc( (int) source[ iterator ] );
		}
#endif
		else
		{
			LIBEWF_WARNING_PRINT( "ewcommon_copy_libewf_char_to_char_t: unsupported converstion.\n" );

			return( -1 );
		}
	}
	destination[ length - 1 ] = (CHAR_T) '\0';

	return( 1 );
}

/* Get variable input from the user
 * with a maximum of 1023 characters
 */
LIBEWF_CHAR *ewfcommon_get_user_input_variable( FILE *stream, LIBEWF_CHAR *request_string )
{
	LIBEWF_CHAR user_input_buffer[ 1024 ];

	LIBEWF_CHAR *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	LIBEWF_CHAR *user_input            = NULL;
	LIBEWF_CHAR *end_of_input          = NULL;
	size_t input_length                = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_variable: Invalid output stream.\n" );

		return( NULL );
	}
	if( request_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_variable: Invalid request string.\n" );

		return( NULL );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs_EWF ": ", request_string );

		user_input_buffer_ptr = libewf_string_get_from_stream( user_input_buffer_ptr, 1023, stdin );

		if( user_input_buffer_ptr != NULL )
		{
			end_of_input = libewf_string_search( user_input_buffer_ptr, (LIBEWF_CHAR) '\n', 1023 );

			if( end_of_input == NULL )
			{
				return( NULL );
			}
			input_length = (uint32_t) ( end_of_input - user_input_buffer_ptr );

			if( input_length <= 0 )
			{
				return( NULL );
			}
			/* One additional character required for end of string
			 */
			user_input = libewf_string_duplicate( user_input_buffer_ptr, input_length );

			if( user_input == NULL )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_variable: unable to create string.\n" );

				return( NULL );
			}
			break;
		}
		else
		{
			fprintf( stream, "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( user_input );
}

/* Get variable input from the user
 * with a maximum of 1023 characters
 */
CHAR_T *ewfcommon_get_user_input_variable_char_t( FILE *stream, LIBEWF_CHAR *request_string )
{
	LIBEWF_CHAR *user_input   = NULL;
	CHAR_T *user_input_char_t = NULL;
	size_t user_input_length  = 0;

	user_input = ewfcommon_get_user_input_variable( stream, request_string );

	if( sizeof( CHAR_T ) == sizeof( LIBEWF_CHAR ) )
	{
		return( (CHAR_T *) user_input );
	}
	if( sizeof( CHAR_T ) < sizeof( LIBEWF_CHAR ) )
	{
		user_input_length = libewf_string_length( user_input );
		user_input_char_t = libewf_common_alloc( ( user_input_length + 1 ) * sizeof( CHAR_T ) );

		if( user_input_char_t == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_variable_char_t: unable to create conversion string.\n" );

			return( NULL );
		}
		if( ewcommon_copy_libewf_char_to_char_t( user_input, user_input_char_t, ( user_input_length + 1 ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_variable_char_t: unable to set conversion string.\n" );

			return( NULL );
		}
		return( user_input_char_t );
	}
	LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_variable_char_t: character conversion unsupported.\n" );

	return( NULL );
}

/* Get variable containing a size definnition input from the user
 * with a maximum of 1023 characters
 */
uint64_t ewfcommon_get_user_input_size_variable( FILE *stream, LIBEWF_CHAR *request_string, uint64_t minimum, uint64_t maximum, uint64_t default_value )
{
	LIBEWF_CHAR user_input_buffer[ 1024 ];

	LIBEWF_CHAR *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	size_t input_length                = 0;
	uint64_t size_value                = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_size_variable: Invalid output stream.\n" );

		return( 0 );
	}
	if( request_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_size_variable: Invalid request string.\n" );

		return( 0 );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs_EWF " (%" PRIu64 " >= value >= %" PRIu64 ") [%" PRIu64 "]: ", request_string, minimum, maximum, default_value );

		user_input_buffer_ptr = libewf_string_get_from_stream( user_input_buffer_ptr, 1023, stdin );

		if( user_input_buffer_ptr != NULL )
		{
			/* Remove the trailing newline character
			 */
			input_length = libewf_string_length( user_input_buffer_ptr ) - 1;

			if( input_length <= 0 )
			{
				return( default_value );
			}
			size_value = libewf_string_to_uint64( user_input_buffer_ptr, input_length );

			if( ( size_value >= minimum ) && ( size_value <= maximum ) )
			{
				break;
			}
			else
			{
				fprintf( stream, "Value not within specified range, please try again or terminate using Ctrl^C.\n" );
			}
		}
		else
		{
			fprintf( stream, "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( size_value );
}

/* Get fixed value input from the user
 * The first value is considered the default value
 */
LIBEWF_CHAR *ewfcommon_get_user_input_fixed_value( FILE *stream, LIBEWF_CHAR *request_string, LIBEWF_CHAR **values, uint8_t amount, uint8_t default_value )
{
	LIBEWF_CHAR user_input_buffer[ 1024 ];

	LIBEWF_CHAR *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	LIBEWF_CHAR *user_input            = NULL;
	size_t input_length                = 0;
	size_t value_length                = 0;
	uint8_t iterator                   = 0;
	uint8_t value_match                = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_fixed_value: Invalid output stream.\n" );

		return( NULL );
	}
	if( request_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_fixed_value: Invalid request string.\n" );

		return( NULL );
	}
	if( default_value >= amount )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_fixed_value: Default value cannot be larger than or equal as amount.\n" );

		return( NULL );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs_EWF " (", request_string );

		for( iterator = 0; iterator < amount; iterator++ )
		{
			if( iterator > 0 )
			{
				fprintf( stream, ", " );
			}
			fprintf( stream, "%" PRIs_EWF "", values[ iterator ] );
		}
		fprintf( stream, ") [%" PRIs_EWF "]: ", values[ default_value ] );

		user_input_buffer_ptr = libewf_string_get_from_stream( user_input_buffer_ptr, 1023, stdin );

		if( user_input_buffer_ptr != NULL )
		{
			iterator = 0;

			/* Remove the trailing newline character
			 */
			input_length = libewf_string_length( user_input_buffer_ptr ) - 1;

			/* Check if the default value was selected
			 */
			if( input_length == 0 )
			{
				iterator     = default_value;
				input_length = libewf_string_length( values[ iterator ] );
				value_match  = 1;
			}
			else
			{
				while( iterator < amount )
				{
					value_length = libewf_string_length( values[ iterator ] );

					if( libewf_string_compare( user_input_buffer_ptr, values[ iterator ], value_length ) == 0 )
					{
						/* Make sure no trailing characters were given
						 */
						if( user_input_buffer_ptr[ value_length ] == (LIBEWF_CHAR) '\n' )
						{
							value_match = 1;

							break;
						}
					}
					iterator++;
				}
			}
		}
		else
		{
			fprintf( stream, "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
		if( value_match == 1 )
		{
			value_length = libewf_string_length( values[ iterator ] );

			/* One additional character required for end of string
			 */
			user_input = libewf_string_duplicate( values[ iterator ], value_length );

			if( user_input == NULL )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_get_user_input_fixed_value: unable to create string.\n" );

				return( NULL );
			}
			break;
		}
		else
		{
			fprintf( stream, "Selected option not supported, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( user_input );
}

/* Finalize the SHA1 digest context and retrieve the SHA1 hash string
 * Returns 1 if successful, or -1 on errror
 */
int8_t ewfcommon_get_sha1_hash( EWFSHA1_CONTEXT *sha1_context, LIBEWF_CHAR *sha1_hash_string, size_t size )
{
	EWF_DIGEST_HASH *sha1_hash = NULL;
	size_t sha1_hash_size      = EWF_DIGEST_HASH_SIZE_SHA1;

	if( sha1_context == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_sha1_hash: invalid SHA1 digest context.\n" );

		return( -1 );
	}
	if( sha1_hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_sha1_hash: invalid SHA1 hash string.\n" );

		return( -1 );
	}
	if( size < LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_sha1_hash: SHA1 hash string too small.\n" );

		return( -1 );
	}
	sha1_hash = (EWF_DIGEST_HASH *) libewf_common_alloc( sha1_hash_size );

	if( sha1_hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_sha1_hash: unable to create SHA1 hash.\n" );

		return( -1 );
	}
	if( ( ewfsha1_finalize( sha1_context, sha1_hash, &sha1_hash_size ) != 1 )
	 || ( sha1_hash_size != EWF_DIGEST_HASH_SIZE_SHA1 ) )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_sha1_hash: unable to set SHA1 hash.\n" );

		libewf_common_free( sha1_hash );

		return( -1 );
	}
	if( libewf_string_copy_from_digest_hash( sha1_hash_string, size, sha1_hash, EWF_DIGEST_HASH_SIZE_SHA1 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_get_sha1_hash: unable to set SHA1 hash string.\n" );

		libewf_common_free( sha1_hash );

		return( -1 );
	}
	libewf_common_free( sha1_hash );

	return( 1 );
}

/* Print the version information to a stream
 */
void ewfcommon_version_fprint( FILE *stream, LIBEWF_CHAR *program )
{
	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_version_fprint: invalid stream.\n" );

		return;
	}
	if( program == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_version_fprint: invalid program name.\n" );

		return;
	}
	fprintf( stream, "%" PRIs_EWF " %" PRIs_EWF " (libewf %" PRIs_EWF ", zlib %s", program, LIBEWF_VERSION, LIBEWF_VERSION, ZLIB_VERSION );
#ifdef HAVE_LIBCRYPTO
	fprintf( stream, ", libcrypto %s", SHLIB_VERSION_NUMBER );
#endif
#ifdef HAVE_LIBUUID
	fprintf( stream, ", libuuid" );
#endif
	fprintf( stream, ")\n\n" );
}

/* Prints the executable version information
 */
void ewfcommon_copyright_fprint( FILE *stream )
{
	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_version_fprint: invalid stream.\n" );

		return;
	}
	fprintf( stream, "Copyright (c) 2006-2007, Joachim Metz, Hoffmann Investigations <%s> and contributors.\n", PACKAGE_BUGREPORT );
	fprintf( stream, "This is free software; see the source for copying conditions. There is NO\n" );
	fprintf( stream, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" );
}

/* Prints an overview of the aquiry parameters
 */
void ewfcommon_acquiry_paramters_fprint( FILE *stream, CHAR_T *filename, LIBEWF_CHAR *case_number, LIBEWF_CHAR *description, LIBEWF_CHAR *evidence_number, LIBEWF_CHAR *examiner_name, LIBEWF_CHAR *notes, uint8_t media_type, uint8_t volume_type, int8_t compression_level, uint8_t compress_empty_block, uint8_t libewf_format, uint64_t acquiry_offset, uint64_t acquiry_size, uint32_t segment_file_size, uint64_t sectors_per_chunk, uint32_t sector_error_granularity, uint8_t read_error_retry, uint8_t wipe_block_on_read_error )
{
	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_acquiry_paramters_fprint: invalid stream.\n" );

		return;
	}
	fprintf( stream, "Image path and filename:\t%" PRIs ".", filename );

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
		fprintf( stream, "%" PRIs_EWF "", case_number );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Description:\t\t\t" );

	if( description != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "", description );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Evidence number:\t\t" );

	if( evidence_number != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "", evidence_number );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Examiner name:\t\t\t" );

	if( examiner_name != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "", examiner_name );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Notes:\t\t\t\t" );

	if( notes != NULL )
	{
		fprintf( stream, "%" PRIs_EWF "", notes );
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
	fprintf( stream, "Acquiry start offet:\t\t%" PRIu64 "\n", acquiry_offset );
	fprintf( stream, "Amount of bytes to acquire:\t%" PRIu64 "", acquiry_size );

	if( acquiry_size == 0 )
	{
		fprintf( stream, " (until end of input)" );
	}
	fprintf( stream, "\n" );

	fprintf( stream, "Evidence segment file size:\t%" PRIu32 " kbytes\n", ( segment_file_size / 1024 ) );
	fprintf( stream, "Block size:\t\t\t%" PRIu64 " sectors\n", sectors_per_chunk );
	fprintf( stream, "Error granularity:\t\t%" PRIu32 " sectors\n", sector_error_granularity );
	fprintf( stream, "Retries on read error:\t\t%" PRIu8 "\n", read_error_retry );

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
void ewfcommon_acquiry_errors_fprint( FILE *stream, LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	uint64_t sector                         = 0;
	uint32_t amount_of_sectors              = 0;
	uint32_t iterator                       = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_acquiry_errors_fprint: invalid stream.\n" );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_acquiry_errors_fprint: invalid handle.\n" );

		return;
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->acquiry_amount_of_errors > 0 )
	{
		if( internal_handle->acquiry_error_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_acquiry_errors_fprint: missing acquiry read error sector list.\n" );

			return;
		}
		fprintf( stream, "Read errors during acquiry:\n" );
		fprintf( stream, "\ttotal amount: %" PRIu32 "\n", internal_handle->acquiry_amount_of_errors );
		
		for( iterator = 0; iterator < internal_handle->acquiry_amount_of_errors; iterator++ )
		{
			sector            = internal_handle->acquiry_error_sectors[ iterator ].sector;
			amount_of_sectors = internal_handle->acquiry_error_sectors[ iterator ].amount_of_sectors;

			fprintf( stream, "\tin sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n", sector, ( sector + amount_of_sectors ), amount_of_sectors );
		}
		fprintf( stream, "\n" );
	}
}

/* Print the read (CRC) errors to a stream
 */
void ewfcommon_crc_errors_fprint( FILE *stream, LIBEWF_HANDLE *handle )
{
	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	uint64_t sector                         = 0;
	uint32_t amount_of_sectors              = 0;
	uint32_t iterator                       = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_crc_errors_fprint: invalid stream.\n" );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_crc_errors_fprint: invalid handle.\n" );

		return;
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->read == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_crc_errors_fprint: invalid handle - missing subhandle read.\n" );

		return;
	}
	if( internal_handle->read->crc_amount_of_errors > 0 )
	{
		if( internal_handle->read->crc_error_sectors == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_crc_errors_fprint: missing CRC error sector list.\n" );

			return;
		}
		fprintf( stream, "Sector validation errors:\n" );
		fprintf( stream, "\ttotal amount: %" PRIu32 "\n", internal_handle->read->crc_amount_of_errors );

		for( iterator = 0; iterator < internal_handle->read->crc_amount_of_errors; iterator++ )
		{
			sector            = internal_handle->read->crc_error_sectors[ iterator ].sector;
			amount_of_sectors = internal_handle->read->crc_error_sectors[ iterator ].amount_of_sectors;

			fprintf( stream, "\tin sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n", sector, (sector + amount_of_sectors), amount_of_sectors );
		}
		fprintf( stream, "\n" );
	}
}

/* Print the header values to a stream
 */
void ewfcommon_header_values_fprint( FILE *stream, LIBEWF_HANDLE *handle )
{
	LIBEWF_CHAR header_value[ 128 ];

	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	uint32_t header_value_length            = 128;
	uint32_t iterator                       = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_header_values_fprint: invalid stream.\n" );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_header_values_fprint: invalid handle.\n" );

		return;
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	if( internal_handle->header_values == NULL )
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
	if( internal_handle->header_values->amount > LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		fprintf( stream, "\n\tAdditional values:\n" );

		for( iterator = LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT; iterator < internal_handle->header_values->amount; iterator++ )
		{
			if( libewf_get_header_value( handle, internal_handle->header_values->identifiers[ iterator ], header_value, header_value_length ) == 1 )
			{
				fprintf( stream, "\t%" PRIs_EWF ": %" PRIs_EWF "\n", internal_handle->header_values->identifiers[ iterator ], header_value );
			}
		}
	}
}

/* Print the hash values to a stream
 */
void ewfcommon_hash_values_fprint( FILE *stream, LIBEWF_HANDLE *handle )
{
	LIBEWF_CHAR hash_value[ 128 ];

	LIBEWF_INTERNAL_HANDLE *internal_handle = NULL;
	LIBEWF_CHAR *stored_md5_hash_string     = NULL;
	uint32_t hash_value_length              = 128;
	uint32_t iterator                       = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_hash_values_fprint: invalid stream.\n" );

		return;
	}
	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_hash_values_fprint: invalid handle.\n" );

		return;
	}
	internal_handle = (LIBEWF_INTERNAL_HANDLE *) handle;

	stored_md5_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

	if( ( stored_md5_hash_string != NULL ) && ( libewf_get_stored_md5_hash( handle, stored_md5_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 ) == 1 ) )
	{
		fprintf( stream, "\tMD5 hash in file:\t%" PRIs_EWF "\n", stored_md5_hash_string );

		libewf_common_free( stored_md5_hash_string );
	}
	else
	{
		fprintf( stream, "\tMD5 hash in file:\tN/A\n" );
	}
	if( ( libewf_parse_hash_values( handle ) == 1 ) && ( internal_handle->hash_values != NULL ) )
	{
		if( internal_handle->hash_values->amount > LIBEWF_HASH_VALUES_DEFAULT_AMOUNT )
		{
			fprintf( stream, "\n\tAdditional hash values:\n" );

			for( iterator = LIBEWF_HASH_VALUES_DEFAULT_AMOUNT; iterator < internal_handle->hash_values->amount; iterator++ )
			{
				if( libewf_get_hash_value( handle, internal_handle->hash_values->identifiers[ iterator ], hash_value, hash_value_length ) == 1 )
				{
					fprintf( stream, "\t%" PRIs_EWF ": %" PRIs_EWF "\n", internal_handle->hash_values->identifiers[ iterator ], hash_value );
				}
			}
		}
	}
}

/* Prints a time stamp (with a leading space) to a stream
 */
void ewfcommon_timestamp_fprint( FILE *stream, time_t timestamp )
{
	struct tm *time_elements = NULL;

	if( stream == NULL )
	{
		return;
	}
	time_elements = libewf_common_localtime( &timestamp );

	if( time_elements != NULL )
	{
		fprintf( stream, " in" );

		if( time_elements->tm_yday > 0 )
		{
			fprintf( stream, " %i day(s), %i hour(s), %i minute(s) and", time_elements->tm_yday, ( time_elements->tm_hour - 1 ), time_elements->tm_min );
		}
		else if( time_elements->tm_hour > 1 )
		{
			fprintf( stream, " %i hour(s), %i minute(s) and", ( time_elements->tm_hour - 1 ), time_elements->tm_min );
		}
		else if( time_elements->tm_min > 0 )
		{
			fprintf( stream, " %i minute(s) and", time_elements->tm_min );
		}
		fprintf( stream, " %i second(s)", time_elements->tm_sec );

		libewf_common_free( time_elements );
	}
}

/* Prints the amount of bytes per second (with a leading space) to a stream
 */
void ewfcommon_bytes_per_second_fprint( FILE *stream, uint64_t bytes, uint64_t seconds )
{
	LIBEWF_CHAR *bytes_per_second_string = NULL;
	uint64_t bytes_per_second            = 0;

	if( stream == NULL )
	{
		return;
	}
	if( seconds > 0 )
	{
		bytes_per_second        = bytes / seconds;
		bytes_per_second_string = ewfcommon_determine_human_readable_size_string( bytes_per_second );

		fprintf( stream, " with" );

		if( bytes_per_second_string != NULL )
		{
			fprintf( stream, " %" PRIs_EWF "/s (%" PRIu64 " bytes/second)", bytes_per_second_string, bytes_per_second );

			libewf_common_free( bytes_per_second_string );
		}
		else
		{
			fprintf( stream, " %" PRIu64 " bytes/second", bytes_per_second );
		}
	}
}

/* Prints the amount of bytes (with a leading space) to a stream
 * Creates a human readable version of the amount of bytes if possible
 */
void ewfcommon_bytes_fprint( FILE *stream, uint64_t bytes )
{
	LIBEWF_CHAR *bytes_string = NULL;

	if( stream == NULL )
	{
		return;
	}
	bytes_string = ewfcommon_determine_human_readable_size_string( bytes );

	if( bytes_string != NULL )
	{
		fprintf( stream, " %" PRIs_EWF " (%" PRIi64 " bytes)", bytes_string, bytes );

		libewf_common_free( bytes_string );
	}
	else
	{
		fprintf( stream, " %" PRIi64 " bytes", bytes );
	}
}

/* Static values for status information of the process
 */
FILE* ewfcommon_process_status_stream              = NULL;
LIBEWF_CHAR *ewfcommon_process_status_string       = NULL;
time_t ewfcommon_process_status_timestamp_start    = 0;
time_t ewfcommon_process_status_timestamp_last     = 0;
int8_t ewfcommon_process_status_last_percentage    = -1;
uint64_t ewfcommon_process_status_last_bytes_total = 0;

/* Initializes the status information of the process
 */
void ewfcommon_process_status_initialize( FILE *stream, LIBEWF_CHAR *string, time_t timestamp_start )
{
	ewfcommon_process_status_stream          = stream;
	ewfcommon_process_status_string          = string;
	ewfcommon_process_status_timestamp_start = timestamp_start;
	ewfcommon_process_status_timestamp_last  = timestamp_start;
}

/* Prints status information of the process
 */
void ewfcommon_process_status_fprint( uint64_t bytes_read, uint64_t bytes_total )
{
	time_t timestamp_current  = 0;
	int64_t seconds_remaining = 0;
	uint64_t seconds_current  = 0;
	uint64_t seconds_total    = 0;
	int8_t new_percentage     = 0;

	if( ewfcommon_process_status_stream == NULL )
	{
		return;
	}
	if( ewfcommon_process_status_string == NULL )
	{
		return;
	}
	if( ( bytes_total > 0 ) && ( bytes_read > 0 ) )
	{
		new_percentage = (int8_t) ( (uint64_t) ( bytes_read * 100 ) / bytes_total );
	}
	/* Estimate the remaining acquiry time
	 */
	timestamp_current = time( NULL );

	if( ( new_percentage > ewfcommon_process_status_last_percentage )
	 && ( timestamp_current > ewfcommon_process_status_timestamp_last ) )
	{
		ewfcommon_process_status_last_percentage = new_percentage;

		fprintf( ewfcommon_process_status_stream, "Status: at %" PRIu8 "%%.\n", new_percentage );
		fprintf( ewfcommon_process_status_stream, "        %" PRIs_EWF "", ewfcommon_process_status_string );

		ewfcommon_bytes_fprint( ewfcommon_process_status_stream, bytes_read );

		fprintf( ewfcommon_process_status_stream, " of total" );

		ewfcommon_bytes_fprint( ewfcommon_process_status_stream, bytes_total );

		fprintf( ewfcommon_process_status_stream, ".\n" );

		if( ( timestamp_current > ewfcommon_process_status_timestamp_start ) && ( new_percentage > 0 ) )
		{
			ewfcommon_process_status_timestamp_last = timestamp_current;

			seconds_current   = (uint64_t) difftime( timestamp_current, ewfcommon_process_status_timestamp_start );
			seconds_total     = ( (uint64_t) ( seconds_current * 100 ) / new_percentage );
			seconds_remaining = seconds_total - seconds_current;

			/* Negative time means nearly finished
			 */
			if( seconds_remaining < 0 )
			{
				seconds_remaining = 0;
			}
			fprintf( ewfcommon_process_status_stream, "        completion" );

			ewfcommon_timestamp_fprint( ewfcommon_process_status_stream, (time_t) seconds_remaining );
			ewfcommon_bytes_per_second_fprint( ewfcommon_process_status_stream, bytes_total, seconds_total );

			fprintf( ewfcommon_process_status_stream, ".\n" );
		}
		fprintf( ewfcommon_process_status_stream, "\n" );
	}
}

/* Prints status information of the stream process
 */
void ewfcommon_stream_process_status_fprint( uint64_t bytes_read, uint64_t bytes_total )
{
	time_t timestamp_current = 0;
	uint64_t seconds_current = 0;

	if( ewfcommon_process_status_stream == NULL )
	{
		return;
	}
	if( ewfcommon_process_status_string == NULL )
	{
		return;
	}
	timestamp_current = time( NULL );

	if( timestamp_current > ewfcommon_process_status_timestamp_last )
	{
		/* Update state
		 * - if no status was printed before
		 * - or input has grown > 10 Mb
		 * - or the last update was 30 seconds ago
		 */
		if( ( ewfcommon_process_status_last_bytes_total == 0 )
		 || ( bytes_read > ( ewfcommon_process_status_last_bytes_total + ( 10 * 1024 * 1024 ) ) )
		 || ( ( timestamp_current - ewfcommon_process_status_timestamp_last ) > 30 ) )
		{
			ewfcommon_process_status_timestamp_last   = timestamp_current;
			ewfcommon_process_status_last_bytes_total = bytes_read;

			fprintf( ewfcommon_process_status_stream, "Status: %" PRIs_EWF "", ewfcommon_process_status_string );

			ewfcommon_bytes_fprint( ewfcommon_process_status_stream, bytes_read );

			fprintf( ewfcommon_process_status_stream, "\n" );

			seconds_current = (uint64_t) difftime( timestamp_current, ewfcommon_process_status_timestamp_start );

			fprintf( ewfcommon_process_status_stream, "       " );

			ewfcommon_timestamp_fprint( ewfcommon_process_status_stream, (time_t) seconds_current );
			ewfcommon_bytes_per_second_fprint( ewfcommon_process_status_stream, bytes_read, seconds_current );

			fprintf( ewfcommon_process_status_stream, ".\n\n" );
		}
	}
}

/* Prints summary information of the process
 */
void ewfcommon_process_summary_fprint( FILE *stream, LIBEWF_CHAR *string, int64_t byte_count, time_t timestamp_start, time_t timestamp_end )
{
	time_t timestamp_acquiry = 0;
	uint64_t seconds_acquiry = 0;

	if( stream == NULL )
	{
		return;
	}
	if( string == NULL )
	{
		return;
	}
	timestamp_acquiry = timestamp_end - timestamp_start;
	seconds_acquiry   = (uint64_t) difftime( timestamp_end, timestamp_start );

	fprintf( stream, "%" PRIs_EWF ":", string );

	ewfcommon_bytes_fprint( stream, byte_count );
	ewfcommon_timestamp_fprint( stream, timestamp_acquiry );
	ewfcommon_bytes_per_second_fprint( stream, byte_count, seconds_acquiry );

	fprintf( stream, ".\n" );
}

/* Reads data from a file descriptor into the chunk cache
 * Returns the amount of bytes read, 0 if at end of input, or -1 on error
 */
int32_t ewfcommon_read_input( LIBEWF_HANDLE *handle, int file_descriptor, EWF_CHUNK *buffer, uint64_t size, int64_t total_read_count, uint64_t total_input_size, uint8_t read_error_retry, uint32_t sector_error_granularity, uint8_t wipe_block_on_read_error, uint8_t seek_on_error )
{
#if defined(HAVE_STRERROR_R) || defined(HAVE_STRERROR)
	CHAR_T *error_string              = NULL;
#endif
	ssize_t read_count                = 0;
	size_t read_size                  = 0;
	size_t bytes_to_read              = 0;
	size_t read_remaining_bytes       = 0;
	size_t error_remaining_bytes      = 0;
	int64_t current_read_offset       = 0;
	int64_t current_calculated_offset = 0;
	int64_t chunk_amount              = 0;
	int64_t buffer_offset             = 0;
	uint64_t error2_sector            = 0;
	int32_t read_amount_of_errors     = 0;
	uint32_t chunk_size               = 0;
	uint32_t bytes_per_sector         = 0;
	uint32_t read_error_offset        = 0;
	uint32_t error_skip_bytes         = 0;
	uint32_t error_granularity_offset = 0;
	uint32_t error2_amount_of_sectors = 0;
	uint32_t byte_error_granularity   = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: invalid handle.\n" );

		return( -1 );
	}
	if( buffer == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: invalid read buffer.\n" );

		return( -1 );
	}
	chunk_size = libewf_get_chunk_size( handle );

	if( chunk_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: unable to determine chunk media.\n" );

		return( -1 );
	}
	if( size > (uint64_t) INT64_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: invalid size - size larger than 2^63 not supported.\n" );

		return( -1 );
	}
	if( file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: invalid file descriptor.\n" );

		return( -1 );
	}
	if( total_read_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: invalid total read count.\n" );

		return( -1 );
	}
	bytes_per_sector = libewf_get_bytes_per_sector( handle );

	if( bytes_per_sector == 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: invalid amount of bytes per sector.\n" );

		return( -1 );
	}
	chunk_amount = libewf_get_write_amount_of_chunks( handle );

	if( ( chunk_amount <= -1 ) || ( chunk_amount > (int64_t) UINT32_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_input: invalid amount of chunks written.\n" );

		return( -1 );
	}
	while( size > 0 )
	{
		/* Determine the amount of bytes to read from the input
		 * Read as much as possible in chunk sizes
		 */
		if( size < (uint64_t) chunk_size )
		{
			read_size = (uint32_t) size;
		}
		else
		{
			read_size = chunk_size;
		}
		bytes_to_read = read_size;

		while( read_amount_of_errors <= read_error_retry )
		{
			read_count = libewf_common_read( file_descriptor, &buffer[ buffer_offset + read_error_offset ], bytes_to_read );

			LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: read chunk: %" PRIi64 " with size: %zi.\n", ( chunk_amount + 1 ), read_count );

			current_calculated_offset = (off_t) ( total_read_count + buffer_offset + read_error_offset );

			if( read_count <= -1 )
			{
#if defined(HAVE_STRERROR_R) || defined(HAVE_STRERROR)
				if( ( errno == ESPIPE ) || ( errno == EPERM ) || ( errno == ENXIO ) || ( errno == ENODEV ) )
				{
					error_string = libewf_common_strerror( errno );

					if( error_string != NULL )
					{
						LIBEWF_WARNING_PRINT( "ewfcommon_read_input: error reading data: %s.\n", error_string );

						libewf_common_free( error_string );
					}
					return( -1 );
				}
#else
				if( errno == ESPIPE )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_read_input: error reading data: Invalid seek.\n" );

					return( -1 );
				}
				else if( errno == EPERM )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_read_input: error reading data: Operation not permitted.\n" );

					return( -1 );
				}
				else if( errno == ENXIO )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_read_input: error reading data: No such device or address.\n" );

					return( -1 );
				}
				else if( errno == ENODEV )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_read_input: error reading data: No such device.\n" );

					return( -1 );
				}
#endif
				if( seek_on_error == 1 )
				{
					current_read_offset = libewf_common_lseek( file_descriptor, 0, SEEK_CUR );

					if( current_read_offset != current_calculated_offset )
					{
						LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: correcting offset drift current: %jd, calculated: %jd.\n",
						                      current_read_offset, current_calculated_offset );

						if( current_read_offset < current_calculated_offset )
						{
							LIBEWF_WARNING_PRINT( "ewfcommon_read_input: unable to correct offset drift.\n" );

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
				if( read_count == (int32_t) bytes_to_read )
				{
					read_count = read_error_offset + bytes_to_read;
				}
				/* The entire read is OK
				 */
				if( read_count == (int32_t) read_size )
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
						return( (int32_t) ( buffer_offset + read_count ) );
					}
				}
				else
				{
					/* Check if the end of the input was reached
					 */
					if( ( total_read_count + buffer_offset + read_count ) >= (int64_t) total_input_size )
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
				LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: read error at offset %jd after reading %zi bytes.\n",
				                      current_calculated_offset, read_count );

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
					LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: unable to handle more input.\n" );

					return( 0 );
				}
				current_calculated_offset = total_read_count + buffer_offset;

				/* Check if last chunk is smaller than the chunk size and take corrective measures
				 */
				if( ( total_input_size != 0 ) && ( ( current_calculated_offset + chunk_size ) > (int64_t) total_input_size ) )
				{
					read_remaining_bytes = (size_t) ( total_input_size - current_calculated_offset );
				}
				else
				{
					read_remaining_bytes = (size_t) chunk_size;
				}
				if( read_remaining_bytes > (size_t) SSIZE_MAX )
				{
					LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: invalid remaining bytes value exceeds maximum.\n" );

					return( -1 );
				}
				byte_error_granularity   = sector_error_granularity * bytes_per_sector;
				error_remaining_bytes    = read_remaining_bytes - read_error_offset;
				error2_sector            = current_calculated_offset;
				error_granularity_offset = ( read_error_offset / byte_error_granularity ) * byte_error_granularity;
				error_skip_bytes         = ( error_granularity_offset + byte_error_granularity ) - read_error_offset;

				if( wipe_block_on_read_error == 1 )
				{
					LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: wiping block of %" PRIu32 " bytes at offset %" PRIu32 ".\n", byte_error_granularity, error_granularity_offset );

					if( libewf_common_memset( &buffer[ error_granularity_offset ], 0, byte_error_granularity ) == NULL )
					{
						LIBEWF_WARNING_PRINT( "ewfcommon_read_input: unable to wipe data in chunk on error.\n" );

						return( -1 );
					}
					error2_sector            += error_granularity_offset;
					error2_amount_of_sectors  = byte_error_granularity;
				}
				else
				{
					error2_sector            += read_error_offset;
					error2_amount_of_sectors  = error_skip_bytes;
				}
				error2_sector            /= bytes_per_sector;
				error2_amount_of_sectors /= bytes_per_sector;

				if( libewf_add_acquiry_error( handle, error2_sector, error2_amount_of_sectors ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_read_input: unable to add acquiry read errror sectors.\n" );

					return( -1 );
				}
				LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: adding error2: %" PRIu32 " sector: %" PRIu64 ", count: %" PRIu32 ".\n", ( (LIBEWF_INTERNAL_HANDLE *) handle )->acquiry_amount_of_errors, error2_sector, error2_amount_of_sectors );

				LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: skipping %" PRIu32 " bytes.\n", error_skip_bytes );

				/* At the end of the input
				 */
				if( ( total_input_size != 0 ) && ( ( current_calculated_offset + read_remaining_bytes ) >= (int64_t) total_input_size ) )
				{
					LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: at end of input no remaining bytes to read from chunk.\n" );

					read_count = (ssize_t) read_remaining_bytes;

					break;
				}
				if( libewf_common_lseek( file_descriptor, error_skip_bytes, SEEK_CUR ) == -1 )
				{
#if defined(HAVE_STRERROR_R) || defined(HAVE_STRERROR)
					error_string = libewf_common_strerror( errno );

					if( error_string != NULL )
					{
						LIBEWF_WARNING_PRINT( "ewfcommon_read_input: unable skip %" PRIu32 " bytes after sector with error - %s.\n", error_skip_bytes, error_string );

						libewf_common_free( error_string );
					}
#else
					LIBEWF_WARNING_PRINT( "ewfcommon_read_input: unable skip %" PRIu32 " bytes after sector with error.\n", error_skip_bytes );
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

					LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: remaining to read from chunk %" PRIu32 " bytes.\n", bytes_to_read );
				}
				else
				{
					read_count = (ssize_t) read_remaining_bytes;

					LIBEWF_VERBOSE_PRINT( "ewfcommon_read_input: no remaining bytes to read from chunk.\n" );
					break;
				}
			}
		}
		size          -= read_count;
		buffer_offset += read_count;

		/* At the end of the input
		 */
		if( ( total_input_size != 0 ) && ( ( total_read_count + buffer_offset ) >= (int64_t) total_input_size ) )
		{
			break;
		}
	}
	return( (int32_t) buffer_offset );
}

/* Reads the data (to nowhere)
 * This processes the entire file for the MD5 calculation with status information
 * Returns a -1 on error, the amount of bytes read on success
 */
int64_t ewfcommon_read( LIBEWF_HANDLE *handle, uint8_t calculate_sha1, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) )
{
	EWFSHA1_CONTEXT sha1_context;

	LIBEWF_CHAR *sha1_hash_string       = NULL;
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	uint8_t *data                       = NULL;
#endif
	off_t read_offset                   = 0;
	ssize_t read_count                  = 0;
	size_t size                         = 0;
	size_t buffer_size                  = 0;
	int64_t total_read_count            = 0;
	uint64_t media_size                 = 0;
	uint32_t chunk_size                 = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read: invalid handle.\n" );

		return( -1 );
	}
	media_size = libewf_get_media_size( handle );

	if( media_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read: unable to determine media size.\n" );

		return( -1 );
	}
	chunk_size = libewf_get_chunk_size( handle );

	if( chunk_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read: unable to determine chunk size.\n" );

		return( -1 );
	}
	if( chunk_size > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read: invalid chunk size only values below 2^32 are supported.\n" );

		return( -1 );
	}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	buffer_size = EWFCOMMON_BUFFER_SIZE;
	data        = (uint8_t *) libewf_common_alloc( buffer_size * sizeof( uint8_t ) );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read: unable to allocate data.\n" );

		return( -1 );
	}
#else
	buffer_size = (size_t) chunk_size;
#endif
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_initialize( &sha1_context ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read: unable to initialize SHA1 digest context.\n" );

			return( -1 );
		}
	}
	while( total_read_count < (int64_t) media_size )
	{
		size = buffer_size;

		if( ( media_size - total_read_count ) < (uint64_t) size )
		{
			size = (size_t) ( media_size - total_read_count );
		}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
		read_count = libewf_read_random( handle, data, size, read_offset );
#else
		read_count = libewf_read_random( handle, ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->data, size, read_offset );
#endif

		if( read_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read: error reading data.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		if( read_count == 0 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read: unexpected end of data.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		if( read_count > (ssize_t) size )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read: more bytes read than requested.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		if( calculate_sha1 == 1 )
		{
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			ewfsha1_update( &sha1_context, data, read_count );
#else
			ewfsha1_update( &sha1_context, ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->data, read_count );
#endif
		}
		read_offset      += (off_t) size;
		total_read_count += (int64_t) read_count;

		if( callback != NULL )
		{
			callback( total_read_count, media_size );
		}
  	}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	libewf_common_free( data );
#endif

	if( calculate_sha1 == 1 )
	{
		sha1_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

		if( sha1_hash_string == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read: unable to create SHA1 hash string.\n" );

			return( -1 );
		}
		if( ewfcommon_get_sha1_hash( &sha1_context, sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read: unable to set SHA1 hash string.\n" );

			libewf_common_free( sha1_hash_string );

			return( -1 );
		}
		if( libewf_set_hash_value( handle, _S_LIBEWF_CHAR( "ewfcommon_calculated_SHA1" ), sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read: unable to set SHA1 hash string in handle.\n" );

			libewf_common_free( sha1_hash_string );

			return( -1 );
		}
		libewf_common_free( sha1_hash_string );
	}
	return( total_read_count );
}

/* Reads the data to a file descriptor
 * Returns a -1 on error, the amount of bytes read on success
 */
int64_t ewfcommon_read_to_file_descriptor( LIBEWF_HANDLE *handle, int output_file_descriptor, uint64_t read_size, uint64_t read_offset, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) )
{
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	uint8_t *data            = NULL;
#endif
	ssize_t read_count       = 0;
	ssize_t write_count      = 0;
	size_t size              = 0;
	size_t buffer_size       = 0;
	int64_t total_read_count = 0;
	uint64_t media_size      = 0;
	uint32_t chunk_size      = 0;
	uint8_t read_all         = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: invalid handle.\n" );

		return( -1 );
	}
	media_size = libewf_get_media_size( handle );

	if( media_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: unable to determine media size.\n" );

		return( -1 );
	}
	chunk_size = libewf_get_chunk_size( handle );

	if( chunk_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: unable to determine chunk size.\n" );

		return( -1 );
	}
	if( ( read_size == 0 ) || ( read_size > media_size ) || ( read_size > (uint64_t) INT64_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: invalid size.\n" );

		return( -1 );
	}
	if( read_offset >= media_size )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: invalid offset.\n" );

		return( -1 );
	}
	if( ( read_size + read_offset ) > media_size )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: unable to export beyond size of media.\n" );

		return( -1 );
	}
	read_all = (uint8_t) ( ( read_size == media_size ) && ( read_offset == 0 ) );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	buffer_size = EWFCOMMON_BUFFER_SIZE;
	data        = (uint8_t *) libewf_common_alloc( buffer_size * sizeof( uint8_t ) );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: unable to allocate data.\n" );

		return( -1 );
	}
#else
	buffer_size = chunk_size;
#endif
	while( total_read_count < (int64_t) read_size )
	{
		size = buffer_size;

		if( ( media_size - total_read_count ) < (uint64_t) size )
		{
			size = (size_t) ( media_size - total_read_count );
		}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
		read_count = libewf_read_random( handle, data, size, (off_t) read_offset );
#else
		read_count = libewf_read_random( handle, ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->data, size, (off_t) read_offset );
#endif

		if( read_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: error reading data.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		if( read_count == 0 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: unexpected end of data.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		if( read_count > (ssize_t) size )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: more bytes read than requested.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		read_offset += size;

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
		write_count = libewf_common_write( output_file_descriptor, data, (size_t) read_count );
#else
		write_count = libewf_common_write( output_file_descriptor, ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->data, (size_t) read_count );
#endif

		if( write_count < read_count )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_read_to_file_descriptor: error writing data.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		total_read_count += read_count;

		if( callback != NULL )
		{
			callback( total_read_count, read_size );
		}
  	}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	libewf_common_free( data );
#endif

	return( total_read_count );
}

/* Writes data in EWF format from a file descriptor
 * Returns the amount of bytes written, or -1 on error
 */
int64_t ewfcommon_write_from_file_descriptor( LIBEWF_HANDLE *handle, int input_file_descriptor, uint64_t write_size, uint64_t write_offset, uint8_t read_error_retry, uint32_t sector_error_granularity, uint8_t wipe_block_on_read_error, uint8_t seek_on_error, uint8_t calculate_sha1, void (*callback)( uint64_t bytes_read, uint64_t bytes_total ) )
{
	EWFSHA1_CONTEXT sha1_context;

	LIBEWF_CHAR *sha1_hash_string       = NULL;
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	uint8_t *data                       = 0;
#endif
	size_t buffer_size                  = 0;
	int64_t total_write_count           = 0;
	int64_t write_count                 = 0;
	uint32_t chunk_size                 = 0;
	int32_t read_count                  = 0;

	if( handle == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: invalid handle.\n" );

		return( -1 );
	}
	if( input_file_descriptor == -1 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: invalid file descriptor.\n" );

		return( -1 );
	}
	chunk_size = libewf_get_chunk_size( handle );

	if( chunk_size == 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to determine chunk media.\n" );

		return( -1 );
	}
	if( write_size > 0 )
	{
		if( libewf_set_write_input_size( handle, write_size ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to set input write size in handle.\n" );

			return( -1 );
		}
		if( write_offset > 0 )
		{
			if( write_offset >= write_size )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: invalid offset to write.\n" );

				return( -1 );
			}
			if( libewf_common_lseek( input_file_descriptor, write_offset, SEEK_SET ) != (int64_t) write_offset )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to find write offset.\n" );

				return( -1 );
			}
		}
	}
	else if( write_offset > 0 )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: ignoring write offset in a stream mode.\n" );
	}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	buffer_size = EWFCOMMON_BUFFER_SIZE;
	data        = (uint8_t *) libewf_common_alloc( buffer_size * sizeof( uint8_t ) );

	if( data == NULL )
	{
		LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to allocate data.\n" );

		return( -1 );
	}
#else
	if( chunk_size > ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->allocated_size )
	{
		/* Add 4 bytes for CRC
		 */
		if( libewf_internal_handle_chunk_cache_realloc( (LIBEWF_INTERNAL_HANDLE *) handle, ( chunk_size + EWF_CRC_SIZE ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to reallocate chunk cache.\n" );

			return( -1 );
		}
	}
	buffer_size = (size_t) chunk_size;
#endif
	if( calculate_sha1 == 1 )
	{
		if( ewfsha1_initialize( &sha1_context ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to initialize SHA1 digest context.\n" );

			return( -1 );
		}
	}
	while( ( write_size == 0 ) || ( total_write_count < (int64_t) write_size ) )
	{
		/* Read a chunk from the file descriptor
		 */
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
		read_count = ewfcommon_read_input( handle, input_file_descriptor, data, buffer_size, total_write_count, write_size, read_error_retry, sector_error_granularity, wipe_block_on_read_error, seek_on_error );
#else
		read_count = ewfcommon_read_input( handle, input_file_descriptor, ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->data, buffer_size, total_write_count, write_size, read_error_retry, sector_error_granularity, wipe_block_on_read_error, seek_on_error );
#endif

		if( read_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to read chunk from file.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		if( read_count == 0 )
		{
			if( write_size != 0 )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unexpected end of input.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
				libewf_common_free( data );
#endif
				return( -1 );
			}
			break;
		}
		if( calculate_sha1 == 1 )
		{
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			ewfsha1_update( &sha1_context, data, read_count );
#else
			ewfsha1_update( &sha1_context, ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->data, read_count );
#endif
		}
		if( ( write_size != 0 ) && ( ( total_write_count + read_count ) == (int64_t) write_size ) )
		{
			if( calculate_sha1 == 1 )
			{
				sha1_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

				if( sha1_hash_string == NULL )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to create SHA1 hash string.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
					libewf_common_free( data );
#endif
					return( -1 );
				}
				if( ewfcommon_get_sha1_hash( &sha1_context, sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to set SHA1 hash string.\n" );

					libewf_common_free( sha1_hash_string );
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
					libewf_common_free( data );
#endif
					return( -1 );
				}
				/* The SHA1 hash must be set before the last chunk is written
				 */
				if( libewf_set_hash_value( handle, _S_LIBEWF_CHAR( "SHA1" ), sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 ) != 1 )
				{
					LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to set SHA1 hash string in handle.\n" );

					libewf_common_free( sha1_hash_string );
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
					libewf_common_free( data );
#endif
					return( -1 );
				}
				libewf_common_free( sha1_hash_string );
			}
		}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
		write_count = libewf_write_buffer( handle, data, read_count );
#else
		write_count = libewf_write_buffer( handle, ( (LIBEWF_INTERNAL_HANDLE *) handle )->chunk_cache->data, read_count );
#endif

		if( write_count != read_count )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to write chunk to file.\n" );

#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
			libewf_common_free( data );
#endif
			return( -1 );
		}
		total_write_count += write_count;

		/* Callback for status update
		 */
		if( callback != NULL )
		{
			callback( (uint64_t) total_write_count, write_size );
		}
	}
#ifndef HAVE_CHUNK_CACHE_PASSTHROUGH
	libewf_common_free( data );
#endif
	if( write_size == 0 )
	{
		if( calculate_sha1 == 1 )
		{
			sha1_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

			if( sha1_hash_string == NULL )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to create SHA1 hash string.\n" );

				return( -1 );
			}
			if( ewfcommon_get_sha1_hash( &sha1_context, sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to set SHA1 hash string.\n" );

				libewf_common_free( sha1_hash_string );

				return( -1 );
			}
			/* The SHA1 hash must be set before write finalized is used
			 */
			if( libewf_set_hash_value( handle, _S_LIBEWF_CHAR( "SHA1" ), sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 ) != 1 )
			{
				LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to set SHA1 hash string in handle.\n" );

				libewf_common_free( sha1_hash_string );

				return( -1 );
			}
			libewf_common_free( sha1_hash_string );
		}
		write_count = libewf_write_finalize( handle );

		if( write_count == -1 )
		{
			LIBEWF_WARNING_PRINT( "ewfcommon_write_from_file_descriptor: unable to finalize EWF file(s).\n" );

			return( -1 );
		}
		total_write_count += write_count;
	}
	return( total_write_count );
}


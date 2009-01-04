/*
 * ewfinput
 * User input functions for the ewf tools
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

#include "../libewf/libewf_includes.h"

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "../libewf/libewf_common.h"
#include "../libewf/libewf_notify.h"
#include "../libewf/libewf_string.h"

#include "ewfinput.h"

/* Input selection defintions
 */
libewf_char_t *ewfinput_compression_levels[ 3 ] = \
 { _S_LIBEWF_CHAR( "none" ),
   _S_LIBEWF_CHAR( "fast" ),
   _S_LIBEWF_CHAR( "best" ) };

libewf_char_t *ewfinput_format_types[ 12 ] = \
 { _S_LIBEWF_CHAR( "ewf" ),
   _S_LIBEWF_CHAR( "smart" ),
   _S_LIBEWF_CHAR( "ftk" ),
   _S_LIBEWF_CHAR( "encase1" ),
   _S_LIBEWF_CHAR( "encase2" ),
   _S_LIBEWF_CHAR( "encase3" ),
   _S_LIBEWF_CHAR( "encase4" ),
   _S_LIBEWF_CHAR( "encase5" ),
   _S_LIBEWF_CHAR( "encase6" ),
   _S_LIBEWF_CHAR( "linen5" ),
   _S_LIBEWF_CHAR( "linen6" ),
   _S_LIBEWF_CHAR( "ewfx" ) };

libewf_char_t *ewfinput_media_types[ 2 ] = \
 { _S_LIBEWF_CHAR( "fixed" ),
   _S_LIBEWF_CHAR( "removable" ) };

libewf_char_t *ewfinput_volume_types[ 2 ] = \
 { _S_LIBEWF_CHAR( "logical" ),
   _S_LIBEWF_CHAR( "physical" ) };

libewf_char_t *ewfinput_sector_per_block_sizes[ 10 ] = \
 { _S_LIBEWF_CHAR( "64" ),
   _S_LIBEWF_CHAR( "128" ),
   _S_LIBEWF_CHAR( "256" ),
   _S_LIBEWF_CHAR( "512" ),
   _S_LIBEWF_CHAR( "1024" ),
   _S_LIBEWF_CHAR( "2048" ),
   _S_LIBEWF_CHAR( "4096" ),
   _S_LIBEWF_CHAR( "8192" ),
   _S_LIBEWF_CHAR( "16384" ),
   _S_LIBEWF_CHAR( "32768" ) };

libewf_char_t *ewfinput_yes_no[ 2 ] = \
 { _S_LIBEWF_CHAR( "yes" ),
   _S_LIBEWF_CHAR( "no" ) };

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint8_t ewfinput_determine_libewf_format( const libewf_char_t *argument )
{
	static char *function = "ewfinput_determine_libewf_format";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "smart" ), 3 ) == 0 )
	{
		return( LIBEWF_FORMAT_SMART );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "ftk" ), 3 ) == 0 )
	{
		return( LIBEWF_FORMAT_FTK );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "encase1" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE1 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "encase2" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE2 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "encase3" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE3 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "encase4" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE4 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "encase5" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE5 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "encase6" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE6 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "linen5" ), 6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN5 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "linen6" ), 6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN6 );
	}
	/* This check must before the check for "ewf"
	 */
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "ewfx" ), 4 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWFX );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "ewf" ), 3 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWF );
	}
	return( 0 );
}

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint8_t ewfinput_determine_libewf_format_char_t( const CHAR_T *argument )
{
	static char *function = "ewfinput_determine_libewf_format_char_t";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	if( CHAR_T_COMPARE( argument, _S_CHAR_T( "smart" ), 3 ) == 0 )
	{
		return( LIBEWF_FORMAT_SMART );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "ftk" ), 3 ) == 0 )
	{
		return( LIBEWF_FORMAT_FTK );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "encase1" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE1 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "encase2" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE2 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "encase3" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE3 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "encase4" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE4 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "encase5" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE5 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "encase6" ), 7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE6 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "linen5" ), 6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN5 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "linen6" ), 6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN6 );
	}
	/* This check must before the check for "ewf"
	 */
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "ewfx" ), 4 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWFX );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "ewf" ), 3 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWF );
	}
	return( 0 );
}

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint32_t ewfinput_determine_sectors_per_chunk( const libewf_char_t *argument )
{
	static char *function = "ewfinput_determine_sectors_per_chunk";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "32768" ), 5 ) == 0 )
	{
		return( 32768 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "16384" ), 5 ) == 0 )
	{
		return( 16384 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "8192" ), 4 ) == 0 )
	{
		return( 8192 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "4096" ), 4 ) == 0 )
	{
		return( 4096 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "2048" ), 4 ) == 0 )
	{
		return( 2048 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "1024" ), 4 ) == 0 )
	{
		return( 1024 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "512" ), 3 ) == 0 )
	{
		return( 512 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "256" ), 3 ) == 0 )
	{
		return( 256 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "128" ), 3 ) == 0 )
	{
		return( 128 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "64" ), 2 ) == 0 )
	{
		return( 64 );
	}
	return( 0 );
}

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint32_t ewfinput_determine_sectors_per_chunk_char_t( const CHAR_T *argument )
{
	static char *function = "ewfinput_determine_sectors_per_chunk_char_t";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "32768" ), 5 ) == 0 )
	{
		return( 32768 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "16384" ), 5 ) == 0 )
	{
		return( 16384 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "8192" ), 4 ) == 0 )
	{
		return( 8192 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "4096" ), 4 ) == 0 )
	{
		return( 4096 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "2048" ), 4 ) == 0 )
	{
		return( 2048 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "1024" ), 4 ) == 0 )
	{
		return( 1024 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "512" ), 3 ) == 0 )
	{
		return( 512 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "256" ), 3 ) == 0 )
	{
		return( 256 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "128" ), 3 ) == 0 )
	{
		return( 128 );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "64" ), 2 ) == 0 )
	{
		return( 64 );
	}
	return( 0 );
}

/* Determines the compression level value from an argument string
 * Returns the compression level value, or -1 on error
 */
int8_t ewfinput_determine_compression_level( const libewf_char_t *argument )
{
	static char *function = "ewfinput_determine_compression_level";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "none" ), 4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_NONE );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "fast" ), 4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_FAST );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "best" ), 4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_BEST );
	}
	return( -1 );
}

/* Determines the compression level value from an argument string
 * Returns the compression level value, or -1 on error
 */
int8_t ewfinput_determine_compression_level_char_t( const CHAR_T *argument )
{
	static char *function = "ewfinput_determine_compression_level_char_t";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	if( CHAR_T_COMPARE( argument, _S_CHAR_T( "none" ), 4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_NONE );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "fast" ), 4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_FAST );
	}
	else if( CHAR_T_COMPARE( argument, _S_CHAR_T( "best" ), 4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_BEST );
	}
	return( -1 );
}

/* Determines the media type value from an argument string
 * Returns the media type value, or -1 on error
 */
int8_t ewfinput_determine_media_type( const libewf_char_t *argument )
{
	static char *function = "ewfinput_determine_media_type";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "fixed" ), 5 ) == 0 )
	{
		return( LIBEWF_MEDIA_TYPE_FIXED );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "removable" ), 9 ) == 0 )
	{
		return( LIBEWF_MEDIA_TYPE_REMOVABLE );
	}
	return( -1 );
}

/* Determines the volume type value from an argument string
 * Returns the volume type value, or -1 on error
 */
int8_t ewfinput_determine_volume_type( const libewf_char_t *argument )
{
	static char *function = "ewfinput_determine_volume_type";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "logical" ), 7 ) == 0 )
	{
		return( LIBEWF_VOLUME_TYPE_LOGICAL );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "physical" ), 8 ) == 0 )
	{
		return( LIBEWF_VOLUME_TYPE_PHYSICAL );
	}
	return( -1 );
}

/* Determines the yes or no value from an argument string
 * Returns 1 if yes, 0 if no, or -1 on error
 */
int8_t ewfinput_determine_yes_no( const libewf_char_t *argument )
{
	static char *function = "ewfinput_determine_yes_no";

	if( argument == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "yes" ), 3 ) == 0 )
	{
		return( 1 );
	}
	else if( libewf_string_compare( argument, _S_LIBEWF_CHAR( "no" ), 2 ) == 0 )
	{
		return( 0 );
	}
	return( -1 );
}

/* Get variable input from the user
 * with a maximum of 1023 characters
 */
libewf_char_t *ewfinput_get_variable( FILE *stream, libewf_char_t *request_string )
{
	libewf_char_t user_input_buffer[ 1024 ];

	libewf_char_t *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	libewf_char_t *user_input            = NULL;
	libewf_char_t *end_of_input          = NULL;
	static char *function                = "ewfinput_get_variable";
	size_t input_length                  = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid output stream.\n",
		 function );

		return( NULL );
	}
	if( request_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid request string.\n",
		 function );

		return( NULL );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs_EWF ": ", request_string );

		user_input_buffer_ptr = libewf_string_get_from_stream( user_input_buffer_ptr, 1023, stdin );

		if( user_input_buffer_ptr != NULL )
		{
			end_of_input = libewf_string_search( user_input_buffer_ptr, (libewf_char_t) '\n', 1023 );

			if( end_of_input == NULL )
			{
				return( NULL );
			}
			input_length = (uint32_t) ( end_of_input - user_input_buffer_ptr );

			if( input_length <= 0 )
			{
				return( NULL );
			}
#if defined( MEMWATCH )
			/* One additional character required for end of string
			 */
			user_input = libewf_common_alloc( sizeof( libewf_char_t ) * ( input_length + 1 ) );

			if( user_input == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create string.\n",
				 function );

				return( NULL );
			}
			if( libewf_string_copy( user_input, user_input_buffer_ptr, input_length ) == NULL  )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to copy string.\n",
				 function );

				libewf_common_free( user_input );

				return( NULL );
			}
			/* Make sure the string is \0 terminated
			 */
			user_input[ input_length ] = (libewf_char_t) '\0';
#else
			user_input = libewf_string_duplicate( user_input_buffer_ptr, input_length );

			if( user_input == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create string.\n",
				 function );

				return( NULL );
			}
#endif
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
CHAR_T *ewfinput_get_variable_char_t( FILE *stream, libewf_char_t *request_string )
{
	libewf_char_t *user_input = NULL;
	CHAR_T *user_input_char_t = NULL;
	static char *function     = "ewfinput_get_variable_char_t";
	size_t user_input_length  = 0;

	user_input = ewfinput_get_variable( stream, request_string );

	if( sizeof( CHAR_T ) == sizeof( libewf_char_t ) )
	{
		return( (CHAR_T *) user_input );
	}
	if( sizeof( CHAR_T ) < sizeof( libewf_char_t ) )
	{
		user_input_length = libewf_string_length(
		                     user_input );

		user_input_char_t = libewf_common_alloc(
		                     ( user_input_length + 1 ) * sizeof( CHAR_T ) );

		if( user_input_char_t == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create conversion string.\n",
			 function );

			libewf_common_free(
			 user_input );

			return( NULL );
		}
		if( ewfstring_copy_libewf_char_to_char_t(
		     user_input,
		     user_input_char_t,
		     ( user_input_length + 1 ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set conversion string.\n",
			 function );

			libewf_common_free(
			 user_input );
			libewf_common_free(
			 user_input_char_t );

			return( NULL );
		}
		libewf_common_free( user_input );

		return( user_input_char_t );
	}
	LIBEWF_WARNING_PRINT( "%s: character conversion unsupported.\n",
	 function );

	return( NULL );
}

/* Get variable containing a size definnition input from the user
 * with a maximum of 1023 characters
 */
uint64_t ewfinput_get_size_variable( FILE *stream, libewf_char_t *request_string, uint64_t minimum, uint64_t maximum, uint64_t default_value )
{
	libewf_char_t user_input_buffer[ 1024 ];

	libewf_char_t *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	static char *function                = "ewfinput_get_size_variable";
	size_t input_length                  = 0;
	uint64_t size_value                  = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid output stream.\n",
		 function );

		return( 0 );
	}
	if( request_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid request string.\n",
		 function );

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
libewf_char_t *ewfinput_get_fixed_value( FILE *stream, libewf_char_t *request_string, libewf_char_t **values, uint8_t amount, uint8_t default_value )
{
	libewf_char_t user_input_buffer[ 1024 ];

	libewf_char_t *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	libewf_char_t *user_input            = NULL;
	static char *function                = "ewfinput_get_fixed_value";
	size_t input_length                  = 0;
	size_t value_length                  = 0;
	uint8_t iterator                     = 0;
	uint8_t value_match                  = 0;

	if( stream == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid output stream.\n",
		 function );

		return( NULL );
	}
	if( request_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid request string.\n",
		 function );

		return( NULL );
	}
	if( default_value >= amount )
	{
		LIBEWF_WARNING_PRINT( "%s: default value exceeds amount.\n",
		 function );

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
						if( user_input_buffer_ptr[ value_length ] == (libewf_char_t) '\n' )
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

#if defined( MEMWATCH )
			/* One additional character required for end of string
			 */
			user_input = libewf_common_alloc( sizeof( libewf_char_t ) * ( value_length + 1 ) );

			if( user_input == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create string.\n",
				 function );

				return( NULL );
			}
			if( libewf_string_copy( user_input, values[ iterator ], value_length ) == NULL  )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to copy string.\n",
				 function );

				libewf_common_free( user_input );

				return( NULL );
			}
			/* Make sure the string is \0 terminated
			 */
			user_input[ input_length ] = (libewf_char_t) '\0';
#else
			user_input = libewf_string_duplicate( values[ iterator ], value_length );

			if( user_input == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create string.\n",
				 function );

				return( NULL );
			}
#endif
			break;
		}
		else
		{
			fprintf( stream, "Selected option not supported, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( user_input );
}


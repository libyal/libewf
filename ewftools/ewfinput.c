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

#include <common.h>
#include <character_string.h>
#include <memory.h>
#include <notify.h>
#include <system_string.h>

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

#include "ewfbyte_size_string.h"
#include "ewfinput.h"
#include "ewfstring.h"

/* Input selection defintions
 */
character_t *ewfinput_compression_levels[ 3 ] = \
 { _CHARACTER_T_STRING( "none" ),
   _CHARACTER_T_STRING( "fast" ),
   _CHARACTER_T_STRING( "best" ) };

character_t *ewfinput_format_types[ 12 ] = \
 { _CHARACTER_T_STRING( "ewf" ),
   _CHARACTER_T_STRING( "smart" ),
   _CHARACTER_T_STRING( "ftk" ),
   _CHARACTER_T_STRING( "encase1" ),
   _CHARACTER_T_STRING( "encase2" ),
   _CHARACTER_T_STRING( "encase3" ),
   _CHARACTER_T_STRING( "encase4" ),
   _CHARACTER_T_STRING( "encase5" ),
   _CHARACTER_T_STRING( "encase6" ),
   _CHARACTER_T_STRING( "linen5" ),
   _CHARACTER_T_STRING( "linen6" ),
   _CHARACTER_T_STRING( "ewfx" ) };

character_t *ewfinput_media_types[ 2 ] = \
 { _CHARACTER_T_STRING( "fixed" ),
   _CHARACTER_T_STRING( "removable" ) };

character_t *ewfinput_volume_types[ 2 ] = \
 { _CHARACTER_T_STRING( "logical" ),
   _CHARACTER_T_STRING( "physical" ) };

character_t *ewfinput_sector_per_block_sizes[ 10 ] = \
 { _CHARACTER_T_STRING( "64" ),
   _CHARACTER_T_STRING( "128" ),
   _CHARACTER_T_STRING( "256" ),
   _CHARACTER_T_STRING( "512" ),
   _CHARACTER_T_STRING( "1024" ),
   _CHARACTER_T_STRING( "2048" ),
   _CHARACTER_T_STRING( "4096" ),
   _CHARACTER_T_STRING( "8192" ),
   _CHARACTER_T_STRING( "16384" ),
   _CHARACTER_T_STRING( "32768" ) };

character_t *ewfinput_yes_no[ 2 ] = \
 { _CHARACTER_T_STRING( "yes" ),
   _CHARACTER_T_STRING( "no" ) };

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint8_t ewfinput_determine_libewf_format(
         const character_t *argument )
{
	static char *function = "ewfinput_determine_libewf_format";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "smart" ),
	          3 ) == 0 )
	{
		return( LIBEWF_FORMAT_SMART );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "ftk" ),
	          3 ) == 0 )
	{
		return( LIBEWF_FORMAT_FTK );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "encase1" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE1 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "encase2" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE2 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "encase3" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE3 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "encase4" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE4 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "encase5" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE5 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "encase6" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE6 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "linen5" ),
	          6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN5 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "linen6" ),
	          6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN6 );
	}
	/* This check must before the check for "ewf"
	 */
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "ewfx" ),
	          4 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWFX );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "ewf" ),
	          3 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWF );
	}
	return( 0 );
}

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint8_t ewfinput_determine_libewf_format_char_t(
         const system_character_t *argument )
{
	static char *function = "ewfinput_determine_libewf_format_char_t";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "smart" ),
	          3 ) == 0 )
	{
		return( LIBEWF_FORMAT_SMART );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "ftk" ),
	          3 ) == 0 )
	{
		return( LIBEWF_FORMAT_FTK );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "encase1" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE1 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "encase2" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE2 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "encase3" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE3 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "encase4" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE4 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "encase5" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE5 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "encase6" ),
	          7 ) == 0 )
	{
		return( LIBEWF_FORMAT_ENCASE6 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "linen5" ),
	          6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN5 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "linen6" ),
	          6 ) == 0 )
	{
		return( LIBEWF_FORMAT_LINEN6 );
	}
	/* This check must before the check for "ewf"
	 */
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "ewfx" ),
	          4 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWFX );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "ewf" ),
	          3 ) == 0 )
	{
		return( LIBEWF_FORMAT_EWF );
	}
	return( 0 );
}

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint32_t ewfinput_determine_sectors_per_chunk(
          const character_t *argument )
{
	static char *function = "ewfinput_determine_sectors_per_chunk";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "32768" ),
	          5 ) == 0 )
	{
		return( 32768 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "16384" ),
	          5 ) == 0 )
	{
		return( 16384 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "8192" ),
	          4 ) == 0 )
	{
		return( 8192 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "4096" ),
	          4 ) == 0 )
	{
		return( 4096 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "2048" ),
	          4 ) == 0 )
	{
		return( 2048 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "1024" ),
	          4 ) == 0 )
	{
		return( 1024 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "512" ),
	          3 ) == 0 )
	{
		return( 512 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "256" ),
	          3 ) == 0 )
	{
		return( 256 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "128" ),
	          3 ) == 0 )
	{
		return( 128 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "64" ),
	          2 ) == 0 )
	{
		return( 64 );
	}
	return( 0 );
}

/* Determines the sectors per chunk value from an argument string
 * Returns the sectors per chunk value, or 0 on error
 */
uint32_t ewfinput_determine_sectors_per_chunk_char_t(
          const system_character_t *argument )
{
	static char *function = "ewfinput_determine_sectors_per_chunk_char_t";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( 0 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "32768" ),
	          5 ) == 0 )
	{
		return( 32768 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "16384" ),
	          5 ) == 0 )
	{
		return( 16384 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "8192" ),
	          4 ) == 0 )
	{
		return( 8192 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "4096" ),
	          4 ) == 0 )
	{
		return( 4096 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "2048" ),
	          4 ) == 0 )
	{
		return( 2048 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "1024" ),
	          4 ) == 0 )
	{
		return( 1024 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "512" ),
	          3 ) == 0 )
	{
		return( 512 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "256" ),
	          3 ) == 0 )
	{
		return( 256 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "128" ),
	          3 ) == 0 )
	{
		return( 128 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "64" ),
	          2 ) == 0 )
	{
		return( 64 );
	}
	return( 0 );
}

/* Determines the compression level value from an argument string
 * Returns the compression level value, or -1 on error
 */
int8_t ewfinput_determine_compression_level(
        const character_t *argument )
{
	static char *function = "ewfinput_determine_compression_level";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "none" ),
	          4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_NONE );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "fast" ),
	          4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_FAST );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "best" ),
	          4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_BEST );
	}
	return( -1 );
}

/* Determines the compression level value from an argument string
 * Returns the compression level value, or -1 on error
 */
int8_t ewfinput_determine_compression_level_char_t(
        const system_character_t *argument )
{
	static char *function = "ewfinput_determine_compression_level_char_t";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "none" ),
	          4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_NONE );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "fast" ),
	          4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_FAST );
	}
	else if( system_string_compare(
	          argument,
	          _SYSTEM_CHARACTER_T_STRING( "best" ),
	          4 ) == 0 )
	{
		return( LIBEWF_COMPRESSION_BEST );
	}
	return( -1 );
}

/* Determines the media type value from an argument string
 * Returns the media type value, or -1 on error
 */
int8_t ewfinput_determine_media_type(
        const character_t *argument )
{
	static char *function = "ewfinput_determine_media_type";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "fixed" ),
	          5 ) == 0 )
	{
		return( LIBEWF_MEDIA_TYPE_FIXED );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "removable" ),
	          9 ) == 0 )
	{
		return( LIBEWF_MEDIA_TYPE_REMOVABLE );
	}
	return( -1 );
}

/* Determines the volume type value from an argument string
 * Returns the volume type value, or -1 on error
 */
int8_t ewfinput_determine_volume_type(
        const character_t *argument )
{
	static char *function = "ewfinput_determine_volume_type";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "logical" ),
	          7 ) == 0 )
	{
		return( LIBEWF_VOLUME_TYPE_LOGICAL );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "physical" ),
	          8 ) == 0 )
	{
		return( LIBEWF_VOLUME_TYPE_PHYSICAL );
	}
	return( -1 );
}

/* Determines the yes or no value from an argument string
 * Returns 1 if yes, 0 if no, or -1 on error
 */
int8_t ewfinput_determine_yes_no(
        const character_t *argument )
{
	static char *function = "ewfinput_determine_yes_no";

	if( argument == NULL )
	{
		notify_warning_printf( "%s: invalid argument string.\n",
		 function );

		return( -1 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "yes" ),
	          3 ) == 0 )
	{
		return( 1 );
	}
	else if( string_compare(
	          argument,
	          _CHARACTER_T_STRING( "no" ),
	          2 ) == 0 )
	{
		return( 0 );
	}
	return( -1 );
}

/* Get variable input from the user
 * with a maximum of 1023 characters
 */
character_t *ewfinput_get_variable(
              FILE *stream,
              character_t *request_string )
{
	character_t user_input_buffer[ 1024 ];

	character_t *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	character_t *user_input            = NULL;
	character_t *end_of_input          = NULL;
	static char *function              = "ewfinput_get_variable";
	size_t input_length                = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid output stream.\n",
		 function );

		return( NULL );
	}
	if( request_string == NULL )
	{
		notify_warning_printf( "%s: invalid request string.\n",
		 function );

		return( NULL );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs ": ", request_string );

		user_input_buffer_ptr = string_get_from_stream(
		                         user_input_buffer_ptr,
		                         1023,
		                         stdin );

		if( user_input_buffer_ptr != NULL )
		{
			end_of_input = string_search(
			                user_input_buffer_ptr,
			                (character_t) '\n',
			                1023 );

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
			user_input = memory_allocate(
			              sizeof( character_t ) * ( input_length + 1 ) );

			if( user_input == NULL )
			{
				notify_warning_printf( "%s: unable to create string.\n",
				 function );

				return( NULL );
			}
			if( string_copy(
			     user_input,
			     user_input_buffer_ptr,
			     input_length ) == NULL  )
			{
				notify_warning_printf( "%s: unable to copy string.\n",
				 function );

				memory_free(
				 user_input );

				return( NULL );
			}
			/* Make sure the string is \0 terminated
			 */
			user_input[ input_length ] = (character_t) '\0';
#else
			user_input = string_duplicate(
			              user_input_buffer_ptr,
			              input_length );

			if( user_input == NULL )
			{
				notify_warning_printf( "%s: unable to create string.\n",
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
system_character_t *ewfinput_get_variable_char_t(
                     FILE *stream,
                     character_t *request_string )
{
	character_t *user_input               = NULL;
	system_character_t *user_input_char_t = NULL;
	static char *function                 = "ewfinput_get_variable_char_t";
	size_t user_input_length              = 0;

	user_input = ewfinput_get_variable(
	              stream,
	              request_string );

	if( sizeof( system_character_t ) == sizeof( character_t ) )
	{
		return( (system_character_t *) user_input );
	}
	if( sizeof( system_character_t ) < sizeof( character_t ) )
	{
		user_input_length = string_length(
		                     user_input );

		user_input_char_t = memory_allocate(
		                     sizeof( system_character_t ) * ( user_input_length + 1 ) );

		if( user_input_char_t == NULL )
		{
			notify_warning_printf( "%s: unable to create conversion string.\n",
			 function );

			memory_free(
			 user_input );

			return( NULL );
		}
		if( ewfstring_copy_character_string_to_system_string(
		     user_input,
		     user_input_char_t,
		     ( user_input_length + 1 ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to set conversion string.\n",
			 function );

			memory_free(
			 user_input );
			memory_free(
			 user_input_char_t );

			return( NULL );
		}
		memory_free(
		 user_input );

		return( user_input_char_t );
	}
	notify_warning_printf( "%s: character conversion unsupported.\n",
	 function );

	return( NULL );
}

/* Get variable containing a size definition input from the user
 * with a maximum of 1023 characters
 */
uint64_t ewfinput_get_size_variable(
          FILE *stream,
          character_t *request_string,
          uint64_t minimum,
          uint64_t maximum,
          uint64_t default_value )
{
	character_t user_input_buffer[ 1024 ];

	character_t *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	static char *function              = "ewfinput_get_size_variable";
	size_t input_length                = 0;
	uint64_t size_value                = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid output stream.\n",
		 function );

		return( 0 );
	}
	if( request_string == NULL )
	{
		notify_warning_printf( "%s: invalid request string.\n",
		 function );

		return( 0 );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs " (%" PRIu64 " >= value >= %" PRIu64 ") [%" PRIu64 "]: ",
		 request_string, minimum, maximum, default_value );

		user_input_buffer_ptr = string_get_from_stream(
		                         user_input_buffer_ptr,
		                         1023,
		                         stdin );

		if( user_input_buffer_ptr != NULL )
		{
			/* Remove the trailing newline character
			 */
			input_length = string_length(
			                user_input_buffer_ptr ) - 1;

			if( input_length <= 0 )
			{
				return( default_value );
			}
			size_value = string_to_uint64(
			              user_input_buffer_ptr,
			              input_length );

			if( ( size_value >= minimum )
			 && ( size_value <= maximum ) )
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

/* Get variable containing a byte size definition input from the user
 * with a maximum of 1023 characters
 */
uint64_t ewfinput_get_byte_size_variable(
          FILE *stream,
          character_t *request_string,
          uint64_t minimum,
          uint64_t maximum,
          uint64_t default_value )
{
	character_t minimum_size_string[ 16 ];
	character_t maximum_size_string[ 16 ];
	character_t default_size_string[ 16 ];
	character_t user_input_buffer[ 1024 ];

	character_t *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	static char *function              = "ewfinput_get_byte_size_variable";
	size_t input_length                = 0;
	uint64_t size_value                = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid output stream.\n",
		 function );

		return( 0 );
	}
	if( request_string == NULL )
	{
		notify_warning_printf( "%s: invalid request string.\n",
		 function );

		return( 0 );
	}
	if( ewfbyte_size_string_create(
	     minimum_size_string,
	     16,
	     minimum,
	     EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE ) != 1 )
	{
		notify_warning_printf( "%s: unable to create minimum byte size string.\n",
		 function );

		return( 0 );
	}
	if( ewfbyte_size_string_create(
	     default_size_string,
	     16,
	     default_value,
	     EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE ) != 1 )
	{
		notify_warning_printf( "%s: unable to create default byte size string.\n",
		 function );

		return( 0 );
	}
	if( ewfbyte_size_string_create(
	     maximum_size_string,
	     16,
	     maximum,
	     EWFBYTE_SIZE_STRING_UNIT_MEBIBYTE ) != 1 )
	{
		notify_warning_printf( "%s: unable to create maximum byte size string.\n",
		 function );

		return( 0 );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs " (%" PRIs " >= value >= %" PRIs ") [%" PRIs "]: ",
		 request_string, minimum_size_string, maximum_size_string, default_size_string );

		user_input_buffer_ptr = string_get_from_stream(
		                         user_input_buffer_ptr,
		                         1023,
		                         stdin );

		if( user_input_buffer_ptr != NULL )
		{
			/* Remove the trailing newline character
			 */
			input_length = string_length(
			                user_input_buffer_ptr ) - 1;

			if( input_length <= 0 )
			{
				return( default_value );
			}
			if( ewfbyte_size_string_convert(
			     user_input_buffer_ptr,
			     input_length,
			     &size_value ) != 1 )
			{
				fprintf( stream, "Invalid value, please try again or terminate using Ctrl^C.\n" );
			}
			else if( ( size_value >= minimum )
			 && ( size_value <= maximum ) )
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
character_t *ewfinput_get_fixed_value(
              FILE *stream,
              character_t *request_string,
              character_t **values,
              uint8_t amount,
              uint8_t default_value )
{
	character_t user_input_buffer[ 1024 ];

	character_t *user_input_buffer_ptr = &user_input_buffer[ 0 ];
	character_t *user_input            = NULL;
	static char *function              = "ewfinput_get_fixed_value";
	size_t input_length                = 0;
	size_t value_length                = 0;
	uint8_t iterator                   = 0;
	uint8_t value_match                = 0;

	if( stream == NULL )
	{
		notify_warning_printf( "%s: invalid output stream.\n",
		 function );

		return( NULL );
	}
	if( request_string == NULL )
	{
		notify_warning_printf( "%s: invalid request string.\n",
		 function );

		return( NULL );
	}
	if( default_value >= amount )
	{
		notify_warning_printf( "%s: default value exceeds amount.\n",
		 function );

		return( NULL );
	}
	while( 1 )
	{
		fprintf( stream, "%" PRIs " (", request_string );

		for( iterator = 0; iterator < amount; iterator++ )
		{
			if( iterator > 0 )
			{
				fprintf( stream, ", " );
			}
			fprintf( stream, "%" PRIs "", values[ iterator ] );
		}
		fprintf( stream, ") [%" PRIs "]: ", values[ default_value ] );

		user_input_buffer_ptr = string_get_from_stream(
		                         user_input_buffer_ptr,
		                         1023,
		                         stdin );

		if( user_input_buffer_ptr != NULL )
		{
			iterator = 0;

			/* Remove the trailing newline character
			 */
			input_length = string_length(
			                user_input_buffer_ptr ) - 1;

			/* Check if the default value was selected
			 */
			if( input_length == 0 )
			{
				iterator = default_value;

				input_length = string_length(
				                values[ iterator ] );

				value_match  = 1;
			}
			else
			{
				while( iterator < amount )
				{
					value_length = string_length(
					                values[ iterator ] );

					if( string_compare(
					     user_input_buffer_ptr,
					     values[ iterator ],
					     value_length ) == 0 )
					{
						/* Make sure no trailing characters were given
						 */
						if( user_input_buffer_ptr[ value_length ] == (character_t) '\n' )
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
			value_length = string_length(
			                values[ iterator ] );

#if defined( MEMWATCH )
			/* One additional character required for end of string
			 */
			user_input = memory_allocate(
			              sizeof( character_t ) * ( value_length + 1 ) );

			if( user_input == NULL )
			{
				notify_warning_printf( "%s: unable to create string.\n",
				 function );

				return( NULL );
			}
			if( string_copy(
			     user_input,
			     values[ iterator ],
			     value_length ) == NULL  )
			{
				notify_warning_printf( "%s: unable to copy string.\n",
				 function );

				memory_free(
				 user_input );

				return( NULL );
			}
			/* Make sure the string is \0 terminated
			 */
			user_input[ input_length ] = (character_t) '\0';
#else
			user_input = string_duplicate(
			              values[ iterator ],
			              value_length );

			if( user_input == NULL )
			{
				notify_warning_printf( "%s: unable to create string.\n",
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


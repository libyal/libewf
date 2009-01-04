/*
 * ewfstring
 * String functions for the ewf tools
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
#include <notify.h>
#include <system_string.h>

/* Copies the source string (of system_character_t) into the destination string for a certain length
 * Terminates the destination string with \0 at ( length - 1 )
 * Returns 1 if successful, -1 on error
 */
int ewfstring_copy_system_string_to_character_string(
     character_t *destination,
     const system_character_t *source,
     size_t length )
{
	static char *function = "ewfstring_copy_system_string_to_character_string";
	ssize_t conversion    = (ssize_t) ( sizeof( character_t ) - sizeof( system_character_t ) );
	size_t iterator       = 0;

	if( source == NULL )
	{
		notify_warning_printf( "%s: invalid source.\n",
		 function );

		return( -1 );
	}
	if( destination == NULL )
	{
		notify_warning_printf( "%s: invalid destination.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < length; iterator++ )
	{
		if( conversion == 0 )
		{
			destination[ iterator ] = (character_t) source[ iterator ];
		}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
		else if( conversion > 0 )
		{
			destination[ iterator ] = (character_t) btowc(
			                                         (int) source[ iterator ] );
		}
		else if( conversion < 0 )
		{
			destination[ iterator ] = (character_t) wctob(
			                                         (wint_t) source[ iterator ] );

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
			notify_warning_printf( "%s: unsupported conversion.\n",
			 function );

			return( -1 );
		}
	}
	destination[ length - 1 ] = (character_t) '\0';

	return( 1 );
}

/* Copies the source string into the destination string (of system_character_t) for a certain length
 * Terminates the destination string with \0 at ( length - 1 )
 * Returns 1 if successful, -1 on error
 */
int ewfstring_copy_character_string_to_system_string(
     system_character_t *destination,
     const character_t *source,
     size_t length )
{
	static char *function = "ewfstring_copy_character_string_to_system_string";
	ssize_t conversion    = (ssize_t) ( sizeof( character_t ) - sizeof( system_character_t ) );
	size_t iterator       = 0;

	if( source == NULL )
	{
		notify_warning_printf( "%s: invalid source.\n",
		 function );

		return( -1 );
	}
	if( destination == NULL )
	{
		notify_warning_printf( "%s: invalid destination.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < length; iterator++ )
	{
		if( conversion == 0 )
		{
			destination[ iterator ] = (system_character_t) source[ iterator ];
		}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
		else if( conversion > 0 )
		{
			destination[ iterator ] = (system_character_t) wctob(
			                                                (wint_t) source[ iterator ] );

			/* If character is out of the basic ASCII range use '_' as a place holder
			 */
			if( destination[ iterator ] == EOF )
			{
				destination[ iterator ] = '_';
			}
		}
		else if( conversion < 0 )
		{
			destination[ iterator ] = (system_character_t) btowc(
			                                                (int) source[ iterator ] );
		}
#endif
		else
		{
			notify_warning_printf( "%s: unsupported conversion.\n",
			 function );

			return( -1 );
		}
	}
	destination[ length - 1 ] = (system_character_t) '\0';

	return( 1 );
}


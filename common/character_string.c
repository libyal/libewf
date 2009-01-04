/*
 * Character string functions
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

#include "common.h"
#include "character_string.h"
#include "memory.h"
#include "notify.h"
#include "types.h"

/* Duplicates a string
 * Returns the pointer to the duplicate string, or NULL on error
 */
character_t *libewf_string_duplicate(
              character_t *string,
              size_t size )
{
	character_t *duplicate = NULL;
	static char *function  = "libewf_string_duplicate";

	if( string == NULL )
	{
		return( NULL );
	}
	if( size == 0 )
	{
		return( NULL );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( NULL );
	}
	/* Add an additional character for the end of string
	 */
	size += 1;

	duplicate = (character_t *) memory_allocate(
	                             sizeof( character_t ) * size );

	if( duplicate == NULL )
	{
		notify_warning_printf( "%s: unable to create duplicate string.\n",
		 function );

		return( NULL );
	}
	if( string_copy(
	     duplicate,
	     string, size ) == NULL )
	{
		notify_warning_printf( "%s: unable to set duplicate string.\n",
		 function );

		memory_free(
		 duplicate );

		return( NULL );
	}
	duplicate[ size - 1 ] = (character_t) '\0';

	return( duplicate );
}

/* Returns the value represented by a string, returns 0 error
 */
int64_t libewf_string_to_int64(
         const character_t *string,
         size_t size )
{
	character_t *end_of_string = NULL;
	static char *function      = "libewf_string_to_int64";
	int64_t value              = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( 0 );
	}
	if( size == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	end_of_string = (character_t *) &string[ size - 1 ];

	value = string_to_signed_long_long(
	         string,
	         &end_of_string,
	         0 );

	if( value == (int64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

/* Returns the value represented by a string, returns 0 on error
 */
uint64_t libewf_string_to_uint64(
          const character_t *string,
          size_t size )
{
	character_t *end_of_string = NULL;
	static char *function      = "libewf_string_to_uint64";
	uint64_t value             = 0;

	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( 0 );
	}
	if( size == 0 )
	{
		notify_warning_printf( "%s: string is emtpy.\n",
		 function );

		return( 0 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( 0 );
	}
	end_of_string = (character_t *) &string[ size - 1 ];

	value = string_to_unsigned_long_long(
	         string,
	         &end_of_string,
	         0 );

	if( value == (uint64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: unable to convert string.\n",
		 function );

		return( 0 );
	}
	return( value );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && !defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Sets ctime in the string
 * The string must be at least 32 characters of length
 * Returns the pointer to the string if successful or NULL on error
 */
character_t *libewf_string_ctime(
              const time_t *timestamp,
              character_t *string,
              size_t length )
{
	char ctime_string[ 32 ];

	static char *function = "libewf_string_ctime";

	if( timestamp == NULL )
	{
		notify_warning_printf( "%s: invalid timestamp.\n",
		 function );

		return( NULL );
	}
	if( string == NULL )
	{
		notify_warning_printf( "%s: invalid string.\n",
		 function );

		return( NULL );
	}
	if( length > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid length.\n",
		 function );

		return( NULL );
	}
	if( length < 32 )
	{
		notify_warning_printf( "%s: string too small.\n",
		 function );

		return( NULL );
	}
	if( date_time_ctime(
	     timestamp,
	     ctime_string,
	     32 ) == NULL )
	{
		notify_warning_printf( "%s: unable to set ctime string.\n",
		 function );

		return( NULL );
	}
	if( string_copy_char_to_wchar(
	     string,
	     ctime_string,
	     32 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set string.\n",
		 function );

		return( NULL );
	}
	return( string );
}

#endif


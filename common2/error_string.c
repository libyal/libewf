/*
 * Error string functions
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
#include "error_string.h"
#include "memory.h"
#include "notify.h"

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

/* Function to wrap strerror()
 * Returns a new instance to a string containing the error string, NULL on error
 */
char *libewf_error_string_strerror(
       int error_number )
{
	static char *function     = "libewf_error_string_strerror";
#if !defined( error_string_strerror_r ) && defined( HAVE_STRERROR )
	char *static_error_string = NULL;
#endif
#if defined( error_string_strerror_r ) || defined( HAVE_STRERROR )
	char *error_string        = NULL;
	size_t error_string_size  = 256;

	error_string = (char *) memory_allocate(
	                         sizeof( char ) * error_string_size );

	if( error_string == NULL )
	{
		notify_warning_printf( "%s: unable to create error string.\n",
		 function );

		return( NULL );
	}
#endif
#if defined( error_string_strerror_r )
	if( error_string_strerror_r(
	     error_number,
	     error_string,
	     error_string_size ) != ERROR_STRING_STRERROR_R_RETURN_ERROR )
	{
		notify_warning_printf( "%s: unable to set error string.\n",
		 function );

		memory_free(
		 error_string );

		return( NULL );
	}
	return( error_string );

#elif defined( HAVE_STRERROR )
	static_error_string = strerror(
	                       error_number );

	if( static_error_string == NULL )
	{
		notify_warning_printf( "%s: unable to create static error string.\n",
		 function );

		memory_free(
		 error_string );

		return( NULL );
	}
	if( memory_copy(
	     error_string,
	     static_error_string,
	     error_string_size ) == NULL )
	{
		notify_warning_printf( "%s: unable to set error string.\n",
		 function );

		memory_free(
		 error_string );

		return( NULL );
	}
	return( error_string );
#else

	return( NULL );
#endif
}

#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )

/* Function to wrap wide character equivalent of strerror()
 * Returns a new instance to a string containing the error string, NULL on error
 */
wchar_t *libewf_error_string_wcserror(
          int error_number )
{
	static char *function      = "libewf_error_string_wcserror";
#if defined( error_string_wcserror_r )
	wchar_t *error_string      = NULL;
	uint16_t error_string_size = 256;

	error_string = (wchar_t *) memory_allocate(
	                            sizeof( wchar_t ) * error_string_size );

	if( error_string == NULL )
	{
		notify_warning_printf( "%s: unable to create error string.\n",
		 function );

		return( NULL );
	}
	if( error_string_wcserror_r(
	     error_number,
	     error_string,
	     error_string_size ) != ERROR_STRING_STRERROR_R_RETURN_ERROR )
	{
		notify_warning_printf( "%s: unable to set error string.\n",
		 function );

		memory_free(
		 error_string );

		return( NULL );
	}
	return( error_string );
#else

	return( NULL );
#endif
}
#endif


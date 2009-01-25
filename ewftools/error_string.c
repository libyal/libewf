/*
 * Error string functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include "common.h"
#include "memory.h"
#include "notify.h"

#include <errno.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H )
#include <string.h>
#endif

#include "error_string.h"

#if defined( error_string_strerror_r ) || defined( HAVE_STRERROR )

/* Function to wrap strerror()
 * Returns a new instance to a string containing the error string, NULL on error
 */
char *error_string_strerror(
       int error_number )
{
	static char *function     = "error_string_strerror";
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
#endif
}
#endif

#if defined( error_string_wcserror_r )

/* Function to wrap wide character equivalent of strerror()
 * Returns a new instance to a string containing the error string, NULL on error
 */
wchar_t *error_string_wcserror(
          int error_number )
{
	static char *function      = "error_string_wcserror";
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
}
#endif


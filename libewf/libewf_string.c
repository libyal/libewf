/*
 * Character type string functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <types.h>

#include <liberror.h>

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error Missing headers stdarg.h and varargs.h
#endif

#include "libewf_string.h"

/* Copies a string to a 64-bit vlue
 * Return 1 if successful or -1 on error
 */
int libewf_string_copy_to_64bit(
     libewf_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     liberror_error_t **error )
{
	libewf_character_t *end_of_string = NULL;
	static char *function             = "libewf_string_copy_to_64bit";

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	end_of_string = &( string[ string_size - 1 ] );

	errno = 0;

	*value_64bit = libewf_string_to_uint64(
	                string,
	                &end_of_string,
	                0 );

	if( errno != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#if !defined( narrow_string_snprintf )

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#define VARARGS( function, type, argument ) \
	function( libewf_character_t *string, size_t string_size, type argument, ... )
#define VASTART( argument_list, type, name ) \
	va_start( argument_list, name )
#define VAEND( argument_list ) \
	va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, type, argument ) \
	function( libewf_character_t *string, size_t string_size, va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
	{ type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
	va_end( argument_list ); }

#endif

/* Print a formatted string on the string
 * Returns the amount of printed characters if successful or -1 on error
 */
int VARARGS(
     libewf_string_snprintf,
     const libewf_character_t *,
     format )
{
	va_list argument_list;

	int print_count = 0;

	if( string == NULL )
	{
		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		return( -1 );
	}
	VASTART(
	 argument_list,
	 char *,
	 format );

	print_count = narrow_string_sprintf(
		       string,
		       format,
		       argument_list );

	VAEND(
	 argument_list );

	if( print_count <= -1 )
	{
		return( -1 );
	}
	if( (size_t) print_count >= string_size )
	{
		return( -1 );
	}
	return( print_count );
}

#undef VARARGS
#undef VASTART
#undef VAEND

#endif /* !defined( narrow_string_snprintf ) */


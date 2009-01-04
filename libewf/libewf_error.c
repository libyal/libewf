/*
 * Error functions
 *
 * Copyright (c) 2008, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>

#if defined( HAVE_STDARG_H )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error Missing headers stdarg.h and varargs.h
#endif

#include "libewf_definitions.h"
#include "libewf_error.h"

#if defined( HAVE_STDARG_H )
#define VARARGS( function, error, error_domain, error_code, type, argument ) \
        function( error, error_domain, error_code, type argument, ... )
#define VASTART( argument_list, type, name ) \
        va_start( argument_list, name )
#define VAEND( argument_list ) \
        va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, error, error_domain, error_code, type, argument ) \
        function( error, error_domain, error_code, va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
        { type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
        va_end( argument_list ); }

#endif

/* Set an error initializes the error with the error domain and code if necessary
 * Otherwise it will just append the error message for back tracing
 */
void VARARGS(
      libewf_error_set,
      libewf_error_t **error,
      int error_domain,
      int error_code,
      const char *,
      format )
{
	va_list argument_list;

	if( error == NULL )
	{
		return;
	}
	if( *error == NULL )
	{
		*error = (libewf_error_t *) memory_allocate(
		                             sizeof( libewf_internal_error_t ) );

		if( *error == NULL )
		{
			return;
		}
		( (libewf_internal_error_t *) *error )->domain             = error_domain;
		( (libewf_internal_error_t *) *error )->code               = error_code;
		( (libewf_internal_error_t *) *error )->amount_of_messages = 0;
		( (libewf_internal_error_t *) *error )->message            = NULL;

	}
	VASTART(
	 argument_list,
	 const char *,
	 format );

	libewf_error_add_message(
	 *error,
	 format,
	 argument_list );

	VAEND(
	 argument_list );
}

#undef VARARGS
#undef VASTART
#undef VAEND

#if defined( HAVE_STDARG_H )
#define VARARGS( function, error, type, argument ) \
        function( error, type argument, ... )
#define VASTART( argument_list, type, name ) \
        va_start( argument_list, name )
#define VAEND( argument_list ) \
        va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( error, function, error, type, argument ) \
        function( va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
        { type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
        va_end( argument_list ); }

#endif

/* Adds a message to an error
 */
void VARARGS(
      libewf_error_add_message,
      libewf_error_t *error,
      const char *,
      format )
{
	va_list argument_list;

	void *reallocation  = NULL;
	size_t message_size = 64;
	int print_count     = 0;

	if( error == NULL )
	{
		return;
	}
	reallocation = memory_reallocate(
	                ( (libewf_internal_error_t *) error )->message,
	                sizeof( char * ) * ( ( (libewf_internal_error_t *) error )->amount_of_messages + 1 ) );

	if( reallocation == NULL )
	{
		return;
	}
	( (libewf_internal_error_t *) error )->amount_of_messages += 1;
	( (libewf_internal_error_t *) error )->message             = (char **) reallocation;

	VASTART(
	 argument_list,
	 const char *,
	 format );

	do
	{
		reallocation = memory_reallocate(
		                ( (libewf_internal_error_t *) error )->message[ ( (libewf_internal_error_t *) error )->amount_of_messages - 1 ],
		                sizeof( char ) * message_size );

		if( reallocation == NULL )
		{
			memory_free(
			 ( (libewf_internal_error_t *) error )->message[ ( (libewf_internal_error_t *) error )->amount_of_messages - 1 ] );

			( (libewf_internal_error_t *) error )->message[ ( (libewf_internal_error_t *) error )->amount_of_messages - 1 ] = NULL;
		}
		( (libewf_internal_error_t *) error )->message[ ( (libewf_internal_error_t *) error )->amount_of_messages - 1 ] = reallocation;

		print_count = vsnprintf(
		               ( (libewf_internal_error_t *) error )->message[ ( (libewf_internal_error_t *) error )->amount_of_messages - 1 ],
		               message_size,
		               format,
		               argument_list );

		if( ( print_count > -1 )
		 && ( (size_t) print_count > message_size ) )
		{
			message_size = (size_t) ( print_count + 1 );
		}
		else if( print_count <= -1 )
		{
			message_size += 64;
		}
	}
	while( ( print_count <= -1 )
	 || ( (size_t) print_count > message_size ) );

	VAEND(
	 argument_list );
}

#undef VARARGS
#undef VASTART
#undef VAEND

/* Free an error and its elements
 */
void libewf_error_free(
      libewf_error_t **error )
{
	int message_iterator = 0;

	if( error == NULL )
	{
		return;
	}
	if( *error != NULL )
	{
		if( ( (libewf_internal_error_t *) *error )->message != NULL )
		{
			for( message_iterator = 0; message_iterator < ( (libewf_internal_error_t *) error )->amount_of_messages; message_iterator++ )
			{
				if( ( (libewf_internal_error_t *) *error )->message[ message_iterator ] != NULL )
				{
					memory_free(
					 ( (libewf_internal_error_t *) *error )->message[ message_iterator ] );
				}
			}
			memory_free(
			 ( (libewf_internal_error_t *) *error )->message );
		}
		memory_free(
		 *error );

		*error = NULL;
	}
}

/* Prints a descriptive string of the error to the stream
 */
void libewf_error_fprint(
     libewf_error_t *error,
     FILE *stream )
{
	int message_iterator = 0;

	if( stream == NULL )
	{
		return;
	}
	if( error == NULL )
	{
		return;
	}
	if( ( (libewf_internal_error_t *) *error )->message == NULL )
	{
		return;
	}
	message_iterator = ( (libewf_internal_error_t *) error )->amount_of_messages - 1;

	if( ( (libewf_internal_error_t *) *error )->message[ message_iterator ] != NULL )
	{
		fprintf(
		 stream,
		 "%s\n",
		 ( (libewf_internal_error_t *) *error )->message[ message_iterator ] );
	}
	else
	{
		fprintf(
		 stream,
		 "<missing>" );
	}
}

/* Prints a backtrace of the error to the stream
 */
void libewf_error_backtrace_fprint(
     libewf_error_t *error,
     FILE *stream )
{
	int message_iterator = 0;

	if( stream == NULL )
	{
		return;
	}
	if( error == NULL )
	{
		return;
	}
	if( ( (libewf_internal_error_t *) *error )->message == NULL )
	{
		return;
	}
	for( message_iterator = 0; message_iterator < ( (libewf_internal_error_t *) error )->amount_of_messages; message_iterator++ )
	{
		if( ( (libewf_internal_error_t *) *error )->message[ message_iterator ] != NULL )
		{
			fprintf(
			 stream,
			 "%s\n",
			 ( (libewf_internal_error_t *) *error )->message[ message_iterator ] );
		}
		else
		{
			fprintf(
			 stream,
			 "<missing>" );
		}
	}
}


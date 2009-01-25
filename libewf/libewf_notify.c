/*
 * Notification functions
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

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <stdio.h>

#if defined( HAVE_STDARG_H )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error Missing headers stdarg.h and varargs.h
#endif

#include "libewf_notify.h"

FILE *libewf_notify_stream = NULL;
int libewf_notify_verbose  = 0;

/* Set the notify values
 */
void libewf_set_notify_values(
      FILE *stream,
      int verbose )
{
	libewf_notify_stream  = stream;
	libewf_notify_verbose = verbose;
}

#if defined( HAVE_STDARG_H )
#define VARARGS( function, type, argument ) \
	function( type argument, ... )
#define VASTART( argument_list, type, name ) \
	va_start( argument_list, name )
#define VAEND( argument_list ) \
	va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, type, argument ) \
	function( va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
	{ type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
	va_end( argument_list ); }

#endif

/* Print a formatted string on the notify stream
 */
void VARARGS(
      libewf_notify_printf,
      char *,
      format )
{
	va_list argument_list;

	if( libewf_notify_stream != NULL )
	{
		VASTART(
		 argument_list,
		 char *,
		 format );

		vfprintf(
		 libewf_notify_stream,
		 format,
		 argument_list );

		VAEND(
		 argument_list );
	}
}

#undef VARARGS
#undef VASTART
#undef VAEND

/* Prints a backtrace of the error using notify_printf
 */
void libewf_notify_error_backtrace(
      liberror_error_t *error )
{
	if( libewf_notify_stream != NULL )
	{
		liberror_error_backtrace_fprint(
		 error,
		 libewf_notify_stream );
	}
}

/* Prints a dump of data
 */
void libewf_notify_dump_data(
      void *data,
      size_t size )
{
	size_t byte_iterator = 0;
	size_t size_iterator = 0;

	if( libewf_notify_stream == NULL )
	{
		return;
	}
	while( size_iterator < size )
	{
		while( byte_iterator < size )
		{
			if( byte_iterator % 16 == 0 )
			{
				fprintf(
				 libewf_notify_stream,
				 "%.8" PRIzx ": ",
				 byte_iterator );
			}
			fprintf(
			 libewf_notify_stream,
			 "%.2" PRIx8 " ",
			 ( (unsigned char *) data )[ byte_iterator++ ] );

			if( byte_iterator % 16 == 0 )
			{
				break;
			}
			else if( byte_iterator % 8 == 0 )
			{
				fprintf(
				 libewf_notify_stream,
				 " " );
			}
		}
		while( byte_iterator % 16 != 0 )
		{
			byte_iterator++;

			fprintf(
			 libewf_notify_stream,
			 "   " );

			if( ( byte_iterator % 8 == 0 )
			 && ( byte_iterator % 16 != 0 ) )
			{
				fprintf(
				 libewf_notify_stream,
				 " " );
			}
		}
		fprintf(
		 libewf_notify_stream,
		 "  " );

		byte_iterator = size_iterator;

		while( byte_iterator < size )
		{
			if( ( ( (char *) data )[ byte_iterator ] >= 0x20 )
			 && ( ( (char *) data )[ byte_iterator ] <= 0x7e ) )
			{
				fprintf(
				 libewf_notify_stream,
				 "%c",
				 ( (char *) data )[ byte_iterator ] );
			}
			else
			{
				fprintf(
				 libewf_notify_stream,
				 "." );
			}
			byte_iterator++;

			if( byte_iterator % 16 == 0 )
			{
				break;
			}
			else if( byte_iterator % 8 == 0 )
			{
				fprintf(
				 libewf_notify_stream,
				 " " );
			}
		}
		fprintf(
		 libewf_notify_stream,
		 "\n" );

		size_iterator = byte_iterator;
	}
	fprintf(
	 libewf_notify_stream,
	 "\n" );
}


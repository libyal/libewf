/*
 * Output functions
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

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

#if defined( HAVE_STDARG_H )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error No variable argument support available
#endif

#include <libewf/features.h>

#if defined( HAVE_LOCAL_LIBUNA )
#include <libuna_definitions.h>
#elif defined( HAVE_LIBUNA_H )
#include <libuna.h>
#endif

#if defined( HAVE_LOCAL_LIBBFIO )
#include <libbfio_definitions.h>
#elif defined( HAVE_LIBBFIO )
#include <libbfio.h>
#endif

#if defined( HAVE_ZLIB_H )
#include <zlib.h>
#endif

#if defined( HAVE_OPENSSL_OPENSSLV_H )
#include <openssl/opensslv.h>
#endif

#if defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

#include <libewf.h>

#include "error_string.h"
#include "ewfoutput.h"
#include "notify.h"
#include "system_string.h"

/* Print the version information to a stream
 */
void ewfoutput_version_fprint(
      FILE *stream,
      const system_character_t *program )
{
	static char *function = "ewfoutput_version_fprint";

	if( stream == NULL )
	{
		notify_warning_printf(
		 "%s: invalid stream.\n",
		 function );

		return;
	}
	if( program == NULL )
	{
		notify_warning_printf(
		 "%s: invalid program name.\n",
		 function );

		return;
	}
	fprintf(
	 stream,
	 "%" PRIs_SYSTEM " %s (libewf %s",
	 program,
	 LIBEWF_VERSION_STRING,
	 LIBEWF_VERSION_STRING );

	fprintf(
	 stream,
	 ", libuna %s",
	 LIBUNA_VERSION_STRING );

	fprintf(
	 stream,
	 ", libbfio %s",
	 LIBBFIO_VERSION_STRING );

#if defined( HAVE_LIBZ )
	fprintf(
	 stream,
	 ", zlib %s",
	 ZLIB_VERSION );
#endif

#if defined( HAVE_LIBCRYPTO )
	fprintf(
	 stream,
	 ", libcrypto %s",
	 SHLIB_VERSION_NUMBER );
#endif

#if defined( HAVE_LIBUUID )
	fprintf(
	 stream,
	 ", libuuid" );
#endif

	fprintf(
	 stream,
	 ")\n\n" );
}

/* Prints the executable version information
 */
void ewfoutput_copyright_fprint(
      FILE *stream )
{
	static char *function = "ewfoutput_copyright_fprint";

	if( stream == NULL )
	{
		notify_warning_printf(
		 "%s: invalid stream.\n",
		 function );

		return;
	}
	fprintf(
	 stream,
	 "Copyright (c) 2006-2008, Joachim Metz, Hoffmann Investigations <%s> and contributors.\n"
	 "This is free software; see the source for copying conditions. There is NO\n"
	 "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
	 PACKAGE_BUGREPORT );
}

#if defined( HAVE_STDARG_H )
#define VARIABLE_ARGUMENTS_FUNCTION( function, type, argument ) \
        function( FILE *stream, type argument, ... )
#define VARIABLE_ARGUMENTS_START( argument_list, type, name ) \
        va_start( argument_list, name )
#define VARIABLE_ARGUMENTS_END( argument_list ) \
        va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARIABLE_ARGUMENTS_FUNCTION( function, type, argument ) \
        function( FILE *stream, va_alist ) va_dcl
#define VARIABLE_ARGUMENTS_START( argument_list, type, name ) \
        { type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VARIABLE_ARGUMENTS_END( argument_list ) \
        va_end( argument_list ); }

#endif

void VARIABLE_ARGUMENTS_FUNCTION(
      ewfoutput_error_fprint,
      char *,
      format )
{
	va_list argument_list;

#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
        system_character_t *error_string = NULL;
#endif

	if( stream == NULL )
	{
		return;
	}
	VARIABLE_ARGUMENTS_START(
	 argument_list,
	 char *,
       	 format );

	vfprintf(
       	 stream,
	 format,
       	 argument_list );

	VARIABLE_ARGUMENTS_END(
       	 argument_list );

#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
	if( errno != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
		error_string = error_string_wcserror(
		                errno );
#else
		error_string = error_string_strerror(
		                errno );
#endif

		if( error_string != NULL )
		{
			fprintf(
			 stream, " with error: %" PRIs_SYSTEM "",
			 error_string );

			memory_free(
			 error_string );
		}
	}
#endif
	fprintf(
	 stream,
	 ".\n" );
}

#undef VARIABLE_ARGUMENTS_FUNCTION
#undef VARIABLE_ARGUMENTS_START
#undef VARIABLE_ARGUMENTS_END


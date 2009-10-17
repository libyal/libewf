/*
 * Output functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include <errno.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
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

#if defined( HAVE_ZLIB_H ) || defined( ZLIB_DLL )
#include <zlib.h>
#endif

#if defined( HAVE_OPENSSL_OPENSSLV_H )
#include <openssl/opensslv.h>
#endif

#if defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

#include <libewf.h>

#include <libsystem.h>

#include "ewfoutput.h"

/* Print the version information to a stream
 */
void ewfoutput_version_fprint(
      FILE *stream,
      const libsystem_character_t *program )
{
	static char *function = "ewfoutput_version_fprint";

	if( stream == NULL )
	{
		libsystem_notify_printf(
		 "%s: invalid stream.\n",
		 function );

		return;
	}
	if( program == NULL )
	{
		libsystem_notify_printf(
		 "%s: invalid program name.\n",
		 function );

		return;
	}
	fprintf(
	 stream,
	 "%" PRIs_LIBSYSTEM " %s (libewf %s",
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
		libsystem_notify_printf(
		 "%s: invalid stream.\n",
		 function );

		return;
	}
	fprintf(
	 stream,
	 "Copyright (c) 2006-2009, Joachim Metz, Hoffmann Investigations <%s> and contributors.\n"
	 "This is free software; see the source for copying conditions. There is NO\n"
	 "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
	 PACKAGE_BUGREPORT );
}


/*
 * Output functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_ZLIB ) || defined( ZLIB_DLL )
#include <zlib.h>
#endif

#if defined( HAVE_OPENSSL_OPENSSLV_H ) || defined( HAVE_LIBCRYPTO )
#include <openssl/opensslv.h>
#endif

#if defined( HAVE_UUID_UUID_H )
#include <uuid/uuid.h>
#endif

#include "ewftools_i18n.h"
#include "ewftools_libbfio.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libewf.h"
#include "ewftools_libfvalue.h"
#include "ewftools_libhmac.h"
#include "ewftools_libodraw.h"
#include "ewftools_libsmdev.h"
#include "ewftools_libsmraw.h"
#include "ewftools_libuna.h"
#include "ewftools_output.h"

/* Initializes output settings
 * Returns 1 if successful or -1 on error
 */
int ewftools_output_initialize(
     int stdio_mode,
     libcerror_error_t **error )
{
	static char *function = "ewftools_output_initialize";

	if( ( stdio_mode != _IOFBF )
	 && ( stdio_mode != _IOLBF )
	 && ( stdio_mode != _IONBF ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported standard IO mode.",
		 function );

		return( -1 );
	}
#if !defined( __BORLANDC__ )
	if( setvbuf(
	     stdout,
	     NULL,
	     stdio_mode,
	     0 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set IO mode of stdout.",
		 function );

		return( -1 );
	}
	if( setvbuf(
	     stderr,
	     NULL,
	     stdio_mode,
	     0 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set IO mode of stderr.",
		 function );

		return( -1 );
	}
#endif /* !defined( __BORLANDC__ ) */

	return( 1 );
}

/* Prints the executable version information
 */
void ewftools_output_copyright_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	/* TRANSLATORS: This is a proper name.
	 */
	fprintf(
	 stream,
	 _( "Copyright (C) 2006-2020, %s.\n" ),
	 _( "Joachim Metz" ) );

	fprintf(
	 stream,
	 _( "This is free software; see the source for copying conditions. There is NO\n"
	    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" ) );

	/* TRANSLATORS: The placeholder indicates the bug-reporting address
	 * for this package.  Please add _another line_ saying
	 * "Report translation bugs to <...>\n" with the address for translation
	 * bugs (typically your translation team's web or email address).
	 */
	fprintf(
	 stream,
	 _( "Report bugs to <%s>.\n" ),
	 PACKAGE_BUGREPORT );
}

/* Prints the version information to a stream
 */
void ewftools_output_version_fprint(
      FILE *stream,
      const system_character_t *program )
{
	static char *function = "ewftools_output_version_fprint";

	if( stream == NULL )
	{
		libcnotify_printf(
		 "%s: invalid stream.\n",
		 function );

		return;
	}
	if( program == NULL )
	{
		libcnotify_printf(
		 "%s: invalid program name.\n",
		 function );

		return;
	}
	fprintf(
	 stream,
	 "%" PRIs_SYSTEM " %s\n\n",
	 program,
	 LIBEWF_VERSION_STRING );
}

/* Prints the detailed version information to a stream
 */
void ewftools_output_version_detailed_fprint(
      FILE *stream,
      const system_character_t *program )
{
	static char *function = "ewftools_output_version_detailed_fprint";

	if( stream == NULL )
	{
		libcnotify_printf(
		 "%s: invalid stream.\n",
		 function );

		return;
	}
	if( program == NULL )
	{
		libcnotify_printf(
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

#if defined( HAVE_LIBUNA ) || defined( HAVE_LOCAL_LIBUNA )
	fprintf(
	 stream,
	 ", libuna %s",
	 LIBUNA_VERSION_STRING );
#endif

#if defined( HAVE_LIBBFIO ) || defined( HAVE_LOCAL_LIBBFIO )
	fprintf(
	 stream,
	 ", libbfio %s",
	 LIBBFIO_VERSION_STRING );
#endif

#if defined( HAVE_LIBFVALUE ) || defined( HAVE_LOCAL_LIBFVALUE )
	fprintf(
	 stream,
	 ", libfvalue %s",
	 LIBFVALUE_VERSION_STRING );
#endif

#if defined( HAVE_LIBZ )
	fprintf(
	 stream,
	 ", zlib %s",
	 ZLIB_VERSION );
#endif

#if defined( HAVE_LIBHMAC ) || defined( HAVE_LOCAL_LIBHMAC )
	fprintf(
	 stream,
	 ", libhmac %s",
	 LIBHMAC_VERSION_STRING );

#if defined( HAVE_LIBCRYPTO )
	fprintf(
	 stream,
	 " (libcrypto %s)",
	 SHLIB_VERSION_NUMBER );
#endif
#endif /* defined( HAVE_LIBHMAC ) || defined( HAVE_LOCAL_LIBHMAC ) */

#if defined( HAVE_LIBODRAW ) || defined( HAVE_LOCAL_LIBODRAW )
	fprintf(
	 stream,
	 ", libodraw %s",
	 LIBODRAW_VERSION_STRING );
#endif

#if defined( HAVE_LIBSMDEV ) || defined( HAVE_LOCAL_LIBSMDEV )
	fprintf(
	 stream,
	 ", libsmdev %s",
	 LIBSMDEV_VERSION_STRING );
#endif

#if defined( HAVE_LIBSMRAW ) || defined( HAVE_LOCAL_LIBSMRAW )
	fprintf(
	 stream,
	 ", libsmraw %s",
	 LIBSMRAW_VERSION_STRING );
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


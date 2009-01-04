/*
 * libewf notification
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include "libewf_includes.h"

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_notify.h"

FILE *libewf_notify_stream    = NULL;
uint8_t libewf_notify_verbose = 0;

/* Set the notify values
 */
void libewf_notify_set_values( FILE *stream, uint8_t verbose )
{
	libewf_notify_stream  = stream;
	libewf_notify_verbose = verbose;
}

/* Print a remark on libewf notify stream and continue
 */
void VARARGS( libewf_verbose_print, char *, format )
{
#ifdef HAVE_VERBOSE_OUTPUT
	va_list argument_list;

	if( ( libewf_notify_stream != NULL ) && ( libewf_notify_verbose != 0 ) )
	{
		VASTART( argument_list, char *, format );

		vfprintf( libewf_notify_stream, format, argument_list );

		VAEND( argument_list );
	}
#endif
}

/* Print error on libewf notify stream, does not terminate
 */
void VARARGS( libewf_warning_print, char *, format )
{
	va_list argument_list;

	if( ( libewf_notify_stream != NULL ) && ( libewf_notify_verbose != 0 ) )
	{
		VASTART( argument_list, char *, format );

		vfprintf( libewf_notify_stream, format, argument_list );

		VAEND( argument_list );
	}
}

/* Print a fatal error on libewf notify stream, and terminate
 */
void VARARGS( libewf_fatal_print, char *, format )
{
	va_list argument_list;

	if( libewf_notify_stream != NULL )
	{
		VASTART( argument_list, char *, format );

		vfprintf( libewf_notify_stream, format, argument_list );

		VAEND( argument_list );
	}
	exit( EXIT_FAILURE );
}

/* Prints a dump of data
 */
void libewf_dump_data( uint8_t *data, size_t size )
{
	size_t iterator = 0;

	if( libewf_notify_stream == NULL )
	{
		return;
	}
	while( iterator < size )
	{
		if( iterator % 16 == 0 )
		{
			fprintf( libewf_notify_stream, "%.8zx: ", iterator );
		}
		fprintf( libewf_notify_stream, "%.2" PRIx8 "", data[ iterator++ ] );

		if( iterator % 16 == 0 )
		{
			fprintf( libewf_notify_stream, "\n" );
		}
		else if( iterator % 8 == 0 )
		{
			fprintf( libewf_notify_stream, "  " );
		}
	}
	if( iterator % 16 != 0 )
	{
		fprintf( libewf_notify_stream, "\n" );
	}
	fprintf( libewf_notify_stream, "\n" );

	iterator = 0;

	while( iterator < size )
	{
		if( iterator % 32 == 0 )
		{
			fprintf( libewf_notify_stream, "%.8zx: ", iterator );
		}
		fprintf( libewf_notify_stream, "%c ", (char) data[ iterator++ ] );

		if( iterator % 32 == 0 )
		{
			fprintf( libewf_notify_stream, "\n" );
		}
		else if( iterator % 8 == 0 )
		{
			fprintf( libewf_notify_stream, "  " );
		}
	}
	if( iterator % 32 != 0 )
	{
		fprintf( libewf_notify_stream, "\n" );
	}
	fprintf( libewf_notify_stream, "\n" );
}


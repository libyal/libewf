/*
 * ewfalter
 * Alters media data in EWF files
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

#include "../libewf/libewf_includes.h"

#include <stdio.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <libewf.h>

#include "../libewf/libewf_common.h"

#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfglob.h"
#include "ewfsignal.h"

/* Prints the executable usage information
 */
void usage( void )
{
	fprintf( stderr, "Usage: ewfalter [ -hsqvV ] ewf_files\n\n" );

	fprintf( stderr, "\t-h: shows this help\n" );
	fprintf( stderr, "\t-q: quiet shows no status information\n" );
	fprintf( stderr, "\t-s: swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stderr, "\t    (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stderr, "\t-v: verbose output to stderr\n" );
	fprintf( stderr, "\t-V: print version\n" );
}

/* The main program
 */
#ifdef HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#ifndef HAVE_GLOB_H
	EWFGLOB *glob              = NULL;
	int32_t glob_count         = 0;
#endif
	LIBEWF_HANDLE *handle      = NULL;
	uint8_t *buffer            = NULL;
	INT_T option               = 0;
	int64_t count              = 0;
	uint64_t size              = 0;
	uint64_t alter_offset      = 0;
	uint64_t alter_size        = 0;
	uint8_t swap_byte_pairs    = 0;
	uint8_t verbose            = 0;

	ewfsignal_initialize();

	fprintf( stderr, "ewfalter is for expirimental usage only.\n" );

	ewfcommon_version_fprint( stderr, _S_LIBEWF_CHAR( "ewfalter" ) );

	while( ( option = ewfgetopt( argc, argv, _S_CHAR_T( "hsqvV" ) ) ) != (INT_T) -1 )
	{
		switch( option )
		{
			case (INT_T) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs ".\n", argv[ optind ] );

				usage();

				return( EXIT_FAILURE );

			case (INT_T) 'h':
				usage();

				return( EXIT_SUCCESS );

			case (INT_T) 's':
				swap_byte_pairs = 1;

				break;

			case (INT_T) 'q':
				break;

			case (INT_T) 'v':
				verbose = 1;

				break;

			case (INT_T) 'V':
				ewfcommon_copyright_fprint( stderr );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf( stderr, "Missing EWF image file(s).\n" );

		usage();

		return( EXIT_FAILURE );
	}
	libewf_set_notify_values( stderr, verbose );

#ifndef HAVE_GLOB_H
	glob = ewfglob_alloc();

	if( glob == NULL )
	{
		fprintf( stderr, "Unable to create glob.\n" );

		return( EXIT_FAILURE );
	}
	glob_count = ewfglob_resolve( glob, &argv[ optind ], ( argc - optind ) );

	if( glob_count <= 0 )
	{
		fprintf( stderr, "Unable to resolve glob.\n" );

		ewfglob_free( glob );

		return( EXIT_FAILURE );
	}
	handle = libewf_open( glob->results, glob->amount, LIBEWF_OPEN_READ_WRITE );

	ewfglob_free( glob );
#else
	handle = libewf_open( &argv[ optind ], ( argc - optind ), LIBEWF_OPEN_READ_WRITE );
#endif

	if( handle == NULL )
	{
		fprintf( stderr, "Unable to open EWF image file(s).\n" );

		return( EXIT_FAILURE );
	}
	if( libewf_set_swap_byte_pairs( handle, swap_byte_pairs ) != 1 )
	{
		fprintf( stderr, "Unable to set swap byte pairs in handle.\n" );

		return( EXIT_FAILURE );
	}
	size = libewf_get_media_size( handle );

	if( size == 0 )
	{
		fprintf( stderr, "Error altering data from EWF file(s) - media size is 0.\n" );

		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	fprintf( stderr, "Information for alter required, please provide the necessary input\n" );

	alter_offset = ewfcommon_get_user_input_size_variable( stderr, _S_LIBEWF_CHAR( "Start altering at offset" ), 0, size, 0 );
	alter_size   = ewfcommon_get_user_input_size_variable( stderr, _S_LIBEWF_CHAR( "Amount of bytes to alter" ), 0, size, size );

	buffer = libewf_common_alloc( alter_size * sizeof( uint8_t ) );

	if( buffer == NULL )
	{
		fprintf( stderr, "Unable to allocate buffer.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stdout, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_common_memset( buffer, 'X', alter_size ) == NULL )
	{
		fprintf( stderr, "Unable to set buffer.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stdout, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	count = libewf_write_random( handle, buffer, alter_size, alter_offset );

	if( count <= -1 )
	{
		fprintf( stderr, "Alteration failed.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stdout, "Unable to close EWF file handle.\n" );
		}
		return( EXIT_FAILURE );
	}
	fprintf( stderr, "Alteration completed.\n" );

	if( libewf_close( handle ) != 0 )
	{
		fprintf( stdout, "Unable to close EWF file handle.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}


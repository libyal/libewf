/*
 * ewfalter
 * Alters media data in EWF files
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

#include "../libewf/libewf_includes.h"

#include <errno.h>
#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include "../libewf/libewf_common.h"

#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfglob.h"
#include "ewfinput.h"
#include "ewfoutput.h"
#include "ewfsignal.h"
#include "ewfstring.h"

/* Prints the executable usage information
 */
void usage( void )
{
	fprintf( stdout, "Usage: ewfalter [ -t target_file ] [ -hsqvV ] ewf_files\n\n" );

	fprintf( stdout, "\t-h: shows this help\n" );
	fprintf( stdout, "\t-q: quiet shows no status information\n" );
	fprintf( stdout, "\t-s: swap byte pairs of the media data (from AB to BA)\n" );
	fprintf( stdout, "\t    (use this for big to little endian conversion and vice versa)\n" );
	fprintf( stderr, "\t-t: specify the target delta path and base filename (default is the same as the ewf_files)\n" );
	fprintf( stdout, "\t-v: verbose output to stderr\n" );
	fprintf( stdout, "\t-V: print version\n" );
}

/* The main program
 */
#if defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if !defined( HAVE_GLOB_H )
	EWFGLOB *glob              = NULL;
	int32_t glob_count         = 0;
#endif

	libewf_char_t *program     = _S_LIBEWF_CHAR( "ewfalter" );

	CHAR_T *target_filename    = NULL;
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
        CHAR_T *error_string       = NULL;
#endif
	LIBEWF_HANDLE *handle      = NULL;
	uint8_t *buffer            = NULL;
	INT_T option               = 0;
	size64_t media_size        = 0;
	int64_t count              = 0;
	uint64_t alter_offset      = 0;
	uint64_t alter_size        = 0;
	uint8_t swap_byte_pairs    = 0;
	uint8_t verbose            = 0;

	ewfsignal_initialize();

	fprintf( stdout, "ewfalter is for testing purposes only.\n" );

	ewfoutput_version_fprint( stdout, program );

	while( ( option = ewfgetopt( argc, argv, _S_CHAR_T( "hqst:vV" ) ) ) != (INT_T) -1 )
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

			case (INT_T) 'q':
				break;

			case (INT_T) 's':
				swap_byte_pairs = 1;

				break;

			case (INT_T) 't':
				target_filename = optarg;

				break;

			case (INT_T) 'v':
				verbose = 1;

				break;

			case (INT_T) 'V':
				ewfoutput_copyright_fprint( stdout );

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

#if !defined( HAVE_GLOB_H )
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
#if defined( HAVE_STRERROR_R ) || defined( HAVE_STRERROR )
		if( errno != 0 )
		{
			error_string = ewfstring_strerror( errno );
		}
		if( error_string != NULL )
		{
			fprintf( stderr, "Unable to open EWF file(s) with failure: %" PRIs ".\n",
			 error_string );

			libewf_common_free( error_string );
		}
		else
		{
			fprintf( stderr, "Unable to open EWF file(s).\n" );
		}
#else
		fprintf( stderr, "Unable to open EWF file(s).\n" );
#endif

		return( EXIT_FAILURE );
	}
	if( libewf_get_media_size( handle, &media_size ) != 1 )
	{
		fprintf( stderr, "Unable to determine media size.\n" );

		return( EXIT_FAILURE );
	}
	/* Request the necessary case data
	 */
	fprintf( stdout, "Information for alter required, please provide the necessary input\n" );

	alter_offset = ewfinput_get_size_variable(
	                stdout,
	                _S_LIBEWF_CHAR( "Start altering at offset" ),
	                0,
	                media_size,
	                0 );

	alter_size = ewfinput_get_size_variable(
	              stdout,
	              _S_LIBEWF_CHAR( "Amount of bytes to alter" ),
	              0,
	              ( media_size - alter_offset ),
	              ( media_size - alter_offset ) );

	if( alter_size > (size_t) SSIZE_MAX )
	{
		fprintf( stderr, "Invalid amount of bytes to alter value exceeds maximum.\n" );

		return( EXIT_FAILURE );
	}
	if( buffer == NULL )
	{
		fprintf( stderr, "Unable to allocate buffer.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_common_memset( buffer, 'X', (size_t) alter_size ) == NULL )
	{
		fprintf( stderr, "Unable to set buffer.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		libewf_common_free( buffer );

		return( EXIT_FAILURE );
	}
	if( target_filename != NULL )
	{
		if( libewf_set_delta_segment_filename(
		     handle,
		     target_filename,
		     CHAR_T_LENGTH( target_filename ) ) != 1 )
		{
			fprintf( stderr, "Unable to set delta segment filename in handle.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stderr, "Unable to close EWF file(s).\n" );
			}
			libewf_common_free( buffer );

			return( EXIT_FAILURE );
		}
	}
	fprintf( stderr, "\n" );

	/* First alteration run
	 */
	count = libewf_write_random( handle, buffer, (size_t) alter_size, alter_offset );

	if( count <= -1 )
	{
		fprintf( stdout, "Alteration failed.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		libewf_common_free( buffer );

		return( EXIT_FAILURE );
	}
	/* Second alteration run
	 */
	count = libewf_write_random( handle, buffer, (size_t) alter_size, alter_offset );

	libewf_common_free( buffer );

	if( count <= -1 )
	{
		fprintf( stdout, "Alteration failed.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stderr, "Unable to close EWF file(s).\n" );
		}
		return( EXIT_FAILURE );
	}
	if( libewf_close( handle ) != 0 )
	{
		fprintf( stderr, "Unable to close EWF file(s).\n" );

		return( EXIT_FAILURE );
	}
	fprintf( stdout, "Alteration completed.\n" );

	return( EXIT_SUCCESS );
}


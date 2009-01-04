/*
 * ewfverify
 * Verifies the integrity of the media data within the EWF file
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
#include "../libewf/libewf_string.h"

#include "ewfcommon.h"
#include "ewfgetopt.h"
#include "ewfglob.h"
#include "ewfsignal.h"

/* Prints the executable usage information
 */
void usage( void )
{
	fprintf( stderr, "Usage: ewfverify [ -d digest_type ] [ -hqvV ] ewf_files\n\n" );

	fprintf( stderr, "\t-d: calculate additional digest (hash) types besides md5, options: sha1\n" );
	fprintf( stderr, "\t-h: shows this help\n" );
	fprintf( stderr, "\t-q: quiet shows no status information\n" );
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
	EWFGLOB *glob                            = NULL;
	int32_t glob_count                       = 0;
#endif
	LIBEWF_HANDLE *handle                    = NULL;
	LIBEWF_CHAR *stored_md5_hash_string      = NULL;
	LIBEWF_CHAR *calculated_md5_hash_string  = NULL;
	LIBEWF_CHAR *stored_sha1_hash_string     = NULL;
	LIBEWF_CHAR *calculated_sha1_hash_string = NULL;
	CHAR_T *time_string                      = NULL;
	void *callback                           = &ewfcommon_process_status_fprint;
	INT_T option                             = 0;
	time_t timestamp_start                   = 0;
	time_t timestamp_end                     = 0;
	int64_t count                            = 0;
	int8_t stored_md5_hash_result            = 0;
	int8_t calculated_md5_hash_result        = 0;
	int8_t stored_sha1_hash_result           = 0;
	int8_t calculated_sha1_hash_result       = 0;
	uint8_t calculate_sha1                   = 0;
	uint8_t verbose                          = 0;
	int match_md5_hash                       = 0;
	int match_sha1_hash                      = 0;

	ewfsignal_initialize();

	ewfcommon_version_fprint( stderr, _S_LIBEWF_CHAR( "ewfverify" ) );

	while( ( option = ewfgetopt( argc, argv, _S_CHAR_T( "d:hqvV" ) ) ) != (INT_T) -1 )
	{
		switch( option )
		{
			case (INT_T) '?':
			default:
				fprintf( stderr, "Invalid argument: %" PRIs "\n", argv[ optind ] );

				usage();

				return( EXIT_FAILURE );

			case (INT_T) 'd':
				if( CHAR_T_COMPARE( optarg, _S_CHAR_T( "sha1" ), 4 ) == 0 )
				{
					calculate_sha1 = 1;
				}
				else
				{
					fprintf( stderr, "unsuported digest type.\n" );
				}
				break;

			case (INT_T) 'h':
				usage();

				return( EXIT_SUCCESS );

			case (INT_T) 'q':
				callback = NULL;

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
	handle = libewf_open( glob->results, glob->amount, LIBEWF_OPEN_READ );

	ewfglob_free( glob );
#else
	handle = libewf_open( &argv[ optind ], ( argc - optind ), LIBEWF_OPEN_READ );
#endif

	if( handle == NULL )
	{
		fprintf( stderr, "Unable to open EWF image file(s).\n" );

		return( EXIT_FAILURE );
	}
	stored_md5_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

	if( stored_md5_hash_string == NULL )
	{
		fprintf( stderr, "Unable to create stored MD5 hash string.\n" );

		return( EXIT_FAILURE );
	}
	calculated_md5_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

	if( calculated_md5_hash_string == NULL )
	{
		fprintf( stderr, "Unable to create calculated MD5 hash string.\n" );

		libewf_common_free( stored_md5_hash_string );

		return( EXIT_FAILURE );
	}
	if( calculate_sha1 == 1 )
	{
		stored_sha1_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

		if( stored_sha1_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create stored SHA1 hash string.\n" );

			libewf_common_free( stored_md5_hash_string );
			libewf_common_free( calculated_md5_hash_string );

			return( EXIT_FAILURE );
		}
		calculated_sha1_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

		if( calculated_sha1_hash_string == NULL )
		{
			fprintf( stderr, "Unable to create calculated SHA1 hash string.\n" );

			libewf_common_free( stored_md5_hash_string );
			libewf_common_free( calculated_md5_hash_string );
			libewf_common_free( stored_sha1_hash_string );

			return( EXIT_FAILURE );
		}
	}
	/* Start verifying data
	 */
	timestamp_start = time( NULL );
	time_string     = libewf_common_ctime( &timestamp_start );

	if( time_string != NULL )
	{
		fprintf( stderr, "Verify started at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stderr, "Verify started.\n" );
	}
	if( callback != NULL )
	{
		ewfcommon_process_status_initialize( stderr, _S_LIBEWF_CHAR( "verified" ), timestamp_start );
	}
	fprintf( stderr, "This could take a while.\n\n" );

	if( calculate_sha1 == 1 )
	{
		if( libewf_parse_hash_values( handle ) != 1 )
		{
			fprintf( stderr, "Unable to get parse hash values.\n" );
		}
	}
	count = ewfcommon_read( handle, calculate_sha1, callback );

	timestamp_end = time( NULL );
	time_string   = libewf_common_ctime( &timestamp_end );

	if( count <= -1 )
	{
		if( time_string != NULL )
		{
			fprintf( stderr, "Verify failed at: %" PRIs "\n", time_string );

			libewf_common_free( time_string );
		}
		else
		{
			fprintf( stderr, "Verify failed.\n" );
		}
		if( libewf_close( handle ) != 0 )
		{
			fprintf( stdout, "Unable to close EWF file handle.\n" );
		}
		libewf_common_free( stored_md5_hash_string );
		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( stored_sha1_hash_string );
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( time_string != NULL )
	{
		fprintf( stderr, "Verify completed at: %" PRIs "\n", time_string );

		libewf_common_free( time_string );
	}
	else
	{
		fprintf( stderr, "Verify completed.\n" );
	}
	ewfcommon_process_summary_fprint( stderr, _S_LIBEWF_CHAR( "Read" ), count, timestamp_start, timestamp_end );

	fprintf( stderr, "\n" );

	stored_md5_hash_result = libewf_get_stored_md5_hash( handle, stored_md5_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

	if( stored_md5_hash_result == -1 )
	{
		fprintf( stderr, "Unable to get stored MD5 hash.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stdout, "Unable to close EWF file handle.\n" );
		}
		libewf_common_free( stored_md5_hash_string );
		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( stored_sha1_hash_string );
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	calculated_md5_hash_result = libewf_get_calculated_md5_hash( handle, calculated_md5_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 );

	if( calculated_md5_hash_result != 1 )
	{
		fprintf( stderr, "Unable to calculate MD5 hash.\n" );

		if( libewf_close( handle ) != 0 )
		{
			fprintf( stdout, "Unable to close EWF file handle.\n" );
		}
		libewf_common_free( stored_md5_hash_string );
		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( stored_sha1_hash_string );
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( calculate_sha1 == 1 )
	{
		stored_sha1_hash_result = libewf_get_hash_value( handle, _S_LIBEWF_CHAR( "SHA1" ), stored_sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

		if( stored_sha1_hash_result == -1 )
		{
			fprintf( stderr, "Unable to get stored SHA1 hash.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stdout, "Unable to close EWF file handle.\n" );
			}
			libewf_common_free( stored_md5_hash_string );
			libewf_common_free( calculated_md5_hash_string );
			libewf_common_free( stored_sha1_hash_string );
			libewf_common_free( calculated_sha1_hash_string );

			return( EXIT_FAILURE );
		}
		calculated_sha1_hash_result = libewf_get_hash_value( handle, _S_LIBEWF_CHAR( "ewfcommon_calculated_SHA1" ), calculated_sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 );

		if( calculated_sha1_hash_result != 1 )
		{
			fprintf( stderr, "Unable to calculate SHA1 hash.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stdout, "Unable to close EWF file handle.\n" );
			}
			libewf_common_free( stored_md5_hash_string );
			libewf_common_free( calculated_md5_hash_string );
			libewf_common_free( stored_sha1_hash_string );
			libewf_common_free( calculated_sha1_hash_string );

			return( EXIT_FAILURE );
		}
		if( calculated_sha1_hash_result != 1 )
		{
			fprintf( stderr, "Unable to calculate SHA1 hash.\n" );

			if( libewf_close( handle ) != 0 )
			{
				fprintf( stdout, "Unable to close EWF file handle.\n" );
			}
			libewf_common_free( stored_md5_hash_string );
			libewf_common_free( calculated_md5_hash_string );
			libewf_common_free( stored_sha1_hash_string );
			libewf_common_free( calculated_sha1_hash_string );

			return( EXIT_FAILURE );
		}
	}
	ewfcommon_crc_errors_fprint( stderr, handle );

	if( libewf_close( handle ) != 0 )
	{
		fprintf( stdout, "Unable to close EWF file handle.\n" );

		libewf_common_free( stored_md5_hash_string );
		libewf_common_free( calculated_md5_hash_string );

		if( calculate_sha1 == 1 )
		{
			libewf_common_free( stored_sha1_hash_string );
			libewf_common_free( calculated_sha1_hash_string );
		}
		return( EXIT_FAILURE );
	}
	if( stored_md5_hash_result == 0 )
	{
		fprintf( stderr, "MD5 hash stored in file:\tN/A\n" );
	}
	else
	{
		fprintf( stderr, "MD5 hash stored in file:\t%" PRIs_EWF "\n", stored_md5_hash_string );
	}
	fprintf( stderr, "MD5 hash calculated over data:\t%" PRIs_EWF "\n", calculated_md5_hash_string );

	match_md5_hash = ( libewf_string_compare( stored_md5_hash_string, calculated_md5_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_MD5 ) == 0 );

	libewf_common_free( stored_md5_hash_string );
	libewf_common_free( calculated_md5_hash_string );

	if( calculate_sha1 == 1 )
	{
		if( stored_sha1_hash_result == 0 )
		{
			fprintf( stderr, "SHA1 hash stored in file:\tN/A\n" );
		}
		else
		{
			fprintf( stderr, "SHA1 hash stored in file:\t%" PRIs_EWF "\n", stored_sha1_hash_string );
		}
		fprintf( stderr, "SHA1 hash calculated over data:\t%" PRIs_EWF "\n", calculated_sha1_hash_string );

		match_sha1_hash = ( libewf_string_compare( stored_sha1_hash_string, calculated_sha1_hash_string, LIBEWF_STRING_DIGEST_HASH_LENGTH_SHA1 ) == 0 );

		libewf_common_free( stored_sha1_hash_string );
		libewf_common_free( calculated_sha1_hash_string );
	}
	if( match_md5_hash && ( ( calculate_sha1 == 0 ) || ( match_sha1_hash ) ) )
	{
		fprintf( stderr, "\newfverify: SUCCESS\n" );

		return( EXIT_SUCCESS );
	}
	else
	{
		fprintf( stderr, "\newfverify: FAILURE\n" );

		return( EXIT_FAILURE );
	}
}


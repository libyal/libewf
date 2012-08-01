/*
 * Expert Witness Compression Format (EWF) library seek offset testing program
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "ewf_test_libcerror.h"
#include "ewf_test_libcstring.h"
#include "ewf_test_libewf.h"

/* Define to make ewf_test_seek generate verbose output
#define EWF_TEST_SEEK_VERBOSE
 */

/* Tests seeking an offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_seek_offset(
     libewf_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     off64_t expected_offset )
{
	libcerror_error_t *error  = NULL;
	const char *whence_string = NULL;
	static char *function     = "ewf_test_seek_offset";
	off64_t result_offset     = 0;
	int result                = 0;

	if( input_whence == SEEK_CUR )
	{
		whence_string = "SEEK_CUR";
	}
	else if( input_whence == SEEK_END )
	{
		whence_string = "SEEK_END";
	}
	else if( input_whence == SEEK_SET )
	{
		whence_string = "SEEK_SET";
	}
	else
	{
		whence_string = "UNKNOWN";
	}
	fprintf(
	 stdout,
	 "Testing seek of offset: %" PRIi64 " and whence: %s\t",
	 input_offset,
	 whence_string );

	result_offset = libewf_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset != expected_offset )
	{
		if( result_offset == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: %" PRIi64 ".",
			 function,
			 input_offset );

			result = -1;
		}
	}
	else
	{
		if( result_offset == -1 )
		{
			libcerror_error_free(
			 &error );
		}
		result = 1;
	}
	if( result != 0 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	else if( result == 0 )
	{
		fprintf(
		 stdout,
		 "%s: unexpected result offset: %" PRIi64 "\n",
		 function,
		 result_offset );
	}
	return( result );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error = NULL;
	libewf_handle_t *handle  = NULL;
	size64_t media_size      = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename(s).\n" );

		return( EXIT_FAILURE );
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( EWF_TEST_SEEK_VERBOSE )
	libewf_notify_set_verbose(
	 1 );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif
	/* Initialization
	 */
	if( libewf_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     handle,
	     &( argv[ 1 ] ),
	     argc - 1,
	     LIBEWF_OPEN_READ,
	     &error ) != 1 )
#else
	if( libewf_handle_open(
	     handle,
	     &( argv[ 1 ] ),
	     argc - 1,
	     LIBEWF_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		goto on_error;
	}
	if( libewf_handle_get_media_size(
	     handle,
	     &media_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		goto on_error;
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		goto on_error;
	}
	/* Test: SEEK_SET offset: 0
	 * Expected result: 0
	 */
	if( ewf_test_seek_offset(
	     handle,
	     0,
	     SEEK_SET,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_SET offset: <media_size>
	 * Expected result: <media_size>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     (off64_t) media_size,
	     SEEK_SET,
	     (off64_t) media_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_SET offset: <media_size / 5>
	 * Expected result: <media_size / 5>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     (off64_t) ( media_size / 5 ),
	     SEEK_SET,
	     (off64_t) ( media_size / 5 ) ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_SET offset: <media_size + 987>
	 * Expected result: <media_size + 987>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     (off64_t) ( media_size + 987 ),
	     SEEK_SET,
	     (off64_t) ( media_size + 987 ) ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_SET offset: -987
	 * Expected result: -1
	 */
	if( ewf_test_seek_offset(
	     handle,
	     -987,
	     SEEK_SET,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_CUR offset: 0
	 * Expected result: <media_size + 987>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     0,
	     SEEK_CUR,
	     (off64_t) ( media_size + 987 ) ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_CUR offset: <-1 * (media_size + 987)>
	 * Expected result: 0
	 */
	if( ewf_test_seek_offset(
	     handle,
	     -1 * (off64_t) ( media_size + 987 ),
	     SEEK_CUR,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_CUR offset: <media_size / 3>
	 * Expected result: <media_size / 3>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     (off64_t) ( media_size / 3 ),
	     SEEK_CUR,
	     (off64_t) ( media_size / 3 ) ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	if( media_size == 0 )
	{
		/* Test: SEEK_CUR offset: <-2 * (media_size / 3)>
		 * Expected result: 0
		 */
		if( ewf_test_seek_offset(
		     handle,
		     -2 * (off64_t) ( media_size / 3 ),
		     SEEK_CUR,
		     0 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			goto on_error;
		}
	}
	else
	{
		/* Test: SEEK_CUR offset: <-2 * (media_size / 3)>
		 * Expected result: -1
		 */
		if( ewf_test_seek_offset(
		     handle,
		     -2 * (off64_t) ( media_size / 3 ),
		     SEEK_CUR,
		     -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test seek offset.\n" );

			goto on_error;
		}
	}
	/* Test: SEEK_END offset: 0
	 * Expected result: <media_size>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     0,
	     SEEK_END,
	     (off64_t) media_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_END offset: <-1 * media_size>
	 * Expected result: 0
	 */
	if( ewf_test_seek_offset(
	     handle,
	     -1 * (off64_t) media_size,
	     SEEK_END,
	     0 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_END offset: <-1 * (media_size / 4)>
	 * Expected result: <media_size - (media_size / 4)>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     -1 * (off64_t) ( media_size / 4 ),
	     SEEK_END,
	     (off64_t) media_size - (off64_t) ( media_size / 4 ) ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_END offset: 542
	 * Expected result: <media_size + 542>
	 */
	if( ewf_test_seek_offset(
	     handle,
	     542,
	     SEEK_END,
	     (off64_t) ( media_size + 542 ) ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: SEEK_END offset: <-1 * (media_size + 542)>
	 * Expected result: -1
	 */
	if( ewf_test_seek_offset(
	     handle,
	     -1 * (off64_t) ( media_size + 542 ),
	     SEEK_END,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Test: UNKNOWN (88) offset: 0
	 * Expected result: -1
	 */
	if( ewf_test_seek_offset(
	     handle,
	     0,
	     88,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test seek offset.\n" );

		goto on_error;
	}
	/* Clean up
	 */
	if( libewf_handle_close(
	     handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file(s).\n" );

		goto on_error;
	}
	if( libewf_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libewf_error_backtrace_fprint(
		 error,
		 stderr );
		libewf_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}


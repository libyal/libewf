/*
 * Expert Witness Compression Format (EWF) library read testing program
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#define EWF_TEST_READ_BUFFER_SIZE	4096

/* Tests libewf_handle_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_seek_offset(
     libewf_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     off64_t output_offset )
{
	libewf_error_t *error = NULL;
	off64_t result_offset = 0;
	int result            = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	result_offset = libewf_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 &error );

	if( result_offset == -1 )
	{
		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );
	}
	if( result_offset == -1 )
	{
		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );
	}
	if( result_offset != output_offset )
	{
		fprintf(
		 stderr,
		 "Unexpected result offset: %" PRIi64 "\n",
		 result_offset );
	}
	else
	{
		result = 1;
	}
	return( result );
}

/* Tests libewf_handle_read_buffer
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_buffer(
     libewf_handle_t *handle,
     size64_t input_size,
     size64_t output_size )
{
	uint8_t buffer[ EWF_TEST_READ_BUFFER_SIZE ];

	libewf_error_t *error   = NULL;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	remaining_size = input_size;

	while( remaining_size > 0 )
	{
		read_size = EWF_TEST_READ_BUFFER_SIZE;

		if( remaining_size < (size64_t) read_size )
		{
			read_size = (size_t) remaining_size;
		}
		read_count = libewf_handle_read_buffer(
			      handle,
			      buffer,
			      read_size,
			      &error );

		if( read_count < 0 )
		{
			libewf_error_backtrace_fprint(
			 error,
			 stderr );

			libewf_error_free(
			 &error );

			break;
		}
		remaining_size -= (size64_t) read_count;
		result_size    += (size64_t) read_count;

		if( read_count != (ssize_t) read_size )
		{
			break;
		}
	}
	if( output_size == result_size )
	{
		result = 1;
	}
	else
	{
		fprintf(
		 stderr,
		 "Unexpected read count: %" PRIu64 "\n",
		 result_size );
	}
	return( result );
}

/* Tests reading data at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read(
     libewf_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t output_offset,
     size64_t output_size )
{
	const char *whence_string = NULL;
	int result                = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
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
	 "Testing reading range with offset: %" PRIi64 ", whence: %s and size: %" PRIu64 "\t",
	 input_offset,
	 whence_string,
	 input_size );

	result = ewf_test_seek_offset(
	          handle,
	          input_offset,
	          input_whence,
	          output_offset );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = ewf_test_read_buffer(
				  handle,
				  input_size,
				  output_size );
		}
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

	return( result );
}

/* Tests reading data on the segment file size boundary
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_segment_file(
     libewf_handle_t *handle )
{
	libewf_error_t *error    = NULL;
	size64_t segment_file_size = 0;
	int result                 = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( libewf_handle_get_segment_file_size(
	     handle,
	     &segment_file_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve segment file size.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		return( -1 );
	}
	if( segment_file_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Segment file size exceeds maximum.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		return( -1 );
	}
	segment_file_size -= 1;

	fprintf(
	 stdout,
	 "Testing reading range with offset: %" PRIu64 ", whence: SEEK_SET and size: %" PRIu64 "\t",
	 segment_file_size,
	 segment_file_size );

	result = ewf_test_seek_offset(
	          handle,
	          segment_file_size,
	          SEEK_SET,
	          segment_file_size );

	if( result == 1 )
	{
		result = ewf_test_read_buffer(
			  handle,
			  segment_file_size,
			  segment_file_size );
	}
	if( result == 1 )
	{
		result = ewf_test_read_buffer(
			  handle,
			  segment_file_size,
			  segment_file_size );
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
	libewf_error_t *error   = NULL;
	libewf_handle_t *handle = NULL;
	size64_t media_size       = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename(s).\n" );

		return( EXIT_FAILURE );
	}
	/* Initialization
	 */
	if( libewf_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );

		return( EXIT_FAILURE );
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

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libewf_handle_get_media_size(
	     handle,
	     &media_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );
		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );
		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Media size: %" PRIu64 " bytes\n",
	 media_size );

	/* Case 0: test full read
	 */

	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	if( ewf_test_read(
	     handle,
	     0,
	     SEEK_SET,
	     media_size,
	     0,
	     media_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	if( ewf_test_read(
	     handle,
	     0,
	     SEEK_SET,
	     media_size,
	     0,
	     media_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}

	/* Case 1: test random read
	 */

	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: <media_size / 7> size: <media_size / 2>
	 */
	if( ewf_test_read(
	     handle,
	     (off64_t) ( media_size / 7 ),
	     SEEK_SET,
	     media_size / 2,
	     (off64_t) ( media_size / 7 ),
	     media_size / 2 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: <media_size / 7> size: <media_size / 2>
	 */
	if( ewf_test_read(
	     handle,
	     (off64_t) ( media_size / 7 ),
	     SEEK_SET,
	     media_size / 2,
	     (off64_t) ( media_size / 7 ),
	     media_size / 2 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}

	/* Case 3: test read beyond media size
	 */
	if( media_size < 1024 )
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		if( ewf_test_read(
		     handle,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		if( ewf_test_read(
		     handle,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	else
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: <media_size - 1024> size: 1024
		 */
		if( ewf_test_read(
		     handle,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     (off64_t) ( media_size - 1024 ),
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: <media_size - 1024> size: 1024
		 */
		if( ewf_test_read(
		     handle,
		     (off64_t) ( media_size - 1024 ),
		     SEEK_SET,
		     4096,
		     (off64_t) ( media_size - 1024 ),
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	if( argc > 3 )
	{
		/* Case 4: test on segment file size boundary
		 */
		if( ewf_test_read_segment_file(
		     handle ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read segment file.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
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

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libewf_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free handle.\n" );

		libewf_error_backtrace_fprint(
		 error,
		 stderr );

		libewf_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}


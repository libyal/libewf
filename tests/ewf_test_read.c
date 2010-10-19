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
#include <memory.h>

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
     uint8_t *buffer,
     size_t buffer_size,
     size64_t input_size,
     size64_t output_size )
{
	libewf_error_t *error   = NULL;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	remaining_size = input_size;

	while( remaining_size > 0 )
	{
		read_size = buffer_size;

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
	if( output_size != result_size )
	{
		fprintf(
		 stderr,
		 "Unexpected read count: %" PRIu64 "\n",
		 result_size );

		return( 0 );
	}
	return( 1 );
}

/* Tests libewf_handle_read_chunk and libewf_handle_prepare_read_chunk
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_chunk(
     libewf_handle_t *handle,
     uint8_t *chunk_buffer,
     size_t chunk_buffer_size,
     uint8_t *buffer,
     size_t buffer_size,
     size64_t input_size,
     size64_t output_size )
{
	uint8_t checksum_buffer[ 4 ];

	libewf_error_t *error   = NULL;
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t data_size        = 0;
	ssize_t process_count   = 0;
	ssize_t read_count      = 0;
	uint32_t chunk_checksum = 0;
	int8_t is_compressed    = 0;
	int8_t read_checksum    = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	remaining_size = input_size;

	while( remaining_size > 0 )
	{
		read_count = libewf_handle_read_chunk(
			      handle,
			      chunk_buffer,
			      chunk_buffer_size,
			      &is_compressed,
			      (void *) checksum_buffer,
			      &chunk_checksum,
			      &read_checksum,
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
		else if( read_count == 0 )
		{
			break;
		}
		data_size = buffer_size;

		process_count = libewf_handle_prepare_read_chunk(
		                 handle,
		                 chunk_buffer,
		                 (size_t) read_count,
		                 buffer,
		                 &data_size,
		                 is_compressed,
		                 chunk_checksum,
		                 read_checksum,
		                 &error );

		if( process_count < 0 )
		{
			libewf_error_backtrace_fprint(
			 error,
			 stderr );

			libewf_error_free(
			 &error );

			break;
		}
		remaining_size -= (size64_t) process_count;
		result_size    += (size64_t) process_count;
	}
	if( output_size != result_size )
	{
		fprintf(
		 stderr,
		 "Unexpected read count: %" PRIu64 "\n",
		 result_size );

		return( 0 );
	}
	return( 1 );
}

/* Tests libewf_handle_read_buffer at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_buffer_at_offset(
     libewf_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t output_offset,
     size64_t output_size )
{
	uint8_t *buffer           = NULL;
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

	buffer = (uint8_t *) memory_allocate(
	                      EWF_TEST_READ_BUFFER_SIZE );

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
				  buffer,
				  EWF_TEST_READ_BUFFER_SIZE,
				  input_size,
				  output_size );
		}
	}
	memory_free(
	 buffer );

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

/* Tests libewf_handle_read_chunk and libewf_handle_prepare_read_chunk at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_chunk_at_offset(
     libewf_handle_t *handle,
     size32_t chunk_size,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t output_offset,
     size64_t output_size )
{
	uint8_t *buffer           = NULL;
	uint8_t *chunk_buffer     = NULL;
	const char *whence_string = NULL;
	int result                = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( ( chunk_size == 0 )
	 || ( (size_t) chunk_size > (size_t) SSIZE_MAX ) )
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

	buffer = (uint8_t *) memory_allocate(
	                      chunk_size * 2 );

	chunk_buffer = (uint8_t *) memory_allocate(
	                            chunk_size + 16 );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = ewf_test_read_chunk(
				  handle,
				  chunk_buffer,
				  (size_t) chunk_size + 16,
				  buffer,
				  (size_t) chunk_size * 2,
				  input_size,
				  output_size );
		}
	}
	memory_free(
	 chunk_buffer );
	memory_free(
	 buffer );

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
	off64_t read_offset     = 0;
	size64_t media_size     = 0;
	size64_t read_size      = 0;
	size32_t chunk_size     = 0;

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
	if( libewf_handle_get_chunk_size(
	     handle,
	     &chunk_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve chunk size.\n" );

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
	if( chunk_size == 0 )
	{
		fprintf(
		 stderr,
		 "Invalid chunk size.\n" );

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
	read_offset = 0;
	read_size   = media_size;

	if( ewf_test_read_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read buffer.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ewf_test_read_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read buffer.\n" );

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
	read_offset = (off64_t) ( media_size / 7 );
	read_size   = media_size / 2;

	if( ewf_test_read_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read buffer.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ewf_test_read_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read buffer.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}

	/* Case 3: test read buffer beyond media size
	 */
	if( media_size < 1024 )
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = 4096;

		if( ewf_test_read_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read buffer.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		if( ewf_test_read_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read buffer.\n" );

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
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = 4096;

		if( ewf_test_read_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     (off64_t) ( media_size - 1024 ),
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read buffer.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		if( ewf_test_read_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     (off64_t) ( media_size - 1024 ),
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read buffer.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
	}
	fprintf(
	 stdout,
	 "\nChunk size: %" PRIu32 " bytes\n",
	 chunk_size );

	/* Case 0: test full read
	 */

	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	read_offset = 0;
	read_size   = media_size;

	if( ewf_test_read_chunk_at_offset(
	     handle,
	     chunk_size,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read chunk.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ewf_test_read_chunk_at_offset(
	     handle,
	     chunk_size,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read chunk.\n" );

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

	/* Test: offset: <( ( media_size / 7 ) / chunk_size ) * chunk_size> size: <( ( ( media_size / 2 ) / chunk_size ) + 1 ) * chunk_size>
	 * Expected result: offset: <( ( media_size / 7 ) / chunk_size ) * chunk_size> size: <( ( ( media_size / 2 ) / chunk_size ) + 1 ) * chunk_size>
	 */
	read_offset = (off64_t) ( ( media_size / 7 ) / chunk_size ) * chunk_size;
	read_size   = ( ( ( media_size / 2 ) / chunk_size ) + 1 ) * chunk_size;

	if( ewf_test_read_chunk_at_offset(
	     handle,
	     chunk_size,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read chunk.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( ewf_test_read_chunk_at_offset(
	     handle,
	     chunk_size,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read chunk.\n" );

		libewf_handle_close(
		 handle,
		 NULL );
		libewf_handle_free(
		 &handle,
		 NULL );

		return( EXIT_FAILURE );
	}

	/* Case 3: test read chunk beyond media size
	 */
	if( media_size < 1024 )
	{
		/* Test: offset: <media_size - 1024> size: chunk_size
		 * Expected result: offset: -1 size: <undetermined>
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = chunk_size;

		if( ewf_test_read_chunk_at_offset(
		     handle,
		     chunk_size,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		if( ewf_test_read_chunk_at_offset(
		     handle,
		     chunk_size,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

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
		/* Test: offset: <media_size - 1024> size: chunk_size
		 * Expected result: offset: <media_size - 1024> size: chunk size or media_size % chunk_size
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = chunk_size;

		if( ewf_test_read_chunk_at_offset(
		     handle,
		     chunk_size,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     (off64_t) ( media_size - 1024 ),
		     ( ( media_size % chunk_size ) == 0 ) ? chunk_size : media_size % chunk_size ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			libewf_handle_close(
			 handle,
			 NULL );
			libewf_handle_free(
			 &handle,
			 NULL );

			return( EXIT_FAILURE );
		}
		if( ewf_test_read_chunk_at_offset(
		     handle,
		     chunk_size,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     (off64_t) ( media_size - 1024 ),
		     ( ( media_size % chunk_size ) == 0 ) ? chunk_size : media_size % chunk_size ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

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


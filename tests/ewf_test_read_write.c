/*
 * Expert Witness Compression Format (EWF) library read/write testing program
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
#include <memory.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "ewf_test_definitions.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libcstring.h"
#include "ewf_test_libcsystem.h"
#include "ewf_test_libewf.h"

/* Define to make ewf_test_read_write generate verbose output
#define EWF_TEST_READ_WRITE_VERBOSE
 */

/* Tests libewf_handle_seek_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_seek_offset(
     libewf_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     off64_t expected_offset,
     libcerror_error_t **error )
{
	static char *function = "ewf_test_seek_offset";
	off64_t result_offset = 0;

	result_offset = libewf_handle_seek_offset(
	                 handle,
	                 input_offset,
	                 input_whence,
	                 error );

	if( result_offset != expected_offset )
	{
		if( result_offset == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: %" PRIi64 ".",
			 function,
			 input_offset );

			return( -1 );
		}
		fprintf(
		 stderr,
		 "%s: unexpected result offset: %" PRIi64 "\n",
		 function,
		 result_offset );

		return( 0 );
	}
	if( result_offset == -1 )
	{
		libcerror_error_free(
		 error );
	}
	return( 1 );
}

/* Tests libewf_handle_read_buffer and libewf_handle_write_buffer
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_write_buffer(
     libewf_handle_t *handle,
     uint8_t *buffer,
     size_t buffer_size,
     size64_t input_size,
     size64_t expected_size,
     libcerror_error_t **error )
{
	static char *function   = "ewf_test_read_write_buffer";
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t read_size        = 0;
	ssize_t read_count      = 0;
	ssize_t write_count     = 0;

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
			      error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer of size: %" PRIzd ".",
			 function,
			 read_size );

			return( -1 );
		}
		else if( read_count == 0 )
		{
			break;
		}
		if( libewf_handle_seek_offset(
		     handle,
	             -1 * (off64_t) read_count,
	             SEEK_CUR,
	             error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to seek previous offset.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     buffer,
		     (int) 'B',
		     (size_t) read_count ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			return( -1 );
		}
		write_count = libewf_handle_write_buffer(
			       handle,
			       buffer,
			       (size_t) read_count,
			       error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable write buffer of size: %" PRIzd ".",
			 function,
			 read_count );

			return( -1 );
		}
		if( write_count != read_count )
		{
			break;
		}
		remaining_size -= (size64_t) write_count;
		result_size    += (size64_t) write_count;
	}
	if( expected_size != result_size )
	{
		fprintf(
		 stderr,
		 "%s: unexpected read/write count: %" PRIu64 "\n",
		 function,
		 result_size );

		return( 0 );
	}
	return( 1 );
}

/* Tests libewf_handle_read_chunk, libewf_handle_write_chunk, libewf_handle_prepare_read_chunk and libewf_handle_prepare_write_chunk
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_write_chunk(
     libewf_handle_t *handle,
     uint8_t *data_buffer,
     size_t data_buffer_size,
     uint8_t *chunk_buffer,
     size_t chunk_buffer_size,
     size64_t input_size,
     size64_t expected_size,
     libcerror_error_t **error )
{
	uint8_t checksum_buffer[ 4 ];

	static char *function   = "ewf_test_read_write_chunk";
	size64_t remaining_size = 0;
	size64_t result_size    = 0;
	size_t chunk_data_size  = 0;
	size_t data_size        = 0;
	size_t write_size       = 0;
	ssize_t process_count   = 0;
	ssize_t read_count      = 0;
	ssize_t write_count     = 0;
	uint32_t chunk_checksum = 0;
	int8_t is_compressed    = 0;
	int8_t process_checksum = 0;

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
			      &process_checksum,
			      error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read chunk of size: %" PRIzd ".",
			 function,
			 chunk_buffer_size );

			return( -1 );
		}
		else if( read_count == 0 )
		{
			break;
		}
		data_size = data_buffer_size;

		process_count = libewf_handle_prepare_read_chunk(
		                 handle,
		                 chunk_buffer,
		                 (size_t) read_count,
		                 data_buffer,
		                 &data_size,
		                 is_compressed,
		                 chunk_checksum,
		                 process_checksum,
		                 error );

		if( process_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to prepare chunk of size: %" PRIzd " after read.",
			 function,
			 read_count );

			return( -1 );
		}
		if( libewf_handle_seek_offset(
		     handle,
	             -1 * (off64_t) process_count,
	             SEEK_CUR,
	             error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to seek previous offset.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     data_buffer,
		     (int) 'C',
		     data_buffer_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			return( -1 );
		}
		write_size      = process_count;
		chunk_data_size = chunk_buffer_size;

		process_count = libewf_handle_prepare_write_chunk(
				 handle,
				 data_buffer,
				 write_size,
				 chunk_buffer,
				 &chunk_data_size,
				 &is_compressed,
				 &chunk_checksum,
				 &process_checksum,
				 error );

		if( process_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to prepare chunk buffer before writing.",
			 function );

			return( -1 );
		}
		if( is_compressed == 0 )
		{
			write_count = libewf_handle_write_chunk(
				       handle,
				       data_buffer,
				       process_count,
				       write_size,
				       is_compressed,
				       checksum_buffer,
				       chunk_checksum,
				       process_checksum,
				       error );
		}
		else
		{
			write_count = libewf_handle_write_chunk(
				       handle,
				       chunk_buffer,
				       chunk_data_size,
				       write_size,
				       is_compressed,
				       checksum_buffer,
				       chunk_checksum,
				       process_checksum,
				       error );
		}
		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable write chunk of size: %" PRIzd ".",
			 function,
			 chunk_data_size );

			return( -1 );
		}
		remaining_size -= (size64_t) write_size;
		result_size    += (size64_t) write_size;
	}
	if( expected_size != result_size )
	{
		fprintf(
		 stderr,
		 "%s: unexpected read/write count: %" PRIu64 "\n",
		 function,
		 result_size );

		return( 0 );
	}
	return( 1 );
}

/* Tests reading and writing a buffers at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_write_buffer_at_offset(
     libewf_handle_t *handle,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t expected_offset,
     size64_t expected_size )
{
	libcerror_error_t *error   = NULL;
	uint8_t *buffer           = NULL;
	const char *whence_string = NULL;
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
	 "Testing reading and writing range with offset: %" PRIi64 ", whence: %s and size: %" PRIu64 "\t",
	 input_offset,
	 whence_string,
	 input_size );

	buffer = (uint8_t *) memory_allocate(
	                      EWF_TEST_BUFFER_SIZE );

	result = ewf_test_seek_offset(
	          handle,
	          input_offset,
	          input_whence,
	          expected_offset,
	          &error );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = ewf_test_read_write_buffer(
				  handle,
				  buffer,
				  EWF_TEST_BUFFER_SIZE,
				  input_size,
				  expected_size,
			          &error );
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

	if( result == -1 )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );

		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Tests reading and writing chunks at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_write_chunk_at_offset(
     libewf_handle_t *handle,
     size32_t chunk_size,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t expected_offset,
     size64_t expected_size )
{
	libcerror_error_t *error   = NULL;
	uint8_t *chunk_buffer     = NULL;
	uint8_t *data_buffer      = NULL;
	const char *whence_string = NULL;
	size_t chunk_buffer_size  = 0;
	size_t data_buffer_size   = 0;
	int result                = 0;

	if( chunk_size == 0 )
	{
		return( -1 );
	}
#if SIZEOF_SIZE_T < 8
	if( (size_t) chunk_size > (size_t) SSIZE_MAX )
	{
		return( -1 );
	}
#endif
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
	 "Testing reading range and writing with offset: %" PRIi64 ", whence: %s and size: %" PRIu64 "\t",
	 input_offset,
	 whence_string,
	 input_size );

	result = ewf_test_seek_offset(
	          handle,
	          input_offset,
	          input_whence,
	          expected_offset,
	          &error );

	data_buffer_size = chunk_size;

	data_buffer = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * data_buffer_size );

	/* The compressed data can become larger than the uncompressed data
	 */
	chunk_buffer_size = chunk_size * 2;

	chunk_buffer = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * chunk_buffer_size );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = ewf_test_read_write_chunk(
				  handle,
				  data_buffer,
				  data_buffer_size,
				  chunk_buffer,
				  chunk_buffer_size,
				  input_size,
				  expected_size,
			          &error );
		}
	}
	memory_free(
	 chunk_buffer );
	memory_free(
	 data_buffer );

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
		 stderr );

		libcerror_error_free(
		 &error );
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
	libcstring_system_character_t *target_filename = NULL;
	libcerror_error_t *error                        = NULL;
	libewf_handle_t *handle                        = NULL;
	libcstring_system_integer_t option             = 0;
	off64_t read_offset                            = 0;
	size64_t media_size                            = 0;
	size64_t read_size                             = 0;
	size32_t chunk_size                            = 0;
	size_t delta_segment_filename_length           = 0;

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "t:" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 't':
				target_filename = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing EWF image filename(s).\n" );

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

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( EWF_TEST_READ_WRITE_VERBOSE )
	libewf_notify_set_verbose(
	 1 );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     handle,
	     &( argv[ optind ] ),
	     argc - optind,
	     LIBEWF_OPEN_READ_WRITE,
	     &error ) != 1 )
#else
	if( libewf_handle_open(
	     handle,
	     &( argv[ optind ] ),
	     argc - optind,
	     LIBEWF_OPEN_READ_WRITE,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file(s).\n" );

		goto on_error;
	}
	if( target_filename != NULL )
	{
		delta_segment_filename_length = libcstring_system_string_length(
		                                 target_filename );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_handle_set_delta_segment_filename_wide(
		     handle,
		     target_filename,
		     delta_segment_filename_length,
		     &error ) != 1 )
#else
		if( libewf_handle_set_delta_segment_filename(
		     handle,
		     target_filename,
		     delta_segment_filename_length,
		     &error ) != 1 )
#endif
		{
			fprintf(
			 stderr,
			 "Unable to set delta segment filename.\n" );

			goto on_error;
		}
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
	if( libewf_handle_get_chunk_size(
	     handle,
	     &chunk_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve chunk size.\n" );

		goto on_error;
	}
	if( chunk_size == 0 )
	{
		fprintf(
		 stderr,
		 "Invalid chunk size.\n" );

		goto on_error;
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

	if( ewf_test_read_write_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read/write buffer.\n" );

		goto on_error;
	}
	if( ewf_test_read_write_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read/write buffer.\n" );

		goto on_error;
	}

	/* Case 1: test random read
	 */

	/* Test: offset: <media_size / 7> size: <media_size / 2>
	 * Expected result: offset: <media_size / 7> size: <media_size / 2>
	 */
	read_offset = (off64_t) ( media_size / 7 );
	read_size   = media_size / 2;

	if( ewf_test_read_write_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read/write buffer.\n" );

		goto on_error;
	}
	if( ewf_test_read_write_buffer_at_offset(
	     handle,
	     read_offset,
	     SEEK_SET,
	     read_size,
	     read_offset,
	     read_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read/write buffer.\n" );

		goto on_error;
	}

	/* Case 2: test read buffer beyond media size
	 */
	if( media_size < 1024 )
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: -1 size: <undetermined>
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = 4096;

		if( ewf_test_read_write_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read/write buffer.\n" );

			goto on_error;
		}
		if( ewf_test_read_write_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     -1,
		     (size64_t) -1 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read/write buffer.\n" );

			goto on_error;
		}
	}
	else
	{
		/* Test: offset: <media_size - 1024> size: 4096
		 * Expected result: offset: <media_size - 1024> size: 1024
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = 4096;

		if( ewf_test_read_write_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     read_offset,
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read/write buffer.\n" );

			goto on_error;
		}
		if( ewf_test_read_write_buffer_at_offset(
		     handle,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     read_offset,
		     1024 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read/write buffer.\n" );

			goto on_error;
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

	if( ewf_test_read_write_chunk_at_offset(
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
		 "Unable to test read/write chunk.\n" );

		goto on_error;
	}
	if( ewf_test_read_write_chunk_at_offset(
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
		 "Unable to test read/write chunk.\n" );

		goto on_error;
	}

	/* Case 1: test random read
	 */

	/* Test: offset: <( ( media_size / 7 ) / chunk_size ) * chunk_size> size: <( ( ( media_size / 2 ) / chunk_size ) + 1 ) * chunk_size>
	 * Expected result: offset: <( ( media_size / 7 ) / chunk_size ) * chunk_size> size: <( ( ( media_size / 2 ) / chunk_size ) + 1 ) * chunk_size>
	 */
	read_offset = (off64_t) ( ( media_size / 7 ) / chunk_size ) * chunk_size;
	read_size   = ( ( ( media_size / 2 ) / chunk_size ) + 1 ) * chunk_size;

	if( media_size == 0 )
	{
		if( ewf_test_read_write_chunk_at_offset(
		     handle,
		     chunk_size,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     read_offset,
		     0 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read/write chunk.\n" );

			goto on_error;
		}
		if( ewf_test_read_write_chunk_at_offset(
		     handle,
		     chunk_size,
		     read_offset,
		     SEEK_SET,
		     read_size,
		     read_offset,
		     0 ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read/write chunk.\n" );

			goto on_error;
		}
	}
	else
	{
		if( ewf_test_read_write_chunk_at_offset(
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
			 "Unable to test read/write chunk.\n" );

			goto on_error;
		}
		if( ewf_test_read_write_chunk_at_offset(
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
			 "Unable to test read/write chunk.\n" );

			goto on_error;
		}
	}
	/* Case 2: test read chunk beyond media size
	 */
	if( media_size < 1024 )
	{
		/* Test: offset: <media_size - 1024> size: chunk_size
		 * Expected result: offset: -1 size: <undetermined>
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = chunk_size;

		if( ewf_test_read_write_chunk_at_offset(
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
			 "Unable to test read/write chunk.\n" );

			goto on_error;
		}
		if( ewf_test_read_write_chunk_at_offset(
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
			 "Unable to test read/write chunk.\n" );

			goto on_error;
		}
	}
	else
	{
		/* Test: offset: <media_size - 1024> size: chunk_size
		 * Expected result: offset: <media_size - 1024> size: chunk size or media_size % chunk_size
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = chunk_size;

		if( ewf_test_read_write_chunk_at_offset(
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
			 "Unable to test read/write chunk.\n" );

			goto on_error;
		}
		if( ewf_test_read_write_chunk_at_offset(
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
			 "Unable to test read/write chunk.\n" );

			goto on_error;
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


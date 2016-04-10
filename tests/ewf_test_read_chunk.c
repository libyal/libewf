/*
 * Expert Witness Compression Format (EWF) library read testing program
 *
 * Copyright (C) 2006-2016, Joachim Metz <joachim.metz@gmail.com>
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

#include "ewf_test_libcerror.h"
#include "ewf_test_libcstring.h"
#include "ewf_test_libcsystem.h"
#include "ewf_test_libewf.h"

/* Define to make ewf_test_read generate verbose output
#define EWF_TEST_READ_VERBOSE
 */

#define EWF_TEST_READ_CHUNK_NUMBER_OF_THREADS	4

/* Tests libewf_handle_get_offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_get_offset(
     libewf_handle_t *handle,
     off64_t expected_offset,
     libcerror_error_t **error )
{
	static char *function = "ewf_test_get_offset";
	off64_t result_offset = 0;

	if( expected_offset != -1 )
	{
		if( libewf_handle_get_offset(
		     handle,
		     &result_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset.",
			 function );

			return( -1 );
		}
		if( result_offset != expected_offset )
		{
			fprintf(
			 stderr,
			 "%s: unexpected result offset: %" PRIi64 "\n",
			 function,
			 result_offset );

			return( 0 );
		}
	}
	return( 1 );
}

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

/* Tests libewf_handle_read_data_chunk and libewf_data_chunk_read_buffer
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_chunk(
     libewf_handle_t *handle,
     uint8_t *data_buffer,
     size_t data_buffer_size,
     size64_t input_size,
     size64_t expected_size,
     libcerror_error_t **error )
{
	libewf_data_chunk_t *data_chunk = NULL;
	static char *function           = "ewf_test_read_chunk";
	size64_t remaining_size         = 0;
	size64_t result_size            = 0;
	ssize_t process_count           = 0;
	ssize_t read_count              = 0;

	remaining_size = input_size;

	if( libewf_handle_get_data_chunk(
	     handle,
	     &data_chunk,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to retrieve data chunk.",
		 function );

		goto on_error;
	}
	while( remaining_size > 0 )
	{
		read_count = libewf_handle_read_data_chunk(
			      handle,
			      data_chunk,
			      error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read chunk of size: %" PRIzd ".",
			 function,
			 data_buffer_size );

			goto on_error;
		}
		else if( read_count == 0 )
		{
			break;
		}
		process_count = libewf_data_chunk_read_buffer(
		                 data_chunk,
		                 data_buffer,
		                 data_buffer_size,
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

			goto on_error;
		}
		remaining_size -= (size64_t) process_count;
		result_size    += (size64_t) process_count;
	}
	if( libewf_data_chunk_free(
	     &data_chunk,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free data chunk.",
		 function );

		goto on_error;
	}
	if( expected_size != result_size )
	{
		fprintf(
		 stderr,
		 "%s: unexpected read count: %" PRIu64 "\n",
		 function,
		 result_size );

		return( 0 );
	}
	return( 1 );

on_error:
	if( data_chunk != NULL )
	{
		libewf_data_chunk_free(
		 &data_chunk,
		 NULL );
	}
	return( -1 );
}

/* Tests reading chunks at a specific offset
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_chunk_at_offset(
     libewf_handle_t *handle,
     size32_t chunk_size,
     off64_t input_offset,
     int input_whence,
     size64_t input_size,
     off64_t expected_offset,
     size64_t expected_size )
{
	libcerror_error_t *error  = NULL;
	uint8_t *data_buffer      = NULL;
	const char *whence_string = NULL;
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
	 "Testing reading range with offset: %" PRIi64 ", whence: %s and size: %" PRIu64 "\t",
	 input_offset,
	 whence_string,
	 input_size );

	result = ewf_test_seek_offset(
	          handle,
	          input_offset,
	          input_whence,
	          expected_offset,
	          &error );

	data_buffer = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * chunk_size );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = ewf_test_read_chunk(
				  handle,
				  data_buffer,
				  chunk_size,
				  input_size,
				  expected_size,
			          &error );
		}
	}
	memory_free(
	 data_buffer );

	if( result == 1 )
	{
		if( input_offset >= 0 )
		{
			result = ewf_test_get_offset(
			          handle,
			          input_offset - ( input_offset % chunk_size ) + expected_size,
			          &error );
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

/* Tests reading chunks from a handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_chunk_from_handle(
     libewf_handle_t *handle,
     size64_t media_size,
     size32_t chunk_size )
{
	off64_t read_offset = 0;
	size64_t read_size  = 0;
	int result          = 0;

	if( handle == NULL )
	{
		return( -1 );
	}
	if( media_size > (size64_t) INT64_MAX )
	{
		fprintf(
		 stderr,
		 "Media size exceeds maximum.\n" );

		return( -1 );
	}
	/* Case 0: test full read
	 */

	/* Test: offset: 0 size: <media_size>
	 * Expected result: offset: 0 size: <media_size>
	 */
	read_offset = 0;
	read_size   = media_size;

	result = ewf_test_read_chunk_at_offset(
	          handle,
	          chunk_size,
	          read_offset,
	          SEEK_SET,
	          read_size,
	          read_offset,
	          read_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read chunk.\n" );

		return( -1 );
	}
	result = ewf_test_read_chunk_at_offset(
	          handle,
	          chunk_size,
	          read_offset,
	          SEEK_SET,
	          read_size,
	          read_offset,
	          read_size );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read chunk.\n" );

		return( -1 );
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
		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          read_offset,
		          0 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
		}
		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          read_offset,
		          0 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
		}
	}
	else
	{
		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          read_offset,
		          read_size );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
		}
		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          read_offset,
		          read_size );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
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

		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          -1,
		          (size64_t) -1 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
		}
		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          -1,
		          (size64_t) -1 );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
		}
	}
	else
	{
		/* Test: offset: <media_size - 1024> size: chunk_size
		 * Expected result: offset: <media_size - 1024> size: chunk size or media_size % chunk_size
		 */
		read_offset = (off64_t) ( media_size - 1024 );
		read_size   = chunk_size;

		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          (off64_t) ( media_size - 1024 ),
		          ( ( media_size % chunk_size ) == 0 ) ? chunk_size : media_size % chunk_size );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
		}
		result = ewf_test_read_chunk_at_offset(
		          handle,
		          chunk_size,
		          read_offset,
		          SEEK_SET,
		          read_size,
		          (off64_t) ( media_size - 1024 ),
		          ( ( media_size % chunk_size ) == 0 ) ? chunk_size : media_size % chunk_size );

		if( result != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to test read chunk.\n" );

			return( -1 );
		}
	}
	return( 1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcstring_system_character_t **filenames = NULL;
	libcstring_system_character_t *source     = NULL;
	libcerror_error_t *error                  = NULL;
	libewf_handle_t *handle                   = NULL;
	libcstring_system_integer_t option        = 0;
	size64_t media_size                       = 0;
	size32_t chunk_size                       = 0;
	size_t string_length                      = 0;
	int number_of_filenames                   = 0;

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "" ) ) ) != (libcstring_system_integer_t) -1 )
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
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

#if defined( HAVE_DEBUG_OUTPUT ) && defined( EWF_TEST_READ_VERBOSE )
	libewf_notify_set_verbose(
	 1 );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	string_length = libcstring_wide_string_length(
	                 source );

	if( libewf_glob_wide(
	     source,
	     string_length,
	     LIBEWF_FORMAT_UNKNOWN,
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
#else
	string_length = libcstring_narrow_string_length(
	                 source );

	if( libewf_glob(
	     source,
	     string_length,
	     LIBEWF_FORMAT_UNKNOWN,
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to glob filenames.\n" );

		goto on_error;
	}
	if( number_of_filenames < 0 )
	{
		fprintf(
		 stderr,
		 "Invalid number of filenames.\n" );

		goto on_error;
	}
	else if( number_of_filenames == 0 )
	{
		fprintf(
		 stderr,
		 "Missing filenames.\n" );

		goto on_error;
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     &error ) != 1 )
#else
	if( libewf_handle_open(
	     handle,
	     filenames,
	     number_of_filenames,
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

	fprintf(
	 stdout,
	 "\nChunk size: %" PRIu32 " bytes\n",
	 chunk_size );

	if( ewf_test_read_chunk_from_handle(
	     handle,
	     media_size,
	     chunk_size ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to read chunk from handle.\n" );

		goto on_error;
	}
/* TODO implement
#if defined( HAVE_MULTI_THREAD_SUPPORT )
	if( ewf_test_read_chunk_from_handle_multi_thread(
	     handle,
	     media_size,
	     EWF_TEST_READ_CHUNK_NUMBER_OF_THREADS ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to read chunk from handle in multiple threads.\n" );

		goto on_error;
	}
#endif
*/
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_glob_wide_free(
	     filenames,
	     number_of_filenames,
	     &error ) != 1 )
#else
	if( libewf_glob_free(
	     filenames,
	     number_of_filenames,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

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
	if( filenames != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libewf_glob_wide_free(
		 filenames,
		 number_of_filenames,
		 NULL );
#else
		libewf_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );
#endif
	}
	return( EXIT_FAILURE );
}


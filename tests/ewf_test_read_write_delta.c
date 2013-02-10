/*
 * Expert Witness Compression Format (EWF) library write testing program
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

/* Define to make ewf_test_read_write_delta generate verbose output
#define EWF_TEST_READ_WRITE_DELTA_VERBOSE
 */

/* Tests reading/writing data of a specific size at a specific offset
 * Return 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_write_delta(
     char * const filenames[],
     int number_of_filenames,
     const libcstring_system_character_t *delta_segment_filename,
     off64_t write_offset,
     size64_t write_size,
     libcerror_error_t **error )
{
	libewf_handle_t *handle              = NULL;
	uint8_t *buffer                      = NULL;
	static char *function                = "ewf_test_read_write_delta";
	size_t delta_segment_filename_length = 0;
	size_t read_size                     = 0;
	ssize_t read_count                   = 0;
	ssize_t write_count                  = 0;

	if( libewf_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( EWF_TEST_READ_WRITE_DELTA_VERBOSE )
	libewf_notify_set_verbose(
	 1 );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ_WRITE,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ_WRITE,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	if( delta_segment_filename != NULL )
	{
		delta_segment_filename_length = libcstring_system_string_length(
		                                 delta_segment_filename );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_handle_set_delta_segment_filename_wide(
		     handle,
		     delta_segment_filename,
		     delta_segment_filename_length,
		     error ) != 1 )
#else
		if( libewf_handle_set_delta_segment_filename(
		     handle,
		     delta_segment_filename,
		     delta_segment_filename_length,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set delta segment filename.",
			 function );

			goto on_error;
		}
	}
	if( libewf_handle_seek_offset(
	     handle,
	     write_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to seek offset: %" PRIi64 ".",
		 function,
		 write_offset );

		goto on_error;
	}
	buffer = (uint8_t *) memory_allocate(
	                      EWF_TEST_BUFFER_SIZE );

	while( write_size > 0 )
	{
		if( write_size > (size64_t) EWF_TEST_BUFFER_SIZE )
		{
			read_size = EWF_TEST_BUFFER_SIZE;
		}
		else
		{
			read_size = (size_t) write_size;
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
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable read buffer of size: %" PRIzd ".",
			 function,
			 read_size );

			goto on_error;
		}
		if( memory_set(
		     buffer,
		     (int) 'X',
		     (size_t) read_count ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in buffer.",
			 function );

			goto on_error;
		}
		if( libewf_handle_seek_offset(
		     handle,
		     -1 * (off64_t) read_size,
		     SEEK_CUR,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to seek previous offset.",
			 function );

			goto on_error;
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

			goto on_error;
		}
		if( write_count != read_count )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable write buffer of size: %" PRIzd ".",
			 function,
			 read_count );

			goto on_error;
		}
		write_offset += write_count;
		write_size   -= write_count;
	}
	memory_free(
	 buffer );

	buffer = NULL;
	
	if( libewf_handle_close(
	     handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		goto on_error;
	}
	if( libewf_handle_free(
	     &handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( buffer != NULL )
	{
		memory_free(
		 buffer );
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
	return( -1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcstring_system_character_t *option_offset   = NULL;
	libcstring_system_character_t *option_size     = NULL;
	libcstring_system_character_t *target_filename = NULL;
	libcerror_error_t *error                       = NULL;
	libcstring_system_integer_t option             = 0;
	off64_t write_offset                           = 0;
	size64_t write_size                            = 0;
	size_t string_length                           = 0;

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "B:o:t:" ) ) ) != (libcstring_system_integer_t) -1 )
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

			case (libcstring_system_integer_t) 'B':
				option_size = optarg;

				break;

			case (libcstring_system_integer_t) 'o':
				option_offset = optarg;

				break;

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
	if( option_offset != NULL )
	{
		string_length = libcstring_system_string_length(
				 option_offset );

		if( libcsystem_string_decimal_copy_to_64_bit(
		     option_offset,
		     string_length + 1,
		     (uint64_t *) &write_offset,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported write offset.\n" );

			goto on_error;
		}
	}
	if( option_size != NULL )
	{
		string_length = libcstring_system_string_length(
				 option_size );

		if( libcsystem_string_decimal_copy_to_64_bit(
		     option_size,
		     string_length + 1,
		     &write_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported write size.\n" );

			goto on_error;
		}
	}
	if( ewf_test_read_write_delta(
	     &( argv[ optind ] ),
	     argc - optind,
	     target_filename,
	     write_offset,
	     write_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read/write.\n" );

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
	return( EXIT_FAILURE );
}


/*
 * Expert Witness Compression Format (EWF) library write testing program
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
#include <liberror.h>

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

#include <libsystem.h>

#include "ewf_test_definitions.h"

/* Tests reading/writing data of a specific size at a specific offset
 * Return 1 if successful, 0 if not or -1 on error
 */
int ewf_test_read_write(
     char * const filenames[],
     int number_of_filenames,
     const libcstring_system_character_t *delta_segment_filename,
     off64_t write_offset,
     size64_t write_size,
     liberror_error_t **error )
{
	libewf_handle_t *handle              = NULL;
	uint8_t *buffer                      = NULL;
	static char *function                = "ewf_test_read_write";
	size_t delta_segment_filename_length = 0;
	size_t read_size                     = 0;
	ssize_t read_count                   = 0;
	ssize_t write_count                  = 0;
	int result                           = 1;

	if( libewf_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		return( -1 );
	}
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		libewf_handle_free(
		 &handle,
		 NULL );

		return( -1 );
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set delta segment filename.",
			 function );

			return( -1 );
		}
	}
	if( libewf_handle_seek_offset(
	     handle,
	     write_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to seek offset: %" PRIi64 ".",
		 function,
		 write_offset );

		result = -1;
	}
	buffer = (uint8_t *) memory_allocate(
	                      EWF_TEST_BUFFER_SIZE );

	if( result != -1 )
	{
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
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable read buffer of size: %" PRIzd ".",
				 function,
				 read_size );

				result = -1;

				break;
			}
			if( memory_set(
			     buffer,
			     (int) 'X',
			     (size_t) read_count ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable set value in buffer.",
				 function );

				result = -1;

				break;
			}
			if( libewf_handle_seek_offset(
			     handle,
			     write_offset,
			     SEEK_SET,
			     error ) == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to seek offset: %" PRIi64 ".",
				 function,
				 write_offset );

				result = -1;

				break;
			}
			write_count = libewf_handle_write_buffer(
				       handle,
				       buffer,
				       (size_t) read_count,
				       error );

			if( write_count < 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable write buffer of size: %" PRIzd ".",
				 function,
				 read_count );

				result = -1;

				break;
			}
			if( write_count != read_count )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable write buffer of size: %" PRIzd ".",
				 function,
				 write_size );

				result = -1;

				break;
			}
			write_offset += write_count;
			write_size   -= write_count;
		}
	}
	memory_free(
	 buffer );
	
	if( libewf_handle_close(
	     handle,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		result = -1;
	}
	if( libewf_handle_free(
	     &handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
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
	liberror_error_t *error                        = NULL;
	libcstring_system_integer_t option             = 0;
	off64_t write_offset                           = 0;
	size64_t write_size                            = 0;
	size_t string_length                           = 0;
	int result                                     = 0;

	while( ( option = libsystem_getopt(
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
				 argv[ optind ] );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'B':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &write_size,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unsupported write size.\n" );

					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					return( EXIT_FAILURE );
				}
				break;

			case (libcstring_system_integer_t) 'o':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     (uint64_t *) &write_offset,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unsupported write offset.\n" );

					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					return( EXIT_FAILURE );
				}
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
	result = ewf_test_read_write(
	          &( argv[ optind ] ),
	          argc - optind,
	          target_filename,
	          write_offset,
	          write_size,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to test read/write.\n" );

		liberror_error_backtrace_fprint(
		 error,
		 stdout );

		liberror_error_free(
		 &error );
	}
	if( result != 1 )
	{
		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}


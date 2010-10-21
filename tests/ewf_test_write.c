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

/* Tests writing data of media size to EWF file(s) with a maximum segment size
 * Return 1 if successful, 0 if not or -1 on error
 */
int ewf_test_write(
     const char *filename,
     size64_t media_size,
     size64_t maximum_segment_size,
     int8_t compression_level,
     uint8_t compression_flags,
     liberror_error_t **error )
{
	libewf_handle_t *handle = NULL;
	uint8_t *buffer         = NULL;
	static char *function   = "ewf_test_write";
	size_t write_size       = 0;
	ssize_t write_count     = 0;
	int result              = 1;
	int sector_iterator     = 0;

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
	     (wchar_t * const *) &filename,
	     1,
	     LIBEWF_OPEN_WRITE,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     handle,
	     (char * const *) &filename,
	     1,
	     LIBEWF_OPEN_WRITE,
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
	if( media_size > 0 )
	{
		if( libewf_handle_set_media_size(
		     handle,
		     media_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set media size.",
			 function );

			result = -1;
		}
	}
	if( maximum_segment_size > 0 )
	{
		if( libewf_handle_set_maximum_segment_size(
		     handle,
		     maximum_segment_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set maximum segment size.",
			 function );

			result = -1;
		}
	}
	if( libewf_handle_set_compression_values(
	     handle,
	     compression_level,
	     compression_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable set compression values.",
		 function );

		result = -1;
	}
	buffer = (uint8_t *) memory_allocate(
	                      EWF_TEST_BUFFER_SIZE );

	if( result != -1 )
	{
		write_size = 512;

		for( sector_iterator = 0;
		     sector_iterator < 26;
		     sector_iterator++ )
		{
			if( memory_set(
			     buffer,
			     (int) 'A' + sector_iterator,
			     write_size ) == NULL )
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
			write_count = libewf_handle_write_buffer(
				       handle,
				       buffer,
				       write_size,
				       error );

			if( write_count < 0 )
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
			if( write_count != (ssize_t) write_size )
			{
				if( (size64_t) write_count != media_size )
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
			}
			if( media_size > 0 )
			{
				media_size -= write_count;
			}
		}
	}
	if( result != -1 )
	{
		write_size = 3751;

		for( sector_iterator = 0;
		     sector_iterator < 26;
		     sector_iterator++ )
		{
			if( memory_set(
			     buffer,
			     (int) 'a' + sector_iterator,
			     write_size ) == NULL )
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
			write_count = libewf_handle_write_buffer(
				       handle,
				       buffer,
				       write_size,
				       error );

			if( write_count < 0 )
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
			if( write_count != (ssize_t) write_size )
			{
				if( (size64_t) write_count != media_size )
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
			}
			if( media_size > 0 )
			{
				media_size -= write_count;
			}
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
	liberror_error_t *error            = NULL;
	libcstring_system_integer_t option = 0;
	size64_t chunk_size                = 0;
	size64_t maximum_segment_size      = 0;
	size64_t media_size                = 0;
	size_t string_length               = 0;
	uint8_t compression_flags          = 0;
	int8_t compression_level           = LIBEWF_COMPRESSION_NONE;
	int result                         = 0;

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "b:B:c:S:" ) ) ) != (libcstring_system_integer_t) -1 )
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

			case (libcstring_system_integer_t) 'b':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &chunk_size,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unsupported chunk size.\n" );

					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					return( EXIT_FAILURE );
				}
				break;

			case (libcstring_system_integer_t) 'c':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( string_length != 1 )
				{
					fprintf(
					 stderr,
					 "Unsupported compression level.\n" );

					return( EXIT_FAILURE );
				}
				if( optarg[ 0 ] == (libcstring_system_integer_t) 'n' )
				{
					compression_level = LIBEWF_COMPRESSION_NONE;
					compression_flags = 0;
				}
				else if( optarg[ 0 ] == (libcstring_system_integer_t) 'e' )
				{
					compression_level = LIBEWF_COMPRESSION_NONE;
					compression_flags = LIBEWF_FLAG_COMPRESS_EMPTY_BLOCK;
				}
				else if( optarg[ 0 ] == (libcstring_system_integer_t) 'f' )
				{
					compression_level = LIBEWF_COMPRESSION_FAST;
					compression_flags = 0;
				}
				else if( optarg[ 0 ] == (libcstring_system_integer_t) 'b' )
				{
					compression_level = LIBEWF_COMPRESSION_BEST;
					compression_flags = 0;
				}
				else
				{
					fprintf(
					 stderr,
					 "Unsupported compression level.\n" );

					return( EXIT_FAILURE );
				}
				break;

			case (libcstring_system_integer_t) 'B':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &media_size,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unsupported media size.\n" );

					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					return( EXIT_FAILURE );
				}
				break;

			case (libcstring_system_integer_t) 'S':
				string_length = libcstring_system_string_length(
				                 optarg );

				if( libsystem_string_to_uint64(
				     optarg,
				     string_length + 1,
				     &maximum_segment_size,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unsupported maximum segment size.\n" );

					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					return( EXIT_FAILURE );
				}
				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing EWF image filename.\n" );

		return( EXIT_FAILURE );
	}
	result = ewf_test_write(
	          argv[ optind ],
	          media_size,
	          maximum_segment_size,
	          compression_level,
	          compression_flags,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

		liberror_error_backtrace_fprint(
		 error,
		 stderr );

		liberror_error_free(
		 &error );
	}
	if( result != 1 )
	{
		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}


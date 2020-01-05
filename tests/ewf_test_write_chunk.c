/*
 * Expert Witness Compression Format (EWF) library write chunktesting program
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "ewf_test_getopt.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int ewf_test_system_string_decimal_copy_to_64_bit(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "ewf_test_system_string_decimal_copy_to_64_bit";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

/* Tests writing data of media size to EWF file(s) with a maximum segment size
 * Return 1 if successful, 0 if not or -1 on error
 */
int ewf_test_write_chunk(
     const system_character_t *filename,
     size64_t media_size,
     size64_t maximum_segment_size,
     int8_t compression_level,
     uint8_t compression_flags,
     libcerror_error_t **error )
{
	libewf_data_chunk_t *data_chunk = NULL;
	libewf_handle_t *handle         = NULL;
	uint8_t *chunk_buffer           = NULL;
	static char *function           = "ewf_test_write_chunk";
	size_t chunk_buffer_size        = 0;
	size_t write_size               = 0;
	ssize_t process_count           = 0;
	ssize_t write_count             = 0;
	uint32_t sectors_per_chunk      = 0;
	int sector_iterator             = 0;

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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	if( media_size > 0 )
	{
		if( libewf_handle_set_media_size(
		     handle,
		     media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set media size.",
			 function );

			goto on_error;
		}
	}
	if( maximum_segment_size > 0 )
	{
		if( libewf_handle_set_maximum_segment_size(
		     handle,
		     maximum_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set maximum segment size.",
			 function );

			goto on_error;
		}
	}
	if( libewf_handle_set_compression_values(
	     handle,
	     compression_level,
	     compression_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable set compression values.",
		 function );

		goto on_error;
	}
	sectors_per_chunk = 64;

	if( libewf_handle_set_sectors_per_chunk(
	     handle,
	     sectors_per_chunk,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable set sectors per chunk.",
		 function );

		goto on_error;
	}
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
	chunk_buffer_size = sectors_per_chunk * 512;

	/* Use the chunk buffer also as checksum buffer
	 */
	chunk_buffer = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * chunk_buffer_size );

	for( sector_iterator = 0;
	     sector_iterator < 26;
	     sector_iterator++ )
	{
		if( memory_set(
		     chunk_buffer,
		     (int) 'A' + sector_iterator,
		     chunk_buffer_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable set value in chunk buffer.",
			 function );

			goto on_error;
		}
		if( chunk_buffer_size < media_size )
		{
			write_size = chunk_buffer_size;
		}
		else
		{
			write_size = (size_t) media_size;
		}
		process_count = libewf_data_chunk_write_buffer(
				 data_chunk,
				 chunk_buffer,
				 write_size,
				 error );

		if( process_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write buffer to data chunk.",
			 function );

			goto on_error;
		}
		write_count = libewf_handle_write_data_chunk(
			       handle,
			       data_chunk,
			       error );

		if( write_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable write data chunk to handle.",
			 function );

			goto on_error;
		}
		if( media_size > (size64_t) chunk_buffer_size )
		{
			media_size -= chunk_buffer_size;
		}
		else if( media_size > 0 )
		{
			media_size = 0;
		}
		if( media_size == 0 )
		{
			break;
		}
	}
	memory_free(
	 chunk_buffer );

	chunk_buffer = NULL;
	
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
	if( chunk_buffer != NULL )
	{
		memory_free(
		 chunk_buffer );
	}
	if( data_chunk != NULL )
	{
		libewf_data_chunk_free(
		 &data_chunk,
		 NULL );
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                        = NULL;
	system_character_t *option_chunk_size           = NULL;
	system_character_t *option_compression_level    = NULL;
	system_character_t *option_maximum_segment_size = NULL;
	system_character_t *option_media_size           = NULL;
	system_integer_t option                         = 0;
	size64_t chunk_size                             = 0;
	size64_t maximum_segment_size                   = 0;
	size64_t media_size                             = 0;
	size_t string_length                            = 0;
	uint8_t compression_flags                       = 0;
	int8_t compression_level                        = LIBEWF_COMPRESSION_NONE;

	while( ( option = ewf_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "b:B:c:S:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'b':
				option_chunk_size = optarg;

				break;

			case (system_integer_t) 'c':
				option_compression_level = optarg;

				break;

			case (system_integer_t) 'B':
				option_media_size = optarg;

				break;

			case (system_integer_t) 'S':
				option_maximum_segment_size = optarg;

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
	if( option_chunk_size != NULL )
	{
		string_length = system_string_length(
				 option_chunk_size );

		if( ewf_test_system_string_decimal_copy_to_64_bit(
		     option_chunk_size,
		     string_length + 1,
		     &chunk_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported chunk size.\n" );

			goto on_error;
		}
	}
	if( option_compression_level != NULL )
	{
		string_length = system_string_length(
				 option_compression_level );

		if( string_length != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported compression level.\n" );

			goto on_error;
		}
		if( option_compression_level[ 0 ] == (system_character_t) 'b' )
		{
			compression_level = LIBEWF_COMPRESSION_BEST;
			compression_flags = 0;
		}
		else if( option_compression_level[ 0 ] == (system_character_t) 'e' )
		{
			compression_level = LIBEWF_COMPRESSION_NONE;
			compression_flags = LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
		}
		else if( option_compression_level[ 0 ] == (system_character_t) 'f' )
		{
			compression_level = LIBEWF_COMPRESSION_FAST;
			compression_flags = 0;
		}
		else if( option_compression_level[ 0 ] == (system_character_t) 'n' )
		{
			compression_level = LIBEWF_COMPRESSION_NONE;
			compression_flags = 0;
		}
		else
		{
			fprintf(
			 stderr,
			 "Unsupported compression level.\n" );

			goto on_error;
		}
	}
	if( option_maximum_segment_size != NULL )
	{
		string_length = system_string_length(
				 option_maximum_segment_size );

		if( ewf_test_system_string_decimal_copy_to_64_bit(
		     option_maximum_segment_size,
		     string_length + 1,
		     &maximum_segment_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported maximum segment size.\n" );

			goto on_error;
		}
	}
	if( option_media_size != NULL )
	{
		string_length = system_string_length(
				 option_media_size );

		if( ewf_test_system_string_decimal_copy_to_64_bit(
		     option_media_size,
		     string_length + 1,
		     &media_size,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported media size.\n" );

			goto on_error;
		}
	}
	if( ewf_test_write_chunk(
	     argv[ optind ],
	     media_size,
	     maximum_segment_size,
	     compression_level,
	     compression_flags,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test write.\n" );

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


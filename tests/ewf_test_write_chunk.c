/*
 * Expert Witness Compression Format (EWF) library write chunktesting program
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

/* Tests writing data of media size to EWF file(s) with a maximum segment size
 * Return 1 if successful, 0 if not or -1 on error
 */
int ewf_test_write_chunk(
     const char *filename,
     size64_t media_size,
     size64_t maximum_segment_size,
     int8_t compression_level,
     uint8_t compression_flags,
     libcerror_error_t **error )
{
	libewf_handle_t *handle             = NULL;
	uint8_t *checksum_buffer            = NULL;
	uint8_t *chunk_buffer               = NULL;
	uint8_t *compressed_chunk_buffer    = NULL;
	static char *function               = "ewf_test_write_chunk";
	size_t chunk_buffer_size            = 0;
	size_t compressed_chunk_buffer_size = 0;
	ssize_t process_count               = 0;
	ssize_t write_count                 = 0;
	uint32_t chunk_checksum             = 0;
	uint32_t sectors_per_chunk          = 0;
	int8_t is_compressed                = 0;
	int8_t process_checksum             = 0;
	int sector_iterator                 = 0;

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
	chunk_buffer_size = sectors_per_chunk * 512;

	/* Use the chunck buffer also as checksum buffer
	 */
	chunk_buffer = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * ( chunk_buffer_size + 4 ) );

	/* The compressed data can become larger than the uncompressed data
	 */
	compressed_chunk_buffer_size = chunk_buffer_size * 2;

	compressed_chunk_buffer = (uint8_t *) memory_allocate(
	                                       sizeof( uint8_t ) * compressed_chunk_buffer_size );

	if( chunk_buffer != NULL )
	{
		checksum_buffer = &( chunk_buffer[ chunk_buffer_size - 1 ] );
	}
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
		process_count = libewf_handle_prepare_write_chunk(
				 handle,
				 chunk_buffer,
				 chunk_buffer_size,
				 compressed_chunk_buffer,
				 &compressed_chunk_buffer_size,
				 &is_compressed,
				 &chunk_checksum,
				 &process_checksum,
				 error );

		if( process_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to prepare chunk buffer before writing.",
			 function );

			goto on_error;
		}
		if( is_compressed == 0 )
		{
			write_count = libewf_handle_write_chunk(
				       handle,
				       chunk_buffer,
				       chunk_buffer_size,
				       chunk_buffer_size,
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
				       compressed_chunk_buffer,
				       compressed_chunk_buffer_size,
				       chunk_buffer_size,
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
			 chunk_buffer_size );

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
	 compressed_chunk_buffer );

	compressed_chunk_buffer = NULL;
	
	memory_free(
	 chunk_buffer );

	chunk_buffer = NULL;
	
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
	if( compressed_chunk_buffer != NULL )
	{
		memory_free(
		 compressed_chunk_buffer );
	}
	if( chunk_buffer != NULL )
	{
		memory_free(
		 chunk_buffer );
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
	libcstring_system_character_t *option_chunk_size           = NULL;
	libcstring_system_character_t *option_compression_level    = NULL;
	libcstring_system_character_t *option_maximum_segment_size = NULL;
	libcstring_system_character_t *option_media_size           = NULL;
	libcerror_error_t *error                                     = NULL;
	libcstring_system_integer_t option                          = 0;
	size64_t chunk_size                                         = 0;
	size64_t maximum_segment_size                               = 0;
	size64_t media_size                                         = 0;
	size_t string_length                                        = 0;
	uint8_t compression_flags                                   = 0;
	int8_t compression_level                                    = LIBEWF_COMPRESSION_NONE;

	while( ( option = libcsystem_getopt(
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
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'b':
				option_chunk_size = optarg;

				break;

			case (libcstring_system_integer_t) 'c':
				option_compression_level = optarg;

				break;

			case (libcstring_system_integer_t) 'B':
				option_media_size = optarg;

				break;

			case (libcstring_system_integer_t) 'S':
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
		string_length = libcstring_system_string_length(
				 option_chunk_size );

		if( libcsystem_string_decimal_copy_to_64_bit(
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
		string_length = libcstring_system_string_length(
				 option_compression_level );

		if( string_length != 1 )
		{
			fprintf(
			 stderr,
			 "Unsupported compression level.\n" );

			goto on_error;
		}
		if( option_compression_level[ 0 ] == (libcstring_system_character_t) 'n' )
		{
			compression_level = LIBEWF_COMPRESSION_NONE;
			compression_flags = 0;
		}
		else if( option_compression_level[ 0 ] == (libcstring_system_character_t) 'e' )
		{
			compression_level = LIBEWF_COMPRESSION_NONE;
			compression_flags = LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
		}
		else if( option_compression_level[ 0 ] == (libcstring_system_character_t) 'f' )
		{
			compression_level = LIBEWF_COMPRESSION_FAST;
			compression_flags = 0;
		}
		else if( option_compression_level[ 0 ] == (libcstring_system_character_t) 'b' )
		{
			compression_level = LIBEWF_COMPRESSION_BEST;
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
		string_length = libcstring_system_string_length(
				 option_maximum_segment_size );

		if( libcsystem_string_decimal_copy_to_64_bit(
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
		string_length = libcstring_system_string_length(
				 option_media_size );

		if( libcsystem_string_decimal_copy_to_64_bit(
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


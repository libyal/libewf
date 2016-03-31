/*
 * Library handle type get values testing program
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
#include <file_stream.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_libcstring.h"
#include "ewf_test_libcsystem.h"
#include "ewf_test_libewf.h"
#include "ewf_test_unused.h"

/* Define to make ewf_test_handle_get_values generate verbose output
#define EWF_TEST_HANDLE_GET_VALUES_VERBOSE
 */

/* Tests retrieving header values from the handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_handle_get_header_values(
     libewf_handle_t *handle,
     libcerror_error_t **error )
{
	uint8_t identifier[ 128 ];
	uint8_t utf8_value[ 128 ];
	uint16_t utf16_value[ 128 ];

	size_t identifier_size           = 0;
	size_t utf8_value_size           = 0;
	size_t utf16_value_size          = 0;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	fprintf(
	 stdout,
	 "Testing libewf_handle_get_number_of_header_values\t" );

	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          error );

	if( result == 1 )
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

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve number of header values.\n" );

		return( -1 );
	}
	if( number_of_header_values == 0 )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_header_value_identifier_size\t" );

	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve header value: 0 identifier size.\n" );

		return( -1 );
	}
	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_header_value_identifier\t" );

	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve header value: 0 identifier.\n" );

		return( -1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf8_header_value_size\t" );

	result = libewf_handle_get_utf8_header_value_size(
		  handle,
		  identifier,
		  identifier_size - 1,
		  &utf8_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-8 header value size.\n" );

		return( -1 );
	}
	if( ( utf8_value_size <= 1 )
	 || ( utf8_value_size > 128 ) )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf8_header_value\t" );

	result = libewf_handle_get_utf8_header_value(
		  handle,
		  identifier,
		  identifier_size - 1,
		  utf8_value,
		  utf8_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-8 header value.\n" );

		return( -1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf16_header_value_size\t" );

	result = libewf_handle_get_utf16_header_value_size(
		  handle,
		  identifier,
		  identifier_size - 1,
		  &utf16_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-16 header value size.\n" );

		return( -1 );
	}
	if( ( utf16_value_size <= 1 )
	 || ( utf16_value_size > 128 ) )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf16_header_value\t" );

	result = libewf_handle_get_utf16_header_value(
		  handle,
		  identifier,
		  identifier_size - 1,
		  utf16_value,
		  utf16_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-16 header value.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Tests retrieving hash values from the handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_handle_get_hash_values(
     libewf_handle_t *handle,
     libcerror_error_t **error )
{
	uint8_t identifier[ 128 ];
	uint8_t utf8_value[ 128 ];
	uint16_t utf16_value[ 128 ];

	size_t identifier_size         = 0;
	size_t utf8_value_size         = 0;
	size_t utf16_value_size        = 0;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	fprintf(
	 stdout,
	 "Testing libewf_handle_get_number_of_hash_values\t" );

	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          error );

	if( result == 1 )
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

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve number of hash values.\n" );

		return( -1 );
	}
	if( number_of_hash_values == 0 )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_hash_value_identifier_size\t" );

	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve hash value: 0 identifier size.\n" );

		return( -1 );
	}
	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_hash_value_identifier\t" );

	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve hash value: 0 identifier.\n" );

		return( -1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf8_hash_value_size\t" );

	result = libewf_handle_get_utf8_hash_value_size(
		  handle,
		  identifier,
		  identifier_size - 1,
		  &utf8_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-8 hash value size.\n" );

		return( -1 );
	}
	if( ( utf8_value_size <= 1 )
	 || ( utf8_value_size > 128 ) )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf8_hash_value\t" );

	result = libewf_handle_get_utf8_hash_value(
		  handle,
		  identifier,
		  identifier_size - 1,
		  utf8_value,
		  utf8_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-8 hash value.\n" );

		return( -1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf16_hash_value_size\t" );

	result = libewf_handle_get_utf16_hash_value_size(
		  handle,
		  identifier,
		  identifier_size - 1,
		  &utf16_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-16 hash value size.\n" );

		return( -1 );
	}
	if( ( utf16_value_size <= 1 )
	 || ( utf16_value_size > 128 ) )
	{
		return( 1 );
	}
	fprintf(
	 stdout,
	 "Testing libewf_handle_get_utf16_hash_value\t" );

	result = libewf_handle_get_utf16_hash_value(
		  handle,
		  identifier,
		  identifier_size - 1,
		  utf16_value,
		  utf16_value_size,
		  error );

	if( result == -1 )
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve UTF-16 hash value.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Tests retrieving values from the handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int ewf_test_handle_get_values(
     libcstring_system_character_t *source,
     libcerror_error_t **error )
{
	libcstring_system_character_t **filenames = NULL;
	libewf_handle_t *handle                   = NULL;
	size_t string_length                      = 0;
	int number_of_filenames                   = 0;
	int result                                = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	string_length = libcstring_wide_string_length(
	                 source );

	if( libewf_glob_wide(
	     source,
	     string_length,
	     LIBEWF_FORMAT_UNKNOWN,
	     &filenames,
	     &number_of_filenames,
	     error ) != 1 )
#else
	string_length = libcstring_narrow_string_length(
	                 source );

	if( libewf_glob(
	     source,
	     string_length,
	     LIBEWF_FORMAT_UNKNOWN,
	     &filenames,
	     &number_of_filenames,
	     error ) != 1 )
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
	if( libewf_handle_initialize(
	     &handle,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create handle.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     handle,
	     (wchar_t * const *) filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     handle,
	     (char * const *) filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open handle.\n" );

		goto on_error;
	}
	result = ewf_test_handle_get_header_values(
	          handle,
	          error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve header values.\n" );

		goto on_error;
	}
	result = ewf_test_handle_get_hash_values(
	          handle,
	          error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve hash values.\n" );

		goto on_error;
	}
	if( libewf_handle_close(
	     handle,
	     error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close handle.\n" );

		goto on_error;
	}
	if( libewf_handle_free(
	     &handle,
	     error ) != 1 )
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
	     error ) != 1 )
#else
	if( libewf_glob_free(
	     filenames,
	     number_of_filenames,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to free glob.\n" );

		goto on_error;
	}
	return( result );

on_error:
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
	libcerror_error_t *error              = NULL;
	libcstring_system_character_t *source = NULL;
	libcstring_system_integer_t option    = 0;
	int result                            = 0;

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

#if defined( HAVE_DEBUG_OUTPUT ) && defined( EWF_TEST_HANDLE_GET_VALUES_VERBOSE )
	libewf_notify_set_verbose(
	 1 );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif
	result = ewf_test_handle_get_values(
	          source,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve values from handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	return( EXIT_FAILURE );
}


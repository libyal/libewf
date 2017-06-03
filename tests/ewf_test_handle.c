/*
 * Library handle type test program
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_getopt.h"
#include "ewf_test_libbfio.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libclocale.h"
#include "ewf_test_libewf.h"
#include "ewf_test_libuna.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make ewf_test_handle generate verbose output
#define EWF_TEST_HANDLE_VERBOSE
 */

/* Retrieves source as a narrow string
 * Returns 1 if successful or -1 on error
 */
int ewf_test_handle_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error )
{
	static char *function     = "ewf_test_handle_get_narrow_source";
	size_t narrow_source_size = 0;
	size_t source_length      = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( narrow_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid narrow string.",
		 function );

		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid narrow string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow string size.",
		 function );

		return( -1 );
	}
#else
	narrow_source_size = source_length + 1;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( narrow_string_size < narrow_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: narrow string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     narrow_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
	narrow_string[ source_length ] = 0;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves source as a wide string
 * Returns 1 if successful or -1 on error
 */
int ewf_test_handle_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error )
{
	static char *function   = "ewf_test_handle_get_wide_source";
	size_t source_length    = 0;
	size_t wide_source_size = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result              = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( wide_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wide string.",
		 function );

		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wide string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_source_size = source_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine wide string size.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( wide_string_size < wide_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: wide string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     wide_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}
	wide_string[ source_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int ewf_test_handle_open_source(
     libewf_handle_t **handle,
     const system_character_t *source,
     libcerror_error_t **error )
{
	system_character_t **filenames = NULL;
	static char *function          = "ewf_test_handle_open_source";
	size_t source_length           = 0;
	int number_of_filenames        = 0;
	int result                     = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_glob_wide(
	          source,
	          source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          error );
#else
	result = libewf_glob(
	          source,
	          source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to resolve filename(s).",
		 function );

		goto on_error;
	}
	if( libewf_handle_initialize(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_open_wide(
	          *handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          error );
#else
	result = libewf_handle_open(
	          *handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          error );
#else
	result = libewf_glob_free(
	          filenames,
	          number_of_filenames,
	          error );
#endif
	filenames = NULL;

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free globbed filenames.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *handle != NULL )
	{
		libewf_handle_free(
		 handle,
		 NULL );
	}
	if( filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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

/* Closes and frees a source handle
 * Returns 1 if successful or -1 on error
 */
int ewf_test_handle_close_source(
     libewf_handle_t **handle,
     libcerror_error_t **error )
{
	static char *function = "ewf_test_handle_close_source";
	int result            = 0;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_close(
	     *handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close handle.",
		 function );

		result = -1;
	}
	if( libewf_handle_free(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libewf_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libewf_handle_t *handle         = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_handle_initialize(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_free(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libewf_handle_initialize(
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	handle = (libewf_handle_t *) 0x12345678UL;

	result = libewf_handle_initialize(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	handle = NULL;

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_handle_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_handle_initialize(
		          &handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libewf_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EWF_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libewf_handle_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_handle_initialize(
		          &handle,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libewf_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EWF_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libewf_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_handle_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_handle_free(
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_open functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_open(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libewf_handle_t *handle  = NULL;
	char **filenames         = NULL;
	size_t source_length     = 0;
	int number_of_filenames  = 0;
	int result               = 0;

	/* Initialize test
	 */
	source_length = system_string_length(
	                 source );

	result = libewf_glob(
	          source,
	          source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "filenames",
         filenames );

        EWF_TEST_ASSERT_GREATER_THAN_INT(
         "number_of_filenames",
         number_of_filenames,
	 0 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_initialize(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open
	 */
	result = libewf_handle_open(
	          handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Clean up
	 */
	result = libewf_handle_close(
	          handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_free(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_glob_free(
	          filenames,
	          number_of_filenames,
	          &error );

	filenames = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libewf_handle_free(
		 &handle,
		 NULL );
	}
	if( filenames != NULL )
	{
		libewf_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libewf_handle_open functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_open(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libewf_handle_t *handle  = NULL;
	wchar_t **filenames      = NULL;
	size_t source_length     = 0;
	int number_of_filenames  = 0;
	int result               = 0;

	/* Initialize test
	 */
	source_length = system_string_length(
	                 source );

	result = libewf_glob_wide(
	          source,
	          source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "filenames",
         filenames );

        EWF_TEST_ASSERT_GREATER_THAN_INT(
         "number_of_filenames",
         number_of_filenames,
	 0 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_initialize(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open
	 */
	result = libewf_handle_open_wide(
	          handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Clean up
	 */
	result = libewf_handle_close(
	          handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_free(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libewf_handle_free(
		 &handle,
		 NULL );
	}
	if( filenames != NULL )
	{
		libewf_glob_wide_free(
		 filenames,
		 number_of_filenames,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libewf_handle_close function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_handle_close(
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_open and libewf_handle_close functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error       = NULL;
	libewf_handle_t *handle        = NULL;
	system_character_t **filenames = NULL;
	size_t source_length           = 0;
	int number_of_filenames        = 0;
	int result                     = 0;

	/* Initialize test
	 */
	source_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_glob_wide(
	          source,
	          source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );
#else
	result = libewf_glob(
	          source,
	          source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );
#endif

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "filenames",
         filenames );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_initialize(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_open_wide(
	          handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          &error );
#else
	result = libewf_handle_open(
	          handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          &error );
#endif

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_close(
	          handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_open_wide(
	          handle,
	          (wchar_t * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          &error );
#else
	result = libewf_handle_open(
	          handle,
	          (char * const *) filenames,
	          number_of_filenames,
	          LIBEWF_OPEN_READ,
	          &error );
#endif

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_close(
	          handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Clean up
	 */
	result = libewf_handle_free(
	          &handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "handle",
         handle );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_glob_wide_free(
	          filenames,
	          number_of_filenames,
	          &error );
#else
	result = libewf_glob_free(
	          filenames,
	          number_of_filenames,
	          &error );
#endif

	filenames = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libewf_handle_free(
		 &handle,
		 NULL );
	}
	if( filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	return( 0 );
}

/* Tests the libewf_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_signal_abort(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_signal_abort(
	          handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libewf_handle_signal_abort(
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_read_buffer(
     libewf_handle_t *handle )
{
	uint8_t buffer[ 16 ];

	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;

	/* Determine size
	 */
	offset = libewf_handle_seek_offset(
	          handle,
	          0,
	          SEEK_END,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	/* Reset offset to 0
	 */
	offset = libewf_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	if( size > 16 )
	{
		read_count = libewf_handle_read_buffer(
		              handle,
		              buffer,
		              16,
		              &error );

		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 16 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
/* TODO read on size boundary */
/* TODO read beyond size boundary */

	/* Reset offset to 0
	 */
	offset = libewf_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libewf_handle_read_buffer(
	              NULL,
	              buffer,
	              16,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libewf_handle_read_buffer(
	              handle,
	              NULL,
	              16,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libewf_handle_read_buffer(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_data_chunk function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_data_chunk(
     libewf_handle_t *handle )
{
	libcerror_error_t *error        = NULL;
	libewf_data_chunk_t *data_chunk = 0;
	int data_chunk_is_set           = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_data_chunk(
	          handle,
	          &data_chunk,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_chunk_is_set = result;

	if( data_chunk_is_set != 0 )
	{
		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "data_chunk",
		 data_chunk );

		result = libewf_data_chunk_free(
		          &data_chunk,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_handle_get_data_chunk(
	          NULL,
	          &data_chunk,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "data_chunk",
	 data_chunk );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( data_chunk_is_set != 0 )
	{
		result = libewf_handle_get_data_chunk(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "data_chunk",
		 data_chunk );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( data_chunk != NULL )
	{
		libewf_data_chunk_free(
		 &data_chunk,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_handle_seek_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_seek_offset(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	off64_t offset           = 0;

	/* Test regular cases
	 */
	offset = libewf_handle_seek_offset(
	          handle,
	          0,
	          SEEK_END,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	offset = libewf_handle_seek_offset(
	          handle,
	          1024,
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 1024 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libewf_handle_seek_offset(
	          handle,
	          -512,
	          SEEK_CUR,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 512 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libewf_handle_seek_offset(
	          handle,
	          (off64_t) ( size + 512 ),
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) ( size + 512 ) );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libewf_handle_seek_offset(
	          handle,
	          0,
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libewf_handle_seek_offset(
	          NULL,
	          0,
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libewf_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libewf_handle_seek_offset(
	          handle,
	          -1,
	          SEEK_CUR,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	offset = libewf_handle_seek_offset(
	          handle,
	          (off64_t) ( -1 * ( size + 1 ) ),
	          SEEK_END,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_offset(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int offset_is_set        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_offset(
	          handle,
	          &offset,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_offset(
	          NULL,
	          &offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( offset_is_set != 0 )
	{
		result = libewf_handle_get_offset(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_segment_filename_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_segment_filename_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	size_t segment_filename_size     = 0;
	int result                       = 0;
	int segment_filename_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_segment_filename_size(
	          handle,
	          &segment_filename_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	segment_filename_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_segment_filename_size(
	          NULL,
	          &segment_filename_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( segment_filename_size_is_set != 0 )
	{
		result = libewf_handle_get_segment_filename_size(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libewf_handle_get_segment_filename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_segment_filename_size_wide(
     libewf_handle_t *handle )
{
	libcerror_error_t *error              = NULL;
	size_t segment_filename_size_wide     = 0;
	int result                            = 0;
	int segment_filename_size_wide_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_segment_filename_size_wide(
	          handle,
	          &segment_filename_size_wide,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	segment_filename_size_wide_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_segment_filename_size_wide(
	          NULL,
	          &segment_filename_size_wide,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( segment_filename_size_wide_is_set != 0 )
	{
		result = libewf_handle_get_segment_filename_size_wide(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libewf_handle_get_maximum_segment_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_maximum_segment_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error        = NULL;
	size64_t maximum_segment_size   = 0;
	int maximum_segment_size_is_set = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_maximum_segment_size(
	          handle,
	          &maximum_segment_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	maximum_segment_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_maximum_segment_size(
	          NULL,
	          &maximum_segment_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( maximum_segment_size_is_set != 0 )
	{
		result = libewf_handle_get_maximum_segment_size(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_filename_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_filename_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size_t filename_size     = 0;
	int filename_size_is_set = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_filename_size(
	          handle,
	          &filename_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	filename_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_filename_size(
	          NULL,
	          &filename_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( filename_size_is_set != 0 )
	{
		result = libewf_handle_get_filename_size(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libewf_handle_get_filename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_filename_size_wide(
     libewf_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	size_t filename_size_wide     = 0;
	int filename_size_wide_is_set = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_filename_size_wide(
	          handle,
	          &filename_size_wide,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	filename_size_wide_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_filename_size_wide(
	          NULL,
	          &filename_size_wide,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( filename_size_wide_is_set != 0 )
	{
		result = libewf_handle_get_filename_size_wide(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEWF_HAVE_BFIO )

/* Tests the libewf_handle_get_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_file_io_handle(
     libewf_handle_t *handle )
{
	libcerror_error_t *error        = NULL;
	libbfio_handle_t file_io_handle = 0;
	int file_io_handle_is_set       = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_file_io_handle(
	          handle,
	          &file_io_handle,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	file_io_handle_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_file_io_handle(
	          NULL,
	          &file_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( file_io_handle_is_set != 0 )
	{
		result = libewf_handle_get_file_io_handle(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( LIBEWF_HAVE_BFIO ) */

/* Tests the libewf_handle_get_root_file_entry function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_root_file_entry(
     libewf_handle_t *handle )
{
	libcerror_error_t *error             = NULL;
	libewf_file_entry_t *root_file_entry = 0;
	int result                           = 0;
	int root_file_entry_is_set           = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_root_file_entry(
	          handle,
	          &root_file_entry,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	root_file_entry_is_set = result;

	if( root_file_entry_is_set != 0 )
	{
		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "root_file_entry",
		 root_file_entry );

		result = libewf_file_entry_free(
		          &root_file_entry,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_handle_get_root_file_entry(
	          NULL,
	          &root_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "root_file_entry",
	 root_file_entry );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( root_file_entry_is_set != 0 )
	{
		result = libewf_handle_get_root_file_entry(
		          handle,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "root_file_entry",
		 root_file_entry );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( root_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &root_file_entry,
		 NULL );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libcerror_error_t *error   = NULL;
	libewf_handle_t *handle    = NULL;
	system_character_t *source = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

	while( ( option = ewf_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
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
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( EWF_TEST_HANDLE_VERBOSE )
	libewf_notify_set_verbose(
	 1 );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

	EWF_TEST_RUN(
	 "libewf_handle_initialize",
	 ewf_test_handle_initialize );

	EWF_TEST_RUN(
	 "libewf_handle_free",
	 ewf_test_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_check_file_signature_wide(
		          source,
		          &error );
#else
		result = libewf_check_file_signature(
		          source,
		          &error );
#endif

		EWF_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

	        EWF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
	if( result != 0 )
	{
		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_open",
		 ewf_test_handle_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_open_wide",
		 ewf_test_handle_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEWF_HAVE_BFIO )

		/* TODO add test for libewf_handle_open_file_io_handle */

#endif /* defined( LIBEWF_HAVE_BFIO ) */

		EWF_TEST_RUN(
		 "libewf_handle_close",
		 ewf_test_handle_close );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_open_close",
		 ewf_test_handle_open_close,
		 source );

		/* Initialize test
		 */
		result = ewf_test_handle_open_source(
		          &handle,
		          source,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        EWF_TEST_ASSERT_IS_NOT_NULL(
	         "handle",
	         handle );

	        EWF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		/* TODO: add tests for libewf_handle_clone */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_signal_abort",
		 ewf_test_handle_signal_abort,
		 handle );

		/* TODO: add tests for libewf_handle_open_file_io_pool */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_read_buffer",
		 ewf_test_handle_read_buffer,
		 handle );

		/* TODO: add tests for libewf_handle_read_buffer_at_offset */

		/* TODO: add tests for libewf_handle_write_buffer */

		/* TODO: add tests for libewf_handle_write_buffer_at_offset */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_data_chunk",
		 ewf_test_handle_get_data_chunk,
		 handle );

		/* TODO: add tests for libewf_handle_read_data_chunk */

		/* TODO: add tests for libewf_handle_write_data_chunk */

		/* TODO: add tests for libewf_handle_write_finalize */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_seek_offset",
		 ewf_test_handle_seek_offset,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_offset",
		 ewf_test_handle_get_offset,
		 handle );

		/* TODO: add tests for libewf_handle_set_maximum_number_of_open_handles */

		/* TODO: add tests for libewf_handle_segment_files_corrupted */

		/* TODO: add tests for libewf_handle_segment_files_encrypted */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_segment_filename_size",
		 ewf_test_handle_get_segment_filename_size,
		 handle );

		/* TODO: add tests for libewf_handle_get_segment_filename */

		/* TODO: add tests for libewf_handle_set_segment_filename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_segment_filename_size_wide",
		 ewf_test_handle_get_segment_filename_size_wide,
		 handle );

		/* TODO: add tests for libewf_handle_get_segment_filename_wide */

		/* TODO: add tests for libewf_handle_set_segment_filename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_maximum_segment_size",
		 ewf_test_handle_get_maximum_segment_size,
		 handle );

		/* TODO: add tests for libewf_handle_set_maximum_segment_size */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_filename_size",
		 ewf_test_handle_get_filename_size,
		 handle );

		/* TODO: add tests for libewf_handle_get_filename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_filename_size_wide",
		 ewf_test_handle_get_filename_size_wide,
		 handle );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		/* TODO: add tests for libewf_handle_get_filename_wide */

#if defined( LIBEWF_HAVE_BFIO )

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_file_io_handle",
		 ewf_test_handle_get_file_io_handle,
		 handle );

#endif /* defined( LIBEWF_HAVE_BFIO ) */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_root_file_entry",
		 ewf_test_handle_get_root_file_entry,
		 handle );

		/* TODO: add tests for libewf_handle_get_file_entry_by_utf8_path */

		/* TODO: add tests for libewf_handle_get_file_entry_by_utf16_path */

		/* Clean up
		 */
		result = ewf_test_handle_close_source(
		          &handle,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		EWF_TEST_ASSERT_IS_NULL(
	         "handle",
	         handle );

	        EWF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		ewf_test_handle_close_source(
		 &handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}


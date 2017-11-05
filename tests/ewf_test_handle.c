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

#include "ewf_test_functions.h"
#include "ewf_test_getopt.h"
#include "ewf_test_libbfio.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_handle.h"

#if !defined( LIBEWF_HAVE_BFIO )

LIBEWF_EXTERN \
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libewf_error_t **error );

#endif /* !defined( LIBEWF_HAVE_BFIO ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make ewf_test_handle generate verbose output
#define EWF_TEST_HANDLE_VERBOSE
 */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int ewf_test_handle_open_source(
     libewf_handle_t **handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	static char *function = "ewf_test_handle_open_source";
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
	if( file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO pool.",
		 function );

		return( -1 );
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
	result = libewf_handle_open_file_io_pool(
	          *handle,
	          file_io_pool,
	          LIBEWF_OPEN_READ,
	          error );

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
	return( 1 );

on_error:
	if( *handle != NULL )
	{
		libewf_handle_free(
		 handle,
		 NULL );
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

/* Tests the libewf_handle_open function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error    = NULL;
	libewf_handle_t *handle     = NULL;
	char **filenames            = NULL;
	size_t narrow_source_length = 0;
	int number_of_filenames     = 0;
	int result                  = 0;

	/* Initialize test
	 */
	result = ewf_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	narrow_source_length = narrow_string_length(
	                        narrow_source );

	result = libewf_glob(
	          narrow_source,
	          narrow_source_length,
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

/* Tests the libewf_handle_open_wide functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error  = NULL;
	libewf_handle_t *handle   = NULL;
	wchar_t **filenames       = NULL;
	size_t wide_source_length = 0;
	int number_of_filenames   = 0;
	int result                = 0;

	/* Initialize test
	 */
	result = ewf_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	wide_source_length = wide_string_length(
	                      wide_source );

	result = libewf_glob_wide(
	          wide_source,
	          wide_source_length,
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

/* Tests the libewf_handle_open_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_open_file_io_pool(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libcerror_error_t *error         = NULL;
	libewf_handle_t *handle          = NULL;
	system_character_t **filenames   = NULL;
	size_t string_length             = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	/* Initialize test
	 */
	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_glob_wide(
	          source,
	          string_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );
#else
	result = libewf_glob(
	          source,
	          string_length,
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

	EWF_TEST_ASSERT_GREATER_THAN_INT(
	 "number_of_filenames",
	 number_of_filenames,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_pool_initialize(
	          &file_io_pool,
	          number_of_filenames,
	          LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NOT_NULL(
         "file_io_pool",
         file_io_pool );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        EWF_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_handle",
	         file_io_handle );

	        EWF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		string_length = system_string_length(
		                 filenames[ filename_index ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          filenames[ filename_index ],
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          filenames[ filename_index ],
		          string_length,
		          &error );
#endif
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_pool_set_handle(
		          file_io_pool,
		          filename_index,
		          file_io_handle,
		          LIBBFIO_OPEN_READ,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        EWF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		file_io_handle = NULL;
	}
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
	result = libewf_handle_open_file_io_pool(
	          handle,
	          file_io_pool,
	          LIBEWF_OPEN_READ,
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
	result = libewf_handle_open_file_io_pool(
	          NULL,
	          file_io_pool,
	          LIBEWF_OPEN_READ,
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

	result = libewf_handle_open_file_io_pool(
	          handle,
	          NULL,
	          LIBEWF_OPEN_READ,
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

	result = libewf_handle_open_file_io_pool(
	          handle,
	          file_io_pool,
	          -1,
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

	/* Test open when already opened
	 */
	result = libewf_handle_open_file_io_pool(
	          handle,
	          file_io_pool,
	          LIBEWF_OPEN_READ,
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

	result = libbfio_pool_free(
	          &file_io_pool,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
         "file_io_pool",
         file_io_pool );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
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
	size64_t media_size      = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	int result               = 0;

	/* Determine size
	 */
	result = libewf_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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

	/* Test regular cases
	 */
	if( media_size > 16 )
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

		/* Set offset to media_size - 8
		 */
		offset = libewf_handle_seek_offset(
		          handle,
		          -8,
		          SEEK_END,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) media_size - 8 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on media_size boundary
		 */
		read_count = libewf_handle_read_buffer(
		              handle,
		              buffer,
		              16,
		              &error );

		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond media_size boundary
		 */
		read_count = libewf_handle_read_buffer(
		              handle,
		              buffer,
		              16,
		              &error );

		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

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
	}
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

/* Tests the libewf_handle_read_buffer_at_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_read_buffer_at_offset(
     libewf_handle_t *handle )
{
	uint8_t buffer[ 16 ];

	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	ssize_t read_count       = 0;
	int result               = 0;

	/* Determine size
	 */
	result = libewf_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	if( media_size > 16 )
	{
		read_count = libewf_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              16,
		              0,
		              &error );

		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 16 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on media_size boundary
		 */
		read_count = libewf_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              16,
		              media_size - 8,
		              &error );

		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond media_size boundary
		 */
		read_count = libewf_handle_read_buffer_at_offset(
		              handle,
		              buffer,
		              16,
		              media_size + 8,
		              &error );

		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	read_count = libewf_handle_read_buffer_at_offset(
	              NULL,
	              buffer,
	              16,
	              0,
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

	read_count = libewf_handle_read_buffer_at_offset(
	              handle,
	              NULL,
	              16,
	              0,
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

	read_count = libewf_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              0,
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

	read_count = libewf_handle_read_buffer_at_offset(
	              handle,
	              buffer,
	              16,
	              -1,
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

/* Tests the libewf_handle_get_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_file_io_handle(
     libewf_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = 0;
	int file_io_handle_is_set        = 0;
	int result                       = 0;

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

/* Tests the libewf_handle_get_sectors_per_chunk function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_sectors_per_chunk(
     libewf_handle_t *handle )
{
	libcerror_error_t *error     = NULL;
	uint32_t sectors_per_chunk   = 0;
	int result                   = 0;
	int sectors_per_chunk_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_sectors_per_chunk(
	          handle,
	          &sectors_per_chunk,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	sectors_per_chunk_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_sectors_per_chunk(
	          NULL,
	          &sectors_per_chunk,
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

	if( sectors_per_chunk_is_set != 0 )
	{
		result = libewf_handle_get_sectors_per_chunk(
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

/* Tests the libewf_handle_get_bytes_per_sector function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_bytes_per_sector(
     libewf_handle_t *handle )
{
	libcerror_error_t *error    = NULL;
	uint32_t bytes_per_sector   = 0;
	int bytes_per_sector_is_set = 0;
	int result                  = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_bytes_per_sector(
	          handle,
	          &bytes_per_sector,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	bytes_per_sector_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_bytes_per_sector(
	          NULL,
	          &bytes_per_sector,
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

	if( bytes_per_sector_is_set != 0 )
	{
		result = libewf_handle_get_bytes_per_sector(
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

/* Tests the libewf_handle_get_number_of_sectors function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_sectors(
     libewf_handle_t *handle )
{
	libcerror_error_t *error     = NULL;
	uint64_t number_of_sectors   = 0;
	int number_of_sectors_is_set = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_number_of_sectors(
	          handle,
	          &number_of_sectors,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_sectors_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_number_of_sectors(
	          NULL,
	          &number_of_sectors,
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

	if( number_of_sectors_is_set != 0 )
	{
		result = libewf_handle_get_number_of_sectors(
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

/* Tests the libewf_handle_get_chunk_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_chunk_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size32_t chunk_size      = 0;
	int chunk_size_is_set    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_chunk_size(
	          handle,
	          &chunk_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	chunk_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_chunk_size(
	          NULL,
	          &chunk_size,
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

	if( chunk_size_is_set != 0 )
	{
		result = libewf_handle_get_chunk_size(
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

/* Tests the libewf_handle_get_error_granularity function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_error_granularity(
     libewf_handle_t *handle )
{
	libcerror_error_t *error     = NULL;
	uint32_t error_granularity   = 0;
	int error_granularity_is_set = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_error_granularity(
	          handle,
	          &error_granularity,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	error_granularity_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_error_granularity(
	          NULL,
	          &error_granularity,
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

	if( error_granularity_is_set != 0 )
	{
		result = libewf_handle_get_error_granularity(
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

/* Tests the libewf_handle_get_compression_method function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_compression_method(
     libewf_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	uint16_t compression_method   = 0;
	int compression_method_is_set = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_compression_method(
	          handle,
	          &compression_method,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	compression_method_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_compression_method(
	          NULL,
	          &compression_method,
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

	if( compression_method_is_set != 0 )
	{
		result = libewf_handle_get_compression_method(
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

/* Tests the libewf_handle_get_media_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_media_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	int media_size_is_set    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_media_size(
	          handle,
	          &media_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	media_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_media_size(
	          NULL,
	          &media_size,
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

	if( media_size_is_set != 0 )
	{
		result = libewf_handle_get_media_size(
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

/* Tests the libewf_handle_get_media_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_media_type(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t media_type       = 0;
	int media_type_is_set    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_media_type(
	          handle,
	          &media_type,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	media_type_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_media_type(
	          NULL,
	          &media_type,
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

	if( media_type_is_set != 0 )
	{
		result = libewf_handle_get_media_type(
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

/* Tests the libewf_handle_get_media_flags function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_media_flags(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t media_flags      = 0;
	int media_flags_is_set   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_media_flags(
	          handle,
	          &media_flags,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	media_flags_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_media_flags(
	          NULL,
	          &media_flags,
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

	if( media_flags_is_set != 0 )
	{
		result = libewf_handle_get_media_flags(
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

/* Tests the libewf_handle_get_format function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_format(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t format           = 0;
	int format_is_set        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_format(
	          handle,
	          &format,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	format_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_format(
	          NULL,
	          &format,
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

	if( format_is_set != 0 )
	{
		result = libewf_handle_get_format(
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

/* Tests the libewf_handle_get_number_of_acquiry_errors function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_acquiry_errors(
     libewf_handle_t *handle )
{
	libcerror_error_t *error            = NULL;
	uint32_t number_of_acquiry_errors   = 0;
	int number_of_acquiry_errors_is_set = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_number_of_acquiry_errors(
	          handle,
	          &number_of_acquiry_errors,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_acquiry_errors_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_number_of_acquiry_errors(
	          NULL,
	          &number_of_acquiry_errors,
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

	if( number_of_acquiry_errors_is_set != 0 )
	{
		result = libewf_handle_get_number_of_acquiry_errors(
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

/* Tests the libewf_handle_get_number_of_checksum_errors function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_checksum_errors(
     libewf_handle_t *handle )
{
	libcerror_error_t *error             = NULL;
	uint32_t number_of_checksum_errors   = 0;
	int number_of_checksum_errors_is_set = 0;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_number_of_checksum_errors(
	          handle,
	          &number_of_checksum_errors,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_checksum_errors_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_number_of_checksum_errors(
	          NULL,
	          &number_of_checksum_errors,
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

	if( number_of_checksum_errors_is_set != 0 )
	{
		result = libewf_handle_get_number_of_checksum_errors(
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

/* Tests the libewf_handle_get_number_of_sessions function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_sessions(
     libewf_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	uint32_t number_of_sessions   = 0;
	int number_of_sessions_is_set = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_number_of_sessions(
	          handle,
	          &number_of_sessions,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_sessions_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_number_of_sessions(
	          NULL,
	          &number_of_sessions,
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

	if( number_of_sessions_is_set != 0 )
	{
		result = libewf_handle_get_number_of_sessions(
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

/* Tests the libewf_handle_get_number_of_tracks function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_tracks(
     libewf_handle_t *handle )
{
	libcerror_error_t *error    = NULL;
	uint32_t number_of_tracks   = 0;
	int number_of_tracks_is_set = 0;
	int result                  = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_number_of_tracks(
	          handle,
	          &number_of_tracks,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_tracks_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_number_of_tracks(
	          NULL,
	          &number_of_tracks,
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

	if( number_of_tracks_is_set != 0 )
	{
		result = libewf_handle_get_number_of_tracks(
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

/* Tests the libewf_handle_get_header_values_date_format function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_header_values_date_format(
     libewf_handle_t *handle )
{
	libcerror_error_t *error             = NULL;
	int header_values_date_format        = 0;
	int header_values_date_format_is_set = 0;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_header_values_date_format(
	          handle,
	          &header_values_date_format,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	header_values_date_format_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_header_values_date_format(
	          NULL,
	          &header_values_date_format,
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

	if( header_values_date_format_is_set != 0 )
	{
		result = libewf_handle_get_header_values_date_format(
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

/* Tests the libewf_handle_get_number_of_header_values function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_header_values(
     libewf_handle_t *handle )
{
	libcerror_error_t *error           = NULL;
	uint32_t number_of_header_values   = 0;
	int number_of_header_values_is_set = 0;
	int result                         = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_header_values_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_number_of_header_values(
	          NULL,
	          &number_of_header_values,
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

	if( number_of_header_values_is_set != 0 )
	{
		result = libewf_handle_get_number_of_header_values(
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

/* Tests the libewf_handle_get_number_of_hash_values function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_hash_values(
     libewf_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	uint32_t number_of_hash_values   = 0;
	int number_of_hash_values_is_set = 0;
	int result                       = 0;

	/* Test regular cases
	 */
	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_hash_values_is_set = result;

	/* Test error cases
	 */
	result = libewf_handle_get_number_of_hash_values(
	          NULL,
	          &number_of_hash_values,
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

	if( number_of_hash_values_is_set != 0 )
	{
		result = libewf_handle_get_number_of_hash_values(
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
	libbfio_handle_t *file_io_handle = NULL;
	libbfio_pool_t *file_io_pool     = NULL;
	libcerror_error_t *error         = NULL;
	libewf_handle_t *handle          = NULL;
	system_character_t **filenames   = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

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

	/* TODO: add tests for libewf_handle_clone */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_glob_wide(
		          source,
		          string_length,
		          LIBEWF_FORMAT_UNKNOWN,
		          &filenames,
		          &number_of_filenames,
		          &error );
#else
		result = libewf_glob(
		          source,
		          string_length,
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

		EWF_TEST_ASSERT_GREATER_THAN_INT(
		 "number_of_filenames",
		 number_of_filenames,
		 0 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_pool_initialize(
		          &file_io_pool,
		          number_of_filenames,
		          LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        EWF_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_pool",
	         file_io_pool );

	        EWF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			result = libbfio_file_initialize(
			          &file_io_handle,
			          &error );

			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 1 );

		        EWF_TEST_ASSERT_IS_NOT_NULL(
		         "file_io_handle",
		         file_io_handle );

		        EWF_TEST_ASSERT_IS_NULL(
		         "error",
		         error );

			string_length = system_string_length(
			                 filenames[ filename_index ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libbfio_file_set_name_wide(
			          file_io_handle,
			          filenames[ filename_index ],
			          string_length,
			          &error );
#else
			result = libbfio_file_set_name(
			          file_io_handle,
			          filenames[ filename_index ],
			          string_length,
			          &error );
#endif
			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 1 );

			EWF_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			result = libbfio_pool_set_handle(
			          file_io_pool,
			          filename_index,
			          file_io_handle,
			          LIBBFIO_OPEN_READ,
			          &error );

			EWF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 1 );

		        EWF_TEST_ASSERT_IS_NULL(
		         "error",
		         error );

			file_io_handle = NULL;
		}
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

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_open_file_io_pool",
		 ewf_test_handle_open_file_io_pool,
		 source );

		EWF_TEST_RUN(
		 "libewf_handle_close",
		 ewf_test_handle_close );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_open_close",
		 ewf_test_handle_open_close,
		 source );

		/* Initialize handle for tests
		 */
		result = ewf_test_handle_open_source(
		          &handle,
		          file_io_pool,
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

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_signal_abort",
		 ewf_test_handle_signal_abort,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_read_buffer",
		 ewf_test_handle_read_buffer,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_read_buffer_at_offset",
		 ewf_test_handle_read_buffer_at_offset,
		 handle );

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

		/* TODO: add tests for libewf_handle_get_filename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_file_io_handle",
		 ewf_test_handle_get_file_io_handle,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_root_file_entry",
		 ewf_test_handle_get_root_file_entry,
		 handle );

		/* TODO: add tests for libewf_handle_get_file_entry_by_utf8_path */

		/* TODO: add tests for libewf_handle_get_file_entry_by_utf16_path */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_sectors_per_chunk",
		 ewf_test_handle_get_sectors_per_chunk,
		 handle );

		/* TODO: add tests for libewf_handle_set_sectors_per_chunk */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_bytes_per_sector",
		 ewf_test_handle_get_bytes_per_sector,
		 handle );

		/* TODO: add tests for libewf_handle_set_bytes_per_sector */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_sectors",
		 ewf_test_handle_get_number_of_sectors,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_chunk_size",
		 ewf_test_handle_get_chunk_size,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_error_granularity",
		 ewf_test_handle_get_error_granularity,
		 handle );

		/* TODO: add tests for libewf_handle_set_error_granularity */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_compression_method",
		 ewf_test_handle_get_compression_method,
		 handle );

		/* TODO: add tests for libewf_handle_set_compression_method */

		/* TODO: add tests for libewf_handle_get_compression_values */

		/* TODO: add tests for libewf_handle_set_compression_values */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_media_size",
		 ewf_test_handle_get_media_size,
		 handle );

		/* TODO: add tests for libewf_handle_set_media_size */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_media_type",
		 ewf_test_handle_get_media_type,
		 handle );

		/* TODO: add tests for libewf_handle_set_media_type */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_media_flags",
		 ewf_test_handle_get_media_flags,
		 handle );

		/* TODO: add tests for libewf_handle_set_media_flags */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_format",
		 ewf_test_handle_get_format,
		 handle );

		/* TODO: add tests for libewf_handle_set_format */

		/* TODO: add tests for libewf_handle_get_segment_file_version */

		/* TODO: add tests for libewf_handle_get_segment_file_set_identifier */

		/* TODO: add tests for libewf_handle_set_segment_file_set_identifier */

		/* TODO: add tests for libewf_handle_get_md5_hash */

		/* TODO: add tests for libewf_handle_set_md5_hash */

		/* TODO: add tests for libewf_handle_get_sha1_hash */

		/* TODO: add tests for libewf_handle_set_sha1_hash */

		/* TODO: add tests for libewf_handle_set_read_zero_chunk_on_error */

		/* TODO: add tests for libewf_handle_copy_media_values */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_acquiry_errors",
		 ewf_test_handle_get_number_of_acquiry_errors,
		 handle );

		/* TODO: add tests for libewf_handle_get_acquiry_error */

		/* TODO: add tests for libewf_handle_append_acquiry_error */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_checksum_errors",
		 ewf_test_handle_get_number_of_checksum_errors,
		 handle );

		/* TODO: add tests for libewf_handle_get_checksum_error */

		/* TODO: add tests for libewf_handle_append_checksum_error */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_sessions",
		 ewf_test_handle_get_number_of_sessions,
		 handle );

		/* TODO: add tests for libewf_handle_get_session */

		/* TODO: add tests for libewf_handle_append_session */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_tracks",
		 ewf_test_handle_get_number_of_tracks,
		 handle );

		/* TODO: add tests for libewf_handle_get_track */

		/* TODO: add tests for libewf_handle_append_track */

		/* TODO: add tests for libewf_handle_get_header_codepage */

		/* TODO: add tests for libewf_handle_set_header_codepage */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_header_values_date_format",
		 ewf_test_handle_get_header_values_date_format,
		 handle );

		/* TODO: add tests for libewf_handle_set_header_values_date_format */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_header_values",
		 ewf_test_handle_get_number_of_header_values,
		 handle );

		/* TODO: add tests for libewf_handle_get_header_value_identifier_size */

		/* TODO: add tests for libewf_handle_get_header_value_identifier */

		/* TODO: add tests for libewf_handle_get_utf8_header_value_size */

		/* TODO: add tests for libewf_handle_get_utf8_header_value */

		/* TODO: add tests for libewf_handle_set_utf8_header_value */

		/* TODO: add tests for libewf_handle_get_utf16_header_value_size */

		/* TODO: add tests for libewf_handle_get_utf16_header_value */

		/* TODO: add tests for libewf_handle_set_utf16_header_value */

		/* TODO: add tests for libewf_handle_copy_header_values */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_hash_values",
		 ewf_test_handle_get_number_of_hash_values,
		 handle );

		/* TODO: add tests for libewf_handle_get_hash_value_identifier_size */

		/* TODO: add tests for libewf_handle_get_hash_value_identifier */

		/* TODO: add tests for libewf_handle_get_utf8_hash_value_size */

		/* TODO: add tests for libewf_handle_get_utf8_hash_value */

		/* TODO: add tests for libewf_handle_set_utf8_hash_value */

		/* TODO: add tests for libewf_handle_get_utf16_hash_value_size */

		/* TODO: add tests for libewf_handle_get_utf16_hash_value */

		/* TODO: add tests for libewf_handle_set_utf16_hash_value */

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

		result = libbfio_pool_free(
		          &file_io_pool,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
	         "file_io_pool",
	         file_io_pool );

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

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

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
		libewf_handle_free(
		 &handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
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
	return( EXIT_FAILURE );
}


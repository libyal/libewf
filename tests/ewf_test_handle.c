/*
 * Library handle type testing program
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
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"

/* Define to make ewf_test_handle generate verbose output
#define EWF_TEST_HANDLE_VERBOSE
 */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int ewf_test_handle_open_source(
     libewf_handle_t **handle,
     const libcstring_system_character_t *source,
     libcerror_error_t **error )
{
	libcstring_system_character_t **filenames = NULL;
	static char *function                     = "ewf_test_handle_open_source";
	size_t source_length                      = 0;
	int number_of_filenames                   = 0;
	int result                                = 0;

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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	source_length = libcstring_wide_string_length(
	                 source );
#else
	source_length = libcstring_narrow_string_length(
	                 source );
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "%s: unable to initialize input handle.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		libewf_handle_close(
		 *handle,
		 NULL );
		libewf_handle_free(
		 handle,
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
	libcerror_error_t *error = NULL;
	libewf_handle_t *handle  = NULL;
	int result               = 0;

	/* Test libewf_handle_initialize
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

	/* Test libewf_handle_initialize with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_handle_initialize(
	          &handle,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;
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
	/* Test libewf_handle_initialize with memset failing
	 */
	ewf_test_memset_attempts_before_fail = 0;

	result = libewf_handle_initialize(
	          &handle,
	          &error );

	if( ewf_test_memset_attempts_before_fail != -1 )
	{
		ewf_test_memset_attempts_before_fail = -1;
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
     const libcstring_system_character_t *source )
{
	libcerror_error_t *error                  = NULL;
	libcstring_system_character_t **filenames = NULL;
	libewf_handle_t *handle                   = NULL;
	size_t source_length                      = 0;
	int number_of_filenames                   = 0;
	int result                                = 0;

	/* Initialize test
	 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	source_length = libcstring_wide_string_length(
	                 source );
#else
	source_length = libcstring_narrow_string_length(
	                 source );
#endif

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
	return( 0 );
}

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )

/* TODO split open and open_wide */

#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

/* Tests the libewf_handle_get_maximum_segment_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_maximum_segment_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	size64_t maximum_segment_size = 0;
	int result                    = 0;

	result = libewf_handle_get_maximum_segment_size(
	          handle,
	          &maximum_segment_size,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_filename_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_filename_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size_t filename_size     = 0;
	int result               = 0;

	result = libewf_handle_get_filename_size(
	          handle,
	          &filename_size,
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

/* Tests the libewf_handle_get_filename_size_wide functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_filename_size_wide(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size_t filename_size     = 0;
	int result               = 0;

	result = libewf_handle_get_filename_size_wide(
	          handle,
	          &filename_size,
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
	result = libewf_handle_get_filename_size_wide(
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

/* Tests the libewf_handle_get_sectors_per_chunk functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_sectors_per_chunk(
     libewf_handle_t *handle )
{
	libcerror_error_t *error   = NULL;
	uint32_t sectors_per_chunk = 0;
	int result                 = 0;

	result = libewf_handle_get_sectors_per_chunk(
	          handle,
	          &sectors_per_chunk,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_bytes_per_sector functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_bytes_per_sector(
     libewf_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	uint32_t bytes_per_sector = 0;
	int result                = 0;

	result = libewf_handle_get_bytes_per_sector(
	          handle,
	          &bytes_per_sector,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_number_of_sectors functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_sectors(
     libewf_handle_t *handle )
{
	libcerror_error_t *error   = NULL;
	uint64_t number_of_sectors = 0;
	int result                 = 0;

	result = libewf_handle_get_number_of_sectors(
	          handle,
	          &number_of_sectors,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_chunk_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_chunk_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size32_t chunk_size      = 0;
	int result               = 0;

	result = libewf_handle_get_chunk_size(
	          handle,
	          &chunk_size,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_error_granularity functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_error_granularity(
     libewf_handle_t *handle )
{
	libcerror_error_t *error   = NULL;
	uint32_t error_granularity = 0;
	int result                 = 0;

	result = libewf_handle_get_error_granularity(
	          handle,
	          &error_granularity,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_compression_method functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_compression_method(
     libewf_handle_t *handle )
{
	libcerror_error_t *error    = NULL;
	uint16_t compression_method = 0;
	int result                  = 0;

	result = libewf_handle_get_compression_method(
	          handle,
	          &compression_method,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_compression_values functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_compression_values(
     libewf_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	int8_t compression_level  = 0;
	uint8_t compression_flags = 0;
	int result                = 0;

	result = libewf_handle_get_compression_values(
	          handle,
	          &compression_level,
	          &compression_flags,
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
	result = libewf_handle_get_compression_values(
	          NULL,
	          &compression_level,
	          &compression_flags,
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

	result = libewf_handle_get_compression_values(
	          handle,
	          NULL,
	          &compression_flags,
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

	result = libewf_handle_get_compression_values(
	          handle,
	          &compression_level,
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

/* Tests the libewf_handle_get_media_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_media_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size64_t media_size      = 0;
	int result               = 0;

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_media_type functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_media_type(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t media_type       = 0;
	int result               = 0;

	result = libewf_handle_get_media_type(
	          handle,
	          &media_type,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_media_flags functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_media_flags(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t media_flags      = 0;
	int result               = 0;

	result = libewf_handle_get_media_flags(
	          handle,
	          &media_flags,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_format functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_format(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t format           = 0;
	int result               = 0;

	result = libewf_handle_get_format(
	          handle,
	          &format,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_segment_file_version functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_segment_file_version(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	uint8_t major_version    = 0;
	uint8_t minor_version    = 0;
	int result               = 0;

	result = libewf_handle_get_segment_file_version(
	          handle,
	          &major_version,
	          &minor_version,
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
	result = libewf_handle_get_segment_file_version(
	          NULL,
	          &major_version,
	          &minor_version,
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

	result = libewf_handle_get_segment_file_version(
	          handle,
	          NULL,
	          &minor_version,
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

	result = libewf_handle_get_segment_file_version(
	          handle,
	          &major_version,
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

/* Tests the libewf_handle_get_number_of_acquiry_errors functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_acquiry_errors(
     libewf_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	uint32_t number_of_errors = 0;
	int result                = 0;

	result = libewf_handle_get_number_of_acquiry_errors(
	          handle,
	          &number_of_errors,
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
	result = libewf_handle_get_number_of_acquiry_errors(
	          NULL,
	          &number_of_errors,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_acquiry_error functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_acquiry_error(
     libewf_handle_t *handle )
{
	libcerror_error_t *error   = NULL;
	uint64_t start_sector      = 0;
	uint64_t number_of_sectors = 0;
	uint32_t number_of_errors  = 0;
	int result                 = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_acquiry_errors(
	          handle,
	          &number_of_errors,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_errors < 1 )
	{
		return( 1 );
	}
	/* Test retrieving acquiry errors
	 */
	result = libewf_handle_get_acquiry_error(
	          handle,
	          0,
	          &start_sector,
	          &number_of_sectors,
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
	result = libewf_handle_get_acquiry_error(
	          NULL,
	          0,
	          &start_sector,
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

/* TODO test index */
	result = libewf_handle_get_acquiry_error(
	          handle,
	          0,
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

	result = libewf_handle_get_acquiry_error(
	          handle,
	          0,
	          &start_sector,
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

/* Tests the libewf_handle_get_number_of_checksum_errors functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_checksum_errors(
     libewf_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	uint32_t number_of_errors = 0;
	int result                = 0;

	result = libewf_handle_get_number_of_checksum_errors(
	          handle,
	          &number_of_errors,
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
	result = libewf_handle_get_number_of_checksum_errors(
	          NULL,
	          &number_of_errors,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_checksum_error functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_checksum_error(
     libewf_handle_t *handle )
{
	libcerror_error_t *error   = NULL;
	uint64_t start_sector      = 0;
	uint64_t number_of_sectors = 0;
	uint32_t number_of_errors  = 0;
	int result                 = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_checksum_errors(
	          handle,
	          &number_of_errors,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_errors < 1 )
	{
		return( 1 );
	}
	/* Test retrieving checksum errors
	 */
	result = libewf_handle_get_checksum_error(
	          handle,
	          0,
	          &start_sector,
	          &number_of_sectors,
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
	result = libewf_handle_get_checksum_error(
	          NULL,
	          0,
	          &start_sector,
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

/* TODO test index */
	result = libewf_handle_get_checksum_error(
	          handle,
	          0,
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

	result = libewf_handle_get_checksum_error(
	          handle,
	          0,
	          &start_sector,
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

/* Tests the libewf_handle_get_number_of_sessions functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_sessions(
     libewf_handle_t *handle )
{
	libcerror_error_t *error    = NULL;
	uint32_t number_of_sessions = 0;
	int result                  = 0;

	result = libewf_handle_get_number_of_sessions(
	          handle,
	          &number_of_sessions,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_number_of_tracks functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_tracks(
     libewf_handle_t *handle )
{
	libcerror_error_t *error  = NULL;
	uint32_t number_of_tracks = 0;
	int result                = 0;

	result = libewf_handle_get_number_of_tracks(
	          handle,
	          &number_of_tracks,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_header_codepage functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_header_codepage(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libewf_handle_get_header_codepage(
	          handle,
	          &codepage,
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
	result = libewf_handle_get_header_codepage(
	          NULL,
	          &codepage,
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

	result = libewf_handle_get_header_codepage(
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_header_values_date_format functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_header_values_date_format(
     libewf_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int date_format          = 0;
	int result               = 0;

	result = libewf_handle_get_header_values_date_format(
	          handle,
	          &date_format,
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
	result = libewf_handle_get_header_values_date_format(
	          NULL,
	          &date_format,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_number_of_header_values functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_header_values(
     libewf_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_header_value_identifier_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_header_value_identifier_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error         = NULL;
	size_t identifier_size           = 0;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_header_values <= 0 )
	{
		return( 1 );
	}
	/* Tests retrieving header value identifier size
	 */
	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
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
	result = libewf_handle_get_header_value_identifier_size(
	          NULL,
	          0,
	          &identifier_size,
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

	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          (uint32_t) -1,
	          &identifier_size,
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

	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
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

/* Tests the libewf_handle_get_header_value_identifier functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_header_value_identifier(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];

	libcerror_error_t *error         = NULL;
	size_t identifier_size           = 0;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_header_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	/* Test retrieving header values
	 */
	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
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
	result = libewf_handle_get_header_value_identifier(
	          NULL,
	          0,
	          identifier,
	          128,
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

	result = libewf_handle_get_header_value_identifier(
	          handle,
	          (uint32_t) -1,
	          identifier,
	          128,
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

	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          NULL,
	          128,
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

	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          1,
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

	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libewf_handle_get_utf8_header_value_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf8_header_value_size(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];

	libcerror_error_t *error         = NULL;
	size_t identifier_size           = 0;
	size_t utf8_value_size           = 0;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_header_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test retrieving an UTF-8 header value size
	 */
	result = libewf_handle_get_utf8_header_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf8_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libewf_handle_get_utf8_header_value_size(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          &utf8_value_size,
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

	result = libewf_handle_get_utf8_header_value_size(
	          handle,
	          NULL,
	          identifier_size - 1,
	          &utf8_value_size,
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

	result = libewf_handle_get_utf8_header_value_size(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          &utf8_value_size,
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

	result = libewf_handle_get_utf8_header_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
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

/* Tests the libewf_handle_get_utf8_header_value functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf8_header_value(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];
	uint8_t utf8_value[ 128 ];

	libcerror_error_t *error         = NULL;
	size_t identifier_size           = 0;
	size_t utf8_value_size           = 0;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_header_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_get_utf8_header_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf8_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( utf8_value_size <= 1 )
	 || ( utf8_value_size > 128 ) )
	{
		return( 1 );
	}
	/* Test retrieving an UTF-8 header value
	 */
	result = libewf_handle_get_utf8_header_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf8_value,
	          utf8_value_size,
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
	result = libewf_handle_get_utf8_header_value(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          utf8_value,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_header_value(
	          handle,
	          NULL,
	          identifier_size - 1,
	          utf8_value,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_header_value(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          utf8_value,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_header_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          NULL,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_header_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf8_value,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libewf_handle_get_utf16_header_value_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf16_header_value_size(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];

	libcerror_error_t *error         = NULL;
	size_t identifier_size           = 0;
	size_t utf16_value_size          = 0;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_header_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test retrieving an UTF-16 header value size
	 */
	result = libewf_handle_get_utf16_header_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf16_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libewf_handle_get_utf16_header_value_size(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          &utf16_value_size,
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

	result = libewf_handle_get_utf16_header_value_size(
	          handle,
	          NULL,
	          identifier_size - 1,
	          &utf16_value_size,
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

	result = libewf_handle_get_utf16_header_value_size(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          &utf16_value_size,
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

	result = libewf_handle_get_utf16_header_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
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

/* Tests the libewf_handle_get_utf16_header_value functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf16_header_value(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];
	uint16_t utf16_value[ 128 ];

	libcerror_error_t *error         = NULL;
	size_t identifier_size           = 0;
	size_t utf16_value_size          = 0;
	uint32_t number_of_header_values = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_header_values(
	          handle,
	          &number_of_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_header_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_header_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_get_utf16_header_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf16_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( ( utf16_value_size <= 1 )
	 || ( utf16_value_size > 128 ) )
	{
		return( 1 );
	}
	/* Test retrieving an UTF-16 header value
	 */
	result = libewf_handle_get_utf16_header_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf16_value,
	          utf16_value_size,
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
	result = libewf_handle_get_utf16_header_value(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          utf16_value,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_header_value(
	          handle,
	          NULL,
	          identifier_size - 1,
	          utf16_value,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_header_value(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          utf16_value,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_header_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          NULL,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_header_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf16_value,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libewf_handle_get_number_of_hash_values functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_number_of_hash_values(
     libewf_handle_t *handle )
{
	libcerror_error_t *error       = NULL;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_handle_get_hash_value_identifier_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_hash_value_identifier_size(
     libewf_handle_t *handle )
{
	libcerror_error_t *error       = NULL;
	size_t identifier_size         = 0;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_hash_values <= 0 )
	{
		return( 1 );
	}
	/* Tests retrieving hash value identifier size
	 */
	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
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
	result = libewf_handle_get_hash_value_identifier_size(
	          NULL,
	          0,
	          &identifier_size,
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

	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          (uint32_t) -1,
	          &identifier_size,
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

	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
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

/* Tests the libewf_handle_get_hash_value_identifier functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_hash_value_identifier(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];

	libcerror_error_t *error       = NULL;
	size_t identifier_size         = 0;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_hash_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	/* Test retrieving hash values
	 */
	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
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
	result = libewf_handle_get_hash_value_identifier(
	          NULL,
	          0,
	          identifier,
	          128,
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

	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          (uint32_t) -1,
	          identifier,
	          128,
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

	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          NULL,
	          128,
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

	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          1,
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

	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libewf_handle_get_utf8_hash_value_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf8_hash_value_size(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];

	libcerror_error_t *error       = NULL;
	size_t identifier_size         = 0;
	size_t utf8_value_size         = 0;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_hash_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test retrieving an UTF-8 hash value size
	 */
	result = libewf_handle_get_utf8_hash_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf8_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libewf_handle_get_utf8_hash_value_size(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          &utf8_value_size,
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

	result = libewf_handle_get_utf8_hash_value_size(
	          handle,
	          NULL,
	          identifier_size - 1,
	          &utf8_value_size,
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

	result = libewf_handle_get_utf8_hash_value_size(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          &utf8_value_size,
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

	result = libewf_handle_get_utf8_hash_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
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

/* Tests the libewf_handle_get_utf8_hash_value functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf8_hash_value(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];
	uint8_t utf8_value[ 128 ];

	libcerror_error_t *error       = NULL;
	size_t identifier_size         = 0;
	size_t utf8_value_size         = 0;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_hash_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_get_utf8_hash_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf8_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( utf8_value_size <= 1 )
	 || ( utf8_value_size > 128 ) )
	{
		return( 1 );
	}
	/* Test retrieving an UTF-8 hash value
	 */
	result = libewf_handle_get_utf8_hash_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf8_value,
	          utf8_value_size,
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
	result = libewf_handle_get_utf8_hash_value(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          utf8_value,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_hash_value(
	          handle,
	          NULL,
	          identifier_size - 1,
	          utf8_value,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_hash_value(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          utf8_value,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_hash_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          NULL,
	          utf8_value_size,
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

	result = libewf_handle_get_utf8_hash_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf8_value,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libewf_handle_get_utf16_hash_value_size functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf16_hash_value_size(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];

	libcerror_error_t *error       = NULL;
	size_t identifier_size         = 0;
	size_t utf16_value_size        = 0;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_hash_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test retrieving an UTF-16 hash value size
	 */
	result = libewf_handle_get_utf16_hash_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf16_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libewf_handle_get_utf16_hash_value_size(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          &utf16_value_size,
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

	result = libewf_handle_get_utf16_hash_value_size(
	          handle,
	          NULL,
	          identifier_size - 1,
	          &utf16_value_size,
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

	result = libewf_handle_get_utf16_hash_value_size(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          &utf16_value_size,
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

	result = libewf_handle_get_utf16_hash_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
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

/* Tests the libewf_handle_get_utf16_hash_value functions
 * Returns 1 if successful or 0 if not
 */
int ewf_test_handle_get_utf16_hash_value(
     libewf_handle_t *handle )
{
	uint8_t identifier[ 128 ];
	uint16_t utf16_value[ 128 ];

	libcerror_error_t *error       = NULL;
	size_t identifier_size         = 0;
	size_t utf16_value_size        = 0;
	uint32_t number_of_hash_values = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libewf_handle_get_number_of_hash_values(
	          handle,
	          &number_of_hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( number_of_hash_values <= 0 )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier_size(
	          handle,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	if( ( identifier_size <= 1 )
	 || ( identifier_size > 128 ) )
	{
		return( 1 );
	}
	result = libewf_handle_get_hash_value_identifier(
	          handle,
	          0,
	          identifier,
	          128,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EWF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libewf_handle_get_utf16_hash_value_size(
	          handle,
	          identifier,
	          identifier_size - 1,
	          &utf16_value_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( ( utf16_value_size <= 1 )
	 || ( utf16_value_size > 128 ) )
	{
		return( 1 );
	}
	/* Test retrieving an UTF-16 hash value
	 */
	result = libewf_handle_get_utf16_hash_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf16_value,
	          utf16_value_size,
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
	result = libewf_handle_get_utf16_hash_value(
	          NULL,
	          identifier,
	          identifier_size - 1,
	          utf16_value,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_hash_value(
	          handle,
	          NULL,
	          identifier_size - 1,
	          utf16_value,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_hash_value(
	          handle,
	          identifier,
	          (size_t) SSIZE_MAX + 1,
	          utf16_value,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_hash_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          NULL,
	          utf16_value_size,
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

	result = libewf_handle_get_utf16_hash_value(
	          handle,
	          identifier,
	          identifier_size - 1,
	          utf16_value,
	          (size_t) SSIZE_MAX + 1,
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

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libcerror_error_t *error              = NULL;
	libcstring_system_character_t *source = NULL;
	libewf_handle_t *handle               = NULL;
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
	 ewf_test_handle_initialize() )

	EWF_TEST_RUN(
	 "libewf_handle_free",
	 ewf_test_handle_free() )

	/* TODO add test for libewf_handle_clone */
	/* TODO add test for libewf_handle_signal_abort */

	/* TODO seperate read/write and file entry functions */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_open",
		 ewf_test_handle_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
		/* TODO add test for libewf_handle_open_wide */
#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		/* TODO add test for libewf_handle_open_file_io_pool */
		/* TODO add test for libewf_handle_close */

		/* TODO add test for libewf_handle_read_buffer */
		/* TODO add test for libewf_handle_write_buffer_at_offset */

		/* TODO add test for libewf_handle_get_data_chunk */
		/* TODO add test for libewf_handle_write_data_chunk */
		/* TODO add test for libewf_handle_write_finalize */

		/* TODO add test for libewf_handle_seek_offset */
		/* TODO add test for libewf_handle_get_offset */

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

		/* Run tests with handle
		 */
		/* TODO add test for libewf_handle_set_maximum_number_of_open_handles */
		/* TODO add test for libewf_handle_segment_files_corrupted */
		/* TODO add test for libewf_handle_segment_files_encrypted */

		/* TODO add test for libewf_handle_get_segment_filename_size */
		/* TODO add test for libewf_handle_get_segment_filename */
		/* TODO add test for libewf_handle_set_segment_filename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )
		/* TODO add test for libewf_handle_get_segment_filename_size_wide */
		/* TODO add test for libewf_handle_get_segment_filename_wide */
		/* TODO add test for libewf_handle_set_segment_filename_wide */
#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_maximum_segment_size",
		 ewf_test_handle_get_maximum_segment_size,
		 handle );

		/* TODO add test for libewf_handle_set_maximum_segment_size */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_filename_size",
		 ewf_test_handle_get_filename_size,
		 handle );

		/* TODO add test for libewf_handle_get_filename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )
		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_filename_size_wide",
		 ewf_test_handle_get_filename_size_wide,
		 handle );

		/* TODO add test for libewf_handle_get_filename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		/* TODO add test for libewf_handle_get_file_io_handle */

		/* TODO add test for libewf_handle_get_root_file_entry */
		/* TODO add test for libewf_handle_get_file_entry_by_utf8_path */
		/* TODO add test for libewf_handle_get_file_entry_by_utf16_path */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_sectors_per_chunk",
		 ewf_test_handle_get_sectors_per_chunk,
		 handle );

		/* TODO add test for libewf_handle_set_bytes_per_sector */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_bytes_per_sector",
		 ewf_test_handle_get_bytes_per_sector,
		 handle );

		/* TODO add test for libewf_handle_set_bytes_per_sector */

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

		/* TODO add test for libewf_handle_set_error_granularity */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_compression_method",
		 ewf_test_handle_get_compression_method,
		 handle );

		/* TODO add test for libewf_handle_set_compression_method */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_compression_values",
		 ewf_test_handle_get_compression_values,
		 handle );

		/* TODO add test for libewf_handle_set_compression_values */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_media_size",
		 ewf_test_handle_get_media_size,
		 handle );

		/* TODO add test for libewf_handle_set_media_size */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_media_type",
		 ewf_test_handle_get_media_type,
		 handle );

		/* TODO add test for libewf_handle_set_media_type */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_media_flags",
		 ewf_test_handle_get_media_flags,
		 handle );

		/* TODO add test for libewf_handle_set_media_flags */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_format",
		 ewf_test_handle_get_format,
		 handle );

		/* TODO add test for libewf_handle_set_format */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_segment_file_version",
		 ewf_test_handle_get_segment_file_version,
		 handle );

		/* TODO add test for libewf_handle_get_segment_file_set_identifier */
		/* TODO add test for libewf_handle_set_segment_file_set_identifier */

		/* TODO add test for libewf_handle_get_md5_hash */
		/* TODO add test for libewf_handle_set_md5_hash */
		/* TODO add test for libewf_handle_get_sha1_hash */
		/* TODO add test for libewf_handle_set_sha1_hash */

		/* TODO add test for libewf_handle_set_read_zero_chunk_on_error */

		/* TODO add test for libewf_handle_copy_media_values */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_acquiry_errors",
		 ewf_test_handle_get_number_of_acquiry_errors,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_acquiry_error",
		 ewf_test_handle_get_acquiry_error,
		 handle );

		/* TODO add test for libewf_handle_append_acquiry_error */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_checksum_errors",
		 ewf_test_handle_get_number_of_checksum_errors,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_checksum_error",
		 ewf_test_handle_get_checksum_error,
		 handle );

		/* TODO add test for libewf_handle_append_checksum_error */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_sessions",
		 ewf_test_handle_get_number_of_sessions,
		 handle );

		/* TODO add test for libewf_handle_get_session */
		/* TODO add test for libewf_handle_append_session */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_tracks",
		 ewf_test_handle_get_number_of_tracks,
		 handle );

		/* TODO add test for libewf_handle_get_track */
		/* TODO add test for libewf_handle_append_track */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_header_codepage",
		 ewf_test_handle_get_header_codepage,
		 handle );

		/* TODO add test for libewf_handle_set_header_codepage */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_header_values_date_format",
		 ewf_test_handle_get_header_values_date_format,
		 handle );

		/* TODO add test for libewf_handle_set_header_values_date_format */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_header_values",
		 ewf_test_handle_get_number_of_header_values,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_header_value_identifier_size",
		 ewf_test_handle_get_header_value_identifier_size,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_header_value_identifier",
		 ewf_test_handle_get_header_value_identifier,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf8_header_value_size",
		 ewf_test_handle_get_utf8_header_value_size,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf8_header_value",
		 ewf_test_handle_get_utf8_header_value,
		 handle );

		/* TODO add test for libewf_handle_set_utf8_header_value */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf16_header_value_size",
		 ewf_test_handle_get_utf16_header_value_size,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf16_header_value",
		 ewf_test_handle_get_utf16_header_value,
		 handle );

		/* TODO add test for libewf_handle_set_utf16_header_value */

		/* TODO add test for libewf_handle_copy_header_values */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_number_of_hash_values",
		 ewf_test_handle_get_number_of_hash_values,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_hash_value_identifier_size",
		 ewf_test_handle_get_hash_value_identifier_size,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_hash_value_identifier",
		 ewf_test_handle_get_hash_value_identifier,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf8_hash_value_size",
		 ewf_test_handle_get_utf8_hash_value_size,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf8_hash_value",
		 ewf_test_handle_get_utf8_hash_value,
		 handle );

		/* TODO add test for libewf_handle_set_utf8_hash_value */

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf16_hash_value_size",
		 ewf_test_handle_get_utf16_hash_value_size,
		 handle );

		EWF_TEST_RUN_WITH_ARGS(
		 "libewf_handle_get_utf16_hash_value",
		 ewf_test_handle_get_utf16_hash_value,
		 handle );

		/* TODO add test for libewf_handle_set_utf16_hash_value */

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


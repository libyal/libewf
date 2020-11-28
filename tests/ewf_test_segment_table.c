/*
 * Library segment_table type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_io_handle.h"
#include "../libewf/libewf_segment_table.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_segment_table_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_initialize(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_io_handle_t *io_handle         = NULL;
	libewf_segment_table_t *segment_table = NULL;
	int result                            = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests       = 3;
	int number_of_memset_fail_tests       = 1;
	int test_number                       = 0;
#endif

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_segment_table_initialize(
	          &segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_free(
	          &segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_segment_table_initialize(
	          NULL,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
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

	segment_table = (libewf_segment_table_t *) 0x12345678UL;

	result = libewf_segment_table_initialize(
	          &segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	segment_table = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_segment_table_initialize(
	          &segment_table,
	          NULL,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_segment_table_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_segment_table_initialize(
		          &segment_table,
		          io_handle,
		          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( segment_table != NULL )
			{
				libewf_segment_table_free(
				 &segment_table,
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
			 "segment_table",
			 segment_table );

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
		/* Test libewf_segment_table_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_segment_table_initialize(
		          &segment_table,
		          io_handle,
		          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( segment_table != NULL )
			{
				libewf_segment_table_free(
				 &segment_table,
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
			 "segment_table",
			 segment_table );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( segment_table != NULL )
	{
		libewf_segment_table_free(
		 &segment_table,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_segment_table_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_segment_table_free(
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

/* Tests the libewf_segment_table_clear function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_clear(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_segment_table_clear(
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

/* Tests the libewf_segment_table_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_clone(
     void )
{
	libcerror_error_t *error                          = NULL;
	libewf_io_handle_t *io_handle                     = NULL;
	libewf_segment_table_t *destination_segment_table = NULL;
	libewf_segment_table_t *source_segment_table      = NULL;
	int result                                        = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                   = 3;
	int number_of_memset_fail_tests                   = 1;
	int test_number                                   = 0;
#endif

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_initialize(
	          &source_segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_segment_table",
	 source_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_segment_table_clone(
	          &destination_segment_table,
	          source_segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_segment_table",
	 destination_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_free(
	          &destination_segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_segment_table",
	 destination_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_clone(
	          &destination_segment_table,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_segment_table",
	 destination_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_segment_table_clone(
	          NULL,
	          source_segment_table,
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

	destination_segment_table = (libewf_segment_table_t *) 0x12345678UL;

	result = libewf_segment_table_clone(
	          &destination_segment_table,
	          source_segment_table,
	          &error );

	destination_segment_table = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_segment_table_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_segment_table_clone(
		          &destination_segment_table,
		          source_segment_table,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_segment_table != NULL )
			{
				libewf_segment_table_free(
				 &destination_segment_table,
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
			 "destination_segment_table",
			 destination_segment_table );

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
		/* Test libewf_segment_table_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_segment_table_clone(
		          &destination_segment_table,
		          source_segment_table,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( destination_segment_table != NULL )
			{
				libewf_segment_table_free(
				 &destination_segment_table,
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
			 "destination_segment_table",
			 destination_segment_table );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_segment_table_free(
	          &source_segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_segment_table",
	 source_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( destination_segment_table != NULL )
	{
		libewf_segment_table_free(
		 &destination_segment_table,
		 NULL );
	}
	if( source_segment_table != NULL )
	{
		libewf_segment_table_free(
		 &source_segment_table,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_segment_table_get_basename_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_basename_size(
     libewf_segment_table_t *segment_table )
{
	libcerror_error_t *error = NULL;
	size_t basename_size     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_segment_table_get_basename_size(
	          segment_table,
	          &basename_size,
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
	result = libewf_segment_table_get_basename_size(
	          NULL,
	          &basename_size,
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

	result = libewf_segment_table_get_basename_size(
	          segment_table,
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

/* Tests the libewf_segment_table_get_basename function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_basename(
     libewf_segment_table_t *segment_table )
{
	char basename[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_segment_table_get_basename(
	          segment_table,
	          basename,
	          32,
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
	result = libewf_segment_table_get_basename(
	          NULL,
	          basename,
	          32,
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

	result = libewf_segment_table_get_basename(
	          segment_table,
	          NULL,
	          32,
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

/* Tests the libewf_segment_table_set_basename function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_set_basename(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_io_handle_t *io_handle         = NULL;
	libewf_segment_table_t *segment_table = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_initialize(
	          &segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_segment_table_set_basename(
	          segment_table,
	          "test",
	          4,
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
	result = libewf_segment_table_set_basename(
	          NULL,
	          "test",
	          4,
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

	result = libewf_segment_table_set_basename(
	          segment_table,
	          NULL,
	          4,
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
	result = libewf_segment_table_free(
	          &segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( segment_table != NULL )
	{
		libewf_segment_table_free(
		 &segment_table,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libewf_segment_table_get_basename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_basename_size_wide(
     libewf_segment_table_t *segment_table )
{
	libcerror_error_t *error = NULL;
	size_t basename_size     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_segment_table_get_basename_size_wide(
	          segment_table,
	          &basename_size,
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
	result = libewf_segment_table_get_basename_size_wide(
	          NULL,
	          &basename_size,
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

	result = libewf_segment_table_get_basename_size_wide(
	          segment_table,
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

/* Tests the libewf_segment_table_get_basename_wide function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_basename_wide(
     libewf_segment_table_t *segment_table )
{
	wchar_t basename[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_segment_table_get_basename_wide(
	          segment_table,
	          basename,
	          32,
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
	result = libewf_segment_table_get_basename_wide(
	          NULL,
	          basename,
	          32,
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

	result = libewf_segment_table_get_basename_wide(
	          segment_table,
	          NULL,
	          32,
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

/* Tests the libewf_segment_table_set_basename_wide function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_set_basename_wide(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_io_handle_t *io_handle         = NULL;
	libewf_segment_table_t *segment_table = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_initialize(
	          &segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_segment_table_set_basename_wide(
	          segment_table,
	          L"test",
	          4,
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
	result = libewf_segment_table_set_basename_wide(
	          NULL,
	          L"test",
	          4,
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

	result = libewf_segment_table_set_basename_wide(
	          segment_table,
	          NULL,
	          4,
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
	result = libewf_segment_table_free(
	          &segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( segment_table != NULL )
	{
		libewf_segment_table_free(
		 &segment_table,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libewf_segment_table_set_maximum_segment_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_set_maximum_segment_size(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_io_handle_t *io_handle         = NULL;
	libewf_segment_table_t *segment_table = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_initialize(
	          &segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_segment_table_set_maximum_segment_size(
	          segment_table,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
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
	result = libewf_segment_table_set_maximum_segment_size(
	          NULL,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
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
	result = libewf_segment_table_free(
	          &segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( segment_table != NULL )
	{
		libewf_segment_table_free(
		 &segment_table,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_segment_table_get_number_of_segments function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_number_of_segments(
     libewf_segment_table_t *segment_table )
{
	libcerror_error_t *error    = NULL;
	uint32_t number_of_segments = 0;
	int result                  = 0;

	/* Test regular cases
	 */
	result = libewf_segment_table_get_number_of_segments(
	          segment_table,
	          &number_of_segments,
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
	result = libewf_segment_table_get_number_of_segments(
	          NULL,
	          &number_of_segments,
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

	result = libewf_segment_table_get_number_of_segments(
	          segment_table,
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

/* Tests the libewf_segment_table_get_segment_by_index function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_segment_by_index(
     libewf_segment_table_t *segment_table )
{
	libcerror_error_t *error   = NULL;
	size64_t segment_file_size = 0;
	int file_io_pool_entry     = 0;
	int result                 = 0;

	/* Test error cases
	 */
	result = libewf_segment_table_get_segment_by_index(
	          NULL,
	          0,
	          &file_io_pool_entry,
	          &segment_file_size,
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

	result = libewf_segment_table_get_segment_by_index(
	          segment_table,
	          0,
	          NULL,
	          &segment_file_size,
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

/* Tests the libewf_segment_table_get_segment_at_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_segment_at_offset(
     libewf_segment_table_t *segment_table )
{
	libcerror_error_t *error   = NULL;
	size64_t segment_file_size = 0;
	int file_io_pool_entry     = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libewf_segment_table_get_segment_at_offset(
	          segment_table,
	          0,
	          &file_io_pool_entry,
	          &segment_file_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_segment_table_get_segment_at_offset(
	          NULL,
	          0,
	          &file_io_pool_entry,
	          &segment_file_size,
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

/* Tests the libewf_segment_table_get_segment_storage_media_size_by_index function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_get_segment_storage_media_size_by_index(
     libewf_segment_table_t *segment_table )
{
	libcerror_error_t *error    = NULL;
	size64_t storage_media_size = 0;
	int result                  = 0;

	/* Test error cases
	 */
	result = libewf_segment_table_get_segment_storage_media_size_by_index(
	          NULL,
	          0,
	          &storage_media_size,
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

	result = libewf_segment_table_get_segment_storage_media_size_by_index(
	          segment_table,
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

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc EWF_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc EWF_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#endif
{
#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	libcerror_error_t *error              = NULL;
	libewf_io_handle_t *io_handle         = NULL;
	libewf_segment_table_t *segment_table = NULL;
	int result                            = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_segment_table_initialize",
	 ewf_test_segment_table_initialize );

	EWF_TEST_RUN(
	 "libewf_segment_table_free",
	 ewf_test_segment_table_free );

	EWF_TEST_RUN(
	 "libewf_segment_table_clear",
	 ewf_test_segment_table_clear );

	EWF_TEST_RUN(
	 "libewf_segment_table_clone",
	 ewf_test_segment_table_clone );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_initialize(
	          &segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_set_basename(
	          segment_table,
	          "test",
	          4,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_basename_size",
	 ewf_test_segment_table_get_basename_size,
	 segment_table );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_basename",
	 ewf_test_segment_table_get_basename,
	 segment_table );

	EWF_TEST_RUN(
	 "libewf_segment_table_set_basename",
	 ewf_test_segment_table_set_basename );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_basename_size_wide",
	 ewf_test_segment_table_get_basename_size_wide,
	 segment_table );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_basename_wide",
	 ewf_test_segment_table_get_basename_wide,
	 segment_table );

	EWF_TEST_RUN(
	 "libewf_segment_table_set_basename_wide",
	 ewf_test_segment_table_set_basename_wide );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

	EWF_TEST_RUN(
	 "libewf_segment_table_set_maximum_segment_size",
	 ewf_test_segment_table_set_maximum_segment_size );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_number_of_segments",
	 ewf_test_segment_table_get_number_of_segments,
	 segment_table );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_segment_by_index",
	 ewf_test_segment_table_get_segment_by_index,
	 segment_table );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_segment_at_offset",
	 ewf_test_segment_table_get_segment_at_offset,
	 segment_table );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_segment_table_get_segment_storage_media_size_by_index",
	 ewf_test_segment_table_get_segment_storage_media_size_by_index,
	 segment_table );

	/* TODO: add tests for libewf_segment_table_set_segment_storage_media_size_by_index */

	/* TODO: add tests for libewf_segment_table_get_segment_file_by_index */

	/* TODO: add tests for libewf_segment_table_get_segment_file_at_offset */

	/* TODO: add tests for libewf_segment_table_set_segment_file_by_index */

	/* TODO: add tests for libewf_segment_table_append_segment_by_segment_file */

	/* Clean up
	 */
	result = libewf_segment_table_free(
	          &segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "segment_table",
	 segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( segment_table != NULL )
	{
		libewf_segment_table_free(
		 &segment_table,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


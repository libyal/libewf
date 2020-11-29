/*
 * Library chunk_table type test program
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

#include "ewf_test_libcdata.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_chunk_table.h"
#include "../libewf/libewf_io_handle.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_chunk_table_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libewf_chunk_table_t *chunk_table = NULL;
	libewf_io_handle_t *io_handle     = NULL;
	int result                        = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 3;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
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
	result = libewf_chunk_table_initialize(
	          &chunk_table,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_table",
	 chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_table_free(
	          &chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_table",
	 chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_table_initialize(
	          NULL,
	          io_handle,
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

	chunk_table = (libewf_chunk_table_t *) 0x12345678UL;

	result = libewf_chunk_table_initialize(
	          &chunk_table,
	          io_handle,
	          &error );

	chunk_table = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_chunk_table_initialize(
	          &chunk_table,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_chunk_table_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_chunk_table_initialize(
		          &chunk_table,
		          io_handle,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( chunk_table != NULL )
			{
				libewf_chunk_table_free(
				 &chunk_table,
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
			 "chunk_table",
			 chunk_table );

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
		/* Test libewf_chunk_table_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_chunk_table_initialize(
		          &chunk_table,
		          io_handle,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( chunk_table != NULL )
			{
				libewf_chunk_table_free(
				 &chunk_table,
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
			 "chunk_table",
			 chunk_table );

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
	if( chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &chunk_table,
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

/* Tests the libewf_chunk_table_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_chunk_table_free(
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

/* Tests the libewf_chunk_table_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_clone(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_chunk_table_t *destination_chunk_table = NULL;
	libewf_chunk_table_t *source_chunk_table      = NULL;
	libewf_io_handle_t *io_handle                 = NULL;
	int result                                    = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 2;
	int test_number                               = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests               = 1;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

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

	result = libewf_chunk_table_initialize(
	          &source_chunk_table,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_chunk_table",
	 source_chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_chunk_table_clone(
	          &destination_chunk_table,
	          source_chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_chunk_table",
	 destination_chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_table_free(
	          &destination_chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_chunk_table",
	 destination_chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_table_clone(
	          &destination_chunk_table,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_chunk_table",
	 destination_chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_table_clone(
	          NULL,
	          source_chunk_table,
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

	destination_chunk_table = (libewf_chunk_table_t *) 0x12345678UL;

	result = libewf_chunk_table_clone(
	          &destination_chunk_table,
	          source_chunk_table,
	          &error );

	destination_chunk_table = NULL;

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
		/* Test libewf_chunk_table_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_chunk_table_clone(
		          &destination_chunk_table,
		          source_chunk_table,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_chunk_table != NULL )
			{
				libewf_chunk_table_free(
				 &destination_chunk_table,
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
			 "destination_chunk_table",
			 destination_chunk_table );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libewf_chunk_table_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_chunk_table_clone(
		          &destination_chunk_table,
		          source_chunk_table,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_chunk_table != NULL )
			{
				libewf_chunk_table_free(
				 &destination_chunk_table,
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
			 "destination_chunk_table",
			 destination_chunk_table );

			EWF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_chunk_table_free(
	          &source_chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_chunk_table",
	 source_chunk_table );

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
	if( destination_chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &destination_chunk_table,
		 NULL );
	}
	if( source_chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &source_chunk_table,
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

/* Tests the libewf_chunk_table_get_number_of_checksum_errors function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_get_number_of_checksum_errors(
     void )
{
	libcdata_range_list_t *checksum_errors = NULL;
	libcerror_error_t *error               = NULL;
	libewf_chunk_table_t *chunk_table      = NULL;
	libewf_io_handle_t *io_handle          = NULL;
	uint32_t number_of_checksum_errors     = 0;
	int result                             = 0;

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

	result = libewf_chunk_table_initialize(
	          &chunk_table,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_table",
	 chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_table_append_checksum_error(
	          chunk_table,
	          1024,
	          16,
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
	result = libewf_chunk_table_get_number_of_checksum_errors(
	          chunk_table,
	          &number_of_checksum_errors,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT32(
	 "number_of_checksum_errors",
	 number_of_checksum_errors,
	 (uint32_t) 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_table_get_number_of_checksum_errors(
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

	checksum_errors = chunk_table->checksum_errors;

	chunk_table->checksum_errors = NULL;

	result = libewf_chunk_table_get_number_of_checksum_errors(
	          chunk_table,
	          &number_of_checksum_errors,
	          &error );

	chunk_table->checksum_errors = checksum_errors;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_chunk_table_get_number_of_checksum_errors(
	          chunk_table,
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

	/* Clean up
	 */
	result = libewf_chunk_table_free(
	          &chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_table",
	 chunk_table );

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
	if( chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &chunk_table,
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

/* Tests the libewf_chunk_table_get_checksum_error function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_get_checksum_error(
     void )
{
	libcdata_range_list_t *checksum_errors = NULL;
	libcerror_error_t *error               = NULL;
	libewf_chunk_table_t *chunk_table      = NULL;
	libewf_io_handle_t *io_handle          = NULL;
	uint64_t number_of_sectors             = 0;
	uint64_t start_sector                  = 0;
	int result                             = 0;

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

	result = libewf_chunk_table_initialize(
	          &chunk_table,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_table",
	 chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_table_append_checksum_error(
	          chunk_table,
	          1024,
	          16,
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
	result = libewf_chunk_table_get_checksum_error(
	          chunk_table,
	          0,
	          &start_sector,
	          &number_of_sectors,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "start_sector",
	 start_sector,
	 (uint64_t) 1024 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "number_of_sectors",
	 number_of_sectors,
	 (uint64_t) 16 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_table_get_checksum_error(
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

	checksum_errors = chunk_table->checksum_errors;

	chunk_table->checksum_errors = NULL;

	result = libewf_chunk_table_get_checksum_error(
	          chunk_table,
	          0,
	          &start_sector,
	          &number_of_sectors,
	          &error );

	chunk_table->checksum_errors = checksum_errors;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_chunk_table_get_checksum_error(
	          chunk_table,
	          -1,
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

	result = libewf_chunk_table_get_checksum_error(
	          chunk_table,
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

	result = libewf_chunk_table_get_checksum_error(
	          chunk_table,
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

	/* Clean up
	 */
	result = libewf_chunk_table_free(
	          &chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_table",
	 chunk_table );

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
	if( chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &chunk_table,
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

/* Tests the libewf_chunk_table_append_checksum_error function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_append_checksum_error(
     void )
{
	libcdata_range_list_t *checksum_errors = NULL;
	libcerror_error_t *error               = NULL;
	libewf_chunk_table_t *chunk_table      = NULL;
	libewf_io_handle_t *io_handle          = NULL;
	int result                             = 0;

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

	result = libewf_chunk_table_initialize(
	          &chunk_table,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_table",
	 chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_chunk_table_append_checksum_error(
	          chunk_table,
	          1024,
	          16,
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
	result = libewf_chunk_table_append_checksum_error(
	          NULL,
	          1024,
	          16,
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

	checksum_errors = chunk_table->checksum_errors;

	chunk_table->checksum_errors = NULL;

	result = libewf_chunk_table_append_checksum_error(
	          chunk_table,
	          1024,
	          16,
	          &error );

	chunk_table->checksum_errors = checksum_errors;

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
	result = libewf_chunk_table_free(
	          &chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_table",
	 chunk_table );

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
	if( chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &chunk_table,
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

/* Tests the libewf_chunk_table_get_segment_file_chunk_group_by_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_get_segment_file_chunk_group_by_offset(
     void )
{
	libcerror_error_t *error          = NULL;
	libewf_chunk_table_t *chunk_table = NULL;
	libewf_io_handle_t *io_handle     = NULL;
	int result                        = 0;

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

	result = libewf_chunk_table_initialize(
	          &chunk_table,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_table",
	 chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_table_append_checksum_error(
	          chunk_table,
	          1024,
	          16,
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
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_chunk_table_get_segment_file_chunk_group_by_offset(
	          NULL,
	          NULL,
	          NULL,
	          0,
	          NULL,
	          NULL,
	          NULL,
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

	/* Clean up
	 */
	result = libewf_chunk_table_free(
	          &chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_table",
	 chunk_table );

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
	if( chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &chunk_table,
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

/* Tests the libewf_chunk_table_get_chunk_data_by_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_table_get_chunk_data_by_offset(
     void )
{
	libcerror_error_t *error          = NULL;
	libewf_chunk_table_t *chunk_table = NULL;
	libewf_io_handle_t *io_handle     = NULL;
	int result                        = 0;

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

	result = libewf_chunk_table_initialize(
	          &chunk_table,
	          io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_table",
	 chunk_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_table_append_checksum_error(
	          chunk_table,
	          1024,
	          16,
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
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_chunk_table_get_chunk_data_by_offset(
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          0,
	          NULL,
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

	/* Clean up
	 */
	result = libewf_chunk_table_free(
	          &chunk_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_table",
	 chunk_table );

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
	if( chunk_table != NULL )
	{
		libewf_chunk_table_free(
		 &chunk_table,
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
	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_chunk_table_initialize",
	 ewf_test_chunk_table_initialize );

	EWF_TEST_RUN(
	 "libewf_chunk_table_free",
	 ewf_test_chunk_table_free );

	EWF_TEST_RUN(
	 "libewf_chunk_table_clone",
	 ewf_test_chunk_table_clone );

	EWF_TEST_RUN(
	 "libewf_chunk_table_get_number_of_checksum_errors",
	 ewf_test_chunk_table_get_number_of_checksum_errors );

	EWF_TEST_RUN(
	 "libewf_chunk_table_get_checksum_error",
	 ewf_test_chunk_table_get_checksum_error );

	EWF_TEST_RUN(
	 "libewf_chunk_table_append_checksum_error",
	 ewf_test_chunk_table_append_checksum_error );

	EWF_TEST_RUN(
	 "libewf_chunk_table_get_segment_file_chunk_group_by_offset",
	 ewf_test_chunk_table_get_segment_file_chunk_group_by_offset );

	EWF_TEST_RUN(
	 "libewf_chunk_table_get_chunk_data_by_offset",
	 ewf_test_chunk_table_get_chunk_data_by_offset );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


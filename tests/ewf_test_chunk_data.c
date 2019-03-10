/*
 * Library chunk_data type test program
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_chunk_data.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_chunk_data_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_data_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libewf_chunk_data_t *chunk_data = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 2;
	int number_of_memset_fail_tests = 2;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_chunk_data_initialize(
	          &chunk_data,
	          512,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_data",
	 chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_data_free(
	          &chunk_data,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_data",
	 chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_data_initialize(
	          NULL,
	          512,
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

	chunk_data = (libewf_chunk_data_t *) 0x12345678UL;

	result = libewf_chunk_data_initialize(
	          &chunk_data,
	          512,
	          1,
	          &error );

	chunk_data = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_chunk_data_initialize(
	          &chunk_data,
	          0,
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

	result = libewf_chunk_data_initialize(
	          &chunk_data,
	          (size32_t) ( INT32_MAX - 16 ) + 1,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_chunk_data_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_chunk_data_initialize(
		          &chunk_data,
		          512,
		          1,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( chunk_data != NULL )
			{
				libewf_chunk_data_free(
				 &chunk_data,
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
			 "chunk_data",
			 chunk_data );

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
		/* Test libewf_chunk_data_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_chunk_data_initialize(
		          &chunk_data,
		          512,
		          1,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( chunk_data != NULL )
			{
				libewf_chunk_data_free(
				 &chunk_data,
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
			 "chunk_data",
			 chunk_data );

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
	if( chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &chunk_data,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_chunk_data_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_data_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_chunk_data_free(
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

/* Tests the libewf_chunk_data_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_data_clone(
     void )
{
	libcerror_error_t *error                    = NULL;
	libewf_chunk_data_t *destination_chunk_data = NULL;
	libewf_chunk_data_t *source_chunk_data      = NULL;
	int result                                  = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 2;
	int test_number                             = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests             = 2;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libewf_chunk_data_initialize(
	          &source_chunk_data,
	          512,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_chunk_data",
	 source_chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_chunk_data_clone(
	          &destination_chunk_data,
	          source_chunk_data,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_chunk_data",
	 destination_chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_data_free(
	          &destination_chunk_data,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_chunk_data",
	 destination_chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_data_clone(
	          &destination_chunk_data,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_chunk_data",
	 destination_chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_chunk_data_clone(
	          NULL,
	          source_chunk_data,
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

	destination_chunk_data = (libewf_chunk_data_t *) 0x12345678UL;

	result = libewf_chunk_data_clone(
	          &destination_chunk_data,
	          source_chunk_data,
	          &error );

	destination_chunk_data = NULL;

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
		/* Test libewf_chunk_data_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_chunk_data_clone(
		          &destination_chunk_data,
		          source_chunk_data,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_chunk_data != NULL )
			{
				libewf_chunk_data_free(
				 &destination_chunk_data,
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
			 "destination_chunk_data",
			 destination_chunk_data );

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
		/* Test libewf_chunk_data_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_chunk_data_clone(
		          &destination_chunk_data,
		          source_chunk_data,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_chunk_data != NULL )
			{
				libewf_chunk_data_free(
				 &destination_chunk_data,
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
			 "destination_chunk_data",
			 destination_chunk_data );

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
	result = libewf_chunk_data_free(
	          &source_chunk_data,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_chunk_data",
	 source_chunk_data );

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
	if( destination_chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &destination_chunk_data,
		 NULL );
	}
	if( source_chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &source_chunk_data,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_chunk_data_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_data_read_buffer(
     void )
{
	uint8_t buffer[ 512 ];

	libcerror_error_t *error        = NULL;
	libewf_chunk_data_t *chunk_data = NULL;
	ssize_t read_count              = 0;
	int result                      = 0;

	/* Initialize test
	 */
	result = libewf_chunk_data_initialize(
	          &chunk_data,
	          512,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_data",
	 chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	chunk_data->data_size = 512;

	/* Test regular cases
	 */
	read_count = libewf_chunk_data_read_buffer(
	              chunk_data,
	              buffer,
	              512,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 512 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libewf_chunk_data_read_buffer(
	              NULL,
	              buffer,
	              512,
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

	read_count = libewf_chunk_data_read_buffer(
	              chunk_data,
	              NULL,
	              512,
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

	read_count = libewf_chunk_data_read_buffer(
	              chunk_data,
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

	read_count = libewf_chunk_data_read_buffer(
	              chunk_data,
	              buffer,
	              64,
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

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libewf_chunk_data_clone with memcpy failing
	 */
	ewf_test_memcpy_attempts_before_fail = 0;

	read_count = libewf_chunk_data_read_buffer(
	              chunk_data,
	              buffer,
	              512,
	              &error );

	if( ewf_test_memcpy_attempts_before_fail != -1 )
	{
		ewf_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	/* Clean up
	 */
	result = libewf_chunk_data_free(
	          &chunk_data,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_data",
	 chunk_data );

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
	if( chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &chunk_data,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_chunk_data_write_buffer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_data_write_buffer(
     void )
{
	uint8_t buffer[ 512 ];

	libcerror_error_t *error        = NULL;
	libewf_chunk_data_t *chunk_data = NULL;
	ssize_t write_count             = 0;
	int result                      = 0;

	/* Initialize test
	 */
	result = libewf_chunk_data_initialize(
	          &chunk_data,
	          512,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "chunk_data",
	 chunk_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	chunk_data->data_size = 512;

	/* Test regular cases
	 */
	write_count = libewf_chunk_data_write_buffer(
	               chunk_data,
	               buffer,
	               512,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) 512 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	write_count = libewf_chunk_data_write_buffer(
	               NULL,
	               buffer,
	               512,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_chunk_data_write_buffer(
	               chunk_data,
	               NULL,
	               512,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_chunk_data_write_buffer(
	               chunk_data,
	               buffer,
	               (size_t) SSIZE_MAX + 1,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_chunk_data_write_buffer(
	               chunk_data,
	               buffer,
	               576,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libewf_chunk_data_clone with memcpy failing
	 */
	ewf_test_memcpy_attempts_before_fail = 0;

	write_count = libewf_chunk_data_write_buffer(
	               chunk_data,
	               buffer,
	               512,
	               &error );

	if( ewf_test_memcpy_attempts_before_fail != -1 )
	{
		ewf_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "write_count",
		 write_count,
		 (ssize_t) -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	/* Clean up
	 */
	result = libewf_chunk_data_free(
	          &chunk_data,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "chunk_data",
	 chunk_data );

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
	if( chunk_data != NULL )
	{
		libewf_chunk_data_free(
		 &chunk_data,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_chunk_data_check_for_empty_block function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_data_check_for_empty_block(
     void )
{
	uint8_t buffer[ 512 ];

	libcerror_error_t *error = NULL;
	void *memset_result      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 buffer,
	                 0,
	                 512 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	/* Test regular cases
	 */
	result = libewf_chunk_data_check_for_empty_block(
	          buffer,
	          512,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	buffer[ 500 ] = (uint8_t) 'A';

	result = libewf_chunk_data_check_for_empty_block(
	          buffer,
	          512,
	          &error );

	buffer[ 500 ] = 0;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_data_check_for_empty_block(
	          buffer,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_data_check_for_empty_block(
	          buffer,
	          1,
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
	result = libewf_chunk_data_check_for_empty_block(
	          NULL,
	          512,
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

	result = libewf_chunk_data_check_for_empty_block(
	          buffer,
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

/* Tests the libewf_chunk_data_check_for_64_bit_pattern_fill function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_chunk_data_check_for_64_bit_pattern_fill(
     void )
{
	uint8_t buffer[ 512 ];

	libcerror_error_t *error = NULL;
	void *memset_result      = NULL;
	uint64_t pattern         = 0;
	int result               = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 buffer,
	                 'X',
	                 512 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	/* Test regular cases
	 */
	pattern = 0;

	result = libewf_chunk_data_check_for_64_bit_pattern_fill(
	          buffer,
	          512,
	          &pattern,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "pattern",
	 pattern,
	 (uint64_t) 0x5858585858585858UL );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	pattern = 0;

	buffer[ 500 ] = (uint8_t) 'A';

	result = libewf_chunk_data_check_for_64_bit_pattern_fill(
	          buffer,
	          512,
	          &pattern,
	          &error );

	buffer[ 500 ] = 0;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "pattern",
	 pattern,
	 (uint64_t) 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_data_check_for_64_bit_pattern_fill(
	          buffer,
	          0,
	          &pattern,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_chunk_data_check_for_64_bit_pattern_fill(
	          buffer,
	          511,
	          &pattern,
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
	result = libewf_chunk_data_check_for_64_bit_pattern_fill(
	          NULL,
	          512,
	          &pattern,
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

	result = libewf_chunk_data_check_for_64_bit_pattern_fill(
	          buffer,
	          (size_t) SSIZE_MAX + 1,
	          &pattern,
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

	result = libewf_chunk_data_check_for_64_bit_pattern_fill(
	          buffer,
	          512,
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
	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_chunk_data_initialize",
	 ewf_test_chunk_data_initialize );

	EWF_TEST_RUN(
	 "libewf_chunk_data_free",
	 ewf_test_chunk_data_free );

	EWF_TEST_RUN(
	 "libewf_chunk_data_clone",
	 ewf_test_chunk_data_clone );

	EWF_TEST_RUN(
	 "libewf_chunk_data_read_buffer",
	 ewf_test_chunk_data_read_buffer );

	EWF_TEST_RUN(
	 "libewf_chunk_data_write_buffer",
	 ewf_test_chunk_data_write_buffer );

	/* TODO: add tests for libewf_chunk_data_pack */

	/* TODO: add tests for libewf_chunk_data_unpack */

	EWF_TEST_RUN(
	 "libewf_chunk_data_check_for_empty_block",
	 ewf_test_chunk_data_check_for_empty_block );

	EWF_TEST_RUN(
	 "libewf_chunk_data_check_for_64_bit_pattern_fill",
	 ewf_test_chunk_data_check_for_64_bit_pattern_fill );

	/* TODO: add tests for libewf_chunk_data_write */

	/* TODO: add tests for libewf_chunk_data_get_write_size */

	/* TODO: add tests for libewf_chunk_data_get_checksum */

	/* TODO: add tests for libewf_chunk_data_read_from_file_io_pool */

	/* TODO: add tests for libewf_chunk_data_read_element_data */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


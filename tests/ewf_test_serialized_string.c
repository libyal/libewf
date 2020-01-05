/*
 * Library serialized_string functions test program
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

#include "../libewf/libewf_serialized_string.h"

uint8_t ewf_test_serialized_string_values_data1[ 33 ] = {
	0x44, 0x43, 0x31, 0x38, 0x35, 0x43, 0x36, 0x38, 0x31, 0x31, 0x34, 0x44, 0x34, 0x45, 0x41, 0x45,
	0x42, 0x33, 0x41, 0x37, 0x38, 0x45, 0x43, 0x33, 0x33, 0x36, 0x33, 0x43, 0x36, 0x34, 0x42, 0x36,
	0x00 };

uint8_t ewf_test_serialized_string_values_data2[ 32 ] = {
	0x64, 0x63, 0x31, 0x38, 0x35, 0x63, 0x36, 0x38, 0x31, 0x31, 0x34, 0x64, 0x34, 0x65, 0x61, 0x65,
	0x62, 0x33, 0x61, 0x37, 0x38, 0x65, 0x63, 0x33, 0x33, 0x36, 0x33, 0x63, 0x36, 0x34, 0x62, 0x36 };

uint8_t ewf_test_serialized_string_values_data3[ 32 ] = {
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_serialized_string_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_initialize(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_serialized_string_t *serialized_string = NULL;
	int result                                    = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int number_of_memset_fail_tests               = 1;
	int test_number                               = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_serialized_string_initialize(
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

	serialized_string = (libewf_serialized_string_t *) 0x12345678UL;

	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	serialized_string = NULL;

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
		/* Test libewf_serialized_string_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_serialized_string_initialize(
		          &serialized_string,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( serialized_string != NULL )
			{
				libewf_serialized_string_free(
				 &serialized_string,
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
			 "serialized_string",
			 serialized_string );

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
		/* Test libewf_serialized_string_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_serialized_string_initialize(
		          &serialized_string,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( serialized_string != NULL )
			{
				libewf_serialized_string_free(
				 &serialized_string,
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
			 "serialized_string",
			 serialized_string );

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
	if( serialized_string != NULL )
	{
		libewf_serialized_string_free(
		 &serialized_string,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_serialized_string_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_serialized_string_free(
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

/* Tests the libewf_serialized_string_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_clone(
     void )
{
	libcerror_error_t *error                                  = NULL;
	libewf_serialized_string_t *destination_serialized_string = NULL;
	libewf_serialized_string_t *source_serialized_string      = NULL;
	int result                                                = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                           = 2;
	int test_number                                           = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                           = 1;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &source_serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_serialized_string",
	 source_serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_serialized_string_read_data(
	          source_serialized_string,
	          ewf_test_serialized_string_values_data1,
	          33,
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
	result = libewf_serialized_string_clone(
	          &destination_serialized_string,
	          source_serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_serialized_string",
	 destination_serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_serialized_string_free(
	          &destination_serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_serialized_string",
	 destination_serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_serialized_string_clone(
	          &destination_serialized_string,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_serialized_string",
	 destination_serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_serialized_string_clone(
	          NULL,
	          source_serialized_string,
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

	destination_serialized_string = (libewf_serialized_string_t *) 0x12345678UL;

	result = libewf_serialized_string_clone(
	          &destination_serialized_string,
	          source_serialized_string,
	          &error );

	destination_serialized_string = NULL;

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
		/* Test libewf_serialized_string_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_serialized_string_clone(
		          &destination_serialized_string,
		          source_serialized_string,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_serialized_string != NULL )
			{
				libewf_serialized_string_free(
				 &destination_serialized_string,
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
			 "destination_serialized_string",
			 destination_serialized_string );

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
		/* Test libewf_serialized_string_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_serialized_string_clone(
		          &destination_serialized_string,
		          source_serialized_string,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_serialized_string != NULL )
			{
				libewf_serialized_string_free(
				 &destination_serialized_string,
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
			 "destination_serialized_string",
			 destination_serialized_string );

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
	result = libewf_serialized_string_free(
	          &source_serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_serialized_string",
	 source_serialized_string );

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
	if( destination_serialized_string != NULL )
	{
		libewf_serialized_string_free(
		 &destination_serialized_string,
		 NULL );
	}
	if( source_serialized_string != NULL )
	{
		libewf_serialized_string_free(
		 &source_serialized_string,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_serialized_string_read_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_read_data(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_serialized_string_t *serialized_string = NULL;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_serialized_string_read_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data1,
	          33,
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
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_serialized_string_read_data(
	          serialized_string,
	          NULL,
	          0,
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
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_serialized_string_read_data(
	          NULL,
	          ewf_test_serialized_string_values_data1,
	          33,
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

	result = libewf_serialized_string_read_data(
	          serialized_string,
	          NULL,
	          33,
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

	result = libewf_serialized_string_read_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data1,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_serialized_string_read_data with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_serialized_string_read_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data1,
	          33,
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

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

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
	if( serialized_string != NULL )
	{
		libewf_serialized_string_free(
		 &serialized_string,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_serialized_string_read_hexadecimal_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_read_hexadecimal_data(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_serialized_string_t *serialized_string = NULL;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_serialized_string_read_hexadecimal_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data1,
	          33,
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
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test with lower case
	 */
	result = libewf_serialized_string_read_hexadecimal_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data2,
	          32,
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
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test with all zeros
	 */
	result = libewf_serialized_string_read_hexadecimal_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data3,
	          32,
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
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_serialized_string_read_hexadecimal_data(
	          NULL,
	          ewf_test_serialized_string_values_data1,
	          33,
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

	result = libewf_serialized_string_read_hexadecimal_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data1,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_serialized_string_read_hexadecimal_data with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_serialized_string_read_hexadecimal_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data1,
	          33,
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

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

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
	if( serialized_string != NULL )
	{
		libewf_serialized_string_free(
		 &serialized_string,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_serialized_string_get_utf8_string_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_get_utf8_string_size(
     libewf_serialized_string_t *serialized_string )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	utf8_string_size = 0;

	result = libewf_serialized_string_get_utf8_string_size(
	          serialized_string,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 33 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	utf8_string_size = 0;

	result = libewf_serialized_string_get_utf8_string_size(
	          NULL,
	          &utf8_string_size,
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

	result = libewf_serialized_string_get_utf8_string_size(
	          serialized_string,
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

/* Tests the libewf_serialized_string_get_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_get_utf8_string(
     libewf_serialized_string_t *serialized_string )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_serialized_string_get_utf8_string(
	          serialized_string,
	          utf8_string,
	          64,
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
	result = libewf_serialized_string_get_utf8_string(
	          NULL,
	          utf8_string,
	          64,
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

	result = libewf_serialized_string_get_utf8_string(
	          serialized_string,
	          NULL,
	          64,
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

	result = libewf_serialized_string_get_utf8_string(
	          serialized_string,
	          utf8_string,
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

	result = libewf_serialized_string_get_utf8_string(
	          serialized_string,
	          utf8_string,
	          0,
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

/* Tests the libewf_serialized_string_get_utf16_string_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_get_utf16_string_size(
     libewf_serialized_string_t *serialized_string )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	utf16_string_size = 0;

	result = libewf_serialized_string_get_utf16_string_size(
	          serialized_string,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 33 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	utf16_string_size = 0;

	result = libewf_serialized_string_get_utf16_string_size(
	          NULL,
	          &utf16_string_size,
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

	result = libewf_serialized_string_get_utf16_string_size(
	          serialized_string,
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

/* Tests the libewf_serialized_string_get_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_serialized_string_get_utf16_string(
     libewf_serialized_string_t *serialized_string )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_serialized_string_get_utf16_string(
	          serialized_string,
	          utf16_string,
	          64,
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
	result = libewf_serialized_string_get_utf16_string(
	          NULL,
	          utf16_string,
	          64,
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

	result = libewf_serialized_string_get_utf16_string(
	          serialized_string,
	          NULL,
	          64,
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

	result = libewf_serialized_string_get_utf16_string(
	          serialized_string,
	          utf16_string,
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

	result = libewf_serialized_string_get_utf16_string(
	          serialized_string,
	          utf16_string,
	          0,
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

	libcerror_error_t *error                      = NULL;
	libewf_serialized_string_t *serialized_string = NULL;
	int result                                    = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_serialized_string_initialize",
	 ewf_test_serialized_string_initialize );

	EWF_TEST_RUN(
	 "libewf_serialized_string_free",
	 ewf_test_serialized_string_free );

	EWF_TEST_RUN(
	 "libewf_serialized_string_clone",
	 ewf_test_serialized_string_clone );

	EWF_TEST_RUN(
	 "libewf_serialized_string_read_data",
	 ewf_test_serialized_string_read_data );

	EWF_TEST_RUN(
	 "libewf_serialized_string_read_hexadecimal_data",
	 ewf_test_serialized_string_read_hexadecimal_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize serialized_string for tests
	 */
	result = libewf_serialized_string_initialize(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_serialized_string_read_data(
	          serialized_string,
	          ewf_test_serialized_string_values_data1,
	          33,
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
	 "libewf_serialized_string_get_utf8_string_size",
	 ewf_test_serialized_string_get_utf8_string_size,
	 serialized_string );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_serialized_string_get_utf8_string",
	 ewf_test_serialized_string_get_utf8_string,
	 serialized_string );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_serialized_string_get_utf16_string_size",
	 ewf_test_serialized_string_get_utf16_string_size,
	 serialized_string );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_serialized_string_get_utf16_string",
	 ewf_test_serialized_string_get_utf16_string,
	 serialized_string );

/* TODO add tests for libewf_serialized_string_compare_with_utf8_string */

/* TODO add tests for libewf_serialized_string_compare_with_utf16_string */

	/* Clean up
	 */
	result = libewf_serialized_string_free(
	          &serialized_string,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "serialized_string",
	 serialized_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( serialized_string != NULL )
	{
		libewf_serialized_string_free(
		 &serialized_string,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


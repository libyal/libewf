/*
 * Library value_table functions test program
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
#include "ewf_test_libfvalue.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_value_table.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_value_table_get_value_utf8_string_length function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_get_value_utf8_string_length(
     void )
{
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *value_table = NULL;
	size_t utf8_string_length      = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement
	result = libewf_value_table_get_value_utf8_string_length(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          &utf8_string_length,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libewf_value_table_get_value_utf8_string_length(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          &utf8_string_length,
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

	result = libewf_value_table_get_value_utf8_string_length(
	          value_table,
	          NULL,
	          10,
	          &utf8_string_length,
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

	result = libewf_value_table_get_value_utf8_string_length(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
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
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_get_value_copy_to_utf8_string_with_index function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_get_value_copy_to_utf8_string_with_index(
     void )
{
	uint8_t utf8_string[ 16 ];

	libcerror_error_t *error       = NULL;
	libfvalue_table_t *value_table = NULL;
	size_t utf8_string_index       = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
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
	utf8_string_index = 0;

	result = libewf_value_table_get_value_copy_to_utf8_string_with_index(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          utf8_string,
	          16,
	          &utf8_string_index,
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
	utf8_string_index = 0;

	result = libewf_value_table_get_value_copy_to_utf8_string_with_index(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          utf8_string,
	          16,
	          &utf8_string_index,
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

	result = libewf_value_table_get_value_copy_to_utf8_string_with_index(
	          value_table,
	          NULL,
	          10,
	          utf8_string,
	          16,
	          &utf8_string_index,
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

	result = libewf_value_table_get_value_copy_to_utf8_string_with_index(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          utf8_string,
	          16,
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
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_set_value_by_index function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_set_value_by_index(
     void )
{
	libcerror_error_t *error        = NULL;
	libfvalue_table_t *value_table  = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 3;
	int test_number                 = 0;
#endif

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
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
	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          NULL,
	          12,
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
		/* Test libewf_value_table_set_value_by_index with malloc failing in
		 * 1. libfvalue_value_type_initialize
		 * 2. libfvalue_value_set_identifier
		 * 3. libfvalue_table_set_value_by_index
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_value_table_set_value_by_index(
		          value_table,
		          0,
		          (uint8_t *) "identifier",
		          10,
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
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_get_utf8_value_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_get_utf8_value_size(
     void )
{
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *value_table = NULL;
	size_t utf8_value_size         = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_copy_value_from_utf8_string(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          (uint8_t *) "test",
	          4,
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
	utf8_value_size = 0;

	result = libewf_value_table_get_utf8_value_size(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          &utf8_value_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_value_size",
	 utf8_value_size,
	 (size_t) 5 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	utf8_value_size = 0;

	result = libewf_value_table_get_utf8_value_size(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          &utf8_value_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_value_size",
	 utf8_value_size,
	 (size_t) 0 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_get_utf8_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_get_utf8_value(
     void )
{
	uint8_t value[ 16 ];

	uint8_t expected_utf8_value[ 5 ] = { 't', 'e', 's', 't', 0 };
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *value_table   = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_copy_value_from_utf8_string(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          (uint8_t *) "test",
	          4,
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
	result = libewf_value_table_get_utf8_value(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          value,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          value,
	          expected_utf8_value,
	          5 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libewf_value_table_get_utf8_value(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          value,
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

	/* Clean up
	 */
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_copy_value_from_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_copy_value_from_utf8_string(
     void )
{
	uint8_t utf8_string[ 5 ]       = { 't', 'e', 's', 't', 0 };
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *value_table = NULL;
	int result                     = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
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
	result = libewf_value_table_copy_value_from_utf8_string(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          utf8_string,
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
	result = libewf_value_table_copy_value_from_utf8_string(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          utf8_string,
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

	result = libewf_value_table_copy_value_from_utf8_string(
	          value_table,
	          NULL,
	          10,
	          utf8_string,
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

	result = libewf_value_table_copy_value_from_utf8_string(
	          value_table,
	          (uint8_t *) "identifier",
	          (size_t) SSIZE_MAX + 1,
	          utf8_string,
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
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_get_utf16_value_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_get_utf16_value_size(
     void )
{
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *value_table = NULL;
	size_t utf16_value_size        = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_copy_value_from_utf8_string(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          (uint8_t *) "test",
	          4,
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
	utf16_value_size = 0;

	result = libewf_value_table_get_utf16_value_size(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          &utf16_value_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_value_size",
	 utf16_value_size,
	 (size_t) 5 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	utf16_value_size = 0;

	result = libewf_value_table_get_utf16_value_size(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          &utf16_value_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_value_size",
	 utf16_value_size,
	 (size_t) 0 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_get_utf16_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_get_utf16_value(
     void )
{
	uint16_t value[ 16 ];

	uint16_t expected_utf16_value[ 5 ] = { 't', 'e', 's', 't', 0 };
	libcerror_error_t *error           = NULL;
	libfvalue_table_t *value_table     = NULL;
	int result                         = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_copy_value_from_utf8_string(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          (uint8_t *) "test",
	          4,
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
	result = libewf_value_table_get_utf16_value(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          value,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          value,
	          expected_utf16_value,
	          5 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libewf_value_table_get_utf16_value(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          value,
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

	/* Clean up
	 */
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_value_table_copy_value_from_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_value_table_copy_value_from_utf16_string(
     void )
{
	uint16_t utf16_string[ 5 ]     = { 't', 'e', 's', 't', 0 };
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *value_table = NULL;
	int result                     = 0;

	/* Initialize test
	 */
	result = libfvalue_table_initialize(
	          &value_table,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "value_table",
	 value_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_value_table_set_value_by_index(
	          value_table,
	          0,
	          (uint8_t *) "identifier",
	          10,
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
	result = libewf_value_table_copy_value_from_utf16_string(
	          value_table,
	          (uint8_t *) "identifier",
	          10,
	          utf16_string,
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
	result = libewf_value_table_copy_value_from_utf16_string(
	          NULL,
	          (uint8_t *) "identifier",
	          10,
	          utf16_string,
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

	result = libewf_value_table_copy_value_from_utf16_string(
	          value_table,
	          NULL,
	          10,
	          utf16_string,
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

	result = libewf_value_table_copy_value_from_utf16_string(
	          value_table,
	          (uint8_t *) "identifier",
	          (size_t) SSIZE_MAX + 1,
	          utf16_string,
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
	result = libfvalue_table_free(
	          &value_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "value_table",
	 value_table );

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
	if( value_table != NULL )
	{
		libfvalue_table_free(
		 &value_table,
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
	 "libewf_value_table_get_value_utf8_string_length",
	 ewf_test_value_table_get_value_utf8_string_length );

	EWF_TEST_RUN(
	 "libewf_value_table_get_value_copy_to_utf8_string_with_index",
	 ewf_test_value_table_get_value_copy_to_utf8_string_with_index );

	EWF_TEST_RUN(
	 "libewf_value_table_set_value_by_index",
	 ewf_test_value_table_set_value_by_index );

	EWF_TEST_RUN(
	 "libewf_value_table_get_utf8_value_size",
	 ewf_test_value_table_get_utf8_value_size );

	EWF_TEST_RUN(
	 "libewf_value_table_get_utf8_value",
	 ewf_test_value_table_get_utf8_value );

	EWF_TEST_RUN(
	 "libewf_value_table_copy_value_from_utf8_string",
	 ewf_test_value_table_copy_value_from_utf8_string );

	EWF_TEST_RUN(
	 "libewf_value_table_get_utf16_value_size",
	 ewf_test_value_table_get_utf16_value_size );

	EWF_TEST_RUN(
	 "libewf_value_table_get_utf16_value",
	 ewf_test_value_table_get_utf16_value );

	EWF_TEST_RUN(
	 "libewf_value_table_copy_value_from_utf16_string",
	 ewf_test_value_table_copy_value_from_utf16_string );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


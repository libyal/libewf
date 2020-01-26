/*
 * Library header_values type test program
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

#include "../libewf/libewf_header_values.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_header_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	int result                       = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 88;
	int number_of_memset_fail_tests  = 1;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_table_free(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_header_values_initialize(
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
		/* Test libewf_header_values_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_header_values_initialize(
		          &header_values,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( header_values != NULL )
			{
				libfvalue_table_free(
				 &header_values,
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
			 "header_values",
			 header_values );

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
		/* Test libewf_header_values_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_header_values_initialize(
		          &header_values,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( header_values != NULL )
			{
				libfvalue_table_free(
				 &header_values,
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
			 "header_values",
			 header_values );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_set_value_by_index function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_set_value_by_index(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	int result                       = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 3;
	int test_number                  = 0;
#endif

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_set_value_by_index(
	          header_values,
	          LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
	          (uint8_t *) "case_number",
	          12,
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
	result = libewf_header_values_set_value_by_index(
	          header_values,
	          LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
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
		/* Test libewf_header_values_set_value_by_index with malloc failing in
		 * 1. libfvalue_value_type_initialize
		 * 2. libfvalue_value_set_identifier
		 * 3. libfvalue_table_set_value_by_index
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_header_values_set_value_by_index(
		          header_values,
		          LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
		          (uint8_t *) "case_number",
		          12,
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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_convert_date_header_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_convert_date_header_value(
     void )
{
	libcerror_error_t *error            = NULL;
	uint8_t *date_time_values_string    = NULL;
	size_t date_time_values_string_size = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_convert_date_header_value(
	          (uint8_t *) "2006 4 1 20 47 44",
	          18,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "date_time_values_string",
	 date_time_values_string );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "date_time_values_string_size",
	 date_time_values_string_size,
	 (size_t) 20 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          date_time_values_string,
	          "2006 04 01 20 47 44",
	          20 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 date_time_values_string );

	date_time_values_string = NULL;

	/* Test error cases
	 */
	result = libewf_convert_date_header_value(
	          (uint8_t *) "2006 4 1 20 47 44",
	          18,
	          NULL,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_convert_date_header_value(
	          (uint8_t *) "2006 4 1 20 47 44",
	          18,
	          &date_time_values_string,
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

	/* Test libewf_convert_date_header_value with failing libfvalue_utf8_string_split
	 */
	result = libewf_convert_date_header_value(
	          NULL,
	          18,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_convert_date_header_value(
	          (uint8_t *) "2006 4 1 20 47",
	          15,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

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

	/* Test libewf_convert_date_header_value with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_convert_date_header_value(
	          (uint8_t *) "2006 4 1 20 47 44",
	          18,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( date_time_values_string != NULL )
		{
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );
	}
	return( 0 );
}

/* Tests the libewf_generate_date_header_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_generate_date_header_value(
     void )
{
	libcerror_error_t *error            = NULL;
	uint8_t *date_time_values_string    = NULL;
	size_t date_time_values_string_size = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_generate_date_header_value(
	          1578288885,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "date_time_values_string",
	 date_time_values_string );

	/* Note that libewf_generate_date_header_value allocates a string of size 20 */

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "date_time_values_string_size",
	 date_time_values_string_size,
	 (size_t) 20 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          date_time_values_string,
	          "2020 1 6 6 34 45",
	          17 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 date_time_values_string );

	date_time_values_string = NULL;

	/* Test error cases
	 */
	result = libewf_generate_date_header_value(
	          1578288885,
	          NULL,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_generate_date_header_value(
	          1578288885,
	          &date_time_values_string,
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

	/* Test libewf_generate_date_header_value with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_generate_date_header_value(
	          1578288885,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( date_time_values_string != NULL )
		{
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );
	}
	return( 0 );
}

/* Tests the libewf_convert_date_header2_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_convert_date_header2_value(
     void )
{
	libcerror_error_t *error            = NULL;
	uint8_t *date_time_values_string    = NULL;
	size_t date_time_values_string_size = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_convert_date_header2_value(
	          (uint8_t *) "1578288885",
	          11,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "date_time_values_string",
	 date_time_values_string );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "date_time_values_string_size",
	 date_time_values_string_size,
	 (size_t) 20 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          date_time_values_string,
	          "2020 01 06 06 34 45",
	          20 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 date_time_values_string );

	date_time_values_string = NULL;

	/* Test error cases
	 */
	result = libewf_convert_date_header2_value(
	          (uint8_t *) "1578288885",
	          11,
	          NULL,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_convert_date_header2_value(
	          (uint8_t *) "1578288885",
	          11,
	          &date_time_values_string,
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

	/* Test libewf_convert_date_header2_value with failing libfvalue_utf8_string_copy_to_integer
	 */
	result = libewf_convert_date_header2_value(
	          NULL,
	          11,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

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

	/* Test libewf_convert_date_header2_value with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_convert_date_header2_value(
	          (uint8_t *) "1578288885",
	          11,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( date_time_values_string != NULL )
		{
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );
	}
	return( 0 );
}

/* Tests the libewf_generate_date_header2_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_generate_date_header2_value(
     void )
{
	libcerror_error_t *error            = NULL;
	uint8_t *date_time_values_string    = NULL;
	size_t date_time_values_string_size = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_generate_date_header2_value(
	          1578288885,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "date_time_values_string",
	 date_time_values_string );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "date_time_values_string_size",
	 date_time_values_string_size,
	 (size_t) 11 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          date_time_values_string,
	          "1578288885",
	          11 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 date_time_values_string );

	date_time_values_string = NULL;

	/* Test error cases
	 */
	result = libewf_generate_date_header2_value(
	          1578288885,
	          NULL,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_generate_date_header2_value(
	          1578288885,
	          &date_time_values_string,
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

	/* Test libewf_generate_date_header2_value with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_generate_date_header2_value(
	          1578288885,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( date_time_values_string != NULL )
		{
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );
	}
	return( 0 );
}

/* Tests the libewf_header_values_copy function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_copy(
     void )
{
	libcerror_error_t *error                     = NULL;
	libfvalue_table_t *destination_header_values = NULL;
	libfvalue_table_t *source_header_values      = NULL;
	int result                                   = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &source_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_header_values",
	 source_header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_header_values_initialize(
	          &destination_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_header_values",
	 destination_header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_copy(
	          destination_header_values,
	          source_header_values,
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
	result = libewf_header_values_copy(
	          NULL,
	          source_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_header_values_copy(
	          destination_header_values,
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
	          &destination_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_header_values",
	 destination_header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_table_free(
	          &source_header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_header_values",
	 source_header_values );

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
	if( destination_header_values != NULL )
	{
		libfvalue_table_free(
		 &destination_header_values,
		 NULL );
	}
	if( source_header_values != NULL )
	{
		libfvalue_table_free(
		 &source_header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_parse_utf8_header_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_parse_utf8_header_string(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header_string           = (uint8_t *) "";
	uint8_t format                   = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_header_values_parse_utf8_header_string(
	          header_values,
	          NULL,
	          0,
	          1,
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

	result = libewf_header_values_parse_utf8_header_string(
	          header_values,
	          header_string,
	          0,
	          0xff,
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

	result = libewf_header_values_parse_utf8_header_string(
	          header_values,
	          header_string,
	          0,
	          1,
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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_parse_header function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_parse_header(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	char *header                     = "1\nmain\nc\tn\ta\te\tt\tm\tu\tp\tr\ncase\tevidence\tdescription\texaminer\tnotes\t2006 4 1 20 47 44\t2006 4 1 8 47 28\t0\tf\n\n";
	uint8_t format                   = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_parse_header(
	          header_values,
	          (uint8_t *) header,
	          107,
	          LIBEWF_CODEPAGE_ASCII,
	          &format,
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
	result = libfvalue_table_free(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */

	/* Test libewf_header_values_parse_header with libuna_utf8_string_size_from_byte_stream failing
	 */
	result = libewf_header_values_parse_header(
	          header_values,
	          NULL,
	          107,
	          LIBEWF_CODEPAGE_ASCII,
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

	/* Test libewf_header_values_parse_header with libewf_header_values_parse_utf8_header_string failing
	 */
	result = libewf_header_values_parse_header(
	          NULL,
	          (uint8_t *) header,
	          107,
	          LIBEWF_CODEPAGE_ASCII,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_header_values_parse_header with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_header_values_parse_header(
	          header_values,
	          (uint8_t *) header,
	          107,
	          LIBEWF_CODEPAGE_ASCII,
	          &format,
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
	result = libfvalue_table_free(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_parse_header2 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_parse_header2(
     void )
{
	uint8_t header2[ 454 ] = {
		0xff, 0xfe, 0x31, 0x00, 0x0a, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x0a, 0x00,
		0x61, 0x00, 0x09, 0x00, 0x63, 0x00, 0x09, 0x00, 0x6e, 0x00, 0x09, 0x00, 0x65, 0x00, 0x09, 0x00,
		0x74, 0x00, 0x09, 0x00, 0x61, 0x00, 0x76, 0x00, 0x09, 0x00, 0x6f, 0x00, 0x76, 0x00, 0x09, 0x00,
		0x6d, 0x00, 0x09, 0x00, 0x75, 0x00, 0x09, 0x00, 0x70, 0x00, 0x0a, 0x00, 0x66, 0x00, 0x6c, 0x00,
		0x6f, 0x00, 0x70, 0x00, 0x70, 0x00, 0x79, 0x00, 0x2d, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x63, 0x00,
		0x61, 0x00, 0x73, 0x00, 0x65, 0x00, 0x34, 0x00, 0x2e, 0x00, 0x32, 0x00, 0x32, 0x00, 0x2d, 0x00,
		0x6e, 0x00, 0x6f, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x72, 0x00, 0x65, 0x00,
		0x73, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x09, 0x00, 0x6c, 0x00, 0x69, 0x00,
		0x62, 0x00, 0x65, 0x00, 0x77, 0x00, 0x66, 0x00, 0x2d, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x63, 0x00,
		0x61, 0x00, 0x73, 0x00, 0x65, 0x00, 0x34, 0x00, 0x2e, 0x00, 0x32, 0x00, 0x32, 0x00, 0x09, 0x00,
		0x66, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x70, 0x00, 0x70, 0x00, 0x79, 0x00, 0x2d, 0x00, 0x65, 0x00,
		0x6e, 0x00, 0x63, 0x00, 0x61, 0x00, 0x73, 0x00, 0x65, 0x00, 0x34, 0x00, 0x2e, 0x00, 0x32, 0x00,
		0x32, 0x00, 0x2d, 0x00, 0x6e, 0x00, 0x6f, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00,
		0x72, 0x00, 0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x09, 0x00,
		0x6a, 0x00, 0x62, 0x00, 0x6d, 0x00, 0x09, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x67, 0x00,
		0x65, 0x00, 0x20, 0x00, 0x6f, 0x00, 0x66, 0x00, 0x20, 0x00, 0x66, 0x00, 0x6c, 0x00, 0x6f, 0x00,
		0x70, 0x00, 0x70, 0x00, 0x79, 0x00, 0x20, 0x00, 0x75, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6e, 0x00,
		0x67, 0x00, 0x20, 0x00, 0x45, 0x00, 0x6e, 0x00, 0x63, 0x00, 0x61, 0x00, 0x73, 0x00, 0x65, 0x00,
		0x20, 0x00, 0x34, 0x00, 0x2e, 0x00, 0x32, 0x00, 0x32, 0x00, 0x20, 0x00, 0x6e, 0x00, 0x6f, 0x00,
		0x20, 0x00, 0x63, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x72, 0x00, 0x65, 0x00, 0x73, 0x00,
		0x73, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x6e, 0x00, 0x6f, 0x00, 0x20, 0x00,
		0x70, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00, 0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x64, 0x00,
		0x20, 0x00, 0x77, 0x00, 0x69, 0x00, 0x74, 0x00, 0x68, 0x00, 0x20, 0x00, 0x68, 0x00, 0x61, 0x00,
		0x73, 0x00, 0x68, 0x00, 0x09, 0x00, 0x34, 0x00, 0x2e, 0x00, 0x32, 0x00, 0x32, 0x00, 0x09, 0x00,
		0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x64, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x73, 0x00, 0x20, 0x00,
		0x58, 0x00, 0x50, 0x00, 0x09, 0x00, 0x31, 0x00, 0x31, 0x00, 0x34, 0x00, 0x32, 0x00, 0x31, 0x00,
		0x36, 0x00, 0x32, 0x00, 0x32, 0x00, 0x36, 0x00, 0x38, 0x00, 0x09, 0x00, 0x31, 0x00, 0x31, 0x00,
		0x34, 0x00, 0x32, 0x00, 0x31, 0x00, 0x36, 0x00, 0x32, 0x00, 0x32, 0x00, 0x36, 0x00, 0x38, 0x00,
		0x09, 0x00, 0x0a, 0x00, 0x0a, 0x00 };

	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t format                   = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_parse_header2(
	          header_values,
	          header2,
	          454,
	          &format,
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
	result = libfvalue_table_free(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */

	/* Test libewf_header_values_parse_header2 with libuna_utf8_string_size_from_utf16_stream failing
	 */
	result = libewf_header_values_parse_header2(
	          header_values,
	          NULL,
	          454,
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

	/* Test libewf_header_values_parse_header2 with libewf_header_values_parse_utf8_header_string failing
	 */
	result = libewf_header_values_parse_header2(
	          NULL,
	          header2,
	          454,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_header_values_parse_header with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_header_values_parse_header2(
	          header_values,
	          header2,
	          454,
	          &format,
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
	result = libfvalue_table_free(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_convert_utf8_header_string_to_header function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_convert_utf8_header_string_to_header(
     void )
{
	libcerror_error_t *error = NULL;
	uint8_t *header          = NULL;
	uint8_t *header_string   = (uint8_t *) "";
	size_t header_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_header_values_convert_utf8_header_string_to_header(
	          NULL,
	          0,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_header_values_convert_utf8_header_string_to_header(
	          header_string,
	          0,
	          NULL,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	header = (uint8_t *) 0x12345678UL;

	result = libewf_header_values_convert_utf8_header_string_to_header(
	          header_string,
	          0,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	header = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_header_values_convert_utf8_header_string_to_header(
	          header_string,
	          0,
	          &header,
	          NULL,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

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

/* Tests the libewf_header_values_generate_utf8_header_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_utf8_header_string(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *utf8_string             = NULL;
	size_t utf8_string_size          = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_header_values_generate_utf8_header_string(
	          header_values,
	          0xff,
	          (uint8_t *) "\n",
	          1,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &utf8_string,
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

	result = libewf_header_values_generate_utf8_header_string(
	          header_values,
	          LIBEWF_HEADER_STRING_TYPE_1,
	          NULL,
	          1,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &utf8_string,
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

	result = libewf_header_values_generate_utf8_header_string(
	          header_values,
	          LIBEWF_HEADER_STRING_TYPE_1,
	          (uint8_t *) "\n",
	          0,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &utf8_string,
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

	result = libewf_header_values_generate_utf8_header_string(
	          header_values,
	          LIBEWF_HEADER_STRING_TYPE_1,
	          (uint8_t *) "\n",
	          1,
	          1578288885,
	          0x7f,
	          &utf8_string,
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

	result = libewf_header_values_generate_utf8_header_string(
	          header_values,
	          LIBEWF_HEADER_STRING_TYPE_1,
	          (uint8_t *) "\n",
	          1,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
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

	result = libewf_header_values_generate_utf8_header_string(
	          header_values,
	          LIBEWF_HEADER_STRING_TYPE_1,
	          (uint8_t *) "\n",
	          1,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &utf8_string,
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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_generate_header_ewf function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_header_ewf(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header                  = NULL;
	char *expected_header            = "1\nmain\nc\tn\ta\te\tt\tm\tu\tp\tr\n\t\t\t\t\t2020 1 6 6 34 45\t2020 1 6 6 34 45\t0\tf\n\n";
	size_t header_size               = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_generate_header_ewf(
	          header_values,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header",
	 header );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "header_size",
	 header_size,
	 (size_t) 70 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          header,
	          expected_header,
	          70 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 header );

	header = NULL;

	/* Test error cases
	 */
	result = libewf_header_values_generate_header_ewf(
	          header_values,
	          1578288885,
	          0x7f,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header != NULL )
	{
		memory_free(
		 header );
	}
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_generate_header_encase1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_header_encase1(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header                  = NULL;
	char *expected_header            = "1\r\nmain\r\nc\tn\ta\te\tt\tm\tu\tp\tr\r\n\t\t\t\t\t2020 1 6 6 34 45\t2020 1 6 6 34 45\t0\tf\r\n\r\n";
	size_t header_size               = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_generate_header_encase1(
	          header_values,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header",
	 header );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "header_size",
	 header_size,
	 (size_t) 75 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          header,
	          expected_header,
	          75 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 header );

	header = NULL;

	/* Test error cases
	 */
	result = libewf_header_values_generate_header_encase1(
	          header_values,
	          1578288885,
	          0x7f,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header != NULL )
	{
		memory_free(
		 header );
	}
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_generate_header_ftk function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_header_ftk(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header                  = NULL;
	char *expected_header            = "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\n\t\t\t\t\t\t\t2020 1 6 6 34 45\t2020 1 6 6 34 45\t0\tf\n\n";
	size_t header_size               = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_generate_header_ftk(
	          header_values,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header",
	 header );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "header_size",
	 header_size,
	 (size_t) 78 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          header,
	          expected_header,
	          78 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 header );

	header = NULL;

	/* Test error cases
	 */
	result = libewf_header_values_generate_header_ftk(
	          header_values,
	          1578288885,
	          0x7f,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header != NULL )
	{
		memory_free(
		 header );
	}
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_generate_header_encase2 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_header_encase2(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header                  = NULL;
	char *expected_header            = "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\r\n\t\t\t\t\t\t\t2020 1 6 6 34 45\t2020 1 6 6 34 45\t0\tf\r\n\r\n";
	size_t header_size               = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_generate_header_encase2(
	          header_values,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header",
	 header );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "header_size",
	 header_size,
	 (size_t) 83 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          header,
	          expected_header,
	          78 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 header );

	header = NULL;

	/* Test error cases
	 */
	result = libewf_header_values_generate_header_encase2(
	          header_values,
	          1578288885,
	          0x7f,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header != NULL )
	{
		memory_free(
		 header );
	}
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_generate_header_encase4 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_header_encase4(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header                  = NULL;
	char *expected_header            = "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\r\n\t\t\t\t\t\t\t2020 1 6 6 34 45\t2020 1 6 6 34 45\t0\r\n\r\n";
	size_t header_size               = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_generate_header_encase4(
	          header_values,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header",
	 header );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "header_size",
	 header_size,
	 (size_t) 79 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          header,
	          expected_header,
	          79 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 header );

	header = NULL;

	/* Test error cases
	 */
	result = libewf_header_values_generate_header_encase4(
	          header_values,
	          1578288885,
	          0x7f,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header != NULL )
	{
		memory_free(
		 header );
	}
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_generate_header_linen function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_header_linen(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header                  = NULL;
	char *expected_header            = "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n\t\t\t\t\t\t\t1578288885\t1578288885\t0\n\nsrce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\nsub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n";
	size_t header_size               = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_generate_header_linen(
	          header_values,
	          LIBEWF_FORMAT_LINEN5,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header",
	 header );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "header_size",
	 header_size,
	 (size_t) 154 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          header,
	          expected_header,
	          154 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 header );

	header = NULL;

	/* Test error cases
	 */
	result = libewf_header_values_generate_header_linen(
	          header_values,
	          LIBEWF_FORMAT_LINEN5,
	          1578288885,
	          0x7f,
	          &header,
	          &header_size,
	          LIBEWF_CODEPAGE_ASCII,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header != NULL )
	{
		memory_free(
		 header );
	}
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_header_values_generate_header2 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_generate_header2(
     void )
{
	uint8_t expected_header2[ 126 ] = {
		0xff, 0xfe, '1', 0, '\n', 0, 'm', 0, 'a', 0, 'i', 0, 'n', 0, '\n', 0,
		'a', 0, '\t', 0, 'c', 0, '\t', 0, 'n', 0, '\t', 0, 'e', 0, '\t', 0,
		't', 0, '\t', 0, 'a', 0, 'v', 0, '\t', 0, 'o', 0, 'v', 0, '\t', 0,
		'm', 0, '\t', 0, 'u', 0, '\t', 0, 'p', 0, '\n', 0, '\t', 0, '\t', 0,
		'\t', 0, '\t', 0, '\t', 0, '\t', 0, '\t', 0, '1', 0, '5', 0, '7', 0,
		'8', 0, '2', 0, '8', 0, '8', 0, '8', 0, '8', 0, '5', 0, '\t', 0,
		'1', 0, '5', 0, '7', 0, '8', 0, '2', 0, '8', 0, '8', 0, '8', 0,
		'8', 0, '5', 0, '\t', 0, '0', 0, '\n', 0, '\n', 0, 0, 0 };

	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	uint8_t *header2                 = NULL;
	size_t header2_size              = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_generate_header2(
	          header_values,
	          LIBEWF_FORMAT_ENCASE4,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header2,
	          &header2_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header2",
	 header2 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "header2_size",
	 header2_size,
	 (size_t) 126 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          header2,
	          expected_header2,
	          126 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 header2 );

	header2 = NULL;

	/* Test error cases
	 */
	result = libewf_header_values_generate_header2(
	          header_values,
	          0xff,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header2,
	          &header2_size,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_header_values_set_value_by_index with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_header_values_generate_header2(
	          header_values,
	          LIBEWF_FORMAT_ENCASE4,
	          1578288885,
	          LIBEWF_COMPRESSION_FAST,
	          &header2,
	          &header2_size,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( header2 != NULL )
		{
			memory_free(
			 header2 );

			header2 = NULL;
		}
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( header2 != NULL )
	{
		memory_free(
		 header2 );
	}
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_convert_date_xheader_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_convert_date_xheader_value(
     void )
{
	libcerror_error_t *error            = NULL;
	uint8_t *date_time_values_string    = NULL;
	size_t date_time_values_string_size = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_convert_date_xheader_value(
	          (uint8_t *) "Sat Jan 20 18:32:08 2007 CET",
	          29,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "date_time_values_string",
	 date_time_values_string );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "date_time_values_string_size",
	 date_time_values_string_size,
	 (size_t) 24 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          date_time_values_string,
	          "2007 01 20 18 32 08 CET",
	          24 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 date_time_values_string );

	date_time_values_string = NULL;

	result = libewf_convert_date_xheader_value(
	          (uint8_t *) "Sat Jan 20 18:32:08",
	          20,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "date_time_values_string",
	 date_time_values_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_convert_date_xheader_value(
	          (uint8_t *) "Sat Jan 20 18:32:08 2007 CET",
	          29,
	          NULL,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_convert_date_xheader_value(
	          (uint8_t *) "Sat Jan 20 18:32:08 2007 CET",
	          29,
	          &date_time_values_string,
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

	/* Test libewf_convert_date_xheader_value with failing libfvalue_utf8_string_split
	 */
	result = libewf_convert_date_xheader_value(
	          NULL,
	          29,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

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

	/* Test libewf_convert_date_xheader_value with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_convert_date_xheader_value(
	          (uint8_t *) "Sat Jan 20 18:32:08 2007 CET",
	          29,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( date_time_values_string != NULL )
		{
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );
	}
	return( 0 );
}

/* Tests the libewf_generate_date_xheader_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_generate_date_xheader_value(
     void )
{
	libcerror_error_t *error            = NULL;
	uint8_t *date_time_values_string    = NULL;
	size_t date_time_values_string_size = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_generate_date_xheader_value(
	          1578288885,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "date_time_values_string",
	 date_time_values_string );

	/* Note libewf_generate_date_xheader_value libewf_generate_date_header_value allocates a string of size 64 */

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "date_time_values_string_size",
	 date_time_values_string_size,
	 (size_t) 64 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          date_time_values_string,
	          "Mon Jan  6 06:34:45 2020 CET",
	          29 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 date_time_values_string );

	date_time_values_string = NULL;

	/* Test error cases
	 */
	result = libewf_generate_date_xheader_value(
	          1578288885,
	          NULL,
	          &date_time_values_string_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_generate_date_xheader_value(
	          1578288885,
	          &date_time_values_string,
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

	/* Test libewf_generate_date_xheader_value with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_generate_date_xheader_value(
	          1578288885,
	          &date_time_values_string,
	          &date_time_values_string_size,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( date_time_values_string != NULL )
		{
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );
	}
	return( 0 );
}

/* Tests the libewf_header_values_parse_xheader function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_header_values_parse_xheader(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *header_values = NULL;
	char *xheader                    = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<xheader>\n\t<case_number>1</case_number>\n\t<description>Description</description>\n\t<examiner_name>John D.</examiner_name>\n\t<evidence_number>1.1</evidence_number>\n\t<notes>Just a floppy in my system</notes>\n\t<acquiry_operating_system>Linux</acquiry_operating_system>\n\t<acquiry_date>Sat Jan 20 18:32:08 2007 CET</acquiry_date>\n\t<acquiry_software>ewfacquire</acquiry_software>\n\t<acquiry_software_version>20070120</acquiry_software_version>\n</xheader>\n";
	int result                       = 0;

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_header_values_parse_xheader(
	          header_values,
	          (uint8_t *) xheader,
	          481,
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
	result = libfvalue_table_free(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */

	/* Test libewf_header_values_parse_xheader with libfvalue_table_copy_from_utf8_xml_string failing
	 */
	result = libewf_header_values_parse_xheader(
	          NULL,
	          (uint8_t *) xheader,
	          481,
	          &error );

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
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
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
	 "libewf_header_values_initialize",
	 ewf_test_header_values_initialize );

	EWF_TEST_RUN(
	 "libewf_header_values_set_value_by_index",
	 ewf_test_header_values_set_value_by_index );

	EWF_TEST_RUN(
	 "libewf_convert_date_header_value",
	 ewf_test_convert_date_header_value );

	EWF_TEST_RUN(
	 "libewf_generate_date_header_value",
	 ewf_test_generate_date_header_value );

	EWF_TEST_RUN(
	 "libewf_convert_date_header2_value",
	 ewf_test_convert_date_header2_value );

	EWF_TEST_RUN(
	 "libewf_generate_date_header2_value",
	 ewf_test_generate_date_header2_value );

	EWF_TEST_RUN(
	 "libewf_header_values_copy",
	 ewf_test_header_values_copy );

	EWF_TEST_RUN(
	 "libewf_header_values_parse_utf8_header_string",
	 ewf_test_header_values_parse_utf8_header_string );

	EWF_TEST_RUN(
	 "libewf_header_values_parse_header",
	 ewf_test_header_values_parse_header );

	EWF_TEST_RUN(
	 "libewf_header_values_parse_header2",
	 ewf_test_header_values_parse_header2 );

	EWF_TEST_RUN(
	 "libewf_header_values_convert_utf8_header_string_to_header",
	 ewf_test_header_values_convert_utf8_header_string_to_header );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_utf8_header_string",
	 ewf_test_header_values_generate_utf8_header_string );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_header_ewf",
	 ewf_test_header_values_generate_header_ewf );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_header_encase1",
	 ewf_test_header_values_generate_header_encase1 );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_header_ftk",
	 ewf_test_header_values_generate_header_ftk );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_header_encase2",
	 ewf_test_header_values_generate_header_encase2 );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_header_encase4",
	 ewf_test_header_values_generate_header_encase4 );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_header_linen",
	 ewf_test_header_values_generate_header_linen );

	EWF_TEST_RUN(
	 "libewf_header_values_generate_header2",
	 ewf_test_header_values_generate_header2 );

	EWF_TEST_RUN(
	 "libewf_convert_date_xheader_value",
	 ewf_test_convert_date_xheader_value );

	EWF_TEST_RUN(
	 "libewf_generate_date_xheader_value",
	 ewf_test_generate_date_xheader_value );

	EWF_TEST_RUN(
	 "libewf_header_values_parse_xheader",
	 ewf_test_header_values_parse_xheader );

	/* TODO: add tests for libewf_header_values_parse_xheader_date_value */

	/* TODO: add tests for libewf_header_values_generate_xheader */

	/* TODO: add tests for libewf_header_values_generate_header_ewfx */

	/* TODO: add tests for libewf_header_values_get_identifier_size */

	/* TODO: add tests for libewf_header_values_get_identifier */

	/* TODO: add tests for libewf_header_values_get_utf8_value_size */

	/* TODO: add tests for libewf_header_values_get_utf8_value */

	/* TODO: add tests for libewf_header_values_set_utf8_value */

	/* TODO: add tests for libewf_header_values_get_utf16_value_size */

	/* TODO: add tests for libewf_header_values_get_utf16_value */

	/* TODO: add tests for libewf_header_values_set_utf16_value */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


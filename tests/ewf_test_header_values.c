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
	int number_of_malloc_fail_tests  = 18;
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
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_convert_date_header_value(
	          NULL,
	          0,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_convert_date_header2_value(
	          NULL,
	          0,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
	result = libewf_header_values_parse_header(
	          header_values,
	          NULL,
	          0,
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
/* TODO implement */

	/* Test error cases
	 */
	result = libewf_header_values_parse_header2(
	          header_values,
	          NULL,
	          0,
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

/* TODO compare header against expected value */

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

/* TODO compare header against expected value */

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

/* TODO compare header against expected value */

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

/* TODO compare header against expected value */

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

/* TODO compare header against expected value */

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

/* TODO compare header against expected value */

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

/* TODO compare header2 against expected value */

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

	/* TODO: add tests for libewf_convert_date_xheader_value */

	/* TODO: add tests for libewf_generate_date_xheader_value */

	/* TODO: add tests for libewf_header_values_parse_xheader */

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


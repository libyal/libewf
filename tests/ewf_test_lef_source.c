/*
 * Library lef_source type test program
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
#include "ewf_test_libfvalue.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_lef_source.h"

uint8_t ewf_test_lef_source_types_data1[ 28 ] = {
	0x70, 0x09, 0x6e, 0x09, 0x69, 0x64, 0x09, 0x65, 0x76, 0x09, 0x74, 0x62, 0x09, 0x6c, 0x6f, 0x09,
	0x70, 0x6f, 0x09, 0x61, 0x68, 0x09, 0x67, 0x75, 0x09, 0x61, 0x71, 0x0d };

uint8_t ewf_test_lef_source_values_data1[ 36 ] = {
	0x09, 0x45, 0x09, 0x31, 0x09, 0x45, 0x09, 0x32, 0x36, 0x31, 0x38, 0x39, 0x30, 0x30, 0x34, 0x38,
	0x09, 0x2d, 0x31, 0x09, 0x2d, 0x31, 0x09, 0x09, 0x09, 0x31, 0x31, 0x34, 0x32, 0x36, 0x38, 0x33,
	0x36, 0x32, 0x34, 0x0a };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_lef_source_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libewf_lef_source_t *lef_source = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 14;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_lef_source_initialize(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_source",
	 lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_free(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_source",
	 lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_source_initialize(
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

	lef_source = (libewf_lef_source_t *) 0x12345678UL;

	result = libewf_lef_source_initialize(
	          &lef_source,
	          &error );

	lef_source = NULL;

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
		/* Test libewf_lef_source_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_source_initialize(
		          &lef_source,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( lef_source != NULL )
			{
				libewf_lef_source_free(
				 &lef_source,
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
			 "lef_source",
			 lef_source );

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
		/* Test libewf_lef_source_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_lef_source_initialize(
		          &lef_source,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( lef_source != NULL )
			{
				libewf_lef_source_free(
				 &lef_source,
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
			 "lef_source",
			 lef_source );

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
	if( lef_source != NULL )
	{
		libewf_lef_source_free(
		 &lef_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_lef_source_free(
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

/* Tests the libewf_lef_source_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_clone(
     void )
{
	libcerror_error_t *error                    = NULL;
	libewf_lef_source_t *destination_lef_source = NULL;
	libewf_lef_source_t *source_lef_source      = NULL;
	libfvalue_split_utf8_string_t *types        = NULL;
	int result                                  = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 16;
	int test_number                             = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests             = 1;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libewf_lef_source_initialize(
	          &source_lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_lef_source",
	 source_lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_source_types_data1,
	          28,
	          (uint8_t) '\t',
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_read_data(
	          source_lef_source,
	          types,
	          ewf_test_lef_source_values_data1,
	          36,
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
	result = libewf_lef_source_clone(
	          &destination_lef_source,
	          source_lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_lef_source",
	 destination_lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_free(
	          &destination_lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_source",
	 destination_lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_clone(
	          &destination_lef_source,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_source",
	 destination_lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_source_clone(
	          NULL,
	          source_lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	destination_lef_source = (libewf_lef_source_t *) 0x12345678UL;

	result = libewf_lef_source_clone(
	          &destination_lef_source,
	          source_lef_source,
	          &error );

	destination_lef_source = NULL;

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
		/* Test libewf_lef_source_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_source_clone(
		          &destination_lef_source,
		          source_lef_source,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_lef_source != NULL )
			{
				libewf_lef_source_free(
				 &destination_lef_source,
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
			 "destination_lef_source",
			 destination_lef_source );

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
		/* Test libewf_lef_source_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_lef_source_clone(
		          &destination_lef_source,
		          source_lef_source,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_lef_source != NULL )
			{
				libewf_lef_source_free(
				 &destination_lef_source,
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
			 "destination_lef_source",
			 destination_lef_source );

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
	result = libfvalue_split_utf8_string_free(
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_free(
	          &source_lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_lef_source",
	 source_lef_source );

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
	if( destination_lef_source != NULL )
	{
		libewf_lef_source_free(
		 &destination_lef_source,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( source_lef_source != NULL )
	{
		libewf_lef_source_free(
		 &source_lef_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_read_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_read_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libewf_lef_source_t *lef_source      = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	int result                           = 0;

	/* Initialize test
	 */
	result = libewf_lef_source_initialize(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_source",
	 lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_source_types_data1,
	          28,
	          (uint8_t) '\t',
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_lef_source_read_data(
	          lef_source,
	          types,
	          ewf_test_lef_source_values_data1,
	          36,
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
	result = libewf_lef_source_free(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_source",
	 lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_lef_source_initialize(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_source",
	 lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_source_read_data(
	          NULL,
	          types,
	          ewf_test_lef_source_values_data1,
	          36,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_read_data(
	          lef_source,
	          NULL,
	          ewf_test_lef_source_values_data1,
	          36,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_read_data(
	          lef_source,
	          types,
	          NULL,
	          36,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_read_data(
	          lef_source,
	          types,
	          ewf_test_lef_source_values_data1,
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

	result = libewf_lef_source_read_data(
	          lef_source,
	          types,
	          ewf_test_lef_source_values_data1,
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

	/* Test number_of_types != number_of_values
	 */
	result = libewf_lef_source_read_data(
	          lef_source,
	          types,
	          ewf_test_lef_source_values_data1,
	          36 - 12,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* TODO: test with invalid data */

	/* Clean up
	 */
	result = libfvalue_split_utf8_string_free(
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_free(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_source",
	 lef_source );

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
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lef_source != NULL )
	{
		libewf_lef_source_free(
		 &lef_source,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_identifier(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	int identifier           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_identifier(
	          lef_source,
	          &identifier,
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
	result = libewf_lef_source_get_identifier(
	          NULL,
	          &identifier,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_get_identifier(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_name_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_name_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_name_size(
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

	result = libewf_lef_source_get_utf8_name_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_name(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_name(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_name(
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

	result = libewf_lef_source_get_utf8_name(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_name(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_name(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_name_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_name_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_name_size(
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

	result = libewf_lef_source_get_utf16_name_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_name(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_name(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_name(
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

	result = libewf_lef_source_get_utf16_name(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_name(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_name(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_evidence_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_evidence_number_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_evidence_number_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_evidence_number_size(
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

	result = libewf_lef_source_get_utf8_evidence_number_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_evidence_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_evidence_number(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_evidence_number(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_evidence_number(
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

	result = libewf_lef_source_get_utf8_evidence_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_evidence_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_evidence_number(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_evidence_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_evidence_number_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_evidence_number_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_evidence_number_size(
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

	result = libewf_lef_source_get_utf16_evidence_number_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_evidence_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_evidence_number(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_evidence_number(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_evidence_number(
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

	result = libewf_lef_source_get_utf16_evidence_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_evidence_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_evidence_number(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_location_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_location_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_location_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_location_size(
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

	result = libewf_lef_source_get_utf8_location_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_location function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_location(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_location(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_location(
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

	result = libewf_lef_source_get_utf8_location(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_location(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_location(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_location_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_location_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_location_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_location_size(
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

	result = libewf_lef_source_get_utf16_location_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_location function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_location(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_location(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_location(
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

	result = libewf_lef_source_get_utf16_location(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_location(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_location(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_device_guid_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_device_guid_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_device_guid_size(
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

	result = libewf_lef_source_get_utf8_device_guid_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_device_guid(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_device_guid(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_device_guid(
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

	result = libewf_lef_source_get_utf8_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_device_guid(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_device_guid_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_device_guid_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_device_guid_size(
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

	result = libewf_lef_source_get_utf16_device_guid_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_device_guid(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_device_guid(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_device_guid(
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

	result = libewf_lef_source_get_utf16_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_device_guid(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_primary_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_primary_device_guid_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_primary_device_guid_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_primary_device_guid_size(
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

	result = libewf_lef_source_get_utf8_primary_device_guid_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_primary_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_primary_device_guid(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_primary_device_guid(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_primary_device_guid(
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

	result = libewf_lef_source_get_utf8_primary_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_primary_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_primary_device_guid(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_primary_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_primary_device_guid_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_primary_device_guid_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_primary_device_guid_size(
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

	result = libewf_lef_source_get_utf16_primary_device_guid_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_primary_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_primary_device_guid(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_primary_device_guid(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_primary_device_guid(
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

	result = libewf_lef_source_get_utf16_primary_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_primary_device_guid(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_primary_device_guid(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_drive_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_drive_type(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	uint8_t drive_type       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_drive_type(
	          lef_source,
	          &drive_type,
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
	result = libewf_lef_source_get_drive_type(
	          NULL,
	          &drive_type,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_get_drive_type(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_manufacturer_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_manufacturer_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_manufacturer_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_manufacturer_size(
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

	result = libewf_lef_source_get_utf8_manufacturer_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_manufacturer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_manufacturer(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_manufacturer(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_manufacturer(
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

	result = libewf_lef_source_get_utf8_manufacturer(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_manufacturer(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_manufacturer(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_manufacturer_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_manufacturer_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_manufacturer_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_manufacturer_size(
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

	result = libewf_lef_source_get_utf16_manufacturer_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_manufacturer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_manufacturer(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_manufacturer(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_manufacturer(
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

	result = libewf_lef_source_get_utf16_manufacturer(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_manufacturer(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_manufacturer(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_model_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_model_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_model_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_model_size(
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

	result = libewf_lef_source_get_utf8_model_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_model function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_model(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_model(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_model(
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

	result = libewf_lef_source_get_utf8_model(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_model(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_model(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_model_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_model_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_model_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_model_size(
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

	result = libewf_lef_source_get_utf16_model_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_model function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_model(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_model(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_model(
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

	result = libewf_lef_source_get_utf16_model(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_model(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_model(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_serial_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_serial_number_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_serial_number_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_serial_number_size(
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

	result = libewf_lef_source_get_utf8_serial_number_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_serial_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_serial_number(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_serial_number(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_serial_number(
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

	result = libewf_lef_source_get_utf8_serial_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_serial_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_serial_number(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_serial_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_serial_number_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_serial_number_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_serial_number_size(
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

	result = libewf_lef_source_get_utf16_serial_number_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_serial_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_serial_number(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_serial_number(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_serial_number(
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

	result = libewf_lef_source_get_utf16_serial_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_serial_number(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_serial_number(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_domain_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_domain_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_domain_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_domain_size(
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

	result = libewf_lef_source_get_utf8_domain_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_domain function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_domain(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_domain(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_domain(
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

	result = libewf_lef_source_get_utf8_domain(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_domain(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_domain(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_domain_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_domain_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_domain_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_domain_size(
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

	result = libewf_lef_source_get_utf16_domain_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_domain function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_domain(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_domain(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_domain(
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

	result = libewf_lef_source_get_utf16_domain(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_domain(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_domain(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_ip_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_ip_address_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_ip_address_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_ip_address_size(
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

	result = libewf_lef_source_get_utf8_ip_address_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_ip_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_ip_address(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_ip_address(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_ip_address(
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

	result = libewf_lef_source_get_utf8_ip_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_ip_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_ip_address(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_ip_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_ip_address_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_ip_address_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_ip_address_size(
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

	result = libewf_lef_source_get_utf16_ip_address_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_ip_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_ip_address(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_ip_address(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_ip_address(
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

	result = libewf_lef_source_get_utf16_ip_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_ip_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_ip_address(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf8_mac_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_mac_address_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_mac_address_size(
	          lef_source,
	          &utf8_string_size,
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
	result = libewf_lef_source_get_utf8_mac_address_size(
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

	result = libewf_lef_source_get_utf8_mac_address_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_mac_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_mac_address(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_mac_address(
	          lef_source,
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
	result = libewf_lef_source_get_utf8_mac_address(
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

	result = libewf_lef_source_get_utf8_mac_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_mac_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf8_mac_address(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_utf16_mac_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_mac_address_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_mac_address_size(
	          lef_source,
	          &utf16_string_size,
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
	result = libewf_lef_source_get_utf16_mac_address_size(
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

	result = libewf_lef_source_get_utf16_mac_address_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf16_mac_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_mac_address(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_mac_address(
	          lef_source,
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
	result = libewf_lef_source_get_utf16_mac_address(
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

	result = libewf_lef_source_get_utf16_mac_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_mac_address(
	          lef_source,
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

	result = libewf_lef_source_get_utf16_mac_address(
	          lef_source,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_lef_source_get_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_size(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_size(
	          lef_source,
	          &size,
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
	result = libewf_lef_source_get_size(
	          NULL,
	          &size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_get_size(
	          lef_source,
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

/* Tests the libewf_lef_source_get_logical_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_logical_offset(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	off64_t logical_offset   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	lef_source->logical_offset = 1;

	result = libewf_lef_source_get_logical_offset(
	          lef_source,
	          &logical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	lef_source->logical_offset = -1;

	result = libewf_lef_source_get_logical_offset(
	          lef_source,
	          &logical_offset,
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
	result = libewf_lef_source_get_logical_offset(
	          NULL,
	          &logical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_get_logical_offset(
	          lef_source,
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

/* Tests the libewf_lef_source_get_physical_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_physical_offset(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	off64_t physical_offset  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	lef_source->physical_offset = 1;

	result = libewf_lef_source_get_physical_offset(
	          lef_source,
	          &physical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	lef_source->physical_offset = -1;

	result = libewf_lef_source_get_physical_offset(
	          lef_source,
	          &physical_offset,
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
	result = libewf_lef_source_get_physical_offset(
	          NULL,
	          &physical_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_get_physical_offset(
	          lef_source,
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

/* Tests the libewf_lef_source_get_acquisition_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_acquisition_time(
     libewf_lef_source_t *lef_source )
{
	libcerror_error_t *error = NULL;
	int64_t acquisition_time = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_acquisition_time(
	          lef_source,
	          &acquisition_time,
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
	result = libewf_lef_source_get_acquisition_time(
	          NULL,
	          &acquisition_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_source_get_acquisition_time(
	          lef_source,
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

/* Tests the libewf_lef_source_get_utf8_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_hash_value_md5(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_hash_value_md5[ 64 ];

	libcerror_error_t *error       = NULL;
	int result                     = 0;
	int utf8_hash_value_md5_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_hash_value_md5(
	          lef_source,
	          utf8_hash_value_md5,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_hash_value_md5_is_set = result;

	/* Test error cases
	 */
	result = libewf_lef_source_get_utf8_hash_value_md5(
	          NULL,
	          utf8_hash_value_md5,
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

	if( utf8_hash_value_md5_is_set != 0 )
	{
		result = libewf_lef_source_get_utf8_hash_value_md5(
		          lef_source,
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

		result = libewf_lef_source_get_utf8_hash_value_md5(
		          lef_source,
		          utf8_hash_value_md5,
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

		result = libewf_lef_source_get_utf8_hash_value_md5(
		          lef_source,
		          utf8_hash_value_md5,
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

/* Tests the libewf_lef_source_get_utf16_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_hash_value_md5(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_hash_value_md5[ 64 ];

	libcerror_error_t *error        = NULL;
	int result                      = 0;
	int utf16_hash_value_md5_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_hash_value_md5(
	          lef_source,
	          utf16_hash_value_md5,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_hash_value_md5_is_set = result;

	/* Test error cases
	 */
	result = libewf_lef_source_get_utf16_hash_value_md5(
	          NULL,
	          utf16_hash_value_md5,
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

	if( utf16_hash_value_md5_is_set != 0 )
	{
		result = libewf_lef_source_get_utf16_hash_value_md5(
		          lef_source,
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

		result = libewf_lef_source_get_utf16_hash_value_md5(
		          lef_source,
		          utf16_hash_value_md5,
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

		result = libewf_lef_source_get_utf16_hash_value_md5(
		          lef_source,
		          utf16_hash_value_md5,
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

/* Tests the libewf_lef_source_get_utf8_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf8_hash_value_sha1(
     libewf_lef_source_t *lef_source )
{
	uint8_t utf8_hash_value_sha1[ 64 ];

	libcerror_error_t *error        = NULL;
	int result                      = 0;
	int utf8_hash_value_sha1_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf8_hash_value_sha1(
	          lef_source,
	          utf8_hash_value_sha1,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_hash_value_sha1_is_set = result;

	/* Test error cases
	 */
	result = libewf_lef_source_get_utf8_hash_value_sha1(
	          NULL,
	          utf8_hash_value_sha1,
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

	if( utf8_hash_value_sha1_is_set != 0 )
	{
		result = libewf_lef_source_get_utf8_hash_value_sha1(
		          lef_source,
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

		result = libewf_lef_source_get_utf8_hash_value_sha1(
		          lef_source,
		          utf8_hash_value_sha1,
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

		result = libewf_lef_source_get_utf8_hash_value_sha1(
		          lef_source,
		          utf8_hash_value_sha1,
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

/* Tests the libewf_lef_source_get_utf16_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_source_get_utf16_hash_value_sha1(
     libewf_lef_source_t *lef_source )
{
	uint16_t utf16_hash_value_sha1[ 64 ];

	libcerror_error_t *error         = NULL;
	int result                       = 0;
	int utf16_hash_value_sha1_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_lef_source_get_utf16_hash_value_sha1(
	          lef_source,
	          utf16_hash_value_sha1,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_hash_value_sha1_is_set = result;

	/* Test error cases
	 */
	result = libewf_lef_source_get_utf16_hash_value_sha1(
	          NULL,
	          utf16_hash_value_sha1,
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

	if( utf16_hash_value_sha1_is_set != 0 )
	{
		result = libewf_lef_source_get_utf16_hash_value_sha1(
		          lef_source,
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

		result = libewf_lef_source_get_utf16_hash_value_sha1(
		          lef_source,
		          utf16_hash_value_sha1,
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

		result = libewf_lef_source_get_utf16_hash_value_sha1(
		          lef_source,
		          utf16_hash_value_sha1,
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

	libcerror_error_t *error             = NULL;
	libewf_lef_source_t *lef_source      = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	int result                           = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_lef_source_initialize",
	 ewf_test_lef_source_initialize );

	EWF_TEST_RUN(
	 "libewf_lef_source_free",
	 ewf_test_lef_source_free );

	EWF_TEST_RUN(
	 "libewf_lef_source_clone",
	 ewf_test_lef_source_clone );

	EWF_TEST_RUN(
	 "libewf_lef_source_read_data",
	 ewf_test_lef_source_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize lef_source for tests
	 */
	result = libewf_lef_source_initialize(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_source",
	 lef_source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_source_types_data1,
	          28,
	          (uint8_t) '\t',
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_read_data(
	          lef_source,
	          types,
	          ewf_test_lef_source_values_data1,
	          36,
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
	 "libewf_lef_source_get_identifier",
	 ewf_test_lef_source_get_identifier,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_name_size",
	 ewf_test_lef_source_get_utf8_name_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_name",
	 ewf_test_lef_source_get_utf8_name,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_name_size",
	 ewf_test_lef_source_get_utf16_name_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_name",
	 ewf_test_lef_source_get_utf16_name,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_evidence_number_size",
	 ewf_test_lef_source_get_utf8_evidence_number_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_evidence_number",
	 ewf_test_lef_source_get_utf8_evidence_number,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_evidence_number_size",
	 ewf_test_lef_source_get_utf16_evidence_number_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_evidence_number",
	 ewf_test_lef_source_get_utf16_evidence_number,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_location_size",
	 ewf_test_lef_source_get_utf8_location_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_location",
	 ewf_test_lef_source_get_utf8_location,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_location_size",
	 ewf_test_lef_source_get_utf16_location_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_location",
	 ewf_test_lef_source_get_utf16_location,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_device_guid_size",
	 ewf_test_lef_source_get_utf8_device_guid_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_device_guid",
	 ewf_test_lef_source_get_utf8_device_guid,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_device_guid_size",
	 ewf_test_lef_source_get_utf16_device_guid_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_device_guid",
	 ewf_test_lef_source_get_utf16_device_guid,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_primary_device_guid_size",
	 ewf_test_lef_source_get_utf8_primary_device_guid_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_primary_device_guid",
	 ewf_test_lef_source_get_utf8_primary_device_guid,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_primary_device_guid_size",
	 ewf_test_lef_source_get_utf16_primary_device_guid_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_primary_device_guid",
	 ewf_test_lef_source_get_utf16_primary_device_guid,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_drive_type",
	 ewf_test_lef_source_get_drive_type,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_manufacturer_size",
	 ewf_test_lef_source_get_utf8_manufacturer_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_manufacturer",
	 ewf_test_lef_source_get_utf8_manufacturer,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_manufacturer_size",
	 ewf_test_lef_source_get_utf16_manufacturer_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_manufacturer",
	 ewf_test_lef_source_get_utf16_manufacturer,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_model_size",
	 ewf_test_lef_source_get_utf8_model_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_model",
	 ewf_test_lef_source_get_utf8_model,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_model_size",
	 ewf_test_lef_source_get_utf16_model_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_model",
	 ewf_test_lef_source_get_utf16_model,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_serial_number_size",
	 ewf_test_lef_source_get_utf8_serial_number_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_serial_number",
	 ewf_test_lef_source_get_utf8_serial_number,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_serial_number_size",
	 ewf_test_lef_source_get_utf16_serial_number_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_serial_number",
	 ewf_test_lef_source_get_utf16_serial_number,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_domain_size",
	 ewf_test_lef_source_get_utf8_domain_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_domain",
	 ewf_test_lef_source_get_utf8_domain,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_domain_size",
	 ewf_test_lef_source_get_utf16_domain_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_domain",
	 ewf_test_lef_source_get_utf16_domain,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_ip_address_size",
	 ewf_test_lef_source_get_utf8_ip_address_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_ip_address",
	 ewf_test_lef_source_get_utf8_ip_address,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_ip_address_size",
	 ewf_test_lef_source_get_utf16_ip_address_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_ip_address",
	 ewf_test_lef_source_get_utf16_ip_address,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_mac_address_size",
	 ewf_test_lef_source_get_utf8_mac_address_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_mac_address",
	 ewf_test_lef_source_get_utf8_mac_address,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_mac_address_size",
	 ewf_test_lef_source_get_utf16_mac_address_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_mac_address",
	 ewf_test_lef_source_get_utf16_mac_address,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_size",
	 ewf_test_lef_source_get_size,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_logical_offset",
	 ewf_test_lef_source_get_logical_offset,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_physical_offset",
	 ewf_test_lef_source_get_physical_offset,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_acquisition_time",
	 ewf_test_lef_source_get_acquisition_time,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_hash_value_md5",
	 ewf_test_lef_source_get_utf8_hash_value_md5,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_hash_value_md5",
	 ewf_test_lef_source_get_utf16_hash_value_md5,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf8_hash_value_sha1",
	 ewf_test_lef_source_get_utf8_hash_value_sha1,
	 lef_source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_source_get_utf16_hash_value_sha1",
	 ewf_test_lef_source_get_utf16_hash_value_sha1,
	 lef_source );

	/* Clean up
	 */
	result = libfvalue_split_utf8_string_free(
	          &types,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "types",
	 types );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_source_free(
	          &lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_source",
	 lef_source );

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
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lef_source != NULL )
	{
		libewf_lef_source_free(
		 &lef_source,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


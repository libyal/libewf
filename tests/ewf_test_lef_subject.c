/*
 * Library lef_subject type test program
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

#include "../libewf/libewf_lef_subject.h"

uint8_t ewf_test_lef_subject_types_data1[ 64 ] = {
	0x70, 0x09, 0x6e, 0x09, 0x69, 0x64, 0x09, 0x65, 0x76, 0x09, 0x64, 0x6f, 0x09, 0x6c, 0x6f, 0x63,
	0x09, 0x73, 0x65, 0x09, 0x6d, 0x66, 0x72, 0x09, 0x6d, 0x6f, 0x09, 0x74, 0x62, 0x09, 0x6c, 0x6f,
	0x09, 0x70, 0x6f, 0x09, 0x61, 0x68, 0x09, 0x73, 0x68, 0x09, 0x67, 0x75, 0x09, 0x70, 0x67, 0x75,
	0x09, 0x61, 0x71, 0x09, 0x69, 0x70, 0x09, 0x73, 0x69, 0x09, 0x6d, 0x61, 0x09, 0x64, 0x74, 0x0d };

uint8_t ewf_test_lef_subject_values_data1[ 26 ] = {
        0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x2d, 0x31, 0x09, 0x2d, 0x31,
        0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0d };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_lef_subject_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libewf_lef_subject_t *lef_subject = NULL;
	int result                        = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 2;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_lef_subject_initialize(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_subject",
	 lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_subject_free(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_subject",
	 lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_subject_initialize(
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

	lef_subject = (libewf_lef_subject_t *) 0x12345678UL;

	result = libewf_lef_subject_initialize(
	          &lef_subject,
	          &error );

	lef_subject = NULL;

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
		/* Test libewf_lef_subject_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_subject_initialize(
		          &lef_subject,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( lef_subject != NULL )
			{
				libewf_lef_subject_free(
				 &lef_subject,
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
			 "lef_subject",
			 lef_subject );

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
		/* Test libewf_lef_subject_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_lef_subject_initialize(
		          &lef_subject,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( lef_subject != NULL )
			{
				libewf_lef_subject_free(
				 &lef_subject,
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
			 "lef_subject",
			 lef_subject );

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
	if( lef_subject != NULL )
	{
		libewf_lef_subject_free(
		 &lef_subject,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_subject_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_lef_subject_free(
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

/* Tests the libewf_lef_subject_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_clone(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_lef_subject_t *destination_lef_subject = NULL;
	libewf_lef_subject_t *source_lef_subject      = NULL;
	libfvalue_split_utf8_string_t *types          = NULL;
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
	result = libewf_lef_subject_initialize(
	          &source_lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_lef_subject",
	 source_lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_subject_types_data1,
	          64,
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

	result = libewf_lef_subject_read_data(
	          source_lef_subject,
	          types,
	          ewf_test_lef_subject_values_data1,
	          26,
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
	result = libewf_lef_subject_clone(
	          &destination_lef_subject,
	          source_lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_lef_subject",
	 destination_lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_subject_free(
	          &destination_lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_subject",
	 destination_lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_subject_clone(
	          &destination_lef_subject,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_subject",
	 destination_lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_subject_clone(
	          NULL,
	          source_lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	destination_lef_subject = (libewf_lef_subject_t *) 0x12345678UL;

	result = libewf_lef_subject_clone(
	          &destination_lef_subject,
	          source_lef_subject,
	          &error );

	destination_lef_subject = NULL;

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
		/* Test libewf_lef_subject_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_subject_clone(
		          &destination_lef_subject,
		          source_lef_subject,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_lef_subject != NULL )
			{
				libewf_lef_subject_free(
				 &destination_lef_subject,
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
			 "destination_lef_subject",
			 destination_lef_subject );

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
		/* Test libewf_lef_subject_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_lef_subject_clone(
		          &destination_lef_subject,
		          source_lef_subject,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_lef_subject != NULL )
			{
				libewf_lef_subject_free(
				 &destination_lef_subject,
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
			 "destination_lef_subject",
			 destination_lef_subject );

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

	result = libewf_lef_subject_free(
	          &source_lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_lef_subject",
	 source_lef_subject );

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
	if( destination_lef_subject != NULL )
	{
		libewf_lef_subject_free(
		 &destination_lef_subject,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( source_lef_subject != NULL )
	{
		libewf_lef_subject_free(
		 &source_lef_subject,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_subject_read_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_read_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libewf_lef_subject_t *lef_subject    = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	int result                           = 0;

	/* Initialize test
	 */
	result = libewf_lef_subject_initialize(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_subject",
	 lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_subject_types_data1,
	          64,
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
	result = libewf_lef_subject_read_data(
	          lef_subject,
	          types,
	          ewf_test_lef_subject_values_data1,
	          26,
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
	result = libewf_lef_subject_free(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_subject",
	 lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_lef_subject_initialize(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_subject",
	 lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_subject_read_data(
	          NULL,
	          types,
	          ewf_test_lef_subject_values_data1,
	          26,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_subject_read_data(
	          lef_subject,
	          NULL,
	          ewf_test_lef_subject_values_data1,
	          26,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_subject_read_data(
	          lef_subject,
	          types,
	          NULL,
	          26,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_lef_subject_read_data(
	          lef_subject,
	          types,
	          ewf_test_lef_subject_values_data1,
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

	result = libewf_lef_subject_read_data(
	          lef_subject,
	          types,
	          ewf_test_lef_subject_values_data1,
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

	result = libewf_lef_subject_free(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_subject",
	 lef_subject );

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
	if( lef_subject != NULL )
	{
		libewf_lef_subject_free(
		 &lef_subject,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_subject_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_get_identifier(
     libewf_lef_subject_t *lef_subject )
{
	libcerror_error_t *error = NULL;
	uint32_t identifier      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_subject_get_identifier(
	          lef_subject,
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
	result = libewf_lef_subject_get_identifier(
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

	result = libewf_lef_subject_get_identifier(
	          lef_subject,
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

/* Tests the libewf_lef_subject_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_get_utf8_name_size(
     libewf_lef_subject_t *lef_subject )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_subject_get_utf8_name_size(
	          lef_subject,
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
	result = libewf_lef_subject_get_utf8_name_size(
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

	result = libewf_lef_subject_get_utf8_name_size(
	          lef_subject,
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

/* Tests the libewf_lef_subject_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_get_utf8_name(
     libewf_lef_subject_t *lef_subject )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_subject_get_utf8_name(
	          lef_subject,
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
	result = libewf_lef_subject_get_utf8_name(
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

	result = libewf_lef_subject_get_utf8_name(
	          lef_subject,
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

	result = libewf_lef_subject_get_utf8_name(
	          lef_subject,
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

	result = libewf_lef_subject_get_utf8_name(
	          lef_subject,
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

/* Tests the libewf_lef_subject_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_get_utf16_name_size(
     libewf_lef_subject_t *lef_subject )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_subject_get_utf16_name_size(
	          lef_subject,
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
	result = libewf_lef_subject_get_utf16_name_size(
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

	result = libewf_lef_subject_get_utf16_name_size(
	          lef_subject,
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

/* Tests the libewf_lef_subject_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_subject_get_utf16_name(
     libewf_lef_subject_t *lef_subject )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_lef_subject_get_utf16_name(
	          lef_subject,
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
	result = libewf_lef_subject_get_utf16_name(
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

	result = libewf_lef_subject_get_utf16_name(
	          lef_subject,
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

	result = libewf_lef_subject_get_utf16_name(
	          lef_subject,
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

	result = libewf_lef_subject_get_utf16_name(
	          lef_subject,
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
	libewf_lef_subject_t *lef_subject    = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	int result                           = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_lef_subject_initialize",
	 ewf_test_lef_subject_initialize );

	EWF_TEST_RUN(
	 "libewf_lef_subject_free",
	 ewf_test_lef_subject_free );

	EWF_TEST_RUN(
	 "libewf_lef_subject_clone",
	 ewf_test_lef_subject_clone );

	EWF_TEST_RUN(
	 "libewf_lef_subject_read_data",
	 ewf_test_lef_subject_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize lef_subject for tests
	 */
	result = libewf_lef_subject_initialize(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_subject",
	 lef_subject );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_lef_subject_types_data1,
	          64,
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

	result = libewf_lef_subject_read_data(
	          lef_subject,
	          types,
	          ewf_test_lef_subject_values_data1,
	          26,
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
	 "libewf_lef_subject_get_identifier",
	 ewf_test_lef_subject_get_identifier,
	 lef_subject );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_subject_get_utf8_name_size",
	 ewf_test_lef_subject_get_utf8_name_size,
	 lef_subject );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_subject_get_utf8_name",
	 ewf_test_lef_subject_get_utf8_name,
	 lef_subject );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_subject_get_utf16_name_size",
	 ewf_test_lef_subject_get_utf16_name_size,
	 lef_subject );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_subject_get_utf16_name",
	 ewf_test_lef_subject_get_utf16_name,
	 lef_subject );

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

	result = libewf_lef_subject_free(
	          &lef_subject,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_subject",
	 lef_subject );

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
	if( lef_subject != NULL )
	{
		libewf_lef_subject_free(
		 &lef_subject,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


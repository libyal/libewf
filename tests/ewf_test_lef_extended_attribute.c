/*
 * Library lef_extended_attribute type test program
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "../libewf/libewf_lef_extended_attribute.h"

uint8_t ewf_test_lef_extended_attribute_data1[ 123 ] = {
	0x00, 0x00, 0x00, 0x00, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x41, 0x00, 0x74,
	0x00, 0x74, 0x00, 0x72, 0x00, 0x69, 0x00, 0x62, 0x00, 0x75, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x05, 0x00,
	0x00, 0x00, 0x44, 0x00, 0x6f, 0x00, 0x4e, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x50, 0x00, 0x72, 0x00,
	0x6f, 0x00, 0x63, 0x00, 0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x00, 0x00, 0x74, 0x00, 0x72, 0x00,
	0x75, 0x00, 0x65, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x05,
	0x00, 0x00, 0x00, 0x48, 0x00, 0x69, 0x00, 0x64, 0x00, 0x64, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x00,
	0x00, 0x74, 0x00, 0x72, 0x00, 0x75, 0x00, 0x65, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_lef_extended_attribute_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_extended_attribute_initialize(
     void )
{
	libcerror_error_t *error                                = NULL;
	libewf_lef_extended_attribute_t *lef_extended_attribute = NULL;
	int result                                              = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                         = 1;
	int number_of_memset_fail_tests                         = 1;
	int test_number                                         = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_lef_extended_attribute_initialize(
	          &lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_extended_attribute",
	 lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_extended_attribute_free(
	          &lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_extended_attribute",
	 lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_extended_attribute_initialize(
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

	lef_extended_attribute = (libewf_lef_extended_attribute_t *) 0x12345678UL;

	result = libewf_lef_extended_attribute_initialize(
	          &lef_extended_attribute,
	          &error );

	lef_extended_attribute = NULL;

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
		/* Test libewf_lef_extended_attribute_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_extended_attribute_initialize(
		          &lef_extended_attribute,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( lef_extended_attribute != NULL )
			{
				libewf_lef_extended_attribute_free(
				 &lef_extended_attribute,
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
			 "lef_extended_attribute",
			 lef_extended_attribute );

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
		/* Test libewf_lef_extended_attribute_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_lef_extended_attribute_initialize(
		          &lef_extended_attribute,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( lef_extended_attribute != NULL )
			{
				libewf_lef_extended_attribute_free(
				 &lef_extended_attribute,
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
			 "lef_extended_attribute",
			 lef_extended_attribute );

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
	if( lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &lef_extended_attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_extended_attribute_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_extended_attribute_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_lef_extended_attribute_free(
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

/* Tests the libewf_lef_extended_attribute_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_extended_attribute_clone(
     void )
{
	libcerror_error_t *error                                            = NULL;
	libewf_lef_extended_attribute_t *destination_lef_extended_attribute = NULL;
	libewf_lef_extended_attribute_t *source_lef_extended_attribute      = NULL;
	int result                                                          = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                                     = 3;
	int test_number                                                     = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                                     = 2;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libewf_lef_extended_attribute_initialize(
	          &source_lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_lef_extended_attribute",
	 source_lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_lef_extended_attribute_clone(
	          &destination_lef_extended_attribute,
	          source_lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_lef_extended_attribute",
	 destination_lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_extended_attribute_free(
	          &destination_lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_extended_attribute",
	 destination_lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_lef_extended_attribute_clone(
	          &destination_lef_extended_attribute,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_lef_extended_attribute",
	 destination_lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_lef_extended_attribute_clone(
	          NULL,
	          source_lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	destination_lef_extended_attribute = (libewf_lef_extended_attribute_t *) 0x12345678UL;

	result = libewf_lef_extended_attribute_clone(
	          &destination_lef_extended_attribute,
	          source_lef_extended_attribute,
	          &error );

	destination_lef_extended_attribute = NULL;

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
		/* Test libewf_lef_extended_attribute_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_lef_extended_attribute_clone(
		          &destination_lef_extended_attribute,
		          source_lef_extended_attribute,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_lef_extended_attribute != NULL )
			{
				libewf_lef_extended_attribute_free(
				 &destination_lef_extended_attribute,
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
			 "destination_lef_extended_attribute",
			 destination_lef_extended_attribute );

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
		/* Test libewf_lef_extended_attribute_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_lef_extended_attribute_clone(
		          &destination_lef_extended_attribute,
		          source_lef_extended_attribute,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_lef_extended_attribute != NULL )
			{
				libewf_lef_extended_attribute_free(
				 &destination_lef_extended_attribute,
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
			 "destination_lef_extended_attribute",
			 destination_lef_extended_attribute );

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
	result = libewf_lef_extended_attribute_free(
	          &source_lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_lef_extended_attribute",
	 source_lef_extended_attribute );

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
	if( destination_lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &destination_lef_extended_attribute,
		 NULL );
	}
	if( source_lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &source_lef_extended_attribute,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_lef_extended_attribute_read_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_lef_extended_attribute_read_data(
     void )
{
	libcerror_error_t *error                                = NULL;
	libewf_lef_extended_attribute_t *lef_extended_attribute = NULL;
	ssize_t read_count                                      = 0;
	int result                                              = 0;

	/* Initialize test
	 */
	result = libewf_lef_extended_attribute_initialize(
	          &lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_extended_attribute",
	 lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_count = libewf_lef_extended_attribute_read_data(
	              lef_extended_attribute,
	              ewf_test_lef_extended_attribute_data1,
	              123,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 37 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libewf_lef_extended_attribute_read_data(
	              NULL,
	              ewf_test_lef_extended_attribute_data1,
	              123,
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

	read_count = libewf_lef_extended_attribute_read_data(
	              lef_extended_attribute,
	              NULL,
	              123,
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

	read_count = libewf_lef_extended_attribute_read_data(
	              lef_extended_attribute,
	              ewf_test_lef_extended_attribute_data1,
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

	read_count = libewf_lef_extended_attribute_read_data(
	              lef_extended_attribute,
	              ewf_test_lef_extended_attribute_data1,
	              0,
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

	/* Clean up
	 */
	result = libewf_lef_extended_attribute_free(
	          &lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_extended_attribute",
	 lef_extended_attribute );

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
	if( lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &lef_extended_attribute,
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
#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	libcerror_error_t *error                                = NULL;
	libewf_lef_extended_attribute_t *lef_extended_attribute = NULL;
	ssize_t read_count                                      = 0;
	int result                                              = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_lef_extended_attribute_initialize",
	 ewf_test_lef_extended_attribute_initialize );

	EWF_TEST_RUN(
	 "libewf_lef_extended_attribute_free",
	 ewf_test_lef_extended_attribute_free );

	EWF_TEST_RUN(
	 "libewf_lef_extended_attribute_clone",
	 ewf_test_lef_extended_attribute_clone );

	EWF_TEST_RUN(
	 "libewf_lef_extended_attribute_read_data",
	 ewf_test_lef_extended_attribute_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize lef_extended_attribute for tests
	 */
	result = libewf_lef_extended_attribute_initialize(
	          &lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_extended_attribute",
	 lef_extended_attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	read_count = libewf_lef_extended_attribute_read_data(
	              lef_extended_attribute,
	              ewf_test_lef_extended_attribute_data1,
	              123,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 37 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
/* TODO implement
	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_lef_extended_attribute_get_type",
	 ewf_test_lef_extended_attribute_get_type,
	 lef_extended_attribute );
*/

	/* Clean up
	 */
	result = libewf_lef_extended_attribute_free(
	          &lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_extended_attribute",
	 lef_extended_attribute );

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
	if( lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &lef_extended_attribute,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


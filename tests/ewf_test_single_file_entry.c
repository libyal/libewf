/*
 * Library single_file_entry type test program
 *
 * Copyright (C) 2006-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_single_file_entry.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_single_file_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_initialize(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	int result                                    = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int number_of_memset_fail_tests               = 1;
	int test_number                               = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_single_file_entry_initialize(
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

	single_file_entry = (libewf_single_file_entry_t *) 0x12345678UL;

	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	single_file_entry = NULL;

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_single_file_entry_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_single_file_entry_initialize(
		          &single_file_entry,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( single_file_entry != NULL )
			{
				libewf_single_file_entry_free(
				 &single_file_entry,
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
			 "single_file_entry",
			 single_file_entry );

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
		/* Test libewf_single_file_entry_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_single_file_entry_initialize(
		          &single_file_entry,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( single_file_entry != NULL )
			{
				libewf_single_file_entry_free(
				 &single_file_entry,
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
			 "single_file_entry",
			 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_single_file_entry_free(
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

/* Tests the libewf_single_file_entry_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_clone(
     void )
{
	libcerror_error_t *error                                  = NULL;
	libewf_single_file_entry_t *destination_single_file_entry = NULL;
	libewf_single_file_entry_t *source_single_file_entry      = NULL;
	int result                                                = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &source_single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_single_file_entry",
	 source_single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_clone(
	          &destination_single_file_entry,
	          source_single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_single_file_entry",
	 destination_single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_single_file_entry_free(
	          &destination_single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_single_file_entry",
	 destination_single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_single_file_entry_clone(
	          &destination_single_file_entry,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_single_file_entry",
	 destination_single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_single_file_entry_clone(
	          NULL,
	          source_single_file_entry,
	          &error );

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
	result = libewf_single_file_entry_free(
	          &source_single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_single_file_entry",
	 source_single_file_entry );

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
	if( destination_single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &destination_single_file_entry,
		 NULL );
	}
	if( source_single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &source_single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_type(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	uint8_t type                                  = 0;
	int result                                    = 0;
	int type_is_set                               = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_type(
	          single_file_entry,
	          &type,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	type_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_type(
	          NULL,
	          &type,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( type_is_set != 0 )
	{
		result = libewf_single_file_entry_get_type(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_flags function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_flags(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	uint32_t flags                                = 0;
	int flags_is_set                              = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_flags(
	          single_file_entry,
	          &flags,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	flags_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_flags(
	          NULL,
	          &flags,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( flags_is_set != 0 )
	{
		result = libewf_single_file_entry_get_flags(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_data_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_data_offset(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	off64_t data_offset                           = 0;
	int data_offset_is_set                        = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_data_offset(
	          single_file_entry,
	          &data_offset,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_offset_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_data_offset(
	          NULL,
	          &data_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( data_offset_is_set != 0 )
	{
		result = libewf_single_file_entry_get_data_offset(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_data_size(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	size64_t data_size                            = 0;
	int data_size_is_set                          = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_data_size(
	          single_file_entry,
	          &data_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_data_size(
	          NULL,
	          &data_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( data_size_is_set != 0 )
	{
		result = libewf_single_file_entry_get_data_size(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_duplicate_data_offset function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_duplicate_data_offset(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	off64_t duplicate_data_offset                 = 0;
	int duplicate_data_offset_is_set              = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_duplicate_data_offset(
	          single_file_entry,
	          &duplicate_data_offset,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	duplicate_data_offset_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_duplicate_data_offset(
	          NULL,
	          &duplicate_data_offset,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( duplicate_data_offset_is_set != 0 )
	{
		result = libewf_single_file_entry_get_duplicate_data_offset(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf8_name_size(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	size_t utf8_name_size                         = 0;
	int result                                    = 0;
	int utf8_name_size_is_set                     = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf8_name_size(
	          single_file_entry,
	          &utf8_name_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( utf8_name_size_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf8_name_size(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf8_name(
     void )
{
	uint8_t utf8_name[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	int result                                    = 0;
	int utf8_name_is_set                          = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf8_name(
	          single_file_entry,
	          utf8_name,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_name_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_utf8_name(
	          NULL,
	          utf8_name,
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

	if( utf8_name_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf8_name(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf8_name(
		          single_file_entry,
		          utf8_name,
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

		result = libewf_single_file_entry_get_utf8_name(
		          single_file_entry,
		          utf8_name,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf16_name_size(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	size_t utf16_name_size                        = 0;
	int result                                    = 0;
	int utf16_name_size_is_set                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf16_name_size(
	          single_file_entry,
	          &utf16_name_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( utf16_name_size_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf16_name_size(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf16_name(
     void )
{
	uint16_t utf16_name[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	int result                                    = 0;
	int utf16_name_is_set                         = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf16_name(
	          single_file_entry,
	          utf16_name,
	          512,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_name_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_utf16_name(
	          NULL,
	          utf16_name,
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

	if( utf16_name_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf16_name(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf16_name(
		          single_file_entry,
		          utf16_name,
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

		result = libewf_single_file_entry_get_utf16_name(
		          single_file_entry,
		          utf16_name,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_size(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	size64_t size                                 = 0;
	int result                                    = 0;
	int size_is_set                               = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_size(
	          single_file_entry,
	          &size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_size(
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

	if( size_is_set != 0 )
	{
		result = libewf_single_file_entry_get_size(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_creation_time(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	uint32_t creation_time                        = 0;
	int creation_time_is_set                      = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_creation_time(
	          single_file_entry,
	          &creation_time,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	creation_time_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_creation_time(
	          NULL,
	          &creation_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( creation_time_is_set != 0 )
	{
		result = libewf_single_file_entry_get_creation_time(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_modification_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_modification_time(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	uint32_t modification_time                    = 0;
	int modification_time_is_set                  = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_modification_time(
	          single_file_entry,
	          &modification_time,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	modification_time_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_modification_time(
	          NULL,
	          &modification_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( modification_time_is_set != 0 )
	{
		result = libewf_single_file_entry_get_modification_time(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_access_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_access_time(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	uint32_t access_time                          = 0;
	int access_time_is_set                        = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_access_time(
	          single_file_entry,
	          &access_time,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	access_time_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_access_time(
	          NULL,
	          &access_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( access_time_is_set != 0 )
	{
		result = libewf_single_file_entry_get_access_time(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_entry_modification_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_entry_modification_time(
     void )
{
	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	uint32_t entry_modification_time              = 0;
	int entry_modification_time_is_set            = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_entry_modification_time(
	          single_file_entry,
	          &entry_modification_time,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	entry_modification_time_is_set = result;

	/* Test error cases
	 */
	result = libewf_single_file_entry_get_entry_modification_time(
	          NULL,
	          &entry_modification_time,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( entry_modification_time_is_set != 0 )
	{
		result = libewf_single_file_entry_get_entry_modification_time(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf8_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf8_hash_value_md5(
     void )
{
	uint8_t utf8_hash_value_md5[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	int result                                    = 0;
	int utf8_hash_value_md5_is_set                = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf8_hash_value_md5(
	          single_file_entry,
	          utf8_hash_value_md5,
	          512,
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
	result = libewf_single_file_entry_get_utf8_hash_value_md5(
	          NULL,
	          utf8_hash_value_md5,
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

	if( utf8_hash_value_md5_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf8_hash_value_md5(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf8_hash_value_md5(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf8_hash_value_md5(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf16_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf16_hash_value_md5(
     void )
{
	uint16_t utf16_hash_value_md5[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	int result                                    = 0;
	int utf16_hash_value_md5_is_set               = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf16_hash_value_md5(
	          single_file_entry,
	          utf16_hash_value_md5,
	          512,
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
	result = libewf_single_file_entry_get_utf16_hash_value_md5(
	          NULL,
	          utf16_hash_value_md5,
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

	if( utf16_hash_value_md5_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf16_hash_value_md5(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf16_hash_value_md5(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf16_hash_value_md5(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf8_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf8_hash_value_sha1(
     void )
{
	uint8_t utf8_hash_value_sha1[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	int result                                    = 0;
	int utf8_hash_value_sha1_is_set               = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf8_hash_value_sha1(
	          single_file_entry,
	          utf8_hash_value_sha1,
	          512,
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
	result = libewf_single_file_entry_get_utf8_hash_value_sha1(
	          NULL,
	          utf8_hash_value_sha1,
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

	if( utf8_hash_value_sha1_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf8_hash_value_sha1(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf8_hash_value_sha1(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf8_hash_value_sha1(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_single_file_entry_get_utf16_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_single_file_entry_get_utf16_hash_value_sha1(
     void )
{
	uint16_t utf16_hash_value_sha1[ 512 ];

	libcerror_error_t *error                      = NULL;
	libewf_single_file_entry_t *single_file_entry = NULL;
	int result                                    = 0;
	int utf16_hash_value_sha1_is_set              = 0;

	/* Initialize test
	 */
	result = libewf_single_file_entry_initialize(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "single_file_entry",
	 single_file_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_single_file_entry_get_utf16_hash_value_sha1(
	          single_file_entry,
	          utf16_hash_value_sha1,
	          512,
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
	result = libewf_single_file_entry_get_utf16_hash_value_sha1(
	          NULL,
	          utf16_hash_value_sha1,
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

	if( utf16_hash_value_sha1_is_set != 0 )
	{
		result = libewf_single_file_entry_get_utf16_hash_value_sha1(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf16_hash_value_sha1(
		          single_file_entry,
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

		result = libewf_single_file_entry_get_utf16_hash_value_sha1(
		          single_file_entry,
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
	}
	/* Clean up
	 */
	result = libewf_single_file_entry_free(
	          &single_file_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "single_file_entry",
	 single_file_entry );

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
	if( single_file_entry != NULL )
	{
		libewf_single_file_entry_free(
		 &single_file_entry,
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
	 "libewf_single_file_entry_initialize",
	 ewf_test_single_file_entry_initialize );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_free",
	 ewf_test_single_file_entry_free );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_clone",
	 ewf_test_single_file_entry_clone );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_type",
	 ewf_test_single_file_entry_get_type );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_flags",
	 ewf_test_single_file_entry_get_flags );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_data_offset",
	 ewf_test_single_file_entry_get_data_offset );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_data_size",
	 ewf_test_single_file_entry_get_data_size );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_duplicate_data_offset",
	 ewf_test_single_file_entry_get_duplicate_data_offset );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf8_name_size",
	 ewf_test_single_file_entry_get_utf8_name_size );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf8_name",
	 ewf_test_single_file_entry_get_utf8_name );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf16_name_size",
	 ewf_test_single_file_entry_get_utf16_name_size );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf16_name",
	 ewf_test_single_file_entry_get_utf16_name );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_size",
	 ewf_test_single_file_entry_get_size );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_creation_time",
	 ewf_test_single_file_entry_get_creation_time );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_modification_time",
	 ewf_test_single_file_entry_get_modification_time );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_access_time",
	 ewf_test_single_file_entry_get_access_time );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_entry_modification_time",
	 ewf_test_single_file_entry_get_entry_modification_time );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf8_hash_value_md5",
	 ewf_test_single_file_entry_get_utf8_hash_value_md5 );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf16_hash_value_md5",
	 ewf_test_single_file_entry_get_utf16_hash_value_md5 );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf8_hash_value_sha1",
	 ewf_test_single_file_entry_get_utf8_hash_value_sha1 );

	EWF_TEST_RUN(
	 "libewf_single_file_entry_get_utf16_hash_value_sha1",
	 ewf_test_single_file_entry_get_utf16_hash_value_sha1 );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


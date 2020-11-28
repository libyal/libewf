/*
 * Tools storage_media_buffer functions test program
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
#include "ewf_test_macros.h"
#include "ewf_test_unused.h"

#include "../ewftools/storage_media_buffer.h"

/* Tests the storage_media_buffer_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_storage_media_buffer_initialize(
     void )
{
	libcerror_error_t *error                     = NULL;
	storage_media_buffer_t *storage_media_buffer = NULL;
	int result                                   = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests              = 1;
	int number_of_memset_fail_tests              = 1;
	int test_number                              = 0;
#endif

	/* Test regular cases
	 */
	result = storage_media_buffer_initialize(
	          &storage_media_buffer,
	          NULL,
	          STORAGE_MEDIA_BUFFER_MODE_BUFFERED,
	          512,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "storage_media_buffer",
	 storage_media_buffer );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = storage_media_buffer_free(
	          &storage_media_buffer,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "storage_media_buffer",
	 storage_media_buffer );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = storage_media_buffer_initialize(
	          NULL,
	          NULL,
	          STORAGE_MEDIA_BUFFER_MODE_BUFFERED,
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

	storage_media_buffer = (storage_media_buffer_t *) 0x12345678UL;

	result = storage_media_buffer_initialize(
	          &storage_media_buffer,
	          NULL,
	          STORAGE_MEDIA_BUFFER_MODE_BUFFERED,
	          512,
	          &error );

	storage_media_buffer = NULL;

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
		/* Test storage_media_buffer_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = storage_media_buffer_initialize(
		          &storage_media_buffer,
		          NULL,
		          STORAGE_MEDIA_BUFFER_MODE_BUFFERED,
		          512,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( storage_media_buffer != NULL )
			{
				storage_media_buffer_free(
				 &storage_media_buffer,
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
			 "storage_media_buffer",
			 storage_media_buffer );

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
		/* Test storage_media_buffer_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = storage_media_buffer_initialize(
		          &storage_media_buffer,
		          NULL,
		          STORAGE_MEDIA_BUFFER_MODE_BUFFERED,
		          512,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( storage_media_buffer != NULL )
			{
				storage_media_buffer_free(
				 &storage_media_buffer,
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
			 "storage_media_buffer",
			 storage_media_buffer );

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
	if( storage_media_buffer != NULL )
	{
		storage_media_buffer_free(
		 &storage_media_buffer,
		 NULL );
	}
	return( 0 );
}

/* Tests the storage_media_buffer_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_storage_media_buffer_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = storage_media_buffer_free(
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

	EWF_TEST_RUN(
	 "storage_media_buffer_initialize",
	 ewf_test_tools_storage_media_buffer_initialize );

	EWF_TEST_RUN(
	 "storage_media_buffer_free",
	 ewf_test_tools_storage_media_buffer_free );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


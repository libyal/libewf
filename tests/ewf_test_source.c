/*
 * Library source type test program
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
#include "ewf_test_rwlock.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_lef_source.h"
#include "../libewf/libewf_source.h"

uint8_t ewf_test_source_types_data1[ 28 ] = {
	0x70, 0x09, 0x6e, 0x09, 0x69, 0x64, 0x09, 0x65, 0x76, 0x09, 0x74, 0x62, 0x09, 0x6c, 0x6f, 0x09,
	0x70, 0x6f, 0x09, 0x61, 0x68, 0x09, 0x67, 0x75, 0x09, 0x61, 0x71, 0x0d };

uint8_t ewf_test_source_values_data1[ 36 ] = {
	0x09, 0x45, 0x09, 0x31, 0x09, 0x45, 0x09, 0x32, 0x36, 0x31, 0x38, 0x39, 0x30, 0x30, 0x34, 0x38,
	0x09, 0x2d, 0x31, 0x09, 0x2d, 0x31, 0x09, 0x09, 0x09, 0x31, 0x31, 0x34, 0x32, 0x36, 0x38, 0x33,
	0x36, 0x32, 0x34, 0x0a };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_source_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libewf_lef_source_t *lef_source = NULL;
	libewf_source_t *source         = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( HAVE_EWF_TEST_RWLOCK )
	number_of_malloc_fail_tests += 1;
#endif

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

	/* Test regular cases
	 */
	result = libewf_source_initialize(
	          &source,
	          lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source",
	 source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_source_free(
	          &source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source",
	 source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_source_initialize(
	          NULL,
	          lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	source = (libewf_source_t *) 0x12345678UL;

	result = libewf_source_initialize(
	          &source,
	          lef_source,
	          &error );

	source = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_source_initialize(
	          &source,
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
		/* Test libewf_source_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_source_initialize(
		          &source,
		          lef_source,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( source != NULL )
			{
				libewf_source_free(
				 &source,
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
			 "source",
			 source );

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
		/* Test libewf_source_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_source_initialize(
		          &source,
		          lef_source,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( source != NULL )
			{
				libewf_source_free(
				 &source,
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
			 "source",
			 source );

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
	if( source != NULL )
	{
		libewf_source_free(
		 &source,
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

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

/* Tests the libewf_source_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_source_free(
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

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_source_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_name_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_name_size(
	          source,
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
	result = libewf_source_get_utf8_name_size(
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

	result = libewf_source_get_utf8_name_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_name_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_name_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_name(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_name(
	          source,
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
	result = libewf_source_get_utf8_name(
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

	result = libewf_source_get_utf8_name(
	          source,
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

	result = libewf_source_get_utf8_name(
	          source,
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

	result = libewf_source_get_utf8_name(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_name(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_name(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_name_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_name_size(
	          source,
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
	result = libewf_source_get_utf16_name_size(
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

	result = libewf_source_get_utf16_name_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_name_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_name_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_name(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_name(
	          source,
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
	result = libewf_source_get_utf16_name(
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

	result = libewf_source_get_utf16_name(
	          source,
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

	result = libewf_source_get_utf16_name(
	          source,
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

	result = libewf_source_get_utf16_name(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_name(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_name(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_evidence_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_evidence_number_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_evidence_number_size(
	          source,
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
	result = libewf_source_get_utf8_evidence_number_size(
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

	result = libewf_source_get_utf8_evidence_number_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_evidence_number_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_evidence_number_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_evidence_number_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_evidence_number_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_evidence_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_evidence_number(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_evidence_number(
	          source,
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
	result = libewf_source_get_utf8_evidence_number(
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

	result = libewf_source_get_utf8_evidence_number(
	          source,
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

	result = libewf_source_get_utf8_evidence_number(
	          source,
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

	result = libewf_source_get_utf8_evidence_number(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_evidence_number with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_evidence_number(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_evidence_number with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_evidence_number(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_evidence_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_evidence_number_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_evidence_number_size(
	          source,
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
	result = libewf_source_get_utf16_evidence_number_size(
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

	result = libewf_source_get_utf16_evidence_number_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_evidence_number_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_evidence_number_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_evidence_number_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_evidence_number_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_evidence_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_evidence_number(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_evidence_number(
	          source,
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
	result = libewf_source_get_utf16_evidence_number(
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

	result = libewf_source_get_utf16_evidence_number(
	          source,
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

	result = libewf_source_get_utf16_evidence_number(
	          source,
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

	result = libewf_source_get_utf16_evidence_number(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_evidence_number with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_evidence_number(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_evidence_number with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_evidence_number(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_location_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_location_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_location_size(
	          source,
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
	result = libewf_source_get_utf8_location_size(
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

	result = libewf_source_get_utf8_location_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_location_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_location_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_location_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_location_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_location function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_location(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_location(
	          source,
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
	result = libewf_source_get_utf8_location(
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

	result = libewf_source_get_utf8_location(
	          source,
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

	result = libewf_source_get_utf8_location(
	          source,
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

	result = libewf_source_get_utf8_location(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_location with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_location(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_location with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_location(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_location_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_location_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_location_size(
	          source,
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
	result = libewf_source_get_utf16_location_size(
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

	result = libewf_source_get_utf16_location_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_location_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_location_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_location_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_location_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_location function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_location(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_location(
	          source,
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
	result = libewf_source_get_utf16_location(
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

	result = libewf_source_get_utf16_location(
	          source,
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

	result = libewf_source_get_utf16_location(
	          source,
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

	result = libewf_source_get_utf16_location(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_location with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_location(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_location with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_location(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_device_guid_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_device_guid_size(
	          source,
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
	result = libewf_source_get_utf8_device_guid_size(
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

	result = libewf_source_get_utf8_device_guid_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_device_guid_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_device_guid_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_device_guid_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_device_guid_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_device_guid(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_device_guid(
	          source,
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
	result = libewf_source_get_utf8_device_guid(
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

	result = libewf_source_get_utf8_device_guid(
	          source,
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

	result = libewf_source_get_utf8_device_guid(
	          source,
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

	result = libewf_source_get_utf8_device_guid(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_device_guid with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_device_guid(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_device_guid with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_device_guid(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_device_guid_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_device_guid_size(
	          source,
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
	result = libewf_source_get_utf16_device_guid_size(
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

	result = libewf_source_get_utf16_device_guid_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_device_guid_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_device_guid_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_device_guid_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_device_guid_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_device_guid(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_device_guid(
	          source,
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
	result = libewf_source_get_utf16_device_guid(
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

	result = libewf_source_get_utf16_device_guid(
	          source,
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

	result = libewf_source_get_utf16_device_guid(
	          source,
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

	result = libewf_source_get_utf16_device_guid(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_device_guid with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_device_guid(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_device_guid with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_device_guid(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_primary_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_primary_device_guid_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_primary_device_guid_size(
	          source,
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
	result = libewf_source_get_utf8_primary_device_guid_size(
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

	result = libewf_source_get_utf8_primary_device_guid_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_primary_device_guid_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_primary_device_guid_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_primary_device_guid_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_primary_device_guid_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_primary_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_primary_device_guid(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_primary_device_guid(
	          source,
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
	result = libewf_source_get_utf8_primary_device_guid(
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

	result = libewf_source_get_utf8_primary_device_guid(
	          source,
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

	result = libewf_source_get_utf8_primary_device_guid(
	          source,
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

	result = libewf_source_get_utf8_primary_device_guid(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_primary_device_guid with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_primary_device_guid(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_primary_device_guid with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_primary_device_guid(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_primary_device_guid_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_primary_device_guid_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_primary_device_guid_size(
	          source,
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
	result = libewf_source_get_utf16_primary_device_guid_size(
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

	result = libewf_source_get_utf16_primary_device_guid_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_primary_device_guid_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_primary_device_guid_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_primary_device_guid_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_primary_device_guid_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_primary_device_guid function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_primary_device_guid(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_primary_device_guid(
	          source,
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
	result = libewf_source_get_utf16_primary_device_guid(
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

	result = libewf_source_get_utf16_primary_device_guid(
	          source,
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

	result = libewf_source_get_utf16_primary_device_guid(
	          source,
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

	result = libewf_source_get_utf16_primary_device_guid(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_primary_device_guid with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_primary_device_guid(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_primary_device_guid with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_primary_device_guid(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_manufacturer_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_manufacturer_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_manufacturer_size(
	          source,
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
	result = libewf_source_get_utf8_manufacturer_size(
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

	result = libewf_source_get_utf8_manufacturer_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_manufacturer_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_manufacturer_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_manufacturer_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_manufacturer_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_manufacturer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_manufacturer(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_manufacturer(
	          source,
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
	result = libewf_source_get_utf8_manufacturer(
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

	result = libewf_source_get_utf8_manufacturer(
	          source,
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

	result = libewf_source_get_utf8_manufacturer(
	          source,
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

	result = libewf_source_get_utf8_manufacturer(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_manufacturer with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_manufacturer(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_manufacturer with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_manufacturer(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_manufacturer_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_manufacturer_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_manufacturer_size(
	          source,
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
	result = libewf_source_get_utf16_manufacturer_size(
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

	result = libewf_source_get_utf16_manufacturer_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_manufacturer_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_manufacturer_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_manufacturer_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_manufacturer_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_manufacturer function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_manufacturer(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_manufacturer(
	          source,
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
	result = libewf_source_get_utf16_manufacturer(
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

	result = libewf_source_get_utf16_manufacturer(
	          source,
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

	result = libewf_source_get_utf16_manufacturer(
	          source,
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

	result = libewf_source_get_utf16_manufacturer(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_manufacturer with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_manufacturer(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_manufacturer with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_manufacturer(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_model_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_model_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_model_size(
	          source,
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
	result = libewf_source_get_utf8_model_size(
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

	result = libewf_source_get_utf8_model_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_model_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_model_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_model_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_model_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_model function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_model(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_model(
	          source,
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
	result = libewf_source_get_utf8_model(
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

	result = libewf_source_get_utf8_model(
	          source,
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

	result = libewf_source_get_utf8_model(
	          source,
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

	result = libewf_source_get_utf8_model(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_model with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_model(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_model with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_model(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_model_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_model_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_model_size(
	          source,
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
	result = libewf_source_get_utf16_model_size(
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

	result = libewf_source_get_utf16_model_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_model_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_model_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_model_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_model_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_model function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_model(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_model(
	          source,
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
	result = libewf_source_get_utf16_model(
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

	result = libewf_source_get_utf16_model(
	          source,
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

	result = libewf_source_get_utf16_model(
	          source,
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

	result = libewf_source_get_utf16_model(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_model with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_model(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_model with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_model(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_serial_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_serial_number_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_serial_number_size(
	          source,
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
	result = libewf_source_get_utf8_serial_number_size(
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

	result = libewf_source_get_utf8_serial_number_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_serial_number_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_serial_number_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_serial_number_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_serial_number_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_serial_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_serial_number(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_serial_number(
	          source,
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
	result = libewf_source_get_utf8_serial_number(
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

	result = libewf_source_get_utf8_serial_number(
	          source,
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

	result = libewf_source_get_utf8_serial_number(
	          source,
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

	result = libewf_source_get_utf8_serial_number(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_serial_number with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_serial_number(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_serial_number with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_serial_number(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_serial_number_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_serial_number_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_serial_number_size(
	          source,
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
	result = libewf_source_get_utf16_serial_number_size(
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

	result = libewf_source_get_utf16_serial_number_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_serial_number_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_serial_number_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_serial_number_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_serial_number_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_serial_number function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_serial_number(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_serial_number(
	          source,
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
	result = libewf_source_get_utf16_serial_number(
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

	result = libewf_source_get_utf16_serial_number(
	          source,
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

	result = libewf_source_get_utf16_serial_number(
	          source,
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

	result = libewf_source_get_utf16_serial_number(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_serial_number with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_serial_number(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_serial_number with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_serial_number(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_domain_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_domain_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_domain_size(
	          source,
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
	result = libewf_source_get_utf8_domain_size(
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

	result = libewf_source_get_utf8_domain_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_domain_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_domain_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_domain_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_domain_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_domain function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_domain(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_domain(
	          source,
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
	result = libewf_source_get_utf8_domain(
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

	result = libewf_source_get_utf8_domain(
	          source,
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

	result = libewf_source_get_utf8_domain(
	          source,
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

	result = libewf_source_get_utf8_domain(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_domain with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_domain(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_domain with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_domain(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_domain_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_domain_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_domain_size(
	          source,
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
	result = libewf_source_get_utf16_domain_size(
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

	result = libewf_source_get_utf16_domain_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_domain_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_domain_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_domain_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_domain_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_domain function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_domain(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_domain(
	          source,
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
	result = libewf_source_get_utf16_domain(
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

	result = libewf_source_get_utf16_domain(
	          source,
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

	result = libewf_source_get_utf16_domain(
	          source,
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

	result = libewf_source_get_utf16_domain(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_domain with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_domain(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_domain with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_domain(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_ip_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_ip_address_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_ip_address_size(
	          source,
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
	result = libewf_source_get_utf8_ip_address_size(
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

	result = libewf_source_get_utf8_ip_address_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_ip_address_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_ip_address_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_ip_address_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_ip_address_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_ip_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_ip_address(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_ip_address(
	          source,
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
	result = libewf_source_get_utf8_ip_address(
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

	result = libewf_source_get_utf8_ip_address(
	          source,
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

	result = libewf_source_get_utf8_ip_address(
	          source,
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

	result = libewf_source_get_utf8_ip_address(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_ip_address with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_ip_address(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_ip_address with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_ip_address(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_ip_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_ip_address_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_ip_address_size(
	          source,
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
	result = libewf_source_get_utf16_ip_address_size(
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

	result = libewf_source_get_utf16_ip_address_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_ip_address_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_ip_address_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_ip_address_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_ip_address_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_ip_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_ip_address(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_ip_address(
	          source,
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
	result = libewf_source_get_utf16_ip_address(
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

	result = libewf_source_get_utf16_ip_address(
	          source,
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

	result = libewf_source_get_utf16_ip_address(
	          source,
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

	result = libewf_source_get_utf16_ip_address(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_ip_address with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_ip_address(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_ip_address with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_ip_address(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_mac_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_mac_address_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_mac_address_size(
	          source,
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
	result = libewf_source_get_utf8_mac_address_size(
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

	result = libewf_source_get_utf8_mac_address_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_mac_address_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_mac_address_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_mac_address_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_mac_address_size(
	          source,
	          &utf8_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_mac_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_mac_address(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_mac_address(
	          source,
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
	result = libewf_source_get_utf8_mac_address(
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

	result = libewf_source_get_utf8_mac_address(
	          source,
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

	result = libewf_source_get_utf8_mac_address(
	          source,
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

	result = libewf_source_get_utf8_mac_address(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_mac_address with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_mac_address(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_mac_address with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_mac_address(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_mac_address_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_mac_address_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_mac_address_size(
	          source,
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
	result = libewf_source_get_utf16_mac_address_size(
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

	result = libewf_source_get_utf16_mac_address_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_mac_address_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_mac_address_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_mac_address_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_mac_address_size(
	          source,
	          &utf16_string_size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_mac_address function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_mac_address(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_mac_address(
	          source,
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
	result = libewf_source_get_utf16_mac_address(
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

	result = libewf_source_get_utf16_mac_address(
	          source,
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

	result = libewf_source_get_utf16_mac_address(
	          source,
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

	result = libewf_source_get_utf16_mac_address(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_mac_address with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_mac_address(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_mac_address with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_mac_address(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_size(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_size(
	          source,
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
	result = libewf_source_get_size(
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

	result = libewf_source_get_size(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_size(
	          source,
	          &size,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_size(
	          source,
	          &size,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_acquisition_time function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_acquisition_time(
     libewf_source_t *source )
{
	libcerror_error_t *error = NULL;
	int64_t acquisition_time = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_acquisition_time(
	          source,
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
	result = libewf_source_get_acquisition_time(
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

	result = libewf_source_get_acquisition_time(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_acquisition_time with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_acquisition_time(
	          source,
	          &acquisition_time,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_acquisition_time with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_acquisition_time(
	          source,
	          &acquisition_time,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_hash_value_md5(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_hash_value_md5(
	          source,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_source_get_utf8_hash_value_md5(
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

	result = libewf_source_get_utf8_hash_value_md5(
	          source,
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

	result = libewf_source_get_utf8_hash_value_md5(
	          source,
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

	result = libewf_source_get_utf8_hash_value_md5(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_hash_value_md5 with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_hash_value_md5(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_hash_value_md5 with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_hash_value_md5(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_hash_value_md5 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_hash_value_md5(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_hash_value_md5(
	          source,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_source_get_utf16_hash_value_md5(
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

	result = libewf_source_get_utf16_hash_value_md5(
	          source,
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

	result = libewf_source_get_utf16_hash_value_md5(
	          source,
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

	result = libewf_source_get_utf16_hash_value_md5(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_hash_value_md5 with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_hash_value_md5(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_hash_value_md5 with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_hash_value_md5(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf8_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf8_hash_value_sha1(
     libewf_source_t *source )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf8_hash_value_sha1(
	          source,
	          utf8_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_source_get_utf8_hash_value_sha1(
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

	result = libewf_source_get_utf8_hash_value_sha1(
	          source,
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

	result = libewf_source_get_utf8_hash_value_sha1(
	          source,
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

	result = libewf_source_get_utf8_hash_value_sha1(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf8_hash_value_sha1 with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_hash_value_sha1(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf8_hash_value_sha1 with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf8_hash_value_sha1(
	          source,
	          utf8_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_source_get_utf16_hash_value_sha1 function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_source_get_utf16_hash_value_sha1(
     libewf_source_t *source )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_source_get_utf16_hash_value_sha1(
	          source,
	          utf16_string,
	          64,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_source_get_utf16_hash_value_sha1(
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

	result = libewf_source_get_utf16_hash_value_sha1(
	          source,
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

	result = libewf_source_get_utf16_hash_value_sha1(
	          source,
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

	result = libewf_source_get_utf16_hash_value_sha1(
	          source,
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

#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_source_get_utf16_hash_value_sha1 with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_hash_value_sha1(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_rdlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_rdlock_attempts_before_fail = -1;
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
	/* Test libewf_source_get_utf16_hash_value_sha1 with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_source_get_utf16_hash_value_sha1(
	          source,
	          utf16_string,
	          64,
	          &error );

	if( ewf_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_unlock_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_RWLOCK ) */

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
	libewf_source_t *source              = NULL;
	libfvalue_split_utf8_string_t *types = NULL;
	int result                           = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_source_initialize",
	 ewf_test_source_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_RUN(
	 "libewf_source_free",
	 ewf_test_source_free );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize source for tests
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
	          ewf_test_source_types_data1,
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
	          ewf_test_source_values_data1,
	          36,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_source_initialize(
	          &source,
	          lef_source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source",
	 source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_name_size",
	 ewf_test_source_get_utf8_name_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_name",
	 ewf_test_source_get_utf8_name,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_name_size",
	 ewf_test_source_get_utf16_name_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_name",
	 ewf_test_source_get_utf16_name,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_evidence_number_size",
	 ewf_test_source_get_utf8_evidence_number_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_evidence_number",
	 ewf_test_source_get_utf8_evidence_number,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_evidence_number_size",
	 ewf_test_source_get_utf16_evidence_number_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_evidence_number",
	 ewf_test_source_get_utf16_evidence_number,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_location_size",
	 ewf_test_source_get_utf8_location_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_location",
	 ewf_test_source_get_utf8_location,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_location_size",
	 ewf_test_source_get_utf16_location_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_location",
	 ewf_test_source_get_utf16_location,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_device_guid_size",
	 ewf_test_source_get_utf8_device_guid_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_device_guid",
	 ewf_test_source_get_utf8_device_guid,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_device_guid_size",
	 ewf_test_source_get_utf16_device_guid_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_device_guid",
	 ewf_test_source_get_utf16_device_guid,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_primary_device_guid_size",
	 ewf_test_source_get_utf8_primary_device_guid_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_primary_device_guid",
	 ewf_test_source_get_utf8_primary_device_guid,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_primary_device_guid_size",
	 ewf_test_source_get_utf16_primary_device_guid_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_primary_device_guid",
	 ewf_test_source_get_utf16_primary_device_guid,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_manufacturer_size",
	 ewf_test_source_get_utf8_manufacturer_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_manufacturer",
	 ewf_test_source_get_utf8_manufacturer,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_manufacturer_size",
	 ewf_test_source_get_utf16_manufacturer_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_manufacturer",
	 ewf_test_source_get_utf16_manufacturer,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_model_size",
	 ewf_test_source_get_utf8_model_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_model",
	 ewf_test_source_get_utf8_model,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_model_size",
	 ewf_test_source_get_utf16_model_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_model",
	 ewf_test_source_get_utf16_model,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_serial_number_size",
	 ewf_test_source_get_utf8_serial_number_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_serial_number",
	 ewf_test_source_get_utf8_serial_number,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_serial_number_size",
	 ewf_test_source_get_utf16_serial_number_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_serial_number",
	 ewf_test_source_get_utf16_serial_number,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_domain_size",
	 ewf_test_source_get_utf8_domain_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_domain",
	 ewf_test_source_get_utf8_domain,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_domain_size",
	 ewf_test_source_get_utf16_domain_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_domain",
	 ewf_test_source_get_utf16_domain,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_ip_address_size",
	 ewf_test_source_get_utf8_ip_address_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_ip_address",
	 ewf_test_source_get_utf8_ip_address,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_ip_address_size",
	 ewf_test_source_get_utf16_ip_address_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_ip_address",
	 ewf_test_source_get_utf16_ip_address,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_mac_address_size",
	 ewf_test_source_get_utf8_mac_address_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_mac_address",
	 ewf_test_source_get_utf8_mac_address,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_mac_address_size",
	 ewf_test_source_get_utf16_mac_address_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_mac_address",
	 ewf_test_source_get_utf16_mac_address,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_size",
	 ewf_test_source_get_size,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_acquisition_time",
	 ewf_test_source_get_acquisition_time,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_hash_value_md5",
	 ewf_test_source_get_utf8_hash_value_md5,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_hash_value_md5",
	 ewf_test_source_get_utf16_hash_value_md5,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf8_hash_value_sha1",
	 ewf_test_source_get_utf8_hash_value_sha1,
	 source );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_source_get_utf16_hash_value_sha1",
	 ewf_test_source_get_utf16_hash_value_sha1,
	 source );

	/* Clean up
	 */
	result = libewf_source_free(
	          &source,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source",
	 source );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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

on_error:
#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( source != NULL )
	{
		libewf_source_free(
		 &source,
		 NULL );
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
#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}


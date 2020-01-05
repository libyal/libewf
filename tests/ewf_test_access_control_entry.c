/*
 * Library access_control_entry type test program
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

#include "../libewf/libewf_access_control_entry.h"
#include "../libewf/libewf_lef_permission.h"

uint8_t ewf_test_access_control_entry_types_data1[ 17 ] = {
	0x70, 0x09, 0x6e, 0x09, 0x73, 0x09, 0x70, 0x72, 0x09, 0x6e, 0x74, 0x61, 0x09, 0x6e, 0x74, 0x69,
	0x0a };

/* p	n	s		pr	nta	nti
 * 	System	S-1-5-18	2	2032127	16
 */
uint8_t ewf_test_access_control_entry_values_data1[ 30 ] = {
	0x09, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x09, 0x53, 0x2d, 0x31, 0x2d, 0x35, 0x2d, 0x31, 0x38,
	0x09, 0x32, 0x09, 0x32, 0x30, 0x33, 0x32, 0x31, 0x32, 0x37, 0x09, 0x31, 0x36, 0x0d };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_access_control_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_initialize(
     void )
{
	libcerror_error_t *error                            = NULL;
	libewf_access_control_entry_t *access_control_entry = NULL;
	libewf_lef_permission_t *lef_permission             = NULL;
	int result                                          = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                     = 1;
	int number_of_memset_fail_tests                     = 1;
	int test_number                                     = 0;
#endif

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( HAVE_EWF_TEST_RWLOCK )
	number_of_malloc_fail_tests += 1;
#endif

	/* Initialize test
	 */
	result = libewf_lef_permission_initialize(
	          &lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_permission",
	 lef_permission );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_access_control_entry_initialize(
	          &access_control_entry,
	          lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "access_control_entry",
	 access_control_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_access_control_entry_free(
	          &access_control_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "access_control_entry",
	 access_control_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_access_control_entry_initialize(
	          NULL,
	          lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	access_control_entry = (libewf_access_control_entry_t *) 0x12345678UL;

	result = libewf_access_control_entry_initialize(
	          &access_control_entry,
	          lef_permission,
	          &error );

	access_control_entry = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_access_control_entry_initialize(
	          &access_control_entry,
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
		/* Test libewf_access_control_entry_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_access_control_entry_initialize(
		          &access_control_entry,
		          lef_permission,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( access_control_entry != NULL )
			{
				libewf_access_control_entry_free(
				 &access_control_entry,
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
			 "access_control_entry",
			 access_control_entry );

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
		/* Test libewf_access_control_entry_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_access_control_entry_initialize(
		          &access_control_entry,
		          lef_permission,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( access_control_entry != NULL )
			{
				libewf_access_control_entry_free(
				 &access_control_entry,
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
			 "access_control_entry",
			 access_control_entry );

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
	result = libewf_lef_permission_free(
	          &lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_permission",
	 lef_permission );

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
	if( access_control_entry != NULL )
	{
		libewf_access_control_entry_free(
		 &access_control_entry,
		 NULL );
	}
	if( lef_permission != NULL )
	{
		libewf_lef_permission_free(
		 &lef_permission,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

/* Tests the libewf_access_control_entry_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_free(
     void )
{
	libcerror_error_t *error                            = NULL;
	int result                                          = 0;

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) && defined( HAVE_EWF_TEST_RWLOCK )
	libewf_access_control_entry_t *access_control_entry = NULL;
	libewf_lef_permission_t *lef_permission             = NULL;
#endif

	/* Test error cases
	 */
	result = libewf_access_control_entry_free(
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

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) && defined( HAVE_EWF_TEST_RWLOCK )

	/* Initialize test
	 */
	result = libewf_lef_permission_initialize(
	          &lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_permission",
	 lef_permission );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_access_control_entry_initialize(
	          &access_control_entry,
	          lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "access_control_entry",
	 access_control_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libewf_access_control_entry_free with pthread_rwlock_destroy failing in libcthreads_read_write_lock_free
	 */
	ewf_test_pthread_rwlock_destroy_attempts_before_fail = 0;

	result = libewf_access_control_entry_free(
	          &access_control_entry,
	          &error );

	if( ewf_test_pthread_rwlock_destroy_attempts_before_fail != -1 )
	{
		ewf_test_pthread_rwlock_destroy_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "access_control_entry",
		 access_control_entry );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Clean up
	 */
	result = libewf_lef_permission_free(
	          &lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_permission",
	 lef_permission );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) && defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) && defined( HAVE_EWF_TEST_RWLOCK )
	if( access_control_entry != NULL )
	{
		libewf_access_control_entry_free(
		 &access_control_entry,
		 NULL );
	}
	if( lef_permission != NULL )
	{
		libewf_lef_permission_free(
		 &lef_permission,
		 NULL );
	}
#endif
	return( 0 );
}

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_access_control_entry_get_type function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_type(
     libewf_access_control_entry_t *access_control_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_type(
	          access_control_entry,
	          &type,
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
	result = libewf_access_control_entry_get_type(
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

	result = libewf_access_control_entry_get_type(
	          access_control_entry,
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

	/* Test libewf_access_control_entry_get_type with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_type(
	          access_control_entry,
	          &type,
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
	/* Test libewf_access_control_entry_get_type with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_type(
	          access_control_entry,
	          &type,
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

/* Tests the libewf_access_control_entry_get_utf8_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf8_identifier_size(
     libewf_access_control_entry_t *access_control_entry )
{
	libcerror_error_t *error        = NULL;
	size_t utf8_string_size         = 0;
	int result                      = 0;
	int utf8_identifier_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf8_identifier_size(
	          access_control_entry,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_identifier_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_access_control_entry_get_utf8_identifier_size(
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

	if( utf8_identifier_size_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf8_identifier_size(
		          access_control_entry,
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
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf8_identifier_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_identifier_size(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf8_identifier_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_identifier_size(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_utf8_identifier function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf8_identifier(
     libewf_access_control_entry_t *access_control_entry )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error   = NULL;
	int result                 = 0;
	int utf8_identifier_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf8_identifier(
	          access_control_entry,
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

	utf8_identifier_is_set = result;

	/* Test error cases
	 */
	result = libewf_access_control_entry_get_utf8_identifier(
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

	if( utf8_identifier_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf8_identifier(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf8_identifier(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf8_identifier(
		          access_control_entry,
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
	}
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf8_identifier with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_identifier(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf8_identifier with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_identifier(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_utf16_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf16_identifier_size(
     libewf_access_control_entry_t *access_control_entry )
{
	libcerror_error_t *error         = NULL;
	size_t utf16_string_size         = 0;
	int result                       = 0;
	int utf16_identifier_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf16_identifier_size(
	          access_control_entry,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_identifier_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_access_control_entry_get_utf16_identifier_size(
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

	if( utf16_identifier_size_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf16_identifier_size(
		          access_control_entry,
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
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf16_identifier_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_identifier_size(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf16_identifier_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_identifier_size(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_utf16_identifier function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf16_identifier(
     libewf_access_control_entry_t *access_control_entry )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error    = NULL;
	int result                  = 0;
	int utf16_identifier_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf16_identifier(
	          access_control_entry,
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

	utf16_identifier_is_set = result;

	/* Test error cases
	 */
	result = libewf_access_control_entry_get_utf16_identifier(
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

	if( utf16_identifier_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf16_identifier(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf16_identifier(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf16_identifier(
		          access_control_entry,
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
	}
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf16_identifier with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_identifier(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf16_identifier with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_identifier(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf8_name_size(
     libewf_access_control_entry_t *access_control_entry )
{
	libcerror_error_t *error  = NULL;
	size_t utf8_string_size   = 0;
	int result                = 0;
	int utf8_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf8_name_size(
	          access_control_entry,
	          &utf8_string_size,
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
	result = libewf_access_control_entry_get_utf8_name_size(
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

	if( utf8_name_size_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf8_name_size(
		          access_control_entry,
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
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf8_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_name_size(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf8_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_name_size(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf8_name(
     libewf_access_control_entry_t *access_control_entry )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;
	int utf8_name_is_set     = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf8_name(
	          access_control_entry,
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

	utf8_name_is_set = result;

	/* Test error cases
	 */
	result = libewf_access_control_entry_get_utf8_name(
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

	if( utf8_name_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf8_name(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf8_name(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf8_name(
		          access_control_entry,
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
	}
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf8_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_name(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf8_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf8_name(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf16_name_size(
     libewf_access_control_entry_t *access_control_entry )
{
	libcerror_error_t *error   = NULL;
	size_t utf16_string_size   = 0;
	int result                 = 0;
	int utf16_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf16_name_size(
	          access_control_entry,
	          &utf16_string_size,
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
	result = libewf_access_control_entry_get_utf16_name_size(
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

	if( utf16_name_size_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf16_name_size(
		          access_control_entry,
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
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf16_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_name_size(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf16_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_name_size(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_utf16_name(
     libewf_access_control_entry_t *access_control_entry )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;
	int utf16_name_is_set    = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_utf16_name(
	          access_control_entry,
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

	utf16_name_is_set = result;

	/* Test error cases
	 */
	result = libewf_access_control_entry_get_utf16_name(
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

	if( utf16_name_is_set != 0 )
	{
		result = libewf_access_control_entry_get_utf16_name(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf16_name(
		          access_control_entry,
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

		result = libewf_access_control_entry_get_utf16_name(
		          access_control_entry,
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
	}
#if defined( HAVE_EWF_TEST_RWLOCK )

	/* Test libewf_access_control_entry_get_utf16_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_name(
	          access_control_entry,
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
	/* Test libewf_access_control_entry_get_utf16_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_utf16_name(
	          access_control_entry,
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

/* Tests the libewf_access_control_entry_get_access_mask function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_access_mask(
     libewf_access_control_entry_t *access_control_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t access_mask     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_access_mask(
	          access_control_entry,
	          &access_mask,
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
	result = libewf_access_control_entry_get_access_mask(
	          NULL,
	          &access_mask,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_access_control_entry_get_access_mask(
	          access_control_entry,
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

	/* Test libewf_access_control_entry_get_access_mask with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_access_mask(
	          access_control_entry,
	          &access_mask,
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
	/* Test libewf_access_control_entry_get_access_mask with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_access_mask(
	          access_control_entry,
	          &access_mask,
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

/* Tests the libewf_access_control_entry_get_flags function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_get_flags(
     libewf_access_control_entry_t *access_control_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t flags           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_access_control_entry_get_flags(
	          access_control_entry,
	          &flags,
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
	result = libewf_access_control_entry_get_flags(
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

	result = libewf_access_control_entry_get_flags(
	          access_control_entry,
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

	/* Test libewf_access_control_entry_get_flags with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_flags(
	          access_control_entry,
	          &flags,
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
	/* Test libewf_access_control_entry_get_flags with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_access_control_entry_get_flags(
	          access_control_entry,
	          &flags,
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

	libcerror_error_t *error                            = NULL;
	libewf_access_control_entry_t *access_control_entry = NULL;
	libewf_lef_permission_t *lef_permission             = NULL;
	libfvalue_split_utf8_string_t *types                = NULL;
	int result                                          = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_access_control_entry_initialize",
	 ewf_test_access_control_entry_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_RUN(
	 "libewf_access_control_entry_free",
	 ewf_test_access_control_entry_free );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize access_control_entry for tests
	 */
	result = libewf_lef_permission_initialize(
	          &lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "lef_permission",
	 lef_permission );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_utf8_string_split(
	          ewf_test_access_control_entry_types_data1,
	          17,
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

	result = libewf_lef_permission_read_data(
	          lef_permission,
	          types,
	          ewf_test_access_control_entry_values_data1,
	          30,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_access_control_entry_initialize(
	          &access_control_entry,
	          lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "access_control_entry",
	 access_control_entry );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_type",
	 ewf_test_access_control_entry_get_type,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf8_identifier_size",
	 ewf_test_access_control_entry_get_utf8_identifier_size,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf8_identifier",
	 ewf_test_access_control_entry_get_utf8_identifier,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf16_identifier_size",
	 ewf_test_access_control_entry_get_utf16_identifier_size,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf16_identifier",
	 ewf_test_access_control_entry_get_utf16_identifier,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf8_name_size",
	 ewf_test_access_control_entry_get_utf8_name_size,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf8_name",
	 ewf_test_access_control_entry_get_utf8_name,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf16_name_size",
	 ewf_test_access_control_entry_get_utf16_name_size,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_utf16_name",
	 ewf_test_access_control_entry_get_utf16_name,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_access_mask",
	 ewf_test_access_control_entry_get_access_mask,
	 access_control_entry );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_access_control_entry_get_flags",
	 ewf_test_access_control_entry_get_flags,
	 access_control_entry );

	/* Clean up
	 */
	result = libewf_access_control_entry_free(
	          &access_control_entry,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "access_control_entry",
	 access_control_entry );

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

	result = libewf_lef_permission_free(
	          &lef_permission,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "lef_permission",
	 lef_permission );

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
	if( access_control_entry != NULL )
	{
		libewf_access_control_entry_free(
		 &access_control_entry,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lef_permission != NULL )
	{
		libewf_lef_permission_free(
		 &lef_permission,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}


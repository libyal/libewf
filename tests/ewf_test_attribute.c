/*
 * Library attribute type test program
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
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_rwlock.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_attribute.h"
#include "../libewf/libewf_lef_extended_attribute.h"

uint8_t ewf_test_attribute_data1[ 37 ] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x48, 0x00, 0x69,
	0x00, 0x64, 0x00, 0x64, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x74, 0x00, 0x72, 0x00, 0x75,
	0x00, 0x65, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_attribute_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_initialize(
     void )
{
	libcerror_error_t *error                                = NULL;
	libewf_attribute_t *attribute                           = NULL;
	libewf_lef_extended_attribute_t *lef_extended_attribute = NULL;
	int result                                              = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                         = 1;
	int number_of_memset_fail_tests                         = 1;
	int test_number                                         = 0;
#endif

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( HAVE_EWF_TEST_RWLOCK )
	number_of_malloc_fail_tests += 1;
#endif

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
	result = libewf_attribute_initialize(
	          &attribute,
	          lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_attribute_free(
	          &attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_attribute_initialize(
	          NULL,
	          lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	attribute = (libewf_attribute_t *) 0x12345678UL;

	result = libewf_attribute_initialize(
	          &attribute,
	          lef_extended_attribute,
	          &error );

	attribute = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_attribute_initialize(
	          &attribute,
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
		/* Test libewf_attribute_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_attribute_initialize(
		          &attribute,
		          lef_extended_attribute,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( attribute != NULL )
			{
				libewf_attribute_free(
				 &attribute,
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
			 "attribute",
			 attribute );

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
		/* Test libewf_attribute_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_attribute_initialize(
		          &attribute,
		          lef_extended_attribute,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( attribute != NULL )
			{
				libewf_attribute_free(
				 &attribute,
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
			 "attribute",
			 attribute );

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
	if( attribute != NULL )
	{
		libewf_attribute_free(
		 &attribute,
		 NULL );
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

/* Tests the libewf_attribute_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_free(
     void )
{
	libcerror_error_t *error                                = NULL;
	int result                                              = 0;

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) && defined( HAVE_EWF_TEST_RWLOCK )
	libewf_attribute_t *attribute                           = NULL;
	libewf_lef_extended_attribute_t *lef_extended_attribute = NULL;
#endif

	/* Test error cases
	 */
	result = libewf_attribute_free(
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

	result = libewf_attribute_initialize(
	          &attribute,
	          lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libewf_attribute_free with pthread_rwlock_destroy failing in libcthreads_read_write_lock_free
	 */
	ewf_test_pthread_rwlock_destroy_attempts_before_fail = 0;

	result = libewf_attribute_free(
	          &attribute,
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
		 "attribute",
		 attribute );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
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

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) && defined( HAVE_EWF_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) && defined( HAVE_EWF_TEST_RWLOCK )
	if( attribute != NULL )
	{
		libewf_attribute_free(
		 &attribute,
		 NULL );
	}
	if( lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &lef_extended_attribute,
		 NULL );
	}
#endif
	return( 0 );
}

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_attribute_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf8_name_size(
     libewf_attribute_t *attribute )
{
	libcerror_error_t *error  = NULL;
	size_t utf8_string_size   = 0;
	int result                = 0;
	int utf8_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf8_name_size(
	          attribute,
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
	result = libewf_attribute_get_utf8_name_size(
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
		result = libewf_attribute_get_utf8_name_size(
		          attribute,
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

	/* Test libewf_attribute_get_utf8_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_name_size(
	          attribute,
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
	/* Test libewf_attribute_get_utf8_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_name_size(
	          attribute,
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

/* Tests the libewf_attribute_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf8_name(
     libewf_attribute_t *attribute )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;
	int utf8_name_is_set     = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf8_name(
	          attribute,
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
	result = libewf_attribute_get_utf8_name(
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
		result = libewf_attribute_get_utf8_name(
		          attribute,
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

		result = libewf_attribute_get_utf8_name(
		          attribute,
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

		result = libewf_attribute_get_utf8_name(
		          attribute,
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

	/* Test libewf_attribute_get_utf8_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_name(
	          attribute,
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
	/* Test libewf_attribute_get_utf8_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_name(
	          attribute,
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

/* Tests the libewf_attribute_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf16_name_size(
     libewf_attribute_t *attribute )
{
	libcerror_error_t *error   = NULL;
	size_t utf16_string_size   = 0;
	int result                 = 0;
	int utf16_name_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf16_name_size(
	          attribute,
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
	result = libewf_attribute_get_utf16_name_size(
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
		result = libewf_attribute_get_utf16_name_size(
		          attribute,
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

	/* Test libewf_attribute_get_utf16_name_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_name_size(
	          attribute,
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
	/* Test libewf_attribute_get_utf16_name_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_name_size(
	          attribute,
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

/* Tests the libewf_attribute_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf16_name(
     libewf_attribute_t *attribute )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;
	int utf16_name_is_set    = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf16_name(
	          attribute,
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
	result = libewf_attribute_get_utf16_name(
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
		result = libewf_attribute_get_utf16_name(
		          attribute,
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

		result = libewf_attribute_get_utf16_name(
		          attribute,
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

		result = libewf_attribute_get_utf16_name(
		          attribute,
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

	/* Test libewf_attribute_get_utf16_name with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_name(
	          attribute,
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
	/* Test libewf_attribute_get_utf16_name with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_name(
	          attribute,
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

/* Tests the libewf_attribute_get_utf8_value_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf8_value_size(
     libewf_attribute_t *attribute )
{
	libcerror_error_t *error   = NULL;
	size_t utf8_string_size    = 0;
	int result                 = 0;
	int utf8_value_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf8_value_size(
	          attribute,
	          &utf8_string_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_value_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_attribute_get_utf8_value_size(
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

	if( utf8_value_size_is_set != 0 )
	{
		result = libewf_attribute_get_utf8_value_size(
		          attribute,
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

	/* Test libewf_attribute_get_utf8_value_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_value_size(
	          attribute,
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
	/* Test libewf_attribute_get_utf8_value_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_value_size(
	          attribute,
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

/* Tests the libewf_attribute_get_utf8_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf8_value(
     libewf_attribute_t *attribute )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;
	int utf8_value_is_set    = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf8_value(
	          attribute,
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

	utf8_value_is_set = result;

	/* Test error cases
	 */
	result = libewf_attribute_get_utf8_value(
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

	if( utf8_value_is_set != 0 )
	{
		result = libewf_attribute_get_utf8_value(
		          attribute,
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

		result = libewf_attribute_get_utf8_value(
		          attribute,
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

		result = libewf_attribute_get_utf8_value(
		          attribute,
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

	/* Test libewf_attribute_get_utf8_value with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_value(
	          attribute,
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
	/* Test libewf_attribute_get_utf8_value with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf8_value(
	          attribute,
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

/* Tests the libewf_attribute_get_utf16_value_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf16_value_size(
     libewf_attribute_t *attribute )
{
	libcerror_error_t *error    = NULL;
	size_t utf16_string_size    = 0;
	int result                  = 0;
	int utf16_value_size_is_set = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf16_value_size(
	          attribute,
	          &utf16_string_size,
	          &error );

	EWF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_value_size_is_set = result;

	/* Test error cases
	 */
	result = libewf_attribute_get_utf16_value_size(
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

	if( utf16_value_size_is_set != 0 )
	{
		result = libewf_attribute_get_utf16_value_size(
		          attribute,
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

	/* Test libewf_attribute_get_utf16_value_size with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_value_size(
	          attribute,
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
	/* Test libewf_attribute_get_utf16_value_size with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_value_size(
	          attribute,
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

/* Tests the libewf_attribute_get_utf16_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_attribute_get_utf16_value(
     libewf_attribute_t *attribute )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;
	int utf16_value_is_set   = 0;

	/* Test regular cases
	 */
	result = libewf_attribute_get_utf16_value(
	          attribute,
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

	utf16_value_is_set = result;

	/* Test error cases
	 */
	result = libewf_attribute_get_utf16_value(
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

	if( utf16_value_is_set != 0 )
	{
		result = libewf_attribute_get_utf16_value(
		          attribute,
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

		result = libewf_attribute_get_utf16_value(
		          attribute,
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

		result = libewf_attribute_get_utf16_value(
		          attribute,
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

	/* Test libewf_attribute_get_utf16_value with pthread_rwlock_rdlock failing in libcthreads_read_write_lock_grab_for_read
	 */
	ewf_test_pthread_rwlock_rdlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_value(
	          attribute,
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
	/* Test libewf_attribute_get_utf16_value with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_read
	 */
	ewf_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	result = libewf_attribute_get_utf16_value(
	          attribute,
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

	libcerror_error_t *error                                = NULL;
	libewf_attribute_t *attribute                           = NULL;
	libewf_lef_extended_attribute_t *lef_extended_attribute = NULL;
	ssize_t read_count                                      = 0;
	int result                                              = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_UNREFERENCED_PARAMETER( argc )
	EWF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_attribute_initialize",
	 ewf_test_attribute_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_RUN(
	 "libewf_attribute_free",
	 ewf_test_attribute_free );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize attribute for tests
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
	              ewf_test_attribute_data1,
	              37,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 37 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_attribute_initialize(
	          &attribute,
	          lef_extended_attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "attribute",
	 attribute );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf8_name_size",
	 ewf_test_attribute_get_utf8_name_size,
	 attribute );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf8_name",
	 ewf_test_attribute_get_utf8_name,
	 attribute );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf16_name_size",
	 ewf_test_attribute_get_utf16_name_size,
	 attribute );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf16_name",
	 ewf_test_attribute_get_utf16_name,
	 attribute );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf8_value_size",
	 ewf_test_attribute_get_utf8_value_size,
	 attribute );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf8_value",
	 ewf_test_attribute_get_utf8_value,
	 attribute );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf16_value_size",
	 ewf_test_attribute_get_utf16_value_size,
	 attribute );

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_attribute_get_utf16_value",
	 ewf_test_attribute_get_utf16_value,
	 attribute );

	/* Clean up
	 */
	result = libewf_attribute_free(
	          &attribute,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "attribute",
	 attribute );

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
	if( attribute != NULL )
	{
		libewf_attribute_free(
		 &attribute,
		 NULL );
	}
	if( lef_extended_attribute != NULL )
	{
		libewf_lef_extended_attribute_free(
		 &lef_extended_attribute,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}


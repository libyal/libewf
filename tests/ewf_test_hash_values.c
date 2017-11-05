/*
 * Library hash_values type test program
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
#include "ewf_test_libfvalue.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_hash_values.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_hash_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libfvalue_table_t *hash_values = NULL;
	int result                       = 0;

	/* Test regular cases
	 */
	result = libewf_hash_values_initialize(
	          &hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "hash_values",
	 hash_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_table_free(
	          &hash_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "hash_values",
	 hash_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_hash_values_initialize(
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

	/* Test libewf_hash_values_initialize with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_hash_values_initialize(
	          &hash_values,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( hash_values != NULL )
		{
			libfvalue_table_free(
			 &hash_values,
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
		 "hash_values",
		 hash_values );

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
	if( hash_values != NULL )
	{
		libfvalue_table_free(
		 &hash_values,
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
	 "libewf_hash_values_initialize",
	 ewf_test_hash_values_initialize );

	/* TODO: add tests for libewf_hash_values_parse_md5_hash */

	/* TODO: add tests for libewf_hash_values_parse_sha1_hash */

	/* TODO: add tests for libewf_hash_values_parse_xhash */

	/* TODO: add tests for libewf_hash_values_generate_xhash */

	/* TODO: add tests for libewf_hash_values_generate_md5_hash */

	/* TODO: add tests for libewf_hash_values_generate_sha1_hash */

	/* TODO: add tests for libewf_hash_values_get_identifier_size */

	/* TODO: add tests for libewf_hash_values_get_identifier */

	/* TODO: add tests for libewf_hash_values_get_utf8_value_size */

	/* TODO: add tests for libewf_hash_values_get_utf8_value */

	/* TODO: add tests for libewf_hash_values_set_utf8_value */

	/* TODO: add tests for libewf_hash_values_get_utf16_value_size */

	/* TODO: add tests for libewf_hash_values_get_utf16_value */

	/* TODO: add tests for libewf_hash_values_set_utf16_value */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


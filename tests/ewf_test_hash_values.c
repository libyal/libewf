/*
 * Library hash_values type test program
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

#include "../libewf/libewf_hash_values.h"
#include "../libewf/libewf_value_table.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_hash_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfvalue_table_t *hash_values  = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_hash_values_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

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
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libewf_hash_values_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_hash_values_initialize(
		          &hash_values,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

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

/* Tests the libewf_hash_values_parse_md5_hash function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_parse_md5_hash(
     void )
{
	uint8_t md5_hash[ 16 ] = {
		0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e };

	libcerror_error_t *error        = NULL;
	libfvalue_table_t *hash_values  = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 4;
	int test_number                 = 0;
#endif

	/* Initialize test
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

	/* Test regular cases
	 */
	result = libewf_hash_values_parse_md5_hash(
	          hash_values,
	          md5_hash,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO compare hash value */

	/* Test error cases
	 */
	result = libewf_hash_values_parse_md5_hash(
	          NULL,
	          md5_hash,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_parse_md5_hash(
	          hash_values,
	          NULL,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_parse_md5_hash(
	          hash_values,
	          md5_hash,
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

	result = libewf_hash_values_parse_md5_hash(
	          hash_values,
	          md5_hash,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_hash_values_parse_md5_hash with malloc failing in
		 * 1. libfvalue_value_type_initialize
		 * 2. libfvalue_value_set_identifier
		 * 3. libfvalue_value_set_data
		 * 4. libfvalue_table_set_value
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_hash_values_parse_md5_hash(
		          hash_values,
		          md5_hash,
		          16,
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

/* Tests the libewf_hash_values_parse_sha1_hash function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_parse_sha1_hash(
     void )
{
	uint8_t sha1_hash[ 20 ] = {
		0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90,
		0xaf, 0xd8, 0x07, 0x09 };

	libcerror_error_t *error        = NULL;
	libfvalue_table_t *hash_values  = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 4;
	int test_number                 = 0;
#endif

	/* Initialize test
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

	/* Test regular cases
	 */
	result = libewf_hash_values_parse_sha1_hash(
	          hash_values,
	          sha1_hash,
	          20,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO compare hash value */

	/* Test error cases
	 */
	result = libewf_hash_values_parse_sha1_hash(
	          NULL,
	          sha1_hash,
	          20,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_parse_sha1_hash(
	          hash_values,
	          NULL,
	          20,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_parse_sha1_hash(
	          hash_values,
	          sha1_hash,
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

	result = libewf_hash_values_parse_sha1_hash(
	          hash_values,
	          sha1_hash,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_hash_values_parse_sha1_hash with malloc failing in
		 * 1. libfvalue_value_type_initialize
		 * 2. libfvalue_value_set_identifier
		 * 3. libfvalue_value_set_data
		 * 4. libfvalue_table_set_value
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_hash_values_parse_sha1_hash(
		          hash_values,
		          sha1_hash,
		          20,
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

/* Tests the libewf_hash_values_parse_xhash function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_parse_xhash(
     void )
{
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *hash_values = NULL;
	char *xhash                    = "\xef\xbb\xbf<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<xhash>\n\t<md5>d41d8cd98f00b204e9800998ecf8427e</md5>\n</xhash>\n\n";
	int result                     = 0;

	/* Initialize test
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

	/* Test regular cases
	 */
	result = libewf_hash_values_parse_xhash(
	          hash_values,
	          (uint8_t *) xhash,
	          106,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO compare hash value */

	/* Test error cases
	 */
	result = libewf_hash_values_parse_xhash(
	          NULL,
	          (uint8_t *) xhash,
	          106,
	          &error );

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

/* Tests the libewf_hash_values_generate_xhash function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_generate_xhash(
     void )
{
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *hash_values = NULL;
	uint8_t *xhash                 = NULL;
	char *expected_xhash           = "\xef\xbb\xbf<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<xhash>\n\t<md5>d41d8cd98f00b204e9800998ecf8427e</md5>\n</xhash>\n\n";
	size_t xhash_size              = 0;
	int result                     = 0;

	/* Initialize test
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

	result = libewf_value_table_copy_value_from_utf8_string(
	          hash_values,
	          (uint8_t *) "md5",
	          3,
	          (uint8_t *) "d41d8cd98f00b204e9800998ecf8427e",
	          32,
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
	result = libewf_hash_values_generate_xhash(
	          hash_values,
	          &xhash,
	          &xhash_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "xhash_size",
	 xhash_size,
	 (size_t) 106 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          xhash,
	          expected_xhash,
	          106 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 xhash );

	xhash = NULL;

	/* Test error cases
	 */
	result = libewf_hash_values_generate_xhash(
	          hash_values,
	          NULL,
	          &xhash_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	xhash = (uint8_t *) 0x12345678UL;

	result = libewf_hash_values_generate_xhash(
	          hash_values,
	          &xhash,
	          &xhash_size,
	          &error );

	xhash = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_generate_xhash(
	          hash_values,
	          &xhash,
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

	/* Test libewf_hash_values_generate_xhash with libfvalue_table_get_number_of_values failing
	 */
	result = libewf_hash_values_generate_xhash(
	          NULL,
	          &xhash,
	          &xhash_size,
	          &error );

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

	/* Test libewf_hash_values_generate_xhash with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_hash_values_generate_xhash(
	          hash_values,
	          &xhash,
	          &xhash_size,
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
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( xhash != NULL )
	{
		memory_free(
		 xhash );
	}
	if( hash_values != NULL )
	{
		libfvalue_table_free(
		 &hash_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_hash_values_generate_md5_hash function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_generate_md5_hash(
     void )
{
	uint8_t md5_hash[ 16 ];

	libcerror_error_t *error       = NULL;
	libfvalue_table_t *hash_values = NULL;
	uint8_t md5_hash_set           = 0;
	int result                     = 0;

	/* Initialize test
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

	result = libewf_value_table_copy_value_from_utf8_string(
	          hash_values,
	          (uint8_t *) "md5",
	          3,
	          (uint8_t *) "d41d8cd98f00b204e9800998ecf8427e",
	          32,
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
	result = libewf_hash_values_generate_md5_hash(
	          hash_values,
	          md5_hash,
	          16,
	          &md5_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO compare MD5 hash */

	/* Test error cases
	 */
	result = libewf_hash_values_generate_md5_hash(
	          NULL,
	          md5_hash,
	          16,
	          &md5_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_generate_md5_hash(
	          hash_values,
	          NULL,
	          16,
	          &md5_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_generate_md5_hash(
	          hash_values,
	          md5_hash,
	          0,
	          &md5_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_generate_md5_hash(
	          hash_values,
	          md5_hash,
	          16,
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

/* Tests the libewf_hash_values_generate_sha1_hash function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_generate_sha1_hash(
     void )
{
	uint8_t sha1_hash[ 20 ];

	libcerror_error_t *error       = NULL;
	libfvalue_table_t *hash_values = NULL;
	uint8_t sha1_hash_set          = 0;
	int result                     = 0;

	/* Initialize test
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

	result = libewf_value_table_copy_value_from_utf8_string(
	          hash_values,
	          (uint8_t *) "sha1",
	          3,
	          (uint8_t *) "da39a3ee5e6b4b0d3255bfef95601890afd80709",
	          40,
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
	result = libewf_hash_values_generate_sha1_hash(
	          hash_values,
	          sha1_hash,
	          20,
	          &sha1_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO compare MD5 hash */

	/* Test error cases
	 */
	result = libewf_hash_values_generate_sha1_hash(
	          NULL,
	          sha1_hash,
	          20,
	          &sha1_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_generate_sha1_hash(
	          hash_values,
	          NULL,
	          20,
	          &sha1_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_generate_sha1_hash(
	          hash_values,
	          sha1_hash,
	          0,
	          &sha1_hash_set,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_hash_values_generate_sha1_hash(
	          hash_values,
	          sha1_hash,
	          20,
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

/* Tests the libewf_hash_values_get_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_get_identifier_size(
     void )
{
	libcerror_error_t *error       = NULL;
	libfvalue_table_t *hash_values = NULL;
	size_t identifier_size         = 0;
	int result                     = 0;

	/* Initialize test
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

	result = libewf_value_table_copy_value_from_utf8_string(
	          hash_values,
	          (uint8_t *) "md5",
	          3,
	          (uint8_t *) "d41d8cd98f00b204e9800998ecf8427e",
	          32,
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
	identifier_size = 0;

	result = libewf_hash_values_get_identifier_size(
	          hash_values,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "identifier_size",
	 identifier_size,
	 (size_t) 4 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	identifier_size = 0;

	result = libewf_hash_values_get_identifier_size(
	          NULL,
	          0,
	          &identifier_size,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_EQUAL_SIZE(
	 "identifier_size",
	 identifier_size,
	 (size_t) 0 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
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

/* Tests the libewf_hash_values_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_hash_values_get_identifier(
     void )
{
	uint8_t identifier[ 16 ];

	libcerror_error_t *error       = NULL;
	libfvalue_table_t *hash_values = NULL;
	int result                     = 0;

	/* Initialize test
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

	result = libewf_value_table_copy_value_from_utf8_string(
	          hash_values,
	          (uint8_t *) "md5",
	          3,
	          (uint8_t *) "d41d8cd98f00b204e9800998ecf8427e",
	          32,
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
	result = libewf_hash_values_get_identifier(
	          hash_values,
	          0,
	          identifier,
	          16,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          identifier,
	          "md5",
	          4 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libewf_hash_values_get_identifier(
	          NULL,
	          0,
	          identifier,
	          16,
	          &error );

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

	EWF_TEST_RUN(
	 "libewf_hash_values_parse_md5_hash",
	 ewf_test_hash_values_parse_md5_hash );

	EWF_TEST_RUN(
	 "libewf_hash_values_parse_sha1_hash",
	 ewf_test_hash_values_parse_sha1_hash );

	EWF_TEST_RUN(
	 "libewf_hash_values_parse_xhash",
	 ewf_test_hash_values_parse_xhash );

	EWF_TEST_RUN(
	 "libewf_hash_values_generate_xhash",
	 ewf_test_hash_values_generate_xhash );

	EWF_TEST_RUN(
	 "libewf_hash_values_generate_md5_hash",
	 ewf_test_hash_values_generate_md5_hash );

	EWF_TEST_RUN(
	 "libewf_hash_values_generate_sha1_hash",
	 ewf_test_hash_values_generate_sha1_hash );

	EWF_TEST_RUN(
	 "libewf_hash_values_get_identifier_size",
	 ewf_test_hash_values_get_identifier_size );

	EWF_TEST_RUN(
	 "libewf_hash_values_get_identifier",
	 ewf_test_hash_values_get_identifier );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


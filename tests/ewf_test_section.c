/*
 * Library section type test program
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

#include "../libewf/libewf_section.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_section_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libewf_section_t *section       = NULL;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_section_initialize(
	          &section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "section",
	 section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_free(
	          &section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "section",
	 section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_section_initialize(
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

	section = (libewf_section_t *) 0x12345678UL;

	result = libewf_section_initialize(
	          &section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	section = NULL;

#if defined( HAVE_EWF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libewf_section_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_section_initialize(
		          &section,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( section != NULL )
			{
				libewf_section_free(
				 &section,
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
			 "section",
			 section );

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
		/* Test libewf_section_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_section_initialize(
		          &section,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( section != NULL )
			{
				libewf_section_free(
				 &section,
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
			 "section",
			 section );

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
	if( section != NULL )
	{
		libewf_section_free(
		 &section,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_section_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_section_free(
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

/* Tests the libewf_section_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_clone(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_section_t *destination_section = NULL;
	libewf_section_t *source_section      = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_section_initialize(
	          &source_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_section",
	 source_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_section_clone(
	          &destination_section,
	          source_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_section",
	 destination_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_free(
	          &destination_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_section",
	 destination_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_clone(
	          &destination_section,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_section",
	 destination_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_section_clone(
	          NULL,
	          source_section,
	          &error );

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
	result = libewf_section_free(
	          &source_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_section",
	 source_section );

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
	if( destination_section != NULL )
	{
		libewf_section_free(
		 &destination_section,
		 NULL );
	}
	if( source_section != NULL )
	{
		libewf_section_free(
		 &source_section,
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
	 "libewf_section_initialize",
	 ewf_test_section_initialize );

	EWF_TEST_RUN(
	 "libewf_section_free",
	 ewf_test_section_free );

	EWF_TEST_RUN(
	 "libewf_section_clone",
	 ewf_test_section_clone );

	/* TODO: add tests for libewf_section_test_zero */

	/* TODO: add tests for libewf_section_get_data_offset */

	/* TODO: add tests for libewf_section_set_values */

	/* TODO: add tests for libewf_section_descriptor_read */

	/* TODO: add tests for libewf_section_descriptor_write */

	/* TODO: add tests for libewf_section_read_data */

	/* TODO: add tests for libewf_section_write_data */

	/* TODO: add tests for libewf_section_compressed_string_read */

	/* TODO: add tests for libewf_section_write_compressed_string */

	/* TODO: add tests for libewf_section_data_read */

	/* TODO: add tests for libewf_section_data_write */

	/* TODO: add tests for libewf_section_digest_read */

	/* TODO: add tests for libewf_section_digest_write */

	/* TODO: add tests for libewf_section_error_read */

	/* TODO: add tests for libewf_section_error_write */

	/* TODO: add tests for libewf_section_md5_hash_read */

	/* TODO: add tests for libewf_section_md5_hash_write */

	/* TODO: add tests for libewf_section_sha1_hash_read */

	/* TODO: add tests for libewf_section_sha1_hash_write */

	/* TODO: add tests for libewf_section_ltree_read */

	/* TODO: add tests for libewf_section_ltree_write */

	/* TODO: add tests for libewf_section_sectors_write */

	/* TODO: add tests for libewf_section_session_read */

	/* TODO: add tests for libewf_section_session_write */

	/* TODO: add tests for libewf_section_table_read */

	/* TODO: add tests for libewf_section_table_write */

	/* TODO: add tests for libewf_section_volume_e01_read */

	/* TODO: add tests for libewf_section_volume_e01_write */

	/* TODO: add tests for libewf_section_volume_s01_read */

	/* TODO: add tests for libewf_section_volume_s01_write */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


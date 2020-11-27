/*
 * Library section_descriptor type test program
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
#include <byte_stream.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_functions.h"
#include "ewf_test_libbfio.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_section_descriptor.h"

uint8_t ewf_test_section_descriptor_data1[ 76 ] = {
	0x64, 0x6f, 0x6e, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xa1, 0x21, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x02, 0x03, 0x9f };

uint8_t ewf_test_section_descriptor_data2[ 64 ] = {
	0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x63, 0x30 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_section_descriptor_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_descriptor_initialize(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_section_descriptor_t *section_descriptor = NULL;
	int result                                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 1;
	int number_of_memset_fail_tests                 = 1;
	int test_number                                 = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_section_descriptor_initialize(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "section_descriptor",
	 section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_descriptor_free(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "section_descriptor",
	 section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_section_descriptor_initialize(
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

	section_descriptor = (libewf_section_descriptor_t *) 0x12345678UL;

	result = libewf_section_descriptor_initialize(
	          &section_descriptor,
	          &error );

	section_descriptor = NULL;

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
		/* Test libewf_section_descriptor_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_section_descriptor_initialize(
		          &section_descriptor,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( section_descriptor != NULL )
			{
				libewf_section_descriptor_free(
				 &section_descriptor,
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
			 "section_descriptor",
			 section_descriptor );

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
		/* Test libewf_section_descriptor_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_section_descriptor_initialize(
		          &section_descriptor,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( section_descriptor != NULL )
			{
				libewf_section_descriptor_free(
				 &section_descriptor,
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
			 "section_descriptor",
			 section_descriptor );

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
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_section_descriptor_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_descriptor_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_section_descriptor_free(
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

/* Tests the libewf_section_descriptor_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_descriptor_clone(
     void )
{
	libcerror_error_t *error                                    = NULL;
	libewf_section_descriptor_t *destination_section_descriptor = NULL;
	libewf_section_descriptor_t *source_section_descriptor      = NULL;
	int result                                                  = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests                             = 1;
	int test_number                                             = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                             = 1;
#endif
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libewf_section_descriptor_initialize(
	          &source_section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_section_descriptor",
	 source_section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_section_descriptor_clone(
	          &destination_section_descriptor,
	          source_section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_section_descriptor",
	 destination_section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_descriptor_free(
	          &destination_section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_section_descriptor",
	 destination_section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_descriptor_clone(
	          &destination_section_descriptor,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_section_descriptor",
	 destination_section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_section_descriptor_clone(
	          NULL,
	          source_section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	destination_section_descriptor = (libewf_section_descriptor_t *) 0x12345678UL;

	result = libewf_section_descriptor_clone(
	          &destination_section_descriptor,
	          source_section_descriptor,
	          &error );

	destination_section_descriptor = NULL;

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
		/* Test libewf_section_descriptor_clone with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_section_descriptor_clone(
		          &destination_section_descriptor,
		          source_section_descriptor,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( destination_section_descriptor != NULL )
			{
				libewf_section_descriptor_free(
				 &destination_section_descriptor,
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
			 "destination_section_descriptor",
			 destination_section_descriptor );

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
		/* Test libewf_section_descriptor_clone with memcpy failing
		 */
		ewf_test_memcpy_attempts_before_fail = test_number;

		result = libewf_section_descriptor_clone(
		          &destination_section_descriptor,
		          source_section_descriptor,
		          &error );

		if( ewf_test_memcpy_attempts_before_fail != -1 )
		{
			ewf_test_memcpy_attempts_before_fail = -1;

			if( destination_section_descriptor != NULL )
			{
				libewf_section_descriptor_free(
				 &destination_section_descriptor,
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
			 "destination_section_descriptor",
			 destination_section_descriptor );

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
	result = libewf_section_descriptor_free(
	          &source_section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_section_descriptor",
	 source_section_descriptor );

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
	if( destination_section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &destination_section_descriptor,
		 NULL );
	}
	if( source_section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &source_section_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_section_descriptor_read_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_descriptor_read_data(
     void )
{
	libcerror_error_t *error                        = NULL;
	libewf_section_descriptor_t *section_descriptor = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_section_descriptor_initialize(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "section_descriptor",
	 section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          ewf_test_section_descriptor_data1,
	          76,
	          0,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          ewf_test_section_descriptor_data2,
	          64,
	          0x031f4b40UL,
	          2,
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
	result = libewf_section_descriptor_read_data(
	          NULL,
	          ewf_test_section_descriptor_data1,
	          76,
	          0,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          NULL,
	          76,
	          0,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          ewf_test_section_descriptor_data1,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          ewf_test_section_descriptor_data1,
	          0,
	          0,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          ewf_test_section_descriptor_data1,
	          76,
	          0,
	          0xff,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test with an invalid checksum
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_section_descriptor_data1[ 72 ] ),
	 0xffffffffUL );

	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          ewf_test_section_descriptor_data1,
	          76,
	          0,
	          1,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_section_descriptor_data1[ 72 ] ),
	 0x9f03026aUL );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libewf_section_descriptor_read_data with memcpy of set_identifier failing
	 */
	ewf_test_memcpy_attempts_before_fail = 0;

	result = libewf_section_descriptor_read_data(
	          section_descriptor,
	          ewf_test_section_descriptor_data1,
	          76,
	          0,
	          1,
	          &error );

	if( ewf_test_memcpy_attempts_before_fail != -1 )
	{
		ewf_test_memcpy_attempts_before_fail = -1;
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
#endif /* defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	/* Clean up
	 */
	result = libewf_section_descriptor_free(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "section_descriptor",
	 section_descriptor );

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
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_section_descriptor_read_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_descriptor_read_file_io_pool(
     void )
{
	libbfio_pool_t *file_io_pool                    = NULL;
	libcerror_error_t *error                        = NULL;
	libewf_section_descriptor_t *section_descriptor = NULL;
	ssize_t read_count                              = 0;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_section_descriptor_initialize(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "section_descriptor",
	 section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	section_descriptor->data_size = 76;

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          ewf_test_section_descriptor_data1,
	          76,
	          LIBBFIO_OPEN_READ,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_pool",
	 file_io_pool );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_count = libewf_section_descriptor_read_file_io_pool(
	              section_descriptor,
	              file_io_pool,
	              0,
	              0,
	              1,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 76 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libewf_section_descriptor_read_file_io_pool(
	              NULL,
	              file_io_pool,
	              0,
	              0,
	              1,
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

	/* Clean up file IO pool
	 */
	result = ewf_test_close_file_io_pool(
	          &file_io_pool,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO test with libewf_section_read_data failing */

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          ewf_test_section_descriptor_data1,
	          776,
	          LIBBFIO_OPEN_READ,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_pool",
	 file_io_pool );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test with libewf_section_descriptor_read_data failing due to an invalid checksum
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_section_descriptor_data1[ 72 ] ),
	 0xffffffffUL );

	read_count = libewf_section_descriptor_read_file_io_pool(
	              section_descriptor,
	              file_io_pool,
	              0,
	              0,
	              1,
	              &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_section_descriptor_data1[ 72 ] ),
	 0x9f03026aUL );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up file IO pool
	 */
	result = ewf_test_close_file_io_pool(
	          &file_io_pool,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libewf_section_descriptor_free(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "section_descriptor",
	 section_descriptor );

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
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_section_descriptor_write_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_descriptor_write_data(
     void )
{
	uint8_t section_descriptor_data[ 128 ];

	libcerror_error_t *error                        = NULL;
	libewf_section_descriptor_t *section_descriptor = NULL;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_section_descriptor_initialize(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "section_descriptor",
	 section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_section_descriptor_write_data(
	          section_descriptor,
	          section_descriptor_data,
	          76,
	          1,
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
	result = libewf_section_descriptor_write_data(
	          NULL,
	          section_descriptor_data,
	          76,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_section_descriptor_write_data(
	          section_descriptor,
	          NULL,
	          76,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_section_descriptor_write_data(
	          section_descriptor,
	          section_descriptor_data,
	          (size_t) SSIZE_MAX + 1,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_section_descriptor_write_data(
	          section_descriptor,
	          section_descriptor_data,
	          76,
	          0xff,
	          &error );

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

	/* Test libewf_section_descriptor_write_data with memset failing
	 */
	ewf_test_memset_attempts_before_fail = 0;

	result = libewf_section_descriptor_write_data(
	          section_descriptor,
	          section_descriptor_data,
	          76,
	          1,
	          &error );

	if( ewf_test_memset_attempts_before_fail != -1 )
	{
		ewf_test_memset_attempts_before_fail = -1;
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
	result = libewf_section_descriptor_free(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "section_descriptor",
	 section_descriptor );

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
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_section_descriptor_write_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_section_descriptor_write_file_io_pool(
     void )
{
	uint8_t section_descriptor_data[ 128 ];

	libbfio_pool_t *file_io_pool                    = NULL;
	libcerror_error_t *error                        = NULL;
	libewf_section_descriptor_t *section_descriptor = NULL;
	ssize_t write_count                             = 0;
	off64_t offset                                  = 0;
	int result                                      = 0;

	/* Initialize test
	 */
	result = libewf_section_descriptor_initialize(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "section_descriptor",
	 section_descriptor );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          section_descriptor_data,
	          128,
	          LIBBFIO_OPEN_WRITE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_pool",
	 file_io_pool );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	write_count = libewf_section_descriptor_write_file_io_pool(
	               section_descriptor,
	               file_io_pool,
	               0,
	               1,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) 76 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libbfio_pool_seek_offset(
	          file_io_pool,
	          0,
	          0,
	          SEEK_SET,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	write_count = libewf_section_descriptor_write_file_io_pool(
	               NULL,
	               file_io_pool,
	               0,
	               1,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	write_count = libewf_section_descriptor_write_file_io_pool(
	               section_descriptor,
	               NULL,
	               0,
	               1,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_section_descriptor_write_file_io_pool with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	write_count = libewf_section_descriptor_write_file_io_pool(
	               section_descriptor,
	               file_io_pool,
	               0,
	               1,
	               &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "write_count",
		 write_count,
		 (ssize_t) -1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up file IO pool
	 */
	result = ewf_test_close_file_io_pool(
	          &file_io_pool,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libewf_section_descriptor_free(
	          &section_descriptor,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "section_descriptor",
	 section_descriptor );

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
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( section_descriptor != NULL )
	{
		libewf_section_descriptor_free(
		 &section_descriptor,
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
	 "libewf_section_descriptor_initialize",
	 ewf_test_section_descriptor_initialize );

	EWF_TEST_RUN(
	 "libewf_section_descriptor_free",
	 ewf_test_section_descriptor_free );

	EWF_TEST_RUN(
	 "libewf_section_descriptor_clone",
	 ewf_test_section_descriptor_clone );

/* TODO add tests for libewf_section_descriptor_set */

	EWF_TEST_RUN(
	 "libewf_section_descriptor_read_data",
	 ewf_test_section_descriptor_read_data );

	EWF_TEST_RUN(
	 "libewf_section_descriptor_read_file_io_pool",
	 ewf_test_section_descriptor_read_file_io_pool );

	EWF_TEST_RUN(
	 "libewf_section_descriptor_write_data",
	 ewf_test_section_descriptor_write_data );

	EWF_TEST_RUN(
	 "libewf_section_descriptor_write_file_io_pool",
	 ewf_test_section_descriptor_write_file_io_pool );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


/*
 * Library table section functions test program
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

#include "../libewf/libewf_io_handle.h"
#include "../libewf/libewf_table_section.h"

uint8_t ewf_test_table_section_data1[ 32 ] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x28, 0x00, 0x00, 0x08, 0x00, 0x80, 0x34, 0x69, 0xd5, 0x60 };

uint8_t ewf_test_table_section_data2[ 64 ] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xa0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x7b, 0x12, 0x2e, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_table_section_initialize function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_table_section_initialize(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_table_section_t *table_section = NULL;
	int result                            = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests       = 1;
	int number_of_memset_fail_tests       = 1;
	int test_number                       = 0;
#endif

	/* Test regular cases
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_table_section_initialize(
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

	table_section = (libewf_table_section_t *) 0x12345678UL;

	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	table_section = NULL;

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
		/* Test libewf_table_section_initialize with malloc failing
		 */
		ewf_test_malloc_attempts_before_fail = test_number;

		result = libewf_table_section_initialize(
		          &table_section,
		          &error );

		if( ewf_test_malloc_attempts_before_fail != -1 )
		{
			ewf_test_malloc_attempts_before_fail = -1;

			if( table_section != NULL )
			{
				libewf_table_section_free(
				 &table_section,
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
			 "table_section",
			 table_section );

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
		/* Test libewf_table_section_initialize with memset failing
		 */
		ewf_test_memset_attempts_before_fail = test_number;

		result = libewf_table_section_initialize(
		          &table_section,
		          &error );

		if( ewf_test_memset_attempts_before_fail != -1 )
		{
			ewf_test_memset_attempts_before_fail = -1;

			if( table_section != NULL )
			{
				libewf_table_section_free(
				 &table_section,
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
			 "table_section",
			 table_section );

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
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_table_section_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_table_section_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_table_section_free(
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

/* Tests the libewf_table_section_read_header_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_table_section_read_header_data(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_io_handle_t *io_handle         = NULL;
	libewf_table_section_t *table_section = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_table_section_read_header_data(
	          table_section,
	          io_handle,
	          ewf_test_table_section_data1,
	          24,
	          1,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_table_section_read_header_data(
	          table_section,
	          io_handle,
	          ewf_test_table_section_data2,
	          32,
	          2,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_table_section_read_header_data(
	          NULL,
	          io_handle,
	          ewf_test_table_section_data1,
	          24,
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

	result = libewf_table_section_read_header_data(
	          table_section,
	          NULL,
	          ewf_test_table_section_data1,
	          24,
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

	result = libewf_table_section_read_header_data(
	          table_section,
	          io_handle,
	          NULL,
	          24,
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

	result = libewf_table_section_read_header_data(
	          table_section,
	          io_handle,
	          ewf_test_table_section_data1,
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

	result = libewf_table_section_read_header_data(
	          table_section,
	          io_handle,
	          ewf_test_table_section_data1,
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

	result = libewf_table_section_read_header_data(
	          table_section,
	          io_handle,
	          ewf_test_table_section_data1,
	          24,
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
	 &( ewf_test_table_section_data1[ 20 ] ),
	 0xffffffffUL );

	result = libewf_table_section_read_header_data(
	          table_section,
	          io_handle,
	          ewf_test_table_section_data1,
	          24,
	          1,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_table_section_data1[ 20 ] ),
	 0x00280002UL );

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
	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

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
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_table_section_read_footer_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_table_section_read_footer_data(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_table_section_t *table_section = NULL;
	uint32_t stored_checksum              = 0;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_table_section_read_footer_data(
	          table_section,
	          &( ewf_test_table_section_data1[ 28 ] ),
	          4,
	          1,
	          &stored_checksum,
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
	result = libewf_table_section_read_footer_data(
	          table_section,
	          &( ewf_test_table_section_data2[ 40 ] ),
	          16,
	          2,
	          &stored_checksum,
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
	result = libewf_table_section_read_footer_data(
	          NULL,
	          &( ewf_test_table_section_data1[ 28 ] ),
	          4,
	          1,
	          &stored_checksum,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_read_footer_data(
	          table_section,
	          NULL,
	          4,
	          1,
	          &stored_checksum,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_read_footer_data(
	          table_section,
	          &( ewf_test_table_section_data1[ 28 ] ),
	          (size_t) SSIZE_MAX + 1,
	          1,
	          &stored_checksum,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_read_footer_data(
	          table_section,
	          &( ewf_test_table_section_data1[ 28 ] ),
	          0,
	          1,
	          &stored_checksum,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_read_footer_data(
	          table_section,
	          &( ewf_test_table_section_data1[ 28 ] ),
	          4,
	          0xff,
	          &stored_checksum,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_read_footer_data(
	          table_section,
	          &( ewf_test_table_section_data1[ 28 ] ),
	          4,
	          1,
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
	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

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
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_table_section_read_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_table_section_read_file_io_pool(
     void )
{
	libbfio_pool_t *file_io_pool          = NULL;
	libcerror_error_t *error              = NULL;
	libewf_io_handle_t *io_handle         = NULL;
	libewf_table_section_t *table_section = NULL;
	ssize_t read_count                    = 0;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_io_handle_initialize(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          ewf_test_table_section_data2,
	          64,
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
	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              io_handle,
	              file_io_pool,
	              0,
	              2,
	              LIBEWF_SEGMENT_FILE_TYPE_EWF2,
	              64,
	              0,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 64 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          ewf_test_table_section_data1,
	          32,
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
	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              io_handle,
	              file_io_pool,
	              0,
	              1,
	              LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	              32,
	              0,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 32 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libewf_table_section_read_file_io_pool(
	              NULL,
	              io_handle,
	              file_io_pool,
	              0,
	              1,
	              LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	              32,
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

	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              io_handle,
	              file_io_pool,
	              0,
	              0xff,
	              LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	              32,
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

	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              io_handle,
	              file_io_pool,
	              0,
	              1,
	              LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	              (size_t) SSIZE_MAX + 1,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_table_section_read_file_io_pool with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              io_handle,
	              file_io_pool,
	              0,
	              1,
	              LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	              32,
	              0,
	              &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
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

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          ewf_test_table_section_data1,
	          32,
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

	/* Test with libewf_table_section_read_data failing due to an invalid checksum
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_table_section_data1[ 20 ] ),
	 0xffffffffUL );

	read_count = libewf_table_section_read_file_io_pool(
	              table_section,
	              io_handle,
	              file_io_pool,
	              0,
	              1,
	              LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	              32,
	              0,
	              &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_table_section_data1[ 20 ] ),
	 0x00280002UL );

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
	result = libewf_io_handle_free(
	          &io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

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
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_table_section_write_header_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_table_section_write_header_data(
     void )
{
	uint8_t section_data[ 128 ];

	libcerror_error_t *error              = NULL;
	libewf_table_section_t *table_section = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_table_section_write_header_data(
	          table_section,
	          section_data,
	          128,
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
	result = libewf_table_section_write_header_data(
	          NULL,
	          section_data,
	          128,
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

	result = libewf_table_section_write_header_data(
	          table_section,
	          NULL,
	          128,
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

	result = libewf_table_section_write_header_data(
	          table_section,
	          section_data,
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

	result = libewf_table_section_write_header_data(
	          table_section,
	          section_data,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_table_section_write_header_data with memset of data failing
	 */
	ewf_test_memset_attempts_before_fail = 0;

	result = libewf_table_section_write_header_data(
	          table_section,
	          section_data,
	          128,
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
	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

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
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_table_section_write_footer_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_table_section_write_footer_data(
     void )
{
	uint8_t section_data[ 128 ];

	libcerror_error_t *error              = NULL;
	libewf_table_section_t *table_section = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_table_section_initialize(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "table_section",
	 table_section );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_table_section_write_footer_data(
	          table_section,
	          section_data,
	          128,
	          1,
	          0x12345678UL,
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
	result = libewf_table_section_write_footer_data(
	          NULL,
	          section_data,
	          128,
	          1,
	          0x12345678UL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_write_footer_data(
	          table_section,
	          NULL,
	          128,
	          1,
	          0x12345678UL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_write_footer_data(
	          table_section,
	          section_data,
	          (size_t) SSIZE_MAX + 1,
	          1,
	          0x12345678UL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_write_footer_data(
	          table_section,
	          section_data,
	          0,
	          1,
	          0x12345678UL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_table_section_write_footer_data(
	          table_section,
	          section_data,
	          128,
	          0xff,
	          0x12345678UL,
	          &error );

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

	/* Test libewf_table_section_write_footer_data with memset of data failing
	 */
	ewf_test_memset_attempts_before_fail = 0;

	result = libewf_table_section_write_footer_data(
	          table_section,
	          section_data,
	          128,
	          1,
	          0x12345678UL,
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
	result = libewf_table_section_free(
	          &table_section,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "table_section",
	 table_section );

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
	if( table_section != NULL )
	{
		libewf_table_section_free(
		 &table_section,
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
	 "libewf_table_section_initialize",
	 ewf_test_table_section_initialize );

	EWF_TEST_RUN(
	 "libewf_table_section_free",
	 ewf_test_table_section_free );

	EWF_TEST_RUN(
	 "libewf_table_section_read_header_data",
	 ewf_test_table_section_read_header_data );

	EWF_TEST_RUN(
	 "libewf_table_section_read_footer_data",
	 ewf_test_table_section_read_footer_data );

	EWF_TEST_RUN(
	 "libewf_table_section_read_file_io_pool",
	 ewf_test_table_section_read_file_io_pool );

	EWF_TEST_RUN(
	 "libewf_table_section_write_header_data",
	 ewf_test_table_section_write_header_data );

	EWF_TEST_RUN(
	 "libewf_table_section_write_footer_data",
	 ewf_test_table_section_write_footer_data );

	/* TODO add tests for libewf_table_section_write_file_io_pool */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


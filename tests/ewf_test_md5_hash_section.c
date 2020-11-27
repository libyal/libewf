/*
 * Library MD5 hash section functions test program
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

#include "../libewf/libewf_hash_sections.h"
#include "../libewf/libewf_io_handle.h"
#include "../libewf/libewf_md5_hash_section.h"
#include "../libewf/libewf_section_descriptor.h"

uint8_t ewf_test_md5_hash_section_data1[ 32 ] = {
	0x18, 0x01, 0x74, 0x48, 0x43, 0xf5, 0x66, 0xc6, 0x2a, 0x60, 0x45, 0xc9, 0x26, 0x65, 0x1a, 0xf7,
	0x6e, 0x06, 0xa9, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_md5_hash_section_read_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_md5_hash_section_read_data(
     void )
{
	libcerror_error_t *error              = NULL;
	libewf_hash_sections_t *hash_sections = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_hash_sections_initialize(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "hash_sections",
	 hash_sections );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_md5_hash_section_read_data(
	          ewf_test_md5_hash_section_data1,
	          32,
	          2,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO test with empty MD5 */

	/* Test error cases
	 */
	result = libewf_md5_hash_section_read_data(
	          NULL,
	          32,
	          2,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_read_data(
	          ewf_test_md5_hash_section_data1,
	          (size_t) SSIZE_MAX + 1,
	          2,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_read_data(
	          ewf_test_md5_hash_section_data1,
	          0,
	          2,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_read_data(
	          ewf_test_md5_hash_section_data1,
	          32,
	          0,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_read_data(
	          ewf_test_md5_hash_section_data1,
	          32,
	          2,
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

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libewf_md5_hash_section_read_data with memcpy of md5_hash failing
	 */
	ewf_test_memcpy_attempts_before_fail = 0;

	result = libewf_md5_hash_section_read_data(
	          ewf_test_md5_hash_section_data1,
	          32,
	          2,
	          hash_sections,
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

	/* Test with an invalid checksum
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_md5_hash_section_data1[ 16 ] ),
	 0xffffffffUL );

	result = libewf_md5_hash_section_read_data(
	          ewf_test_md5_hash_section_data1,
	          32,
	          2,
	          hash_sections,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_md5_hash_section_data1[ 16 ] ),
	 0x30a9066eUL );

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
	result = libewf_hash_sections_free(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "hash_sections",
	 hash_sections );

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
	if( hash_sections != NULL )
	{
		libewf_hash_sections_free(
		 &hash_sections,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_md5_hash_section_read_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_md5_hash_section_read_file_io_pool(
     void )
{
	libbfio_pool_t *file_io_pool                    = NULL;
	libcerror_error_t *error                        = NULL;
	libewf_hash_sections_t *hash_sections           = NULL;
	libewf_io_handle_t *io_handle                   = NULL;
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

	section_descriptor->data_size = 32;

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

	result = libewf_hash_sections_initialize(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "hash_sections",
	 hash_sections );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          ewf_test_md5_hash_section_data1,
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
	read_count = libewf_md5_hash_section_read_file_io_pool(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              0,
	              2,
	              hash_sections,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 32 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libewf_md5_hash_section_read_file_io_pool(
	              NULL,
	              io_handle,
	              file_io_pool,
	              0,
	              2,
	              hash_sections,
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
	          ewf_test_md5_hash_section_data1,
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

	/* Test with libewf_md5_hash_section_read_data failing due to an invalid checksum
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_md5_hash_section_data1[ 16 ] ),
	 0xffffffffUL );

	read_count = libewf_md5_hash_section_read_file_io_pool(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              0,
	              2,
	              hash_sections,
	              &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( ewf_test_md5_hash_section_data1[ 16 ] ),
	 0x30a9066eUL );

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
	result = libewf_hash_sections_free(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "hash_sections",
	 hash_sections );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	if( hash_sections != NULL )
	{
		libewf_hash_sections_free(
		 &hash_sections,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
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

/* Tests the libewf_md5_hash_section_write_data function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_md5_hash_section_write_data(
     void )
{
	uint8_t section_data[ 32 ];

	libcerror_error_t *error              = NULL;
	libewf_hash_sections_t *hash_sections = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libewf_hash_sections_initialize(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "hash_sections",
	 hash_sections );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	hash_sections->md5_hash_set = 1;

	/* Test regular cases
	 */
	result = libewf_md5_hash_section_write_data(
	          section_data,
	          32,
	          2,
	          hash_sections,
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
	result = libewf_md5_hash_section_write_data(
	          NULL,
	          32,
	          2,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_write_data(
	          section_data,
	          (size_t) SSIZE_MAX + 1,
	          2,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_write_data(
	          section_data,
	          0,
	          2,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_write_data(
	          section_data,
	          32,
	          0,
	          hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_md5_hash_section_write_data(
	          section_data,
	          32,
	          2,
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

#if defined( HAVE_EWF_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libewf_md5_hash_section_write_data with memset of data failing
	 */
	ewf_test_memset_attempts_before_fail = 0;

	result = libewf_md5_hash_section_write_data(
	          section_data,
	          32,
	          2,
	          hash_sections,
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
	/* Test libewf_md5_hash_section_write_data with memcpy of data->md5_hash failing
	 */
	ewf_test_memcpy_attempts_before_fail = 0;

	result = libewf_md5_hash_section_write_data(
	          section_data,
	          32,
	          2,
	          hash_sections,
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
	result = libewf_hash_sections_free(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "hash_sections",
	 hash_sections );

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
	if( hash_sections != NULL )
	{
		libewf_hash_sections_free(
		 &hash_sections,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_md5_hash_section_write_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_md5_hash_section_write_file_io_pool(
     void )
{
	uint8_t section_data[ 256 ];

	libbfio_pool_t *file_io_pool                    = NULL;
	libcerror_error_t *error                        = NULL;
	libewf_hash_sections_t *hash_sections           = NULL;
	libewf_io_handle_t *io_handle                   = NULL;
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

	result = libewf_hash_sections_initialize(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "hash_sections",
	 hash_sections );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	hash_sections->md5_hash_set = 1;

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          section_data,
	          256,
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
	write_count = libewf_md5_hash_section_write_file_io_pool(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               0,
	               1,
	               0,
	               hash_sections,
	               &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "write_count",
	 write_count,
	 (ssize_t) 112 );

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

	write_count = libewf_md5_hash_section_write_file_io_pool(
	               NULL,
	               io_handle,
	               file_io_pool,
	               0,
	               1,
	               0,
	               hash_sections,
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

	write_count = libewf_md5_hash_section_write_file_io_pool(
	               section_descriptor,
	               NULL,
	               file_io_pool,
	               0,
	               1,
	               0,
	               hash_sections,
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

	write_count = libewf_md5_hash_section_write_file_io_pool(
	               section_descriptor,
	               io_handle,
	               NULL,
	               0,
	               1,
	               0,
	               hash_sections,
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

	write_count = libewf_md5_hash_section_write_file_io_pool(
	               section_descriptor,
	               io_handle,
	               file_io_pool,
	               0,
	               1,
	               0,
	               NULL,
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
	result = libewf_hash_sections_free(
	          &hash_sections,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "hash_sections",
	 hash_sections );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	if( hash_sections != NULL )
	{
		libewf_hash_sections_free(
		 &hash_sections,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
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
	 "libewf_md5_hash_section_read_data",
	 ewf_test_md5_hash_section_read_data );

	EWF_TEST_RUN(
	 "libewf_md5_hash_section_read_file_io_pool",
	 ewf_test_md5_hash_section_read_file_io_pool );

	EWF_TEST_RUN(
	 "libewf_md5_hash_section_write_data",
	 ewf_test_md5_hash_section_write_data );

	EWF_TEST_RUN(
	 "libewf_md5_hash_section_write_file_io_pool",
	 ewf_test_md5_hash_section_write_file_io_pool );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


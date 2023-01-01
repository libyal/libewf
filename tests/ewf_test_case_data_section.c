/*
 * Library case_data section functions test program
 *
 * Copyright (C) 2006-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include "ewf_test_libfvalue.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_case_data_section.h"
#include "../libewf/libewf_definitions.h"
#include "../libewf/libewf_header_values.h"
#include "../libewf/libewf_io_handle.h"
#include "../libewf/libewf_media_values.h"
#include "../libewf/libewf_read_io_handle.h"
#include "../libewf/libewf_section_descriptor.h"

uint8_t ewf_test_case_data_section_data1[ 160 ] = {
	0x48, 0xc7, 0x5d, 0x8e, 0xcb, 0x0e, 0xc2, 0x20, 0x14, 0x05, 0xcf, 0x96, 0x7e, 0x85, 0x89, 0x7b,
	0x23, 0xf6, 0x61, 0xfc, 0x92, 0xae, 0xd1, 0x12, 0xe3, 0x42, 0x34, 0x96, 0xb4, 0xfd, 0x7a, 0x75,
	0xb8, 0xa9, 0x1b, 0x43, 0x18, 0x06, 0xee, 0x8b, 0xcf, 0xdb, 0xab, 0xd2, 0x5d, 0x41, 0x37, 0x25,
	0x2c, 0xe1, 0x4e, 0x17, 0x4e, 0xa7, 0xb8, 0x72, 0x81, 0x49, 0x19, 0x06, 0x4d, 0xf0, 0xa1, 0x11,
	0xe6, 0xf5, 0x25, 0x9b, 0x9f, 0xad, 0xea, 0x09, 0x47, 0xf3, 0xab, 0x5e, 0x70, 0xc6, 0x4b, 0xcf,
	0x40, 0xd7, 0x68, 0x19, 0x81, 0x78, 0xd4, 0xd6, 0xfa, 0x4e, 0xcc, 0x1c, 0x56, 0x5f, 0x2c, 0xa7,
	0xfc, 0x21, 0x5a, 0x65, 0x62, 0x4a, 0xc6, 0xcb, 0xa4, 0xa3, 0x76, 0xda, 0xeb, 0x64, 0x6c, 0xb9,
	0xf7, 0x96, 0x37, 0x90, 0x31, 0x13, 0xdf, 0x10, 0x77, 0xf2, 0x6a, 0x88, 0x7a, 0xbc, 0x56, 0xa7,
	0x03, 0xfe, 0xff, 0xda, 0xd2, 0xa1, 0xb6, 0xfa, 0xc6, 0xe8, 0xd9, 0x1d, 0xfe, 0x63, 0xc5, 0xfa,
	0x02, 0xcd, 0xb2, 0x25, 0x97, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_case_data_section_read_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_case_data_section_read_file_io_pool(
     void )
{
	libbfio_pool_t *file_io_pool                    = NULL;
	libcerror_error_t *error                        = NULL;
	libewf_io_handle_t *io_handle                   = NULL;
	libewf_media_values_t *media_values             = NULL;
	libewf_read_io_handle_t *read_io_handle         = NULL;
	libewf_section_descriptor_t *section_descriptor = NULL;
	libfvalue_table_t *header_values                = NULL;
	ssize_t read_count                              = 0;
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

	section_descriptor->type      = LIBEWF_SECTION_TYPE_CASE_DATA;
	section_descriptor->data_size = 160;

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

	result = libewf_read_io_handle_initialize(
	          &read_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "read_io_handle",
	 read_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_media_values_initialize(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvalue_table_initialize(
	          &header_values,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO pool
	 */
	result = ewf_test_open_file_io_pool(
	          &file_io_pool,
	          ewf_test_case_data_section_data1,
	          160,
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

	/* Test read of case data section without case data set in read IO handle
	 */
	read_count = libewf_case_data_section_read_file_io_pool(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              0,
	              read_io_handle,
	              media_values,
	              header_values,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 160 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test read of case data section with case data set in read IO handle
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

	read_count = libewf_case_data_section_read_file_io_pool(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              0,
	              read_io_handle,
	              media_values,
	              header_values,
	              &error );

	EWF_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) 160 );

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

	read_count = libewf_case_data_section_read_file_io_pool(
	              NULL,
	              io_handle,
	              file_io_pool,
	              0,
	              read_io_handle,
	              media_values,
	              header_values,
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

	read_count = libewf_case_data_section_read_file_io_pool(
	              section_descriptor,
	              NULL,
	              file_io_pool,
	              0,
	              read_io_handle,
	              media_values,
	              header_values,
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

	read_count = libewf_case_data_section_read_file_io_pool(
	              section_descriptor,
	              io_handle,
	              NULL,
	              0,
	              read_io_handle,
	              media_values,
	              header_values,
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

	read_count = libewf_case_data_section_read_file_io_pool(
	              section_descriptor,
	              io_handle,
	              file_io_pool,
	              0,
	              NULL,
	              media_values,
	              header_values,
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

	/* Clean up
	 */
	result = libfvalue_table_free(
	          &header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "header_values",
	 header_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_media_values_free(
	          &media_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "media_values",
	 media_values );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_read_io_handle_free(
	          &read_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "read_io_handle",
	 read_io_handle );

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
	if( header_values != NULL )
	{
		libfvalue_table_free(
		 &header_values,
		 NULL );
	}
	if( media_values != NULL )
	{
		libewf_media_values_free(
		 &media_values,
		 NULL );
	}
	if( read_io_handle != NULL )
	{
		libewf_read_io_handle_free(
		 &read_io_handle,
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
	 "libewf_case_data_section_read_file_io_pool",
	 ewf_test_case_data_section_read_file_io_pool );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


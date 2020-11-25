/*
 * Library case_data type test program
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

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_libfvalue.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_case_data.h"
#include "../libewf/libewf_header_values.h"
#include "../libewf/libewf_media_values.h"

uint8_t ewf_test_case_data1[ 272 ] = {
	0xff, 0xfe, 0x31, 0x00, 0x0a, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x0a, 0x00,
	0x6e, 0x00, 0x6d, 0x00, 0x09, 0x00, 0x63, 0x00, 0x6e, 0x00, 0x09, 0x00, 0x65, 0x00, 0x6e, 0x00,
	0x09, 0x00, 0x65, 0x00, 0x78, 0x00, 0x09, 0x00, 0x6e, 0x00, 0x74, 0x00, 0x09, 0x00, 0x61, 0x00,
	0x76, 0x00, 0x09, 0x00, 0x6f, 0x00, 0x73, 0x00, 0x09, 0x00, 0x74, 0x00, 0x74, 0x00, 0x09, 0x00,
	0x61, 0x00, 0x74, 0x00, 0x09, 0x00, 0x74, 0x00, 0x62, 0x00, 0x09, 0x00, 0x63, 0x00, 0x70, 0x00,
	0x09, 0x00, 0x73, 0x00, 0x62, 0x00, 0x09, 0x00, 0x67, 0x00, 0x72, 0x00, 0x09, 0x00, 0x77, 0x00,
	0x62, 0x00, 0x0a, 0x00, 0x75, 0x00, 0x73, 0x00, 0x62, 0x00, 0x2d, 0x00, 0x6e, 0x00, 0x61, 0x00,
	0x6d, 0x00, 0x65, 0x00, 0x09, 0x00, 0x63, 0x00, 0x61, 0x00, 0x73, 0x00, 0x65, 0x00, 0x09, 0x00,
	0x65, 0x00, 0x76, 0x00, 0x69, 0x00, 0x64, 0x00, 0x09, 0x00, 0x65, 0x00, 0x78, 0x00, 0x61, 0x00,
	0x6d, 0x00, 0x09, 0x00, 0x6e, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x09, 0x00,
	0x37, 0x00, 0x2e, 0x00, 0x34, 0x00, 0x2e, 0x00, 0x31, 0x00, 0x2e, 0x00, 0x31, 0x00, 0x30, 0x00,
	0x09, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x64, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x73, 0x00,
	0x20, 0x00, 0x37, 0x00, 0x09, 0x00, 0x31, 0x00, 0x33, 0x00, 0x34, 0x00, 0x31, 0x00, 0x33, 0x00,
	0x34, 0x00, 0x32, 0x00, 0x34, 0x00, 0x39, 0x00, 0x31, 0x00, 0x09, 0x00, 0x31, 0x00, 0x33, 0x00,
	0x34, 0x00, 0x31, 0x00, 0x33, 0x00, 0x34, 0x00, 0x32, 0x00, 0x34, 0x00, 0x34, 0x00, 0x35, 0x00,
	0x09, 0x00, 0x38, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x09, 0x00, 0x31, 0x00, 0x09, 0x00,
	0x36, 0x00, 0x34, 0x00, 0x09, 0x00, 0x36, 0x00, 0x34, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x0a, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_case_data_generate_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_case_data_generate_utf8_string(
     void )
{
	libcerror_error_t *error            = NULL;
	libewf_media_values_t *media_values = NULL;
	libfvalue_table_t *header_values    = NULL;
	uint8_t *utf8_string                = NULL;
	size_t utf8_string_size             = 0;
	int header_value_index              = 0;
	int result                          = 0;

	/* Initialize test
	 */
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

	result = libewf_header_values_initialize(
	          &header_values,
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

	/* Test regular cases
	 */
	utf8_string_size = 0;

	result = libewf_case_data_generate_utf8_string(
	          &utf8_string,
	          &utf8_string_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "utf8_string",
	 utf8_string );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	memory_free(
	 utf8_string );

	utf8_string = NULL;

	/* Test error cases
	 */
	utf8_string_size = 0;

	result = libewf_case_data_generate_utf8_string(
	          NULL,
	          &utf8_string_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	utf8_string = (uint8_t *) 0x12345678UL;

	result = libewf_case_data_generate_utf8_string(
	          &utf8_string,
	          &utf8_string_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	utf8_string = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_generate_utf8_string(
	          &utf8_string,
	          NULL,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_generate_utf8_string(
	          &utf8_string,
	          &utf8_string_size,
	          NULL,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_generate_utf8_string(
	          &utf8_string,
	          &utf8_string_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_UNKNOWN,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test libewf_case_data_generate_utf8_string with failing libfvalue_table_get_value_by_identifier
	 */
	result = libewf_case_data_generate_utf8_string(
	          &utf8_string,
	          &utf8_string_size,
	          media_values,
	          NULL,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test libewf_case_data_generate_utf8_string with failing libfvalue_table_get_value_by_identifier
	 */
	for( header_value_index = LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION;
	     header_value_index >= LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER;
	     header_value_index-- )
	{
		result = libfvalue_table_set_value_by_index(
		          header_values,
		          header_value_index,
		          NULL,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libewf_case_data_generate_utf8_string(
		          &utf8_string,
		          &utf8_string_size,
		          media_values,
		          header_values,
		          0,
		          LIBEWF_FORMAT_V2_ENCASE7,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "utf8_string",
		 utf8_string );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
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

	/* Initialize test
	 */
	result = libewf_header_values_initialize(
	          &header_values,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_case_data_generate_utf8_string with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 2;

	result = libewf_case_data_generate_utf8_string(
	          &utf8_string,
	          &utf8_string_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( utf8_string != NULL )
		{
			memory_free(
			 utf8_string );

			utf8_string = NULL;
		}
		utf8_string_size = 0;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "utf8_string",
		 utf8_string );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
	return( 0 );
}

/* Tests the libewf_case_data_generate function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_case_data_generate(
     void )
{
	libcerror_error_t *error            = NULL;
	libewf_media_values_t *media_values = NULL;
	libfvalue_table_t *header_values    = NULL;
	uint8_t *case_data                  = NULL;
	size_t case_data_size               = 0;
	int result                          = 0;

	/* Initialize test
	 */
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

	result = libewf_header_values_initialize(
	          &header_values,
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

	/* Test regular cases
	 */
	case_data_size = 0;

	result = libewf_case_data_generate(
	          &case_data,
	          &case_data_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "case_data",
	 case_data );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	memory_free(
	 case_data );

	case_data = NULL;

	/* Test error cases
	 */
	case_data_size = 0;

	result = libewf_case_data_generate(
	          NULL,
	          &case_data_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "case_data",
	 case_data );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	case_data = (uint8_t *) 0x12345678UL;

	result = libewf_case_data_generate(
	          &case_data,
	          &case_data_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	case_data = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "case_data",
	 case_data );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_generate(
	          &case_data,
	          NULL,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "case_data",
	 case_data );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test libewf_case_data_parse with failing libewf_case_data_generate_utf8_string
	 */
	result = libewf_case_data_generate(
	          &case_data,
	          &case_data_size,
	          NULL,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "case_data",
	 case_data );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EWF_TEST_MEMORY )

	/* Test libewf_case_data_generate with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 3;

	case_data_size = 0;

	result = libewf_case_data_generate(
	          &case_data,
	          &case_data_size,
	          media_values,
	          header_values,
	          0,
	          LIBEWF_FORMAT_V2_ENCASE7,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( case_data != NULL )
		{
			memory_free(
			 case_data );

			case_data = NULL;
		}
		case_data_size = 0;
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "case_data",
		 case_data );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	/* Clean up
	 */
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
	return( 0 );
}

/* Tests the libewf_case_data_parse_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_case_data_parse_utf8_string(
     void )
{
	libcerror_error_t *error            = NULL;
	libewf_media_values_t *media_values = NULL;
	libfvalue_table_t *header_values    = NULL;
	uint8_t *utf8_string                = (uint8_t *) "1\nmain\nnm\tcn\ten\tex\tnt\tav\tos\ttt\tat\ttb\tcp\tsb\tgr\twb\nusb-name\tcase\tevid\texam\tnotes\t7.4.1.10\tWindows 7\t1341342491\t1341342445\t8000\t1\t64\t64\t\n\n";
	uint8_t format                      = 0;
	int result                          = 0;

	/* Initialize test
	 */
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

	result = libewf_header_values_initialize(
	          &header_values,
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

	/* Test regular cases
	 */
	result = libewf_case_data_parse_utf8_string(
	          utf8_string,
	          137,
	          media_values,
	          header_values,
	          &format,
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
	result = libewf_case_data_parse_utf8_string(
	          NULL,
	          137,
	          media_values,
	          header_values,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_parse_utf8_string(
	          utf8_string,
	          (size_t) SSIZE_MAX + 1,
	          media_values,
	          header_values,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_parse_utf8_string(
	          utf8_string,
	          137,
	          NULL,
	          header_values,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_parse_utf8_string(
	          utf8_string,
	          137,
	          media_values,
	          NULL,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_parse_utf8_string(
	          utf8_string,
	          137,
	          media_values,
	          header_values,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
	return( 0 );
}

/* Tests the libewf_case_data_parse_utf8_string_value function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_case_data_parse_utf8_string_value(
     void )
{
	libcerror_error_t *error            = NULL;
	libewf_media_values_t *media_values = NULL;
	libfvalue_table_t *header_values    = NULL;
	int result                          = 0;

	/* Initialize test
	 */
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

	result = libewf_header_values_initialize(
	          &header_values,
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

	/* Test regular cases
	 */
	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "av",
	          3,
	          (uint8_t *) "ewftest",
	          8,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "at",
	          3,
	          (uint8_t *) "1599219555",
	          11,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "cp",
	          3,
	          (uint8_t *) "0",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "cp",
	          3,
	          (uint8_t *) "1",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "cp",
	          3,
	          (uint8_t *) "2",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "en",
	          3,
	          (uint8_t *) "evidence number",
	          16,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "ex",
	          3,
	          (uint8_t *) "examiner name",
	          14,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "gr",
	          3,
	          (uint8_t *) "64",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "nm",
	          3,
	          (uint8_t *) "description",
	          12,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "nt",
	          3,
	          (uint8_t *) "notes",
	          6,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "os",
	          3,
	          (uint8_t *) "acquiry operating system",
	          25,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "sb",
	          3,
	          (uint8_t *) "64",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "tb",
	          3,
	          (uint8_t *) "512",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "tt",
	          3,
	          (uint8_t *) "1599219555",
	          11,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "wb",
	          3,
	          (uint8_t *) "0",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "wb",
	          3,
	          (uint8_t *) "1",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_case_data_parse_utf8_string_value(
	          (uint8_t *) "wb",
	          3,
	          (uint8_t *) "2",
	          2,
	          0,
	          media_values,
	          header_values,
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
	result = libewf_case_data_parse_utf8_string_value(
	          NULL,
	          3,
	          (uint8_t *) "0",
	          2,
	          0,
	          media_values,
	          header_values,
	          &error );

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
	return( 0 );
}

/* Tests the libewf_case_data_parse function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_case_data_parse(
     void )
{
	libcerror_error_t *error            = NULL;
	libewf_media_values_t *media_values = NULL;
	libfvalue_table_t *header_values    = NULL;
	uint8_t format                      = 0;
	int result                          = 0;

	/* Initialize test
	 */
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

	result = libewf_header_values_initialize(
	          &header_values,
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

	/* Test regular cases
	 */
	result = libewf_case_data_parse(
	          ewf_test_case_data1,
	          272,
	          media_values,
	          header_values,
	          &format,
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
	result = libewf_case_data_parse(
	          NULL,
	          272,
	          media_values,
	          header_values,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_case_data_parse(
	          ewf_test_case_data1,
	          (size_t) SSIZE_MAX + 1,
	          media_values,
	          header_values,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test libewf_case_data_parse with failing libewf_case_data_parse_utf8_string
	 */
	result = libewf_case_data_parse(
	          ewf_test_case_data1,
	          272,
	          NULL,
	          header_values,
	          &format,
	          &error );

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

	/* Test libewf_case_data_parse with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_case_data_parse(
	          ewf_test_case_data1,
	          272,
	          media_values,
	          header_values,
	          &format,
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

	/* Test with an empty UTF-16 litte-endian stream
	 */
	result = libewf_case_data_parse(
	          ewf_test_case_data1,
	          0,
	          media_values,
	          header_values,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test with an invalid UTF-16 litte-endian stream
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( ewf_test_case_data1[ 6 ] ),
	 0xd800 );

	result = libewf_case_data_parse(
	          ewf_test_case_data1,
	          272,
	          media_values,
	          header_values,
	          &format,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( ewf_test_case_data1[ 6 ] ),
	 0x006d );

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
	 "libewf_case_data_generate_utf8_string",
	 ewf_test_case_data_generate_utf8_string );

	EWF_TEST_RUN(
	 "libewf_case_data_generate",
	 ewf_test_case_data_generate );

	EWF_TEST_RUN(
	 "libewf_case_data_parse_utf8_string",
	 ewf_test_case_data_parse_utf8_string );

	EWF_TEST_RUN(
	 "libewf_case_data_parse_utf8_string_value",
	 ewf_test_case_data_parse_utf8_string_value );

	EWF_TEST_RUN(
	 "libewf_case_data_parse",
	 ewf_test_case_data_parse );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


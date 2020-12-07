/*
 * Tools guid functions test program
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <system_string.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcerror.h"
#include "ewf_test_macros.h"
#include "ewf_test_unused.h"

#include "../ewftools/guid.h"

#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )

/* Tests the guid_generate function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_guid_generate(
     void )
{
	uint8_t guid[ 16 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = guid_generate(
	          guid,
	          16,
	          GUID_TYPE_RANDOM,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = guid_generate(
	          guid,
	          16,
	          GUID_TYPE_TIME,
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
	result = guid_generate(
	          NULL,
	          16,
	          GUID_TYPE_RANDOM,
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

	result = guid_generate(
	          guid,
	          (size_t) SSIZE_MAX + 1,
	          GUID_TYPE_RANDOM,
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

	result = guid_generate(
	          guid,
	          0,
	          GUID_TYPE_RANDOM,
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

	result = guid_generate(
	          guid,
	          16,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_GUID_SUPPORT ) || defined( WINAPI ) */

/* Tests the guid_to_string function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_tools_guid_to_string(
     void )
{
	system_character_t string[ 64 ];

	uint8_t guid[ 16 ] = {
		0xd0, 0x74, 0xeb, 0x89, 0x79, 0xfe, 0x4e, 0x3d, 0x91, 0x9f, 0x95, 0xbf, 0x46, 0xf7, 0xf5, 0x5f };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = guid_to_string(
	          guid,
	          16,
	          _BYTE_STREAM_ENDIAN_BIG,
	          string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          string,
	          "d074eb89-79fe-4e3d-919f-95bf46f7f55f",
	          37 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	result = guid_to_string(
	          guid,
	          16,
	          _BYTE_STREAM_ENDIAN_LITTLE,
	          string,
	          64,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          string,
	          "89eb74d0-fe79-3d4e-919f-95bf46f7f55f",
	          37 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = guid_to_string(
	          NULL,
	          16,
	          _BYTE_STREAM_ENDIAN_BIG,
	          string,
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

	result = guid_to_string(
	          guid,
	          (size_t) SSIZE_MAX + 1,
	          _BYTE_STREAM_ENDIAN_BIG,
	          string,
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

	result = guid_to_string(
	          guid,
	          0,
	          _BYTE_STREAM_ENDIAN_BIG,
	          string,
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

	result = guid_to_string(
	          guid,
	          16,
	          -1,
	          string,
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

	result = guid_to_string(
	          guid,
	          16,
	          _BYTE_STREAM_ENDIAN_BIG,
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

	result = guid_to_string(
	          guid,
	          16,
	          _BYTE_STREAM_ENDIAN_BIG,
	          string,
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

	result = guid_to_string(
	          guid,
	          16,
	          _BYTE_STREAM_ENDIAN_BIG,
	          string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

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

#if defined( HAVE_GUID_SUPPORT ) || defined( WINAPI )

	EWF_TEST_RUN(
	 "guid_generate",
	 ewf_test_tools_guid_generate );

#endif /* defined( HAVE_GUID_SUPPORT ) || defined( WINAPI ) */

	EWF_TEST_RUN(
	 "guid_to_string",
	 ewf_test_tools_guid_to_string );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


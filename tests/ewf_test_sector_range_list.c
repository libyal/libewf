/*
 * Library sector_range_list functions test program
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

#include "ewf_test_libcdata.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_sector_range.h"
#include "../libewf/libewf_sector_range_list.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_sector_range_list_get_range function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_sector_range_list_get_range(
     void )
{
	libcdata_array_t *sector_range_list = NULL;
	libcerror_error_t *error            = NULL;
	uint64_t number_of_sectors          = 0;
	uint64_t start_sector               = 0;
	int result                          = 0;

	/* Initialize test
	 */
	result = libcdata_array_initialize(
	          &sector_range_list,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "sector_range_list",
	 sector_range_list );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_sector_range_list_append_range(
	          sector_range_list,
	          32,
	          64,
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
	result = libewf_sector_range_list_get_range(
	          sector_range_list,
	          0,
	          &start_sector,
	          &number_of_sectors,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "start_sector",
	 start_sector,
	 (uint64_t) 32 );

	EWF_TEST_ASSERT_EQUAL_UINT64(
	 "number_of_sectors",
	 number_of_sectors,
	 (uint64_t) 64 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_sector_range_list_get_range(
	          NULL,
	          0,
	          &start_sector,
	          &number_of_sectors,
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

	result = libewf_sector_range_list_get_range(
	          sector_range_list,
	          0xffffffffUL,
	          &start_sector,
	          &number_of_sectors,
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

	result = libewf_sector_range_list_get_range(
	          sector_range_list,
	          0,
	          NULL,
	          &number_of_sectors,
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

	result = libewf_sector_range_list_get_range(
	          sector_range_list,
	          0,
	          &start_sector,
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
	result = libcdata_array_free(
	          &sector_range_list,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "sector_range_list",
	 sector_range_list );

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
	if( sector_range_list != NULL )
	{
		libcdata_array_free(
		 &sector_range_list,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_sector_range_list_append_range function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_sector_range_list_append_range(
     void )
{
	libcdata_array_t *sector_range_list = NULL;
	libcerror_error_t *error            = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libcdata_array_initialize(
	          &sector_range_list,
	          0,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "sector_range_list",
	 sector_range_list );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_sector_range_list_append_range(
	          sector_range_list,
	          32,
	          64,
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
	result = libewf_sector_range_list_append_range(
	          NULL,
	          32,
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

	/* Clean up
	 */
	result = libcdata_array_free(
	          &sector_range_list,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "sector_range_list",
	 sector_range_list );

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
	if( sector_range_list != NULL )
	{
		libcdata_array_free(
		 &sector_range_list,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libewf_sector_range_free,
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
	 "libewf_sector_range_list_get_range",
	 ewf_test_sector_range_list_get_range );

	EWF_TEST_RUN(
	 "libewf_sector_range_list_append_range",
	 ewf_test_sector_range_list_append_range );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */
}


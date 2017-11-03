/*
 * Library segment_table type test program
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

#include "../libewf/libewf_segment_table.h"

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_segment_table_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_segment_table_free(
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

/* Tests the libewf_segment_table_clone function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_table_clone(
     void )
{
	libcerror_error_t *error                          = NULL;
	libewf_io_handle_t *io_handle                     = NULL;
	libewf_segment_table_t *destination_segment_table = NULL;
	libewf_segment_table_t *source_segment_table      = NULL;
	int result                                        = 0;

	/* Initialize test
	 */
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

	result = libewf_segment_table_initialize(
	          &source_segment_table,
	          io_handle,
	          LIBEWF_DEFAULT_SEGMENT_FILE_SIZE,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "source_segment_table",
	 source_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libewf_segment_table_clone(
	          &destination_segment_table,
	          source_segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_segment_table",
	 destination_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_free(
	          &destination_segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_segment_table",
	 destination_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_segment_table_clone(
	          &destination_segment_table,
	          NULL,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "destination_segment_table",
	 destination_segment_table );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_segment_table_clone(
	          NULL,
	          source_segment_table,
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
	result = libewf_segment_table_free(
	          &source_segment_table,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NULL(
	 "source_segment_table",
	 source_segment_table );

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( destination_segment_table != NULL )
	{
		libewf_segment_table_free(
		 &destination_segment_table,
		 NULL );
	}
	if( source_segment_table != NULL )
	{
		libewf_segment_table_free(
		 &source_segment_table,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libewf_io_handle_free(
		 &io_handle,
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

	/* TODO: add tests for libewf_segment_table_initialize */

	EWF_TEST_RUN(
	 "libewf_segment_table_free",
	 ewf_test_segment_table_free );

#if defined( TODO )

/* TODO: fix libewf_segment_table_clone */

	EWF_TEST_RUN(
	 "libewf_segment_table_clone",
	 ewf_test_segment_table_clone );

#endif

	/* TODO: add tests for libewf_segment_table_empty */

	/* TODO: add tests for libewf_segment_table_get_basename_size */

	/* TODO: add tests for libewf_segment_table_get_basename */

	/* TODO: add tests for libewf_segment_table_set_basename */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

	/* TODO: add tests for libewf_segment_table_get_basename_size_wide */

	/* TODO: add tests for libewf_segment_table_get_basename_wide */

	/* TODO: add tests for libewf_segment_table_set_basename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

	/* TODO: add tests for libewf_segment_table_set_maximum_segment_size */

	/* TODO: add tests for libewf_segment_table_get_number_of_segments */

	/* TODO: add tests for libewf_segment_table_get_segment_by_index */

	/* TODO: add tests for libewf_segment_table_get_segment_at_offset */

	/* TODO: add tests for libewf_segment_table_get_segment_storage_media_size_by_index */

	/* TODO: add tests for libewf_segment_table_set_segment_storage_media_size_by_index */

	/* TODO: add tests for libewf_segment_table_get_segment_file_by_index */

	/* TODO: add tests for libewf_segment_table_get_segment_file_at_offset */

	/* TODO: add tests for libewf_segment_table_set_segment_file_by_index */

	/* TODO: add tests for libewf_segment_table_append_segment_by_segment_file */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


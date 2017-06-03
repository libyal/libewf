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

#if defined( __GNUC__ )

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

#endif /* defined( __GNUC__ ) */

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

#if defined( __GNUC__ )

	/* TODO: add tests for libewf_segment_table_initialize */

	EWF_TEST_RUN(
	 "libewf_segment_table_free",
	 ewf_test_segment_table_free );

	/* TODO: add tests for libewf_segment_table_clone */

	/* TODO: add tests for libewf_segment_table_empty */

	/* TODO: add tests for libewf_segment_table_get_basename_size */

	/* TODO: add tests for libewf_segment_table_get_basename */

	/* TODO: add tests for libewf_segment_table_set_basename */

	/* TODO: add tests for libewf_segment_table_get_basename_size_wide */

	/* TODO: add tests for libewf_segment_table_get_basename_wide */

	/* TODO: add tests for libewf_segment_table_set_basename_wide */

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

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


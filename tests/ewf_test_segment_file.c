/*
 * Library segment_file type test program
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

#include "../libewf/libewf_segment_file.h"

#if defined( __GNUC__ )

/* Tests the libewf_segment_file_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_segment_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_segment_file_free(
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

	/* TODO: add tests for libewf_segment_file_initialize */

	EWF_TEST_RUN(
	 "libewf_segment_file_free",
	 ewf_test_segment_file_free );

	/* TODO: add tests for libewf_segment_file_clone */

	/* TODO: add tests for libewf_segment_file_get_number_of_sections */

	/* TODO: add tests for libewf_segment_file_get_section_by_index */

	/* TODO: add tests for libewf_segment_file_read_file_header */

	/* TODO: add tests for libewf_segment_file_write_file_header */

	/* TODO: add tests for libewf_segment_file_seek_offset */

	/* TODO: add tests for libewf_segment_file_read_table_section */

	/* TODO: add tests for libewf_segment_file_read_table2_section */

	/* TODO: add tests for libewf_segment_file_read_volume_section */

	/* TODO: add tests for libewf_segment_file_write_device_information_section */

	/* TODO: add tests for libewf_segment_file_write_case_data_section */

	/* TODO: add tests for libewf_segment_file_write_header_section */

	/* TODO: add tests for libewf_segment_file_write_header2_section */

	/* TODO: add tests for libewf_segment_file_write_xheader_section */

	/* TODO: add tests for libewf_segment_file_write_header_sections */

	/* TODO: add tests for libewf_segment_file_write_last_section */

	/* TODO: add tests for libewf_segment_file_write_start */

	/* TODO: add tests for libewf_segment_file_write_chunks_section_start */

	/* TODO: add tests for libewf_segment_file_write_chunks_section_end */

	/* TODO: add tests for libewf_segment_file_write_chunk_data */

	/* TODO: add tests for libewf_segment_file_write_hash_sections */

	/* TODO: add tests for libewf_segment_file_write_close */

	/* TODO: add tests for libewf_segment_file_reopen */

	/* TODO: add tests for libewf_segment_file_write_sections_correction */

	/* TODO: add tests for libewf_segment_file_read_element_data */

	/* TODO: add tests for libewf_segment_file_read_section_element_data */

	/* TODO: add tests for libewf_segment_file_read_chunk_group_element_data */

	/* TODO: add tests for libewf_segment_file_get_chunk_group_by_offset */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


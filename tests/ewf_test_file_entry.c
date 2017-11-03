/*
 * Library file_entry type test program
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

#include "../libewf/libewf_file_entry.h"

/* Tests the libewf_file_entry_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_file_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_file_entry_free(
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

	/* TODO: add tests for libewf_file_entry_initialize */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_RUN(
	 "libewf_file_entry_free",
	 ewf_test_file_entry_free );

	/* TODO: add tests for libewf_file_entry_get_type */

	/* TODO: add tests for libewf_file_entry_get_flags */

	/* TODO: add tests for libewf_file_entry_get_media_data_offset */

	/* TODO: add tests for libewf_file_entry_get_media_data_size */

	/* TODO: add tests for libewf_file_entry_get_duplicate_media_data_offset */

	/* TODO: add tests for libewf_file_entry_get_utf8_name_size */

	/* TODO: add tests for libewf_file_entry_get_utf8_name */

	/* TODO: add tests for libewf_file_entry_get_utf16_name_size */

	/* TODO: add tests for libewf_file_entry_get_utf16_name */

	/* TODO: add tests for libewf_file_entry_get_size */

	/* TODO: add tests for libewf_file_entry_get_creation_time */

	/* TODO: add tests for libewf_file_entry_get_modification_time */

	/* TODO: add tests for libewf_file_entry_get_access_time */

	/* TODO: add tests for libewf_file_entry_get_entry_modification_time */

	/* TODO: add tests for libewf_file_entry_get_utf8_hash_value_md5 */

	/* TODO: add tests for libewf_file_entry_get_utf16_hash_value_md5 */

	/* TODO: add tests for libewf_file_entry_get_utf8_hash_value_sha1 */

	/* TODO: add tests for libewf_file_entry_get_utf16_hash_value_sha1 */

	/* TODO: add tests for libewf_file_entry_read_buffer */

	/* TODO: add tests for libewf_file_entry_read_buffer_at_offset */

	/* TODO: add tests for libewf_file_entry_seek_offset */

	/* TODO: add tests for libewf_file_entry_get_offset */

	/* TODO: add tests for libewf_file_entry_get_number_of_sub_file_entries */

	/* TODO: add tests for libewf_file_entry_get_sub_file_entry */

	/* TODO: add tests for libewf_file_entry_get_sub_file_entry_by_utf8_name */

	/* TODO: add tests for libewf_file_entry_get_sub_file_entry_by_utf8_path */

	/* TODO: add tests for libewf_file_entry_get_sub_file_entry_by_utf16_name */

	/* TODO: add tests for libewf_file_entry_get_sub_file_entry_by_utf16_path */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


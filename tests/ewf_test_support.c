/*
 * Library support functions test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_functions.h"
#include "ewf_test_getopt.h"
#include "ewf_test_libbfio.h"
#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_support.h"

#if !defined( LIBEWF_HAVE_BFIO )

LIBEWF_EXTERN \
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* !defined( LIBEWF_HAVE_BFIO ) */

/* Tests the libewf_get_version function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libewf_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBEWF_VERSION_STRING,
	          9 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libewf_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libewf_get_access_flags_read();

	EWF_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBEWF_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}


/* Tests the libewf_get_access_flags_read_write function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_get_access_flags_read_write(
     void )
{
	int access_flags = 0;

	access_flags = libewf_get_access_flags_read_write();

	EWF_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 ( LIBEWF_ACCESS_FLAG_READ | LIBEWF_ACCESS_FLAG_WRITE ) );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libewf_get_access_flags_write function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_get_access_flags_write(
     void )
{
	int access_flags = 0;

	access_flags = libewf_get_access_flags_write();

	EWF_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBEWF_ACCESS_FLAG_WRITE );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libewf_get_access_flags_write_resume function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_get_access_flags_write_resume(
     void )
{
	int access_flags = 0;

	access_flags = libewf_get_access_flags_write_resume();

	EWF_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 ( LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME ) );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libewf_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libewf_get_codepage(
	          &codepage,
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
	result = libewf_get_codepage(
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

/* Tests the libewf_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libewf_set_codepage(
	          0,
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
	result = libewf_set_codepage(
	          -1,
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

/* Tests the libewf_check_file_signature function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_check_file_signature(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = ewf_test_get_narrow_source(
		          source,
		          narrow_source,
		          256,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libewf_check_file_signature(
		          narrow_source,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_check_file_signature(
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

	result = libewf_check_file_signature(
	          "",
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

	if( source != NULL )
	{
#if defined( HAVE_EWF_TEST_MEMORY )

		/* Test libewf_check_file_signature with malloc failing in libbfio_file_initialize
		 */
		ewf_test_malloc_attempts_before_fail = 0;

		result = libewf_check_file_signature(
		          narrow_source,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libewf_check_file_signature_wide function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_check_file_signature_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = ewf_test_get_wide_source(
		          source,
		          wide_source,
		          256,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libewf_check_file_signature_wide(
		          wide_source,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_check_file_signature_wide(
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

	result = libewf_check_file_signature_wide(
	          L"",
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

	if( source != NULL )
	{
#if defined( HAVE_EWF_TEST_MEMORY )

		/* Test libewf_check_file_signature_wide with malloc failing in libbfio_file_initialize
		 */
		ewf_test_malloc_attempts_before_fail = 0;

		result = libewf_check_file_signature_wide(
		          wide_source,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libewf_check_file_signature_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_check_file_signature_file_io_handle(
     const system_character_t *source )
{
	uint8_t empty_block[ 8192 ];

	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	void *memset_result              = NULL;
	size_t source_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 empty_block,
	                 0,
	                 sizeof( uint8_t ) * 8192 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = libbfio_file_initialize(
		          &file_io_handle,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "file_io_handle",
		 file_io_handle );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		source_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          source_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          source_length,
		          &error );
#endif
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_open(
		          file_io_handle,
		          LIBBFIO_OPEN_READ,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libewf_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_check_file_signature_file_io_handle(
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
	if( source != NULL )
	{
		result = libbfio_handle_close(
		          file_io_handle,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "file_io_handle",
		 file_io_handle );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test check file signature with data too small
	 */
	result = ewf_test_open_file_io_handle(
	          &file_io_handle,
	          empty_block,
	          sizeof( uint8_t ) * 1,
	          LIBBFIO_OPEN_READ,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_check_file_signature_file_io_handle(
	          file_io_handle,
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

	result = ewf_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test check file signature with empty block
	 */
	result = ewf_test_open_file_io_handle(
	          &file_io_handle,
	          empty_block,
	          sizeof( uint8_t ) * 8192,
	          LIBBFIO_OPEN_READ,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = ewf_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_glob_determine_format function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob_determine_format(
     void )
{
	libcerror_error_t *error = NULL;
	uint8_t format           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_glob_determine_format(
	          "test.E01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 (uint8_t) LIBEWF_FORMAT_ENCASE5 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_determine_format(
	          "test.L01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 (uint8_t) LIBEWF_FORMAT_LOGICAL_ENCASE5 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_determine_format(
	          "test.e01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 (uint8_t) LIBEWF_FORMAT_EWF );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_determine_format(
	          "test.s01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 (uint8_t) LIBEWF_FORMAT_SMART );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_determine_format(
	          "test.Ex01",
	          9,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 (uint8_t) LIBEWF_FORMAT_V2_ENCASE7 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_determine_format(
	          "test.Lx01",
	          9,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 (uint8_t) LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_glob_determine_format(
	          NULL,
	          8,
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

	result = libewf_glob_determine_format(
	          "test.x01",
	          8,
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

	result = libewf_glob_determine_format(
	          "test.E01",
	          8,
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

	result = libewf_glob_determine_format(
	          "test",
	          4,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_glob_get_segment_filename function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob_get_segment_filename(
     void )
{
	char expected_segment_filename[ 9 ] = {
		't', 'e', 's', 't', '.', 'E', '0', '2', 0 };

	libcerror_error_t *error            = NULL;
	char *segment_filename              = NULL;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libewf_glob_get_segment_filename(
	          "test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_filename",
	 segment_filename );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          segment_filename,
	          expected_segment_filename,
	          9 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 segment_filename );

	segment_filename = NULL;

	/* Test error cases
	 */
	result = libewf_glob_get_segment_filename(
	          NULL,
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
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

	result = libewf_glob_get_segment_filename(
	          "test.E01",
	          0,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
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

	result = libewf_glob_get_segment_filename(
	          "test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
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

	segment_filename = (char *) 0x12345678UL;

	result = libewf_glob_get_segment_filename(
	          "test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
	          &error );

	segment_filename = NULL;

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

	/* Test libewf_glob_get_segment_filename with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_glob_get_segment_filename(
	          "test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( segment_filename != NULL )
		{
			memory_free(
			 segment_filename );

			segment_filename = NULL;
		}
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "segment_filename",
		 segment_filename );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

/* Tests the libewf_glob function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error        = NULL;
	char **filenames                = NULL;
	size_t narrow_source_length     = 0;
	int number_of_filenames         = 0;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 3;
	int test_number                 = 0;
#endif

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = ewf_test_get_narrow_source(
		          source,
		          narrow_source,
		          256,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		narrow_source_length = narrow_string_length(
		                        narrow_source );

		/* Test regular cases
		 */
		result = libewf_glob(
		          narrow_source,
		          narrow_source_length,
		          LIBEWF_FORMAT_UNKNOWN,
		          &filenames,
		          &number_of_filenames,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libewf_glob_free(
		          filenames,
		          number_of_filenames,
		          &error );

		filenames           = NULL;
		number_of_filenames = 0;

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_glob(
	          NULL,
	          narrow_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob(
	          narrow_source,
	          (size_t) SSIZE_MAX + 1,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob(
	          narrow_source,
	          0,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob(
	          narrow_source,
	          narrow_source_length,
	          0xff,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob(
	          narrow_source,
	          narrow_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          NULL,
	          &number_of_filenames,
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

	filenames = (char **) 0x12345678UL;

	result = libewf_glob(
	          narrow_source,
	          narrow_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );

	filenames = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_glob(
	          narrow_source,
	          narrow_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	if( source != NULL )
	{
		for( test_number = 0;
		     test_number < number_of_malloc_fail_tests;
		     test_number++ )
		{
			/* Test libewf_glob with malloc failing
			 */
			ewf_test_malloc_attempts_before_fail = test_number;

			result = libewf_glob(
			          narrow_source,
			          narrow_source_length,
			          LIBEWF_FORMAT_UNKNOWN,
			          &filenames,
			          &number_of_filenames,
			          &error );

			if( ewf_test_malloc_attempts_before_fail != -1 )
			{
				ewf_test_malloc_attempts_before_fail = -1;

				if( filenames != NULL )
				{
					result = libewf_glob_free(
						  filenames,
						  number_of_filenames,
						  &error );

					filenames           = NULL;
					number_of_filenames = 0;
				}
			}
			else
			{
				EWF_TEST_ASSERT_EQUAL_INT(
				 "result",
				 result,
				 -1 );

				EWF_TEST_ASSERT_IS_NULL(
				 "filenames",
				 filenames );

				EWF_TEST_ASSERT_IS_NOT_NULL(
				 "error",
				 error );

				libcerror_error_free(
				 &error );
			}
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
	if( filenames != NULL )
	{
		libewf_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_glob_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob_free(
     void )
{
	libcerror_error_t *error = NULL;
	char **filenames         = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_glob_free(
	          NULL,
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

	filenames = (char **) 0x12345678UL;

	result = libewf_glob_free(
	          filenames,
	          -1,
	          &error );

	filenames = NULL;

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

#if defined( HAVE_WIDE_CHARACTER_TYPE )

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

/* Tests the libewf_glob_wide_determine_format function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob_wide_determine_format(
     void )
{
	libcerror_error_t *error = NULL;
	uint8_t format           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libewf_glob_wide_determine_format(
	          L"test.E01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 LIBEWF_FORMAT_ENCASE5 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_wide_determine_format(
	          L"test.L01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 LIBEWF_FORMAT_LOGICAL_ENCASE5 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_wide_determine_format(
	          L"test.e01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 LIBEWF_FORMAT_EWF );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_wide_determine_format(
	          L"test.s01",
	          8,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 LIBEWF_FORMAT_SMART );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_wide_determine_format(
	          L"test.Ex01",
	          9,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 LIBEWF_FORMAT_V2_ENCASE7 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libewf_glob_wide_determine_format(
	          L"test.Lx01",
	          9,
	          &format,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_EQUAL_UINT8(
	 "format",
	 format,
	 LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libewf_glob_wide_determine_format(
	          NULL,
	          8,
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

	result = libewf_glob_wide_determine_format(
	          L"test.x01",
	          8,
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

	result = libewf_glob_wide_determine_format(
	          L"test.E01",
	          8,
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

	result = libewf_glob_wide_determine_format(
	          L"test",
	          4,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libewf_glob_wide_get_segment_filename function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob_wide_get_segment_filename(
     void )
{
	wchar_t expected_segment_filename[ 9 ] = {
		't', 'e', 's', 't', '.', 'E', '0', '2', 0 };

	libcerror_error_t *error               = NULL;
	wchar_t *segment_filename              = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libewf_glob_wide_get_segment_filename(
	          L"test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
	          &error );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "segment_filename",
	 segment_filename );

	EWF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          segment_filename,
	          expected_segment_filename,
	          9 );

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	memory_free(
	 segment_filename );

	segment_filename = NULL;

	/* Test error cases
	 */
	result = libewf_glob_wide_get_segment_filename(
	          NULL,
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
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

	result = libewf_glob_wide_get_segment_filename(
	          L"test.E01",
	          0,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
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

	result = libewf_glob_wide_get_segment_filename(
	          L"test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
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

	segment_filename = (char *) 0x12345678UL;

	result = libewf_glob_wide_get_segment_filename(
	          L"test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
	          &error );

	segment_filename = NULL;

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

	/* Test libewf_glob_wide_get_segment_filename with malloc failing
	 */
	ewf_test_malloc_attempts_before_fail = 0;

	result = libewf_glob_wide_get_segment_filename(
	          L"test.E01",
	          8,
	          4,
	          LIBEWF_SEGMENT_FILE_TYPE_EWF1,
	          2,
	          LIBEWF_FORMAT_LOGICAL_ENCASE5,
	          &segment_filename,
	          &error );

	if( ewf_test_malloc_attempts_before_fail != -1 )
	{
		ewf_test_malloc_attempts_before_fail = -1;

		if( segment_filename != NULL )
		{
			memory_free(
			 segment_filename );

			segment_filename = NULL;
		}
	}
	else
	{
		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "segment_filename",
		 segment_filename );

		EWF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_EWF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

/* Tests the libewf_glob_wide function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error        = NULL;
	wchar_t **filenames             = NULL;
	size_t wide_source_length       = 0;
	int number_of_filenames         = 0;
	int result                      = 0;

#if defined( HAVE_EWF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 3;
	int test_number                 = 0;
#endif

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = ewf_test_get_wide_source(
		          source,
		          wide_source,
		          256,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		wide_source_length = wide_string_length(
		                      wide_source );

		/* Test regular cases
		 */
		result = libewf_glob_wide(
		          wide_source,
		          wide_source_length,
		          LIBEWF_FORMAT_UNKNOWN,
		          &filenames,
		          &number_of_filenames,
		          &error );

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libewf_glob_wide_free(
		          filenames,
		          number_of_filenames,
		          &error );

		filenames           = NULL;
		number_of_filenames = 0;

		EWF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EWF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libewf_glob_wide(
	          NULL,
	          wide_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob_wide(
	          wide_source,
	          (size_t) SSIZE_MAX + 1,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob_wide(
	          wide_source,
	          0,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob_wide(
	          wide_source,
	          wide_source_length,
	          0xff,
	          &filenames,
	          &number_of_filenames,
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

	result = libewf_glob_wide(
	          wide_source,
	          wide_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          NULL,
	          &number_of_filenames,
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

	filenames = (char **) 0x12345678UL;

	result = libewf_glob_wide(
	          wide_source,
	          wide_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
	          &number_of_filenames,
	          &error );

	filenames = NULL;

	EWF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EWF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libewf_glob_wide(
	          wide_source,
	          wide_source_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
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

#if defined( HAVE_EWF_TEST_MEMORY )

	if( source != NULL )
	{
		for( test_number = 0;
		     test_number < number_of_malloc_fail_tests;
		     test_number++ )
		{
			/* Test libewf_glob_wide with malloc failing
			 */
			ewf_test_malloc_attempts_before_fail = test_number;

			result = libewf_glob_wide(
			          wide_source,
			          wide_source_length,
			          LIBEWF_FORMAT_UNKNOWN,
			          &filenames,
			          &number_of_filenames,
			          &error );

			if( ewf_test_malloc_attempts_before_fail != -1 )
			{
				ewf_test_malloc_attempts_before_fail = -1;

				if( filenames != NULL )
				{
					result = libewf_glob_wide_free(
						  filenames,
						  number_of_filenames,
						  &error );

					filenames           = NULL;
					number_of_filenames = 0;
				}
			}
			else
			{
				EWF_TEST_ASSERT_EQUAL_INT(
				 "result",
				 result,
				 -1 );

				EWF_TEST_ASSERT_IS_NULL(
				 "filenames",
				 filenames );

				EWF_TEST_ASSERT_IS_NOT_NULL(
				 "error",
				 error );

				libcerror_error_free(
				 &error );
			}
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
	if( filenames != NULL )
	{
		libewf_glob_wide_free(
		 filenames,
		 number_of_filenames,
		 NULL );
	}
	return( 0 );
}

/* Tests the libewf_glob_wide_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_glob_wide_free(
     void )
{
	libcerror_error_t *error = NULL;
	wchar_t **filenames      = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_glob_wide_free(
	          NULL,
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

	filenames = (wchar_t **) 0x12345678UL;

	result = libewf_glob_wide_free(
	          filenames,
	          -1,
	          &error );

	filenames = NULL;

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

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	system_character_t *source = NULL;
	system_integer_t option    = 0;

	while( ( option = ewf_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
	EWF_TEST_RUN(
	 "libewf_get_version",
	 ewf_test_get_version );

	EWF_TEST_RUN(
	 "libewf_get_access_flags_read",
	 ewf_test_get_access_flags_read );

	EWF_TEST_RUN(
	 "libewf_get_access_flags_read_write",
	 ewf_test_get_access_flags_read_write );

	EWF_TEST_RUN(
	 "libewf_get_access_flags_write",
	 ewf_test_get_access_flags_write );

	EWF_TEST_RUN(
	 "libewf_get_access_flags_write_resume",
	 ewf_test_get_access_flags_write_resume );

	EWF_TEST_RUN(
	 "libewf_get_codepage",
	 ewf_test_get_codepage );

	EWF_TEST_RUN(
	 "libewf_set_codepage",
	 ewf_test_set_codepage );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_check_file_signature",
	 ewf_test_check_file_signature,
	 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_check_file_signature_wide",
	 ewf_test_check_file_signature_wide,
	 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_check_file_signature_file_io_handle",
	 ewf_test_check_file_signature_file_io_handle,
	 source );

#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_glob_determine_format",
	 ewf_test_glob_determine_format );

	EWF_TEST_RUN(
	 "libewf_glob_get_segment_filename",
	 ewf_test_glob_get_segment_filename );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_glob",
	 ewf_test_glob,
	 source );

	EWF_TEST_RUN(
	 "libewf_glob_free",
	 ewf_test_glob_free );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
#if defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT )

	EWF_TEST_RUN(
	 "libewf_glob_wide_determine_format",
	 ewf_test_glob_wide_determine_format );

	EWF_TEST_RUN(
	 "libewf_glob_wide_get_segment_filename",
	 ewf_test_glob_wide_get_segment_filename );

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_RUN_WITH_ARGS(
	 "libewf_glob_wide",
	 ewf_test_glob_wide,
	 source );

	EWF_TEST_RUN(
	 "libewf_glob_wide_free",
	 ewf_test_glob_wide_free );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


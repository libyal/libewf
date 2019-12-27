/*
 * Library access_control_entry type test program
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "ewf_test_libcerror.h"
#include "ewf_test_libewf.h"
#include "ewf_test_libfvalue.h"
#include "ewf_test_macros.h"
#include "ewf_test_memory.h"
#include "ewf_test_unused.h"

#include "../libewf/libewf_access_control_entry.h"
#include "../libewf/libewf_lef_permission.h"

uint8_t ewf_test_access_control_entry_types_data1[ 17 ] = {
	0x70, 0x09, 0x6e, 0x09, 0x73, 0x09, 0x70, 0x72, 0x09, 0x6e, 0x74, 0x61, 0x09, 0x6e, 0x74, 0x69,
	0x0a };

/* p	n	s		pr	nta	nti
 * 	System	S-1-5-18	2	2032127	16
 */
uint8_t ewf_test_access_control_entry_values_data3[ 30 ] = {
	0x09, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x09, 0x53, 0x2d, 0x31, 0x2d, 0x35, 0x2d, 0x31, 0x38,
	0x09, 0x32, 0x09, 0x32, 0x30, 0x33, 0x32, 0x31, 0x32, 0x37, 0x09, 0x31, 0x36, 0x0d };

/* Tests the libewf_access_control_entry_free function
 * Returns 1 if successful or 0 if not
 */
int ewf_test_access_control_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libewf_access_control_entry_free(
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

	/* TODO: add tests for libewf_access_control_entry_initialize */

#endif /* defined( __GNUC__ ) && !defined( LIBEWF_DLL_IMPORT ) */

	EWF_TEST_RUN(
	 "libewf_access_control_entry_free",
	 ewf_test_access_control_entry_free );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


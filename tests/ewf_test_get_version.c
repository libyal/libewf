/*
 * Library get version test program
 *
 * Copyright (C) 2006-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "ewf_test_libcstring.h"
#include "ewf_test_libewf.h"
#include "ewf_test_unused.h"

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#else
int main( int argc, char * const argv[] EWF_TEST_ATTRIBUTE_UNUSED )
#endif
{
	const char *version_string = NULL;

	EWF_TEST_UNREFERENCED_PARAMETER( argv )

	if( argc != 1 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	version_string = libewf_get_version();

	if( libcstring_narrow_string_compare(
	     version_string,
	     LIBEWF_VERSION_STRING,
	     9 ) != 0 )
	{
		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}


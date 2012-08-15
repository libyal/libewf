/*
 * Expert Witness Compression Format (EWF) library truncate program
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "ewf_test_libcfile.h"
#include "ewf_test_libcnotify.h"
#include "ewf_test_libcstring.h"
#include "ewf_test_libcsystem.h"

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error = NULL;
	libcfile_file_t *file    = NULL;
	size64_t file_size       = 0;
	size_t string_length     = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing file size.\n" );

		return( EXIT_FAILURE );
	}
	if( argc < 3 )
	{
		fprintf(
		 stderr,
		 "Missing filename.\n" );

		return( EXIT_FAILURE );
	}
	string_length = libcstring_system_string_length(
	                 argv[ 1 ]  );

	if( libcsystem_string_decimal_copy_to_64_bit(
	     argv[ 1 ],
	     string_length + 1,
	     &file_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unsupported file size.\n" );

		goto on_error;
	}
	if( libcfile_file_initialize(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file." );

		goto on_error;
	}
	if( libcfile_file_open(
	     file,
	     argv[ 2 ],
	     LIBCFILE_OPEN_WRITE,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ 2 ] );

		goto on_error;
	}
	if( libcfile_file_resize(
	     file,
	     file_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resize file.\n" );

		goto on_error;
	}
	if( libcfile_file_close(
	     file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file.\n" );

		goto on_error;
	}
	if( libcfile_file_free(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libcfile_file_free(
		 &file,
		 NULL );
	}
	return( EXIT_FAILURE );
}


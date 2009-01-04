/*
 * File IO functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <types.h>

#include <errno.h>

#include "libewf_error.h"
#include "libewf_file_io.h"
#include "libewf_system_string.h"

/* Function to open a file
 * Returns the file descriptor or -1 on error 
 */
int libewf_file_io_open(
     const libewf_system_character_t *filename,
     int flags,
     libewf_error_t **error )
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_TYPE ) && !defined( HAVE_WINDOWS_API )
	char *narrow_filename       = NULL;
	size_t filename_size        = 0;
	size_t narrow_filename_size = 0;
#endif

	static char *function       = "libewf_file_io_open";
	int file_descriptor         = 0;

	if( filename == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WINDOWS_API )
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_TYPE )
	if( _wsopen_s(
	     &file_descriptor,
	     filename,
	     ( flags | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
	{
#else
	if( _sopen_s(
	     &file_descriptor,
	     filename,
	     ( flags | _O_BINARY ),
	     _SH_DENYRW,
	     ( _S_IREAD | _S_IWRITE ) ) != 0 )
#endif
#else
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_TYPE )
	filename_size = 1 + libewf_system_string_length(
	                     filename );

	if( utf8_string_size_from_libewf_system_string(
	     filename,
	     filename_size,
	     &narrow_filename_size,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow character filename size.\n",
		 function );

		return( -1 );
	}
	narrow_filename = (char *) memory_allocate(
	                            narrow_filename_size );

	if( narrow_filename == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_MEMORY,
		 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create narrow character filename.\n",
		 function );

		return( -1 );
	}
	if( utf8_string_copy_from_libewf_system_string(
	     narrow_filename,
	     narrow_filename_size,
	     filename,
	     filename_size,
	     error ) != 1 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_CONVERSION,
		 LIBEWF_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow character filename.\n",
		 function );

		return( -1 );
	}
	file_descriptor = open(
	                   narrow_filename,
	                   flags,
	                   0644 );

	memory_free(
	 narrow_filename );
#else
	file_descriptor = open(
	                   filename,
	                   flags,
	                   0644 );
#endif

	if( file_descriptor == -1 )
#endif
	{
		switch( errno )
		{
			case EACCES:
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_IO,
				 LIBEWF_IO_ERROR_ACCESS_DENIED,
				 "%s: access denied to file: %" PRIs_LIBEWF_SYSTEM ".\n",
				 function,
				 filename );

				break;

			case ENOENT:
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_IO,
				 LIBEWF_IO_ERROR_INVALID_RESOURCE,
				 "%s: no such file: %" PRIs_LIBEWF_SYSTEM ".\n",
				 function,
				 filename );

				break;

			default:
				libewf_error_set(
				 error,
				 LIBEWF_ERROR_DOMAIN_IO,
				 LIBEWF_IO_ERROR_OPEN_FAILED,
				 "%s: error opening file: %" PRIs_LIBEWF_SYSTEM ".\n",
				 function,
				 filename );

				break;
		}
		return( -1 );
	}
	return( file_descriptor );
}

/* Function to determine if a file exists
 * Return 1 if file exists, 0 if not or -1 on error
 */
int libewf_file_io_exists(
     const libewf_system_character_t *filename,
     libewf_error_t **error )
{
	libewf_error_t *local_error = NULL;
	static char *function       = "libewf_file_io_exists";
	int file_descriptor         = 0;
	int result                  = 0;

	if( filename == NULL )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( error == NULL )
	{
		error = &local_error;
	}
	file_descriptor = libewf_file_io_open(
	                   filename,
	                   LIBEWF_FILE_IO_O_RDONLY,
	                   error );

	if( file_descriptor == -1 )
	{
		if( libewf_error_matches(
		     *error,
		     LIBEWF_ERROR_DOMAIN_IO,
		     LIBEWF_IO_ERROR_ACCESS_DENIED ) != 0 )
		{
			result = 1;
		}
		else if( libewf_error_matches(
		          *error,
		          LIBEWF_ERROR_DOMAIN_IO,
		          LIBEWF_IO_ERROR_INVALID_RESOURCE ) != 0 )
		{
			result = 0;
		}
		else
		{
			result = -1;
		}
		if( error == &local_error )
		{
			libewf_error_free(
			 error );
		}
	}
	else if( libewf_file_io_close(
		  file_descriptor ) != 0 )
	{
		libewf_error_set(
		 error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %" PRIs_LIBEWF_SYSTEM ".\n",
		 function,
		 filename );

		return( -1 );
	}
	return( result );
}


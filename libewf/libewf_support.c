/*
 * Support functions
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include <stdio.h>

#include "libewf_codepage.h"
#include "libewf_definitions.h"
#include "libewf_filename.h"
#include "libewf_error.h"
#include "libewf_libbfio.h"
#include "libewf_segment_file.h"
#include "libewf_support.h"

#include "ewf_definitions.h"

#if !defined( HAVE_LOCAL_LIBEWF )

/* Returns the library version as a string
 */
const char *libewf_get_version(
             void )
{
	return( (const char *) LIBEWF_VERSION_STRING );
}

/* Returns the access flags for reading
 */
int libewf_get_access_flags_read(
     void )
{
	return( (int) LIBEWF_ACCESS_FLAG_READ );
}

/* Returns the access flags for reading and writing
 */
int libewf_get_access_flags_read_write(
     void )
{
	return( (int) ( LIBEWF_ACCESS_FLAG_READ | LIBEWF_ACCESS_FLAG_WRITE ) );
}

/* Returns the access flags for writing
 */
int libewf_get_access_flags_write(
     void )
{
	return( (int) LIBEWF_ACCESS_FLAG_WRITE );
}

/* Returns the access flags for resume writing
 */
int libewf_get_access_flags_write_resume(
     void )
{
	return( (int) LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME );
}

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libewf_get_codepage(
     int *codepage,
     liberror_error_t **error )
{
	static char *function = "libewf_get_codepage";

	if( codepage == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid codepage.",
		 function );

		return( -1 );
	}
	*codepage = libcstring_narrow_system_string_codepage;

	return( 1 );
}

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libewf_set_codepage(
     int codepage,
     liberror_error_t **error )
{
	static char *function = "libewf_set_codepage";

	if( ( codepage != LIBEWF_CODEPAGE_ASCII )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_1 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_2 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_3 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_4 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_5 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_6 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_7 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_8 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_9 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_10 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_11 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_13 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_14 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_15 )
	 && ( codepage != LIBEWF_CODEPAGE_ISO_8859_16 )
	 && ( codepage != LIBEWF_CODEPAGE_KOI8_R )
	 && ( codepage != LIBEWF_CODEPAGE_KOI8_U )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_874 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1250 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1251 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1252 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1253 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1254 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1256 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1257 )
	 && ( codepage != LIBEWF_CODEPAGE_WINDOWS_1258 )
	 && ( codepage != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported codepage.",
		 function );

		return( -1 );
	}
	libcstring_narrow_system_string_codepage = codepage;

	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBEWF ) */

/* Determines if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
int libewf_check_file_signature(
     const char *filename,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_check_file_signature";
	size_t filename_length           = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = libcstring_narrow_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	result = libewf_check_file_signature_file_io_handle(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to check file signature using a file handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Determines if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_check_file_signature_wide(
     const wchar_t *filename,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_check_file_signature_wide";
	size_t filename_length           = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = libcstring_wide_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	result = libewf_check_file_signature_file_io_handle(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to check file signature using a file handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}
#endif

/* Determines if a file is an EWF file (check for the EWF file signature) using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error )
{
	uint8_t signature[ 8 ];

	static char *function      = "libewf_check_file_signature_file_io_handle";
	ssize_t read_count         = 0;
	int file_io_handle_is_open = 0;

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     LIBBFIO_OPEN_READ,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file.",
			 function );

			return( -1 );
		}
	}
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		if( file_io_handle_is_open == 0 )
		{
			libbfio_handle_close(
			 file_io_handle,
			 error );
		}
		return( -1 );
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              signature,
	              8,
	              error );

	if( read_count != 8 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature.",
		 function );

		libbfio_handle_close(
		 file_io_handle,
		 NULL );

		return( -1 );
	}
	if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_close(
		     file_io_handle,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file.",
			 function );

			return( -1 );
		}
	}
	/* The number of EWF segment files will be the largest
	 */
	if( memory_compare(
	     evf_file_signature,
	     signature,
	     8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          lvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          dvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
int libewf_glob(
     const char *filename,
     size_t filename_length,
     uint8_t format,
     char **filenames[],
     int *number_of_filenames,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	char *segment_filename           = NULL;
	void *reallocation               = NULL;
	static char *function            = "libewf_glob";
	size_t additional_length         = 4;
	size_t segment_filename_length   = 0;
	int result                       = 0;
	uint8_t segment_file_type        = 0;
	uint8_t ewf_format               = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_length == 0 )
	 || ( filename_length > (size_t) SSIZE_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length.",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_UNKNOWN )
	 && ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK )
	 && ( format != LIBEWF_FORMAT_LVF )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( filename[ filename_length - 4 ] != (char) '.' )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - missing extension.",
			 function );

			return( -1 );
		}
		additional_length = 0;

		if( filename[ filename_length - 3 ] == (char) 'E' )
		{
			format = LIBEWF_FORMAT_ENCASE5;
		}
		else if( filename[ filename_length - 3 ] == (char) 'e' )
		{
			format = LIBEWF_FORMAT_EWF;
		}
		else if( filename[ filename_length - 3 ] == (char) 'L' )
		{
			format = LIBEWF_FORMAT_LVF;
		}
		else if( filename[ filename_length - 3 ] == (char) 's' )
		{
			format = LIBEWF_FORMAT_SMART;
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - unsupported extension: %s.",
			 function,
			 &( filename[ filename_length - 4 ] ) );

			return( -1 );
		}
	}
	if( format == LIBEWF_FORMAT_LVF )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
		ewf_format        = EWF_FORMAT_L01;
	}
	else if( format == LIBEWF_FORMAT_SMART )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_S01;
	}
	else
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_E01;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	*number_of_filenames = 0;

	while( *number_of_filenames < (int) UINT16_MAX )
	{
		segment_filename_length = filename_length + additional_length;

		segment_filename = (char * ) memory_allocate(
			                      sizeof( char ) * ( segment_filename_length + 1 ) );

		if( segment_filename == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.",
			 function );

			goto on_error;
		}
		if( libcstring_narrow_string_copy(
		     segment_filename,
		     filename,
		     filename_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy filename.",
			 function );

			goto on_error;
		}
		if( additional_length > 0 )
		{
			segment_filename[ filename_length ] = (char) '.';
		}
		if( libewf_filename_set_extension(
		     &( segment_filename[ segment_filename_length - 3 ] ),
		     (uint16_t) ( *number_of_filenames + 1 ),
		     UINT16_MAX,
		     segment_file_type,
		     format,
		     ewf_format,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set extension.",
			 function );

			goto on_error;
		}
		segment_filename[ segment_filename_length ] = 0;

		if( libbfio_file_set_name(
		     file_io_handle,
		     segment_filename,
		     segment_filename_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name in file IO handle.",
			 function );

			goto on_error;
		}
		result = libbfio_handle_exists(
		          file_io_handle,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_GENERIC,
			 "%s: unable to test if file exists.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		*number_of_filenames += 1;

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( char * ) * *number_of_filenames );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize filenames.",
			 function );

			goto on_error;
		}
		*filenames = (char **) reallocation;

		( *filenames )[ *number_of_filenames - 1 ] = segment_filename;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Frees the globbed filenames
 * Returns 1 if successful or -1 on error
 */
int libewf_glob_free(
     char *filenames[],
     int number_of_filenames,
     liberror_error_t **error )
{
	static char *function = "libewf_glob_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
int libewf_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     uint8_t format,
     wchar_t **filenames[],
     int *number_of_filenames,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	wchar_t *segment_filename        = NULL;
	void *reallocation               = NULL;
	static char *function            = "libewf_glob_wide";
	size_t additional_length         = 4;
	size_t segment_filename_length   = 0;
	int result                       = 0;
	uint8_t segment_file_type        = 0;
	uint8_t ewf_format               = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_length == 0 )
	 || ( filename_length > (size_t) SSIZE_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length.",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_UNKNOWN )
	 && ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK )
	 && ( format != LIBEWF_FORMAT_LVF )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( filename[ filename_length - 4 ] != (wchar_t) '.' )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - missing extension.",
			 function );

			return( -1 );
		}
		additional_length = 0;

		if( filename[ filename_length - 3 ] == (wchar_t) 'E' )
		{
			format = LIBEWF_FORMAT_ENCASE5;
		}
		else if( filename[ filename_length - 3 ] == (wchar_t) 'e' )
		{
			format = LIBEWF_FORMAT_EWF;
		}
		else if( filename[ filename_length - 3 ] == (wchar_t) 'L' )
		{
			format = LIBEWF_FORMAT_LVF;
		}
		else if( filename[ filename_length - 3 ] == (wchar_t) 's' )
		{
			format = LIBEWF_FORMAT_SMART;
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - unsupported extension: %s.",
			 function,
			 &( filename[ filename_length - 4 ] ) );

			return( -1 );
		}
	}
	if( format == LIBEWF_FORMAT_LVF )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
		ewf_format        = EWF_FORMAT_L01;
	}
	else if( format == LIBEWF_FORMAT_SMART )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_S01;
	}
	else
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_E01;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	*number_of_filenames = 0;

	while( *number_of_filenames < (int) UINT16_MAX )
	{
		segment_filename_length = filename_length + additional_length;

		segment_filename = (wchar_t *) memory_allocate(
			                        sizeof( wchar_t ) * ( segment_filename_length + 1 ) );

		if( segment_filename == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.",
			 function );

			goto on_error;
		}
		if( libcstring_wide_string_copy(
		     segment_filename,
		     filename,
		     filename_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy filename.",
			 function );

			goto on_error;
		}
		if( additional_length > 0 )
		{
			segment_filename[ filename_length ] = (wchar_t) '.';
		}
		if( libewf_filename_set_extension_wide(
		     &( segment_filename[ segment_filename_length - 3 ] ),
		     (uint16_t) ( *number_of_filenames + 1 ),
		     UINT16_MAX,
		     segment_file_type,
		     format,
		     ewf_format,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set extension.",
			 function );

			goto on_error;
		}
		segment_filename[ segment_filename_length ] = 0;

		if( libbfio_file_set_name_wide(
		     file_io_handle,
		     segment_filename,
		     segment_filename_length,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name in file IO handle.",
			 function );

			goto on_error;
		}
		result = libbfio_handle_exists(
		          file_io_handle,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_GENERIC,
			 "%s: unable to test if file exists.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		*number_of_filenames += 1;

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( wchar_t * ) * *number_of_filenames );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize filenames.",
			 function );

			goto on_error;
		}
		*filenames = (wchar_t **) reallocation;

		( *filenames )[ *number_of_filenames - 1 ] = segment_filename;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Frees the globbed wide filenames
 * Returns 1 if successful or -1 on error
 */
int libewf_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     liberror_error_t **error )
{
	static char *function = "libewf_glob_wide_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

	return( 1 );
}

#endif


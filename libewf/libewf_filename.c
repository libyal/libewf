/*
 * Filename functions
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_definitions.h"
#include "libewf_filename.h"
#include "libewf_libcerror.h"

/* Sets the extension for a certain segment file
 * Using the format specific naming schema
 * Returns 1 on success or -1 on error
 */
int libewf_filename_set_extension(
     char *filename,
     size_t filename_size,
     size_t *filename_index,
     uint32_t segment_number,
     uint32_t maximum_number_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error )
{
	static char *function           = "libewf_filename_set_extension";
	size_t minimum_filename_size    = 0;
	size_t string_index             = 0;
	uint32_t maximum_segment_number = 0;
	char additional_characters      = 0;
	char first_character            = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_size == 0 )
	 || ( filename_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename size value out of bounds.",
		 function );

		return( -1 );
	}
	if( filename_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename index.",
		 function );

		return( -1 );
	}
	string_index = *filename_index;

	if( ( segment_number == 0 )
	 || ( segment_number > maximum_number_of_segments ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: segment number: %" PRIu32 " is out of bounds.",
		 function,
		 segment_number );

		return( -1 );
	}
	if( ( format == LIBEWF_FORMAT_EWF )
	 || ( format == LIBEWF_FORMAT_EWFX ) )
	{
		first_character       = 'e';
		additional_characters = 'a';
	}
	else if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	      || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 ) )
	{
		first_character       = 'E';
		additional_characters = 'A';
	}
	else if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	      || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		first_character       = 'L';
		additional_characters = 'A';
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		first_character       = 's';
		additional_characters = 'a';
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		return( -1 );
	}
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		minimum_filename_size  = 6;
		maximum_segment_number = 3;
	}
	else
	{
		minimum_filename_size  = 5;
		maximum_segment_number = 25;
	}
	if( filename_size < minimum_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename size value out of bounds.",
		 function );

		return( -1 );
	}
	minimum_filename_size -= 1;

	if( string_index > ( filename_size - minimum_filename_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: filename index value is out of bounds.",
		 function );

		return( -1 );
	}
	filename[ string_index ] = first_character;

	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		filename[ ++string_index ] = 'x';
	}
	if( segment_number <= 99 )
	{
		filename[ string_index + 2 ] = '0' + (char) ( segment_number % 10 );
		filename[ string_index + 1 ] = '0' + (char) ( segment_number / 10 );
	}
	else if( segment_number >= 100 )
	{
		segment_number -= 100;

		filename[ string_index + 2 ] = additional_characters + (char) ( segment_number % 26 );
		segment_number              /= 26;

		filename[ string_index + 1 ] = additional_characters + (char) ( segment_number % 26 );
		segment_number              /= 26;

		if( segment_number > maximum_segment_number )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unable to support for more segment files.",
			 function );

			return( -1 );
		}
		filename[ string_index ] += (char) segment_number;
	}
	/* Safety check
	 */
	if( ( filename[ string_index ] > 'z' )
	 || ( ( filename[ string_index ] > 'Z' )
	  &&  ( filename[ string_index ] < 'a' ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to support for more segment files.",
		 function );

		return( -1 );
	}
	string_index += 3;

	filename[ string_index++ ] = 0;

	*filename_index = string_index;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Sets the extension for a certain segment file
 * Using the format specific naming schema
 * Returns 1 on success or -1 on error
 */
int libewf_filename_set_extension_wide(
     wchar_t *filename,
     size_t filename_size,
     size_t *filename_index,
     uint32_t segment_number,
     uint32_t maximum_number_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error )
{
	static char *function           = "libewf_filename_set_extension_wide";
	size_t minimum_filename_size    = 0;
	size_t string_index             = 0;
	uint32_t maximum_segment_number = 0;
	wchar_t additional_characters   = 0;
	wchar_t first_character         = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( filename_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename index.",
		 function );

		return( -1 );
	}
	string_index = *filename_index;

	if( ( segment_number == 0 )
	 || ( segment_number > maximum_number_of_segments ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: segment number: %" PRIu32 " is out of bounds.",
		 function,
		 segment_number );

		return( -1 );
	}
	if( ( format == LIBEWF_FORMAT_EWF )
	 || ( format == LIBEWF_FORMAT_EWFX ) )
	{
		first_character       = (wchar_t) 'e';
		additional_characters = (wchar_t) 'a';
	}
	else if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1 )
	      || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 ) )
	{
		first_character       = (wchar_t) 'E';
		additional_characters = (wchar_t) 'A';
	}
	else if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL )
	      || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		first_character       = (wchar_t) 'L';
		additional_characters = (wchar_t) 'A';
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART )
	{
		first_character       = (wchar_t) 's';
		additional_characters = (wchar_t) 'a';
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		return( -1 );
	}
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		minimum_filename_size  = 6;
		maximum_segment_number = 3;
	}
	else
	{
		minimum_filename_size  = 5;
		maximum_segment_number = 25;
	}
	if( filename_size < minimum_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename size value out of bounds.",
		 function );

		return( -1 );
	}
	minimum_filename_size -= 1;

	if( string_index > ( filename_size - minimum_filename_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: filename index value is out of bounds.",
		 function );

		return( -1 );
	}
	filename[ string_index ] = first_character;

	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		filename[ ++string_index ] = 'x';
	}
	if( segment_number <= 99 )
	{
		filename[ string_index + 2 ] = (wchar_t) '0' + (wchar_t) ( segment_number % 10 );
		filename[ string_index + 1 ] = (wchar_t) '0' + (wchar_t) ( segment_number / 10 );
	}
	else if( segment_number >= 100 )
	{
		segment_number -= 100;

		filename[ string_index + 2 ] = additional_characters + (wchar_t) ( segment_number % 26 );
		segment_number              /= 26;

		filename[ string_index + 1 ] = additional_characters + (wchar_t) ( segment_number % 26 );
		segment_number              /= 26;

		if( segment_number >= maximum_segment_number )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unable to support for more segment files.",
			 function );

			return( -1 );
		}
		filename[ string_index ] += (wchar_t) segment_number;
	}
	/* Safety check
	 */
	if( ( filename[ string_index ] > (wchar_t) 'z' )
	 || ( ( filename[ string_index ] > (wchar_t) 'Z' )
	  &&  ( filename[ string_index ] < (wchar_t) 'a' ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to support for more segment files.",
		 function );

		return( -1 );
	}
	string_index += 3;

	filename[ string_index++ ] = 0;

	*filename_index = string_index;

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Creates a filename for a certain segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_filename_create(
     system_character_t **filename,
     size_t *filename_size,
     system_character_t *basename,
     size_t basename_length,
     uint32_t segment_number,
     uint32_t maximum_number_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     libcerror_error_t **error )
{
	system_character_t *safe_filename = NULL;
	static char *function             = "libewf_filename_create";
	size_t safe_filename_size         = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( *filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: filename already set.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename size.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2 )
	 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL ) )
	{
		/* The actual filename also contains a '.', 4 character extension and a end of string byte
		 */
		safe_filename_size = basename_length + 6;
	}
	else
	{
		/* The actual filename also contains a '.', 3 character extension and a end of string byte
		 */
		safe_filename_size = basename_length + 5;
	}
	safe_filename = system_string_allocate(
	                 safe_filename_size );

	if( safe_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create filename.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     safe_filename,
	     basename,
	     basename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		goto on_error;
	}
	safe_filename[ basename_length++ ] = (system_character_t) '.';

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_filename_set_extension_wide(
	     safe_filename,
	     safe_filename_size,
	     &basename_length,
	     segment_number,
	     maximum_number_of_segments,
	     segment_file_type,
	     format,
	     error ) != 1 )
#else
	if( libewf_filename_set_extension(
	     safe_filename,
	     safe_filename_size,
	     &basename_length,
	     segment_number,
	     maximum_number_of_segments,
	     segment_file_type,
	     format,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set extension.",
		 function );

		goto on_error;
	}
	*filename      = safe_filename;
	*filename_size = safe_filename_size;

	return( 1 );

on_error:
	if( safe_filename != NULL )
	{
		memory_free(
		 safe_filename );
	}
	return( -1 );
}


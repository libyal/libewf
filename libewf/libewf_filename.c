/*
 * Filename functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libewf_definitions.h"
#include "libewf_filename.h"
#include "libewf_system_string.h"

#include "ewf_definitions.h"

/* Creates the filename extension for a certain segment file
 * For EWF-E01, EWF-S01 segment file extension naming schema
 * Returns 1 on success or -1 on error
 */
int libewf_filename_set_extension(
     char *extension,
     uint16_t segment_number,
     uint16_t maximum_amount_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format,
     liberror_error_t **error )
{
	static char *function                = "libewf_filename_set_extension";
	char extension_first_character       = 0;
	char extension_additional_characters = 0;

	if( extension == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extension.",
		 function );

		return( -1 );
	}
	if( ( segment_number == 0 )
	 || ( segment_number > maximum_amount_of_segments ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: segment number: %" PRIu16 " is out of range.",
		 function,
		 segment_number );

		return( -1 );
	}
	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	{
		if( ( format == LIBEWF_FORMAT_EWF )
		 || ( format == LIBEWF_FORMAT_EWFX ) )
		{
			extension_first_character       = (char) 'e';
			extension_additional_characters = (char) 'a';
		}
		else if( ewf_format == EWF_FORMAT_S01 )
		{
			extension_first_character       = (char) 's';
			extension_additional_characters = (char) 'a';
		}
		else
		{
			extension_first_character       = (char) 'E';
			extension_additional_characters = (char) 'A';
		}
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF )
	{
		extension_first_character       = (char) 'L';
		extension_additional_characters = (char) 'A';
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		extension_first_character       = (char) 'd';
		extension_additional_characters = (char) 'a';
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		return( -1 );
	}
	extension[ 0 ] = extension_first_character;

	if( segment_number <= 99 )
	{
		extension[ 2 ] = (char) '0' + (char) ( segment_number % 10 );
		extension[ 1 ] = (char) '0' + (char) ( segment_number / 10 );
	}
	else if( segment_number >= 100 )
	{
		segment_number -= 100;
		extension[ 2 ]  = extension_additional_characters + (char) ( segment_number % 26 );
		segment_number /= 26;
		extension[ 1 ]  = extension_additional_characters + (char) ( segment_number % 26 );
		segment_number /= 26;

		if( segment_number >= 26 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unable to support for more segment files.",
			 function );

			return( -1 );
		}
		extension[ 0 ] = extension_first_character + (char) segment_number;
	}
	/* Safety check
	 */
	if( ( extension[ 0 ] > (char) 'z' )
	 || ( ( extension[ 0 ] > (char) 'Z' )
	  && ( extension[ 0 ] < (char) 'a' ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to support for more segment files.",
		 function );

		return( -1 );
	}
	extension[ 3 ] = 0;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Creates the filename extension for a certain segment file
 * For EWF-E01, EWF-S01 segment file extension naming schema
 * Returns 1 on success or -1 on error
 */
int libewf_filename_set_extension_wide(
     wchar_t *extension,
     uint16_t segment_number,
     uint16_t maximum_amount_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format,
     liberror_error_t **error )
{
	static char *function                   = "libewf_filename_set_extension_wide";
	wchar_t extension_first_character       = 0;
	wchar_t extension_additional_characters = 0;

	if( extension == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extension.",
		 function );

		return( -1 );
	}
	if( ( segment_number == 0 )
	 || ( segment_number > maximum_amount_of_segments ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: segment number: %" PRIu16 " is out of range.",
		 function,
		 segment_number );

		return( -1 );
	}
	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	{
		if( ( format == LIBEWF_FORMAT_EWF )
		 || ( format == LIBEWF_FORMAT_EWFX ) )
		{
			extension_first_character       = (wchar_t) 'e';
			extension_additional_characters = (wchar_t) 'a';
		}
		else if( ewf_format == EWF_FORMAT_S01 )
		{
			extension_first_character       = (wchar_t) 's';
			extension_additional_characters = (wchar_t) 'a';
		}
		else
		{
			extension_first_character       = (wchar_t) 'E';
			extension_additional_characters = (wchar_t) 'A';
		}
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF )
	{
		extension_first_character       = (wchar_t) 'L';
		extension_additional_characters = (wchar_t) 'A';
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		extension_first_character       = (wchar_t) 'd';
		extension_additional_characters = (wchar_t) 'a';
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported segment file type.",
		 function );

		return( -1 );
	}
	extension[ 0 ] = extension_first_character;

	if( segment_number <= 99 )
	{
		extension[ 2 ] = (wchar_t) '0' + (wchar_t) ( segment_number % 10 );
		extension[ 1 ] = (wchar_t) '0' + (wchar_t) ( segment_number / 10 );
	}
	else if( segment_number >= 100 )
	{
		segment_number -= 100;
		extension[ 2 ]  = extension_additional_characters + (wchar_t) ( segment_number % 26 );
		segment_number /= 26;
		extension[ 1 ]  = extension_additional_characters + (wchar_t) ( segment_number % 26 );
		segment_number /= 26;

		if( segment_number >= 26 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unable to support for more segment files.",
			 function );

			return( -1 );
		}
		extension[ 0 ] = extension_first_character + (wchar_t) segment_number;
	}
	/* Safety check
	 */
	if( ( extension[ 0 ] > (wchar_t) 'z' )
	 || ( ( extension[ 0 ] > (wchar_t) 'Z' )
	  && ( extension[ 0 ] < (wchar_t) 'a' ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to support for more segment files.",
		 function );

		return( -1 );
	}
	extension[ 3 ] = 0;

	return( 1 );
}

#endif

/* Creates a filename for a certain segment file
 * Returns 1 if successful or -1 on error
 */
int libewf_filename_create(
     libewf_system_character_t **filename,
     size_t *filename_size,
     libewf_system_character_t *basename,
     size_t basename_length,
     uint16_t segment_number,
     uint16_t maximum_amount_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format,
     liberror_error_t **error )
{
	libewf_system_character_t *new_filename = NULL;
	static char *function                   = "libewf_filename_create";

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
	if( *filename != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: filename already set.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename size.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	/* The actual filename also contains a '.', 3 character extension and a end of string byte
	 */
	new_filename = memory_allocate(
	                sizeof( libewf_system_character_t ) * ( basename_length + 5 ) );

	if( new_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create filename.",
		 function );

		return( -1 );
	}
	/* Add one additional character for the end of line
	 */
	if( libewf_system_string_copy(
	     new_filename,
	     basename,
	     ( basename_length + 1 ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		memory_free(
		 new_filename );

		return( -1 );
	}
	new_filename[ basename_length ] = (libewf_system_character_t) '.';

#if defined( LIBEWF_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libewf_filename_set_extension_wide(
	     &( new_filename[ basename_length + 1 ] ),
	     segment_number,
	     maximum_amount_of_segments,
	     segment_file_type,
	     format,
	     ewf_format,
	     error ) != 1 )
#else
	if( libewf_filename_set_extension(
	     &( new_filename[ basename_length + 1 ] ),
	     segment_number,
	     maximum_amount_of_segments,
	     segment_file_type,
	     format,
	     ewf_format,
	     error ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set extension.",
		 function );

		memory_free(
		 new_filename );

		return( -1 );
	}
	*filename      = new_filename;
	*filename_size = basename_length + 5;

	return( 1 );
}


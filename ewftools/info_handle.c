/*
 * Info handle
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
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

#include "byte_size_string.h"
#include "digest_hash.h"
#include "ewfinput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libcsplit.h"
#include "ewftools_libewf.h"
#include "ewftools_libfdatetime.h"
#include "guid.h"
#include "info_handle.h"
#include "platform.h"

#define INFO_HANDLE_VALUE_SIZE			512
#define INFO_HANDLE_VALUE_IDENTIFIER_SIZE	64
#define INFO_HANDLE_NOTIFY_STREAM		stdout

#if !defined( USE_LIBEWF_GET_HASH_VALUE_MD5 ) && !defined( USE_LIBEWF_GET_MD5_HASH )
#define USE_LIBEWF_GET_HASH_VALUE_MD5
#define DIGEST_HASH_STRING_SIZE_MD5		33
#endif

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
			info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libewf_handle_initialize(
	     &( ( *info_handle )->input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input handle.",
		 function );

		goto on_error;
	}
	( *info_handle )->output_format   = INFO_HANDLE_OUTPUT_FORMAT_TEXT;
	( *info_handle )->date_format     = LIBEWF_DATE_FORMAT_CTIME;
	( *info_handle )->header_codepage = LIBEWF_CODEPAGE_ASCII;
	( *info_handle )->notify_stream   = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->bodyfile_stream != NULL )
		{
			if( file_stream_close(
			     ( *info_handle )->bodyfile_stream ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close bodyfile stream.",
				 function );

				result = -1;
			}
			( *info_handle )->bodyfile_stream = NULL;
		}
		if( ( *info_handle )->input_handle != NULL )
		{
			if( libewf_handle_free(
			     &( ( *info_handle )->input_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input handle.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle != NULL )
	{
		if( libewf_handle_signal_abort(
		     info_handle->input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the bodyfile
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_bodyfile(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_bodyfile";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - bodyfile stream value already set.",
		 function );

		return( -1 );
	}
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	info_handle->bodyfile_stream = file_stream_open_wide(
	                                filename,
	                                "wb" );
#else
	info_handle->bodyfile_stream = file_stream_open(
	                                filename,
	                                "wb" );
#endif
	if( info_handle->bodyfile_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open bodyfile stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_maximum_number_of_open_handles(
     info_handle_t *info_handle,
     int maximum_number_of_open_handles,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_maximum_number_of_open_handles";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_maximum_number_of_open_handles(
	     info_handle->input_handle,
	     maximum_number_of_open_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set maximum number of open handles in input handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the input of the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	system_character_t **libewf_filenames = NULL;
	static char *function                 = "info_handle_open_input";
	size_t first_filename_length          = 0;
	int filename_index                    = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == 1 )
	{
		first_filename_length = system_string_length(
		                         filenames[ 0 ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_glob_wide(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &number_of_filenames,
		     error ) != 1 )
#else
		if( libewf_glob(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &number_of_filenames,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			goto on_error;
		}
		filenames = (system_character_t * const *) libewf_filenames;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libewf_handle_open_wide(
	     info_handle->input_handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	if( libewf_handle_open(
	     info_handle->input_handle,
	     filenames,
	     number_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file(s).",
		 function );

		goto on_error;
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t<image_filenames>\n" );

		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t\t\t<image_filename>%" PRIs_SYSTEM "</image_filename>\n",
			 filenames[ filename_index ] );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\t\t</image_filenames>\n" );
	}
	if( info_handle->header_codepage != LIBEWF_CODEPAGE_ASCII )
	{
		if( libewf_handle_set_header_codepage(
		     info_handle->input_handle,
		     info_handle->header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header codepage.",
			 function );

			goto on_error;
		}
	}
	if( libewf_filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libewf_glob_wide_free(
		     libewf_filenames,
		     number_of_filenames,
		     error ) != 1 )
#else
		if( libewf_glob_free(
		     libewf_filenames,
		     number_of_filenames,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free globbed filenames.",
			 function );

			return( -1 );
		}
		libewf_filenames = NULL;
	}
	return( 1 );

on_error:
	if( libewf_filenames != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		libewf_glob_wide_free(
		 libewf_filenames,
		 number_of_filenames,
		 NULL );
#else
		libewf_glob_free(
		 libewf_filenames,
		 number_of_filenames,
		 NULL );
#endif
	}
	return( -1 );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_close(
	     info_handle->input_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Sets the output format
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_output_format(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_output_format";
	size_t string_length  = 0;
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 4 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "text" ),
		     4 ) == 0 )
		{
			info_handle->output_format = INFO_HANDLE_OUTPUT_FORMAT_TEXT;
			result                     = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "dfxml" ),
		     5 ) == 0 )
		{
			info_handle->output_format = INFO_HANDLE_OUTPUT_FORMAT_DFXML;
			info_handle->date_format   = LIBEWF_DATE_FORMAT_ISO8601;
			result                     = 1;
		}
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine output format.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the date format
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int info_handle_set_date_format(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_date_format";
	size_t string_length  = 0;
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 2 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "dm" ),
		     2 ) == 0 )
		{
			info_handle->date_format = LIBEWF_DATE_FORMAT_DAYMONTH;
			result                   = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "md" ),
			  3 ) == 0 )
		{
			info_handle->date_format = LIBEWF_DATE_FORMAT_MONTHDAY;
			result                   = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "ctime" ),
		     5 ) == 0 )
		{
			info_handle->date_format = LIBEWF_DATE_FORMAT_CTIME;
			result                   = 1;
		}
	}
	else if( string_length == 7 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "iso8601" ),
		     7 ) == 0 )
		{
			info_handle->date_format = LIBEWF_DATE_FORMAT_ISO8601;
			result                   = 1;
		}
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine date format.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_header_codepage(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_header_codepage";
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	result = ewfinput_determine_header_codepage(
	          string,
	          &( info_handle->header_codepage ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine header codepage.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( info_handle->input_handle != NULL )
		{
			if( libewf_handle_set_header_codepage(
			     info_handle->input_handle,
			     info_handle->header_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set header codepage.",
				 function );

				return( -1 );
			}
		}
	}
	return( result );
}

/* Prints a POSIX value
 * Returns 1 if successful or -1 on error
 */
int info_handle_posix_time_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     int64_t value_64bit,
     libcerror_error_t **error )
{
	system_character_t date_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	static char *function                 = "info_handle_posix_time_fprint";
	int result                            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%s: Not set (0)\n",
		 value_name );
	}
	else
	{
		if( libfdatetime_posix_time_initialize(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create POSIX time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_posix_time_copy_from_64bit(
		     posix_time,
		     (uint64_t) value_64bit,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_64BIT_SIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time from 64-bit value.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_posix_time_copy_to_utf16_string(
			  posix_time,
			  (uint16_t *) date_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_ISO8601 | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#else
		result = libfdatetime_posix_time_copy_to_utf8_string(
			  posix_time,
			  (uint8_t *) date_time_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_ISO8601 | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%s: %" PRIs_SYSTEM "Z\n",
		 value_name,
		 date_time_string );

		if( libfdatetime_posix_time_free(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free POSIX time.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Prints a section header
 * Returns 1 if successful or -1 on error
 */
int info_handle_section_header_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     const char *description,
     libcerror_error_t **error )
{
	static char *function = "info_handle_section_header_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t<%s>\n",
		 identifier );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%s:\n",
		 description );
	}
	return( 1 );
}

/* Prints a section footer
 * Returns 1 if successful or -1 on error
 */
int info_handle_section_footer_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     libcerror_error_t **error )
{
	static char *function = "info_handle_section_footer_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t</%s>\n",
		 identifier );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );
}

/* Prints a string value
 * Returns 1 if successful or -1 on error
 */
int info_handle_section_value_string_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     size_t identifier_length,
     const char *description,
     size_t description_length,
     const system_character_t *value_string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_section_value_string_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		if( identifier_length == 12 )
		{
			if( narrow_string_compare(
			     identifier,
			     "acquiry_date",
			     12 ) == 0 )
			{
				identifier = "acquisition_date";
			}
		}
		else if( identifier_length == 16 )
		{
			if( narrow_string_compare(
			     identifier,
			     "acquiry_software",
			     16 ) == 0 )
			{
				identifier = "acquisition_software";
			}
		}
		else if( identifier_length == 24 )
		{
			if( narrow_string_compare(
			     identifier,
			     "acquiry_operating_system",
			     24 ) == 0 )
			{
				identifier = "acquisition_system";
			}
			else if( narrow_string_compare(
			          identifier,
			          "acquiry_software_version",
			          24 ) == 0 )
			{
				identifier = "acquisition_version";
			}
		}
		fprintf(
		 info_handle->notify_stream,
		 "\t\t\t<%s>%" PRIs_SYSTEM "</%s>\n",
		 identifier,
		 value_string,
		 identifier );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t%s:",
		 description );

		description_length += 1;

		while( description_length < 24 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t" );

			description_length += 8;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "\n",
		 value_string );
	}
	return( 1 );
}

/* Prints a 32-bit value
 * Returns 1 if successful or -1 on error
 */
int info_handle_section_value_32bit_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     const char *description,
     size_t description_length,
     uint32_t value_32bit,
     libcerror_error_t **error )
{
	static char *function = "info_handle_section_value_32bit_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t\t<%s>%" PRIu32 "</%s>\n",
		 identifier,
		 value_32bit,
		 identifier );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t%s:",
		 description );

		description_length += 1;

		while( description_length < 24 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t" );

			description_length += 8;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIu32 "\n",
		 value_32bit );
	}
	return( 1 );
}

/* Prints a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int info_handle_section_value_64bit_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     const char *description,
     size_t description_length,
     uint64_t value_64bit,
     libcerror_error_t **error )
{
	static char *function = "info_handle_section_value_64bit_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t\t<%s>%" PRIu64 "</%s>\n",
		 identifier,
		 value_64bit,
		 identifier );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t%s:",
		 description );

		description_length += 1;

		while( description_length < 24 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t" );

			description_length += 8;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIu64 "\n",
		 value_64bit );
	}
	return( 1 );
}

/* Prints a 64-bit size value
 * Returns 1 if successful or -1 on error
 */
int info_handle_section_value_size_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     const char *description,
     size_t description_length,
     size64_t value_size,
     libcerror_error_t **error )
{
        system_character_t value_size_string[ 16 ];

	static char *function = "info_handle_section_value_size_fprint";
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t\t<%s>%" PRIu64 "</%s>\n",
		 identifier,
		 value_size,
		 identifier );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t%s:",
		 description );

		description_length += 1;

		while( description_length < 24 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t" );

			description_length += 8;
		}
		result = byte_size_string_create(
		          value_size_string,
		          16,
		          value_size,
		          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		          NULL );

		if( result == 1 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIs_SYSTEM " (%" PRIu64 " bytes)\n",
			 value_size_string,
			 value_size );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIu64 " bytes\n",
			 value_size );
		}
	}
	return( 1 );
}

/* Prints a boolean value
 * Returns 1 if successful or -1 on error
 */
int info_handle_section_value_boolean_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     const char *description,
     size_t description_length,
     int value_boolean,
     libcerror_error_t **error )
{
	static char *function = "info_handle_section_value_boolean_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		if( value_boolean != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t\t\t<%s>yes</%s>\n",
			 identifier,
			 identifier );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t\t\t<%s>no</%s>\n",
			 identifier,
			 identifier );
		}
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t%s:",
		 description );

		description_length += 1;

		while( description_length < 24 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t" );

			description_length += 8;
		}
		if( value_boolean != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "yes\n" );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "no\n" );
		}
	}
	return( 1 );
}

/* Prints a header value
 * Returns 1 if successful or -1 on error
 */
int info_handle_header_value_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     size_t identifier_length,
     const char *description,
     size_t description_length,
     libcerror_error_t **error )
{
	system_character_t header_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function    = "info_handle_header_value_fprint";
	size_t header_value_size = INFO_HANDLE_VALUE_SIZE;
	int result               = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_header_value(
	          info_handle->input_handle,
	          (uint8_t *) identifier,
	          identifier_length,
	          (uint16_t *) header_value,
	          header_value_size,
	          error );
#else
	result = libewf_handle_get_utf8_header_value(
	          info_handle->input_handle,
	          (uint8_t *) identifier,
	          identifier_length,
	          (uint8_t *) header_value,
	          header_value_size,
	          error );
#endif

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( info_handle_section_value_string_fprint(
		     info_handle,
		     identifier,
		     identifier_length,
		     description,
		     description_length,
		     header_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section value string: %s.",
			 function,
			 identifier );

			return( -1 );
		}
	}
	return( 1 );
}

/* Prints the header values
 * Returns 1 if successful or -1 on error
 */
int info_handle_header_values_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	char header_value_identifier[ INFO_HANDLE_VALUE_IDENTIFIER_SIZE ];

	const char *description             = NULL;
	static char *function               = "info_handle_header_values_fprint";
	size_t description_length           = 0;
	size_t header_value_identifier_size = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;
	uint32_t header_value_index         = 0;
	uint32_t number_of_values           = 0;
	int result                          = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_set_header_values_date_format(
	     info_handle->input_handle,
	     info_handle->date_format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header values date format.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_header_values(
	     info_handle->input_handle,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of header values.",
		 function );

		return( -1 );
	}
	if( info_handle_section_header_fprint(
	     info_handle,
	     "acquiry_information",
	     "Acquiry information",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section header: acquiry_information.",
		 function );

		result = -1;
	}
	if( number_of_values == 0 )
	{
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tNo information found in file.\n" );
		}
	}
	else
	{
		for( header_value_index = 0;
		     header_value_index < number_of_values;
		     header_value_index++ )
		{
			if( libewf_handle_get_header_value_identifier_size(
			     info_handle->input_handle,
			     header_value_index,
			     &header_value_identifier_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the header identifier size for index: %" PRIu32 ".",
				 function,
				 header_value_index );

				result = -1;

				continue;
			}
			if( header_value_identifier_size > INFO_HANDLE_VALUE_IDENTIFIER_SIZE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: header identifier size value out of bounds for index: %" PRIu32 ".",
				 function,
				 header_value_index );

				result = -1;

				continue;
			}
			if( libewf_handle_get_header_value_identifier(
			     info_handle->input_handle,
			     header_value_index,
			     (uint8_t *) header_value_identifier,
			     header_value_identifier_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the header identifier for index: %" PRIu32 ".",
				 function,
				 header_value_index );

				result = -1;

				continue;
			}
			description        = NULL;
			description_length = 0;

			if( header_value_identifier_size == 6 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "model",
				     5 ) == 0 )
				{
					description        = "Model";
					description_length = 5;
				}
				else if( narrow_string_compare(
					  header_value_identifier,
					  "notes",
					  5 ) == 0 )
				{
					description        = "Notes";
					description_length = 5;
				}
			}
			else if( header_value_identifier_size == 11 )
			{
				/* TODO figure out what this value represents
				 */
				if( narrow_string_compare(
				     header_value_identifier,
				     "unknown_dc",
				     10 ) == 0 )
				{
					description        = "Unknown value dc";
					description_length = 16;
				}
			}
			else if( header_value_identifier_size == 12 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "case_number",
				     11 ) == 0 )
				{
					description        = "Case number";
					description_length = 11;
				}
				else if( narrow_string_compare(
					  header_value_identifier,
					  "description",
					  11 ) == 0 )
				{
					description        = "Description";
					description_length = 11;
				}
				else if( narrow_string_compare(
					  header_value_identifier,
					  "system_date",
					  11 ) == 0 )
				{
					description        = "System date";
					description_length = 11;
				}
			}
			else if( header_value_identifier_size == 13 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "acquiry_date",
				     12 ) == 0 )
				{
					description        = "Acquisition date";
					description_length = 16;
				}
				else if( narrow_string_compare(
				          header_value_identifier,
				          "device_label",
				          12 ) == 0 )
				{
					description        = "Device label";
					description_length = 12;
				}
			}
			else if( header_value_identifier_size == 14 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "examiner_name",
				     13 ) == 0 )
				{
					description        = "Examiner name";
					description_length = 13;
				}
				else if( narrow_string_compare(
					  header_value_identifier,
					  "serial_number",
					  13 ) == 0 )
				{
					description        = "Serial number";
					description_length = 13;
				}
			}
			else if( header_value_identifier_size == 16 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "evidence_number",
				     15 ) == 0 )
				{
					description        = "Evidence number";
					description_length = 15;
				}
			}
			else if( header_value_identifier_size == 17 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "acquiry_software",
				     16 ) == 0 )
				{
					description        = "Software used";
					description_length = 13;
				}
			}
			else if( header_value_identifier_size == 19 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "process_identifier",
				     18 ) == 0 )
				{
					description        = "Process identifier";
					description_length = 18;
				}
			}
			else if( header_value_identifier_size == 25 )
			{
				if( narrow_string_compare(
				     header_value_identifier,
				     "acquiry_operating_system",
				     24 ) == 0 )
				{
					description        = "Operating system used";
					description_length = 21;
				}
				else if( narrow_string_compare(
					  header_value_identifier,
					  "acquiry_software_version",
					  24 ) == 0 )
				{
					description        = "Software version used";
					description_length = 21;
				}
			}
			if( description == NULL )
			{
				if( header_value_identifier_size == 8 )
				{
					if( narrow_string_compare(
					     header_value_identifier,
					     "extents",
					     7 ) == 0 )
					{
						if( info_handle_header_value_extents_fprint(
						     info_handle,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
							 "%s: unable to print header value: extents.",
							 function );

							result = -1;
						}
					}
				}
				else if( header_value_identifier_size == 9 )
				{
					if( narrow_string_compare(
					     header_value_identifier,
					     "password",
					     8 ) == 0 )
					{
						if( info_handle_header_value_password_fprint(
						     info_handle,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
							 "%s: unable to print header value: password.",
							 function );

							result = -1;
						}
					}
				}
				else if( header_value_identifier_size == 18 )
				{
					if( narrow_string_compare(
					     header_value_identifier,
					     "compression_level",
					     17 ) == 0 )
					{
						if( info_handle_header_value_compression_level_fprint(
						     info_handle,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
							 "%s: unable to print header value: compression_level.",
							 function );

							result = -1;
						}
					}
				}
				else
				{
					description        = header_value_identifier;
					description_length = header_value_identifier_size - 1;
				}
			}
			if( description != NULL )
			{
				if( info_handle_header_value_fprint(
				     info_handle,
				     header_value_identifier,
				     header_value_identifier_size - 1,
				     description,
				     description_length,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print header value: %s.",
					 function,
					 header_value_identifier );

					result = -1;
				}
			}
		}
	}
	if( info_handle_section_footer_fprint(
	     info_handle,
	     "acquiry_information",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section footer: acquiry_information.",
		 function );

		result = -1;
	}
	return( result );
}

/* Prints the password header value
 * Returns 1 if successful or -1 on error
 */
int info_handle_header_value_password_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t header_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function    = "info_handle_header_value_password_fprint";
	size_t header_value_size = INFO_HANDLE_VALUE_SIZE;
	int result               = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_header_value(
	          info_handle->input_handle,
	          (uint8_t *) "password",
	          8,
	          (uint16_t *) header_value,
	          header_value_size,
	          error );
#else
	result = libewf_handle_get_utf8_header_value(
	          info_handle->input_handle,
	          (uint8_t *) "password",
	          8,
	          (uint8_t *) header_value,
	          header_value_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: password.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tPassword:\t\tN/A\n" );
		}
	}
	else
	{
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t\t\t<password>%" PRIs_SYSTEM "</password>\n",
			 header_value );
		}
		else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tPassword:\t\t(hash: %" PRIs_SYSTEM ")\n",
			 header_value );
		}
	}
	return( 1 );
}

/* Prints the compression level header value
 * Returns 1 if successful or -1 on error
 */
int info_handle_header_value_compression_level_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t header_value[ INFO_HANDLE_VALUE_SIZE ];

	const system_character_t *value_string = NULL;
	static char *function                  = "info_handle_header_value_compression_level_fprint";
	size_t header_value_size               = INFO_HANDLE_VALUE_SIZE;
	int result                             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_header_value(
	          info_handle->input_handle,
	          (uint8_t *) "compression_level",
	          16,
	          (uint16_t *) header_value,
	          header_value_size,
	          error );
#else
	result = libewf_handle_get_utf8_header_value(
	          info_handle->input_handle,
	          (uint8_t *) "compression_level",
	          16,
	          (uint8_t *) header_value,
	          header_value_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: compression_level.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( system_string_compare(
		     header_value,
		     _SYSTEM_STRING( LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_NONE ),
		     1 ) == 0 )
		{
			value_string = _SYSTEM_STRING( "no compression" );
		}
		else if( system_string_compare(
			  header_value,
			  _SYSTEM_STRING( LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_FAST ),
			  1 ) == 0 )
		{
			value_string = _SYSTEM_STRING( "good (fast) compression" );
		}
		else if( system_string_compare(
			  header_value,
			  _SYSTEM_STRING( LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_BEST ),
			  1 ) == 0 )
		{
			value_string = _SYSTEM_STRING( "best compression" );
		}
		else
		{
			value_string = _SYSTEM_STRING( "unknown compression" );
		}
		if( info_handle_section_value_string_fprint(
		     info_handle,
		     "compression_level",
		     16,
		     "Compression level",
		     16,
		     value_string,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section value string: compression_level.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Prints the extents header value
 * Returns 1 if successful or -1 on error
 */
int info_handle_header_value_extents_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t header_value[ INFO_HANDLE_VALUE_SIZE ];

	system_character_t *string_segment                = NULL;
	static char *function                             = "info_handle_header_value_extents_fprint";
	size_t header_value_length                        = 0;
	size_t header_value_size                          = INFO_HANDLE_VALUE_SIZE;
	size_t string_segment_size                        = 0;
	int number_of_segments                            = 0;
	int result                                        = 0;
	int segment_index                                 = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	libcsplit_wide_split_string_t *extents_elements   = NULL;
#else
	libcsplit_narrow_split_string_t *extents_elements = NULL;
#endif

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_header_value(
	          info_handle->input_handle,
	          (uint8_t *) "extents",
	          7,
	          (uint16_t *) header_value,
	          header_value_size,
	          error );
#else
	result = libewf_handle_get_utf8_header_value(
	          info_handle->input_handle,
	          (uint8_t *) "extents",
	          7,
	          (uint8_t *) header_value,
	          header_value_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: extents.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		/* Need the effective length of the string
		 */
		header_value_length = system_string_length(
		                       header_value );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_string_split(
		     header_value,
		     header_value_length + 1,
		     (wchar_t) ' ',
		     &extents_elements,
		     error ) != 1 )
#else
		if( libcsplit_narrow_string_split(
		     header_value,
		     header_value_length + 1,
		     (char) ' ',
		     &extents_elements,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_split_string_get_number_of_segments(
		     extents_elements,
		     &number_of_segments,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_get_number_of_segments(
		     extents_elements,
		     &number_of_segments,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of segments.",
			 function );

			return( -1 );
		}
		if( ( number_of_segments % 4 ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported number of segments.",
			 function );

			goto on_error;
		}
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libcsplit_wide_split_string_get_segment_by_index(
			     extents_elements,
			     0,
			     &string_segment,
			     &string_segment_size,
			     error ) != 1 )
#else
			if( libcsplit_narrow_split_string_get_segment_by_index(
			     extents_elements,
			     0,
			     &string_segment,
			     &string_segment_size,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string segment: 0.",
				 function );

				goto on_error;
			}
			if( string_segment != NULL )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\tExtents:\t\t%" PRIs_SYSTEM "\n",
				 string_segment );
			}
		}
		if( number_of_segments > 1 )
		{
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\t\t\t<extents>\n" );
			}
			for( segment_index = 1;
			     segment_index < number_of_segments;
			     segment_index++ )
			{
				if( ( segment_index % 4 ) == 1 )
				{
					fprintf(
					 info_handle->notify_stream,
					 "\t\t\t\t" );

					if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
					{
						fprintf(
						 info_handle->notify_stream,
						 "<extent>" );
					}
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				if( libcsplit_wide_split_string_get_segment_by_index(
				     extents_elements,
				     segment_index,
				     &string_segment,
				     &string_segment_size,
				     error ) != 1 )
#else
				if( libcsplit_narrow_split_string_get_segment_by_index(
				     extents_elements,
				     segment_index,
				     &string_segment,
				     &string_segment_size,
				     error ) != 1 )
#endif
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve string segment: %d.",
					 function,
					 segment_index );

					goto on_error;
				}
				if( string_segment != NULL )
				{
					fprintf(
					 info_handle->notify_stream,
					 "%" PRIs_SYSTEM "",
					 string_segment );
				}
				if( ( segment_index % 4 ) != 0 )
				{
					fprintf(
					 info_handle->notify_stream,
					 " " );
				}
				else
				{
					if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
					{
						fprintf(
						 info_handle->notify_stream,
						 "</extent>" );
					}
					fprintf(
					 info_handle->notify_stream,
					 "\n" );
				}
			}
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\t\t\t</extents>\n" );
			}
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_split_string_free(
		     &extents_elements,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_free(
		     &extents_elements,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split string.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( extents_elements != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		libcsplit_wide_split_string_free(
		 &extents_elements,
		 NULL );
#else
		libcsplit_narrow_split_string_free(
		 &extents_elements,
		 NULL );
#endif
	}
	return( -1 );
}

/* Prints the media information
 * Returns 1 if successful or -1 on error
 */
int info_handle_media_information_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
        system_character_t guid_string[ 48 ];
        uint8_t guid[ GUID_SIZE ];

	system_character_t segment_file_version[ 4 ] = { '0', '.', '0', 0 };

	const system_character_t *value_string       = NULL;
	static char *function                        = "info_handle_media_information_fprint";
	size64_t media_size                          = 0;
	uint64_t value_64bit                         = 0;
	uint32_t value_32bit                         = 0;
	uint16_t compression_method                  = 0;
	uint8_t compression_flags                    = 0;
	uint8_t format                               = 0;
	uint8_t major_version                        = 0;
	uint8_t media_flags                          = 0;
	uint8_t media_type                           = 0;
	uint8_t minor_version                        = 0;
	int8_t compression_level                     = 0;
	int is_corrupted                             = 0;
	int is_encrypted                             = 0;
	int result                                   = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	is_encrypted = libewf_handle_segment_files_encrypted(
	                info_handle->input_handle,
	                error );

	if( is_encrypted == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if segment files are encrypted.",
		 function );

		result = -1;
	}
	if( info_handle_section_header_fprint(
	     info_handle,
	     "ewf_information",
	     "EWF information",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section header: ewf_information.",
		 function );

		result = -1;
	}
	if( libewf_handle_get_format(
	     info_handle->input_handle,
	     &format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format.",
		 function );

		return( -1 );
	}
	switch( format )
	{
		case LIBEWF_FORMAT_EWF:
			value_string = _SYSTEM_STRING( "original EWF" );
			break;

		case LIBEWF_FORMAT_SMART:
			value_string = _SYSTEM_STRING( "SMART" );
			break;

		case LIBEWF_FORMAT_FTK_IMAGER:
			value_string = _SYSTEM_STRING( "FTK Imager" );
			break;

		case LIBEWF_FORMAT_ENCASE1:
			value_string = _SYSTEM_STRING( "EnCase 1" );
			break;

		case LIBEWF_FORMAT_ENCASE2:
			value_string = _SYSTEM_STRING( "EnCase 2" );
			break;

		case LIBEWF_FORMAT_ENCASE3:
			value_string = _SYSTEM_STRING( "EnCase 3" );
			break;

		case LIBEWF_FORMAT_ENCASE4:
			value_string = _SYSTEM_STRING( "EnCase 4" );
			break;

		case LIBEWF_FORMAT_ENCASE5:
			value_string = _SYSTEM_STRING( "EnCase 5" );
			break;

		case LIBEWF_FORMAT_ENCASE6:
			value_string = _SYSTEM_STRING( "EnCase 6" );
			break;

		case LIBEWF_FORMAT_ENCASE7:
			value_string = _SYSTEM_STRING( "EnCase 7" );
			break;

		case LIBEWF_FORMAT_LINEN5:
			value_string = _SYSTEM_STRING( "linen 5" );
			break;

		case LIBEWF_FORMAT_LINEN6:
			value_string = _SYSTEM_STRING( "linen 6" );
			break;

		case LIBEWF_FORMAT_LINEN7:
			value_string = _SYSTEM_STRING( "linen 7" );
			break;

		case LIBEWF_FORMAT_EWFX:
			value_string = _SYSTEM_STRING( "EWFX (extended EWF)" );
			break;

		case LIBEWF_FORMAT_LOGICAL_ENCASE5:
			value_string = _SYSTEM_STRING( "Logical Evidence File (LEF) EnCase 5" );
			break;

		case LIBEWF_FORMAT_LOGICAL_ENCASE6:
			value_string = _SYSTEM_STRING( "Logical Evidence File (LEF) EnCase 6" );
			break;

		case LIBEWF_FORMAT_LOGICAL_ENCASE7:
			value_string = _SYSTEM_STRING( "Logical Evidence File (LEF) EnCase 7" );
			break;

		case LIBEWF_FORMAT_V2_ENCASE7:
			value_string = _SYSTEM_STRING( "EnCase 7 (version 2)" );
			break;

		case LIBEWF_FORMAT_V2_LOGICAL_ENCASE7:
			value_string = _SYSTEM_STRING( "Logical Evidence File (LEF) EnCase 7 (version 2)" );
			break;

		case LIBEWF_FORMAT_UNKNOWN:
		default:
			value_string = _SYSTEM_STRING( "unknown" );
			break;

	}
	if( info_handle_section_value_string_fprint(
	     info_handle,
	     "file_format",
	     11,
	     "File format",
	     11,
	     value_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section value string: file_format.",
		 function );

		result = -1;
	}
	if( ( format == LIBEWF_FORMAT_V2_ENCASE7 )
	 || ( format == LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 ) )
	{
		if( libewf_handle_get_segment_file_version(
		     info_handle->input_handle,
		     &major_version,
		     &minor_version,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment file version.",
			 function );

			result = -1;
		}
		else
		{
/* TODO improve this */
			if( major_version <= 9 )
			{
				segment_file_version[ 0 ] += major_version;
			}
			if( minor_version <= 9 )
			{
				segment_file_version[ 2 ] += minor_version;
			}
			if( info_handle_section_value_string_fprint(
			     info_handle,
			     "segment_file_version",
			     20,
			     "Segment file version",
			     20,
			     segment_file_version,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section value string: segment_file_version.",
				 function );

				result = -1;
			}
		}
	}
	if( is_encrypted == 0 )
	{
		if( libewf_handle_get_sectors_per_chunk(
		     info_handle->input_handle,
		     &value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sectors per chunk.",
			 function );

			result = -1;
		}
		else
		{
			if( info_handle_section_value_32bit_fprint(
			     info_handle,
			     "sectors_per_chunk",
			     "Sectors per chunk",
			     17,
			     value_32bit,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section 32-bit value: sectors_per_chunk.",
				 function );

				result = -1;
			}
		}
		if( libewf_handle_get_error_granularity(
		     info_handle->input_handle,
		     &value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve error granularity.",
			 function );

			result = -1;
		}
		else if( value_32bit != 0 )
		{
			if( info_handle_section_value_32bit_fprint(
			     info_handle,
			     "error_granularity",
			     "Error granularity",
			     17,
			     value_32bit,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section 32-bit value: error_granularity.",
				 function );

				result = -1;
			}
		}
	}
	if( libewf_handle_get_compression_method(
	     info_handle->input_handle,
	     &compression_method,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression method.",
		 function );

		result = -1;
	}
	else
	{
		if( compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
		{
			value_string = _SYSTEM_STRING( "deflate" );
		}
		else if( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
		{
			value_string = _SYSTEM_STRING( "bzip2" );
		}
		if( info_handle_section_value_string_fprint(
		     info_handle,
		     "compression_method",
		     18,
		     "Compression method",
		     18,
		     value_string,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section value string: compression_method.",
			 function );

			result = -1;
		}
	}
	if( is_encrypted == 0 )
	{
		if( libewf_handle_get_compression_values(
		     info_handle->input_handle,
		     &compression_level,
		     &compression_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve compression values.",
			 function );

			result = -1;
		}
		else
		{
			if( compression_level == LIBEWF_COMPRESSION_LEVEL_NONE )
			{
				value_string = _SYSTEM_STRING( "no compression" );
			}
			else if( compression_level == LIBEWF_COMPRESSION_LEVEL_FAST )
			{
				value_string = _SYSTEM_STRING( "good (fast) compression" );
			}
			else if( compression_level == LIBEWF_COMPRESSION_LEVEL_BEST )
			{
				value_string = _SYSTEM_STRING( "best compression" );
			}
			else
			{
				value_string = _SYSTEM_STRING( "unknown compression" );
			}
			if( info_handle_section_value_string_fprint(
			     info_handle,
			     "compression_level",
			     17,
			     "Compression level",
			     17,
			     value_string,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section value string: compression_level.",
				 function );

				result = -1;
			}
		}
	}
	if( libewf_handle_get_segment_file_set_identifier(
	     info_handle->input_handle,
	     guid,
	     GUID_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment file set identifier.",
		 function );

		result = -1;
	}
	else if( ( guid[ 0 ] != 0 )
	      || ( guid[ 1 ] != 0 )
	      || ( guid[ 2 ] != 0 )
	      || ( guid[ 3 ] != 0 )
	      || ( guid[ 4 ] != 0 )
	      || ( guid[ 5 ] != 0 )
	      || ( guid[ 6 ] != 0 )
	      || ( guid[ 7 ] != 0 )
	      || ( guid[ 8 ] != 0 )
	      || ( guid[ 9 ] != 0 )
	      || ( guid[ 10 ] != 0 )
	      || ( guid[ 11 ] != 0 )
	      || ( guid[ 12 ] != 0 )
	      || ( guid[ 13 ] != 0 )
	      || ( guid[ 14 ] != 0 )
	      || ( guid[ 15 ] != 0 ) )
	{
		if( guid_to_string(
		     guid,
		     GUID_SIZE,
		     _BYTE_STREAM_ENDIAN_LITTLE,
		     guid_string,
		     48,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create GUID string.",
			 function );

			result = -1;
		}
		else
		{
			if( info_handle_section_value_string_fprint(
			     info_handle,
			     "set_identifier",
			     14,
			     "Set identifier",
			     14,
			     guid_string,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section value string: set_identifier.",
				 function );

				result = -1;
			}
		}
	}
	is_corrupted = libewf_handle_segment_files_corrupted(
	                info_handle->input_handle,
	                error );

	if( is_corrupted == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if segment files are corrupted.",
		 function );

		result = -1;
	}
	else if( is_corrupted != 0 )
	{
		if( info_handle_section_value_boolean_fprint(
		     info_handle,
		     "is_corrupted",
		     "Is corrupted",
		     12,
		     is_corrupted,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section boolean value: is_corrupted.",
			 function );

			result = -1;
		}
	}
	if( is_encrypted != 0 )
	{
		if( info_handle_section_value_boolean_fprint(
		     info_handle,
		     "is_encrypted",
		     "Is encrypted",
		     12,
		     is_encrypted,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section boolean value: is_encrypted.",
			 function );

			result = -1;
		}
	}
	if( info_handle_section_footer_fprint(
	     info_handle,
	     "ewf_information",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section footer: ewf_information.",
		 function );

		result = -1;
	}
/* TODO improved this check once encryption support has been implemented */
	if( is_encrypted != 0 )
	{
		return( result );
	}
	if( info_handle_section_header_fprint(
	     info_handle,
	     "media_information",
	     "Media information",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section header: media_information.",
		 function );

		result = -1;
	}
	if( ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_SMART ) )
	{
		if( libewf_handle_get_media_type(
		     info_handle->input_handle,
		     &media_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media type.",
			 function );

			result = -1;
		}
		else
		{
			if( media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
			{
				value_string = _SYSTEM_STRING( "removable disk" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_FIXED )
			{
				value_string = _SYSTEM_STRING( "fixed disk" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_SINGLE_FILES )
			{
				value_string = _SYSTEM_STRING( "single files" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
			{
				value_string = _SYSTEM_STRING( "optical disk (CD/DVD/BD)" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_MEMORY )
			{
				value_string = _SYSTEM_STRING( "memory (RAM)" );
			}
			else
			{
				value_string = _SYSTEM_STRING( "unknown" );
			}
			if( info_handle_section_value_string_fprint(
			     info_handle,
			     "media_type",
			     10,
			     "Media type",
			     10,
			     value_string,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section value string: media_type.",
				 function );

				result = -1;
			}
		}
		if( libewf_handle_get_media_flags(
		     info_handle->input_handle,
		     &media_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media flags.",
			 function );

			result = -1;
		}
		else
		{
/* TODO remove conditional compile after tests are fixed */
#if defined( HAVE_DEBUG_OUTPUT ) && defined( TODO )
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\tMedia flags:\t\t0x%02" PRIx8 "\n",
				 media_flags );
			}
#endif
			if( info_handle_section_value_boolean_fprint(
			     info_handle,
			     "is_physical",
			     "Is physical",
			     10,
			     media_flags & LIBEWF_MEDIA_FLAG_PHYSICAL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section boolean value: is_physical.",
				 function );

				result = -1;
			}
			if( ( media_flags & LIBEWF_MEDIA_FLAG_FASTBLOC ) != 0 )
			{
				if( info_handle_section_value_string_fprint(
				     info_handle,
				     "write_blocked",
				     13,
				     "Write blocked",
				     13,
				     _SYSTEM_STRING( "Fastbloc" ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print section value string: write_blocked.",
					 function );

					result = -1;
				}
			}
			if( ( media_flags & LIBEWF_MEDIA_FLAG_TABLEAU ) != 0 )
			{
				if( info_handle_section_value_string_fprint(
				     info_handle,
				     "write_blocked",
				     13,
				     "Write blocked",
				     13,
				     _SYSTEM_STRING( "Tableau" ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print section value string: write_blocked.",
					 function );

					result = -1;
				}
			}
		}
	}
	if( format != LIBEWF_FORMAT_LOGICAL_ENCASE5 )
	{
		if( libewf_handle_get_bytes_per_sector(
		     info_handle->input_handle,
		     &value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bytes per sector.",
			 function );

			result = -1;
		}
		else
		{
			if( info_handle_section_value_32bit_fprint(
			     info_handle,
			     "bytes_per_sector",
			     "Bytes per sector",
			     16,
			     value_32bit,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section 32-bit value: bytes_per_sector.",
				 function );

				result = -1;
			}
		}
		if( libewf_handle_get_number_of_sectors(
		     info_handle->input_handle,
		     &value_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sectors.",
			 function );

			result = -1;
		}
		else
		{
			if( info_handle_section_value_64bit_fprint(
			     info_handle,
			     "number_of_sectors",
			     "Number of sectors",
			     17,
			     value_64bit,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section 64-bit value: number_of_sectors.",
				 function );

				result = -1;
			}
		}
/* TODO remove conditional compile after tests are fixed */
#if defined( HAVE_DEBUG_OUTPUT ) && defined( TODO )
		if( libewf_handle_get_chunk_size(
		     info_handle->input_handle,
		     &value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve chunk size.",
			 function );

			result = -1;
		}
		else
		{
			if( info_handle_section_value_32bit_fprint(
			     info_handle,
			     "chunk_size",
			     "Chunk size",
			     10,
			     value_32bit,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print section 32-bit value: number_of_sectors.",
				 function );

				result = -1;
			}
		}
#endif
	}
	if( libewf_handle_get_media_size(
	     info_handle->input_handle,
	     &media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		result = -1;
	}
	else
	{
		if( info_handle_section_value_size_fprint(
		     info_handle,
		     "media_size",
		     "Media size",
		     10,
		     media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section 64-bit value: media_size.",
			 function );

			result = -1;
		}
	}
	if( info_handle_section_footer_fprint(
	     info_handle,
	     "media_information",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section footer: media_information.",
		 function );

		result = -1;
	}
	return( result );
}

/* Prints a hash value
 * Returns 1 if successful or -1 on error
 */
int info_handle_hash_value_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     size_t identifier_length,
     libcerror_error_t **error )
{
	system_character_t hash_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function  = "info_handle_hash_value_fprint";
	size_t hash_value_size = INFO_HANDLE_VALUE_SIZE;
	int result             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_utf16_hash_value(
	          info_handle->input_handle,
	          (uint8_t *) identifier,
	          identifier_length,
	          (uint16_t *) hash_value,
	          hash_value_size,
	          error );
#else
	result = libewf_handle_get_utf8_hash_value(
	          info_handle->input_handle,
	          (uint8_t *) identifier,
	          identifier_length,
	          (uint8_t *) hash_value,
	          hash_value_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: %s.",
		 function,
		 identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
		{
			if( identifier_length == 3 )
			{
				if( narrow_string_compare(
				     identifier,
				     "MD5",
				     3 ) == 0 )
				{
					identifier = "md5";
				}
			}
			else if( identifier_length == 4 )
			{
				if( narrow_string_compare(
				     identifier,
				     "SHA1",
				     4 ) == 0 )
				{
					identifier = "sha1";
				}
			}
			fprintf(
			 info_handle->notify_stream,
			 "\t\t<hashdigest type=\"%s\" coding=\"base16\">%" PRIs_SYSTEM "</hashdigest>\n",
			 identifier,
			 hash_value );
		}
		else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t%s:\t\t\t%" PRIs_SYSTEM "\n",
			 identifier,
			 hash_value );
		}
	}
	return( 1 );
}

/* Prints the hash values
 * Returns 1 if successful or -1 on error
 */
int info_handle_hash_values_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	char hash_value_identifier[ INFO_HANDLE_VALUE_IDENTIFIER_SIZE ];

	static char *function                      = "info_handle_hash_values_fprint";
	size_t hash_value_identifier_size          = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;
	uint32_t hash_value_iterator               = 0;
	uint32_t number_of_values                  = 0;
	uint8_t print_section_header               = 1;
	int result                                 = 1;

#if defined( USE_LIBEWF_GET_MD5_HASH )
	digest_hash_t md5_hash[ DIGEST_HASH_SIZE_MD5 ];

	system_character_t *stored_md5_hash_string = NULL;
#endif

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( USE_LIBEWF_GET_MD5_HASH )
	result = libewf_handle_get_md5_hash(
		  handle,
		  md5_hash,
		  DIGEST_HASH_SIZE_MD5,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MD5 hash.",
		 function );

		return;
	}
	else if( result == 1 )
	{
		stored_md5_hash_string = system_string_allocate(
		                          DIGEST_HASH_STRING_SIZE_MD5 );

		if( stored_md5_hash_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create MD5 hash string.",
			 function );

			return;
		}
		if( digest_hash_copy_to_string(
		     md5_hash,
		     DIGEST_HASH_SIZE_MD5,
		     stored_md5_hash_string,
		     DIGEST_HASH_STRING_SIZE_MD5,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set MD5 hash string.",
			 function );

			memory_free(
			 stored_md5_hash_string );

			return;
		}
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			if( print_section_header != 0 )
			{
				if( info_handle_section_header_fprint(
				     info_handle,
				     "digest_hash_information",
				     "Digest hash information",
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print section header: digest_hash_information.",
					 function );

					result = -1;
				}
				print_section_header = 0;
			}
		}
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t\t<hashdigest type=\"md5\" coding=\"base16\">%" PRIs_SYSTEM "</hashdigest>\n",
			 stored_md5_hash_string );
		}
		else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tMD5:\t\t\t%" PRIs_SYSTEM "\n",
			 stored_md5_hash_string );
		}
		memory_free(
		 stored_md5_hash_string );
	}
#endif
	if( libewf_handle_get_number_of_hash_values(
	     info_handle->input_handle,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of hash values.",
		 function );

		return( -1 );
	}
	for( hash_value_iterator = 0;
	     hash_value_iterator < number_of_values;
	     hash_value_iterator++ )
	{
		if( libewf_handle_get_hash_value_identifier_size(
		     info_handle->input_handle,
		     hash_value_iterator,
		     &hash_value_identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value identifier size for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
		if( hash_value_identifier_size > INFO_HANDLE_VALUE_IDENTIFIER_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: hash value identifier size value out of bounds for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
		if( libewf_handle_get_hash_value_identifier(
		     info_handle->input_handle,
		     hash_value_iterator,
		     (uint8_t *) hash_value_identifier,
		     hash_value_identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value identifier for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
#if defined( USE_LIBEWF_GET_MD5_HASH )
		if( hash_value_identifier_size == 4 )
		{
			if( narrow_string_compare(
			     hash_value_identifier,
			     "MD5",
			     3 ) == 0 )
			{
				continue;
			}
		}
#endif
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			if( print_section_header != 0 )
			{
				if( info_handle_section_header_fprint(
				     info_handle,
				     "digest_hash_information",
				     "Digest hash information",
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print section header: digest_hash_information.",
					 function );

					result = -1;
				}
				print_section_header = 0;
			}
		}
		if( info_handle_hash_value_fprint(
		     info_handle,
		     hash_value_identifier,
		     hash_value_identifier_size - 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print hash value: %s.",
			 function,
			 hash_value_identifier );

			result = -1;
		}
	}
	if( print_section_header == 0 )
	{
		if( info_handle_section_footer_fprint(
		     info_handle,
		     "digest_hash_information.",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section footer: digest_hash_information.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Prints the acquiry errors
 * Returns 1 if successful or -1 on error
 */
int info_handle_acquiry_errors_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function      = "info_handle_acquiry_errors_fprint";
	uint64_t last_sector       = 0;
	uint64_t number_of_sectors = 0;
	uint64_t start_sector      = 0;
	uint32_t bytes_per_sector  = 0;
	uint32_t number_of_errors  = 0;
	uint32_t error_iterator    = 0;
	int result                 = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_bytes_per_sector(
	     info_handle->input_handle,
	     &bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_acquiry_errors(
	     info_handle->input_handle,
	     &number_of_errors,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of acquiry errors.",
		 function );

		return( -1 );
	}
	if( number_of_errors > 0 )
	{
		if( info_handle_section_header_fprint(
		     info_handle,
		     "acquisition_read_errors",
		     "Read errors during acquiry",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section header: acquisition_read_errors.",
			 function );

			result = -1;
		}
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\ttotal number: %" PRIu32 "\n",
			 number_of_errors );
		}
		for( error_iterator = 0;
		     error_iterator < number_of_errors;
		     error_iterator++ )
		{
			if( libewf_handle_get_acquiry_error(
			     info_handle->input_handle,
			     error_iterator,
			     &start_sector,
			     &number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the acquiry error: %" PRIu32 ".",
				 function,
				 error_iterator );

				start_sector      = 0;
				number_of_sectors = 0;

				result = -1;
			}
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\t\t\t<run image_offset=\"%" PRIu64 "\" len=\"%" PRIu64 "\"/>\n",
				 start_sector * bytes_per_sector,
				 number_of_sectors * bytes_per_sector );
			}
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
			{
				last_sector = start_sector + number_of_sectors;

				if( number_of_sectors != 0 )
				{
					last_sector -= 1;
				}
				fprintf(
				 info_handle->notify_stream,
				 "\tat sector(s): %" PRIu64 " - %" PRIu64 " (number: %" PRIu64 ")\n",
				 start_sector,
				 last_sector,
				 number_of_sectors );
			}
		}
		if( info_handle_section_footer_fprint(
		     info_handle,
		     "acquisition_read_errors",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section footer: acquisition_read_errors.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Prints the sessions
 * Returns 1 if successful or -1 on error
 */
int info_handle_sessions_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function       = "info_handle_sessions_fprint";
	uint64_t last_sector        = 0;
	uint64_t number_of_sectors  = 0;
	uint64_t start_sector       = 0;
	uint32_t bytes_per_sector   = 0;
	uint32_t number_of_sessions = 0;
	uint32_t session_iterator   = 0;
	int result                  = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_bytes_per_sector(
	     info_handle->input_handle,
	     &bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_sessions(
	     info_handle->input_handle,
	     &number_of_sessions,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of sessions.",
		 function );

		return( -1 );
	}
	if( number_of_sessions > 0 )
	{
		if( info_handle_section_header_fprint(
		     info_handle,
		     "sessions",
		     "Sessions",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section header: sessions.",
			 function );

			result = -1;
		}
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\ttotal number: %" PRIu32 "\n",
			 number_of_sessions );
		}
		for( session_iterator = 0;
		     session_iterator < number_of_sessions;
		     session_iterator++ )
		{
			if( libewf_handle_get_session(
			     info_handle->input_handle,
			     session_iterator,
			     &start_sector,
			     &number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the session: %" PRIu32 ".",
				 function,
				 session_iterator );

				start_sector      = 0;
				number_of_sectors = 0;

				result = -1;
			}
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\t\t\t<run image_offset=\"%" PRIu64 "\" len=\"%" PRIu64 "\"/>\n",
				 start_sector * bytes_per_sector,
				 number_of_sectors * bytes_per_sector );
			}
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
			{
				last_sector = start_sector + number_of_sectors;

				if( number_of_sectors != 0 )
				{
					last_sector -= 1;
				}
				fprintf(
				 info_handle->notify_stream,
				 "\tat sector(s): %" PRIu64 " - %" PRIu64 " (number: %" PRIu64 ")\n",
				 start_sector,
				 last_sector,
				 number_of_sectors );
			}
		}
		if( info_handle_section_footer_fprint(
		     info_handle,
		     "sessions",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section footer: sessions.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Prints the tracks
 * Returns 1 if successful or -1 on error
 */
int info_handle_tracks_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function      = "info_handle_tracks_fprint";
	uint64_t last_sector        = 0;
	uint64_t number_of_sectors = 0;
	uint64_t start_sector      = 0;
	uint32_t bytes_per_sector  = 0;
	uint32_t number_of_tracks  = 0;
	uint32_t track_iterator    = 0;
	int result                 = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_bytes_per_sector(
	     info_handle->input_handle,
	     &bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_number_of_tracks(
	     info_handle->input_handle,
	     &number_of_tracks,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of tracks.",
		 function );

		return( -1 );
	}
	if( number_of_tracks > 0 )
	{
		if( info_handle_section_header_fprint(
		     info_handle,
		     "tracks",
		     "Tracks",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section header: tracks.",
			 function );

			result = -1;
		}
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\ttotal number: %" PRIu32 "\n",
			 number_of_tracks );
		}
		for( track_iterator = 0;
		     track_iterator < number_of_tracks;
		     track_iterator++ )
		{
			if( libewf_handle_get_track(
			     info_handle->input_handle,
			     track_iterator,
			     &start_sector,
			     &number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the track: %" PRIu32 ".",
				 function,
				 track_iterator );

				start_sector      = 0;
				number_of_sectors = 0;

				result = -1;
			}
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\t\t\t<run image_offset=\"%" PRIu64 "\" len=\"%" PRIu64 "\"/>\n",
				 start_sector * bytes_per_sector,
				 number_of_sectors * bytes_per_sector );
			}
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
			{
				last_sector = start_sector + number_of_sectors;

				if( number_of_sectors != 0 )
				{
					last_sector -= 1;
				}
				fprintf(
				 info_handle->notify_stream,
				 "\tat sector(s): %" PRIu64 " - %" PRIu64 " (number: %" PRIu64 ")\n",
				 start_sector,
				 last_sector,
				 number_of_sectors );
			}
		}
		if( info_handle_section_footer_fprint(
		     info_handle,
		     "tracks",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section footer: tracks.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Prints an access control entry value
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_access_control_entry_value_fprint(
     info_handle_t *info_handle,
     libewf_access_control_entry_t *access_control_entry,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_access_control_entry_value_fprint";
	size_t value_string_size         = 0;
	uint32_t property_type           = 0;
	uint32_t value_32bit             = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_access_control_entry_get_type(
	     access_control_entry,
	     &property_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve property type.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\t\t" );

	switch( property_type )
	{
		case 0:
			fprintf(
			 info_handle->notify_stream,
			 "Onwer" );

			break;

		case 1:
			fprintf(
			 info_handle->notify_stream,
			 "Group" );

			break;

		case 6:
			fprintf(
			 info_handle->notify_stream,
			 "Other" );

			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 "Access" );

			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\t\t\t: " );

	switch( property_type )
	{
		case 0:
		case 1:
		case 6:
			break;

		case 2:
			fprintf(
			 info_handle->notify_stream,
			 "Allow " );

			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 "UNKNOWN " );

			break;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_access_control_entry_get_utf16_identifier_size(
	          access_control_entry,
	          &value_string_size,
	          error );
#else
	result = libewf_access_control_entry_get_utf8_identifier_size(
	          access_control_entry,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create identifier string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_access_control_entry_get_utf16_identifier(
		          access_control_entry,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_access_control_entry_get_utf8_identifier(
		          access_control_entry,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_access_control_entry_get_utf16_name_size(
	          access_control_entry,
	          &value_string_size,
	          error );
#else
	result = libewf_access_control_entry_get_utf8_name_size(
	          access_control_entry,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_access_control_entry_get_utf16_name(
		          access_control_entry,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_access_control_entry_get_utf8_name(
		          access_control_entry,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 " (%" PRIs_SYSTEM ")",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	if( ( property_type != 0 )
	 && ( property_type != 1 ) )
	{
		if( libewf_access_control_entry_get_access_mask(
		     access_control_entry,
		     &value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access mask.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 " [0x%08" PRIx32 "]",
		 value_32bit );

		if( property_type == 2 )
		{
			/* fsdrightReadBody or fsdrightListContents
			 */
			if( ( value_32bit & 0x00000001UL ) != 0 )
			{
			}
			/* fsdrightWriteBody or fsdrightCreateItem
			 */
			if( ( value_32bit & 0x00000002UL ) != 0 )
			{
			}
			/* fsdrightAppendMsg or fsdrightCreateContainer
			 */
			if( ( value_32bit & 0x00000004UL ) != 0 )
			{
			}
			/* fsdrightReadProperty or fsdrightReadProperty
			 */
			if( ( value_32bit & 0x00000008UL ) != 0 )
			{
			}
			/* fsdrightWriteProperty or fsdrightWriteProperty
			 */
			if( ( value_32bit & 0x00000010UL ) != 0 )
			{
			}
			/* fsdrightExecute
			 */
			if( ( value_32bit & 0x00000020UL ) != 0 )
			{
			}
			/* Unknown
			 */
			if( ( value_32bit & 0x00000040UL ) != 0 )
			{
			}
			/* fsdrightReadAttributes or fsdrightReadAttributes
			 */
			if( ( value_32bit & 0x00000080UL ) != 0 )
			{
			}
			/* fsdrightWriteAttributes or fsdrightWriteAttributes
			 */
			if( ( value_32bit & 0x00000100UL ) != 0 )
			{
			}
			/* fsdrightWriteOwnProperty or fsdrightWriteOwnProperty
			 */
			if( ( value_32bit & 0x00000200UL ) != 0 )
			{
			}
			/* fsdrightDeleteOwnItem or fsdrightDeleteOwnItem
			 */
			if( ( value_32bit & 0x00000400UL ) != 0 )
			{
			}
			/* fsdrightViewItem or fsdrightViewItem
			 */
			if( ( value_32bit & 0x00000800UL ) != 0 )
			{
			}

			/* fsdrightOwner
			 */
			if( ( value_32bit & 0x00004000UL ) != 0 )
			{
			}
			/* fsdrightContact
			 */
			if( ( value_32bit & 0x00008000UL ) != 0 )
			{
			}

			/* Standard access rights flags */

			/* fsdrightDelete
			 */
			if( ( value_32bit & 0x00010000UL ) != 0 )
			{
			}
			/* fsdrightReadControl
			 */
			if( ( value_32bit & 0x00020000UL ) != 0 )
			{
			}
			/* fsdrightWriteSD
			 */
			if( ( value_32bit & 0x00040000UL ) != 0 )
			{
			}
			/* fsdrightWriteOwner
			 */
			if( ( value_32bit & 0x00080000UL ) != 0 )
			{
			}
			/* fsdrightSynchronize
			 */
			if( ( value_32bit & 0x00100000UL ) != 0 )
			{
			}
			if( value_32bit == 0x001200a9 )
			{
				fprintf(
				 info_handle->notify_stream,
				 " [R&X] [R] [Sync]" );
			}
			else if( value_32bit == 0x001301bf )
			{
				fprintf(
				 info_handle->notify_stream,
				 " [M] [R&X] [R] [W] [Sync]" );
			}
			else if( value_32bit == 0x001f01ff )
			{
				fprintf(
				 info_handle->notify_stream,
				 " [FC] [M] [R&X] [R] [W] [Sync]" );
			}
		}
		else
		{
			if( ( value_32bit & 0x00000001UL ) != 0 )
			{
				fprintf(
				 info_handle->notify_stream,
				 " [Lst Fldr/Rd Data]" );
			}
			if( ( value_32bit & 0x00000002UL ) != 0 )
			{
				fprintf(
				 info_handle->notify_stream,
				 " [Crt Fl/W Data]" );
			}

			if( ( value_32bit & 0x00000020UL ) != 0 )
			{
				fprintf(
				 info_handle->notify_stream,
				 " [Trav Fldr/X Fl]" );
			}
		}
		if( libewf_access_control_entry_get_flags(
		     access_control_entry,
		     &value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve flags.",
			 function );

			goto on_error;
		}
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints a sub section header
 * Returns 1 if successful or -1 on error
 */
int info_handle_sub_section_header_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     const char *description,
     libcerror_error_t **error )
{
	static char *function = "info_handle_sub_section_header_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t\t<%s>\n",
		 identifier );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t%s:\n",
		 description );
	}
	return( 1 );
}

/* Prints a sub section footer
 * Returns 1 if successful or -1 on error
 */
int info_handle_sub_section_footer_fprint(
     info_handle_t *info_handle,
     const char *identifier,
     libcerror_error_t **error )
{
	static char *function = "info_handle_sub_section_footer_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t\t</%s>\n",
		 identifier );
	}
	return( 1 );
}

/* Prints an attribute value
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_attribute_value_fprint(
     info_handle_t *info_handle,
     libewf_attribute_t *attribute,
     libcerror_error_t **error )
{
	system_character_t *name_string  = NULL;
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_attribute_value_fprint";
	size_t name_string_size          = 0;
	size_t value_string_size         = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_attribute_get_utf16_name_size(
	          attribute,
	          &name_string_size,
	          error );
#else
	result = libewf_attribute_get_utf8_name_size(
	          attribute,
	          &name_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( name_string_size > 0 ) )
	{
		name_string = system_string_allocate(
		               name_string_size );

		if( name_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) name_string,
		          name_string_size,
		          error );
#else
		result = libewf_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) name_string,
		          name_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_attribute_get_utf16_value_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libewf_attribute_get_utf8_value_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_attribute_get_utf16_value(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_attribute_get_utf8_value(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value string.",
			 function );

			goto on_error;
		}
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t\t\t<attribute name=\"%" PRIs_SYSTEM "\">%" PRIs_SYSTEM "</attribute>\n",
		 name_string,
		 value_string );
	}
	else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\t\t%s",
		 name_string );

		do
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t" );

			name_string_size += 8;
		}
		while( name_string_size <= 24 );

		fprintf(
		 info_handle->notify_stream,
		 ": %" PRIs_SYSTEM "\n",
		 value_string );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );

		value_string = NULL;
	}
	if( name_string != NULL )
	{
		memory_free(
		 name_string );

		name_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( name_string != NULL )
	{
		memory_free(
		 name_string );
	}
	return( -1 );
}

/* Prints a (acquisition) source value
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_source_value_fprint(
     info_handle_t *info_handle,
     libewf_source_t *source,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_source_value_fprint";
	size64_t size                    = 0;
	size_t value_string_size         = 0;
	int64_t acquisition_time         = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Source:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_name_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_name_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_name(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_name(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tEvidence number\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_evidence_number_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_evidence_number_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve evidence number string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create evidence number string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_evidence_number(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_evidence_number(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve evidence number string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tLocation\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_location_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_location_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve location string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create location string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_location(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_location(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve location string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tDevice GUID\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_device_guid_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_device_guid_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve device GUID string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create device GUID string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_device_guid(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_device_guid(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve device GUID string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tPrimary device GUID\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_primary_device_guid_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_primary_device_guid_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve primary device GUID string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create primary device GUID string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_primary_device_guid(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_primary_device_guid(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve primary device GUID string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tManufacturer\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_manufacturer_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_manufacturer_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve manufacturer string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create manufacturer string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_manufacturer(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_manufacturer(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve manufacturer string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );


	fprintf(
	 info_handle->notify_stream,
	 "\tModel\t\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_model_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_model_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve model string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create model string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_model(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_model(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve model string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tSerial number\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_serial_number_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_serial_number_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve serial number string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create serial number string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_serial_number(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_serial_number(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve serial number string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tDomain\t\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_domain_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_domain_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve domain string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create domain string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_domain(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_domain(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve domain string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tIP address\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_ip_address_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_ip_address_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve IP address string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create IP address string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_ip_address(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_ip_address(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve IP address string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tMAC address\t\t\t: " );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_mac_address_size(
	          source,
	          &value_string_size,
	          error );
#else
	result = libewf_source_get_utf8_mac_address_size(
	          source,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MAC address string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create MAC address string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_source_get_utf16_mac_address(
		          source,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libewf_source_get_utf8_mac_address(
		          source,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MAC address string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

/* TODO print drive type */

	if( libewf_source_get_size(
	     source,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tSize\t\t\t\t: %" PRIu64 "\n",
	 size );

/* TODO print physical offset */
/* TODO print logical offset */

	if( libewf_source_get_acquisition_time(
	     source,
	     &acquisition_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve acquisition time.",
		 function );

		goto on_error;
	}
	if( info_handle_posix_time_value_fprint(
	     info_handle,
	     "\tAcquisition time\t\t",
	     acquisition_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print POSIX time value.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tMD5\t\t\t\t: " );

	value_string = system_string_allocate(
	                33 );

	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MD5 string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_hash_value_md5(
	          source,
	          (uint16_t *) value_string,
	          33,
	          error );
#else
	result = libewf_source_get_utf8_hash_value_md5(
	          source,
	          (uint8_t *) value_string,
	          33,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MD5 string.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );
	}
	memory_free(
	 value_string );

	value_string = NULL;

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tSHA1\t\t\t\t: " );

	value_string = system_string_allocate(
	                41 );

	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create SHA1 string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_source_get_utf16_hash_value_sha1(
	          source,
	          (uint16_t *) value_string,
	          41,
	          error );
#else
	result = libewf_source_get_utf8_hash_value_sha1(
	          source,
	          (uint8_t *) value_string,
	          41,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve SHA1 string.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 value_string );
	}
	memory_free(
	 value_string );

	value_string = NULL;

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints a file entry value
 * Returns 1 if successful, 0 if not or -1 on error
 */
int info_handle_file_entry_value_fprint(
     info_handle_t *info_handle,
     libewf_file_entry_t *file_entry,
     const system_character_t *path,
     libcerror_error_t **error )
{
	char file_mode_string[ 11 ]                         = { '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 0 };

	libewf_access_control_entry_t *access_control_entry = NULL;
	libewf_attribute_t *attribute                       = NULL;
	libewf_source_t *source                             = NULL;
	system_character_t *file_entry_name                 = NULL;
	system_character_t *value_string                    = NULL;
	static char *function                               = "info_handle_file_entry_value_fprint";
	size64_t size                                       = 0;
	size_t file_entry_name_size                         = 0;
	size_t value_string_size                            = 0;
	uint64_t file_identifier                            = 0;
	uint32_t group_identifier                           = 0;
	uint32_t owner_identifier                           = 0;
	int64_t access_time                                 = 0;
	int64_t creation_time                               = 0;
	int64_t deletion_time                               = 0;
	int64_t entry_modification_time                     = 0;
	int64_t modification_time                           = 0;
	int access_control_entry_index                      = 0;
	int attribute_index                                 = 0;
	int number_of_access_control_entries                = 0;
	int number_of_attributes                            = 0;
	int result                                          = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libewf_file_entry_get_identifier(
	     file_entry,
	     &file_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file identifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_file_entry_get_utf16_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libewf_file_entry_get_utf8_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name string size.",
		 function );

		goto on_error;
	}
	if( ( result == 1 )
	 && ( file_entry_name_size > 0 ) )
	{
		file_entry_name = system_string_allocate(
		                   file_entry_name_size );

		if( file_entry_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file entry name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_file_entry_get_utf16_name(
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libewf_file_entry_get_utf8_name(
		          file_entry,
		          (uint8_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry name string.",
			 function );

			goto on_error;
		}
	}
	if( libewf_file_entry_get_creation_time(
	     file_entry,
	     &creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( libewf_file_entry_get_modification_time(
	     file_entry,
	     &modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		goto on_error;
	}
	if( libewf_file_entry_get_access_time(
	     file_entry,
	     &access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		goto on_error;
	}
	if( libewf_file_entry_get_entry_modification_time(
	     file_entry,
	     &entry_modification_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modification time.",
		 function );

		goto on_error;
	}
	if( libewf_file_entry_get_size(
	     file_entry,
	     &size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		goto on_error;
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		/* Colums in a Sleuthkit 3.x and later bodyfile
		 * MD5|name|inode|mode_as_string|UID|GID|size|atime|mtime|ctime|crtime
		 */
		fprintf(
		 info_handle->bodyfile_stream,
		 "0|" );

		if( path != NULL )
		{
			fprintf(
			 info_handle->bodyfile_stream,
			 "%" PRIs_SYSTEM "",
			 path );
		}
		if( file_entry_name != NULL )
		{
			fprintf(
			 info_handle->bodyfile_stream,
			 "%" PRIs_SYSTEM "",
			 file_entry_name );
		}
/* TODO print data stream name */
/* TODO determine mode as string */
/* TODO determine owner and group */
/* TODO determine Sleuthkit metadata address https://wiki.sleuthkit.org/index.php?title=Metadata_Address */

		fprintf(
		 info_handle->bodyfile_stream,
		 "|%" PRIu64 "|%s|%" PRIu32 "|%" PRIu32 "|%" PRIu64 "|%.9f|%.9f|%.9f|%.9f\n",
		 file_identifier,
		 file_mode_string,
		 owner_identifier,
		 group_identifier,
		 size,
		 (double) access_time,
		 (double) modification_time,
		 (double) entry_modification_time,
		 (double) creation_time );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tFile identifier\t\t\t: %" PRIu64 "\n",
		 file_identifier );

		if( file_entry_name != NULL )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\tName\t\t\t\t: " );

			if( path != NULL )
			{
				fprintf(
				 info_handle->notify_stream,
				 "%" PRIs_SYSTEM "",
				 path );
			}
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIs_SYSTEM "\n",
			 file_entry_name );
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_file_entry_get_utf16_short_name_size(
		          file_entry,
		          &value_string_size,
		          error );
#else
		result = libewf_file_entry_get_utf8_short_name_size(
		          file_entry,
		          &value_string_size,
		          error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve short name string size.",
			 function );

			goto on_error;
		}
		if( ( result == 1 )
		 && ( value_string_size > 0 ) )
		{
			value_string = system_string_allocate(
			                value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create short name string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libewf_file_entry_get_utf16_short_name(
			          file_entry,
			          (uint16_t *) value_string,
			          value_string_size,
			          error );
#else
			result = libewf_file_entry_get_utf8_short_name(
			          file_entry,
			          (uint8_t *) value_string,
			          value_string_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve short name string.",
				 function );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\tShort name\t\t\t: %" PRIs_SYSTEM "\n",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tSize\t\t\t\t: %" PRIu64 "\n",
		 size );

		if( info_handle_posix_time_value_fprint(
		     info_handle,
		     "\tCreation time\t\t\t",
		     creation_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( info_handle_posix_time_value_fprint(
		     info_handle,
		     "\tModification time\t\t",
		     modification_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( info_handle_posix_time_value_fprint(
		     info_handle,
		     "\tAccess time\t\t\t",
		     access_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( info_handle_posix_time_value_fprint(
		     info_handle,
		     "\tEntry modification time\t\t",
		     entry_modification_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( libewf_file_entry_get_deletion_time(
		     file_entry,
		     &deletion_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve deletion time.",
			 function );

			goto on_error;
		}
		if( info_handle_posix_time_value_fprint(
		     info_handle,
		     "\tDeletion time\t\t\t",
		     deletion_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			goto on_error;
		}
		if( libewf_file_entry_get_number_of_attributes(
		     file_entry,
		     &number_of_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of attributes.",
			 function );

			goto on_error;
		}
		if( number_of_attributes > 0 )
		{
			if( info_handle_sub_section_header_fprint(
			     info_handle,
			     "attributes",
			     "Attributes",
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print sub section header: attributes.",
				 function );

				goto on_error;
			}
			for( attribute_index = 0;
			     attribute_index < number_of_attributes;
			     attribute_index++ )
			{
				if( libewf_file_entry_get_attribute(
				     file_entry,
				     attribute_index,
				     &attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve attribute: %d.",
					 function,
					 attribute_index );

					goto on_error;
				}
				if( info_handle_attribute_value_fprint(
				     info_handle,
				     attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print attribute: %d.",
					 function,
					 attribute_index );

					goto on_error;
				}
				if( libewf_attribute_free(
				     &attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free attribute: %d.",
					 function,
					 attribute_index );

					goto on_error;
				}
			}
			if( info_handle_sub_section_footer_fprint(
			     info_handle,
			     "attributes",
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print sub section footer: attributes.",
				 function );

				goto on_error;
			}
		}
		if( libewf_file_entry_get_number_of_access_control_entries(
		     file_entry,
		     &number_of_access_control_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of access control entries.",
			 function );

			goto on_error;
		}
		if( number_of_access_control_entries > 0 )
		{
			if( info_handle_sub_section_header_fprint(
			     info_handle,
			     "permissions",
			     "Permissions",
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print sub section header: permissions.",
				 function );

				goto on_error;
			}
			for( access_control_entry_index = 0;
			     access_control_entry_index < number_of_access_control_entries;
			     access_control_entry_index++ )
			{
				if( libewf_file_entry_get_access_control_entry(
				     file_entry,
				     access_control_entry_index,
				     &access_control_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve access control entry: %d.",
					 function,
					 access_control_entry_index );

					goto on_error;
				}
				if( info_handle_access_control_entry_value_fprint(
				     info_handle,
				     access_control_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print access control entry: %d.",
					 function,
					 access_control_entry_index );

					goto on_error;
				}
				if( libewf_access_control_entry_free(
				     &access_control_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free access control entry: %d.",
					 function,
					 access_control_entry_index );

					goto on_error;
				}
			}
			if( info_handle_sub_section_footer_fprint(
			     info_handle,
			     "permissions",
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print sub section footer: permissions.",
				 function );

				goto on_error;
			}
		}
		result = libewf_file_entry_get_source(
		          file_entry,
		          &source,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve source.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\n" );

			if( info_handle_source_value_fprint(
			     info_handle,
			     source,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print source.",
				 function );

				goto on_error;
			}
			if( libewf_source_free(
			     &source,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free source.",
				 function );

				goto on_error;
			}
		}
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	return( 1 );

on_error:
	if( source != NULL )
	{
		libewf_source_free(
		 &source,
		 NULL );
	}
	if( access_control_entry != NULL )
	{
		libewf_access_control_entry_free(
		 &access_control_entry,
		 NULL );
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints file entry information as part of the logical files hierarchy
 * Returns 1 if successful or -1 on error
 */
int info_handle_logical_files_hierarchy_fprint_file_entry(
     info_handle_t *info_handle,
     libewf_file_entry_t *file_entry,
     const system_character_t *path,
     uint8_t is_root,
     libcerror_error_t **error )
{
	libewf_file_entry_t *sub_file_entry = NULL;
	system_character_t *file_entry_name = NULL;
	system_character_t *sub_path        = NULL;
	static char *function               = "info_handle_logical_files_hierarchy_fprint_file_entry";
	size_t file_entry_name_size         = 0;
	size_t path_length                  = 0;
	size_t sub_path_size                = 0;
	int number_of_sub_file_entries      = 0;
	int result                          = 0;
	int sub_file_entry_index            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	/* Ignore the name of the root file entry
	 */
	if( is_root == 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_file_entry_get_utf16_name_size(
		          file_entry,
		          &file_entry_name_size,
		          error );
#else
		result = libewf_file_entry_get_utf8_name_size(
		          file_entry,
		          &file_entry_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the file entry name size.",
			 function );

			goto on_error;
		}
	}
	if( ( result == 1 )
	 && ( file_entry_name_size > 0 ) )
	{
		file_entry_name = system_string_allocate(
		                   file_entry_name_size );

		if( file_entry_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file entry name.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_file_entry_get_utf16_name(
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libewf_file_entry_get_utf8_name(
		          file_entry,
		          (uint8_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the name.",
			 function );

			goto on_error;
		}
	}
	if( info_handle->bodyfile_stream != NULL )
	{
		if( info_handle_file_entry_value_fprint(
		     info_handle,
		     file_entry,
		     path,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print file entry.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t\t\t<file_entry" );
		}
		if( file_entry_name != NULL )
		{
			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
			{
				fprintf(
				 info_handle->notify_stream,
				 " name=\"" );
			}
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIs_SYSTEM "",
			 path );

			fprintf(
			 info_handle->notify_stream,
			 "%" PRIs_SYSTEM "",
			 file_entry_name );

			if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_TEXT )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\n" );
			}
			else if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
			{
				fprintf(
				 info_handle->notify_stream,
				 "\"" );
			}
		}
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
		{
			fprintf(
			 info_handle->notify_stream,
			 ">\n" );
		}
	}
	if( libewf_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		return( -1 );
	}
	if( number_of_sub_file_entries > 0 )
	{
		sub_path_size = path_length + 1;

		if( file_entry_name != NULL )
		{
			sub_path_size += file_entry_name_size;
		}
		sub_path = system_string_allocate(
		            sub_path_size );

		if( sub_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sub path.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     sub_path,
		     path,
		     path_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy path to sub path.",
			 function );

			goto on_error;
		}
		if( file_entry_name != NULL )
		{
			if( system_string_copy(
			     &( sub_path[ path_length ] ),
			     file_entry_name,
			     file_entry_name_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy file entry name to sub path.",
				 function );

				goto on_error;
			}
			sub_path[ sub_path_size - 2 ] = (system_character_t) LIBEWF_SEPARATOR;
		}
		sub_path[ sub_path_size - 1 ] = (system_character_t) 0;

		for( sub_file_entry_index = 0;
		     sub_file_entry_index < number_of_sub_file_entries;
		     sub_file_entry_index++ )
		{
			if( libewf_file_entry_get_sub_file_entry(
			     file_entry,
			     sub_file_entry_index,
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to free retrieve sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( info_handle_logical_files_hierarchy_fprint_file_entry(
			     info_handle,
			     sub_file_entry,
			     sub_path,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( libewf_file_entry_free(
			     &sub_file_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
		}
		if( sub_path != NULL )
		{
			memory_free(
			 sub_path );

			sub_path = NULL;
		}
	}
	if( info_handle->bodyfile_stream == NULL )
	{
		if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t\t\t</file_entry>\n" );
		}
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	return( 1 );

on_error:
	if( sub_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( sub_path != NULL )
	{
		memory_free(
		 sub_path );
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints the file entry information for a specific path
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entry_fprint_by_path(
     info_handle_t *info_handle,
     const system_character_t *path,
     libcerror_error_t **error )
{
	libewf_file_entry_t *file_entry = NULL;
	static char *function           = "info_handle_file_entry_fprint_by_path";
	size_t path_index               = 0;
	size_t path_length              = 0;
	int result                      = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	for( path_index = path_length - 1;
	     path_index > 0;
	     path_index-- )
	{
		if( path[ path_index ] == (system_character_t) LIBEWF_SEPARATOR )
		{
			break;
		}
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_get_file_entry_by_utf16_path(
	          info_handle->input_handle,
	          (uint16_t *) path,
	          path_length,
	          &file_entry,
	          error );
#else
	result = libewf_handle_get_file_entry_by_utf8_path(
	          info_handle->input_handle,
	          (uint8_t *) path,
	          path_length,
	          &file_entry,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: file entry not found.",
		 function );

		goto on_error;
	}
	if( info_handle_section_header_fprint(
	     info_handle,
	     "single_file",
	     "Logical file entry",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section header: single_file.",
		 function );

		goto on_error;
	}
	if( info_handle_file_entry_value_fprint(
	     info_handle,
	     file_entry,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print file entry.",
		 function );

		goto on_error;
	}
	if( info_handle_section_footer_fprint(
	     info_handle,
	     "single_file",
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print section footer: single_file.",
		 function );

		goto on_error;
	}
	if( libewf_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the logical files hierarchy information
 * Returns 1 if successful or -1 on error
 */
int info_handle_logical_files_hierarchy_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libewf_file_entry_t *file_entry = NULL;
	static char *function           = "info_handle_logical_files_hierarchy_fprint";
	int result                      = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	result = libewf_handle_get_root_file_entry(
	          info_handle->input_handle,
	          &file_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root file entry.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( info_handle_section_header_fprint(
		     info_handle,
		     "single_files",
		     "Logical files",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section header: single_files.",
			 function );

			goto on_error;
		}
		if( info_handle_logical_files_hierarchy_fprint_file_entry(
		     info_handle,
		     file_entry,
		     _SYSTEM_STRING( "" ),
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print root file entry.",
			 function );

			goto on_error;
		}
		if( info_handle_section_footer_fprint(
		     info_handle,
		     "single_files",
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print section footer: single_files.",
			 function );

			goto on_error;
		}
		if( libewf_file_entry_free(
		     &file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free root file entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( file_entry != NULL )
	{
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the image information
 * Returns 1 if successful or -1 on error
 */
int info_handle_image_fprint(
     info_handle_t *info_handle,
     char info_option,
     libcerror_error_t **error )
{
	static char *function = "info_handle_image_fprint";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		if( info_handle_dfxml_header_fprint(
		     info_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print DFXML header.",
			 function );

			return( -1 );
		}
	}
	if( ( info_option == 'a' )
	 || ( info_option == 'i' ) )
	{
		if( info_handle_header_values_fprint(
		     info_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header values.",
			 function );

			return( -1 );
		}
	}
	if( ( info_option == 'a' )
	 || ( info_option == 'm' ) )
	{
		if( info_handle_media_information_fprint(
		     info_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print media information.",
			 function );

			return( -1 );
		}
		if( info_handle_hash_values_fprint(
		     info_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print hash values.",
			 function );

			return( -1 );
		}
		if( info_handle_sessions_fprint(
		     info_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print sessions.",
			 function );

			return( -1 );
		}
		if( info_handle_tracks_fprint(
		     info_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print tracks.",
			 function );

			return( -1 );
		}
	}
	if( ( info_option == 'a' )
	 || ( info_option == 'e' ) )
	{
		if( info_handle_acquiry_errors_fprint(
		     info_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print acquiry errors.",
			 function );

			return( -1 );
		}
	}
	if( info_handle->output_format == INFO_HANDLE_OUTPUT_FORMAT_DFXML )
	{
		if( info_handle_dfxml_footer_fprint(
		     info_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print DFXML footer.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Prints the DFXML header
 * Returns 1 if successful or -1 on error
 */
int info_handle_dfxml_header_fprint(
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function = "info_handle_dfxml_header_fprint";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );

	/* TODO what about DTD or XSD ? */

	fprintf(
	 stream,
	 "<ewfobjects version=\"0.1\">\n" );

	fprintf(
	 stream,
	 "\t<metadata xmlns=\"http://libewf.sourceforge.net/\"\n"
	 "\t          xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
	 "\t          xmlns:dc=\"http://purl.org/dc/elements/1.1/\">\n"
	 "\t\t<dc:type>Disk Image</dc:type>\n"
	 "\t</metadata>\n" );

	fprintf(
	 stream,
	 "\t<creator>\n"
	 "\t\t<program>ewfinfo</program>\n"
	 "\t\t<version>%s</version>\n",
	 LIBEWF_VERSION_STRING );

	if( info_handle_dfxml_build_environment_fprint(
	     stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print build environment.",
		 function );

		return( -1 );
	}
	if( info_handle_dfxml_execution_environment_fprint(
	     stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print execution environment.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\t</creator>\n"
	 "\t<ewfinfo>\n" );

	return( 1 );
}

/* Prints the DFXML footer
 * Returns 1 if successful or -1 on error
 */
int info_handle_dfxml_footer_fprint(
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function = "info_handle_dfxml_footer_fprint";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\t</ewfinfo>\n"
	 "</ewfobjects>\n"
	 "\n" );

	return( 1 );
}

/* Prints the DFXML build environment
 * Returns 1 if successful or -1 on error
 */
int info_handle_dfxml_build_environment_fprint(
     FILE *stream,
     libcerror_error_t **error )
{
	static char *function      = "info_handle_dfxml_build_environment_fprint";

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
	const char *compiler_name  = NULL;
	const char *version_string = NULL;
#endif

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\t\t<build_environment>\n" );

#if defined( _MSC_VER )
#if _MSC_VER == 600
	compiler_name = "Microsoft C";

	version_string = "6.0";
#elif _MSC_VER == 700
	compiler_name = "Microsoft C/C++";

	version_string = "7.0";

#elif _MSC_VER >= 800
	compiler_name = "Microsoft Visual C/C++";

#if _MSC_VER == 800
	version_string = "1.0";
#elif _MSC_VER == 900
	version_string = "2.0";
/* TODO what about 3.0 ? */
#elif _MSC_VER == 1000
	version_string = "4.0";
#elif _MSC_VER == 1100
	version_string = "5.0";
#elif _MSC_VER == 1200
	version_string = "6.0";
#elif _MSC_VER == 1300
	version_string = "7.0";
#elif _MSC_VER == 1310
	version_string = "7.1";
#elif _MSC_VER == 1400
	version_string = "8.0";
#elif _MSC_VER == 1500
	version_string = "9.0";
#elif _MSC_VER == 1600
	version_string = "10.0";
#endif

#endif
	if( ( compiler_name == NULL )
	 || ( version_string == NULL ) )
	{
		fprintf(
		 stream,
		 "\t\t\t<compiler>MSC %d</compiler>\n",
		 _MSC_VER );
	}
	else
	{
		fprintf(
		 stream,
		 "\t\t\t<compiler>%s %s (MSC %d)</compiler>\n",
		 compiler_name,
		 version_string,
		 _MSC_VER );
	}
#elif defined( __BORLANDC__ )
#if __BORLANDC__ >= 0x0520
	compiler_name = "Borland C++ Builder";

#if __BORLANDC__ == 0x0520
	version_string = "1.0";
/* TODO what about 2.0 ? */
#elif __BORLANDC__ == 0x0530
	version_string = "3.0";
#elif __BORLANDC__ == 0x0540
	version_string = "4.0";
#elif __BORLANDC__ == 0x0550
	version_string = "5.0";
#elif __BORLANDC__ == 0x0560
	version_string = "6.0";
#endif

/* TODO what about codegear ? */
#endif
	if( ( compiler_name == NULL )
	 || ( version_string == NULL ) )
	{
		fprintf(
		 stream,
		 "\t\t\t<compiler>BORLANDC 0x%04x</compiler>\n",
		 __BORLANDC__ );
	}
	else
	{
		fprintf(
		 stream,
		 "\t\t\t<compiler>%s %s (BORLANDC 0x%04x)</compiler>\n",
		 compiler_name,
		 version_string,
		 __BORLANDC__ );
	}

#elif defined( __GNUC__ )
#if defined( __CYGWIN__ )
	fprintf(
	 stream,
	 "\t\t\t<compiler>Cygwin</compiler>\n" );
#endif
#if defined( __MINGW64_VERSION_MAJOR ) && defined( __MINGW64_VERSION_MINOR )
	fprintf(
	 stream,
	 "\t\t\t<compiler>MinGW64 %d.%d</compiler>\n",
	 __MINGW64_VERSION_MAJOR,
	 __MINGW64_VERSION_MINOR );
#elif defined( __MINGW32_MAJOR_VERSION ) && defined( __MINGW32_MINOR_VERSION )
	fprintf(
	 stream,
	 "\t\t\t<compiler>MinGW32 %d.%d</compiler>\n",
	 __MINGW32_MAJOR_VERSION,
	 __MINGW32_MINOR_VERSION );
#endif
#if defined( __GNUC_MINOR__ )
	fprintf(
	 stream,
	 "\t\t\t<compiler>GCC %d.%d</compiler>\n",
	 __GNUC__,
	 __GNUC_MINOR__ );
#else
	fprintf(
	 stream,
	 "\t\t\t<compiler>GCC %d</compiler>\n",
	 __GNUC__ );
#endif
#endif
	fprintf(
	 stream,
	 "\t\t\t<compilation_date>" __DATE__ " " __TIME__ "</compilation_date>\n" );

	fprintf(
	 stream,
	 "\t\t\t<library name=\"libewf\" version=\"%s\"/>\n",
	 LIBEWF_VERSION_STRING );

	/* TODO add other libraries
	 */

	fprintf(
	 stream,
	 "\t\t</build_environment>\n" );

	return( 1 );
}

/* Prints the DFXML execution environment
 * Returns 1 if successful or -1 on error
 */
int info_handle_dfxml_execution_environment_fprint(
     FILE *stream,
     libcerror_error_t **error )
{
#if defined( HAVE_UNAME ) && !defined( WINAPI )
	struct utsname utsname_buffer;
#endif
#if defined( WINAPI )
	system_character_t operating_system[ 32 ];
#endif

	static char *function = "info_handle_dfxml_execution_environment_fprint";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	/* TODO what about execution environment on other platforms ? */

	fprintf(
	 stream,
	 "\t\t<execution_environment>\n" );

#if defined( WINAPI )
	if( platform_get_operating_system(
	     operating_system,
	     32,
	     NULL ) == 1 )
	{
		fprintf(
		 stream,
		 "\t\t\t<os_sysname>%" PRIs_SYSTEM "</os_sysname>\n",
		 operating_system );
	}
#elif defined( HAVE_UNAME )
	if( uname(
	     &utsname_buffer ) == 0 )
	{
		fprintf(
		 stream,
		 "\t\t\t<os_sysname>%s</os_sysname>\n"
		 "\t\t\t<os_release>%s</os_release>\n"
		 "\t\t\t<os_version>%s</os_version>\n"
		 "\t\t\t<host>%s</host>\n"
		 "\t\t\t<arch>%s</arch>\n",
		 utsname_buffer.sysname,
		 utsname_buffer.release,
		 utsname_buffer.version,
		 utsname_buffer.nodename,
		 utsname_buffer.machine );
	}
#endif
	/* TODO
	 * <command_line> X </command_line>
	 * <uid> getuid() </uid>
	 * <username> getpwuid( getuid() )->pw_name </username>
	 */
	fprintf(
	 stream,
	 "\t\t</execution_environment>\n" );

	return( 1 );
}


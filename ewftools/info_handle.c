/* 
 * Info handle
 *
 * Copyright (C) 2007-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#include <libsystem.h>

#include "byte_size_string.h"
#include "digest_hash.h"
#include "guid.h"
#include "info_handle.h"

#define INFO_HANDLE_VALUE_SIZE			512
#define INFO_HANDLE_VALUE_IDENTIFIER_SIZE	64

#if !defined( USE_LIBEWF_GET_HASH_VALUE_MD5 ) && !defined( USE_LIBEWF_GET_MD5_HASH )
#define USE_LIBEWF_GET_HASH_VALUE_MD5
#endif

/* Initializes the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     liberror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle == NULL )
	{
		*info_handle = (info_handle_t *) memory_allocate(
		                                  sizeof( info_handle_t ) );

		if( *info_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create info handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *info_handle,
		     0,
		     sizeof( info_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear info handle.",
			 function );

			memory_free(
			 *info_handle );

			*info_handle = NULL;

			return( -1 );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_initialize(
		     &( ( *info_handle )->input_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input handle.",
			 function );

			memory_free(
			 *info_handle );

			*info_handle = NULL;

			return( -1 );
		}
#endif
	}
	return( 1 );
}

/* Frees the info handle and its elements
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     liberror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( ( ( *info_handle )->input_handle != NULL )
		 && ( libewf_handle_free(
		       &( ( *info_handle )->input_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.",
			 function );

			result = -1;
		}
#endif
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
     liberror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle != NULL )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_signal_abort(
		     info_handle->input_handle,
		     error ) != 1 )
#else
		if( libewf_signal_abort(
		     info_handle->input_handle ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the input of the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     libsystem_character_t * const * filenames,
     int amount_of_filenames,
     liberror_error_t **error )
{
	libsystem_character_t **libewf_filenames = NULL;
	static char *function                    = "info_handle_open_input";
	size_t first_filename_length             = 0;
	int result                               = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
#else
	if( info_handle->input_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - input handle already set.",
		 function );

		return( -1 );
	}
#endif
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
	if( amount_of_filenames <= 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of filenames.",
		 function );

		return( -1 );
	}
	if( amount_of_filenames == 1 )
	{
		first_filename_length = libsystem_string_length(
		                         filenames[ 0 ] );

#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
#if defined( HAVE_V2_API )
		if( libewf_glob_wide(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#else
		amount_of_filenames = libewf_glob_wide(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );

		if( amount_of_filenames <= 0 )
#endif
#else
#if defined( HAVE_V2_API )
		if( libewf_glob(
		     filenames[ 0 ],
		     first_filename_length,
		     LIBEWF_FORMAT_UNKNOWN,
		     &libewf_filenames,
		     &amount_of_filenames,
		     error ) != 1 )
#else
		amount_of_filenames = libewf_glob(
		                       filenames[ 0 ],
		                       first_filename_length,
		                       LIBEWF_FORMAT_UNKNOWN,
		                       &libewf_filenames );

		if( amount_of_filenames <= 0 )
#endif
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to resolve filename(s).",
			 function );

			return( -1 );
		}
		filenames = (libsystem_character_t * const *) libewf_filenames;
	}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
#if defined( HAVE_V2_API )
	if( libewf_handle_open_wide(
	     info_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	info_handle->input_handle = libewf_open_wide(
	                             filenames,
	                             amount_of_filenames,
	                             LIBEWF_OPEN_READ );

	if( info_handle->input_handle == NULL )
#endif
#else
#if defined( HAVE_V2_API )
	if( libewf_handle_open(
	     info_handle->input_handle,
	     filenames,
	     amount_of_filenames,
	     LIBEWF_OPEN_READ,
	     error ) != 1 )
#else
	info_handle->input_handle = libewf_open(
	                             filenames,
	                             amount_of_filenames,
	                             LIBEWF_OPEN_READ );

	if( info_handle->input_handle == NULL )
#endif
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file(s).",
		 function );

		result = -1;
	}
	if( libewf_filenames != NULL )
	{
		for( ; amount_of_filenames > 0; amount_of_filenames-- )
		{
			memory_free(
			 libewf_filenames[ amount_of_filenames - 1 ] );
		}
		memory_free(
		 libewf_filenames );
	}
	return( result );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     liberror_error_t **error )
{
	static char *function = "info_handle_close";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_close(
	     info_handle->input_handle,
	     error ) != 0 )
#else
	if( libewf_close(
	     info_handle->input_handle ) != 0 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
#if !defined( HAVE_V2_API )
	info_handle->input_handle = NULL;
#endif
	return( 0 );
}

/* Retrieves the header value from the input handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int info_handle_get_header_value(
     info_handle_t *info_handle,
     char *header_value_identifier,
     size_t header_value_identifier_length,
     libsystem_character_t *header_value,
     size_t header_value_size,
     liberror_error_t **error )
{
	uint8_t utf8_header_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function               = "info_handle_get_header_value";
	size_t calculated_header_value_size = 0;
	size_t utf8_header_value_size       = INFO_HANDLE_VALUE_SIZE;
	int result                          = 0;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	result = libewf_handle_get_header_value(
	          info_handle->input_handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          utf8_header_value,
	          utf8_header_value_size,
	          error );
#else
	result = libewf_get_header_value(
	          info_handle->input_handle,
	          header_value_identifier,
	          (char *) utf8_header_value,
	          utf8_header_value_size );
#endif

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 header_value_identifier );

		return( -1 );
	}
	else if( result == 1 )
	{
		/* Determine the header value size
		 */
		utf8_header_value_size = 1 + narrow_string_length(
		                              (char *) utf8_header_value );

		if( libsystem_string_size_from_utf8_string(
		     utf8_header_value,
		     utf8_header_value_size,
		     &calculated_header_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine header value size.",
			 function );

			return( -1 );
		}
		if( header_value_size < calculated_header_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: header value too small.",
			 function );

			return( -1 );
		}
		if( libsystem_string_copy_from_utf8_string(
		     header_value,
		     header_value_size,
		     utf8_header_value,
		     utf8_header_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set header value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the hash value from the input handle
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int info_handle_get_hash_value(
     info_handle_t *info_handle,
     char *hash_value_identifier,
     size_t hash_value_identifier_length,
     libsystem_character_t *hash_value,
     size_t hash_value_size,
     liberror_error_t **error )
{
	uint8_t utf8_hash_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function             = "info_handle_get_hash_value";
	size_t calculated_hash_value_size = 0;
	size_t utf8_hash_value_size       = INFO_HANDLE_VALUE_SIZE;
	int result                        = 0;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	result = libewf_handle_get_hash_value(
	          info_handle->input_handle,
	          (uint8_t *) hash_value_identifier,
	          hash_value_identifier_length,
	          utf8_hash_value,
	          utf8_hash_value_size,
	          error );
#else
	result = libewf_get_hash_value(
	          info_handle->input_handle,
	          hash_value_identifier,
	          (char *) utf8_hash_value,
	          utf8_hash_value_size );
#endif

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hash value: %s.",
		 function,
		 hash_value_identifier );

		return( -1 );
	}
	else if( result == 1 )
	{
		/* Determine the hash value size
		 */
		utf8_hash_value_size = 1 + narrow_string_length(
		                            (char *) utf8_hash_value );

		if( libsystem_string_size_from_utf8_string(
		     utf8_hash_value,
		     utf8_hash_value_size,
		     &calculated_hash_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine hash value size.",
			 function );

			return( -1 );
		}
		if( hash_value_size < calculated_hash_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: hash value too small.",
			 function );

			return( -1 );
		}
		if( libsystem_string_copy_from_utf8_string(
		     hash_value,
		     hash_value_size,
		     utf8_hash_value,
		     utf8_hash_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set hash value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Sets the header codepage
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_header_codepage(
     info_handle_t *info_handle,
     int header_codepage,
     liberror_error_t **error )
{
	static char *function = "info_handle_set_header_codepage";

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_set_header_codepage(
	     info_handle->input_handle,
	     header_codepage,
	     error ) != 1 )
#else
	if( libewf_set_header_codepage(
	     info_handle->input_handle,
	     header_codepage ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Print the header values to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_header_values_fprint(
     info_handle_t *info_handle,
     uint8_t date_format,
     FILE *stream,
     liberror_error_t **error )
{
	char header_value_identifier[ INFO_HANDLE_VALUE_IDENTIFIER_SIZE ];
	libsystem_character_t header_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function               = "info_handle_header_values_fprint";
	size_t header_value_identifier_size = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;
	size_t header_value_size            = INFO_HANDLE_VALUE_SIZE;
	uint32_t amount_of_values           = 0;
	uint32_t header_value_iterator      = 0;
	int header_value_result             = 0;
	int result                          = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date format: 0x%02" PRIx8 ".",
		 function,
		 date_format );

		return( -1 );
	}
#if !defined( HAVE_V2_API )
	if( libewf_parse_header_values(
	     info_handle->input_handle,
	     date_format ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to parse header values.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_V2_API )
	if( libewf_handle_set_header_values_date_format(
	     info_handle->input_handle,
	     date_format,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header values date format.",
		 function );

		return( -1 );
	}
	if( libewf_handle_get_amount_of_header_values(
	     info_handle->input_handle,
	     &amount_of_values,
	     error ) == -1 )
#else
	if( libewf_get_amount_of_header_values(
	     info_handle->input_handle,
	     &amount_of_values ) == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of header values.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "Acquiry information\n" );

	if( amount_of_values == 0 )
	{
		fprintf(
		 stream,
		 "\tNo information found in file.\n" );
	}
	else
	{
		/* Case number
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "case_number",
		                       11,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tCase number:\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: case_number.",
			 function );

			result = -1;
		}
		/* Description
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "description",
		                       11,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tDescription:\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: description.",
			 function );

			result = -1;
		}
		/* Examiner name
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "examiner_name",
		                       13,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tExaminer name:\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: examiner_name.",
			 function );

			result = -1;
		}
		/* Evidence nubmer
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "evidence_number",
		                       15,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tEvidence number:\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: evidence_number.",
			 function );

			result = -1;
		}
		/* Notes
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "notes",
		                       5,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tNotes:\t\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: notes.",
			 function );

			result = -1;
		}
		/* Acquiry date
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "acquiry_date",
		                       12,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tAcquiry date:\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: acquiry_date.",
			 function );

			result = -1;
		}
		/* System date
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "system_date",
		                       11,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tSystem date:\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: system_date.",
			 function );

			result = -1;
		}
		/* Acquiry operating system
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "acquiry_operating_system",
		                       24,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tOperating system used:\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: acquiry_operating_system.",
			 function );

			result = -1;
		}
		/* Acquiry software version
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "acquiry_software_version",
		                       24,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tSoftware version used:\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: acquiry_software_version.",
			 function );

			result = -1;
		}
		/* Password
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "password",
		                       8,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tPassword:\t\t(hash: %" PRIs_LIBSYSTEM ")\n",
			 header_value );
		}
		else if( header_value_result == 0 )
		{
			fprintf(
			 stream,
			 "\tPassword:\t\tN/A\n" );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: password.",
			 function );

			result = -1;
		}
		/* Compression type
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "compression_type",
		                       16,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tCompression type:\t" );

			if( libsystem_string_compare(
			     header_value,
			     _LIBSYSTEM_CHARACTER_T_STRING( LIBEWF_COMPRESSION_TYPE_NONE ),
			     1 ) == 0 )
			{
				fprintf(
				 stream,
				 "no compression\n" );
			}
			else if( libsystem_string_compare(
				  header_value,
				  _LIBSYSTEM_CHARACTER_T_STRING( LIBEWF_COMPRESSION_TYPE_FAST ),
				  1 ) == 0 )
			{
				fprintf(
				 stream,
				 "good (fast) compression\n" );
			}
			else if( libsystem_string_compare(
				  header_value,
				  _LIBSYSTEM_CHARACTER_T_STRING( LIBEWF_COMPRESSION_TYPE_BEST ),
				  1 ) == 0 )
			{
				fprintf(
				 stream,
				 "best compression\n" );
			}
			else
			{
				fprintf(
				 stream,
				 "unknown compression\n" );
			}
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: compression_type.",
			 function );

			result = -1;
		}
		/* Model
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "model",
		                       5,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tModel:\t\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: model.",
			 function );

			result = -1;
		}
		/* Serial number
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "serial_number",
		                       13,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tSerial number:\t\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: serial_number.",
			 function );

			result = -1;
		}
		/* Process identifier
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "process_identifier",
		                       18,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tProcess identifier:\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: process_identifier.",
			 function );

			result = -1;
		}
		/* Unknown dc
		 * TODO figure out what this value represents and add get & set API functions to libewf
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "unknown_dc",
		                       10,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == 1 )
		{
			fprintf(
			 stream,
			 "\tUnknown value dc:\t%" PRIs_LIBSYSTEM "\n",
			 header_value );
		}
		else if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: unknown_dc.",
			 function );

			result = -1;
		}
		/* Extents
		 */
		header_value_result = info_handle_get_header_value(
		                       info_handle,
		                       "extents",
		                       7,
		                       header_value,
		                       header_value_size,
		                       error );

		if( header_value_result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: extents.",
			 function );

			result = -1;
		}
		else if( ( header_value_result == 1 )
		      && ( info_handle_header_value_extents_fprint(
		            header_value,
		            header_value_size - 1,
		            stream,
		            error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print header value: extents.",
			 function );

			result = -1;
		}
		/* Currently there are 16 default values
		 */
		if( amount_of_values > 16 )
		{
			fprintf(
			 stream,
			 "\n\tAdditional values:\n" );

			for( header_value_iterator = 16; header_value_iterator < amount_of_values; header_value_iterator++ )
			{
#if defined( HAVE_V2_API )
				if( libewf_handle_get_header_value_identifier_size(
				     info_handle->input_handle,
				     header_value_iterator,
				     &header_value_identifier_size,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the header identifier size for index: %" PRIu32 ".",
					 function,
					 header_value_iterator );

					result = -1;

					continue;
				}
				if( header_value_identifier_size > INFO_HANDLE_VALUE_IDENTIFIER_SIZE )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
					 "%s: header identifier size value out of range for index: %" PRIu32 ".",
					 function,
					 header_value_iterator );

					result = -1;

					continue;
				}
				if( libewf_handle_get_header_value_identifier(
				     info_handle->input_handle,
				     header_value_iterator,
				     (uint8_t *) header_value_identifier,
				     header_value_identifier_size,
				     error ) != 1 )
#else
				header_value_identifier_size = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;

				if( libewf_get_header_value_identifier(
				     info_handle->input_handle,
				     header_value_iterator,
				     header_value_identifier,
				     header_value_identifier_size ) != 1 )
#endif
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the header identifier for index: %" PRIu32 ".",
					 function,
					 header_value_iterator );

					result = -1;

					continue;
				}
#if !defined( HAVE_V2_API )
				header_value_identifier_size = 1 + narrow_string_length(
				                                    header_value_identifier );
#endif
				header_value_result = info_handle_get_header_value(
				                       info_handle,
				                       header_value_identifier,
				                       header_value_identifier_size - 1,
				                       header_value,
				                       header_value_size,
				                       error );

				if( header_value_result == 1 )
				{
					fprintf(
					 stream,
					 "\t%s: %" PRIs_LIBSYSTEM "\n",
					 header_value_identifier,
					 header_value );
				}
				else if( header_value_result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the header value for identifier: %s.",
					 function,
					 header_value_identifier );

					result = -1;
				}
			}
		}
	}
	fprintf(
	 stream,
	 "\n" );

	return( result );
}

/* Print the extents header values to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_header_value_extents_fprint(
     libsystem_character_t *header_value,
     size_t header_value_length,
     FILE *stream,
     liberror_error_t **error )
{
	libsystem_character_t **extents_elements = NULL;
	static char *function                    = "info_handle_header_value_extents_fprint";
	size_t amount_of_extents_elements        = 0;
	size_t extents_element_iterator          = 0;

	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( libsystem_string_split(
	     header_value,
	     header_value_length,
	     (libsystem_character_t) ' ',
	     &extents_elements,
	     &amount_of_extents_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header value into elements.",
		 function );

		return( -1 );
	}
	if( ( amount_of_extents_elements % 4 ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported amount of extents elements in header value.",
		 function );

		libsystem_string_split_values_free(
		 extents_elements,
		 amount_of_extents_elements,
		 NULL );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\tExtents:\t\t%" PRIs_LIBSYSTEM "\n",
	 extents_elements[ 0 ] );

	for( extents_element_iterator = 1;
	     extents_element_iterator < amount_of_extents_elements;
	     extents_element_iterator += 4 )
	{
		fprintf(
		 stream,
		 "\t\t\t\t%" PRIs_LIBSYSTEM " %" PRIs_LIBSYSTEM " %" PRIs_LIBSYSTEM " %" PRIs_LIBSYSTEM "\n",
		 extents_elements[ extents_element_iterator ],
		 extents_elements[ extents_element_iterator + 1 ],
		 extents_elements[ extents_element_iterator + 2 ],
		 extents_elements[ extents_element_iterator + 3 ] );
	}
	if( libsystem_string_split_values_free(
	     extents_elements,
	     amount_of_extents_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split date time elements.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Print the media information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_media_information_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
        libsystem_character_t media_size_string[ 16 ];
        libsystem_character_t guid_string[ GUID_STRING_SIZE ];
        uint8_t guid[ GUID_SIZE ];

	char *format_string        = NULL;
	static char *function      = "info_handle_media_information_fprint";
	size64_t media_size        = 0;
#if defined( HAVE_V2_API )
	uint64_t amount_of_sectors = 0;
#else
	uint32_t amount_of_sectors = 0;
#endif
	uint32_t bytes_per_sector  = 0;
	uint32_t error_granularity = 0;
	uint32_t sectors_per_chunk = 0;
	uint8_t compression_flags  = 0;
	uint8_t media_type         = 0;
	uint8_t media_flags        = 0;
	uint8_t format             = 0;
	int8_t compression_level   = 0;
	int result                 = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "EWF information\n" );

#if defined( HAVE_V2_API )
	if( libewf_handle_get_format(
	     info_handle->input_handle,
	     &format,
	     error ) != 1 )
#else
	if( libewf_get_format(
	     info_handle->input_handle,
	     &format ) != 1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format.",
		 function );

		return( -1 );
	}
	else
	{
		switch( format )
		{
			case LIBEWF_FORMAT_EWF:
				format_string = "original EWF";
				break;

			case LIBEWF_FORMAT_SMART:
				format_string = "SMART";
				break;

			case LIBEWF_FORMAT_FTK:
				format_string = "FTK Imager";
				break;

			case LIBEWF_FORMAT_ENCASE1:
				format_string = "EnCase 1";
				break;

			case LIBEWF_FORMAT_ENCASE2:
				format_string = "EnCase 2";
				break;

			case LIBEWF_FORMAT_ENCASE3:
				format_string = "EnCase 3";
				break;

			case LIBEWF_FORMAT_ENCASE4:
				format_string = "EnCase 4";
				break;

			case LIBEWF_FORMAT_ENCASE5:
				format_string = "EnCase 5";
				break;

			case LIBEWF_FORMAT_ENCASE6:
				format_string = "EnCase 6";
				break;

			case LIBEWF_FORMAT_LINEN5:
				format_string = "linen 5";
				break;

			case LIBEWF_FORMAT_LINEN6:
				format_string = "linen 6";
				break;

			case LIBEWF_FORMAT_EWFX:
				format_string = "EWFX (extended EWF)";
				break;

			case LIBEWF_FORMAT_UNKNOWN:
			default:
				format_string = "unknown";
				break;

		}
		fprintf(
		 stdout,
		 "\tFile format:\t\t%s\n",
		 format_string );
	}
	if( ( format == LIBEWF_FORMAT_ENCASE5 )
	 || ( format == LIBEWF_FORMAT_ENCASE6 )
	 || ( format == LIBEWF_FORMAT_LINEN5 )
	 || ( format == LIBEWF_FORMAT_LINEN6 )
	 || ( format == LIBEWF_FORMAT_EWFX ) )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_get_sectors_per_chunk(
		     info_handle->input_handle,
		     &sectors_per_chunk,
		     error ) == 1 )
#else
		if( libewf_get_sectors_per_chunk(
		     info_handle->input_handle,
		     &sectors_per_chunk ) == 1 )
#endif
		{
			fprintf(
			 stream,
			 "\tSectors per chunk:\t%" PRIu32 "\n",
			 sectors_per_chunk );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sectors per chunk.",
			 function );

			result = -1;
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_get_error_granularity(
		     info_handle->input_handle,
		     &error_granularity,
		     error ) == 1 )
#else
		if( libewf_get_error_granularity(
		     info_handle->input_handle,
		     &error_granularity ) == 1 )
#endif
		{
			fprintf(
			 stream,
			 "\tError granularity:\t%" PRIu32 "\n",
			 error_granularity );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve error granularity.",
			 function );

			result = -1;
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_get_compression_values(
		     info_handle->input_handle,
		     &compression_level,
		     &compression_flags,
		     error ) == 1 )
#else
		if( libewf_get_compression_values(
		     info_handle->input_handle,
		     &compression_level,
		     &compression_flags ) == 1 )
#endif
		{
			if( compression_level == LIBEWF_COMPRESSION_NONE )
			{
				fprintf(
				 stream,
				 "\tCompression type:\tno compression\n" );
			}
			else if( compression_level == LIBEWF_COMPRESSION_FAST )
			{
				fprintf(
				 stream,
				 "\tCompression type:\tgood (fast) compression\n" );
			}
			else if( compression_level == LIBEWF_COMPRESSION_BEST )
			{
				fprintf(
				 stream,
				 "\tCompression type:\tbest compression\n" );
			}
			else
			{
				fprintf(
				 stream,
				 "\tCompression type:\tunknown compression\n" );
			}
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve compression values.",
			 function );

			result = -1;
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_get_guid(
		     info_handle->input_handle,
		     guid,
		     GUID_SIZE,
		     error ) != 1 )
#else
		if( libewf_get_guid(
		     info_handle->input_handle,
		     guid,
		     GUID_SIZE ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve GUID.",
			 function );

			result = -1;
		}
		else if( guid_to_string(
		          guid,
		          GUID_SIZE,
		          _BYTE_STREAM_ENDIAN_LITTLE,
		          guid_string,
		          GUID_STRING_SIZE,
		          NULL ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to create GUID string.",
			 function );

			result = -1;
		}
		else
		{
			fprintf(
			 stream,
			 "\tGUID:\t\t\t%" PRIs_LIBSYSTEM "\n",
			 guid_string );
		}
	}
	fprintf(
	 stream,
	 "\n" );
	fprintf(
	 stream,
	 "Media information\n" );

	if( ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_SMART ) )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_get_media_type(
		     info_handle->input_handle,
		     &media_type,
		     error ) == 1 )
#else
		if( libewf_get_media_type(
		     info_handle->input_handle,
		     &media_type ) == 1 )
#endif
		{
			if( media_type == LIBEWF_MEDIA_TYPE_REMOVABLE )
			{
				fprintf(
				 stream,
				 "\tMedia type:\t\tremovable disk\n" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_FIXED )
			{
				fprintf(
				 stream,
				 "\tMedia type:\t\tfixed disk\n" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_OPTICAL )
			{
				fprintf(
				 stream,
				 "\tMedia type:\t\toptical disk (CD/DVD/BD)\n" );
			}
			else if( media_type == LIBEWF_MEDIA_TYPE_MEMORY )
			{
				fprintf(
				 stream,
				 "\tMedia type:\t\tmemory (RAM)\n" );
			}
			else
			{
				fprintf(
				 stream,
				 "\tMedia type:\t\tunknown (0x%" PRIx8 ")\n",
				 media_type );
			}
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media type.",
			 function );

			result = -1;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
#if defined( HAVE_V2_API )
		if( libewf_handle_get_media_flags(
		     info_handle->input_handle,
		     &media_flags,
		     error ) == 1 )
#else
		if( libewf_get_media_flags(
		     info_handle->input_handle,
		     &media_flags ) == 1 )
#endif
		{
			fprintf(
			 stream,
			 "\tMedia flags:\t\t0x%02" PRIx8 "\n",
			 media_flags );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media flags.",
			 function );

			result = -1;
		}
#endif
		fprintf(
		 stream,
		 "\tIs physical:\t\t" );

		if( ( media_flags & LIBEWF_MEDIA_FLAG_PHYSICAL ) == LIBEWF_MEDIA_FLAG_PHYSICAL )
		{
			fprintf(
			 stream,
			 "yes" );
		}
		else
		{
			fprintf(
			 stream,
			 "no" );
		}
		fprintf(
		 stream,
		 "\n" );

		if( ( media_flags & LIBEWF_MEDIA_FLAG_FASTBLOC ) == LIBEWF_MEDIA_FLAG_FASTBLOC )
		{
			fprintf(
			 stream,
			 "\tWrite blocked:\t\tFastbloc\n" );
		}
		if( ( media_flags & LIBEWF_MEDIA_FLAG_TABLEAU ) == LIBEWF_MEDIA_FLAG_TABLEAU )
		{
			fprintf(
			 stream,
			 "\tWrite blocked:\t\tTableau\n" );
		}
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_bytes_per_sector(
	     info_handle->input_handle,
	     &bytes_per_sector,
	     error ) == 1 )
#else
	if( libewf_get_bytes_per_sector(
	     info_handle->input_handle,
	     &bytes_per_sector ) == 1 )
#endif
	{
		fprintf(
		 stream,
		 "\tBytes per sector:\t%" PRIu32 "\n",
		 bytes_per_sector );
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		result = -1;
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_amount_of_sectors(
	     info_handle->input_handle,
	     &amount_of_sectors,
	     error ) == 1 )
	{
		fprintf(
		 stream,
		 "\tAmount of sectors:\t%" PRIu64 "\n",
		 amount_of_sectors );
	}
#else
	if( libewf_get_amount_of_sectors(
	     info_handle->input_handle,
	     &amount_of_sectors ) == 1 )
	{
		fprintf(
		 stream,
		 "\tAmount of sectors:\t%" PRIu32 "\n",
		 amount_of_sectors );
	}
#endif
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of sectors.",
		 function );

		result = -1;
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_media_size(
	     info_handle->input_handle,
	     &media_size,
	     error ) == 1 )
#else
	if( libewf_get_media_size(
	     info_handle->input_handle,
	     &media_size ) == 1 )
#endif
	{
		if( byte_size_string_create(
		     media_size_string,
		     16,
		     media_size,
		     BYTE_SIZE_STRING_UNIT_MEBIBYTE,
		     NULL ) == 1 )
		{
			fprintf(
			 stream,
			 "\tMedia size:\t\t%" PRIs_LIBSYSTEM " (%" PRIu64 " bytes)\n",
			 media_size_string,
			 media_size );
		}
		else
		{
			fprintf(
			 stream,
			 "\tMedia size:\t\t%" PRIu64 " bytes\n",
			 media_size );
		}
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		result = -1;
	}
	fprintf(
	 stream,
	 "\n" );

	return( result );
}

/* Print the hash values to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_hash_values_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
	char hash_value_identifier[ INFO_HANDLE_VALUE_IDENTIFIER_SIZE ];
	libsystem_character_t hash_value[ INFO_HANDLE_VALUE_SIZE ];

	static char *function             = "info_handle_hash_values_fprint";
	size_t hash_value_identifier_size = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;
	size_t hash_value_size            = INFO_HANDLE_VALUE_SIZE;
	uint32_t amount_of_values         = 0;
	uint32_t hash_value_iterator      = 0;
	uint8_t print_header              = 1;
	int result                        = 1;

#if defined( USE_LIBEWF_GET_MD5_HASH )
	digest_hash_t md5_hash[ DIGEST_HASH_SIZE_MD5 ];

	libsystem_character_t *stored_md5_hash_string = NULL;
#endif

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
#if defined( USE_LIBEWF_GET_MD5_HASH )
#if defined( HAVE_V2_API )
	result = libewf_handle_get_md5_hash(
		  handle,
		  md5_hash,
		  DIGEST_HASH_SIZE_MD5,
	          error );
#else
	result = libewf_get_md5_hash(
		  handle,
		  md5_hash,
		  DIGEST_HASH_SIZE_MD5 );
#endif

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MD5 hash.",
		 function );

		return;
	}
	else if( result == 1 )
	{
		stored_md5_hash_string = (libsystem_character_t *) memory_allocate(
		                                                    sizeof( libsystem_character_t ) * DIGEST_HASH_STRING_SIZE_MD5 );

		if( stored_md5_hash_string == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set MD5 hash string.",
			 function );

			memory_free(
			 stored_md5_hash_string );

			return;
		}
		if( print_header != 0 )
		{
			fprintf(
			 stream,
			 "Digest hash information\n" );

			print_header = 0;
		}
		fprintf(
		 stream,
		 "\tMD5:\t\t\t%" PRIs_LIBSYSTEM "\n",
		 stored_md5_hash_string );

		memory_free(
		 stored_md5_hash_string );
	}
#endif
#if !defined( HAVE_V2_API )
	if( libewf_parse_hash_values(
	     info_handle->input_handle ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to parse hash values.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_V2_API )
	if( libewf_handle_get_amount_of_hash_values(
	     info_handle->input_handle,
	     &amount_of_values,
	     error ) == -1 )
#else
	if( libewf_get_amount_of_hash_values(
	     info_handle->input_handle,
	     &amount_of_values ) == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of hash values.",
		 function );

		return( -1 );
	}
	for( hash_value_iterator = 0;
	     hash_value_iterator < amount_of_values;
	     hash_value_iterator++ )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_get_hash_value_identifier_size(
		     info_handle->input_handle,
		     hash_value_iterator,
		     &hash_value_identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value identifier size for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
		if( hash_value_identifier_size > INFO_HANDLE_VALUE_IDENTIFIER_SIZE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: hash value identifier size value out of range for index: %" PRIu32 ".",
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
#else
		hash_value_identifier_size = INFO_HANDLE_VALUE_IDENTIFIER_SIZE;

		if( libewf_get_hash_value_identifier(
		     info_handle->input_handle,
		     hash_value_iterator,
		     hash_value_identifier,
		     hash_value_identifier_size ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value identifier for index: %" PRIu32 ".",
			 function,
			 hash_value_iterator );

			result = -1;

			continue;
		}
#if !defined( HAVE_V2_API )
		hash_value_identifier_size = 1 + narrow_string_length(
		                                  hash_value_identifier );
#endif
#if defined( USE_LIBEWF_GET_MD5_HASH )
		if( narrow_string_compare(
		     hash_value_identifier,
		     "MD5",
		     3 ) == 0 )
		{
			continue;
		}
#endif
		if( info_handle_get_hash_value(
		     info_handle,
		     hash_value_identifier,
		     hash_value_identifier_size - 1,
		     hash_value,
		     hash_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the hash value for identifier: %s.",
			 function,
			 hash_value_identifier );

			result = -1;
		}
		else
		{
			if( print_header != 0 )
			{
				fprintf(
				 stream,
				 "Digest hash information\n" );

				print_header = 0;
			}
			fprintf(
			 stream,
			 "\t%s:\t\t\t%" PRIs_LIBSYSTEM "\n",
			 hash_value_identifier,
			 hash_value );
		}
	}
	if( print_header == 0 )
	{
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

/* Print the acquiry errors to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_acquiry_errors_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
	static char *function      = "info_handle_acquiry_errors_fprint";
	uint64_t start_sector      = 0;
#if defined( HAVE_V2_API )
	uint64_t amount_of_sectors = 0;
#else
	uint32_t amount_of_sectors = 0;
#endif
	uint32_t amount_of_errors  = 0;
	uint32_t error_iterator    = 0;
	int result                 = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_amount_of_acquiry_errors(
	     info_handle->input_handle,
	     &amount_of_errors,
	     error ) == -1 )
#else
	if( libewf_get_amount_of_acquiry_errors(
	     info_handle->input_handle,
	     &amount_of_errors ) == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of acquiry errors.",
		 function );

		return( -1 );
	}
	if( amount_of_errors > 0 )
	{
		fprintf(
		 stream,
		 "Read errors during acquiry:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %" PRIu32 "\n",
		 amount_of_errors );
		
		for( error_iterator = 0; error_iterator < amount_of_errors; error_iterator++ )
		{
#if defined( HAVE_V2_API )
			if( libewf_handle_get_acquiry_error(
			     info_handle->input_handle,
			     error_iterator,
			     &start_sector,
			     &amount_of_sectors,
			     error ) != 1 )
#else
			if( libewf_get_acquiry_error(
			     info_handle->input_handle,
			     error_iterator,
			     (off64_t *) &start_sector,
			     &amount_of_sectors ) != 1 )
#endif
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the acquiry error: %" PRIu32 ".",
				 function,
				 error_iterator );

				start_sector      = 0;
				amount_of_sectors = 0;

				result = -1;
			}
#if defined( HAVE_V2_API )
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu64 "\n",
			 start_sector,
			 start_sector + amount_of_sectors,
			 amount_of_sectors );
#else
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n",
			 start_sector,
			 start_sector + amount_of_sectors,
			 amount_of_sectors );
#endif
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}

/* Print the sessions to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_sessions_fprint(
     info_handle_t *info_handle,
     FILE *stream,
     liberror_error_t **error )
{
	static char *function       = "info_handle_sessions_fprint";
	uint64_t start_sector       = 0;
#if defined( HAVE_V2_API )
	uint64_t amount_of_sectors  = 0;
#else
	uint32_t amount_of_sectors  = 0;
#endif
	uint32_t amount_of_sessions = 0;
	uint32_t session_iterator   = 0;
	int result                  = 1;

	if( info_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_get_amount_of_sessions(
	     info_handle->input_handle,
	     &amount_of_sessions,
	     error ) == -1 )
#else
	if( libewf_get_amount_of_sessions(
	     info_handle->input_handle,
	     &amount_of_sessions ) == -1 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the amount of sessions.",
		 function );

		return( -1 );
	}
	if( amount_of_sessions > 0 )
	{
		fprintf(
		 stream,
		 "Sessions:\n" );
		fprintf(
		 stream,
		 "\ttotal amount: %" PRIu32 "\n",
		 amount_of_sessions );

		for( session_iterator = 0; session_iterator < amount_of_sessions; session_iterator++ )
		{
#if defined( HAVE_V2_API )
			if( libewf_handle_get_session(
			     info_handle->input_handle,
			     session_iterator,
			     &start_sector,
			     &amount_of_sectors,
			     error ) != 1 )
#else
			if( libewf_get_session(
			     info_handle->input_handle,
			     session_iterator,
			     (off64_t *) &start_sector,
			     &amount_of_sectors ) != 1 )
#endif
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the session: %" PRIu32 ".",
				 function,
				 session_iterator );

				start_sector      = 0;
				amount_of_sectors = 0;

				result = -1;
			}
#if defined( HAVE_V2_API )
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu64 "\n",
			 start_sector,
			 start_sector + amount_of_sectors,
			 amount_of_sectors );
#else
			fprintf(
			 stream,
			 "\tat sector(s): %" PRIu64 " - %" PRIu64 " amount: %" PRIu32 "\n",
			 start_sector,
			 start_sector + amount_of_sectors,
			 amount_of_sectors );
#endif
		}
		fprintf(
		 stream,
		 "\n" );
	}
	return( result );
}


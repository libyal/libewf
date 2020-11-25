/*
 * Case data functions
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
#include <types.h>

#include "libewf_case_data.h"
#include "libewf_definitions.h"
#include "libewf_header_values.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_media_values.h"
#include "libewf_value_table.h"

/* Generate an UTF-8 encoded case data string
 * Sets utf8_string and utf8_string_size
 * Returns 1 if successful or -1 on error
 */
int libewf_case_data_generate_utf8_string(
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t format,
     libcerror_error_t **error )
{
	uint8_t *generated_acquiry_date               = NULL;
	uint8_t *generated_system_date                = NULL;
	uint8_t *safe_utf8_string                     = NULL;
	static char *function                         = "libewf_case_data_generate_utf8_string";
	const char *newline_string                    = "\n";
	size_t acquiry_date_string_length             = 0;
	size_t acquiry_operating_system_string_length = 0;
	size_t acquiry_software_version_string_length = 0;
	size_t case_number_string_length              = 0;
	size_t description_string_length              = 0;
	size_t error_granularity_string_length        = 0;
	size_t evidence_number_string_length          = 0;
	size_t examiner_name_string_length            = 0;
	size_t generated_acquiry_date_size            = 0;
	size_t generated_system_date_size             = 0;
	size_t newline_string_length                  = 1;
	size_t notes_string_length                    = 0;
	size_t number_of_chunks_string_length         = 0;
	size_t safe_utf8_string_size                  = 0;
	size_t sectors_per_chunk_string_length        = 0;
	size_t system_date_string_length              = 0;
	size_t utf8_string_index                      = 0;
	uint64_t value_64bit                          = 0;
	int number_of_characters                      = 0;
	int number_of_tabs                            = 0;
	int result                                    = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( *utf8_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: UTF-8 string already set.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( format != LIBEWF_FORMAT_V2_ENCASE7 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format.",
		 function );

		return( -1 );
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "description",
	     12,
	     &description_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: description length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "case_number",
	     12,
	     &case_number_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: case_number length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "evidence_number",
	     16,
	     &evidence_number_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: evidence_number length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "examiner_name",
	     14,
	     &examiner_name_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: examiner_name length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "notes",
	     6,
	     &notes_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: notes length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "acquiry_software_version",
	     25,
	     &acquiry_software_version_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: acquiry_software_version length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "acquiry_operating_system",
	     25,
	     &acquiry_operating_system_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: acquiry_operating_system length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "acquiry_date",
	     13,
	     &acquiry_date_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: acquiry_date length.",
		 function );

		goto on_error;
	}
	if( acquiry_date_string_length == 0 )
	{
		result = libewf_generate_date_header2_value(
			  timestamp,
			  &generated_acquiry_date,
			  &generated_acquiry_date_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to generate acquiry date header value.",
			 function );

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			acquiry_date_string_length = narrow_string_length(
			                              (char *) generated_acquiry_date );
		}
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "system_date",
	     12,
	     &system_date_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: system_date length.",
		 function );

		goto on_error;
	}
	if( system_date_string_length == 0 )
	{
		result = libewf_generate_date_header2_value(
			  timestamp,
			  &generated_system_date,
			  &generated_system_date_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to generate system date header value.",
			 function );

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			 system_date_string_length = narrow_string_length(
			                              (char *) generated_system_date );
		}
	}
	/* Determine the string size
	 * Reserve space for:
	 * 1 <newline>
	 * main <newline>
	 */
	safe_utf8_string_size = 5 + ( 2 * newline_string_length );

	/* Reserve space for:
	 * nm <tab> cn <tab> en <tab> ex <tab> nt <tab> av <tab> os <tab> tt <tab> at <tab> tb <tab> cp <tab> sb <tab> gr <tab> wb <newline>
	 */
	number_of_characters = 28;
	number_of_tabs       = 13;

	safe_utf8_string_size += number_of_characters + number_of_tabs + newline_string_length;

	/* Reserve space for the values
	 */
	safe_utf8_string_size += description_string_length
	                       + case_number_string_length
	                       + evidence_number_string_length
	                       + examiner_name_string_length
	                       + notes_string_length
	                       + acquiry_software_version_string_length
	                       + acquiry_operating_system_string_length
	                       + acquiry_date_string_length
	                       + system_date_string_length;

	/* If we do a streamed write reserve space for the final number of chunks
	 */
	if( media_values->media_size == 0 )
	{
		value_64bit   = 1;
		value_64bit <<= 63;
		value_64bit  -= 1;
	}
	else
	{
		value_64bit = media_values->number_of_chunks;
	}
	if( libfvalue_string_size_from_integer(
	     &number_of_chunks_string_length,
	     value_64bit,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string size of number of chunks.",
		 function );

		goto on_error;
	}
	if( number_of_chunks_string_length > 0 )
	{
		number_of_chunks_string_length -= 1;

		safe_utf8_string_size += number_of_chunks_string_length;
	}
/* TODO: add support for the compression method: cp */

	if( libfvalue_string_size_from_integer(
	     &sectors_per_chunk_string_length,
	     media_values->sectors_per_chunk,
	     32,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string size of sectors per chunk.",
		 function );

		goto on_error;
	}
	if( sectors_per_chunk_string_length > 0 )
	{
		sectors_per_chunk_string_length -= 1;

		safe_utf8_string_size += sectors_per_chunk_string_length;
	}
	if( libfvalue_string_size_from_integer(
	     &error_granularity_string_length,
	     media_values->error_granularity,
	     32,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string size of error granularity.",
		 function );

		goto on_error;
	}
	if( error_granularity_string_length > 0 )
	{
		error_granularity_string_length -= 1;

		safe_utf8_string_size += error_granularity_string_length;
	}
/* TODO: add support for the write blocker type: wb */

	/* Reserve space for the tabs and 2 newlines
	 */
	safe_utf8_string_size += number_of_tabs + ( 2 * newline_string_length );

	/* Reserve space for the end-of-string character
	 */
	safe_utf8_string_size += 1;

	if( safe_utf8_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		goto on_error;
	}
	/* Determine the string
	 */
	safe_utf8_string = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * safe_utf8_string_size );

	if( safe_utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '1';

	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'm';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'a';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'i';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';

	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'm';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'c';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'e';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'e';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'x';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'a';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'v';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'o';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'a';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'b';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'c';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'p';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'b';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'g';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'r';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'w';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'b';

	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	if( description_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "description",
		     12,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: case_number to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( case_number_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "case_number",
		     12,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: case_number to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( evidence_number_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "evidence_number",
		     16,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: evidence_number to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( examiner_name_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "examiner_name",
		     14,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: examiner_name to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( notes_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "notes",
		     6,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: notes to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( acquiry_software_version_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "acquiry_software_version",
		     25,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: acquiry_software_version to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( acquiry_operating_system_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "acquiry_operating_system",
		     25,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: acquiry_operating_system to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( generated_acquiry_date == NULL )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "acquiry_date",
		     13,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: acquiry_date to string.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( narrow_string_copy(
		     (char *) &( safe_utf8_string[ utf8_string_index ] ),
		     (char *) generated_acquiry_date,
		     acquiry_date_string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy generated acquiry date string.",
			 function );

			goto on_error;
		}
		utf8_string_index += acquiry_date_string_length;

		memory_free(
		 generated_acquiry_date );

		generated_acquiry_date = NULL;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( generated_system_date == NULL )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "system_date",
		     12,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: system_date to string.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( narrow_string_copy(
		     (char *) &( safe_utf8_string[ utf8_string_index ] ),
		     (char *) generated_system_date,
		     system_date_string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy generated system date string.",
			 function );

			goto on_error;
		}
		utf8_string_index += system_date_string_length;

		memory_free(
		 generated_system_date );

		generated_system_date = NULL;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( number_of_chunks_string_length > 0 )
	{
		/* If we do a streamed write reserve space for the final number of chunks
		 */
		if( media_values->media_size == 0 )
		{
			value_64bit   = 1;
			value_64bit <<= 63;
			value_64bit  -= 1;
		}
		else
		{
			value_64bit = media_values->number_of_chunks;
		}
		if( libfvalue_utf8_string_with_index_copy_from_integer(
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     value_64bit,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy number of chunks to string.",
			 function );

			goto on_error;
		}
		utf8_string_index--;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

/* TODO: add support for the compression method: cp */

	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( sectors_per_chunk_string_length > 0 )
	{
		if( libfvalue_utf8_string_with_index_copy_from_integer(
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     media_values->sectors_per_chunk,
		     32,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy sectors per chunk to string.",
			 function );

			goto on_error;
		}
		utf8_string_index--;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( error_granularity_string_length > 0 )
	{
		if( libfvalue_utf8_string_with_index_copy_from_integer(
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     media_values->error_granularity,
		     32,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy error granularity to string.",
			 function );

			goto on_error;
		}
		utf8_string_index--;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

/* TODO: add support for the write blocker type: wb */

	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = 0;

	*utf8_string      = safe_utf8_string;
	*utf8_string_size = safe_utf8_string_size;

	return( 1 );

on_error:
	if( generated_acquiry_date != NULL )
	{
		memory_free(
		 generated_acquiry_date );
	}
	if( generated_system_date != NULL )
	{
		memory_free(
		 generated_system_date );
	}
	if( safe_utf8_string != NULL )
	{
		memory_free(
		 safe_utf8_string );
	}
	return( -1 );
}

/* Generate case data
 * Sets case_data and case_data_size
 * Returns 1 if successful or -1 on error
 */
int libewf_case_data_generate(
     uint8_t **case_data,
     size_t *case_data_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t format,
     libcerror_error_t **error )
{
	uint8_t *safe_case_data    = NULL;
	uint8_t *utf8_string       = NULL;
	static char *function      = "libewf_case_data_generate";
	size_t safe_case_data_size = 0;
	size_t utf8_string_size    = 0;

	if( case_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid case data.",
		 function );

		return( -1 );
	}
	if( *case_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: case data already created.",
		 function );

		return( -1 );
	}
	if( case_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid case data size.",
		 function );

		return( -1 );
	}
	if( libewf_case_data_generate_utf8_string(
	     &utf8_string,
	     &utf8_string_size,
	     media_values,
	     header_values,
	     timestamp,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create UTF-8 case data string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: case data string:\n%s",
		 function,
		 utf8_string );
	}
#endif
	if( libuna_utf16_stream_size_from_utf8(
	     utf8_string,
	     utf8_string_size,
	     &safe_case_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine case data size.",
		 function );

		goto on_error;
	}
	if( ( safe_case_data_size == 0 )
	 || ( safe_case_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid case data size value out of bounds.",
		 function );

		goto on_error;
	}
	safe_case_data = (uint8_t *) memory_allocate(
	                              sizeof( uint8_t ) * safe_case_data_size );

	if( safe_case_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create case data.",
		 function );

		goto on_error;
	}
	if( libuna_utf16_stream_copy_from_utf8(
	     safe_case_data,
	     safe_case_data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set case data.",
		 function );

		goto on_error;
	}
	memory_free(
	 utf8_string );

	*case_data      = safe_case_data;
	*case_data_size = safe_case_data_size;

	return( 1 );

on_error:
	if( utf8_string != NULL )
	{
		memory_free(
		 utf8_string );
	}
	if( safe_case_data != NULL )
	{
		memory_free(
		 safe_case_data );
	}
	return( -1 );
}

/* Parses an UTF-8 encoded case data string
 * Returns 1 if successful or -1 on error
 */
int libewf_case_data_parse_utf8_string(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     uint8_t *format,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *lines  = NULL;
	libfvalue_split_utf8_string_t *types  = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_case_data_parse_utf8_string";
	size_t line_string_size               = 0;
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	int number_of_lines                   = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     utf8_string,
	     utf8_string_size,
	     (uint8_t) '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split UTF-8 string into lines.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     lines,
	     &number_of_lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of lines",
		 function );

		goto on_error;
	}
	if( number_of_lines > 0 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     0,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 0.",
			 function );

			goto on_error;
		}
		if( line_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 0.",
			 function );

			goto on_error;
		}
		if( ( line_string == NULL )
		 || ( line_string_size < 2 )
		 || ( line_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 0.",
			 function );

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if( line_string[ line_string_size - 2 ] == (uint8_t) '\r' )
		{
			line_string[ line_string_size - 2 ] = 0;

			line_string_size -= 1;
		}
		if( line_string_size != 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 0.",
			 function );

			goto on_error;
		}
		if( line_string[ 0 ] != (uint8_t) '1' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 0.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     1,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 1.",
			 function );

			goto on_error;
		}
		if( line_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 1.",
			 function );

			goto on_error;
		}
		if( ( line_string == NULL )
		 || ( line_string_size < 5 )
		 || ( line_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 1.",
			 function );

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if( line_string[ line_string_size - 2 ] == (uint8_t) '\r' )
		{
			line_string[ line_string_size - 2 ] = 0;

			line_string_size -= 1;
		}
		if( line_string_size != 5 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 1.",
			 function );

			goto on_error;
		}
		if( ( line_string[ 0 ] != (uint8_t) 'm' )
		 || ( line_string[ 1 ] != (uint8_t) 'a' )
		 || ( line_string[ 2 ] != (uint8_t) 'i' )
		 || ( line_string[ 3 ] != (uint8_t) 'n' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 1.",
			 function );

			goto on_error;
		}
		*format = LIBEWF_FORMAT_V2_ENCASE7;

		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     2,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 2.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_split(
		     line_string,
		     line_string_size,
		     (uint8_t) '\t',
		     &types,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split case data string into types.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_number_of_segments(
		     types,
		     &number_of_types,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of types",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     3,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 3.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_split(
		     line_string,
		     line_string_size,
		     (uint8_t) '\t',
		     &values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split case data string into values.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_number_of_segments(
		     values,
		     &number_of_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of values",
			 function );

			goto on_error;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( number_of_types != number_of_values )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
			 	"%s: mismatch in number of types and values.\n",
				 function );
			}
		}
#endif
		for( value_index = 0;
		     value_index < number_of_types;
		     value_index++ )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     types,
			     value_index,
			     &type_string,
			     &type_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve type string: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( value_index >= number_of_values )
			{
				value_string      = NULL;
				value_string_size = 0;
			}
			else if( libfvalue_split_utf8_string_get_segment_by_index(
			          values,
			          value_index,
			          &value_string,
			          &value_string_size,
			          error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( libewf_case_data_parse_utf8_string_value(
			     type_string,
			     type_string_size,
			     value_string,
			     value_string_size,
			     value_index,
			     media_values,
			     header_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to parse UTF-8 string value: %d.",
				 function,
				 value_index );

				goto on_error;
			}
		}
		if( libfvalue_split_utf8_string_free(
		     &values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split values.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_free(
		     &types,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split types.",
			 function );

			goto on_error;
		}
	}
	if( libfvalue_split_utf8_string_free(
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split lines.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"\n" );
	}
#endif
	return( 1 );

on_error:
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lines != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &lines,
		 NULL );
	}
	return( -1 );
}

/* Parses an UTF-8 encoded case data string value
 * Returns 1 if successful or -1 on error
 */
int libewf_case_data_parse_utf8_string_value(
     uint8_t *type_string,
     size_t type_string_size,
     uint8_t *value_string,
     size_t value_string_size,
     int value_index,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error )
{
	uint8_t *date_time_values_string    = NULL;
	uint8_t *identifier                 = NULL;
	static char *function               = "libewf_case_data_parse_utf8_string_value";
	size_t date_time_values_string_size = 0;
	size_t identifier_size              = 0;
	uint64_t value_64bit                = 0;

	if( ( type_string == NULL )
	 || ( type_string_size < 2 )
	 || ( type_string[ 0 ] == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing type string: %d.",
		 function,
		 value_index );

		goto on_error;
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( header_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header values.",
		 function );

		return( -1 );
	}
	/* Remove trailing carriage return
	 */
	if( type_string[ type_string_size - 2 ] == (uint8_t) '\r' )
	{
		type_string[ type_string_size - 2 ] = 0;

		type_string_size -= 1;
	}
	if( ( value_string == NULL )
	 || ( value_string_size < 2 )
	 || ( value_string[ 0 ] == 0 ) )
	{
		value_string      = NULL;
		value_string_size = 0;
	}
	/* Remove trailing carriage return
	 */
	else if( value_string[ value_string_size - 2 ] == (uint8_t) '\r' )
	{
		value_string[ value_string_size - 2 ] = 0;

		value_string_size -= 1;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type: %s with value: %s.\n",
		 function,
		 (char *) type_string,
		 (char *) value_string );
	}
#endif
	/* Ignore empty values
	 */
	if( value_string == NULL )
	{
		return( 1 );
	}
	identifier      = NULL;
	identifier_size = 0;

	if( type_string_size == 3 )
	{
		if( ( type_string[ 0 ] == (uint8_t) 'a' )
		 && ( type_string[ 1 ] == (uint8_t) 'v' ) )
		{
			identifier      = (uint8_t *) "acquiry_software_version";
			identifier_size = 25;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'a' )
		      && ( type_string[ 1 ] == (uint8_t) 't' ) )
		{
			if( libewf_convert_date_header2_value(
			     value_string,
			     value_string_size,
			     &date_time_values_string,
			     &date_time_values_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to create date time values string.",
				 function );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );
			}
			if( date_time_values_string != NULL )
			{
				value_string      = date_time_values_string;
				value_string_size = 1 + narrow_string_length(
			                         (char *) date_time_values_string );

				identifier      = (uint8_t *) "acquiry_date";
				identifier_size = 13;
			}
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'c' )
		      && ( type_string[ 1 ] == (uint8_t) 'n' ) )
		{
			identifier      = (uint8_t *) "case_number";
			identifier_size = 12;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'c' )
		      && ( type_string[ 1 ] == (uint8_t) 'p' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     32,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set compression method.",
				 function );

				goto on_error;
			}
			if( value_64bit == 0 )
			{
				value_string      = (uint8_t *) "none";
				value_string_size = 5;
			}
			else if( value_64bit == 1 )
			{
				value_string      = (uint8_t *) "deflate";
				value_string_size = 8;
			}
			else if( value_64bit == 2 )
			{
				value_string      = (uint8_t *) "bzip2";
				value_string_size = 6;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else
			{
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					"%s: unsupported compression method: %" PRIu64 ".\n",
					 function,
					 value_64bit );
				}
			}
#endif
			identifier      = (uint8_t *) "compression_method";
			identifier_size = 19;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'e' )
		      && ( type_string[ 1 ] == (uint8_t) 'n' ) )
		{
			identifier      = (uint8_t *) "evidence_number";
			identifier_size = 16;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'e' )
		      && ( type_string[ 1 ] == (uint8_t) 'x' ) )
		{
			identifier      = (uint8_t *) "examiner_name";
			identifier_size = 14;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'g' )
		      && ( type_string[ 1 ] == (uint8_t) 'r' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     32,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set error granularity.",
				 function );

				goto on_error;
			}
			media_values->error_granularity = (uint32_t) value_64bit;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'n' )
		      && ( type_string[ 1 ] == (uint8_t) 'm' ) )
		{
			identifier      = (uint8_t *) "description";
			identifier_size = 12;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'n' )
		      && ( type_string[ 1 ] == (uint8_t) 't' ) )
		{
			identifier      = (uint8_t *) "notes";
			identifier_size = 6;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'o' )
		      && ( type_string[ 1 ] == (uint8_t) 's' ) )
		{
			identifier      = (uint8_t *) "acquiry_operating_system";
			identifier_size = 25;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 's' )
		      && ( type_string[ 1 ] == (uint8_t) 'b' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     32,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set sectors per chunk.",
				 function );

				goto on_error;
			}
			media_values->sectors_per_chunk = (uint32_t) value_64bit;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 't' )
		      && ( type_string[ 1 ] == (uint8_t) 'b' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     64,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set number of chunks.",
				 function );

				goto on_error;
			}
			media_values->number_of_chunks = value_64bit;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 't' )
		      && ( type_string[ 1 ] == (uint8_t) 't' ) )
		{
			if( libewf_convert_date_header2_value(
			     value_string,
			     value_string_size,
			     &date_time_values_string,
			     &date_time_values_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to create date time values string.",
				 function );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );
			}
			if( date_time_values_string != NULL )
			{
				value_string      = date_time_values_string;
				value_string_size = 1 + narrow_string_length(
				                         (char *) date_time_values_string );

				identifier      = (uint8_t *) "system_date";
				identifier_size = 12;
			}
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'w' )
		      && ( type_string[ 1 ] == (uint8_t) 'b' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     32,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set write blocker type.",
				 function );

				goto on_error;
			}
			/* The EnCase specification indicates these are flags and not an enumeration
			 */
			if( ( value_64bit & 0x00000001 ) != 0 )
			{
				media_values->media_flags |= LIBEWF_MEDIA_FLAG_FASTBLOC;
			}
			if( ( value_64bit & 0x00000002 ) != 0 )
			{
				media_values->media_flags |= LIBEWF_MEDIA_FLAG_TABLEAU;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( ( value_64bit & ~( 0x00000003 ) ) != 0 )
			{
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					"%s: unsupported write blocker type.\n",
					 function );
				}
			}
#endif
		}
	}
	if( identifier != NULL )
	{
		if( libewf_value_table_set_value_by_identifier(
		     header_values,
		     identifier,
		     identifier_size,
		     value_string,
		     value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
	}
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );

		date_time_values_string = NULL;
	}
	return( 1 );

on_error:
	if( date_time_values_string != NULL )
	{
		memory_free(
		 date_time_values_string );
	}
	return( -1 );
}

/* Parses EWF version 2 case data
 * Returns 1 if successful or -1 on error
 */
int libewf_case_data_parse(
     const uint8_t *case_data,
     size_t case_data_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     uint8_t *format,
     libcerror_error_t **error )
{
	uint8_t *utf8_string    = NULL;
	static char *function   = "libewf_case_data_parse";
	size_t utf8_string_size = 0;

	if( case_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid case data.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     case_data,
	     case_data_size,
	     0,
	     &utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 string size.",
		 function );

		goto on_error;
	}
	if( ( utf8_string_size == 0 )
	 || ( utf8_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		goto on_error;
	}
	utf8_string = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     case_data,
	     case_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy case data to UTF-8 string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: case data string:\n%s",
		 function,
		 utf8_string );
	}
#endif
	if( libewf_case_data_parse_utf8_string(
	     utf8_string,
	     utf8_string_size,
	     media_values,
	     header_values,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse UTF-8 string.",
		 function );

		goto on_error;
	}
	memory_free(
	 utf8_string );

	return( 1 );

on_error:
	if( utf8_string != NULL )
	{
		memory_free(
		 utf8_string );
	}
	return( -1 );
}


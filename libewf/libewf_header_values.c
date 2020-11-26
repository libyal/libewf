/*
 * Header values functions
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


#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_date_time.h"
#include "libewf_date_time_values.h"
#include "libewf_definitions.h"
#include "libewf_header_values.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_value_table.h"

/* Creates header values
 * Make sure the value header_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_initialize(
     libfvalue_table_t **header_values,
     libcerror_error_t **error )
{
	static char *function = "libewf_header_values_initialize";

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
	if( libfvalue_table_initialize(
	     header_values,
	     LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header values table.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
	     (uint8_t *) "case_number",
	     12,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: case_number.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION,
	     (uint8_t *) "description",
	     12,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: description.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME,
	     (uint8_t *) "examiner_name",
	     14,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: examiner_name.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER,
	     (uint8_t *) "evidence_number",
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: evidence_number.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_NOTES,
	     (uint8_t *) "notes",
	     6,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: notes.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE,
	     (uint8_t *) "acquiry_date",
	     13,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry_date.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE,
	     (uint8_t *) "system_date",
	     12,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: system_date.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION,
	     (uint8_t *) "acquiry_software_version",
	     25,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry_software_version.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM,
	     (uint8_t *) "acquiry_operating_system",
	     25,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry_operating_system.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_PASSWORD,
	     (uint8_t *) "password",
	     9,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: password.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE,
	     (uint8_t *) "compression_level",
	     18,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: compression_level.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_MODEL,
	     (uint8_t *) "model",
	     6,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: model.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER,
	     (uint8_t *) "serial_number",
	     14,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: serial_number.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_DEVICE_LABEL,
	     (uint8_t *) "device_label",
	     13,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: device_label.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_PROCESS_IDENTIFIER,
	     (uint8_t *) "process_identifier",
	     19,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: process_identifier.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC,
	     (uint8_t *) "unknown_dc",
	     11,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: unknown_dc.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EXTENTS,
	     (uint8_t *) "extents",
	     8,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: extents.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *header_values != NULL )
	{
		libfvalue_table_free(
		 header_values,
		 NULL );
	}
	return( -1 );
}

/* Convert a header value into a date time values string
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header_value(
     const uint8_t *header_value,
     size_t header_value_size,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error )
{
	struct tm time_elements;

	libfvalue_split_utf8_string_t *date_time_values = NULL;
	uint8_t *date_time_value_string                 = NULL;
	static char *function                           = "libewf_convert_date_header_value";
	size_t date_time_value_string_size              = 0;
	time_t timestamp                                = 0;
	int number_of_date_time_values                  = 0;

	if( date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     header_value,
	     header_value_size,
	     (uint8_t) ' ',
	     &date_time_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header value.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     date_time_values,
	     &number_of_date_time_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of date time values.",
		 function );

		goto on_error;
	}
	if( number_of_date_time_values != 6 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number date time values in header value.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     0,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 0.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_year_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy year value.",
		 function );

		goto on_error;
	}
	/* Set the month
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     1,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 1.",
		 function );

		goto on_error;
	}
	if( ( date_time_value_string_size != 2 )
	 && ( date_time_value_string_size != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: 1.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_month_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy month value.",
		 function );

		goto on_error;
	}
	/* Set the day of the month
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     2,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 2.",
		 function );

		goto on_error;
	}
	if( ( date_time_value_string_size != 2 )
	 && ( date_time_value_string_size != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: 2.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_day_of_month_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of month value.",
		 function );

		goto on_error;
	}
	/* Set the hours
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     3,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 3.",
		 function );

		goto on_error;
	}
	if( ( date_time_value_string_size != 2 )
	 && ( date_time_value_string_size != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: 3.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_hours_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hours value.",
		 function );

		goto on_error;
	}
	/* Set the minutes
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     4,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 4.",
		 function );

		goto on_error;
	}
	if( ( date_time_value_string_size != 2 )
	 && ( date_time_value_string_size != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: 4.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_minutes_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy minutes value.",
		 function );

		goto on_error;
	}
	/* Set the seconds
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     5,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 5.",
		 function );

		goto on_error;
	}
	if( ( date_time_value_string_size != 2 )
	 && ( date_time_value_string_size != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: 5.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_seconds_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy seconds value.",
		 function );

		goto on_error;
	}
	/* Set to ignore the daylight saving time
	 */
	time_elements.tm_isdst = -1;

	if( libfvalue_split_utf8_string_free(
	     &date_time_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split date time elements.",
		 function );

		goto on_error;
	}
	/* Create a timestamp
	 */
	timestamp = libewf_date_time_mktime(
	             &time_elements );

	if( timestamp == (time_t) -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create timestamp.",
		 function );

		goto on_error;
	}
	*date_time_values_string_size = 20;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		goto on_error;
	}
	if( libewf_date_time_values_copy_from_timestamp(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to convert timestamp into date time values string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( date_time_values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &date_time_values,
	         NULL );
	}
	if( *date_time_values_string != NULL )
	{
		memory_free(
		 *date_time_values_string );

		*date_time_values_string = NULL;
	}
	*date_time_values_string_size = 0;

	return( -1 );
}

/* Generates a date time values string within a header value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error )
{
	struct tm time_elements;

	static char *function                = "libewf_generate_date_header_value";
	size_t date_time_values_string_index = 0;

	if( date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libewf_date_time_localtime(
	     &timestamp,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create time elements.",
		 function );

		goto on_error;
	}
	if( ( time_elements.tm_year + 1900 ) > 10000 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported year value.",
		 function );

		goto on_error;
	}
	*date_time_values_string_size = 20;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		goto on_error;
	}
	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_year + 1900,
	     16,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy year to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index - 1 ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_mon + 1,
	     8,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy month to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index - 1 ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_mday,
	     8,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of month to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index - 1 ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_hour,
	     8,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hours to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index - 1 ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_min,
	     8,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy minutes to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index - 1 ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_sec,
	     8,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy seconds to date time values string.",
		 function );

		goto on_error;
	}
	/* The end-of-string character is set by libfvalue_utf8_string_with_index_copy_from_integer
	 */
	return( 1 );

on_error:
	if( *date_time_values_string != NULL )
	{
		memory_free(
		 *date_time_values_string );

		*date_time_values_string = NULL;
	}
	*date_time_values_string_size = 0;

	return( -1 );
}

/* Convert a header2 value into a date time values string
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header2_value(
     const uint8_t *header_value,
     size_t header_value_size,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_convert_date_header2_value";
	uint64_t value_64bit  = 0;

	if( date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_copy_to_integer(
	     header_value,
	     header_value_size - 1,
	     &value_64bit,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set timestamp.",
		 function );

		goto on_error;
	}
	*date_time_values_string_size = 20;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		goto on_error;
	}
	if( libewf_date_time_values_copy_from_timestamp(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     (time_t) value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to convert timestamp into date time values string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *date_time_values_string != NULL )
	{
		memory_free(
		 *date_time_values_string );

		*date_time_values_string = NULL;
	}
	*date_time_values_string_size = 0;

	return( -1 );
}

/* Generates a date time values string within a header2 value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header2_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error )
{
	static char *function                = "libewf_generate_date_header2_value";
	size_t date_time_values_string_index = 0;

	if( date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	*date_time_values_string_size = 11;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		goto on_error;
	}
	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) timestamp,
	     32,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy timestamp to date time values string.",
		 function );

		goto on_error;
	}
	/* The end-of-string character is set by libfvalue_utf8_string_with_index_copy_from_integer
	 */
	return( 1 );

on_error:
	if( *date_time_values_string != NULL )
	{
		memory_free(
		 *date_time_values_string );

		*date_time_values_string = NULL;
	}
	*date_time_values_string_size = 0;

	return( -1 );
}

/* Copies the header values from the source to the destination
 * Returns 1 if successful -1 on error
 */
int libewf_header_values_copy(
     libfvalue_table_t *destination_header_values,
     libfvalue_table_t *source_header_values,
     libcerror_error_t **error )
{
	libfvalue_value_t *destination_header_value = NULL;
	libfvalue_value_t *source_header_value      = NULL;
	uint8_t *identifier                         = NULL;
	static char *function                       = "libewf_header_values_copy";
	size_t identifier_size                      = 0;
	int header_value_index                      = 0;
	int number_of_header_values                 = 0;
	int result                                  = 0;

	if( destination_header_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination header values.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_number_of_values(
	     source_header_values,
	     &number_of_header_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of header values.",
		 function );

		return( -1 );
	}
	for( header_value_index = 0;
	     header_value_index < number_of_header_values;
	     header_value_index++ )
	{
		if( libfvalue_table_get_value_by_index(
		     source_header_values,
		     header_value_index,
		     &source_header_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %d.",
			 function,
			 header_value_index );

			goto on_error;
		}
		if( libfvalue_value_get_identifier(
		     source_header_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of header value: %d.",
			 function,
			 header_value_index );

			goto on_error;
		}
		/* Ignore header values without an identifier
		 */
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: missing identifier for header value: %d.\n",
				 function,
				 header_value_index );
			}
#endif
			continue;
		}
		/* Ignore the acquiry and system date
		 * They will be auto generated
		 */
		else if( ( identifier_size == 13 )
		      && ( narrow_string_compare(
		            (char *) identifier,
		            "acquiry_date",
		            12 ) == 0 ) )
		{
			continue;
		}
		else if( ( identifier_size == 12 )
		      && ( narrow_string_compare(
		            (char *) identifier,
		            "system_date",
		            11 ) == 0 ) )
		{
			continue;
		}
		/* Ignore empty values
		 */
		result = libfvalue_value_has_data(
		          source_header_value,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data of header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		else if( result == 0 )
		{
			continue;
		}
		if( libfvalue_value_clone(
		     &destination_header_value,
		     source_header_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable create destination header value.",
			 function );

			goto on_error;
		}
		if( libfvalue_table_set_value_by_index(
		     destination_header_values,
		     header_value_index,
		     destination_header_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %d.",
			 function,
			 header_value_index );

			goto on_error;
		}
		destination_header_value = NULL;
	}
	return( 1 );

on_error:
	if( destination_header_value != NULL )
	{
		libfvalue_value_free(
		 &destination_header_value,
		 NULL );
	}
	return( -1 );
}

/* Parses an UTF-8 encoded header string
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_utf8_header_string(
     libfvalue_table_t *header_values,
     const uint8_t *header_string,
     size_t header_string_size,
     uint8_t header_section_number,
     uint8_t *format,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *lines  = NULL;
	libfvalue_split_utf8_string_t *types  = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_header_values_parse_utf8_header_string";
	size_t line_string_size               = 0;
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	uint8_t acquiry_software_version      = 0;
	uint8_t number_of_sections            = 0;
	int number_of_lines                   = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( ( header_section_number != 1 )
	 && ( header_section_number != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header section number.",
		 function );

		goto on_error;
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
	     header_string,
	     header_string_size,
	     (uint8_t) '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header string into lines.",
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
		 "%s: unable to retrieve number of lines.",
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
		if( ( line_string[ 0 ] != (uint8_t) '1' )
		 && ( line_string[ 0 ] != (uint8_t) '3' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 0.",
			 function );

			goto on_error;
		}
		number_of_sections = line_string[ 0 ];

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
		if( header_section_number == 1 )
		{
			/* If the header string contains 3 object the version is at least linen5
			 * otherwise the version is at least EnCase1
			 */
			if( number_of_sections == (uint8_t) '3' )
			{
				*format = LIBEWF_FORMAT_LINEN5;
			}
			else
			{
				*format = LIBEWF_FORMAT_ENCASE1;
			}
		}
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
			 "%s: unable to split header values string into types.",
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
			 "%s: unable to retrieve number of types.",
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
			 "%s: unable to split header values string into values.",
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
			 "%s: unable to retrieve number of values.",
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
			if( libewf_header_values_parse_utf8_header_string_value(
			     header_values,
			     type_string,
			     type_string_size,
			     value_string,
			     value_string_size,
			     value_index,
			     header_section_number,
			     number_of_sections,
			     format,
			     &acquiry_software_version,
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

/* Parses an UTF-8 encoded header string value
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_utf8_header_string_value(
     libfvalue_table_t *header_values,
     uint8_t *type_string,
     size_t type_string_size,
     uint8_t *value_string,
     size_t value_string_size,
     int value_index,
     uint8_t header_section_number,
     uint8_t number_of_sections,
     uint8_t *format,
     uint8_t *acquiry_software_version,
     libcerror_error_t **error )
{
	uint8_t *date_time_values_string    = NULL;
	uint8_t *identifier                 = NULL;
	static char *function               = "libewf_header_values_parse_utf8_header_string_value";
	size_t date_time_values_string_size = 0;
	size_t identifier_size              = 0;
	uint8_t has_carriage_return         = 0;
	int result                          = 0;

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
	if( ( header_section_number != 1 )
	 && ( header_section_number != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header section number.",
		 function );

		goto on_error;
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
	if( acquiry_software_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid acquiry software version.",
		 function );

		return( -1 );
	}
	/* Remove trailing carriage return
	 */
	if( type_string[ type_string_size - 2 ] == (uint8_t) '\r' )
	{
		type_string[ type_string_size - 2 ] = 0;

		type_string_size -= 1;

		has_carriage_return = 1;
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
	identifier      = NULL;
	identifier_size = 0;

	if( type_string_size == 4 )
	{
		if( ( type_string[ 0 ] == (uint8_t) 'e' )
		 && ( type_string[ 1 ] == (uint8_t) 'x' )
		 && ( type_string[ 2 ] == (uint8_t) 't' ) )
		{
			identifier      = (uint8_t *) "extents";
			identifier_size = 8;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'p' )
		      && ( type_string[ 1 ] == (uint8_t) 'i' )
		      && ( type_string[ 2 ] == (uint8_t) 'd' ) )
		{
			identifier      = (uint8_t *) "process_identifier";
			identifier_size = 19;
		}
	}
	else if( type_string_size == 3 )
	{
		if( ( type_string[ 0 ] == (uint8_t) 'a' )
		 && ( type_string[ 1 ] == (uint8_t) 'v' ) )
		{
			identifier      = (uint8_t *) "acquiry_software_version";
			identifier_size = 25;

			if( value_index == 5 )
			{
				/* The linen5 header contains av on the 6th position
				 */
				if( header_section_number == 1 )
				{
					*format = LIBEWF_FORMAT_LINEN5;
				}
				else if( header_section_number == 2 )
				{
					/* The EnCase4 header2 contains av on the 6th position
					 * and the header2 consist of 1 sections
					 */
					if( number_of_sections == (uint8_t) '1' )
					{
						*format = LIBEWF_FORMAT_ENCASE4;
					}
					/* The EnCase5 header2 contains av on the 6th position
					 * and the header2 consist of 3 sections
					 */
					else if( number_of_sections == (uint8_t) '3' )
					{
						*format = LIBEWF_FORMAT_ENCASE5;
					}
				}
			}
			if( ( value_string != NULL )
			 && ( value_string_size > 1 ) )
			{
				*acquiry_software_version = value_string[ 0 ];
			}
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'd' )
		      && ( type_string[ 1 ] == (uint8_t) 'c' ) )
		{
			identifier      = (uint8_t *) "unknown_dc";
			identifier_size = 11;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'm' )
		      && ( type_string[ 1 ] == (uint8_t) 'd' ) )
		{
			identifier      = (uint8_t *) "model";
			identifier_size = 6;

			if( value_index == 5 )
			{
				/* The linen6 header contains md on the 6th position
				 */
				if( header_section_number == 1 )
				{
					*format = LIBEWF_FORMAT_LINEN6;
				}
				/* The EnCase6 header2 contains md on the 6th position
				 */
				else if( header_section_number == 2 )
				{
					*format = LIBEWF_FORMAT_ENCASE6;
				}
			}
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'o' )
		      && ( type_string[ 1 ] == (uint8_t) 'v' ) )
		{
			identifier      = (uint8_t *) "acquiry_operating_system";
			identifier_size = 25;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 's' )
		      && ( type_string[ 1 ] == (uint8_t) 'n' ) )
		{
			identifier      = (uint8_t *) "serial_number";
			identifier_size = 14;
		}
	}
	else if( type_string_size == 2 )
	{
		if( type_string[ 0 ] == (uint8_t) 'a' )
		{
			identifier      = (uint8_t *) "description";
			identifier_size = 12;
		}
		else if( type_string[ 0 ] == (uint8_t) 'c' )
		{
			identifier      = (uint8_t *) "case_number";
			identifier_size = 12;
		}
		else if( type_string[ 0 ] == (uint8_t) 'e' )
		{
			identifier      = (uint8_t *) "examiner_name";
			identifier_size = 14;
		}
		else if( type_string[ 0 ] == (uint8_t) 'l' )
		{
			identifier      = (uint8_t *) "device_label";
			identifier_size = 13;

			/* The linen7 header contains l
			 */
			if( header_section_number == 1 )
			{
				*format = LIBEWF_FORMAT_LINEN7;
			}
			/* The EnCase7 header2 contains l
			 */
			else if( header_section_number == 2 )
			{
				*format = LIBEWF_FORMAT_ENCASE7;
			}
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'm' )
		      || ( type_string[ 0 ] == (uint8_t) 'u' ) )
		{
			if( ( value_string == NULL )
			 || ( value_string_size == 0 )
			 || ( value_string[ 0 ] == 0 ) )
			{
				date_time_values_string = NULL;
				result                  = 0;
			}
			/* If the date time values string contains spaces it is in the old header
			 * format otherwise is in new header2 format
			 */
			else if( narrow_string_search_character(
				  (char *) value_string,
				  ' ',
				  value_string_size - 1 ) != NULL )
			{
				result = libewf_convert_date_header_value(
					  value_string,
					  value_string_size,
					  &date_time_values_string,
					  &date_time_values_string_size,
					  error );
			}
			else
			{
				result = libewf_convert_date_header2_value(
					  value_string,
					  value_string_size,
					  &date_time_values_string,
					  &date_time_values_string_size,
					  error );
			}
			if( result != 1 )
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
			/* The effective size of the date time values string is needed
			 */
			if( date_time_values_string != NULL )
			{
				value_string      = date_time_values_string;
				value_string_size = 1 + narrow_string_length(
							 (char *) date_time_values_string );

				if( type_string[ 0 ] == (uint8_t) 'm' )
				{
					identifier      = (uint8_t *) "acquiry_date";
					identifier_size = 13;
				}
				else if( type_string[ 0 ] == (uint8_t) 'u' )
				{
					identifier      = (uint8_t *) "system_date";
					identifier_size = 12;
				}
			}
		}
		else if( type_string[ 0 ] == (uint8_t) 'n' )
		{
			identifier      = (uint8_t *) "evidence_number";
			identifier_size = 16;
		}
		else if( type_string[ 0 ] == (uint8_t) 'p' )
		{
			if( ( value_string == NULL )
			 || ( value_string_size == 0 )
			 || ( value_string[ 0 ] == 0 ) )
			{
				/* Empty hash do nothing
				 */
			}
			else if( ( value_string_size == 2 )
			      && ( value_string[ 0 ] == (uint8_t) '0' ) )
			{
				/* Empty hash do nothing
				 */
			}
			else
			{
				identifier      = (uint8_t *) "password";
				identifier_size = 9;
			}
		}
		else if( type_string[ 0 ] == (uint8_t) 'r' )
		{
			identifier      = (uint8_t *) "compression_level";
			identifier_size = 18;

			if( header_section_number == 1 )
			{
				/* The EnCase1 header contains r on the 9th position
				 */
				if( value_index == 8 )
				{
					*format = LIBEWF_FORMAT_ENCASE1;
				}
				else if( value_index == 10 )
				{
					/* The EnCase2 and EnCase3 header contains r on the 11th position
					 * and uses \r\n as line ends. The only way to tell both version
					 * apart is to look at the acquiry software version
					 */
					if( has_carriage_return != 0 )
					{
						if( *acquiry_software_version == (uint8_t) '2' )
						{
							*format = LIBEWF_FORMAT_ENCASE2;
						}
						else if( *acquiry_software_version == (uint8_t) '3' )
						{
							*format = LIBEWF_FORMAT_ENCASE3;
						}
					}
					/* The FTK imager header contains r on the 11th position
					 * and uses \n as line ends
					 */
					else
					{
						*format = LIBEWF_FORMAT_FTK_IMAGER;
					}
				}
			}
		}
		else if( type_string[ 0 ] == (uint8_t) 't' )
		{
			identifier      = (uint8_t *) "notes";
			identifier_size = 6;
		}
	}
	/* Ignore empty values
	 */
	if( value_string == NULL )
	{
		return( 1 );
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

/* Parses EWF version 1 header for values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header(
     libfvalue_table_t *header_values,
     uint8_t *header,
     size_t header_size,
     int codepage,
     uint8_t *format,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_parse_header";
	size_t header_string_size = 0;

	if( libuna_utf8_string_size_from_byte_stream(
	     header,
	     header_size,
	     codepage,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header string size.",
		 function );

		goto on_error;
	}
	if( ( header_string_size == 0 )
	 || ( header_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid header string size value out of bounds.",
		 function );

		goto on_error;
	}
	header_string = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * header_string_size );

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_byte_stream(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to set header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_parse_utf8_header_string(
	     header_values,
	     header_string,
	     header_string_size,
	     1,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse header string.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Parses EWF version 1 header2 for values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header2(
     libfvalue_table_t *header_values,
     uint8_t *header2,
     size_t header2_size,
     uint8_t *format,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_parse_header2";
	size_t header_string_size = 0;

	if( libuna_utf8_string_size_from_utf16_stream(
	     header2,
	     header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header string size.",
		 function );

		goto on_error;
	}
	if( ( header_string_size == 0 )
	 || ( header_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid header string size value out of bounds.",
		 function );

		goto on_error;
	}
	header_string = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * header_string_size );

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     header_string,
	     header_string_size,
	     header2,
	     header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to set header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_parse_utf8_header_string(
	     header_values,
	     header_string,
	     header_string_size,
	     2,
	     format,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse header string.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Converts an UTF-8 encoded header string into a header
 * Sets the header and header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_convert_utf8_header_string_to_header(
     const uint8_t *header_string,
     size_t header_string_size,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	static char *function = "libewf_header_values_convert_utf8_header_string_to_header";

	if( header_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header.",
		 function );

		return( -1 );
	}
	if( *header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: header already set.",
		 function );

		return( -1 );
	}
	if( header_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header size.",
		 function );

		return( -1 );
	}
	if( libuna_byte_stream_size_from_utf8(
	     header_string,
	     header_string_size,
	     codepage,
	     header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header size.",
		 function );

		goto on_error;
	}
	if( ( *header_size == 0 )
	 || ( *header_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid header size value out of bounds.",
		 function );

		goto on_error;
	}
	*header = (uint8_t *) memory_allocate(
	                       sizeof( uint8_t ) * *header_size );

	if( *header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	if( libuna_byte_stream_copy_from_utf8(
	     *header,
	     *header_size,
	     codepage,
	     header_string,
	     header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *header != NULL )
	{
		memory_free(
		 header );

		*header = NULL;
	}
	*header_size = 0;

	return( -1 );
}

/* Generate an UTF-8 encoded header string
 * Sets header string and header string size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_utf8_header_string(
     libfvalue_table_t *header_values,
     uint8_t header_type,
     uint8_t header_string_type,
     uint8_t *newline_string,
     size_t newline_string_length,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *generated_acquiry_date               = NULL;
	uint8_t *generated_system_date                = NULL;
	uint8_t *safe_utf8_string                     = NULL;
	static char *function                         = "libewf_header_values_generate_utf8_header_string";
	char *generated_compression_level             = NULL;
	char *generated_extents                       = NULL;
	char *generated_password                      = NULL;
	char *generated_srce_section                  = NULL;
	char *generated_sub_section                   = NULL;
	size_t acquiry_date_string_length             = 0;
	size_t acquiry_operating_system_string_length = 0;
	size_t acquiry_software_version_string_length = 0;
	size_t case_number_string_length              = 0;
	size_t compression_level_string_length        = 0;
	size_t description_string_length              = 0;
	size_t device_label_string_length             = 0;
	size_t evidence_number_string_length          = 0;
	size_t examiner_name_string_length            = 0;
	size_t extents_string_length                  = 0;
	size_t generated_acquiry_date_size            = 0;
	size_t generated_system_date_size             = 0;
	size_t model_string_length                    = 0;
	size_t notes_string_length                    = 0;
	size_t password_string_length                 = 0;
	size_t process_identifier_string_length       = 0;
	size_t safe_utf8_string_size                  = 0;
	size_t serial_number_string_length            = 0;
	size_t srce_section_string_length             = 0;
	size_t sub_section_string_length              = 0;
	size_t system_date_string_length              = 0;
	size_t unknown_dc_string_length               = 0;
	size_t utf8_string_index                      = 0;
	int number_of_characters                      = 0;
	int number_of_tabs                            = 0;
	int result                                    = 0;

	if( ( header_type != 1 )
	 && ( header_type != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header type.",
		 function );

		return( -1 );
	}
	if( ( header_string_type != LIBEWF_HEADER_STRING_TYPE_1 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_2 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_3 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_4 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_5 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_6 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_7 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_8 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header string type.",
		 function );

		return( -1 );
	}
	if( newline_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid newline string.",
		 function );

		return( -1 );
	}
	if( ( newline_string_length != 1 )
	 && ( newline_string_length != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported newline string length.",
		 function );

		return( -1 );
	}
	if( ( compression_level != LIBEWF_COMPRESSION_LEVEL_DEFAULT )
	 && ( compression_level != LIBEWF_COMPRESSION_LEVEL_NONE )
	 && ( compression_level != LIBEWF_COMPRESSION_LEVEL_FAST )
	 && ( compression_level != LIBEWF_COMPRESSION_LEVEL_BEST ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression level.",
		 function );

		return( -1 );
	}
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
		if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
		 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
		 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 ) )
		{
			result = libewf_generate_date_header_value(
				  timestamp,
				  &generated_acquiry_date,
				  &generated_acquiry_date_size,
				  error );
		}
		else
		{
			result = libewf_generate_date_header2_value(
				  timestamp,
				  &generated_acquiry_date,
				  &generated_acquiry_date_size,
				  error );
		}
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
		if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
		 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
		 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 ) )
		{
			result = libewf_generate_date_header_value(
				  timestamp,
				  &generated_system_date,
				  &generated_system_date_size,
				  error );
		}
		else
		{
			result = libewf_generate_date_header2_value(
				  timestamp,
				  &generated_system_date,
				  &generated_system_date_size,
				  error );
		}
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
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
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
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "password",
	     9,
	     &password_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: password length.",
		 function );

		goto on_error;
	}
	if( ( password_string_length == 0 )
	 && ( header_type == 1 ) )
	{
		generated_password = "0";

		password_string_length = 1;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		if( libewf_value_table_get_value_utf8_string_length(
		     header_values,
		     (uint8_t *) "compression_level",
		     18,
		     &compression_level_string_length,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: compression_level length.",
			 function );

			goto on_error;
		}
		if( compression_level_string_length == 0 )
		{
			if( ( compression_level == LIBEWF_COMPRESSION_LEVEL_DEFAULT )
			 || ( compression_level == LIBEWF_COMPRESSION_LEVEL_NONE ) )
			{
				generated_compression_level = LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_NONE;
			}
			else if( compression_level == LIBEWF_COMPRESSION_LEVEL_FAST )
			{
				generated_compression_level = LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_FAST;
			}
			else if( compression_level == LIBEWF_COMPRESSION_LEVEL_BEST )
			{
				generated_compression_level = LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_BEST;
			}
			compression_level_string_length = narrow_string_length(
			                                   generated_compression_level );
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		if( libewf_value_table_get_value_utf8_string_length(
		     header_values,
		     (uint8_t *) "model",
		     6,
		     &model_string_length,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: model length.",
			 function );

			goto on_error;
		}
		if( libewf_value_table_get_value_utf8_string_length(
		     header_values,
		     (uint8_t *) "serial_number",
		     14,
		     &serial_number_string_length,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: serial_number length.",
			 function );

			goto on_error;
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		if( libewf_value_table_get_value_utf8_string_length(
		     header_values,
		     (uint8_t *) "device_label",
		     13,
		     &device_label_string_length,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: device_label length.",
			 function );

			goto on_error;
		}
		if( libewf_value_table_get_value_utf8_string_length(
		     header_values,
		     (uint8_t *) "process_identifier",
		     19,
		     &process_identifier_string_length,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: process_identifier length.",
			 function );

			goto on_error;
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		if( libewf_value_table_get_value_utf8_string_length(
		     header_values,
		     (uint8_t *) "unknown_dc",
		     11,
		     &unknown_dc_string_length,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: unknown_dc length.",
			 function );

			goto on_error;
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		if( libewf_value_table_get_value_utf8_string_length(
		     header_values,
		     (uint8_t *) "extents",
		     8,
		     &extents_string_length,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: extents length.",
			 function );

			goto on_error;
		}
		if( extents_string_length == 0 )
		{
			generated_extents = "0";

			extents_string_length = 1;
		}
	}
	/* Determine the string size
	 * Reserve space for:
	 * # <newline>
	 * main <newline>
	 */
	safe_utf8_string_size = 5 + ( 2 * newline_string_length );

	switch( header_string_type )
	{
		case LIBEWF_HEADER_STRING_TYPE_1:
			/* Reserve space for:
			 * c <tab> n <tab> a <tab> e <tab> t <tab> m <tab> u <tab> p <tab> r <newline>
			 */
			number_of_characters = 9;
			number_of_tabs       = 8;
			break;

		case LIBEWF_HEADER_STRING_TYPE_2:
			/* Reserve space for:
			 * c <tab> n <tab> a <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <tab> r <newline>
			 */
			number_of_characters = 13;
			number_of_tabs       = 10;
			break;

		case LIBEWF_HEADER_STRING_TYPE_3:
			/* Reserve space for:
			 * c <tab> n <tab> a <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <newline>
			 */
			number_of_characters = 12;
			number_of_tabs       = 9;
			break;

		case LIBEWF_HEADER_STRING_TYPE_4:
		case LIBEWF_HEADER_STRING_TYPE_7:
			/* Reserve space for:
			 * a <tab> c <tab> n <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <newline>
			 */
			number_of_characters = 12;
			number_of_tabs       = 9;
			break;

		case LIBEWF_HEADER_STRING_TYPE_5:
			/* Reserve space for:
			 * a <tab> c <tab> n <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <tab> dc <newline>
			 */
			number_of_characters = 14;
			number_of_tabs       = 10;
			break;

		case LIBEWF_HEADER_STRING_TYPE_6:
			/* Reserve space for:
			 * a <tab> c <tab> n <tab> e <tab> t <tab> md <tab> sn <tab> av <tab> ov <tab> m <tab> u <tab> p <tab> dc <newline>
			 */
			number_of_characters = 18;
			number_of_tabs       = 12;
			break;

		case LIBEWF_HEADER_STRING_TYPE_8:
		case LIBEWF_HEADER_STRING_TYPE_9:
			/* Reserve space for:
			 * a <tab> c <tab> n <tab> e <tab> t <tab> md <tab> sn <tab> l <tab> av <tab> ov <tab> m <tab> u <tab> p <tab> pid <tab> dc <tab> ext <newline>
			 */
			number_of_characters = 25;
			number_of_tabs       = 15;
			break;
	}
	safe_utf8_string_size += number_of_characters + number_of_tabs + newline_string_length;

	/* Reserve space for the values
	 */
	safe_utf8_string_size += case_number_string_length
	                       + description_string_length
	                       + examiner_name_string_length
	                       + evidence_number_string_length
	                       + notes_string_length
	                       + acquiry_date_string_length
	                       + system_date_string_length;

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string_size += acquiry_software_version_string_length
		                       + acquiry_operating_system_string_length;
	}
	safe_utf8_string_size += password_string_length;

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		safe_utf8_string_size += compression_level_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string_size += model_string_length
		                       + serial_number_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string_size += device_label_string_length
		                       + process_identifier_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string_size += unknown_dc_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string_size += extents_string_length;
	}
	/* Reserve space for the tabs and 2 newlines
	 */
	safe_utf8_string_size += number_of_tabs + ( 2 * newline_string_length );

	/* Reserve space for additional sections
	 */
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		generated_srce_section = "srce\n"
		                         "0\t1\n"
		                         "p\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n"
		                         "0\t0\n"
		                         "\t\t\t\t\t-1\t-1\t\t\t\n"
		                         "\n";

		srce_section_string_length = narrow_string_length(
		                              generated_srce_section );

		safe_utf8_string_size += srce_section_string_length;
	}
	else if( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	{
		generated_srce_section = "srce\n"
		                         "0\t1\n"
		                         "p\tn\tid\tev\ttb\tlo\tpo\tah\tsh\tgu\taq\n"
		                         "0\t0\n"
		                         "\t\t\t\t\t-1\t-1\t00000000000000000000000000000000\t0000000000000000000000000000000000000000\t00000000000000000000000000000000\t\n"
		                         "\n";

		srce_section_string_length = narrow_string_length(
		                              generated_srce_section );

		safe_utf8_string_size += srce_section_string_length;
	}
	else if( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 )
	{
		generated_srce_section = "srce\n"
		                         "0\t1\n"
		                         "p\tn\tid\tev\ttb\tlo\tpo\tah\tsh\tgu\tpgu\taq\n"
		                         "0\t0\n"
		                         "\t\t\t\t\t-1\t-1\t00000000000000000000000000000000\t0000000000000000000000000000000000000000\t00000000000000000000000000000000\t00000000000000000000000000000000\t\n"
		                         "\n";

		srce_section_string_length = narrow_string_length(
		                              generated_srce_section );

		safe_utf8_string_size += srce_section_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		/* Note that the trailing after the co value is intentional
		 */
		generated_sub_section = "sub\n"
		                        "0\t1\n"
		                        "p\tn\tid\tnu\tco\tgu\n"
		                        "0\t0\n"
		                        "\t\t\t\t1 \t\n"
		                        "\n";

		sub_section_string_length = narrow_string_length(
		                             generated_sub_section );

		safe_utf8_string_size += sub_section_string_length;
	}
	else if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		/* Note that the trailing after the co value is intentional
		 */
		generated_sub_section = "sub\n"
		                        "0\t1\n"
		                        "p\tn\tid\tnu\tco\tgu\n"
		                        "0\t0\n"
		                        "\t\t\t\t1 \t00000000000000000000000000000000\n"
		                        "\n";

		sub_section_string_length = narrow_string_length(
		                             generated_sub_section );

		safe_utf8_string_size += sub_section_string_length;
	}
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
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '1';
	}
	else if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '3';
	}
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
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'a';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'c';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'a';
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'e';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'm';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'd';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'l';
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'a';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'v';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'o';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'v';
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'm';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'u';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'p';

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'r';
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'p';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'i';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'd';
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'd';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'c';
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'e';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'x';
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	}
	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
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
	}
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
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

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
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

		if( model_string_length > 0 )
		{
			if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
			     header_values,
			     (uint8_t *) "model",
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
				 "%s: unable to copy header value: model to string.",
				 function );

				goto on_error;
			}
		}
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

		if( serial_number_string_length > 0 )
		{
			if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
			     header_values,
			     (uint8_t *) "serial_number",
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
				 "%s: unable to copy header value: serial_number to string.",
				 function );

				goto on_error;
			}
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

		if( device_label_string_length > 0 )
		{
			if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
			     header_values,
			     (uint8_t *) "device_label",
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
				 "%s: unable to copy header value: device_label to string.",
				 function );

				goto on_error;
			}
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
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
			 "%s: unable to copy acquiry date string.",
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
			 "%s: unable to copy system date string.",
			 function );

			goto on_error;
		}
		utf8_string_index += system_date_string_length;

		memory_free(
		 generated_system_date );

		generated_system_date = NULL;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( generated_password == NULL )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "password",
		     9,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: password to string.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( narrow_string_copy(
		     (char *) &( safe_utf8_string[ utf8_string_index ] ),
		     generated_password,
		     password_string_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy password string.",
			 function );

			goto on_error;
		}
		utf8_string_index += password_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

		if( generated_compression_level == NULL )
		{
			if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
			     header_values,
			     (uint8_t *) "compression_level",
			     18,
			     safe_utf8_string,
			     safe_utf8_string_size,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy header value: compression_level to string.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( narrow_string_copy(
			     (char *) &( safe_utf8_string[ utf8_string_index ] ),
			     generated_compression_level,
			     compression_level_string_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy compression level string.",
				 function );

				goto on_error;
			}
			utf8_string_index += compression_level_string_length;
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

		if( process_identifier_string_length > 0 )
		{
			if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
			     header_values,
			     (uint8_t *) "process_identifier",
			     19,
			     safe_utf8_string,
			     safe_utf8_string_size,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy header value: process_identifier to string.",
				 function );

				goto on_error;
			}
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

		if( unknown_dc_string_length > 0 )
		{
			if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
			     header_values,
			     (uint8_t *) "unknown_dc",
			     11,
			     safe_utf8_string,
			     safe_utf8_string_size,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy header value: unknown_dc to string.",
				 function );

				goto on_error;
			}
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

		if( generated_extents == NULL )
		{
			if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
			     header_values,
			     (uint8_t *) "extents",
			     8,
			     safe_utf8_string,
			     safe_utf8_string_size,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy header value: extents to string.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( narrow_string_copy(
			     (char *) &( safe_utf8_string[ utf8_string_index ] ),
			     generated_extents,
			     extents_string_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy extents string.",
				 function );

				goto on_error;
			}
			utf8_string_index += extents_string_length;
		}
	}
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
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_8 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_9 ) )
	{
		if( srce_section_string_length > 0 )
		{
			if( narrow_string_copy(
			     (char *) &( safe_utf8_string[ utf8_string_index ] ),
			     generated_srce_section,
			     srce_section_string_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy srce section string.",
				 function );

				goto on_error;
			}
			utf8_string_index += srce_section_string_length;
		}
		if( sub_section_string_length > 0 )
		{
			if( narrow_string_copy(
			     (char *) &( safe_utf8_string[ utf8_string_index ] ),
			     generated_sub_section,
			     sub_section_string_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy sub section string.",
				 function );

				goto on_error;
			}
			utf8_string_index += sub_section_string_length;
		}
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

/* Generate an EWF header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_ewf(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_ewf";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     1,
	     LIBEWF_HEADER_STRING_TYPE_1,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Generate an EnCase1 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_encase1(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_encase1";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     1,
	     LIBEWF_HEADER_STRING_TYPE_1,
	     (uint8_t *) "\r\n",
	     2,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Generate an FTK Imager header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_ftk(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_ftk";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     1,
	     LIBEWF_HEADER_STRING_TYPE_2,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Generate an EnCase2 and EnCase3 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_encase2(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_encase2";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     1,
	     LIBEWF_HEADER_STRING_TYPE_2,
	     (uint8_t *) "\r\n",
	     2,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Generate an EnCase4 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_encase4(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_encase4";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     1,
	     LIBEWF_HEADER_STRING_TYPE_3,
	     (uint8_t *) "\r\n",
	     2,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Generate a linen header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_linen(
     libfvalue_table_t *header_values,
     uint8_t format,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	uint8_t *header_string     = NULL;
	static char *function      = "libewf_header_values_generate_header_linen";
	size_t header_string_size  = 0;
	uint8_t header_string_type = 0;

	switch( format )
	{
		case LIBEWF_FORMAT_LINEN5:
			header_string_type = LIBEWF_HEADER_STRING_TYPE_7;
			break;

		case LIBEWF_FORMAT_LINEN6:
			header_string_type = LIBEWF_HEADER_STRING_TYPE_6;
			break;

		case LIBEWF_FORMAT_LINEN7:
			header_string_type = LIBEWF_HEADER_STRING_TYPE_8;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported format.",
			 function );

			break;
	}
	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     1,
	     header_string_type,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Generate a header2
 * Sets header2 and header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2(
     libfvalue_table_t *header_values,
     uint8_t format,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header2,
     size_t *header2_size,
     libcerror_error_t **error )
{
	uint8_t *header_string     = NULL;
	uint8_t *safe_header2      = NULL;
	static char *function      = "libewf_header_values_generate_header2";
	size_t header_string_size  = 0;
	size_t safe_header2_size   = 0;
	uint8_t header_string_type = 0;

	switch( format )
	{
		case LIBEWF_FORMAT_ENCASE4:
		case LIBEWF_FORMAT_EWFX:
			header_string_type = LIBEWF_HEADER_STRING_TYPE_4;
			break;

		case LIBEWF_FORMAT_ENCASE5:
			header_string_type = LIBEWF_HEADER_STRING_TYPE_5;
			break;

		case LIBEWF_FORMAT_ENCASE6:
			header_string_type = LIBEWF_HEADER_STRING_TYPE_6;
			break;

		case LIBEWF_FORMAT_ENCASE7:
			header_string_type = LIBEWF_HEADER_STRING_TYPE_9;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported format.",
			 function );

			break;
	}
	if( header2 == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header2.",
		 function );

		return( -1 );
	}
	if( *header2 != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: header2 already created.",
		 function );

		return( -1 );
	}
	if( header2_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header2 size.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     2,
	     header_string_type,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libuna_utf16_stream_size_from_utf8(
	     header_string,
	     header_string_size,
	     &safe_header2_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header2 size.",
		 function );

		goto on_error;
	}
	if( ( safe_header2_size == 0 )
	 || ( safe_header2_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid header2 size value out of bounds.",
		 function );

		goto on_error;
	}
	safe_header2 = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * safe_header2_size );

	if( safe_header2 == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header2.",
		 function );

		goto on_error;
	}
	if( libuna_utf16_stream_copy_from_utf8(
	     safe_header2,
	     safe_header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     header_string,
	     header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set header2.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	*header2      = safe_header2;
	*header2_size = safe_header2_size;

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	if( safe_header2 != NULL )
	{
		memory_free(
		 safe_header2 );
	}
	return( -1 );
}

/* Convert date time values string within an xheader value
 * Sets date time values string and length
 * Returns 1 if successful, 0 if not an xheader value or -1 on error
 */
int libewf_convert_date_xheader_value(
     const uint8_t *header_value,
     size_t header_value_length,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error )
{
	struct tm time_elements;

	libfvalue_split_utf8_string_t *date_time_values = NULL;
	uint8_t *date_time_value_string                 = NULL;
	uint8_t *timezone_name                          = NULL;
	uint8_t *timezone_string                        = NULL;
	static char *function                           = "libewf_convert_date_xheader_value";
	size_t date_time_value_string_size              = 0;
	size_t date_time_values_string_index            = 0;
	size_t timezone_name_size                       = 0;
	size_t timezone_string_size                     = 0;
	time_t timestamp                                = 0;
	int empty_date_element_correction               = 0;
	int number_of_date_time_values                  = 0;

	if( header_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value.",
		 function );

		return( -1 );
	}
	if( date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     header_value,
	     header_value_length,
	     (uint8_t) ' ',
	     &date_time_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header value.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     date_time_values,
	     &number_of_date_time_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of date time values.",
		 function );

		goto on_error;
	}
	/* The ctime formatted string use an additional space
	 * when the day is less than 10
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     2,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 2.",
		 function );

		goto on_error;
	}
	if( ( date_time_value_string != NULL )
	 && ( date_time_value_string[ 0 ] == 0 ) )
	{
		empty_date_element_correction = 1;
	}
	if( number_of_date_time_values < ( 5 + empty_date_element_correction ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported number of date time element values: %d.\n",
			 function,
			 number_of_date_time_values );
		}
#endif
		if( libfvalue_split_utf8_string_free(
		     &date_time_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split date time elements.",
			 function );

			goto on_error;
		}
		return( 0 );
	}
	/* Check the day of the week
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     0,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 0.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_day_of_week_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of week string.",
		 function );

		goto on_error;
	}
	/* Set the year
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     4 + empty_date_element_correction,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: %d.",
		 function,
		 4 + empty_date_element_correction );

		goto on_error;
	}
	if( libewf_utf8_string_year_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy year value.",
		 function );

		goto on_error;
	}
	/* Set the month
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     1,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: 1.",
		 function );

		goto on_error;
	}
	if( date_time_value_string_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: 1.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_month_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy month value.",
		 function );

		goto on_error;
	}
	/* Set the day of the month
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     2 + empty_date_element_correction,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: %d.",
		 function,
		 empty_date_element_correction );

		goto on_error;
	}
	if( ( date_time_value_string_size != 2 )
	 && ( date_time_value_string_size != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: %d.",
		 function,
		 empty_date_element_correction );

		goto on_error;
	}
	if( libewf_utf8_string_day_of_month_copy_to_time_elements(
	     date_time_value_string,
	     date_time_value_string_size,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of month value.",
		 function );

		goto on_error;
	}
	/* Set the hours, minutes and seconds
	 */
	if( libfvalue_split_utf8_string_get_segment_by_index(
	     date_time_values,
	     3 + empty_date_element_correction,
	     &date_time_value_string,
	     &date_time_value_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve date time value string: %d.",
		 function,
		 3 + empty_date_element_correction );

		goto on_error;
	}
	if( date_time_value_string_size != 9 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date time value string size: %d.",
		 function,
		 3 + empty_date_element_correction );

		goto on_error;
	}
	if( libewf_utf8_string_hours_copy_to_time_elements(
	     &( date_time_value_string[ 0 ] ),
	     2,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hours value.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_minutes_copy_to_time_elements(
	     &( date_time_value_string[ 3 ] ),
	     2,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy minutes value.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_seconds_copy_to_time_elements(
	     &( date_time_value_string[ 6 ] ),
	     2,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy seconds value.",
		 function );

		goto on_error;
	}
	/* Set to ignore the daylight saving time
	 */
	time_elements.tm_isdst = -1;

	/* Create a timestamp
	 */
	timestamp = libewf_date_time_mktime(
		     &time_elements );

	if( timestamp == (time_t) -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create timestamp.",
		 function );

		goto on_error;
	}
	*date_time_values_string_size = 20;

	if( ( number_of_date_time_values - empty_date_element_correction ) > 5 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     date_time_values,
		     5 + empty_date_element_correction,
		     &timezone_string,
		     &timezone_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve date time value string: %d.",
			 function,
			 5 + empty_date_element_correction );

			goto on_error;
		}
		*date_time_values_string_size += timezone_string_size;
	}
	if( ( number_of_date_time_values - empty_date_element_correction ) > 6 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     date_time_values,
		     6 + empty_date_element_correction,
		     &timezone_name,
		     &timezone_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve date time value string: %d.",
			 function,
			 6 + empty_date_element_correction );

			goto on_error;
		}
		*date_time_values_string_size += timezone_name_size;
	}
	if( ( *date_time_values_string_size == 0 )
	  | ( *date_time_values_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid date time values string size value out of bounds.",
		 function );

		goto on_error;
	}
	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		goto on_error;
	}
	if( libewf_date_time_values_copy_from_timestamp(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to convert timestamp into date time values string.",
		 function );

		goto on_error;
	}
	date_time_values_string_index = 19;

	if( number_of_date_time_values > ( 5 + empty_date_element_correction ) )
	{
		( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

		if( narrow_string_copy(
		     (char *) &( ( *date_time_values_string )[ date_time_values_string_index ] ),
		     (char *) timezone_string,
		     timezone_string_size - 1 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set timezone string in data time values string.",
			 function );

			goto on_error;
		}
		date_time_values_string_index += timezone_string_size - 1;
	}
	if( number_of_date_time_values > ( 6 + empty_date_element_correction ) )
	{
		( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

		if( narrow_string_copy(
		     (char *) &( ( *date_time_values_string )[ date_time_values_string_index ] ),
		     (char *) timezone_name,
		     timezone_name_size - 1 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set timezone name in data time values string.",
			 function );

			goto on_error;
		}
		date_time_values_string_index += timezone_name_size - 1;

	}
	( *date_time_values_string )[ date_time_values_string_index ] = 0;

	if( libfvalue_split_utf8_string_free(
	     &date_time_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split date time elements.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( date_time_values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &date_time_values,
	         NULL );
	}
	if( *date_time_values_string != NULL )
	{
		memory_free(
		 *date_time_values_string );

		*date_time_values_string = NULL;
	}
	*date_time_values_string_size = 0;

	return( -1 );
}

/* Generate date time values string within a xheader value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_xheader_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     libcerror_error_t **error )
{
	struct tm time_elements;

	static char *function                = "libewf_generate_date_xheader_value";
	size_t date_time_values_string_index = 0;
	int print_count                      = 0;

#if defined( WINAPI )
	size_t tzname_length                 = 0;
#endif

	if( date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libewf_date_time_localtime(
	     &timestamp,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create time elements.",
		 function );

		goto on_error;
	}
	if( ( time_elements.tm_year + 1900 ) > 10000 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported year value.",
		 function );

		goto on_error;
	}
	*date_time_values_string_size = 64;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		goto on_error;
	}
	if( libewf_utf8_string_day_of_week_copy_from_time_elements(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of week to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libewf_utf8_string_month_copy_from_time_elements(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy month to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( time_elements.tm_mday < 10 )
	{
		( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';
	}
	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_mday,
	     8,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of month to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index - 1 ] = (uint8_t) ' ';

	if( libewf_utf8_string_time_copy_from_time_elements(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     &time_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy time to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_with_index_copy_from_integer(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint64_t) time_elements.tm_year + 1900,
	     16,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy year to date time values string.",
		 function );

		goto on_error;
	}
	( *date_time_values_string )[ date_time_values_string_index - 1 ] = (uint8_t) ' ';

#if defined( _BSD_SOURCE )
	time_elements.tm_gmtoff /= 60;

	print_count = narrow_string_snprintf(
	               (char *) &( ( ( *date_time_values_string )[ date_time_values_string_index ] ) ),
		       *date_time_values_string_size - date_time_values_string_index,
		       "%+03ld:%02ld (%s)",
	               time_elements.tm_gmtoff / 60,
	               time_elements.tm_gmtoff % 60,
		       time_elements.tm_zone );

#elif defined( WINAPI )
	/* WINAPI sometimes uses long timezone names
	 */
	tzname_length = narrow_string_length(
	                 _tzname[ time_elements.tm_isdst ] );

	if( tzname_length <= 4 )
	{
		print_count = narrow_string_snprintf(
			       (char *) &( ( ( *date_time_values_string )[ date_time_values_string_index ] ) ),
			       *date_time_values_string_size - date_time_values_string_index,
			       "%s",
			       _tzname[ time_elements.tm_isdst ] );
	}
	else
	{
		print_count = 0;
	}
#else
	print_count = narrow_string_snprintf(
	               (char *) &( ( ( *date_time_values_string )[ date_time_values_string_index ] ) ),
		       *date_time_values_string_size - date_time_values_string_index,
		       "%s",
		       tzname[ time_elements.tm_isdst ] );
#endif

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > ( *date_time_values_string_size - date_time_values_string_index ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set date time values string.",
		 function );

		goto on_error;
	}
	date_time_values_string_index += print_count;

	( *date_time_values_string )[ date_time_values_string_index ] = 0;

	return( 1 );

on_error:
	if( *date_time_values_string != NULL )
	{
		memory_free(
		 *date_time_values_string );

		*date_time_values_string = NULL;
	}
	*date_time_values_string_size = 0;

	return( -1 );
}

/* Parses a xheader for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_xheader(
     libfvalue_table_t *header_values,
     const uint8_t *xheader,
     size_t xheader_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_header_values_parse_xheader";

	if( libfvalue_table_copy_from_utf8_xml_string(
	     header_values,
	     xheader,
	     xheader_size,
	     (uint8_t *) "xheader",
	     7,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-8 string to header values table.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_parse_xheader_date_value(
	     header_values,
	     (uint8_t *) "acquiry_date",
	     13,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse xheader date value: acquiry_date.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parses a xheader date value
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_xheader_date_value(
     libfvalue_table_t *header_values,
     const uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *header_value     = NULL;
	uint8_t *date_time_values_string    = NULL;
	uint8_t *value_data                 = NULL;
	static char *function               = "libewf_header_values_parse_xheader_date_value";
	size_t date_time_values_string_size = 0;
	size_t value_data_size              = 0;
	int encoding                        = 0;
	int result                          = 0;

	result = libfvalue_table_get_value_by_identifier(
	          header_values,
	          identifier,
	          identifier_size,
		  &header_value,
		  0,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 (char *) identifier );

		goto on_error;
	}
	else if( result == 1 )
	{
		if( libfvalue_value_get_data(
		     header_value,
		     &value_data,
		     &value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %s data.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		result = libewf_convert_date_xheader_value(
		          value_data,
		          value_data_size,
		          &date_time_values_string,
		          &date_time_values_string_size,
		          error );

		if( result == -1 )
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
		else if( result != 0 )
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			date_time_values_string_size = 1 + narrow_string_length(
							    (char *) date_time_values_string );

			if( libfvalue_value_set_data(
			     header_value,
			     date_time_values_string,
			     date_time_values_string_size,
			     LIBFVALUE_CODEPAGE_UTF8,
			     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set header value: %s data.",
				 function,
				 (char *) identifier );

				goto on_error;
			}
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported xheader date value: %s.\n",
			 function,
			 (char *) identifier );
		}
#endif
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

/* Generate an xheader
 * Sets xheader and the xheader size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_xheader(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **xheader,
     size_t *xheader_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *header_value     = NULL;
	uint8_t *generated_acquiry_date     = NULL;
	uint8_t *identifier                 = NULL;
	char *xml_head                      = NULL;
	char *xml_xheader_close_tag         = NULL;
	char *xml_xheader_open_tag          = NULL;
	static char *function               = "libewf_header_values_generate_xheader";
	size_t acquiry_date_string_length   = 0;
	size_t generated_acquiry_date_size  = 0;
	size_t identifier_size              = 0;
	size_t value_string_size            = 0;
	size_t xheader_index                = 0;
	size_t xml_head_length              = 0;
	size_t xml_xheader_close_tag_length = 0;
	size_t xml_xheader_open_tag_length  = 0;
	int header_value_index              = 0;
	int number_of_header_values         = 0;
	int result                          = 0;

	if( xheader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xheader.",
		 function );

		return( -1 );
	}
	if( *xheader != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: xheader already created.",
		 function );

		return( -1 );
	}
	if( xheader_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_number_of_values(
	     header_values,
	     &number_of_header_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of header values.",
		 function );

		goto on_error;
	}
	xml_head = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	xml_head_length = narrow_string_length(
	                   xml_head );

	xml_xheader_open_tag = "<xheader>\n";

	xml_xheader_open_tag_length = narrow_string_length(
	                               xml_xheader_open_tag );

	xml_xheader_close_tag = "</xheader>\n\n";

	xml_xheader_close_tag_length = narrow_string_length(
	                                xml_xheader_close_tag );

	/* Reserve space for the UTF-8 byte order mark and the XML skeleton data
	 */
	*xheader_size = 3 + xml_head_length + xml_xheader_open_tag_length + xml_xheader_close_tag_length;

	for( header_value_index = 0;
	     header_value_index < number_of_header_values;
	     header_value_index++ )
	{
		if( libfvalue_table_get_value_by_index(
		     header_values,
		     header_value_index,
		     &header_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %d.",
			 function,
			 header_value_index );

			goto on_error;
		}
		if( libfvalue_value_get_identifier(
		     header_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of header value: %d.",
			 function,
			 header_value_index );

			goto on_error;
		}
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: missing identifier for header value: %d.\n",
				 function,
				 header_value_index );
			}
#endif
			continue;
		}
		result = libfvalue_value_has_data(
		          header_value,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data of header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( ( generated_acquiry_date == NULL )
			 && ( identifier_size == 13 )
			 && ( narrow_string_compare(
			       (char *) identifier,
			       "acquiry_date",
			       12 ) == 0 ) )
			{
				if( libewf_generate_date_xheader_value(
				     timestamp,
				     &generated_acquiry_date,
				     &generated_acquiry_date_size,
				     error ) != 1 )
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
					acquiry_date_string_length = narrow_string_length(
								      (char *) generated_acquiry_date );

					/* Reserve space for a leading tab, <acquiry_date>, header value, </acquiry_date> and a newline
					 * Make sure to determine the effective length of the acquiry date time values string
					 */
					*xheader_size += 7 + ( 2 * ( identifier_size - 1 ) ) + acquiry_date_string_length;
				}
			}
			continue;
		}
		result = libfvalue_value_get_utf8_string_size(
		          header_value,
		          0,
		          &value_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		if( value_string_size > 1 )
		{
			/* Reserve space for a leading tab, <identifier>value</identifier> and a newline
			 */
			*xheader_size += 7 + ( 2 * ( identifier_size - 1 ) ) + ( value_string_size - 1 );
		}
	}
	/* Reserve space for the end-of-string character
	 */
	*xheader_size += 1;

	if( *xheader_size > MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid xheader size value out of bounds.",
		 function );

		goto on_error;
	}
	*xheader = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * *xheader_size );

	if( *xheader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create xheader.",
		 function );

		goto on_error;
	}
	( *xheader )[ xheader_index++ ] = 0xef;
	( *xheader )[ xheader_index++ ] = 0xbb;
	( *xheader )[ xheader_index++ ] = 0xbf;

	if( narrow_string_copy(
	     (char *) &( ( *xheader )[ xheader_index ] ),
	     xml_head,
	     xml_head_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy XML head string.",
		 function );

		goto on_error;
	}
	xheader_index += xml_head_length;

	if( narrow_string_copy(
	     (char *) &( ( *xheader )[ xheader_index ] ),
	     xml_xheader_open_tag,
	     xml_xheader_open_tag_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy xheader open tag string.",
		 function );

		goto on_error;
	}
	xheader_index += xml_xheader_open_tag_length;

	for( header_value_index = 0;
	     header_value_index < number_of_header_values;
	     header_value_index++ )
	{
		if( libfvalue_table_get_value_by_index(
		     header_values,
		     header_value_index,
		     &header_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %d.",
			 function,
			 header_value_index );

			goto on_error;
		}
		if( libfvalue_value_get_identifier(
		     header_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of header value: %d.",
			 function,
			 header_value_index );

			goto on_error;
		}
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: missing identifier for header value: %d.\n",
				 function,
				 header_value_index );
			}
#endif
			continue;
		}
		result = libfvalue_value_has_data(
		          header_value,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data of header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		else if( result == 0 )
		{
			continue;
		}
		result = libfvalue_value_get_utf8_string_size(
		          header_value,
		          0,
		          &value_string_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: %s.",
			 function,
			 (char *) identifier );

			goto on_error;
		}
		if( value_string_size > 1 )
		{
			( *xheader )[ xheader_index++ ] = (uint8_t) '\t';
			( *xheader )[ xheader_index++ ] = (uint8_t) '<';

			if( narrow_string_copy(
			     (char *) &( ( *xheader )[ xheader_index ] ),
			     (char *) identifier,
			     identifier_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy %s open tag string.",
				 function,
				 (char *) identifier );

				goto on_error;
			}
			xheader_index += identifier_size - 1;

			( *xheader )[ xheader_index++ ] = (uint8_t) '>';

			result = libfvalue_value_copy_to_utf8_string_with_index(
				  header_value,
				  0,
				  *xheader,
				  *xheader_size,
				  &xheader_index,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy header value: %s to string.",
				 function,
				 (char *) identifier );

				goto on_error;
			}
			( *xheader )[ xheader_index - 1 ] = (uint8_t) '<';
			( *xheader )[ xheader_index++ ]   = (uint8_t) '/';

			if( narrow_string_copy(
			     (char *) &( ( *xheader )[ xheader_index ] ),
			     (char *) identifier,
			     identifier_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy %s close tag string.",
				 function,
				 (char *) identifier );

				goto on_error;
			}
			xheader_index += identifier_size - 1;

			( *xheader )[ xheader_index++ ] = (uint8_t) '>';
			( *xheader )[ xheader_index++ ] = (uint8_t) '\n';
		}
	}
	if( generated_acquiry_date != NULL )
	{
		if( narrow_string_copy(
		     (char *) &( ( *xheader )[ xheader_index ] ),
		     "\t<acquiry_date>",
		     15 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy acquiry_date open tag string.",
			 function );

			goto on_error;
		}
		xheader_index += 15;

		if( narrow_string_copy(
		     (char *) &( ( *xheader )[ xheader_index ] ),
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
		xheader_index += acquiry_date_string_length;

		memory_free(
		 generated_acquiry_date );

		generated_acquiry_date = NULL;

		if( narrow_string_copy(
		     (char *) &( ( *xheader )[ xheader_index ] ),
		     "</acquiry_date>\n",
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy acquiry_date close tag string.",
			 function );

			goto on_error;
		}
		xheader_index += 16;
	}
	if( narrow_string_copy(
	     (char *) &( ( *xheader )[ xheader_index ] ),
	     xml_xheader_close_tag,
	     xml_xheader_close_tag_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy xheader close tag string.",
		 function );

		goto on_error;
	}
	xheader_index += xml_xheader_close_tag_length;

	/* Make sure the string is terminated
	 */
	( *xheader )[ xheader_index ] = 0;

	return( 1 );

on_error:
	if( generated_acquiry_date != NULL )
	{
		memory_free(
		 generated_acquiry_date );
	}
	if( *xheader != NULL )
	{
		memory_free(
		 *xheader );

		*xheader = NULL;
	}
	*xheader_size = 0;

	return( -1 );
}

/* Generate an EWFX header
 * Sets header and the header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_ewfx(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     libcerror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_ewfx";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     1,
	     LIBEWF_HEADER_STRING_TYPE_3,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		goto on_error;
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	memory_free(
	 header_string );

	return( 1 );

on_error:
	if( header_string != NULL )
	{
		memory_free(
		 header_string );
	}
	return( -1 );
}

/* Retrieves the size of the value identifier of a specific index
 * The identifier size includes the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_header_values_get_identifier_size(
     libfvalue_table_t *header_values,
     uint32_t index,
     size_t *identifier_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *header_value  = NULL;
	uint8_t *header_value_identifier = NULL;
	static char *function            = "libewf_header_values_get_identifier_size";

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
	if( libfvalue_table_get_value_by_index(
	     header_values,
	     (int) index,
	     &header_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %" PRIu32 ".",
		 function,
		 index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     header_value,
	     &header_value_identifier,
	     identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value identifier size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the header value identifier of a specific index
 * The identifier size should include the end of string character
 * Returns 1 if successful, 0 if no header values are present or -1 on error
 */
int libewf_header_values_get_identifier(
     libfvalue_table_t *header_values,
     uint32_t index,
     uint8_t *identifier,
     size_t identifier_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *header_value     = NULL;
	uint8_t *header_value_identifier    = NULL;
	static char *function               = "libewf_header_values_get_identifier";
	size_t header_value_identifier_size = 0;

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
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( identifier_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_index(
	     header_values,
	     (int) index,
	     &header_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %" PRIu32 ".",
		 function,
		 index );

		return( -1 );
	}
	if( libfvalue_value_get_identifier(
	     header_value,
	     &header_value_identifier,
	     &header_value_identifier_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %" PRIu32 " identifier size.",
		 function,
		 index );

		return( -1 );
	}
	if( identifier_size < header_value_identifier_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: header value: %" PRIu32 " identifier size too small.",
		 function,
		 index );

		return( -1 );
	}
	if( memory_copy(
	     identifier,
	     header_value_identifier,
	     header_value_identifier_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy header value: %" PRIu32 " identifier.",
		 function,
		 index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_header_values_get_utf8_value_size(
     libfvalue_table_t *header_values,
     const uint8_t *identifier,
     size_t identifier_length,
     int date_format,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t date_time_string[ 64 ];

	libfvalue_value_t *header_value = NULL;
	uint8_t *header_value_data      = NULL;
	static char *function           = "libewf_header_values_get_utf8_value_size";
	size_t date_time_string_size    = 64;
	size_t header_value_data_size   = 0;
	size_t string_index             = 0;
	int encoding                    = 0;
	int result                      = 0;

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
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( identifier_length == 16 )
	 && ( narrow_string_compare(
	       (char *) identifier,
	       "compression_type",
	       16 ) == 0 ) )
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          (uint8_t *) "compression_level",
		          18,
		          &header_value,
		          0,
		          error );
	}
	else
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          identifier,
		          identifier_length + 1,
		          &header_value,
		          0,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = libfvalue_value_has_data(
		  header_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if header value has data.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( ( ( identifier_length == 11 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "system_date",
		 11 ) == 0 ) )
	 || ( ( identifier_length == 12 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "acquiry_date",
		 12 ) == 0 ) ) )
	{
		if( libfvalue_value_get_data(
		     header_value,
		     &header_value_data,
		     &header_value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value data.",
			 function );

			return( -1 );
		}
		if( libewf_date_time_values_copy_to_utf8_string(
		     header_value_data,
		     header_value_data_size,
		     date_format,
		     date_time_string,
		     date_time_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value data to date time string.",
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
		for( string_index = 0;
		     string_index < date_time_string_size;
		     string_index++ )
		{
			if( date_time_string[ string_index ] == 0 )
			{
				break;
			}
		}
		*utf8_string_size = 1 + string_index;
	}
	else
	{
		if( libfvalue_value_get_utf8_string_size(
		     header_value,
		     0,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size of header value.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_header_values_get_utf8_value(
     libfvalue_table_t *header_values,
     const uint8_t *identifier,
     size_t identifier_length,
     int date_format,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *header_value = NULL;
	uint8_t *header_value_data      = NULL;
	static char *function           = "libewf_header_values_get_utf8_value";
	size_t header_value_data_size   = 0;
	int encoding                    = 0;
	int result                      = 0;

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
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( identifier_length == 16 )
	 && ( narrow_string_compare(
	       (char *) identifier,
	       "compression_type",
	       16 ) == 0 ) )
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          (uint8_t *) "compression_level",
		          18,
		          &header_value,
		          0,
		          error );
	}
	else
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          identifier,
		          identifier_length + 1,
		          &header_value,
		          0,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = libfvalue_value_has_data(
		  header_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if header value has data.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( ( ( identifier_length == 11 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "system_date",
		 11 ) == 0 ) )
	 || ( ( identifier_length == 12 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "acquiry_date",
		 12 ) == 0 ) ) )
	{
		if( libfvalue_value_get_data(
		     header_value,
		     &header_value_data,
		     &header_value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value data.",
			 function );

			return( -1 );
		}
		if( libewf_date_time_values_copy_to_utf8_string(
		     header_value_data,
		     header_value_data_size,
		     date_format,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value data to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libfvalue_value_copy_to_utf8_string(
		     header_value,
		     0,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded header value of an identifier
 * The string size includes the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_header_values_get_utf16_value_size(
     libfvalue_table_t *header_values,
     const uint8_t *identifier,
     size_t identifier_length,
     int date_format,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	uint16_t date_time_string[ 64 ];

	libfvalue_value_t *header_value = NULL;
	uint8_t *header_value_data      = NULL;
	static char *function           = "libewf_header_values_get_utf16_value_size";
	size_t date_time_string_size    = 64;
	size_t header_value_data_size   = 0;
	size_t string_index             = 0;
	int encoding                    = 0;
	int result                      = 0;

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
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( identifier_length == 16 )
	 && ( narrow_string_compare(
	       (char *) identifier,
	       "compression_type",
	       16 ) == 0 ) )
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          (uint8_t *) "compression_level",
		          18,
		          &header_value,
		          0,
		          error );
	}
	else
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          identifier,
		          identifier_length + 1,
		          &header_value,
		          0,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = libfvalue_value_has_data(
		  header_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if header value has data.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( ( ( identifier_length == 11 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "system_date",
		 11 ) == 0 ) )
	 || ( ( identifier_length == 12 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "acquiry_date",
		 12 ) == 0 ) ) )
	{
		if( libfvalue_value_get_data(
		     header_value,
		     &header_value_data,
		     &header_value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value data.",
			 function );

			return( -1 );
		}
		if( libewf_date_time_values_copy_to_utf16_string(
		     header_value_data,
		     header_value_data_size,
		     date_format,
		     date_time_string,
		     date_time_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value data to date time string.",
			 function );

			return( -1 );
		}
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		for( string_index = 0;
		     string_index < date_time_string_size;
		     string_index++ )
		{
			if( date_time_string[ string_index ] == 0 )
			{
				break;
			}
		}
		*utf16_string_size = 1 + string_index;
	}
	else
	{
		if( libfvalue_value_get_utf16_string_size(
		     header_value,
		     0,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size of header value.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded header value of an identifier
 * The string size should include the end of string character
 * Returns 1 if successful, 0 if not set or -1 on error
 */
int libewf_header_values_get_utf16_value(
     libfvalue_table_t *header_values,
     const uint8_t *identifier,
     size_t identifier_length,
     int date_format,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfvalue_value_t *header_value = NULL;
	uint8_t *header_value_data      = NULL;
	static char *function           = "libewf_header_values_get_utf16_value";
	size_t header_value_data_size   = 0;
	int encoding                    = 0;
	int result                      = 0;

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
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentifier.",
		 function );

		return( -1 );
	}
	if( identifier_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid identifier length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( identifier_length == 16 )
	 && ( narrow_string_compare(
	       (char *) identifier,
	       "compression_type",
	       16 ) == 0 ) )
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          (uint8_t *) "compression_level",
		          18,
		          &header_value,
		          0,
		          error );
	}
	else
	{
		result = libfvalue_table_get_value_by_identifier(
		          header_values,
		          identifier,
		          identifier_length + 1,
		          &header_value,
		          0,
		          error );
	}
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: %s.",
		 function,
		 (char *) identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = libfvalue_value_has_data(
		  header_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if header value has data.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( ( ( identifier_length == 11 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "system_date",
		 11 ) == 0 ) )
	 || ( ( identifier_length == 12 )
	   && ( narrow_string_compare(
		 (char *) identifier,
		 "acquiry_date",
		 12 ) == 0 ) ) )
	{
		if( libfvalue_value_get_data(
		     header_value,
		     &header_value_data,
		     &header_value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value data.",
			 function );

			return( -1 );
		}
		if( libewf_date_time_values_copy_to_utf16_string(
		     header_value_data,
		     header_value_data_size,
		     date_format,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value data to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libfvalue_value_copy_to_utf16_string(
		     header_value,
		     0,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}


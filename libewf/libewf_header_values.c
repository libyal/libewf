/*
 * Header values functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <types.h>

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

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
#include "libewf_libuna.h"
#include "libewf_split_values.h"

#include "ewf_definitions.h"

/* Initializes the header values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_initialize(
     libfvalue_table_t **header_values,
     liberror_error_t **error )
{
	static char *function = "libewf_header_values_initialize";

	if( header_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header values.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_initialize(
	     header_values,
	     LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header values table.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
	     (uint8_t *) "case_number",
	     12,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: case_number.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION,
	     (uint8_t *) "description",
	     12,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: description.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME,
	     (uint8_t *) "examiner_name",
	     14,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: examiner_name.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER,
	     (uint8_t *) "evidence_number",
	     16,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: evidence_number.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_NOTES,
	     (uint8_t *) "notes",
	     6,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: notes.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE,
	     (uint8_t *) "acquiry_date",
	     13,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry_date.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE,
	     (uint8_t *) "system_date",
	     12,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: system_date.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM,
	     (uint8_t *) "acquiry_operating_system",
	     25,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry_operating_system.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION,
	     (uint8_t *) "acquiry_software_version",
	     25,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: acquiry_software_version.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_PASSWORD,
	     (uint8_t *) "password",
	     9,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: password.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE,
	     (uint8_t *) "compression_type",
	     17,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: compression_type.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_MODEL,
	     (uint8_t *) "model",
	     6,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: model.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER,
	     (uint8_t *) "serial_number",
	     14,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: serial_number.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_PROCESS_IDENTIFIER,
	     (uint8_t *) "process_identifier",
	     19,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: process_identifier.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC,
	     (uint8_t *) "unknown_dc",
	     11,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: unknown_dc.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_set_value_by_index(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EXTENTS,
	     (uint8_t *) "extents",
	     8,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: extents.",
		 function );

		libfvalue_table_free(
		 (intptr_t *) header_values,
		 NULL );

		*header_values = NULL;

		return( -1 );
	}
	return( 1 );
}

/* Sets a header value
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_set_value_by_index(
     libfvalue_table_t *header_values,
     int value_index,
     const uint8_t *identifier,
     size_t identifier_size,
     liberror_error_t **error )
{
	libfvalue_value_t *header_value = NULL;
	static char *function           = "libewf_header_values_set_value_by_index";

	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_initialize(
	     &header_value,
	     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
	     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED | LIBFVALUE_VALUE_FLAG_DATA_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header value.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_set_identifier(
	     header_value,
	     identifier,
	     identifier_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: %s identifier.",
		 function,
		 (char *) identifier );

		libfvalue_value_free(
		 (intptr_t *) header_value,
		 NULL );

		return( -1 );
	}
	if( libfvalue_table_set_value_by_index(
	     header_values,
	     value_index,
	     header_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: %d in table.",
		 function,
		 value_index );

		libfvalue_value_free(
		 (intptr_t *) header_value,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Convert a header value into a date time values string
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header_value(
     const uint8_t *header_value,
     size_t header_value_length,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_split_values_t *date_time_elements = NULL;
	static char *function                     = "libewf_convert_date_header_value";
	time_t timestamp                          = 0;

	if( header_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value.",
		 function );

		return( -1 );
	}
	if( date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libewf_split_values_parse_string(
	     &date_time_elements,
	     header_value,
	     header_value_length + 1,
	     (uint8_t) ' ',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header value into date time elements.",
		 function );

		return( -1 );
	}
	if( date_time_elements->number_of_values != 6 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of date time elements in header value.",
		 function );

		libewf_split_values_free(
		 &date_time_elements,
	         NULL );

		return( -1 );
	}
	/* Set the year
	 */
	time_elements.tm_year = (int) ( ( ( ( date_time_elements->values[ 0 ] )[ 0 ] - (uint8_t) '0' ) * 1000 )
	                      + ( ( ( date_time_elements->values[ 0 ] )[ 1 ] - (uint8_t) '0' ) * 100 )
	                      + ( ( ( date_time_elements->values[ 0 ] )[ 2 ] - (uint8_t) '0' ) * 10 )
	                      + ( ( date_time_elements->values[ 0 ] )[ 3 ] - (uint8_t) '0' )
	                      - 1900 );

	/* Set the month
	 */
	if( ( date_time_elements->values[ 1 ] )[ 1 ] == 0 )
	{
		time_elements.tm_mon = (int) ( ( ( date_time_elements->values[ 1 ] )[ 0 ] - (uint8_t) '0' )
		                     - 1 );
	}
	else
	{
		time_elements.tm_mon = (int) ( ( ( ( date_time_elements->values[ 1 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
		                     + ( ( date_time_elements->values[ 1 ] )[ 1 ] - (uint8_t) '0' )
		                     - 1 );
	}
	/* Set the day of the month
	 */
	if( ( date_time_elements->values[ 2 ] )[ 1 ] == 0 )
	{
		time_elements.tm_mday = (int) ( ( date_time_elements->values[ 2 ] )[ 0 ] - (uint8_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( ( date_time_elements->values[ 2 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
		                      + ( ( date_time_elements->values[ 2 ] )[ 1 ] - (uint8_t) '0' ) );
	}
	/* Set the hour
	 */
	if( ( date_time_elements->values[ 3 ] )[ 1 ] == 0 )
	{
		time_elements.tm_hour = (int) ( ( date_time_elements->values[ 3 ] )[ 0 ] - (uint8_t) '0' );
	}
	else
	{
		time_elements.tm_hour = (int) ( ( ( ( date_time_elements->values[ 3 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
		                      + ( ( date_time_elements->values[ 3 ] )[ 1 ] - (uint8_t) '0' ) );
	}
	/* Set the minutes
	 */
	if( ( date_time_elements->values[ 4 ] )[ 1 ] == 0 )
	{
		time_elements.tm_min = (int) ( ( date_time_elements->values[ 4 ] )[ 0 ] - (uint8_t) '0' );
	}
	else
	{
		time_elements.tm_min = (int) ( ( ( ( date_time_elements->values[ 4 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
		                     + ( ( date_time_elements->values[ 4 ] )[ 1 ] - (uint8_t) '0' ) );
	}
	/* Set the seconds
	 */
	if( ( date_time_elements->values[ 5 ] )[ 1 ] == 0 )
	{
		time_elements.tm_sec = (int) ( ( date_time_elements->values[ 5 ] )[ 0 ] - (uint8_t) '0' );
	}
	else
	{
		time_elements.tm_sec = (int) ( ( ( ( date_time_elements->values[ 5 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
		                     + ( ( date_time_elements->values[ 5 ] )[ 1 ] - (uint8_t) '0' ) );
	}
	/* Set to ignore the daylight saving time
	 */
	time_elements.tm_isdst = -1;

	if( libewf_split_values_free(
	     &date_time_elements,
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
	/* Create a timestamp
	 */
	timestamp = libewf_date_time_mktime(
	             &time_elements );

	if( timestamp == (time_t) -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create timestamp.",
		 function );

		return( -1 );
	}
	*date_time_values_string_size = 20;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( libewf_date_time_values_copy_from_timestamp(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     timestamp,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to convert timestamp into date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generates a date time values string within a header value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	static char *function                = "libewf_generate_date_header_value";
	size_t date_time_values_string_index = 0;

	if( date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libewf_date_time_localtime(
	     &timestamp,
	     &time_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements.tm_year + 1900 ) > 10000 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported year value.",
		 function );

		return( -1 );
	}
	*date_time_values_string_size = 20;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( libfvalue_utf8_string_decimal_copy_from_16bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint16_t) time_elements.tm_year + 1900,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy year to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint8_t) time_elements.tm_mon + 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy month to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint8_t) time_elements.tm_mday,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of month to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint8_t) time_elements.tm_hour,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hours to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint8_t) time_elements.tm_min,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy minutes to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint8_t) time_elements.tm_sec,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy seconds to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index ] = 0;

	return( 1 );
}

/* Convert a header2 value into a date time values string
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header2_value(
     const uint8_t *header_value,
     size_t header_value_length,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	static char *function = "libewf_convert_date_header2_value";
	uint64_t value_64bit  = 0;

	if( date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_decimal_copy_to_64bit(
	     header_value,
	     header_value_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set timestamp.",
		 function );

		return( -1 );
	}
	*date_time_values_string_size = 20;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( libewf_date_time_values_copy_from_timestamp(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     (time_t) value_64bit,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to convert timestamp into date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generates a date time values string within a header2 value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header2_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	static char *function                = "libewf_generate_date_header2_value";
	size_t date_time_values_string_index = 0;

	if( date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	*date_time_values_string_size = 11;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( libfvalue_utf8_string_decimal_copy_from_32bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint32_t) timestamp,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy timestamp to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index ] = 0;

	return( 1 );
}

/* Copies the header values from the source to the destination
 * Returns 1 if successful -1 on error
 */
int libewf_header_values_copy(
     libfvalue_table_t *destination_header_values,
     libfvalue_table_t *source_header_values,
     liberror_error_t **error )
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination header values.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_number_of_values(
	     source_header_values,
	     &number_of_header_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %d.",
			 function,
			 header_value_index );

			return( -1 );
		}
		if( libfvalue_value_get_identifier(
		     source_header_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of header value: %d.",
			 function,
			 header_value_index );

			return( -1 );
		}
		/* Ignore header values without an identifier
		 */
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
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
		      && ( libcstring_narrow_string_compare(
		            (char *) identifier,
		            "acquiry_date",
		            12 ) == 0 ) )
		{
			continue;
		}
		else if( ( identifier_size == 12 )
		      && ( libcstring_narrow_string_compare(
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data of header value: %s.",
			 function,
			 (char *) identifier );

			return( -1 );
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable create destination header value.",
			 function );

			return( -1 );
		}
		if( libfvalue_table_set_value_by_index(
		     destination_header_values,
		     header_value_index,
		     destination_header_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %d.",
			 function,
			 header_value_index );

			libfvalue_value_free(
			 (intptr_t *) destination_header_value,
			 NULL );

			destination_header_value = NULL;

			return( -1 );
		}
		destination_header_value = NULL;
	}
	return( 1 );
}

/* Parse an UTF-8 encoded header string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_utf8_header_string(
     libfvalue_table_t *header_values,
     const uint8_t *header_string,
     size_t header_string_size,
     liberror_error_t **error )
{
	libewf_split_values_t *lines        = NULL;
	libewf_split_values_t *types        = NULL;
	libewf_split_values_t *values       = NULL;
	libfvalue_value_t *header_value     = NULL;
	uint8_t *date_time_values_string    = NULL;
	uint8_t *identifier                 = NULL;
	uint8_t *value_string               = NULL;
	static char *function               = "libewf_header_values_parse_utf8_header_string";
	size_t date_time_values_string_size = 0;
	size_t identifier_size              = 0;
	size_t type_string_length           = 0;
	size_t value_string_length          = 0;
	int iterator                        = 0;
	int result                          = 0;

	if( header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( libewf_split_values_parse_string(
	     &lines,
	     header_string,
	     header_string_size,
	     (uint8_t) '\n',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header string into lines.",
		 function );

		return( -1 );
	}
	if( lines->number_of_values > 0 )
	{
		if( ( ( lines->values[ 0 ] )[ 0 ] < (uint8_t) '0' )
		 || ( ( lines->values[ 0 ] )[ 0 ] > (uint8_t) '9' ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: header values string not supported.",
			 function );

			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		if( libewf_split_values_parse_string(
		     &types,
		     lines->values[ 2 ],
		     lines->sizes[ 2 ],
		     (uint8_t) '\t',
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split header string into types.",
			 function );

			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		if( libewf_split_values_parse_string(
		     &values,
		     lines->values[ 3 ],
		     lines->sizes[ 3 ],
		     (uint8_t) '\t',
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split header string into values.",
			 function );

			libewf_split_values_free(
			 &types,
			 NULL );
			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( types->number_of_values != values->number_of_values )
		{
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
			 	"%s: mismatch in number of types and values.\n",
				 function );
			}
		}
#endif
		for( iterator = 0;
		     iterator < types->number_of_values;
		     iterator++ )
		{
			type_string_length = types->sizes[ iterator ] - 1;

			if( ( iterator >= values->number_of_values )
			 || ( values->values[ iterator ] == NULL )
			 || ( ( values->values[ iterator ] )[ 0 ] == 0 ) )
			{
				value_string        = NULL;
				value_string_length = 0;
			}
			else
			{
				value_string        = values->values[ iterator ];
				value_string_length = values->sizes[ iterator ] - 1;
			}
			/* Remove trailing carriage return
			 */
			if( ( type_string_length > 0 )
			 && ( ( types->values[ iterator ] )[ type_string_length - 1 ] == (uint8_t) '\r' ) )
			{
				( types->values[ iterator ] )[ type_string_length - 1 ] = 0;

				type_string_length -= 1;
			}
			if( ( value_string_length > 0 )
			 && ( value_string[ value_string_length - 1 ] == (uint8_t) '\r' ) )
			{
				value_string[ value_string_length - 1 ] = 0;

				value_string_length -= 1;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: type: %s with value: %s.\n",
				 function,
				 (char *) types->values[ iterator ],
				 (char *) value_string );
			}
#endif
			identifier      = NULL;
			identifier_size = 0;

			if( type_string_length == 3 )
			{
				if( libcstring_narrow_string_compare(
				     (char *) types->values[ iterator ],
				     "ext",
				     type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "extents";
					identifier_size = 8;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "pid",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "process_identifier";
					identifier_size = 19;
				}
			}
			else if( type_string_length == 2 )
			{
				if( libcstring_narrow_string_compare(
				     (char *) types->values[ iterator ],
				     "av",
				     type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "acquiry_software_version";
					identifier_size = 25;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "dc",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "unknown_dc";
					identifier_size = 11;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "md",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "model";
					identifier_size = 6;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "ov",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "acquiry_operating_system";
					identifier_size = 25;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "sn",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "serial_number";
					identifier_size = 14;
				}
			}
			else if( type_string_length == 1 )
			{
				if( ( libcstring_narrow_string_compare(
				       (char *) types->values[ iterator ],
				       "m",
				       type_string_length ) == 0 )
				 || ( libcstring_narrow_string_compare(
				       (char *) types->values[ iterator ],
				       "u",
				       type_string_length ) == 0 ) )
				{
					if( ( value_string == NULL )
					 || ( value_string == (uint8_t *) "" ) )
					{
						/* the string search function is not NULL safe */
					}
					/* If the date time values string contains spaces it's in the old header
					 * format otherwise is in new header2 format
					 */
					else if( libcstring_narrow_string_search_character(
					          (char *) value_string,
					          ' ',
					          value_string_length ) != NULL )
					{
						result = libewf_convert_date_header_value(
							  value_string,
							  value_string_length,
							  &date_time_values_string,
							  &date_time_values_string_size,
						          error );
					}
					else if( value_string_length != 0 )
					{
						result = libewf_convert_date_header2_value(
							  value_string,
							  value_string_length,
							  &date_time_values_string,
							  &date_time_values_string_size,
						          error );
					}
					if( ( value_string_length != 0 )
					 && ( result != 1 ) )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_CONVERSION,
						 LIBERROR_CONVERSION_ERROR_GENERIC,
						 "%s: unable to create date time values string.",
						 function );
					}
					else
					{
						/* The effective size of the date time values string is needed
						 */
						if( value_string_length != 0 )
						{
							value_string_length = libcstring_narrow_string_length(
									       (char *) date_time_values_string );

						}
						if( libcstring_narrow_string_compare(
						     (char *) types->values[ iterator ],
						     "m",
						     1 ) == 0 )
						{
							identifier      = (uint8_t *) "acquiry_date";
							identifier_size = 13;
							value_string    = date_time_values_string;
						}
						else if( libcstring_narrow_string_compare(
							  (char *) types->values[ iterator ],
							  "u",
							  1 ) == 0 )
						{
							identifier      = (uint8_t *) "system_date";
							identifier_size = 12;
							value_string    = date_time_values_string;
						}
					}
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "p",
					  type_string_length ) == 0 )
				{
					if( value_string_length == 0 )
					{
						/* Empty hash do nothing
						 */
					}
					else if( ( value_string_length == 1 )
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
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "a",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "description";
					identifier_size = 12;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "c",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "case_number";
					identifier_size = 12;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "n",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "evidence_number";
					identifier_size = 16;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "e",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "examiner_name";
					identifier_size = 14;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "t",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "notes";
					identifier_size = 6;
				}
				else if( libcstring_narrow_string_compare(
					  (char *) types->values[ iterator ],
					  "r",
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "compression_type";
					identifier_size = 17;
				}
			}
			if( identifier != NULL )
			{
				if( libfvalue_value_initialize(
				     &header_value,
				     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
				     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED | LIBFVALUE_VALUE_FLAG_DATA_MANAGED,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create header value.",
					 function );

					if( date_time_values_string != NULL )
					{
						memory_free(
						 date_time_values_string );
					}
					libewf_split_values_free(
					 &values,
					 NULL );
					libewf_split_values_free(
					 &types,
					 NULL );
					libewf_split_values_free(
					 &lines,
					 NULL );

					return( -1 );
				}
				if( libfvalue_value_set_identifier(
				     header_value,
				     identifier,
				     identifier_size,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set header value: %s identifier.",
					 function,
					 (char *) identifier );

					libfvalue_value_free(
					 (intptr_t *) header_value,
					 NULL );

					if( date_time_values_string != NULL )
					{
						memory_free(
						 date_time_values_string );
					}
					libewf_split_values_free(
					 &values,
					 NULL );
					libewf_split_values_free(
					 &types,
					 NULL );
					libewf_split_values_free(
					 &lines,
					 NULL );

					return( -1 );
				}
				if( libfvalue_value_set_data(
				     header_value,
				     value_string,
				     value_string_length,
				     LIBFVALUE_ENDIAN_NATIVE,
				     0,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set header value: %s data.",
					 function,
					 (char *) identifier );

					libfvalue_value_free(
					 (intptr_t *) header_value,
					 NULL );

					if( date_time_values_string != NULL )
					{
						memory_free(
						 date_time_values_string );
					}
					libewf_split_values_free(
					 &values,
					 NULL );
					libewf_split_values_free(
					 &types,
					 NULL );
					libewf_split_values_free(
					 &lines,
					 NULL );

					return( -1 );
				}
				if( libfvalue_table_set_value(
				     header_values,
				     header_value,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set header value: %s in table.",
					 function,
					 (char *) identifier );

					libfvalue_value_free(
					 (intptr_t *) header_value,
					 NULL );

					if( date_time_values_string != NULL )
					{
						memory_free(
						 date_time_values_string );
					}
					libewf_split_values_free(
					 &values,
					 NULL );
					libewf_split_values_free(
					 &types,
					 NULL );
					libewf_split_values_free(
					 &lines,
					 NULL );

					return( -1 );
				}
				header_value = NULL;
			}
			if( date_time_values_string != NULL )
			{
				memory_free(
				 date_time_values_string );

				date_time_values_string = NULL;
			}
		}
		if( libewf_split_values_free(
		     &values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split values.",
			 function );

			libewf_split_values_free(
			 &types,
			 NULL );
			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		if( libewf_split_values_free(
		     &types,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split types.",
			 function );

			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
	}
	if( libewf_split_values_free(
	     &lines,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split lines.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parse an EWF header for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header(
     libfvalue_table_t *header_values,
     uint8_t *header,
     size_t header_size,
     int codepage,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_parse_header";
	size_t header_string_size = 0;

	if( header == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_byte_stream(
	     header,
	     header_size,
	     codepage,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header string size.",
		 function );

		return( -1 );
	}
	header_string = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * header_string_size );

	if( header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_byte_stream(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	if( libewf_header_values_parse_utf8_header_string(
	     header_values,
	     header_string,
	     header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse header string.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Parse an EWF header2 for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header2(
     libfvalue_table_t *header_values,
     uint8_t *header2,
     size_t header2_size,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_parse_header2";
	size_t header_string_size = 0;

	if( header2 == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header2.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     header2,
	     header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header string size.",
		 function );

		return( -1 );
	}
	header_string = (uint8_t *) memory_allocate(
	                             sizeof( uint8_t ) * (size_t) header_string_size );

	if( header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     header_string,
	     header_string_size,
	     header2,
	     header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	if( libewf_header_values_parse_utf8_header_string(
	     header_values,
	     header_string,
	     header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse header string.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
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
     liberror_error_t **error )
{
	static char *function = "libewf_header_values_convert_utf8_header_string_to_header";

	if( header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( header == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header.",
		 function );

		return( -1 );
	}
	if( *header != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: header already created.",
		 function );

		return( -1 );
	}
	if( header_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header size.",
		 function );

		return( -1 );
	}
	*header = (uint8_t *) memory_allocate(
	                       sizeof( uint8_t ) * *header_size );

	if( *header == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header.",
		 function );

		*header_size = 0;

		return( -1 );
	}
	if( libuna_byte_stream_copy_from_utf8(
	     *header,
	     *header_size,
	     codepage,
	     header_string,
	     header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set header.",
		 function );

		memory_free(
		 header );

		*header      = NULL;
		*header_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Converts an UTF-8 encoded header string into a header2
 * Sets the header and header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_convert_utf8_header_string_to_header2(
     const uint8_t *header_string,
     size_t header_string_size,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	static char *function = "libewf_header_values_convert_utf8_header_string_to_header2";

	if( header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( header2 == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header2.",
		 function );

		return( -1 );
	}
	if( *header2 != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: header2 already created.",
		 function );

		return( -1 );
	}
	if( header2_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header2 size.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_stream_size_from_utf8(
	     header_string,
	     header_string_size,
	     header2_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine header2 size.",
		 function );

		return( -1 );
	}
	*header2 = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * *header2_size );

	if( *header2 == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header2.",
		 function );

		*header2_size = 0;

		return( -1 );
	}
	if( libuna_utf16_stream_copy_from_utf8(
	     *header2,
	     *header2_size,
	     LIBUNA_ENDIAN_LITTLE,
	     header_string,
	     header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set header2.",
		 function );

		memory_free(
		 header2 );

		*header2      = NULL;
		*header2_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate an UTF-8 encoded header string
 * Sets header string and header string size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_utf8_header_string(
     libfvalue_table_t *header_values,
     uint8_t header_string_type,
     uint8_t *newline_string,
     size_t newline_string_length,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libfvalue_value_t *acquiry_date_header_value             = NULL;
	libfvalue_value_t *acquiry_operating_system_header_value = NULL;
	libfvalue_value_t *acquiry_software_version_header_value = NULL;
	libfvalue_value_t *case_number_header_value              = NULL;
	libfvalue_value_t *compression_type_header_value         = NULL;
	libfvalue_value_t *description_header_value              = NULL;
	libfvalue_value_t *examiner_name_header_value            = NULL;
	libfvalue_value_t *evidence_number_header_value          = NULL;
	libfvalue_value_t *model_header_value                    = NULL;
	libfvalue_value_t *notes_header_value                    = NULL;
	libfvalue_value_t *password_header_value                 = NULL;
	libfvalue_value_t *serial_number_header_value            = NULL;
	libfvalue_value_t *system_date_header_value              = NULL;
	libfvalue_value_t *unknown_dc_header_value               = NULL;
	uint8_t *generated_acquiry_date                          = NULL;
	uint8_t *generated_system_date                           = NULL;
	char *generated_compression_type                         = NULL;
	char *generated_password                                 = NULL;
	char *generated_srce_section                             = NULL;
	char *generated_sub_section                              = NULL;
	static char *function                                    = "libewf_header_values_generate_utf8_header_string";
	size_t acquiry_date_string_length                        = 0;
	size_t acquiry_operating_system_string_length            = 0;
	size_t acquiry_software_version_string_length            = 0;
	size_t case_number_string_length                         = 0;
	size_t compression_type_string_length                    = 0;
	size_t description_string_length                         = 0;
	size_t evidence_number_string_length                     = 0;
	size_t examiner_name_string_length                       = 0;
	size_t generated_acquiry_date_size                       = 0;
	size_t generated_system_date_size                        = 0;
	size_t header_string_index                               = 0;
	size_t model_string_length                               = 0;
	size_t notes_string_length                               = 0;
	size_t password_string_length                            = 0;
	size_t srce_section_string_length                        = 0;
	size_t sub_section_string_length                         = 0;
	size_t serial_number_string_length                       = 0;
	size_t system_date_string_length                         = 0;
	size_t unknown_dc_string_length                          = 0;
	int number_of_tabs                                       = 0;
	int result                                               = 0;

	if( ( header_string_type != LIBEWF_HEADER_STRING_TYPE_1 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_2 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_3 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_4 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_5 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_6 )
	 && ( header_string_type != LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header string type.",
		 function );

		return( -1 );
	}
	if( newline_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid newline string.",
		 function );

		return( -1 );
	}
	if( ( newline_string_length != 1 )
	 && ( newline_string_length != 2 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported newline string length.",
		 function );

		return( -1 );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression level.",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string.",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: header string already created.",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "case_number",
	     12,
	     &case_number_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: case_number.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "description",
	     12,
	     &description_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: description.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "examiner_name",
	     14,
	     &examiner_name_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: examiner_name.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "evidence_number",
	     16,
	     &evidence_number_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: evidence_number.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "notes",
	     6,
	     &notes_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: notes.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "acquiry_date",
	     13,
	     &acquiry_date_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: acquiry_date.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "system_date",
	     12,
	     &system_date_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: system_date.",
		 function );

		return( -1 );
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		if( libfvalue_table_get_value_by_identifier(
		     header_values,
		     (uint8_t *) "acquiry_operating_system",
		     25,
		     &acquiry_operating_system_header_value,
		     0,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: acquiry_operating_system.",
			 function );

			return( -1 );
		}
		if( libfvalue_table_get_value_by_identifier(
		     header_values,
		     (uint8_t *) "acquiry_software_version",
		     25,
		     &acquiry_software_version_header_value,
		     0,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: acquiry_software_version.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "password",
	     9,
	     &password_header_value,
	     0,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: password.",
		 function );

		return( -1 );
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		if( libfvalue_table_get_value_by_identifier(
		     header_values,
		     (uint8_t *) "compression_type",
		     17,
		     &compression_type_header_value,
		     0,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: compression_type.",
			 function );

			return( -1 );
		}
	}
	if( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	{
		if( libfvalue_table_get_value_by_identifier(
		     header_values,
		     (uint8_t *) "model",
		     6,
		     &model_header_value,
		     0,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: model.",
			 function );

			return( -1 );
		}
		if( libfvalue_table_get_value_by_identifier(
		     header_values,
		     (uint8_t *) "serial_number",
		     14,
		     &serial_number_header_value,
		     0,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: serial_number.",
			 function );

			return( -1 );
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 ) )
	{
		if( libfvalue_table_get_value_by_identifier(
		     header_values,
		     (uint8_t *) "unknown_dc",
		     11,
		     &unknown_dc_header_value,
		     0,
		     error ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: unknown_dc.",
			 function );

			return( -1 );
		}
	}
	/* Determine the header string size
	 */
	*header_string_size = 0;

	/* Reserve space for:
	 * # <newline>
	 * main <newline>
	 */
	*header_string_size += 5 + ( 2 * newline_string_length );

	if( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	{
		/* Reserve space for:
		 * c <tab> n <tab> a <tab> e <tab> t <tab> m <tab> u <tab> p <tab> r <newline>
		 */
		number_of_tabs = 8;

		*header_string_size += 9 + number_of_tabs + newline_string_length;
	}
	else if( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	{
		/* Reserve space for:
		 * c <tab> n <tab> a <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <tab> r <newline>
		 */
		number_of_tabs = 10;

		*header_string_size += 13 + number_of_tabs + newline_string_length;
	}
	else if( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	{
		/* Reserve space for:
		 * c <tab> n <tab> a <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <newline>
		 */
		number_of_tabs = 9;

		*header_string_size += 12 + number_of_tabs + newline_string_length;
	}
	else if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		/* Reserve space for:
		 * a <tab> c <tab> n <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <newline>
		 */
		number_of_tabs = 9;

		*header_string_size += 12 + number_of_tabs + newline_string_length;
	}
	else if( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	{
		/* Reserve space for:
		 * a <tab> c <tab> n <tab> e <tab> t <tab> av <tab> ov <tab> m <tab> u <tab> p <tab> dc <newline>
		 */
		number_of_tabs = 10;

		*header_string_size += 14 + number_of_tabs + newline_string_length;
	}
	else if( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	{
		/* Reserve space for:
		 * a <tab> c <tab> n <tab> e <tab> t <tab> md <tab> sn <tab> av <tab> ov <tab> m <tab> u <tab> p <tab> dc <newline>
		 */
		number_of_tabs = 12;

		*header_string_size += 18 + number_of_tabs + newline_string_length;
	}
	if( case_number_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          case_number_header_value,
		          0,
		          &case_number_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: case_number.",
			 function );

			return( -1 );
		}
		if( case_number_string_length > 0 )
		{
			case_number_string_length -= 1;

			*header_string_size += case_number_string_length;
		}
	}
	if( description_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          description_header_value,
		          0,
		          &description_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: description.",
			 function );

			return( -1 );
		}
		if( description_string_length > 0 )
		{
			description_string_length -= 1;

			*header_string_size += description_string_length;
		}
	}
	if( examiner_name_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          examiner_name_header_value,
		          0,
		          &examiner_name_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: examiner_name.",
			 function );

			return( -1 );
		}
		if( examiner_name_string_length > 0 )
		{
			examiner_name_string_length -= 1;

			*header_string_size += examiner_name_string_length;
		}
	}
	if( evidence_number_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          evidence_number_header_value,
		          0,
		          &evidence_number_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: evidence_number.",
			 function );

			return( -1 );
		}
		if( evidence_number_string_length > 0 )
		{
			evidence_number_string_length -= 1;

			*header_string_size += evidence_number_string_length;
		}
	}
	if( notes_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          notes_header_value,
		          0,
		          &notes_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: notes.",
			 function );

			return( -1 );
		}
		if( notes_string_length > 0 )
		{
			notes_string_length -= 1;

			*header_string_size += notes_string_length;
		}
	}
	if( acquiry_date_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          acquiry_date_header_value,
		          0,
		          &acquiry_date_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: acquiry_date",
			 function );

			return( -1 );
		}
		if( acquiry_date_string_length > 0 )
		{
			acquiry_date_string_length -= 1;
		}
	}
	if( ( acquiry_date_header_value == NULL )
	 || ( acquiry_date_string_length == 0 ) )
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to generate acquiry date header value.",
			 function );

#if defined( HAVE_VERBOSE_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libnotify_print_error_backtrace(
				 *error );
			}
#endif
			liberror_error_free(
			 error );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			acquiry_date_string_length = libcstring_narrow_string_length(
			                              (char *) generated_acquiry_date );
		}
	}
	*header_string_size += acquiry_date_string_length;

	if( system_date_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          system_date_header_value,
		          0,
		          &system_date_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: system_date",
			 function );

			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		if( system_date_string_length > 0 )
		{
			system_date_string_length -= 1;
		}
	}
	if( ( system_date_header_value == NULL )
	 || ( system_date_string_length == 0 ) )
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to generate system date header value.",
			 function );

#if defined( HAVE_VERBOSE_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libnotify_print_error_backtrace(
				 *error );
			}
#endif
			liberror_error_free(
			 error );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			 system_date_string_length = libcstring_narrow_string_length(
			                              (char *) generated_system_date );
		}
	}
	*header_string_size += system_date_string_length;

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		if( acquiry_operating_system_header_value != NULL )
		{
			result = libfvalue_value_get_utf8_string_size(
				  acquiry_operating_system_header_value,
				  0,
				  &acquiry_operating_system_string_length,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string size of header value: acquiry_operating_system.",
				 function );

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			if( acquiry_operating_system_string_length > 0 )
			{
				acquiry_operating_system_string_length -= 1;

				*header_string_size += acquiry_operating_system_string_length;
			}
		}
		if( acquiry_software_version_header_value != NULL )
		{
			result = libfvalue_value_get_utf8_string_size(
				  acquiry_software_version_header_value,
				  0,
				  &acquiry_software_version_string_length,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string size of header value: acquiry_software_version.",
				 function );

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			if( acquiry_software_version_string_length > 0 )
			{
				acquiry_software_version_string_length -= 1;

				*header_string_size += acquiry_software_version_string_length;
			}
		}
	}
	if( password_header_value != NULL )
	{
		result = libfvalue_value_get_utf8_string_size(
		          password_header_value,
		          0,
		          &password_string_length,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: password",
			 function );

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		if( password_string_length > 0 )
		{
			password_string_length -= 1;
		}
	}
	if( ( password_header_value == NULL )
	 || ( password_string_length == 0 ) )
	{
		generated_password = "0";

		password_string_length = 1;
	}
	*header_string_size += password_string_length;

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		if( compression_type_header_value != NULL )
		{
			result = libfvalue_value_get_utf8_string_size(
				  compression_type_header_value,
				  0,
				  &compression_type_string_length,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string size of header value: compression_type",
				 function );

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			if( compression_type_string_length > 0 )
			{
				compression_type_string_length -= 1;
			}
		}
		if( ( compression_type_header_value == NULL )
		 || ( compression_type_string_length == 0 ) )
		{
			if( compression_level == EWF_COMPRESSION_NONE )
			{
				generated_compression_type = LIBEWF_COMPRESSION_TYPE_NONE;
			}
			else if( compression_level == EWF_COMPRESSION_FAST )
			{
				generated_compression_type = LIBEWF_COMPRESSION_TYPE_FAST;
			}
			else if( compression_level == EWF_COMPRESSION_BEST )
			{
				generated_compression_type = LIBEWF_COMPRESSION_TYPE_BEST;
			}
			compression_type_string_length = libcstring_narrow_string_length(
			                                  generated_compression_type );
		}
		*header_string_size += compression_type_string_length;
	}
	if( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	{
		if( model_header_value != NULL )
		{
			result = libfvalue_value_get_utf8_string_size(
				  model_header_value,
				  0,
				  &model_string_length,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string size of header value: model",
				 function );

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			if( model_string_length > 0 )
			{
				model_string_length -= 1;

				*header_string_size += model_string_length;
			}
		}
		if( serial_number_header_value != NULL )
		{
			result = libfvalue_value_get_utf8_string_size(
				  serial_number_header_value,
				  0,
				  &serial_number_string_length,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string size of header value: serial_number",
				 function );

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			if( serial_number_string_length > 0 )
			{
				serial_number_string_length -= 1;

				*header_string_size += serial_number_string_length;
			}
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 ) )
	{
		if( unknown_dc_header_value != NULL )
		{
			result = libfvalue_value_get_utf8_string_size(
				  unknown_dc_header_value,
				  0,
				  &unknown_dc_string_length,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string size of header value: unknown_dc",
				 function );

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			if( unknown_dc_string_length > 0 )
			{
				unknown_dc_string_length -= 1;

				*header_string_size += unknown_dc_string_length;
			}
		}
	}
	/* Reserve space for the tabs and 2 newlines
	 */
	*header_string_size += number_of_tabs + ( 2 * newline_string_length );

	/* Reserve space for additional sections
	 */
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		generated_srce_section = "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n";

		srce_section_string_length = libcstring_narrow_string_length(
		                              generated_srce_section );

		*header_string_size += srce_section_string_length;

		generated_sub_section = "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n";

		sub_section_string_length = libcstring_narrow_string_length(
		                             generated_sub_section );

		*header_string_size += sub_section_string_length;
	}
	/* Reserve space for the end-of-string character
	 */
	*header_string_size += 1;

	/* Determine the header string
	 */
	*header_string = (uint8_t *) memory_allocate(
	                              sizeof( uint8_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		*header_string_size = 0;

		if( generated_system_date != NULL )
		{
			memory_free(
			 generated_system_date );
		}
		if( generated_acquiry_date != NULL )
		{
			memory_free(
			 generated_acquiry_date );
		}
		return( -1 );
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '1';
	}
	else if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	      || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '3';
	}
	( *header_string )[ header_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		( *header_string )[ header_string_index++ ] = newline_string[ 1 ];
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) 'm';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'a';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'i';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'n';

	( *header_string )[ header_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		( *header_string )[ header_string_index++ ] = newline_string[ 1 ];
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) 'a';
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) 'c';
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'n';

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'a';
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'e';
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	( *header_string )[ header_string_index++ ] = (uint8_t) 't';

	if( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'm';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'd';
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
		( *header_string )[ header_string_index++ ] = (uint8_t) 's';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'n';
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'a';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'v';
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'o';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'v';
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'm';
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'u';
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	( *header_string )[ header_string_index++ ] = (uint8_t) 'p';

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'r';
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'd';
		( *header_string )[ header_string_index++ ] = (uint8_t) 'c';
	}
	( *header_string )[ header_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		( *header_string )[ header_string_index++ ] = newline_string[ 1 ];
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		if( ( description_header_value != NULL )
		 && ( description_string_length > 0 ) )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  description_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: description.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			header_string_index += description_string_length;
		}
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';
	}
	if( ( case_number_header_value != NULL )
	 && ( case_number_string_length > 0 ) )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          case_number_header_value,
		          0,
		          &( ( *header_string )[ header_string_index ] ),
		          *header_string_size - header_string_index,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string of header value: case_number.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		header_string_index += case_number_string_length;
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

	if( ( evidence_number_header_value != NULL )
	 && ( evidence_number_string_length > 0 ) )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          evidence_number_header_value,
		          0,
		          &( ( *header_string )[ header_string_index ] ),
		          *header_string_size - header_string_index,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string of header value: evidence_number.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		header_string_index += evidence_number_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

		if( ( description_header_value != NULL )
		 && ( description_string_length > 0 ) )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  description_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: description.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			header_string_index += description_string_length;
		}
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

	if( ( examiner_name_header_value != NULL )
	 && ( examiner_name_string_length > 0 ) )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          examiner_name_header_value,
		          0,
		          &( ( *header_string )[ header_string_index ] ),
		          *header_string_size - header_string_index,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string of header value: examiner_name.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		header_string_index += examiner_name_string_length;
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

	if( ( notes_header_value != NULL )
	 && ( notes_string_length > 0 ) )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          notes_header_value,
		          0,
		          &( ( *header_string )[ header_string_index ] ),
		          *header_string_size - header_string_index,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string of header value: notes.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		header_string_index += notes_string_length;
	}
	if( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

		if( ( model_header_value != NULL )
		 && ( model_string_length > 0 ) )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  model_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: model.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
			header_string_index += model_string_length;
		}
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

		if( ( serial_number_header_value != NULL )
		 && ( serial_number_string_length > 0 ) )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  serial_number_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: serial_number.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
			header_string_index += serial_number_string_length;
		}
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_3 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_4 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

		if( ( acquiry_operating_system_header_value != NULL )
		 && ( acquiry_operating_system_string_length > 0 ) )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  acquiry_operating_system_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: acquiry_operating_system.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			header_string_index += acquiry_operating_system_string_length;
		}
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

		if( ( acquiry_software_version_header_value != NULL )
		 && ( acquiry_software_version_string_length > 0 ) )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  acquiry_software_version_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: acquiry_software_version.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				if( generated_system_date != NULL )
				{
					memory_free(
					 generated_system_date );
				}
				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			header_string_index += acquiry_software_version_string_length;
		}
	}
	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

	if( generated_acquiry_date == NULL )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          acquiry_date_header_value,
		          0,
		          &( ( *header_string )[ header_string_index ] ),
		          *header_string_size - header_string_index,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string of header value: acquiry_date.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
	}
	else
	{
		if( libcstring_narrow_string_copy(
		     (char *) &( ( *header_string )[ header_string_index ] ),
		     (char *) generated_acquiry_date,
		     acquiry_date_string_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy acquiry date string.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		if( generated_acquiry_date != NULL )
		{
			memory_free(
			 generated_acquiry_date );
		}
	}
	header_string_index += acquiry_date_string_length;

	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

	if( generated_system_date == NULL )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          system_date_header_value,
		          0,
		          &( ( *header_string )[ header_string_index ] ),
		          *header_string_size - header_string_index,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string of header value: system_date.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			return( -1 );
		}
	}
	else
	{
		if( libcstring_narrow_string_copy(
		     (char *) &( ( *header_string )[ header_string_index ] ),
		     (char *) generated_system_date,
		     system_date_string_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy system date string.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_system_date != NULL )
			{
				memory_free(
				 generated_system_date );
			}
			return( -1 );
		}
		if( generated_system_date != NULL )
		{
			memory_free(
			 generated_system_date );
		}
	}
	header_string_index += system_date_string_length;

	( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

	if( generated_password == NULL )
	{
		result = libfvalue_value_copy_to_utf8_string(
		          password_header_value,
		          0,
		          &( ( *header_string )[ header_string_index ] ),
		          *header_string_size - header_string_index,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy string of header value: password.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			return( -1 );
		}
	}
	else
	{
		if( libcstring_narrow_string_copy(
		     (char *) &( ( *header_string )[ header_string_index ] ),
		     generated_password,
		     password_string_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy password string.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			return( -1 );
		}
	}
	header_string_index += password_string_length;

	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_1 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_2 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

		if( generated_compression_type == NULL )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  compression_type_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: compression_type.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
		}
		else
		{
			if( libcstring_narrow_string_copy(
			     (char *) &( ( *header_string )[ header_string_index ] ),
			     generated_compression_type,
			     compression_type_string_length ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy compression type string.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
		}
		header_string_index += compression_type_string_length;
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 ) )
	{
		( *header_string )[ header_string_index++ ] = (uint8_t) '\t';

		if( ( unknown_dc_header_value != NULL )
		 && ( unknown_dc_string_length > 0 ) )
		{
			result = libfvalue_value_copy_to_utf8_string(
				  unknown_dc_header_value,
				  0,
				  &( ( *header_string )[ header_string_index ] ),
			          *header_string_size - header_string_index,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: unknown_dc.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
			header_string_index += unknown_dc_string_length;
		}
	}
	( *header_string )[ header_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		( *header_string )[ header_string_index++ ] = newline_string[ 1 ];
	}
	( *header_string )[ header_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		( *header_string )[ header_string_index++ ] = newline_string[ 1 ];
	}
	if( ( header_string_type == LIBEWF_HEADER_STRING_TYPE_5 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_6 )
	 || ( header_string_type == LIBEWF_HEADER_STRING_TYPE_7 ) )
	{
		if( srce_section_string_length > 0 )
		{
			if( libcstring_narrow_string_copy(
			     (char *) &( ( *header_string )[ header_string_index ] ),
			     generated_srce_section,
			     srce_section_string_length ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy srce section string.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
			header_string_index += srce_section_string_length;
		}
		if( sub_section_string_length > 0 )
		{
			if( libcstring_narrow_string_copy(
			     (char *) &( ( *header_string )[ header_string_index ] ),
			     generated_sub_section,
			     sub_section_string_length ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy sub section string.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
			header_string_index += sub_section_string_length;
		}
	}
	( *header_string )[ header_string_index++ ] = 0;

	return( 1 );
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
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_ewf";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_1,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
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
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_encase1";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_1,
	     (uint8_t *) "\r\n",
	     2,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
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
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_ftk";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_2,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
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
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_encase2";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_2,
	     (uint8_t *) "\r\n",
	     2,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( 1 );
	}
	memory_free(
	 header_string );

	return( 1 );
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
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_encase4";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_3,
	     (uint8_t *) "\r\n",
	     2,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Generate a linen5 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_linen5(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_linen5";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_7,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Generate a linen6 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_linen6(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_linen6";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_6,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Generate an EnCase4 header2
 * Sets header2 and header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase4(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header2_encase4";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_4,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header2(
	     header_string,
	     header_string_size,
	     header2,
	     header2_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Generate an EnCase5 header2
 * Sets header2 and header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase5(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header2_encase5";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_5,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header2(
	     header_string,
	     header_string_size,
	     header2,
	     header2_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Generate an EnCase6 header2
 * Sets header2 and header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase6(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header2_encase6";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_6,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header2(
	     header_string,
	     header_string_size,
	     header2,
	     header2_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Convert date time values string within an xheader value
 * Sets date time values string and length
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_xheader_value(
     const uint8_t *header_value,
     size_t header_value_length,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_split_values_t *date_time_elements = NULL;
	static char *function                     = "libewf_convert_date_xheader_value";
	time_t timestamp                          = 0;
	size_t timezone_name_length               = 0;
	size_t timezone_string_length             = 0;
	int empty_date_element_correction         = 0;

	if( header_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header value.",
		 function );

		return( -1 );
	}
	if( date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libewf_split_values_parse_string(
	     &date_time_elements,
	     header_value,
	     header_value_length + 1,
	     (uint8_t) ' ',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header value into date time elements.",
		 function );

		return( -1 );
	}
	if( date_time_elements->number_of_values < 6 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of date time elements in header value.",
		 function );

		libewf_split_values_free(
		 &date_time_elements,
		 NULL );

		return( -1 );
	}
	/* The ctime formatted string use an additional space
	 * when the day is less than 10
	 */
	if( ( date_time_elements->values[ 2 ] )[ 0 ] == 0 )
	{
		empty_date_element_correction = 1;
	}
	/* Set the year
	 */
	time_elements.tm_year = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 0 ] - (uint8_t) '0' ) * 1000 )
			      + ( ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 1 ] - (uint8_t) '0' ) * 100 )
			      + ( ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 2 ] - (uint8_t) '0' ) * 10 )
			      + ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 3 ] - (uint8_t) '0' )
			      - 1900 );

	/* Set the month
	 */
	if( libcstring_narrow_string_compare(
	     date_time_elements->values[ 1 ],
	     "Jan",
	     3 ) == 0 )
	{
		time_elements.tm_mon = 0;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Feb",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 1;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Mar",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 2;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Apr",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 3;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "May",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 4;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Jun",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 5;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Jul",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 6;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Aug",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 7;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Sep",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 8;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Oct",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 9;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Nov",
	  3 ) == 0 )
	{
		time_elements.tm_mon = 10;
	}
	else if( libcstring_narrow_string_compare(
		  date_time_elements->values[ 1 ],
		  "Dec",
		  3 ) == 0 )
	{
		time_elements.tm_mon = 11;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported month string.",
		 function );

		libewf_split_values_free(
		 &date_time_elements,
		 NULL );

		return( -1 );
	}
	/* Set the day of the month
	 */
	if( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 1 ] == 0 )
	{
		time_elements.tm_mday = (int) ( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 0 ] - (uint8_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
				      + ( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 1 ] - (uint8_t) '0' ) );
	}
	/* Set the hour
	 */
	time_elements.tm_hour = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
			      + ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 1 ] - (uint8_t) '0' ) );

	/* Set the minutes
	 */
	time_elements.tm_min  = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 3 ] - (uint8_t) '0' ) * 10 )
			      + ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 4 ] - (uint8_t) '0' ) );

	/* Set the seconds
	 */
	time_elements.tm_sec = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 6 ] - (uint8_t) '0' ) * 10 )
			     + ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 7 ] - (uint8_t) '0' ) );

	/* Set to ignore the daylight saving time
	 */
	time_elements.tm_isdst = -1;

	/* Create a timestamp
	 */
	timestamp = libewf_date_time_mktime(
		     &time_elements );

	if( timestamp == (time_t) -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create timestamp.",
		 function );

		libewf_split_values_free(
		 &date_time_elements,
		 NULL );

		return( -1 );
	}
	*date_time_values_string_size = 20;

	if( ( date_time_elements->number_of_values - empty_date_element_correction ) > 5 )
	{
		timezone_string_length         = date_time_elements->sizes[ empty_date_element_correction + 5 ] - 1;
		*date_time_values_string_size += date_time_elements->sizes[ empty_date_element_correction + 5 ];
	}
	if( ( date_time_elements->number_of_values - empty_date_element_correction ) > 6 )
	{
		timezone_name_length           = date_time_elements->sizes[ empty_date_element_correction + 6 ] - 1;
		*date_time_values_string_size += date_time_elements->sizes[ empty_date_element_correction + 6 ];
	}
	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		libewf_split_values_free(
		 &date_time_elements,
		 NULL );

		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( libewf_date_time_values_copy_from_timestamp(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     timestamp,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to convert timestamp into date time values string.",
		 function );

		libewf_split_values_free(
		 &date_time_elements,
		 NULL );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( ( date_time_elements->number_of_values - empty_date_element_correction ) > 5 )
	{
		( *date_time_values_string )[ 19 ] = (uint8_t) ' ';

		if( libcstring_narrow_string_copy(
		     (char *) &( ( *date_time_values_string )[ 20 ] ),
		     (char *) date_time_elements->values[ empty_date_element_correction + 5 ],
		     timezone_string_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set timezone string in data time values string.",
			 function );

			libewf_split_values_free(
			 &date_time_elements,
			 NULL );

			memory_free(
			 *date_time_values_string );

			*date_time_values_string      = NULL;
			*date_time_values_string_size = 0;

			return( -1 );
		}
	}
	if( ( date_time_elements->number_of_values - empty_date_element_correction ) > 6 )
	{
		( *date_time_values_string )[ 20 + timezone_string_length ] = (uint8_t) ' ';

		if( libcstring_narrow_string_copy(
		     (char *) &( ( *date_time_values_string )[ 21 + timezone_string_length ] ),
		     (char *) date_time_elements->values[ empty_date_element_correction + 6 ],
		     timezone_name_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set timezone name in data time values string.",
			 function );

			libewf_split_values_free(
			 &date_time_elements,
			 NULL );

			memory_free(
			 *date_time_values_string );

			*date_time_values_string      = NULL;
			*date_time_values_string_size = 0;

			return( -1 );
		}
	}
	( *date_time_values_string )[ *date_time_values_string_size - 1 ] = 0;

	if( libewf_split_values_free(
	     &date_time_elements,
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

/* Generate date time values string within a xheader value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_xheader_value(
     time_t timestamp,
     uint8_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	static char *function                = "libewf_generate_date_xheader_value";
	size_t date_time_values_string_index = 0;
	int print_count                      = 0;

	if( date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string.",
		 function );

		return( -1 );
	}
	if( *date_time_values_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: date time values string already created.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( libewf_date_time_localtime(
	     &timestamp,
	     &time_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements.tm_year + 1900 ) > 10000 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported year value.",
		 function );

		return( -1 );
	}
	*date_time_values_string_size = 64;

	*date_time_values_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( libewf_utf8_string_day_of_week_copy_from_time_elements(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     &time_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of week to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libewf_utf8_string_month_copy_from_time_elements(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     &time_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy month to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( time_elements.tm_mday < 10 )
	{
		( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint8_t) time_elements.tm_mday,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy day of month to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libewf_utf8_string_time_copy_from_time_elements(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     &time_elements,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy time to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( libfvalue_utf8_string_decimal_copy_from_16bit(
	     *date_time_values_string,
	     *date_time_values_string_size,
	     &date_time_values_string_index,
	     (uint16_t) time_elements.tm_year + 1900,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy year to date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
#if defined( _BSD_SOURCE )
	time_elements.tm_gmtoff /= 60;

	print_count = libcstring_narrow_string_snprintf(
	               (char *) &( ( ( *date_time_values_string )[ date_time_values_string_index ] ) ),
		       *date_time_values_string_size - date_time_values_string_index,
		       "%+03ld:%02ld (%s)",
	               time_elements.tm_gmtoff / 60,
	               time_elements.tm_gmtoff % 60,
		       time_elements.tm_zone );

#elif defined( __BORLANDC__ )
	print_count = libcstring_narrow_string_snprintf(
	               (char *) &( ( ( *date_time_values_string )[ date_time_values_string_index ] ) ),
		       *date_time_values_string_size - date_time_values_string_index,
		       "%s",
		       _tzname[ time_elements.tm_isdst ] );

#else
	print_count = libcstring_narrow_string_snprintf(
	               (char *) &( ( ( *date_time_values_string )[ date_time_values_string_index ] ) ),
		       *date_time_values_string_size - date_time_values_string_index,
		       "%s",
		       tzname[ time_elements.tm_isdst ] );
#endif

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > ( *date_time_values_string_size - date_time_values_string_index ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	( *date_time_values_string )[ date_time_values_string_index ] = 0;

	return( 1 );
}

/* Parses a xheader for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_xheader(
     libfvalue_table_t *header_values,
     const uint8_t *xheader,
     size_t xheader_size,
     liberror_error_t **error )
{
	libewf_split_values_t *lines        = NULL;
	libfvalue_value_t *header_value     = NULL;
	uint8_t *close_tag_start            = NULL;
	uint8_t *close_tag_end              = NULL;
	uint8_t *date_time_values_string    = NULL;
	uint8_t *identifier                 = NULL;
	uint8_t *open_tag_start             = NULL;
	uint8_t *open_tag_end               = NULL;
	uint8_t *value_string               = NULL;
	static char *function               = "libewf_header_values_parse_xheader";
	size_t date_time_values_string_size = 0;
	size_t identifier_length            = 0;
	size_t string_length                = 0;
	size_t value_string_length          = 0;
	size_t xheader_index                = 0;
	int line_iterator                   = 0;

	if( xheader == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xheader",
		 function );

		return( -1 );
	}
	/* Check if xheader starts with an UTF-8 byte order mark (BOM)
	*/
        if( xheader_size >= 3 )
        {
                if( ( xheader[ 0 ] == 0x0ef )
                 && ( xheader[ 1 ] == 0x0bb )
                 && ( xheader[ 2 ] == 0x0bf ) )
                {
                        xheader_index += 3;
                }
        }
	if( libewf_split_values_parse_string(
	     &lines,
	     &( xheader[ xheader_index ] ),
	     xheader_size,
	     (uint8_t) '\n',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split header string into lines.",
		 function );

		return( -1 );
	}
/* TODO validate if enclosing tag is <xheader> */

	for( line_iterator = 0;
	     line_iterator < lines->number_of_values;
	     line_iterator++ )
	{
		/* Ignore empty lines
		 */
		if( ( lines->sizes[ line_iterator ] <= 1 )
		 || ( lines->values[ line_iterator ] == NULL )
		 || ( ( lines->values[ line_iterator ] )[ 0 ] == 0 ) )
		{
			continue;
		}
		string_length = lines->sizes[ line_iterator ] - 1;

		open_tag_start = (uint8_t *) libcstring_narrow_string_search_character(
		                              (char *) lines->values[ line_iterator ],
		                              '<',
		                              string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = (uint8_t *) libcstring_narrow_string_search_character(
		                            (char *) lines->values[ line_iterator ],
		                            '>',
		                            string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_end == NULL )
		{
			continue;
		}
		/* Ignore the first part of the XML string
		 */
		string_length -= (size_t) ( open_tag_end - lines->values[ line_iterator ] );

		/* Ignore lines only containing a single tag
		 */
		if( string_length <= 1 )
		{
			continue;
		}
		close_tag_start = (uint8_t *) libcstring_narrow_string_search_character_reverse(
		                               (char *) &open_tag_end[ 1 ],
		                               '<',
		                               string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = (uint8_t *) libcstring_narrow_string_search_character_reverse(
		                             (char *) &open_tag_end[ 1 ],
		                             '>',
		                             string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_end == NULL )
		{
			continue;
		}
		/* Ignore the second part of the XML string
		 */
		identifier        = &( open_tag_start[ 1 ] );
		identifier_length = (size_t) ( open_tag_end - open_tag_start ) - 1;

		/* Ignore the second part of the XML string
		 */
		value_string        = &( open_tag_end[ 1 ] );
		value_string_length = (size_t) ( close_tag_start - open_tag_end ) - 1;

		/* Make sure the string is terminated
		 */
		*open_tag_end = 0;

		if( ( ( identifier_length == 12 )
		   && ( libcstring_narrow_string_compare(
		         (char *) identifier,
		         "acquiry_date",
		         12 ) == 0 ) )
		 || ( ( identifier_length == 11 )
		   && ( libcstring_narrow_string_compare(
		         (char *) identifier,
		         "system_date",
		         11 ) == 0 ) ) )
		{
			if( libewf_convert_date_xheader_value(
			     &open_tag_end[ 1 ],
			     value_string_length,
			     &date_time_values_string,
			     &date_time_values_string_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_CONVERSION,
				 LIBERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to create date time values string.",
				 function );

				libewf_split_values_free(
				 &lines,
				 NULL );

				return( -1 );
			}
			else
			{
				/* Make sure to determine the actual length of the date time values string
				 */
				value_string_length = libcstring_narrow_string_length(
				                       (char *) date_time_values_string );
			}
		}
		if( libfvalue_value_initialize(
		     &header_value,
		     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
		     LIBFVALUE_VALUE_FLAG_IDENTIFIER_MANAGED | LIBFVALUE_VALUE_FLAG_DATA_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header value.",
			 function );

			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		if( libfvalue_value_set_identifier(
		     header_value,
		     identifier,
		     identifier_length + 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %s identifier.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 (intptr_t *) header_value,
			 NULL );

			if( date_time_values_string != NULL )
			{
				memory_free(
				 date_time_values_string );
			}
			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		if( libfvalue_value_set_data(
		     header_value,
		     value_string,
		     value_string_length,
		     LIBFVALUE_ENDIAN_NATIVE,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %s data.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 (intptr_t *) header_value,
			 NULL );

			if( date_time_values_string != NULL )
			{
				memory_free(
				 date_time_values_string );
			}
			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		if( libfvalue_table_set_value(
		     header_values,
		     header_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %s in table.",
			 function,
			 (char *) identifier );

			libfvalue_value_free(
			 (intptr_t *) header_value,
			 NULL );

			if( date_time_values_string != NULL )
			{
				memory_free(
				 date_time_values_string );
			}
			libewf_split_values_free(
			 &lines,
			 NULL );

			return( -1 );
		}
		header_value = NULL;

		if( date_time_values_string != NULL )
		{
			memory_free(
			 date_time_values_string );

			date_time_values_string = NULL;
		}
	}
	if( libewf_split_values_free(
	     &lines,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split lines.",
		 function );

		return( -1 );
	}
	return( 1 );
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
     liberror_error_t **error )
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xheader.",
		 function );

		return( -1 );
	}
	if( *xheader != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: xheader already created.",
		 function );

		return( -1 );
	}
	if( xheader_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_number_of_values(
	     header_values,
	     &number_of_header_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of header values.",
		 function );

		return( -1 );
	}
	xml_head = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	xml_head_length = libcstring_narrow_string_length(
	                   xml_head );

	xml_xheader_open_tag = "<xheader>\n";

	xml_xheader_open_tag_length = libcstring_narrow_string_length(
	                               xml_xheader_open_tag );

	xml_xheader_close_tag = "</xheader>\n\n";

	xml_xheader_close_tag_length = libcstring_narrow_string_length(
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %d.",
			 function,
			 header_value_index );

			return( -1 );
		}
		if( libfvalue_value_get_identifier(
		     header_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of header value: %d.",
			 function,
			 header_value_index );

			return( -1 );
		}
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data of header value: %s.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
		else if( result == 0 )
		{
			if( ( generated_acquiry_date == NULL )
			 && ( identifier_size == 13 )
			 && ( libcstring_narrow_string_compare(
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
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to generate acquiry date header value.",
					 function );

#if defined( HAVE_VERBOSE_OUTPUT )
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libnotify_print_error_backtrace(
						 *error );
					}
#endif
					liberror_error_free(
					 error );
				}
				else
				{
					acquiry_date_string_length = libcstring_narrow_string_length(
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

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: %s.",
			 function,
			 (char *) identifier );

			return( -1 );
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

	*xheader = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * *xheader_size );

	if( *xheader == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create xheader.",
		 function );

		*xheader_size = 0;

		if( generated_acquiry_date != NULL )
		{
			memory_free(
			 generated_acquiry_date );
		}
		return( -1 );
	}
	( *xheader )[ xheader_index++ ] = 0xef;
	( *xheader )[ xheader_index++ ] = 0xbb;
	( *xheader )[ xheader_index++ ] = 0xbf;

	if( libcstring_narrow_string_copy(
	     (char *) &( ( *xheader )[ xheader_index ] ),
	     xml_head,
	     xml_head_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy XML head string.",
		 function );

		memory_free(
		 *xheader );

		*xheader      = NULL;
		*xheader_size = 0;

		if( generated_acquiry_date != NULL )
		{
			memory_free(
			 generated_acquiry_date );
		}
		return( -1 );
	}
	xheader_index += xml_head_length;

	if( libcstring_narrow_string_copy(
	     (char *) &( ( *xheader )[ xheader_index ] ),
	     xml_xheader_open_tag,
	     xml_xheader_open_tag_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy xheader open tag string.",
		 function );

		memory_free(
		 *xheader );

		*xheader      = NULL;
		*xheader_size = 0;

		if( generated_acquiry_date != NULL )
		{
			memory_free(
			 generated_acquiry_date );
		}
		return( -1 );
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve header value: %d.",
			 function,
			 header_value_index );

			memory_free(
			 *xheader );

			*xheader      = NULL;
			*xheader_size = 0;

			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		if( libfvalue_value_get_identifier(
		     header_value,
		     &identifier,
		     &identifier_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier of header value: %d.",
			 function,
			 header_value_index );

			memory_free(
			 *xheader );

			*xheader      = NULL;
			*xheader_size = 0;

			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		if( ( identifier == NULL )
		 || ( identifier_size == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data of header value: %s.",
			 function,
			 (char *) identifier );

			memory_free(
			 *xheader );

			*xheader      = NULL;
			*xheader_size = 0;

			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
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

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: %s.",
			 function,
			 (char *) identifier );

			memory_free(
			 *xheader );

			*xheader      = NULL;
			*xheader_size = 0;

			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
		if( value_string_size > 1 )
		{
			( *xheader )[ xheader_index++ ] = (uint8_t) '\t';
			( *xheader )[ xheader_index++ ] = (uint8_t) '<';

			if( libcstring_narrow_string_copy(
			     (char *) &( ( *xheader )[ xheader_index ] ),
			     (char *) identifier,
			     identifier_size - 1 ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy %s open tag string.",
				 function,
				 (char *) identifier );

				memory_free(
				 *xheader );

				*xheader      = NULL;
				*xheader_size = 0;

				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			xheader_index += identifier_size - 1;

			( *xheader )[ xheader_index++ ] = (uint8_t) '>';

			result = libfvalue_value_copy_to_utf8_string(
				  header_value,
				  0,
				  &( ( *xheader )[ xheader_index ] ),
				  value_string_size,
				  error );

			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy string of header value: %s.",
				 function,
				 (char *) identifier );

				memory_free(
				 *xheader );

				*xheader      = NULL;
				*xheader_size = 0;

				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			xheader_index += value_string_size - 1;

			( *xheader )[ xheader_index++ ] = (uint8_t) '<';
			( *xheader )[ xheader_index++ ] = (uint8_t) '/';

			if( libcstring_narrow_string_copy(
			     (char *) &( ( *xheader )[ xheader_index ] ),
			     (char *) identifier,
			     identifier_size - 1 ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy %s close tag string.",
				 function,
				 (char *) identifier );

				memory_free(
				 *xheader );

				*xheader      = NULL;
				*xheader_size = 0;

				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			xheader_index += identifier_size - 1;

			( *xheader )[ xheader_index++ ] = (uint8_t) '>';
			( *xheader )[ xheader_index++ ] = (uint8_t) '\n';
		}
	}
	if( generated_acquiry_date != NULL )
	{
		if( libcstring_narrow_string_copy(
		     (char *) &( ( *xheader )[ xheader_index ] ),
		     "\t<acquiry_date>",
		     15 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy acquiry_date open tag string.",
			 function );

			memory_free(
			 *xheader );

			*xheader      = NULL;
			*xheader_size = 0;

			memory_free(
			 generated_acquiry_date );

			return( -1 );
		}
		xheader_index += 15;

		if( libcstring_narrow_string_copy(
		     (char *) &( ( *xheader )[ xheader_index ] ),
		     (char *) generated_acquiry_date,
		     acquiry_date_string_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy generated acquiry date string.",
			 function );

			memory_free(
			 *xheader );

			*xheader      = NULL;
			*xheader_size = 0;

			memory_free(
			 generated_acquiry_date );

			return( -1 );
		}
		xheader_index += acquiry_date_string_length;

		memory_free(
		 generated_acquiry_date );

		if( libcstring_narrow_string_copy(
		     (char *) &( ( *xheader )[ xheader_index ] ),
		     "</acquiry_date>\n",
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy acquiry_date close tag string.",
			 function );

			memory_free(
			 *xheader );

			*xheader      = NULL;
			*xheader_size = 0;

			return( -1 );
		}
		xheader_index += 16;
	}
	if( libcstring_narrow_string_copy(
	     (char *) &( ( *xheader )[ xheader_index ] ),
	     xml_xheader_close_tag,
	     xml_xheader_close_tag_length ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy xheader close tag string.",
		 function );

		memory_free(
		 *xheader );

		*xheader      = NULL;
		*xheader_size = 0;

		return( -1 );
	}
	xheader_index += xml_xheader_close_tag_length;

	/* Make sure the string is terminated
	 */
	( *xheader )[ xheader_index ] = 0;

	return( 1 );
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
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header_ewfx";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_3,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header(
	     header_string,
	     header_string_size,
	     header,
	     header_size,
	     codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}

/* Generate an EWFX header2
 * Sets header2 and the header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_ewfx(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	uint8_t *header_string    = NULL;
	static char *function     = "libewf_header_values_generate_header2_ewfx";
	size_t header_string_size = 0;

	if( libewf_header_values_generate_utf8_header_string(
	     header_values,
	     LIBEWF_HEADER_STRING_TYPE_4,
	     (uint8_t *) "\n",
	     1,
	     timestamp,
	     compression_level,
	     &header_string,
	     &header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header string.",
		 function );

		return( -1 );
	}
	if( libewf_header_values_convert_utf8_header_string_to_header2(
	     header_string,
	     header_string_size,
	     header2,
	     header2_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	memory_free(
	 header_string );

	return( 1 );
}


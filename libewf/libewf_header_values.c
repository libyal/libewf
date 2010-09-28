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
#include "libewf_string.h"

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
     liberror_error_t **error );
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
	     header_value_index,
	     header_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header value: %d in table.",
		 function,
		 header_value_index );

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
     libcstring_character_t *header_value,
     size_t header_value_length,
     libcstring_character_t **date_time_values_string,
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
	     (libcstring_character_t) ' ',
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
	time_elements.tm_year = (int) ( ( ( ( date_time_elements->values[ 0 ] )[ 0 ] - (libcstring_character_t) '0' ) * 1000 )
	                      + ( ( ( date_time_elements->values[ 0 ] )[ 1 ] - (libcstring_character_t) '0' ) * 100 )
	                      + ( ( ( date_time_elements->values[ 0 ] )[ 2 ] - (libcstring_character_t) '0' ) * 10 )
	                      + ( ( date_time_elements->values[ 0 ] )[ 3 ] - (libcstring_character_t) '0' )
	                      - 1900 );

	/* Set the month
	 */
	if( ( date_time_elements->values[ 1 ] )[ 1 ] == 0 )
	{
		time_elements.tm_mon = (int) ( ( ( date_time_elements->values[ 1 ] )[ 0 ] - (libcstring_character_t) '0' )
		                     - 1 );
	}
	else
	{
		time_elements.tm_mon = (int) ( ( ( ( date_time_elements->values[ 1 ] )[ 0 ] - (libcstring_character_t) '0' ) * 10 )
		                     + ( ( date_time_elements->values[ 1 ] )[ 1 ] - (libcstring_character_t) '0' )
		                     - 1 );
	}
	/* Set the day of the month
	 */
	if( ( date_time_elements->values[ 2 ] )[ 1 ] == 0 )
	{
		time_elements.tm_mday = (int) ( ( date_time_elements->values[ 2 ] )[ 0 ] - (libcstring_character_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( ( date_time_elements->values[ 2 ] )[ 0 ] - (libcstring_character_t) '0' ) * 10 )
		                      + ( ( date_time_elements->values[ 2 ] )[ 1 ] - (libcstring_character_t) '0' ) );
	}
	/* Set the hour
	 */
	if( ( date_time_elements->values[ 3 ] )[ 1 ] == 0 )
	{
		time_elements.tm_hour = (int) ( ( date_time_elements->values[ 3 ] )[ 0 ] - (libcstring_character_t) '0' );
	}
	else
	{
		time_elements.tm_hour = (int) ( ( ( ( date_time_elements->values[ 3 ] )[ 0 ] - (libcstring_character_t) '0' ) * 10 )
		                      + ( ( date_time_elements->values[ 3 ] )[ 1 ] - (libcstring_character_t) '0' ) );
	}
	/* Set the minutes
	 */
	if( ( date_time_elements->values[ 4 ] )[ 1 ] == 0 )
	{
		time_elements.tm_min = (int) ( ( date_time_elements->values[ 4 ] )[ 0 ] - (libcstring_character_t) '0' );
	}
	else
	{
		time_elements.tm_min = (int) ( ( ( ( date_time_elements->values[ 4 ] )[ 0 ] - (libcstring_character_t) '0' ) * 10 )
		                     + ( ( date_time_elements->values[ 4 ] )[ 1 ] - (libcstring_character_t) '0' ) );
	}
	/* Set the seconds
	 */
	if( ( date_time_elements->values[ 5 ] )[ 1 ] == 0 )
	{
		time_elements.tm_sec = (int) ( ( date_time_elements->values[ 5 ] )[ 0 ] - (libcstring_character_t) '0' );
	}
	else
	{
		time_elements.tm_sec = (int) ( ( ( ( date_time_elements->values[ 5 ] )[ 0 ] - (libcstring_character_t) '0' ) * 10 )
		                     + ( ( date_time_elements->values[ 5 ] )[ 1 ] - (libcstring_character_t) '0' ) );
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

	*date_time_values_string = (libcstring_character_t *) memory_allocate(
	                                                       sizeof( libcstring_character_t ) * *date_time_values_string_size );

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

/* Generate date time values string within a header value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header_value(
     time_t timestamp,
     libcstring_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	static char *function = "libewf_generate_date_header_value";
	int print_count       = 0;

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
	*date_time_values_string_size = 20;

	*date_time_values_string = (libcstring_character_t *) memory_allocate(
	                                                       sizeof( libcstring_character_t ) * *date_time_values_string_size );

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
	print_count = libcstring_string_snprintf(
	               *date_time_values_string,
	               *date_time_values_string_size,
	               "%4d %d %d %d %d %d",
	               time_elements.tm_year + 1900,
	               time_elements.tm_mon + 1,
	               time_elements.tm_mday,
	               time_elements.tm_hour,
	               time_elements.tm_min,
	               time_elements.tm_sec );

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *date_time_values_string_size ) )
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
	return( 1 );
}

/* Convert a header2 value into a date time values string
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header2_value(
     libcstring_character_t *header_value,
     size_t header_value_length,
     libcstring_character_t **date_time_values_string,
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
	if( libewf_string_copy_to_64bit_decimal(
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

	*date_time_values_string = (libcstring_character_t *) memory_allocate(
	                                                       sizeof( libcstring_character_t ) * *date_time_values_string_size );

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

/* Generate date time values string within a header2 value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header2_value(
     time_t timestamp,
     libcstring_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	static char *function = "libewf_generate_date_header2_value";
	int print_count       = 0;

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

	*date_time_values_string = (libcstring_character_t *) memory_allocate(
	                                                       sizeof( libcstring_character_t ) * *date_time_values_string_size );

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
	print_count = libcstring_string_snprintf(
	               *date_time_values_string,
	               *date_time_values_string_size,
	               "%" PRIu32 "",
	               (uint32_t) timestamp );

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *date_time_values_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create date time values string.",
		 function );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
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
	uint8_t *data                               = NULL;
	uint8_t *identifier                         = NULL;
	static char *function                       = "libewf_header_values_copy";
	size_t data_size                            = 0;
	size_t identifier_size                      = 0;
	uint8_t byte_order                          = 0;
	int header_value_index                      = 0;
	int number_of_header_values                 = 0;

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
		      && ( libcstring_narrow_compare(
		            (char *) identifier,
		            "acquiry_date",
		            12 ) == 0 ) )
		{
			continue;
		}
		else if( ( identifier_size == 12 )
		      && ( libcstring_narrow_compare(
		            (char *) identifier,
		            "system_date",
		            11 ) == 0 ) )
		{
			continue;
		}
		/* Ignore empty values
		 */
		if( libfvalue_value_get_data(
		     header_value,
		     &data,
		     &data_size,
		     &byte_order,
		     error ) != 1 )
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
                if( ( data != NULL )
                 && ( data_size > 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: missing data for header value: %d.\n",
				 function,
				 header_value_index );
			}
#endif
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

/* Parse a header string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header_string(
     libfvalue_table_t *header_values,
     libcstring_character_t *header_string,
     size_t header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *date_time_values_string = NULL;
	libcstring_character_t *value_string            = NULL;
	libewf_split_values_t *lines                    = NULL;
	libewf_split_values_t *types                    = NULL;
	libewf_split_values_t *values                   = NULL;
	libfvalue_value_t *header_value                 = NULL;
	uint8_t *identifier                             = NULL;
	static char *function                           = "libewf_header_values_parse_header_string";
	size_t date_time_values_string_size             = 0;
	size_t identifier_size                          = 0;
	size_t type_string_length                       = 0;
	size_t value_string_length                      = 0;
	int iterator                                    = 0;
	int result                                      = 0;

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
	     (libcstring_character_t) '\n',
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
		if( ( ( lines->values[ 0 ] )[ 0 ] < (libcstring_character_t) '0' )
		 || ( ( lines->values[ 0 ] )[ 0 ] > (libcstring_character_t) '9' ) )
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
		     (libcstring_character_t) '\t',
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
		     (libcstring_character_t) '\t',
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
			 && ( ( types->values[ iterator ] )[ type_string_length - 1 ] == (libcstring_character_t) '\r' ) )
			{
				type_string_length -= 1;
			}
			if( ( value_string_length > 0 )
			 && ( value_string[ value_string_length - 1 ] == (libcstring_character_t) '\r' ) )
			{
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
				if( libcstring_string_compare(
				     types->values[ iterator ],
				     _LIBCSTRING_STRING( "ext" ),
				     type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "extents";
					identifier_size = 8;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "pid" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "process_identifier";
					identifier_size = 19;
				}
			}
			else if( type_string_length == 2 )
			{
				if( libcstring_string_compare(
				     types->values[ iterator ],
				     _LIBCSTRING_STRING( "av" ),
				     type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "acquiry_software_version";
					identifier_size = 25;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "dc" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "unknown_dc";
					identifier_size = 11;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "md" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "model";
					identifier_size = 6;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "ov" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "acquiry_operating_system";
					identifier_size = 25;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "sn" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "serial_number";
					identifier_size = 14;
				}
			}
			else if( type_string_length == 1 )
			{
				if( ( libcstring_string_compare(
				       types->values[ iterator ],
				       _LIBCSTRING_STRING( "m" ),
				       type_string_length ) == 0 )
				 || ( libcstring_string_compare(
				       types->values[ iterator ],
				       _LIBCSTRING_STRING( "u" ),
				       type_string_length ) == 0 ) )
				{
					if( ( value_string == NULL )
					 || ( value_string == (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
					{
						/* the string search function is not NULL safe */
					}
					/* If the date time values string contains spaces it's in the old header
					 * format otherwise is in new header2 format
					 */
					else if( libcstring_string_search_character(
					          value_string,
					          (libcstring_character_t) ' ',
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
							value_string_length = libcstring_string_length(
									       date_time_values_string );

						}
						if( libcstring_string_compare(
						     types->values[ iterator ],
						     _LIBCSTRING_STRING( "m" ),
						     1 ) == 0 )
						{
							identifier      = (uint8_t *) "acquiry_date";
							identifier_size = 13;
							value_string    = date_time_values_string;
						}
						else if( libcstring_string_compare(
							  types->values[ iterator ],
							  _LIBCSTRING_STRING( "u" ),
							  1 ) == 0 )
						{
							identifier      = (uint8_t *) "system_date";
							identifier_size = 12;
							value_string    = date_time_values_string;
						}
					}
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "p" ),
					  type_string_length ) == 0 )
				{
					if( value_string_length == 0 )
					{
						/* Empty hash do nothing
						 */
					}
					else if( ( value_string_length == 1 )
					 && ( value_string[ 0 ] == (libcstring_character_t) '0' ) )
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
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "a" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "description";
					identifier_size = 12;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "c" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "case_number";
					identifier_size = 12;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "n" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "evidence_number";
					identifier_size = 16;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "e" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "examiner_name";
					identifier_size = 14;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "t" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "notes";
					identifier_size = 6;
				}
				else if( libcstring_string_compare(
					  types->values[ iterator ],
					  _LIBCSTRING_STRING( "r" ),
					  type_string_length ) == 0 )
				{
					identifier      = (uint8_t *) "compression_type";
					identifier_size = 17;
				}
			}
			if( identifier != NULL )
			{
				if( libfvalue_value_initialize(
				     header_value,
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
	libcstring_character_t *header_string = NULL;
	static char *function                 = "libewf_header_values_parse_header";
	size_t header_string_size             = 0;
	int result                            = 0;

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
	header_string = (libcstring_character_t *) memory_allocate(
	                                            sizeof( libcstring_character_t ) * header_string_size );

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
	result = libewf_header_values_parse_header_string(
	          header_values,
	          header_string,
	          header_string_size,
	          error );

	memory_free(
	 header_string );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse header string.",
		 function );
	}
	return( result );
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
	libcstring_character_t *header_string = NULL;
	static char *function                 = "libewf_header_values_parse_header2";
	size_t header_string_size             = 0;
	int result                            = 0;

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
	header_string = (libcstring_character_t *) memory_allocate(
	                                            sizeof( libcstring_character_t ) * (size_t) header_string_size );

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
	result = libewf_header_values_parse_header_string(
	          header_values,
	          header_string,
	          header_string_size,
	          error );

	memory_free(
	 header_string );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse header string.",
		 function );
	}
	return( result );
}

/* Converts a header string into a header
 * Sets the header and header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_convert_header_string_to_header(
     libcstring_character_t *header_string,
     size_t header_string_size,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	static char *function = "libewf_header_values_convert_header_string_to_header";

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

/* Converts a header string into a header2
 * Sets the header and header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_convert_header_string_to_header2(
     libcstring_character_t *header_string,
     size_t header_string_size,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	static char *function = "libewf_header_values_convert_header_string_to_header2";

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

/* Generate a header string format type 1 (original EWF, EnCase1)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type1(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     libcstring_character_t *header_string_head,
     libcstring_character_t *header_string_tail,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *generated_acquiry_date   = NULL;
	libcstring_character_t *generated_system_date    = NULL;
	libfvalue_value_t *acquiry_date_header_value     = NULL;
	libfvalue_value_t *case_number_header_value      = NULL;
	libfvalue_value_t *compression_type_header_value = NULL;
	libfvalue_value_t *description_header_value      = NULL;
	libfvalue_value_t *examiner_name_header_value    = NULL;
	libfvalue_value_t *evidence_number_header_value  = NULL;
	libfvalue_value_t *notes_header_value            = NULL;
	libfvalue_value_t *password_header_value         = NULL;
	libfvalue_value_t *system_date_header_value      = NULL;
	static char *function                            = "libewf_header_values_generate_header_string_type1";
	size_t generated_acquiry_date_size               = 0;
	size_t generated_system_date_size                = 0;
	size_t header_string_head_size                   = 0;
	size_t header_string_index                       = 0;
	size_t value_string_size                         = 0;

	if( header_string_head == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string head.",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string tail.",
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
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: compression level not supported.",
		 function );

		return( -1 );
	}
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "case_number",
	     12,
	     &case_number_header_value,
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
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "password",
	     9,
	     &password_header_value,
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
	if( libfvalue_table_get_value_by_identifier(
	     header_values,
	     (uint8_t *) "compression_type",
	     17,
	     &compression_type_header_value,
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
	/* Determine the header string size
	 */
	header_string_head_size = libcstring_string_length(
	                           header_string_head );

	*header_string_size = header_string_head_size;

	if( case_number_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     case_number_header_value,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: case_number.",
			 function );

			return( -1 );
		}
		*header_string_size += value_string_size - 1;
	}
	if( description_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     description_header_value,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: description.",
			 function );

			return( -1 );
		}
		*header_string_size += value_string_size - 1;
	}
	if( examiner_name_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     examiner_name_header_value,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: examiner_name.",
			 function );

			return( -1 );
		}
		*header_string_size += value_string_size - 1;
	}
	if( evidence_number_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     evidence_number_header_value,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: evidence_number.",
			 function );

			return( -1 );
		}
		*header_string_size += value_string_size - 1;
	}
	if( notes_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     notes_header_value,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: notes.",
			 function );

			return( -1 );
		}
		*header_string_size += value_string_size - 1;
	}
	value_string_size = 0;

	if( acquiry_date_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     acquiry_date_header_value,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size of header value: acquiry_date",
			 function );

			return( -1 );
		}
	}
	if( ( acquiry_date_header_value != NULL )
	 && ( value_string_size > 0 ) )
	{
		*header_string_size += value_string_size - 1;
	}
	else
	{
		if( libewf_generate_date_header_value(
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
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
						generated_acquiry_date );
		}
	}
	value_string_size = 0;

	if( system_date_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     system_date_header_value,
		     &value_string_size,
		     error ) != 1 )
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
	}
	if( ( system_date_header_value != NULL )
	 && ( value_string_size > 0 ) )
	{
		*header_string_size += value_string_size - 1;
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &generated_system_date,
		     &generated_system_date_size,
		     error ) != 1 )
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
			 *header_string_size += libcstring_string_length(
			                         generated_system_date );
		}
	}
	value_string_size = 0;

	if( password_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     password_header_value,
		     &value_string_size,
		     error ) != 1 )
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
	}
	if( ( password_header_value != NULL )
	 && ( value_string_size > 0 ) )
	{
		*header_string_size += value_string_size - 1;
	}
	else
	{
		*header_string_size += 1;

		password_hash = _LIBCSTRING_STRING( "0" );
	}
	value_string_size = 0;

	if( compression_type_header_value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     compression_type_header_value,
		     &value_string_size,
		     error ) != 1 )
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
	}
	if( ( compression_type_header_value != NULL )
	 && ( value_string_size > 0 ) )
	{
		*header_string_size += value_string_size - 1;
	}
	else
	{
		if( compression_level == EWF_COMPRESSION_NONE )
		{
			compression_type = _LIBCSTRING_STRING( LIBEWF_COMPRESSION_TYPE_NONE );
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = _LIBCSTRING_STRING( LIBEWF_COMPRESSION_TYPE_FAST );
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = _LIBCSTRING_STRING( LIBEWF_COMPRESSION_TYPE_BEST );
		}
		*header_string_size += libcstring_string_length(
		                        compression_type );
	}
	*header_string_size += libcstring_string_length(
	                        header_string_tail );

	/* allow for 8x \t and 1x \0
	 */
	*header_string_size += 9;

	/* Determine the header string
	 */
	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

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
	if( libcstring_string_copy(
	     *header_string,
	     header_string_head,
	     header_string_head_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy header string head.",
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
	header_string_index = header_string_head_size - 1;

/* TODO */

	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s",
	               (char *) header_string_head,
	               (char *) case_number,
	               (char *) evidence_number,
	               (char *) description,
	               (char *) examiner_name,
	               (char *) notes,
	               (char *) acquiry_date,
	               (char *) system_date,
	               (char *) password_hash,
	               (char *) compression_type,
	               (char *) header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a header format type 2 (EnCase2, EnCase3, FTK Imager 2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type2(
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     libcstring_character_t *header_string_head,
     libcstring_character_t *header_string_tail,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *case_number              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *description              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *examiner_name            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *evidence_number          = _LIBCSTRING_STRING( "" );
	libcstring_character_t *notes                    = _LIBCSTRING_STRING( "" );
	libcstring_character_t *system_date              = NULL;
	libcstring_character_t *acquiry_date             = NULL;
	libcstring_character_t *acquiry_operating_system = _LIBCSTRING_STRING( "" );
	libcstring_character_t *acquiry_software_version = _LIBCSTRING_STRING( "" );
	libcstring_character_t *password_hash            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *compression_type         = _LIBCSTRING_STRING( "" );
	static char *function                            = "libewf_header_values_generate_header_string_type2";
	size_t acquiry_date_size                         = 0;
	size_t system_date_size                          = 0;
	int number_of_header_values                      = 0;
	int print_count                                  = 0;

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
	if( header_string_head == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string head.",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string tail.",
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
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: compression level not supported.",
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
	if( number_of_header_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of header values value out of bounds.",
		 function );

		return( -1 );
	}
	*header_string_size = libcstring_string_length(
	                       header_string_head );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size,
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

			acquiry_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &system_date,
		     &system_date_size,
		     error ) != 1 )
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

			system_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*header_string_size += 1;

		password_hash = _LIBCSTRING_STRING( "0" );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];

		compression_type = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];
	}
	else
	{
		if( compression_level == EWF_COMPRESSION_NONE )
		{
			compression_type = _LIBCSTRING_STRING( LIBEWF_COMPRESSION_TYPE_NONE );;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = _LIBCSTRING_STRING( LIBEWF_COMPRESSION_TYPE_FAST );
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = _LIBCSTRING_STRING( LIBEWF_COMPRESSION_TYPE_BEST );
		}
		*header_string_size += libcstring_string_length(
		                        compression_type );
	}
	*header_string_size += libcstring_string_length(
	                        header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*header_string_size += 11;

	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s",
	               (char *) header_string_head,
	               (char *) case_number,
	               (char *) evidence_number,
	               (char *) description,
	               (char *) examiner_name,
	               (char *) notes,
	               (char *) acquiry_software_version,
	               (char *) acquiry_operating_system,
	               (char *) acquiry_date,
	               (char *) system_date,
	               (char *) password_hash,
	               (char *) compression_type,
	               (char *) header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a header format type 3 (EnCase4, EnCase5)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type3(
     libfvalue_table_t *header_values,
     time_t timestamp,
     libcstring_character_t *header_string_head,
     libcstring_character_t *header_string_tail,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *case_number              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *description              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *examiner_name            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *evidence_number          = _LIBCSTRING_STRING( "" );
	libcstring_character_t *notes                    = _LIBCSTRING_STRING( "" );
	libcstring_character_t *system_date              = NULL;
	libcstring_character_t *acquiry_date             = NULL;
	libcstring_character_t *acquiry_operating_system = _LIBCSTRING_STRING( "" );
	libcstring_character_t *acquiry_software_version = _LIBCSTRING_STRING( "" );
	libcstring_character_t *password_hash            = _LIBCSTRING_STRING( "" );
	static char *function                            = "libewf_header_values_generate_header_string_type3";
	size_t system_date_size                          = 0;
	size_t acquiry_date_size                         = 0;
	int print_count                                  = 0;

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
	if( header_values->number_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: missing default header values.",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing values.",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing value lengths.",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string head.",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string tail.",
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
	*header_string_size = libcstring_string_length(
	                       header_string_head );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size,
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

			acquiry_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &system_date,
		     &system_date_size,
		     error ) != 1 )
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

			system_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*header_string_size += 1;

		password_hash = _LIBCSTRING_STRING( "0" );
	}
	*header_string_size += libcstring_string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s",
	               (char *) header_string_head,
	               (char *) case_number,
	               (char *) evidence_number,
	               (char *) description,
	               (char *) examiner_name,
	               (char *) notes,
	               (char *) acquiry_software_version,
	               (char *) acquiry_operating_system,
	               (char *) acquiry_date,
	               (char *) system_date,
	               (char *) password_hash,
	               (char *) header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a header format type 4 (EnCase4 header2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type4(
     libfvalue_table_t *header_values,
     time_t timestamp,
     libcstring_character_t *header_string_head,
     libcstring_character_t *header_string_tail,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *case_number              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *description              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *examiner_name            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *evidence_number          = _LIBCSTRING_STRING( "" );
	libcstring_character_t *notes                    = _LIBCSTRING_STRING( "" );
	libcstring_character_t *system_date              = NULL;
	libcstring_character_t *acquiry_date             = NULL;
	libcstring_character_t *acquiry_operating_system = _LIBCSTRING_STRING( "" );
	libcstring_character_t *acquiry_software_version = _LIBCSTRING_STRING( "" );
	libcstring_character_t *password_hash            = _LIBCSTRING_STRING( "" );
	static char *function                            = "libewf_header_values_generate_header_string_type4";
	size_t system_date_size                          = 0;
	size_t acquiry_date_size                         = 0;
	int print_count                                  = 0;

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
	if( header_values->number_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: missing default header values.",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing values.",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing value lengths.",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string head.",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string tail.",
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
	*header_string_size = libcstring_string_length(
	                       header_string_head );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size,
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

			acquiry_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size,
		     error ) != 1 )
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

			system_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	*header_string_size += libcstring_string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s",
	               (char *) header_string_head,
	               (char *) description,
	               (char *) case_number,
	               (char *) evidence_number,
	               (char *) examiner_name,
	               (char *) notes,
	               (char *) acquiry_software_version,
	               (char *) acquiry_operating_system,
	               (char *) acquiry_date,
	               (char *) system_date,
	               (char *) password_hash,
	               (char *) header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a header format type 5 (EnCase5 header2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type5(
     libfvalue_table_t *header_values,
     time_t timestamp,
     libcstring_character_t *header_string_head,
     libcstring_character_t *header_string_tail,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string_srce       = _LIBCSTRING_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libcstring_character_t *header_string_sub        = _LIBCSTRING_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libcstring_character_t *case_number              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *description              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *examiner_name            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *evidence_number          = _LIBCSTRING_STRING( "" );
	libcstring_character_t *notes                    = _LIBCSTRING_STRING( "" );
	libcstring_character_t *system_date              = NULL;
	libcstring_character_t *acquiry_date             = NULL;
	libcstring_character_t *acquiry_operating_system = _LIBCSTRING_STRING( "" );
	libcstring_character_t *acquiry_software_version = _LIBCSTRING_STRING( "" );
	libcstring_character_t *password_hash            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *unknown_dc               = _LIBCSTRING_STRING( "" );
	static char *function                            = "libewf_header_values_generate_header_string_type5";
	size_t system_date_size                          = 0;
	size_t acquiry_date_size                         = 0;
	int print_count                                  = 0;

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
	if( header_values->number_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: missing default header values.",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing values.",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing value lengths.",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string head.",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string tail.",
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
	*header_string_size = libcstring_string_length(
	                       header_string_head );

	*header_string_size += libcstring_string_length(
	                        header_string_srce );

	*header_string_size += libcstring_string_length(
	                        header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size,
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

			acquiry_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size,
		     error ) != 1 )
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

			system_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];

		unknown_dc = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];
	}
	*header_string_size += libcstring_string_length(
	                        header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*header_string_size += 11;

	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s",
	               (char *) header_string_head,
	               (char *) description,
	               (char *) case_number,
	               (char *) evidence_number,
	               (char *) examiner_name,
	               (char *) notes,
	               (char *) acquiry_software_version,
	               (char *) acquiry_operating_system,
	               (char *) acquiry_date,
	               (char *) system_date,
	               (char *) password_hash,
	               (char *) unknown_dc,
	               (char *) header_string_tail,
	               (char *) header_string_srce,
	               (char *) header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a header format type 6 (EnCase6 header2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type6(
     libfvalue_table_t *header_values,
     time_t timestamp,
     libcstring_character_t *header_string_head,
     libcstring_character_t *header_string_tail,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string_srce       = _LIBCSTRING_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libcstring_character_t *header_string_sub        = _LIBCSTRING_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libcstring_character_t *case_number              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *description              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *examiner_name            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *evidence_number          = _LIBCSTRING_STRING( "" );
	libcstring_character_t *notes                    = _LIBCSTRING_STRING( "" );
	libcstring_character_t *system_date              = NULL;
	libcstring_character_t *acquiry_date             = NULL;
	libcstring_character_t *acquiry_operating_system = _LIBCSTRING_STRING( "" );
	libcstring_character_t *acquiry_software_version = _LIBCSTRING_STRING( "" );
	libcstring_character_t *password_hash            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *model                    = _LIBCSTRING_STRING( "" );
	libcstring_character_t *serial_number            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *unknown_dc               = _LIBCSTRING_STRING( "" );
	static char *function                            = "libewf_header_values_generate_header_string_type6";
	size_t system_date_size                          = 0;
	size_t acquiry_date_size                         = 0;
	int print_count                                  = 0;

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
	if( header_values->number_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: missing default header values.",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing values.",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing value lengths.",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string head.",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string tail.",
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
	*header_string_size = libcstring_string_length(
	                       header_string_head );

	*header_string_size += libcstring_string_length(
	                        header_string_srce );

	*header_string_size += libcstring_string_length(
	                        header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size,
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

			acquiry_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size,
		     error ) != 1 )
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

			system_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_MODEL ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_MODEL ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_MODEL ];

		model = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_MODEL ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ];

		serial_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];

		unknown_dc = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];
	}
	*header_string_size += libcstring_string_length(
	                        header_string_tail );

	/* allow for 12x \t and 1x \0
	 */
	*header_string_size += 13;

	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s",
	               (char *) header_string_head,
	               (char *) description,
	               (char *) case_number,
	               (char *) evidence_number,
	               (char *) examiner_name,
	               (char *) notes,
	               (char *) model,
	               (char *) serial_number,
	               (char *) acquiry_software_version,
	               (char *) acquiry_operating_system,
	               (char *) acquiry_date,
	               (char *) system_date,
	               (char *) password_hash,
	               (char *) unknown_dc,
	               (char *) header_string_tail,
	               (char *) header_string_srce,
	               (char *) header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a header format type 7 (EnCase5 linen header)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type7(
     libfvalue_table_t *header_values,
     time_t timestamp,
     libcstring_character_t *header_string_head,
     libcstring_character_t *header_string_tail,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string_srce       = _LIBCSTRING_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libcstring_character_t *header_string_sub        = _LIBCSTRING_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libcstring_character_t *case_number              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *description              = _LIBCSTRING_STRING( "" );
	libcstring_character_t *examiner_name            = _LIBCSTRING_STRING( "" );
	libcstring_character_t *evidence_number          = _LIBCSTRING_STRING( "" );
	libcstring_character_t *notes                    = _LIBCSTRING_STRING( "" );
	libcstring_character_t *system_date              = NULL;
	libcstring_character_t *acquiry_date             = NULL;
	libcstring_character_t *acquiry_operating_system = _LIBCSTRING_STRING( "" );
	libcstring_character_t *acquiry_software_version = _LIBCSTRING_STRING( "" );
	libcstring_character_t *password_hash            = _LIBCSTRING_STRING( "" );
	static char *function                            = "libewf_header_values_generate_header_string_type7";
	size_t system_date_size                          = 0;
	size_t acquiry_date_size                         = 0;
	int print_count                                  = 0;

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
	if( header_values->number_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: missing default header values.",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing values.",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing value lengths.",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string head.",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string tail.",
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
	*header_string_size = libcstring_string_length(
	                       header_string_head );

	*header_string_size += libcstring_string_length(
	                        header_string_srce );

	*header_string_size += libcstring_string_length(
	                        header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size,
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

			acquiry_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size,
		     error ) != 1 )
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

			system_date = _LIBCSTRING_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libcstring_string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	*header_string_size += libcstring_string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s",
		       (char *) header_string_head,
	               (char *) description,
	               (char *) case_number,
	               (char *) evidence_number,
	               (char *) examiner_name,
		       (char *) notes,
	               (char *) acquiry_software_version,
	               (char *) acquiry_operating_system,
	               (char *) acquiry_date,
		       (char *) system_date,
	               (char *) password_hash,
	               (char *) header_string_tail,
		       (char *) header_string_srce,
	               (char *) header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libcstring_character_t *) _LIBCSTRING_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
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
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\nmain\nc\tn\ta\te\tt\tm\tu\tp\tr\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header_ewf";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type1(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
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
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tm\tu\tp\tr\r\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\r\n\r\n" );
	static char *function                      = "libewf_header_values_generate_header_encase1";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type1(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
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
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header_string_ftk";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type2(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
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
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\r\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\r\n\r\n" );
	static char *function                      = "libewf_header_values_generate_header_encase2";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type2(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EnCase4 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_encase4(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\r\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\r\n\r\n" );
	static char *function                      = "libewf_header_values_generate_header_encase4";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type3(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate a linen5 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_linen5(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header_linen5";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type7(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate a linen6 header
 * Sets header and header length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_linen6(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header_linen6";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type6(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EnCase4 header2
 * Sets header2 and header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase4(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header2_encase4";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type4(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EnCase5 header2
 * Sets header2 and header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase5(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\tdc\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header2_encase5";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type5(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EnCase6 header2
 * Sets header2 and header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase6(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header2_encase6";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type6(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Convert date time values string within an xheader value
 * Sets date time values string and length
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_xheader_value(
     libcstring_character_t *header_value,
     size_t header_value_length,
     libcstring_character_t **date_time_values_string,
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
	     (libcstring_character_t) ' ',
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
	time_elements.tm_year = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 0 ] - (libcstring_character_t) '0' ) * 1000 )
			      + ( ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 1 ] - (libcstring_character_t) '0' ) * 100 )
			      + ( ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 2 ] - (libcstring_character_t) '0' ) * 10 )
			      + ( ( date_time_elements->values[ empty_date_element_correction + 4 ] )[ 3 ] - (libcstring_character_t) '0' )
			      - 1900 );

	/* Set the month
	 */
	if( libcstring_string_compare(
	     date_time_elements->values[ 1 ],
	     _LIBCSTRING_STRING( "Jan" ),
	     3 ) == 0 )
	{
		time_elements.tm_mon = 0;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Feb" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 1;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Mar" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 2;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Apr" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 3;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "May" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 4;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Jun" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 5;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Jul" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 6;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Aug" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 7;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Sep" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 8;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Oct" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 9;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Nov" ),
	  3 ) == 0 )
	{
		time_elements.tm_mon = 10;
	}
	else if( libcstring_string_compare(
		  date_time_elements->values[ 1 ],
		  _LIBCSTRING_STRING( "Dec" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 11;
	}
	/* Set the day of the month
	 */
	if( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 1 ] == 0 )
	{
		time_elements.tm_mday = (int) ( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 0 ] - (libcstring_character_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 0 ] - (libcstring_character_t) '0' ) * 10 )
				      + ( ( date_time_elements->values[ empty_date_element_correction + 2 ] )[ 1 ] - (libcstring_character_t) '0' ) );
	}
	/* Set the hour
	 */
	time_elements.tm_hour = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 0 ] - (libcstring_character_t) '0' ) * 10 )
			      + ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 1 ] - (libcstring_character_t) '0' ) );

	/* Set the minutes
	 */
	time_elements.tm_min  = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 3 ] - (libcstring_character_t) '0' ) * 10 )
			      + ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 4 ] - (libcstring_character_t) '0' ) );

	/* Set the seconds
	 */
	time_elements.tm_sec = (int) ( ( ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 6 ] - (libcstring_character_t) '0' ) * 10 )
			     + ( ( date_time_elements->values[ empty_date_element_correction + 3 ] )[ 7 ] - (libcstring_character_t) '0' ) );

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
	*date_time_values_string = (libcstring_character_t *) memory_allocate(
	                                                       sizeof( libcstring_character_t ) * *date_time_values_string_size );

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
		( *date_time_values_string )[ 19 ] = (libcstring_character_t) ' ';

		if( libcstring_string_copy(
		     &( ( *date_time_values_string )[ 20 ] ),
		     date_time_elements->values[ empty_date_element_correction + 5 ],
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
		( *date_time_values_string )[ 20 + timezone_string_length ] = (libcstring_character_t) ' ';

		if( libcstring_string_copy(
		     &( ( *date_time_values_string )[ 21 + timezone_string_length ] ),
		     date_time_elements->values[ empty_date_element_correction + 6 ],
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
     libcstring_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	libcstring_character_t *day_of_week = NULL;
	libcstring_character_t *month       = NULL;
	static char *function               = "libewf_generate_date_xheader_value";
	int print_count                     = 0;

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
	switch( time_elements.tm_wday )
	{
		case 0:
			day_of_week = _LIBCSTRING_STRING( "Sun" );
			break;
		case 1:
			day_of_week = _LIBCSTRING_STRING( "Mon" );
			break;
		case 2:
			day_of_week = _LIBCSTRING_STRING( "Tue" );
			break;
		case 3:
			day_of_week = _LIBCSTRING_STRING( "Wed" );
			break;
		case 4:
			day_of_week = _LIBCSTRING_STRING( "Thu" );
			break;
		case 5:
			day_of_week = _LIBCSTRING_STRING( "Fri" );
			break;
		case 6:
			day_of_week = _LIBCSTRING_STRING( "Sat" );
			break;

		default:
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported day of the week value.",
			 function );

			return( -1 );
	}
	switch( time_elements.tm_mon )
	{
		case 0:
			month = _LIBCSTRING_STRING( "Jan" );
			break;
		case 1:
			month = _LIBCSTRING_STRING( "Feb" );
			break;
		case 2:
			month = _LIBCSTRING_STRING( "Mar" );
			break;
		case 3:
			month = _LIBCSTRING_STRING( "Apr" );
			break;
		case 4:
			month = _LIBCSTRING_STRING( "May" );
			break;
		case 5:
			month = _LIBCSTRING_STRING( "Jun" );
			break;
		case 6:
			month = _LIBCSTRING_STRING( "Jul" );
			break;
		case 7:
			month = _LIBCSTRING_STRING( "Aug" );
			break;
		case 8:
			month = _LIBCSTRING_STRING( "Sep" );
			break;
		case 9:
			month = _LIBCSTRING_STRING( "Oct" );
			break;
		case 10:
			month = _LIBCSTRING_STRING( "Nov" );
			break;
		case 11:
			month = _LIBCSTRING_STRING( "Dec" );
			break;

		default:
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported month value.",
			 function );

			return( -1 );
	}
	*date_time_values_string_size = 64;

	*date_time_values_string = (libcstring_character_t *) memory_allocate(
	                                                       sizeof( libcstring_character_t ) * *date_time_values_string_size );

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
#if defined( _BSD_SOURCE )
	time_elements.tm_gmtoff /= 60;

	print_count = libcstring_string_snprintf(
		       *date_time_values_string,
		       *date_time_values_string_size,
		       "%s %s %2d %02d:%02d:%02d %04d %+03ld:%02ld (%s)",
		       (char *) day_of_week,
		       (char *) month,
		       time_elements.tm_mday,
		       time_elements.tm_hour,
		       time_elements.tm_min,
		       time_elements.tm_sec,
		       time_elements.tm_year + 1900,
	               time_elements.tm_gmtoff / 60,
	               time_elements.tm_gmtoff % 60,
		       time_elements.tm_zone );

#elif defined( __BORLANDC__ )
	print_count = libcstring_string_snprintf(
		       *date_time_values_string,
		       *date_time_values_string_size,
		       "%s %s %2d %02d:%02d:%02d %04d %s",
		       (char *) day_of_week,
		       (char *) month,
		       time_elements.tm_mday,
		       time_elements.tm_hour,
		       time_elements.tm_min,
		       time_elements.tm_sec,
		       time_elements.tm_year + 1900,
		       _tzname[ time_elements.tm_isdst ] );

#else
	print_count = libcstring_string_snprintf(
		       *date_time_values_string,
		       *date_time_values_string_size,
		       "%s %s %2d %02d:%02d:%02d %04d %s",
		       (char *) day_of_week,
		       (char *) month,
		       time_elements.tm_mday,
		       time_elements.tm_hour,
		       time_elements.tm_min,
		       time_elements.tm_sec,
		       time_elements.tm_year + 1900,
		       tzname[ time_elements.tm_isdst ] );
#endif

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *date_time_values_string_size ) )
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
	return( 1 );
}

/* Parse a XML header string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header_string_xml(
     libfvalue_table_t *header_values,
     libcstring_character_t *header_string_xml,
     size_t header_string_xml_size,
     liberror_error_t **error )
{
	libcstring_character_t *open_tag_start          = NULL;
	libcstring_character_t *open_tag_end            = NULL;
	libcstring_character_t *close_tag_start         = NULL;
	libcstring_character_t *close_tag_end           = NULL;
	libcstring_character_t *date_time_values_string = NULL;
	libewf_split_values_t *lines                    = NULL;
	libfvalue_value_t *header_value                 = NULL;
	uint8_t *identifier                             = NULL;
	uint8_t *value_string_length                    = NULL;
	static char *function                           = "libewf_header_values_parse_header_string_xml";
	size_t date_time_values_string_length           = 0;
	size_t date_time_values_string_size             = 0;
	size_t identifier_length                        = 0;
	size_t string_length                            = 0;
	size_t value_string_length                      = 0;
	int line_iterator                               = 0;

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
	if( header_string_xml == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header string",
		 function );

		return( -1 );
	}
	if( libewf_split_values_parse_string(
	     &lines,
	     header_string_xml,
	     header_string_xml_size,
	     (libcstring_character_t) '\n',
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

		open_tag_start = libcstring_string_search_character(
		                  lines->values[ line_iterator ],
		                  (libcstring_character_t) '<',
		                  string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = libcstring_string_search_character(
		                lines->values[ line_iterator ],
		                (libcstring_character_t) '>',
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
		close_tag_start = libcstring_string_search_character_reverse(
		                   &open_tag_end[ 1 ],
		                   (libcstring_character_t) '<',
		                   string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = libcstring_string_search_character_reverse(
		                 &open_tag_end[ 1 ],
		                 (libcstring_character_t) '>',
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
		   && ( libcstring_narrow_compare(
		         (char *) identifier,
		         "acquiry_date",
		         12 ) == 0 ) )
		 || ( ( identifier_length == 11 )
		   && ( libcstring_narrow_compare(
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
				date_time_values_string_length = libcstring_string_length(
				                                  date_time_values_string );
			}
		}
		if( libfvalue_value_initialize(
		     header_value,
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

/* Parse an EWF xheader for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_xheader(
     libfvalue_table_t *header_values,
     uint8_t *xheader,
     size_t xheader_size,
     liberror_error_t **error )
{
	libcstring_character_t *xml_header_string = NULL;
	static char *function                     = "libewf_header_values_parse_xheader";
	size_t xml_header_string_size             = 0;
	int result                                = 0;

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
	if( libuna_utf8_string_size_from_utf8_stream(
	     xheader,
	     xheader_size,
	     &xml_header_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine XML header string size.",
		 function );

		return( -1 );
	}
	xml_header_string = (libcstring_character_t *) memory_allocate(
	                                                sizeof( libcstring_character_t ) * xml_header_string_size );

	if( xml_header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create XML header string.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
             xml_header_string,
             xml_header_string_size,
             xheader,
             xheader_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set XML header string.",
		 function );

		memory_free(
		 xml_header_string );

		return( -1 );
	}
	result = libewf_header_values_parse_header_string_xml(
                  header_values,
                  xml_header_string,
                  xml_header_string_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse XML header string.",
		 function );
	}
	memory_free(
	 xml_header_string );

	return( result );
}

/* Converts an XML header string into a xheader
 * Sets the xheader and xheader size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_convert_header_string_xml_to_xheader(
     libcstring_character_t *header_string_xml,
     size_t header_string_xml_size,
     uint8_t **xheader,
     size_t *xheader_size,
     liberror_error_t **error )
{
	static char *function = "libewf_header_values_convert_header_string_xml_to_xheader";

	if( header_string_xml == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML header string.",
		 function );

		return( -1 );
	}
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
		 "%s: invalid xheader size.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_stream_size_from_utf8(
	     header_string_xml,
	     header_string_xml_size,
	     xheader_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine xheader size.",
		 function );

		return( -1 );
	}
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

		return( -1 );
	}
	if( libuna_utf8_stream_copy_from_utf8(
	     *xheader,
	     *xheader_size,
	     header_string_xml,
	     header_string_xml_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set xheader.",
		 function );

		memory_free(
		 xheader );

		*xheader      = NULL;
		*xheader_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a header format in XML
 * Sets header string and the header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_xml(
     libfvalue_table_t *header_values,
     time_t timestamp,
     libcstring_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libcstring_character_t *generated_acquiry_date = NULL;
	libcstring_character_t *xml_head               = _LIBCSTRING_STRING( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
	libcstring_character_t *xml_open_tag_xheader   = _LIBCSTRING_STRING( "<xheader>" );
	libcstring_character_t *xml_close_tag_xheader  = _LIBCSTRING_STRING( "</xheader>" );
	libfvalue_value_t *header_value                = NULL;
	uint8_t *data                                  = NULL;
	uint8_t *identifier                            = NULL;
	static char *function                          = "libewf_header_values_generate_header_string_xml";
	size_t data_size                               = 0;
	size_t generated_acquiry_date_size             = 0;
	size_t identifier_size                         = 0;
	uint8_t byte_order                             = 0;
	int header_value_index                         = 0;
	int number_of_header_values                    = 0;
	int print_count                                = 0;
	int string_offset                              = 0;

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
	/* Add space for the XML data and an end of line
	 */
	*header_string_size = 1 + libcstring_string_length(
	                           xml_head );

	*header_string_size += 1 + libcstring_string_length(
	                            xml_open_tag_xheader );

	*header_string_size += 1 + libcstring_string_length(
	                            xml_close_tag_xheader );

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
		if( libfvalue_value_get_data(
		     header_value,
		     &data,
		     &data_size,
		     &byte_order,
		     error ) != 1 )
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
                if( ( data != NULL )
                 && ( data_size > 0 ) )
		{
/* TODO improve solution regarding direct pass of data size
 */
			/* Add space for a leading tab, <identifier>value</identifier> and an end of line
			 */
			*header_string_size += 7 + ( 2 * ( identifier_size - 1 ) ) + ( data_size - 1 );
		}
		else if( ( generated_acquiry_date == NULL )
		      && ( identifier_size == 13 )
		      && ( libcstring_narrow_compare(
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
				/* Add space for a leading tab, <acquiry_date>, header value, </acquiry_date> and an end of line
				 * Make sure to determine the effective length of the acquiry date time values string
				 */
				*header_string_size += 31 + libcstring_string_length(
				                             generated_acquiry_date );
			}
		}
	}
	/* Add space for an empty line and an end of string
	 */
	*header_string_size += 2;

	*header_string = (libcstring_character_t *) memory_allocate(
	                                             sizeof( libcstring_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		*header_string_size = 0;

		if( generated_acquiry_date != NULL )
		{
			memory_free(
			 generated_acquiry_date );
		}
		return( -1 );
	}
	print_count = libcstring_string_snprintf(
	               *header_string,
	               *header_string_size,
	               "%s\n%s\n",
	               (char *) xml_head,
	               (char *) xml_open_tag_xheader );

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *header_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		if( generated_acquiry_date != NULL )
		{
			memory_free(
			 generated_acquiry_date );
		}
		return( -1 );
	}
	string_offset = print_count;

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
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

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
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

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
		if( libfvalue_value_get_data(
		     header_value,
		     &data,
		     &data_size,
		     &byte_order,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data of header value: %s.",
			 function,
			 (char *) identifier );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			if( generated_acquiry_date != NULL )
			{
				memory_free(
				 generated_acquiry_date );
			}
			return( -1 );
		}
                if( ( data != NULL )
                 && ( data_size > 0 ) )
		{
/* TODO improve solution regarding direct pass of data
 */
			print_count = libcstring_string_snprintf(
			               &( ( *header_string )[ string_offset ] ),
			               ( *header_string_size - string_offset ),
			               "\t<%s>%s</%s>\n",
			               (char *) identifier,
			               (char *) data,
			               (char *) identifier );

			if( ( print_count <= -1 )
			 || ( (size_t) print_count > ( *header_string_size - string_offset ) ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set header string.",
				 function );

				memory_free(
				 *header_string );

				*header_string      = NULL;
				*header_string_size = 0;

				if( generated_acquiry_date != NULL )
				{
					memory_free(
					 generated_acquiry_date );
				}
				return( -1 );
			}
			string_offset += print_count;
		}
	}
	if( generated_acquiry_date != NULL )
	{
		print_count = libcstring_string_snprintf(
		               &( ( *header_string )[ string_offset ] ),
		               ( *header_string_size - string_offset ),
		               "\t<acquiry_date>%s</acquiry_date>\n",
		               (char *) generated_acquiry_date );

		memory_free(
		 generated_acquiry_date );

		if( ( print_count <= -1 )
		 || ( (size_t) print_count > ( *header_string_size - string_offset ) ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header string.",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			return( -1 );
		}
		string_offset += print_count;
	}
	print_count = libcstring_string_snprintf(
	               &( ( *header_string )[ string_offset ] ),
	               ( *header_string_size - string_offset ),
	               "%s\n\n",
	               (char *) xml_close_tag_xheader );

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > ( *header_string_size - string_offset ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set header string.",
		 function );

		memory_free(
		 *header_string );

		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	( *header_string )[ *header_string_size - 1 ] = 0;

	return( 1 );
}

/* Generate an EWFX header
 * Sets header and the header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_ewfx(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header_ewfx";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type3(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EWFX header2
 * Sets header2 and the header2 size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_ewfx(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string      = NULL;
	libcstring_character_t *header_string_head = _LIBCSTRING_STRING( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libcstring_character_t *header_string_tail = _LIBCSTRING_STRING( "\n\n" );
	static char *function                      = "libewf_header_values_generate_header2_ewfx";
	size_t header_string_size                  = 0;
	int result                                 = 0;

	if( libewf_header_values_generate_header_string_type4(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
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
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header2.",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EWFX xheader
 * Sets xheader and the xheader size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_xheader_ewfx(
     libfvalue_table_t *header_values,
     time_t timestamp,
     uint8_t **xheader,
     size_t *xheader_size,
     liberror_error_t **error )
{
	libcstring_character_t *header_string_xml = NULL;
	static char *function                     = "libewf_header_values_generate_xheader_ewfx";
	size_t header_string_xml_size             = 0;
	int result                                = 0;

	if( libewf_header_values_generate_header_string_xml(
	     header_values,
	     timestamp,
	     &header_string_xml,
	     &header_string_xml_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create xheader string.",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_xml_to_xheader(
	          header_string_xml,
	          header_string_xml_size,
	          xheader,
	          xheader_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create xheader.",
		 function );
	}
	memory_free(
	 header_string_xml );

	return( result );
}


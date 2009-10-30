/*
 * Header values functions
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
#include <libnotify.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#include "libewf_date_time.h"
#include "libewf_date_time_values.h"
#include "libewf_definitions.h"
#include "libewf_header_values.h"
#include "libewf_libuna.h"
#include "libewf_string.h"

#include "ewf_definitions.h"

/* Initializes the header values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_initialize(
     libewf_values_table_t **header_values,
     liberror_error_t **error )
{
	static char *function = "libewf_header_values_initialize";

	if( libewf_values_table_initialize(
	     header_values,
	     LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header values.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
	     _LIBEWF_STRING( "case_number" ),
	     11,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set case_number identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION,
	     _LIBEWF_STRING( "description" ),
	     11,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set description identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME,
	     _LIBEWF_STRING( "examiner_name" ),
	     13,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set examiner_name identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER,
	     _LIBEWF_STRING( "evidence_number" ),
	     15,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set evidence_number identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_NOTES,
	     _LIBEWF_STRING( "notes" ),
	     5,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set notes identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE,
	     _LIBEWF_STRING( "acquiry_date" ),
	     12,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set acquiry_date identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE,
	     _LIBEWF_STRING( "system_date" ),
	     11,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set system_date identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM,
	     _LIBEWF_STRING( "acquiry_operating_system" ),
	     24,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set acquiry_operating_system identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION,
	     _LIBEWF_STRING( "acquiry_software_version" ),
	     24,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set acquiry_software_version identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_PASSWORD,
	     _LIBEWF_STRING( "password" ),
	     8,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set password identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE,
	     _LIBEWF_STRING( "compression_type" ),
	     16,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set compression_type identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_MODEL,
	     _LIBEWF_STRING( "model" ),
	     5,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set model identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER,
	     _LIBEWF_STRING( "serial_number" ),
	     13,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set serial_number identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_PROCESS_IDENTIFIER,
	     _LIBEWF_STRING( "process_identifier" ),
	     18,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set process_identifier identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC,
	     _LIBEWF_STRING( "unknown_dc" ),
	     10,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set unknown_dc identifier.",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     *header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EXTENTS,
	     _LIBEWF_STRING( "extents" ),
	     7,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set extents identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Convert a header value into a date time values string
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header_value(
     libewf_character_t *header_value,
     size_t header_value_length,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_character_t **date_time_elements = NULL;
	static char *function                   = "libewf_convert_date_header_value";
	time_t timestamp                        = 0;
	size_t amount_of_date_time_elements     = 0;

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
	if( libewf_string_split(
	     header_value,
	     header_value_length,
	     (libewf_character_t) ' ',
	     &date_time_elements,
	     &amount_of_date_time_elements,
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
	if( amount_of_date_time_elements != 6 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported amount of date time elements in header value.",
		 function );

		libewf_string_split_values_free(
		 date_time_elements,
		 amount_of_date_time_elements,
	         NULL );

		return( -1 );
	}
	/* Set the year
	 */
	time_elements.tm_year = (int) ( ( ( date_time_elements[ 0 ][ 0 ] - (libewf_character_t) '0' ) * 1000 )
	                      + ( ( date_time_elements[ 0 ][ 1 ] - (libewf_character_t) '0' ) * 100 )
	                      + ( ( date_time_elements[ 0 ][ 2 ] - (libewf_character_t) '0' ) * 10 )
	                      + ( date_time_elements[ 0 ][ 3 ] - (libewf_character_t) '0' )
	                      - 1900 );

	/* Set the month
	 */
	if( date_time_elements[ 1 ][ 1 ] == 0 )
	{
		time_elements.tm_mon = (int) ( ( date_time_elements[ 1 ][ 0 ] - (libewf_character_t) '0' )
		                     - 1 );
	}
	else
	{
		time_elements.tm_mon = (int) ( ( ( date_time_elements[ 1 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
		                     + ( date_time_elements[ 1 ][ 1 ] - (libewf_character_t) '0' )
		                     - 1 );
	}
	/* Set the day of the month
	 */
	if( date_time_elements[ 2 ][ 1 ] == 0 )
	{
		time_elements.tm_mday = (int) ( date_time_elements[ 2 ][ 0 ] - (libewf_character_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( date_time_elements[ 2 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
		                      + ( date_time_elements[ 2 ][ 1 ] - (libewf_character_t) '0' ) );
	}
	/* Set the hour
	 */
	if( date_time_elements[ 3 ][ 1 ] == 0 )
	{
		time_elements.tm_hour = (int) ( date_time_elements[ 3 ][ 0 ] - (libewf_character_t) '0' );
	}
	else
	{
		time_elements.tm_hour = (int) ( ( ( date_time_elements[ 3 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
		                      + ( date_time_elements[ 3 ][ 1 ] - (libewf_character_t) '0' ) );
	}
	/* Set the minutes
	 */
	if( date_time_elements[ 4 ][ 1 ] == 0 )
	{
		time_elements.tm_min = (int) ( date_time_elements[ 4 ][ 0 ] - (libewf_character_t) '0' );
	}
	else
	{
		time_elements.tm_min = (int) ( ( ( date_time_elements[ 4 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
		                     + ( date_time_elements[ 4 ][ 1 ] - (libewf_character_t) '0' ) );
	}
	/* Set the seconds
	 */
	if( date_time_elements[ 5 ][ 1 ] == 0 )
	{
		time_elements.tm_sec = (int) ( date_time_elements[ 5 ][ 0 ] - (libewf_character_t) '0' );
	}
	else
	{
		time_elements.tm_sec = (int) ( ( ( date_time_elements[ 5 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
		                     + ( date_time_elements[ 5 ][ 1 ] - (libewf_character_t) '0' ) );
	}
	/* Set to ignore the daylight saving time
	 */
	time_elements.tm_isdst = -1;

	if( libewf_string_split_values_free(
	     date_time_elements,
	     amount_of_date_time_elements,
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

	*date_time_values_string = (libewf_character_t *) memory_allocate(
	                                                   sizeof( libewf_character_t ) * *date_time_values_string_size );

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
     libewf_character_t **date_time_values_string,
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

	*date_time_values_string = (libewf_character_t *) memory_allocate(
	                                                   sizeof( libewf_character_t ) * *date_time_values_string_size );

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
	print_count = libewf_string_snprintf(
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
     libewf_character_t *header_value,
     size_t header_value_length,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	libewf_character_t *end_of_header_value = NULL;
	static char *function                   = "libewf_convert_date_header2_value";
	uint64_t timestamp_value                = 0;
	time_t timestamp                        = 0;

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
	end_of_header_value = &( header_value[ header_value_length ] );

	errno = 0;

	timestamp_value = libewf_string_to_uint64(
	                   header_value,
	                   &end_of_header_value,
	                   0 );

	if( errno != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: timestamp value exceeds maximum.",
		 function );

		return( -1 );
	}
	timestamp = (time_t) timestamp_value;

	*date_time_values_string_size = 20;

	*date_time_values_string = (libewf_character_t *) memory_allocate(
	                                                   sizeof( libewf_character_t ) * *date_time_values_string_size );

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

/* Generate date time values string within a header2 value
 * Sets date time values string and size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header2_value(
     time_t timestamp,
     libewf_character_t **date_time_values_string,
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

	*date_time_values_string = (libewf_character_t *) memory_allocate(
	                                                   sizeof( libewf_character_t ) * *date_time_values_string_size );

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
	print_count = libewf_string_snprintf(
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
     libewf_values_table_t *destination_header_values,
     libewf_values_table_t *source_header_values,
     liberror_error_t **error )
{
	static char *function  = "libewf_header_values_copy";
	int values_table_index = 0;

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
	if( source_header_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source header values.",
		 function );

		return( -1 );
	}
	if( source_header_values->amount_of_values < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid source header values - amount is less than zero.",
		 function );

		return( -1 );
	}
	if( source_header_values->identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source header values - missing identifiers.",
		 function );

		return( -1 );
	}
	if( source_header_values->identifier_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source header values - missing identifier lengths.",
		 function );

		return( -1 );
	}
	if( source_header_values->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source header values - missing values.",
		 function );

		return( -1 );
	}
	if( source_header_values->value_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid source header values - missing value lengths.",
		 function );

		return( -1 );
	}
	for( values_table_index = 0; values_table_index < source_header_values->amount_of_values; values_table_index++ )
	{
		/* Skip the acquiry and system date
		 */
		if( ( values_table_index == LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE )
		 || ( values_table_index == LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ) )
		{
			continue;
		}
		/* Skip values without an identifier
		 */
		if( ( source_header_values->identifier[ values_table_index ] == NULL )
		 || ( source_header_values->identifier_length[ values_table_index ] == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: missing identifier for index: %d.\n",
			 function,
			 values_table_index );
#endif

			continue;
		}
		/* Skip empty values
		 */
		if( ( source_header_values->value[ values_table_index ] == NULL )
		 || ( source_header_values->value_length[ values_table_index ] == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			libnotify_verbose_printf(
			 "%s: missing value for index: %d.\n",
			 function,
			 values_table_index );
#endif

			continue;
		}
		if( libewf_values_table_set_value(
		     destination_header_values,
		     source_header_values->identifier[ values_table_index ],
		     source_header_values->identifier_length[ values_table_index ],
		     source_header_values->value[ values_table_index ],
		     source_header_values->value_length[ values_table_index ],
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value for index: %d.",
			 function, values_table_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Parse a header string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header_string(
     libewf_values_table_t *header_values,
     libewf_character_t *header_string,
     size_t header_string_size,
     liberror_error_t **error )
{
	libewf_character_t **lines                   = NULL;
	libewf_character_t **types                   = NULL;
	libewf_character_t **values                  = NULL;
	libewf_character_t *date_time_values_string  = NULL;
	libewf_character_t *value_string             = NULL;
	static char *function                        = "libewf_header_values_parse_header_string";
	size_t amount_of_lines                       = 0;
	size_t amount_of_types                       = 0;
	size_t amount_of_values                      = 0;
	size_t date_time_values_string_size          = 0;
	size_t iterator                              = 0;
	size_t type_string_length                    = 0;
	size_t types_line_size                       = 0;
	size_t value_string_length                   = 0;
	size_t values_line_size                      = 0;
	int result                                   = 0;

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
	if( libewf_string_split(
	     header_string,
	     header_string_size,
	     (libewf_character_t) '\n',
	     &lines,
	     &amount_of_lines,
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
	if( amount_of_lines > 0 )
	{
		if( ( lines[ 0 ][ 0 ] < (libewf_character_t) '0' )
		 || ( lines[ 0 ][ 0 ] > (libewf_character_t) '9' ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: header values string not supported.",
			 function );

			libewf_string_split_values_free(
			 lines,
			 amount_of_lines,
			 NULL );

			return( -1 );
		}
		types_line_size = ( lines[ 2 ] == NULL ) ? 0 : 1 + libewf_string_length(
		                                                    lines[ 2 ] );

		if( libewf_string_split(
		     lines[ 2 ],
		     types_line_size,
		     (libewf_character_t) '\t',
		     &types,
		     &amount_of_types,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split header string into types.",
			 function );

			libewf_string_split_values_free(
			 lines,
			 amount_of_lines,
			 NULL );

			return( -1 );
		}
		values_line_size = ( lines[ 3 ] == NULL ) ? 0 : 1 + libewf_string_length(
		                                                     lines[ 3 ] );

		if(libewf_string_split(
		    lines[ 3 ],
		    values_line_size,
		    (libewf_character_t) '\t',
		    &values,
		    &amount_of_values,
		    error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split header string into values.",
			 function );

			libewf_string_split_values_free(
			 lines,
			 amount_of_lines,
			 NULL );
			libewf_string_split_values_free(
			 types,
			 amount_of_types,
			 NULL );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( amount_of_types != amount_of_values )
		{
			libnotify_verbose_printf(
			 "%s: mismatch in amount of types and values.\n",
			 function );
		}
#endif
		for( iterator = 0;
		     iterator < amount_of_types;
		     iterator++ )
		{
			type_string_length = libewf_string_length(
					      types[ iterator ] );

			if( ( iterator >= amount_of_values )
			 || ( values[ iterator ] == NULL )
			 || ( values[ iterator ] == (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
			{
				value_string        = NULL;
				value_string_length = 0;
			}
			else
			{
				value_string        = values[ iterator ];
				value_string_length = libewf_string_length(
						       value_string );
			}
			/* Remove trailing carriage return
			 */
			if( ( type_string_length > 0 )
			 && ( types[ iterator ][ type_string_length - 1 ] == (libewf_character_t) '\r' ) )
			{
				type_string_length -= 1;
			}
			if( ( value_string_length > 0 )
			 && ( value_string[ value_string_length - 1 ] == (libewf_character_t) '\r' ) )
			{
				value_string_length -= 1;
			}
			if( type_string_length == 3 )
			{
				if( libewf_string_compare(
				     types[ iterator ],
				     _LIBEWF_STRING( "ext" ),
				     type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "extents" ),
					     7,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set extents.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "pid" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "process_identifier" ),
					     18,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set process identifier.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
			}
			else if( type_string_length == 2 )
			{
				if( libewf_string_compare(
				     types[ iterator ],
				     _LIBEWF_STRING( "av" ),
				     type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "acquiry_software_version" ),
					     24,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set acquiry software version.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "dc" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "unknown_dc" ),
					     10,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set unknown: dc.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "md" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "model" ),
					     5,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set model.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "ov" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "acquiry_operating_system" ),
					     24,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set acquiry operating system.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "sn" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "serial_number" ),
					     13,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set serial_number.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
			}
			else if( type_string_length == 1 )
			{
				if( ( libewf_string_compare(
				       types[ iterator ],
				       _LIBEWF_STRING( "m" ),
				       type_string_length ) == 0 )
				 || ( libewf_string_compare(
				       types[ iterator ],
				       _LIBEWF_STRING( "u" ),
				       type_string_length ) == 0 ) )
				{
					if( ( value_string == NULL )
					 || ( value_string == (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
					{
						/* the string search function is not NULL safe */
					}
					/* If the date time values string contains spaces it's in the old header
					 * format otherwise is in new header2 format
					 */
					else if( libewf_string_search(
					          value_string,
					          (libewf_character_t) ' ',
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
							value_string_length = libewf_string_length(
									       date_time_values_string );

						}
						if( libewf_string_compare(
						     types[ iterator ],
						     _LIBEWF_STRING( "m" ),
						     1 ) == 0 )
						{
							if( libewf_values_table_set_value(
							     header_values,
							     _LIBEWF_STRING( "acquiry_date" ),
							     12,
							     date_time_values_string,
							     value_string_length,
							     error ) != 1 )
							{
								liberror_error_set(
								 error,
								 LIBERROR_ERROR_DOMAIN_RUNTIME,
								 LIBERROR_RUNTIME_ERROR_SET_FAILED,
								 "%s: unable to set acquiry date.",
								 function );

								libewf_string_split_values_free(
								 lines,
								 amount_of_lines,
								 NULL );
								libewf_string_split_values_free(
								 types,
								 amount_of_types,
								 NULL );

								return( -1 );
							}
						}
						else if( libewf_string_compare(
							  types[ iterator ],
							  _LIBEWF_STRING( "u" ),
							  1 ) == 0 )
						{
							if( libewf_values_table_set_value(
							     header_values,
							     _LIBEWF_STRING( "system_date" ),
							     11,
							     date_time_values_string,
							     value_string_length,
							     error ) != 1 )
							{
								liberror_error_set(
								 error,
								 LIBERROR_ERROR_DOMAIN_RUNTIME,
								 LIBERROR_RUNTIME_ERROR_SET_FAILED,
								 "%s: unable to set system date.",
								 function );

								libewf_string_split_values_free(
								 lines,
								 amount_of_lines,
								 NULL );
								libewf_string_split_values_free(
								 types,
								 amount_of_types,
								 NULL );

								return( -1 );
							}
						}
						memory_free(
						 date_time_values_string );

						date_time_values_string = NULL;
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "p" ),
					  type_string_length ) == 0 )
				{
					if( value_string_length == 0 )
					{
						/* Empty hash do nothing
						 */
					}
					else if( ( value_string_length == 1 )
					 && ( value_string[ 0 ] == (libewf_character_t) '0' ) )
					{
						/* Empty hash do nothing
						 */
					}
					else
					{
						if( libewf_values_table_set_value(
						     header_values,
						     _LIBEWF_STRING( "password" ),
						     8,
						     value_string,
						     value_string_length,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set password.",
							 function );

							libewf_string_split_values_free(
							 lines,
							 amount_of_lines,
							 NULL );
							libewf_string_split_values_free(
							 types,
							 amount_of_types,
							 NULL );

							return( -1 );
						}
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "a" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "description" ),
					     11,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set description.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "c" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "case_number" ),
					     11,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set case number.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "n" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "evidence_number" ),
					     15,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set evidence number.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "e" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "examiner_name" ),
					     13,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set examiner name.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "t" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "notes" ),
					     5,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set notes.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
				else if( libewf_string_compare(
					  types[ iterator ],
					  _LIBEWF_STRING( "r" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _LIBEWF_STRING( "compression_type" ),
					     16,
					     value_string,
					     value_string_length,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set compression type.",
						 function );

						libewf_string_split_values_free(
						 lines,
						 amount_of_lines,
						 NULL );
						libewf_string_split_values_free(
						 types,
						 amount_of_types,
						 NULL );

						return( -1 );
					}
				}
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			else
			{
				libnotify_verbose_printf(
				 "%s: unsupported type: %s with value: %s.\n",
				 function,
				 (char *) types[ iterator ],
				 (char *) value_string );
			}
#endif
		}
		if( libewf_string_split_values_free(
		     values,
		     amount_of_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split values.",
			 function );

			libewf_string_split_values_free(
			 types,
			 amount_of_types,
			 NULL );
			libewf_string_split_values_free(
			 values,
			 amount_of_values,
			 NULL );

			return( -1 );
		}
		if( libewf_string_split_values_free(
		     types,
		     amount_of_types,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split types.",
			 function );

			libewf_string_split_values_free(
			 lines,
			 amount_of_lines,
			 NULL );

			return( -1 );
		}
	}
	if( libewf_string_split_values_free(
	     lines,
	     amount_of_lines,
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
     libewf_values_table_t *header_values,
     uint8_t *header,
     size_t header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string = NULL;
	static char *function             = "libewf_header_values_parse_header";
	size_t header_string_size         = 0;
	int result                        = 0;

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
	if( libewf_string_size_from_byte_stream(
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
	header_string = (libewf_character_t *) memory_allocate(
	                                        sizeof( libewf_character_t ) * header_string_size );

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
	if( libewf_string_copy_from_byte_stream(
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
     libewf_values_table_t *header_values,
     uint8_t *header2,
     size_t header2_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string = NULL;
	static char *function             = "libewf_header_values_parse_header2";
	size_t header_string_size         = 0;
	int result                        = 0;

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
	if( libewf_string_size_from_utf16_stream(
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
	header_string = (libewf_character_t *) memory_allocate(
	                                        sizeof( libewf_character_t ) * (size_t) header_string_size );

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
	if( libewf_string_copy_from_utf16_stream(
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
     libewf_character_t *header_string,
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
	if( byte_stream_size_from_libewf_string(
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
	if( byte_stream_copy_from_libewf_string(
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
     libewf_character_t *header_string,
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
	if( utf16_stream_size_from_libewf_string(
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
	if( utf16_stream_copy_from_libewf_string(
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *case_number      = _LIBEWF_STRING( "" );
	libewf_character_t *description      = _LIBEWF_STRING( "" );
	libewf_character_t *examiner_name    = _LIBEWF_STRING( "" );
	libewf_character_t *evidence_number  = _LIBEWF_STRING( "" );
	libewf_character_t *notes            = _LIBEWF_STRING( "" );
	libewf_character_t *system_date      = NULL;
	libewf_character_t *acquiry_date     = NULL;
	libewf_character_t *password_hash    = _LIBEWF_STRING( "" );
	libewf_character_t *compression_type = _LIBEWF_STRING( "" );
	static char *function                = "libewf_header_values_generate_header_string_type1";
	size_t system_date_size              = 0;
	size_t acquiry_date_size             = 0;
	int print_count                      = 0;

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
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
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
	*header_string_size = libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			system_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			 *header_string_size += libewf_string_length(
			                         system_date );
		}
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

		password_hash = _LIBEWF_STRING( "0" );
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
			compression_type = _LIBEWF_STRING( LIBEWF_COMPRESSION_TYPE_NONE );
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = _LIBEWF_STRING( LIBEWF_COMPRESSION_TYPE_FAST );
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = _LIBEWF_STRING( LIBEWF_COMPRESSION_TYPE_BEST );
		}
		*header_string_size += libewf_string_length(
		                        compression_type );
	}
	*header_string_size += libewf_string_length(
	                        header_string_tail );

	/* allow for 8x \t and 1x \0
	 */
	*header_string_size += 9;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *case_number              = _LIBEWF_STRING( "" );
	libewf_character_t *description              = _LIBEWF_STRING( "" );
	libewf_character_t *examiner_name            = _LIBEWF_STRING( "" );
	libewf_character_t *evidence_number          = _LIBEWF_STRING( "" );
	libewf_character_t *notes                    = _LIBEWF_STRING( "" );
	libewf_character_t *system_date              = NULL;
	libewf_character_t *acquiry_date             = NULL;
	libewf_character_t *acquiry_operating_system = _LIBEWF_STRING( "" );
	libewf_character_t *acquiry_software_version = _LIBEWF_STRING( "" );
	libewf_character_t *password_hash            = _LIBEWF_STRING( "" );
	libewf_character_t *compression_type         = _LIBEWF_STRING( "" );
	static char *function                        = "libewf_header_values_generate_header_string_type2";
	size_t system_date_size                      = 0;
	size_t acquiry_date_size                     = 0;
	int print_count                              = 0;

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
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
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
	*header_string_size = libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			system_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

		password_hash = _LIBEWF_STRING( "0" );
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
			compression_type = _LIBEWF_STRING( LIBEWF_COMPRESSION_TYPE_NONE );;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = _LIBEWF_STRING( LIBEWF_COMPRESSION_TYPE_FAST );
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = _LIBEWF_STRING( LIBEWF_COMPRESSION_TYPE_BEST );
		}
		*header_string_size += libewf_string_length(
		                        compression_type );
	}
	*header_string_size += libewf_string_length(
	                        header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*header_string_size += 11;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *case_number              = _LIBEWF_STRING( "" );
	libewf_character_t *description              = _LIBEWF_STRING( "" );
	libewf_character_t *examiner_name            = _LIBEWF_STRING( "" );
	libewf_character_t *evidence_number          = _LIBEWF_STRING( "" );
	libewf_character_t *notes                    = _LIBEWF_STRING( "" );
	libewf_character_t *system_date              = NULL;
	libewf_character_t *acquiry_date             = NULL;
	libewf_character_t *acquiry_operating_system = _LIBEWF_STRING( "" );
	libewf_character_t *acquiry_software_version = _LIBEWF_STRING( "" );
	libewf_character_t *password_hash            = _LIBEWF_STRING( "" );
	static char *function                        = "libewf_header_values_generate_header_string_type3";
	size_t system_date_size                      = 0;
	size_t acquiry_date_size                     = 0;
	int print_count                              = 0;

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
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
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
	*header_string_size = libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			system_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

		password_hash = _LIBEWF_STRING( "0" );
	}
	*header_string_size += libewf_string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *case_number              = _LIBEWF_STRING( "" );
	libewf_character_t *description              = _LIBEWF_STRING( "" );
	libewf_character_t *examiner_name            = _LIBEWF_STRING( "" );
	libewf_character_t *evidence_number          = _LIBEWF_STRING( "" );
	libewf_character_t *notes                    = _LIBEWF_STRING( "" );
	libewf_character_t *system_date              = NULL;
	libewf_character_t *acquiry_date             = NULL;
	libewf_character_t *acquiry_operating_system = _LIBEWF_STRING( "" );
	libewf_character_t *acquiry_software_version = _LIBEWF_STRING( "" );
	libewf_character_t *password_hash            = _LIBEWF_STRING( "" );
	static char *function                        = "libewf_header_values_generate_header_string_type4";
	size_t system_date_size                      = 0;
	size_t acquiry_date_size                     = 0;
	int print_count                              = 0;

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
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
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
	*header_string_size = libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			system_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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
	*header_string_size += libewf_string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string_srce       = _LIBEWF_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libewf_character_t *header_string_sub        = _LIBEWF_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libewf_character_t *case_number              = _LIBEWF_STRING( "" );
	libewf_character_t *description              = _LIBEWF_STRING( "" );
	libewf_character_t *examiner_name            = _LIBEWF_STRING( "" );
	libewf_character_t *evidence_number          = _LIBEWF_STRING( "" );
	libewf_character_t *notes                    = _LIBEWF_STRING( "" );
	libewf_character_t *system_date              = NULL;
	libewf_character_t *acquiry_date             = NULL;
	libewf_character_t *acquiry_operating_system = _LIBEWF_STRING( "" );
	libewf_character_t *acquiry_software_version = _LIBEWF_STRING( "" );
	libewf_character_t *password_hash            = _LIBEWF_STRING( "" );
	libewf_character_t *unknown_dc               = _LIBEWF_STRING( "" );
	static char *function                        = "libewf_header_values_generate_header_string_type5";
	size_t system_date_size                      = 0;
	size_t acquiry_date_size                     = 0;
	int print_count                              = 0;

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
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
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
	*header_string_size = libewf_string_length(
	                       header_string_head );

	*header_string_size += libewf_string_length(
	                        header_string_srce );

	*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			system_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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
	*header_string_size += libewf_string_length(
	                        header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*header_string_size += 11;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string_srce       = _LIBEWF_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libewf_character_t *header_string_sub        = _LIBEWF_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libewf_character_t *case_number              = _LIBEWF_STRING( "" );
	libewf_character_t *description              = _LIBEWF_STRING( "" );
	libewf_character_t *examiner_name            = _LIBEWF_STRING( "" );
	libewf_character_t *evidence_number          = _LIBEWF_STRING( "" );
	libewf_character_t *notes                    = _LIBEWF_STRING( "" );
	libewf_character_t *system_date              = NULL;
	libewf_character_t *acquiry_date             = NULL;
	libewf_character_t *acquiry_operating_system = _LIBEWF_STRING( "" );
	libewf_character_t *acquiry_software_version = _LIBEWF_STRING( "" );
	libewf_character_t *password_hash            = _LIBEWF_STRING( "" );
	libewf_character_t *model                    = _LIBEWF_STRING( "" );
	libewf_character_t *serial_number            = _LIBEWF_STRING( "" );
	libewf_character_t *unknown_dc               = _LIBEWF_STRING( "" );
	static char *function                        = "libewf_header_values_generate_header_string_type6";
	size_t system_date_size                      = 0;
	size_t acquiry_date_size                     = 0;
	int print_count                              = 0;

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
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
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
	*header_string_size = libewf_string_length(
	                       header_string_head );

	*header_string_size += libewf_string_length(
	                        header_string_srce );

	*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			system_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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
	*header_string_size += libewf_string_length(
	                        header_string_tail );

	/* allow for 12x \t and 1x \0
	 */
	*header_string_size += 13;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t *header_string_head,
     libewf_character_t *header_string_tail,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string_srce       = _LIBEWF_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libewf_character_t *header_string_sub        = _LIBEWF_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libewf_character_t *case_number              = _LIBEWF_STRING( "" );
	libewf_character_t *description              = _LIBEWF_STRING( "" );
	libewf_character_t *examiner_name            = _LIBEWF_STRING( "" );
	libewf_character_t *evidence_number          = _LIBEWF_STRING( "" );
	libewf_character_t *notes                    = _LIBEWF_STRING( "" );
	libewf_character_t *system_date              = NULL;
	libewf_character_t *acquiry_date             = NULL;
	libewf_character_t *acquiry_operating_system = _LIBEWF_STRING( "" );
	libewf_character_t *acquiry_software_version = _LIBEWF_STRING( "" );
	libewf_character_t *password_hash            = _LIBEWF_STRING( "" );
	static char *function                        = "libewf_header_values_generate_header_string_type7";
	size_t system_date_size                      = 0;
	size_t acquiry_date_size                     = 0;
	int print_count                              = 0;

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
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
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
	*header_string_size = libewf_string_length(
	                       header_string_head );

	*header_string_size += libewf_string_length(
	                        header_string_srce );

	*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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

			libnotify_print_error_backtrace(
			 *error );
			liberror_error_free(
			 error );

			system_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date time values string
			 */
			*header_string_size += libewf_string_length(
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
	*header_string_size += libewf_string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\nmain\nc\tn\ta\te\tt\tm\tu\tp\tr\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header_ewf";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tm\tu\tp\tr\r\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\r\n\r\n" );
	static char *function                  = "libewf_header_values_generate_header_encase1";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header_string_ftk";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\r\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\r\n\r\n" );
	static char *function                  = "libewf_header_values_generate_header_encase2";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\r\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\r\n\r\n" );
	static char *function                  = "libewf_header_values_generate_header_encase4";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header_linen5";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header_linen6";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header2_encase4";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\tdc\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header2_encase5";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header2_encase6";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_character_t *header_value,
     size_t header_value_length,
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_character_t **date_time_elements = NULL;
	static char *function                   = "libewf_convert_date_xheader_value";
	time_t timestamp                        = 0;
	size_t amount_of_date_time_elements     = 0;
	size_t timezone_name_length             = 0;
	size_t timezone_string_length           = 0;
	int empty_date_element_correction       = 0;

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
	if( libewf_string_split(
	     header_value,
	     header_value_length,
	     (libewf_character_t) ' ',
	     &date_time_elements,
	     &amount_of_date_time_elements,
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
	if( amount_of_date_time_elements < 6 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported amount of date time elements in header value.",
		 function );

		libewf_string_split_values_free(
		 date_time_elements,
		 amount_of_date_time_elements,
		 NULL );

		return( -1 );
	}
	/* The ctime formatted string use an additional space
	 * when the day is less than 10
	 */
	if( date_time_elements[ 2 ][ 0 ] == 0 )
	{
		empty_date_element_correction = 1;
	}
	/* Set the year
	 */
	time_elements.tm_year = (int) ( ( ( date_time_elements[ empty_date_element_correction + 4 ][ 0 ] - (libewf_character_t) '0' ) * 1000 )
			      + ( ( date_time_elements[ empty_date_element_correction + 4 ][ 1 ] - (libewf_character_t) '0' ) * 100 )
			      + ( ( date_time_elements[ empty_date_element_correction + 4 ][ 2 ] - (libewf_character_t) '0' ) * 10 )
			      + ( date_time_elements[ empty_date_element_correction + 4 ][ 3 ] - (libewf_character_t) '0' )
			      - 1900 );

	/* Set the month
	 */
	if( libewf_string_compare(
	     date_time_elements[ 1 ],
	     _LIBEWF_STRING( "Jan" ),
	     3 ) == 0 )
	{
		time_elements.tm_mon = 0;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Feb" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 1;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Mar" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 2;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Apr" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 3;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "May" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 4;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Jun" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 5;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Jul" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 6;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Aug" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 7;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Sep" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 8;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Oct" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 9;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Nov" ),
	  3 ) == 0 )
	{
		time_elements.tm_mon = 10;
	}
	else if( libewf_string_compare(
		  date_time_elements[ 1 ],
		  _LIBEWF_STRING( "Dec" ),
		  3 ) == 0 )
	{
		time_elements.tm_mon = 11;
	}
	/* Set the day of the month
	 */
	if( date_time_elements[ empty_date_element_correction + 2 ][ 1 ] == 0 )
	{
		time_elements.tm_mday = (int) ( date_time_elements[ empty_date_element_correction + 2 ][ 0 ] - (libewf_character_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( date_time_elements[ empty_date_element_correction + 2 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
				      + ( date_time_elements[ empty_date_element_correction + 2 ][ 1 ] - (libewf_character_t) '0' ) );
	}
	/* Set the hour
	 */
	time_elements.tm_hour = (int) ( ( ( date_time_elements[ empty_date_element_correction + 3 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
			      + ( date_time_elements[ empty_date_element_correction + 3 ][ 1 ] - (libewf_character_t) '0' ) );

	/* Set the minutes
	 */
	time_elements.tm_min  = (int) ( ( ( date_time_elements[ empty_date_element_correction + 3 ][ 3 ] - (libewf_character_t) '0' ) * 10 )
			      + ( date_time_elements[ empty_date_element_correction + 3 ][ 4 ] - (libewf_character_t) '0' ) );

	/* Set the seconds
	 */
	time_elements.tm_sec = (int) ( ( ( date_time_elements[ empty_date_element_correction + 3 ][ 6 ] - (libewf_character_t) '0' ) * 10 )
			     + ( date_time_elements[ empty_date_element_correction + 3 ][ 7 ] - (libewf_character_t) '0' ) );

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

		libewf_string_split_values_free(
		 date_time_elements,
		 amount_of_date_time_elements,
		 NULL );

		return( -1 );
	}
	*date_time_values_string_size = 20;

	if( ( amount_of_date_time_elements - empty_date_element_correction ) > 5 )
	{
		timezone_string_length = libewf_string_length(
		                          date_time_elements[ empty_date_element_correction + 5 ] );

		*date_time_values_string_size += 1 + timezone_string_length;
	}
	if( ( amount_of_date_time_elements - empty_date_element_correction ) > 6 )
	{
		timezone_name_length = libewf_string_length(
		                        date_time_elements[ empty_date_element_correction + 6 ] );

		*date_time_values_string_size += 1 + timezone_name_length;
	}
	*date_time_values_string = (libewf_character_t *) memory_allocate(
	                                                   sizeof( libewf_character_t ) * *date_time_values_string_size );

	if( *date_time_values_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create date time values string.",
		 function );

		libewf_string_split_values_free(
		 date_time_elements,
		 amount_of_date_time_elements,
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

		libewf_string_split_values_free(
		 date_time_elements,
		 amount_of_date_time_elements,
		 NULL );

		memory_free(
		 *date_time_values_string );

		*date_time_values_string      = NULL;
		*date_time_values_string_size = 0;

		return( -1 );
	}
	if( ( amount_of_date_time_elements - empty_date_element_correction ) > 5 )
	{
		( *date_time_values_string )[ 19 ] = (libewf_character_t) ' ';

		if( libewf_string_copy(
		     &( ( *date_time_values_string )[ 20 ] ),
		     date_time_elements[ empty_date_element_correction + 5 ],
		     timezone_string_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set timezone string in data time values string.",
			 function );

			libewf_string_split_values_free(
			 date_time_elements,
			 amount_of_date_time_elements,
			 NULL );

			memory_free(
			 *date_time_values_string );

			*date_time_values_string      = NULL;
			*date_time_values_string_size = 0;

			return( -1 );
		}
	}
	if( ( amount_of_date_time_elements - empty_date_element_correction ) > 6 )
	{
		( *date_time_values_string )[ 20 + timezone_string_length ] = (libewf_character_t) ' ';

		if( libewf_string_copy(
		     &( ( *date_time_values_string )[ 21 + timezone_string_length ] ),
		     date_time_elements[ empty_date_element_correction + 6 ],
		     timezone_name_length ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set timezone name in data time values string.",
			 function );

			libewf_string_split_values_free(
			 date_time_elements,
			 amount_of_date_time_elements,
			 NULL );

			memory_free(
			 *date_time_values_string );

			*date_time_values_string      = NULL;
			*date_time_values_string_size = 0;

			return( -1 );
		}
	}
	( *date_time_values_string )[ *date_time_values_string_size - 1 ] = 0;

	if( libewf_string_split_values_free(
	     date_time_elements,
	     amount_of_date_time_elements,
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
     libewf_character_t **date_time_values_string,
     size_t *date_time_values_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_character_t *day_of_week = NULL;
	libewf_character_t *month       = NULL;
	static char *function           = "libewf_generate_date_xheader_value";
	int print_count                 = 0;

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
			day_of_week = _LIBEWF_STRING( "Sun" );
			break;
		case 1:
			day_of_week = _LIBEWF_STRING( "Mon" );
			break;
		case 2:
			day_of_week = _LIBEWF_STRING( "Tue" );
			break;
		case 3:
			day_of_week = _LIBEWF_STRING( "Wed" );
			break;
		case 4:
			day_of_week = _LIBEWF_STRING( "Thu" );
			break;
		case 5:
			day_of_week = _LIBEWF_STRING( "Fri" );
			break;
		case 6:
			day_of_week = _LIBEWF_STRING( "Sat" );
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
			month = _LIBEWF_STRING( "Jan" );
			break;
		case 1:
			month = _LIBEWF_STRING( "Feb" );
			break;
		case 2:
			month = _LIBEWF_STRING( "Mar" );
			break;
		case 3:
			month = _LIBEWF_STRING( "Apr" );
			break;
		case 4:
			month = _LIBEWF_STRING( "May" );
			break;
		case 5:
			month = _LIBEWF_STRING( "Jun" );
			break;
		case 6:
			month = _LIBEWF_STRING( "Jul" );
			break;
		case 7:
			month = _LIBEWF_STRING( "Aug" );
			break;
		case 8:
			month = _LIBEWF_STRING( "Sep" );
			break;
		case 9:
			month = _LIBEWF_STRING( "Oct" );
			break;
		case 10:
			month = _LIBEWF_STRING( "Nov" );
			break;
		case 11:
			month = _LIBEWF_STRING( "Dec" );
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

	*date_time_values_string = (libewf_character_t *) memory_allocate(
	                                                   sizeof( libewf_character_t ) * *date_time_values_string_size );

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

	print_count = libewf_string_snprintf(
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
	print_count = libewf_string_snprintf(
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
	print_count = libewf_string_snprintf(
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
     libewf_values_table_t *header_values,
     libewf_character_t *header_string_xml,
     size_t header_string_xml_size,
     liberror_error_t **error )
{
	libewf_character_t **lines                  = NULL;
	libewf_character_t *open_tag_start          = NULL;
	libewf_character_t *open_tag_end            = NULL;
	libewf_character_t *close_tag_start         = NULL;
	libewf_character_t *close_tag_end           = NULL;
	libewf_character_t *date_time_values_string = NULL;
	static char *function                       = "libewf_header_values_parse_header_string_xml";
	size_t amount_of_lines                      = 0;
	size_t date_time_values_string_length       = 0;
	size_t date_time_values_string_size         = 0;
	size_t identifier_length                    = 0;
	size_t iterator                             = 0;
	size_t string_length                        = 0;
	size_t value_length                         = 0;

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
	if( libewf_string_split(
	     header_string_xml,
	     header_string_xml_size,
	     (libewf_character_t) '\n',
	     &lines,
	     &amount_of_lines,
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
	for( iterator = 0; iterator < amount_of_lines; iterator++ )
	{
		if( ( lines[ iterator ] == NULL )
		 || ( lines[ iterator ] == (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			continue;
		}
		string_length = libewf_string_length(
		                 lines[ iterator ] );

		/* Ignore empty lines
		 */
		if( string_length == 0 )
		{
			continue;
		}
		open_tag_start = libewf_string_search(
		                  lines[ iterator ],
		                  (libewf_character_t) '<',
		                  string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = libewf_string_search(
		                lines[ iterator ],
		                (libewf_character_t) '>',
		                string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_end == NULL )
		{
			continue;
		}
		/* Ignore the first part of the XML string
		 */
		string_length -= (size_t) ( open_tag_end - lines[ iterator ] );

		/* Ignore lines only containing a single tag
		 */
		if( string_length <= 1 )
		{
			continue;
		}
		close_tag_start = libewf_string_search_reverse(
		                   &open_tag_end[ 1 ],
		                   (libewf_character_t) '<',
		                   string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = libewf_string_search_reverse(
		                 &open_tag_end[ 1 ],
		                 (libewf_character_t) '>',
		                 string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_end == NULL )
		{
			continue;
		}
		/* Ignore the second part of the XML string
		 */
		identifier_length = (size_t) ( open_tag_end - open_tag_start ) - 1;

		/* Ignore the second part of the XML string
		 */
		value_length = (size_t) ( close_tag_start - open_tag_end ) - 1;

		/* Make sure the string is terminated
		 */
		*open_tag_end = 0;

		if( ( libewf_string_compare(
		       &open_tag_start[ 1 ],
		       _LIBEWF_STRING( "acquiry_date" ),
		       12 ) == 0 )
		 || ( libewf_string_compare(
		       &open_tag_start[ 1 ],
		       _LIBEWF_STRING( "system_date" ),
		       11 ) == 0 ) )
		{
			if( libewf_convert_date_xheader_value(
			     &open_tag_end[ 1 ],
			     value_length,
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

				libewf_string_split_values_free(
				 lines,
				 amount_of_lines,
				 NULL );

				return( -1 );
			}
			else
			{
				/* Make sure to determine the actual length of the date time values string
				 */
				date_time_values_string_length = libewf_string_length(
				                                  date_time_values_string );

				if( libewf_values_table_set_value(
				     header_values,
				     &open_tag_start[ 1 ],
				     identifier_length,
				     date_time_values_string,
				     date_time_values_string_length,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value with identifier: %s.",
					 function,
					 (char *) &open_tag_start[ 1 ] );

					memory_free(
					 date_time_values_string );

					libewf_string_split_values_free(
					 lines,
					 amount_of_lines,
					 NULL );

					return( -1 );
				}
				memory_free(
				 date_time_values_string );
			}
		}
		else
		{
			if( libewf_values_table_set_value(
			     header_values,
			     &open_tag_start[ 1 ],
			     identifier_length,
			     &open_tag_end[ 1 ],
			     value_length,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set value with identifier: %s.",
				 function,
				 (char *) &open_tag_start[ 1 ] );

				libewf_string_split_values_free(
				 lines,
				 amount_of_lines,
				 NULL );

				return( -1 );
			}
		}
	}
	if( libewf_string_split_values_free(
	     lines,
	     amount_of_lines,
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
     libewf_values_table_t *header_values,
     uint8_t *xheader,
     size_t xheader_size,
     liberror_error_t **error )
{
	libewf_character_t *xml_header_string = NULL;
	static char *function                 = "libewf_header_values_parse_xheader";
	size_t xml_header_string_size         = 0;
	int result                            = 0;

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
	if( libewf_string_size_from_utf8_stream(
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
	xml_header_string = (libewf_character_t *) memory_allocate(
	                                            sizeof( libewf_character_t ) * xml_header_string_size );

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
	if( libewf_string_copy_from_utf8_stream(
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
     libewf_character_t *header_string_xml,
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
	if( utf8_stream_size_from_libewf_string(
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
	if( utf8_stream_copy_from_libewf_string(
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     libewf_character_t **header_string,
     size_t *header_string_size,
     liberror_error_t **error )
{
	libewf_character_t *xml_head              = _LIBEWF_STRING( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
	libewf_character_t *xml_open_tag_xheader  = _LIBEWF_STRING( "<xheader>" );
	libewf_character_t *xml_close_tag_xheader = _LIBEWF_STRING( "</xheader>" );
	libewf_character_t *acquiry_date          = NULL;
	static char *function                     = "libewf_header_values_generate_header_string_xml";
	size_t acquiry_date_size                  = 0;
	int print_count                           = 0;
	int string_offset                         = 0;
	int values_table_iterator                 = 0;

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
	if( header_values->amount_of_values < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid header values - amount of values less than zero.",
		 function );

		return( -1 );
	}
	if( header_values->identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing identifiers.",
		 function );

		return( -1 );
	}
	if( header_values->identifier_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid header values - missing identifier lengths.",
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
	/* Add space for the XML data and an end of line
	 */
	*header_string_size = 1 + libewf_string_length(
	                           xml_head );

	*header_string_size += 1 + libewf_string_length(
	                            xml_open_tag_xheader );

	*header_string_size += 1 + libewf_string_length(
	                            xml_close_tag_xheader );

	for( values_table_iterator = 0; values_table_iterator < header_values->amount_of_values; values_table_iterator++ )
	{
		if( ( header_values->identifier[ values_table_iterator ] == NULL )
		 && ( header_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid header value - missing identifier.",
			 function );

			continue;
		}
                if( ( header_values->value[ values_table_iterator ] != NULL )
                 && ( header_values->value_length[ values_table_iterator ] > 0 ) )
		{
			/* Add space for a leading tab, <identifier>value</identifier> and an end of line
			 */
			*header_string_size += ( 2 * header_values->identifier_length[ values_table_iterator ] )
			                     + header_values->value_length[ values_table_iterator ] + 7;
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 || ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == 0 ) )
	{
		if( libewf_generate_date_xheader_value(
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

			acquiry_date = _LIBEWF_STRING( "" );
		}
		else
		{
			/* Add space for a leading tab, <acquiry_date>, header value, </acquiry_date> and an end of line
			 * Make sure to determine the effective length of the acquiry date time values string
			 */
			*header_string_size += 31 + libewf_string_length(
			                             acquiry_date );
		}
	}
	/* allow for an empty line and an end of string
	 */
	*header_string_size += 2;

	*header_string = (libewf_character_t *) memory_allocate(
	                                         sizeof( libewf_character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header string.",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
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

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	string_offset = print_count;

	for( values_table_iterator = 0; values_table_iterator < header_values->amount_of_values; values_table_iterator++ )
	{
		if( ( header_values->identifier[ values_table_iterator ] == NULL )
		 && ( header_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid header value - missing identifier.",
			 function );

			continue;
		}
                if( ( header_values->value[ values_table_iterator ] != NULL )
                 && ( header_values->value_length[ values_table_iterator ] > 0 ) )
		{
			print_count = libewf_string_snprintf(
			               &( ( *header_string )[ string_offset ] ),
			               ( *header_string_size - string_offset ),
			               "\t<%s>%s</%s>\n",
			               (char *) header_values->identifier[ values_table_iterator ],
			               (char *) header_values->value[ values_table_iterator ],
			               (char *) header_values->identifier[ values_table_iterator ] );

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

				if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
				 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
				{
					memory_free(
					 acquiry_date );
				}
				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
			string_offset += print_count;
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
	{
		print_count = libewf_string_snprintf(
		               &( ( *header_string )[ string_offset ] ),
		               ( *header_string_size - string_offset ),
		               "\t<acquiry_date>%s</acquiry_date>\n",
		               (char *) acquiry_date );

		memory_free(
		 acquiry_date );

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
	print_count = libewf_string_snprintf(
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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header,
     size_t *header_size,
     int codepage,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header_ewfx";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string      = NULL;
	libewf_character_t *header_string_head = _LIBEWF_STRING( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_character_t *header_string_tail = _LIBEWF_STRING( "\n\n" );
	static char *function                  = "libewf_header_values_generate_header2_ewfx";
	size_t header_string_size              = 0;
	int result                             = 0;

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
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **xheader,
     size_t *xheader_size,
     liberror_error_t **error )
{
	libewf_character_t *header_string_xml = NULL;
	static char *function                 = "libewf_header_values_generate_xheader_ewfx";
	size_t header_string_xml_size         = 0;
	int result                            = 0;

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


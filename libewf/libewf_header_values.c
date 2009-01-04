/*
 * Header values functions
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <character_string.h>
#include <date_time.h>
#include <endian.h>
#include <memory.h>
#include <notify.h>

#include <libewf/definitions.h>

#include "libewf_header_values.h"
#include "libewf_string.h"

#include "ewf_definitions.h"

/* Initializes the header values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_initialize(
     libewf_values_table_t *header_values )
{
	static char *function = "libewf_header_values_initialize";

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER,
	     _CHARACTER_T_STRING( "case_number" ),
	     11 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set case_number identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION,
	     _CHARACTER_T_STRING( "description" ),
	     11 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set description identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME,
	     _CHARACTER_T_STRING( "examiner_name" ),
	     13 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set examiner_name identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER,
	     _CHARACTER_T_STRING( "evidence_number" ),
	     15 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set evidence_number identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_NOTES,
	     _CHARACTER_T_STRING( "notes" ),
	     5 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set notes identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE,
	     _CHARACTER_T_STRING( "acquiry_date" ),
	     12 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set acquiry_date identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE,
	     _CHARACTER_T_STRING( "system_date" ),
	     11 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set system_date identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM,
	     _CHARACTER_T_STRING( "acquiry_operating_system" ),
	     24 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set acquiry_operating_system identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION,
	     _CHARACTER_T_STRING( "acquiry_software_version" ),
	     24 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set acquiry_software_version identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_PASSWORD,
	     _CHARACTER_T_STRING( "password" ),
	     8 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set password identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE,
	     _CHARACTER_T_STRING( "compression_type" ),
	     16 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set compression_type identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_MODEL,
	     _CHARACTER_T_STRING( "model" ),
	     5 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set model identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER,
	     _CHARACTER_T_STRING( "serial_number" ),
	     13 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set serial_number identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_PID,
	     _CHARACTER_T_STRING( "unknown_pid" ),
	     11 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set unknown_pid identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC,
	     _CHARACTER_T_STRING( "unknown_dc" ),
	     10 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set unknown_dc identifier.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_set_identifier(
	     header_values,
	     LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_EXT,
	     _CHARACTER_T_STRING( "unknown_ext" ),
	     11 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set unknown_ext identifier.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Convert a timestamp into a date string
 * The string must be at least 32 characters of size this includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_timestamp(
     time_t timestamp, 
     uint8_t date_format,
     character_t *date_string,
     size_t date_string_size )
{
	struct tm *time_elements = NULL;
	character_t *newline     = NULL;
	static char *function    = "libewf_convert_timestamp";
	ssize_t print_count      = 0;

	if( date_string == NULL )
	{
		notify_warning_printf( "%s: invalid date string.\n",
		 function );

		return( -1 );
	}
	if( date_string_size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid date string size.\n",
		 function );

		return( -1 );
	}
	if( date_string_size < 32 )
	{
		notify_warning_printf( "%s: date string too small.\n",
		 function );

		return( -1 );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		notify_warning_printf( "%s: unsupported date format.\n",
		 function );

		return( -1 );
	}
	if( date_format == LIBEWF_DATE_FORMAT_CTIME )
	{
		if( string_ctime(
		     &timestamp,
		     date_string,
		     date_string_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to set ctime string.\n",
			 function );

			return( -1 );
		}
		newline = string_search(
		           date_string,
		           (character_t) '\n',
		           date_string_size );

		if( newline != NULL )
		{
			newline[ 0 ] = 0;
		}
	}
	else
	{
		time_elements = date_time_localtime(
		                 &timestamp );

		if( time_elements == NULL )
		{
			notify_warning_printf( "%s: unable to create time elements.\n",
			 function );

			return( -1 );
		}
		if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
		{
			print_count = string_snprintf(
			               date_string,
			               date_string_size,
			               _CHARACTER_T_STRING( "%02d/%02d/%04d %02d:%02d:%02d" ),
			               ( time_elements->tm_mon + 1 ),
			               time_elements->tm_mday,
			               ( time_elements->tm_year + 1900 ),
			               time_elements->tm_hour,
			               time_elements->tm_min,
			               time_elements->tm_sec );
		}
		else if( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
		{
			print_count = string_snprintf(
			               date_string,
			               date_string_size,
			               _CHARACTER_T_STRING( "%02d/%02d/%04d %02d:%02d:%02d" ),
			               time_elements->tm_mday,
			               ( time_elements->tm_mon + 1 ),
			               ( time_elements->tm_year + 1900 ),
			               time_elements->tm_hour,
			               time_elements->tm_min,
			               time_elements->tm_sec );
		}
		else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
		{
			print_count = string_snprintf(
			               date_string,
			               date_string_size,
			               _CHARACTER_T_STRING( "%04d-%02d-%02dT%02d:%02d:%02d" ),
			               ( time_elements->tm_year + 1900 ),
			               ( time_elements->tm_mon + 1 ),
			               time_elements->tm_mday,
			               time_elements->tm_hour,
			               time_elements->tm_min,
			               time_elements->tm_sec );
		}
		if( print_count <= -1 )
		{
			notify_warning_printf( "%s: unable to fill date string.\n",
			 function );

			memory_free(
			 time_elements );

			return( -1 );
		}
		memory_free(
		 time_elements );

		/* Make sure the string is terminated
		 */
		date_string[ date_string_size - 1 ] = 0;
	}
	return( 1 );
}

/* Convert a header value into a date string
 * Sets date string and date string size
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header_value(
     character_t *header_value,
     size_t header_value_length,
     uint8_t date_format,
     character_t **date_string,
     size_t *date_string_length )
{
	struct tm time_elements;

	character_t **date_elements    = NULL;
	static char *function          = "libewf_convert_date_header_value";
	time_t timestamp               = 0;
	size_t amount_of_date_elements = 0;

	if( header_value == NULL )
	{
		notify_warning_printf( "%s: invalid header value.\n",
		 function );

		return( -1 );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		notify_warning_printf( "%s: unsupported date format.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_split(
	     header_value,
	     header_value_length,
	     (character_t) ' ',
	     &date_elements,
	     &amount_of_date_elements ) != 1 )
	{
		notify_warning_printf( "%s: unable to split header value into date elements.\n",
		 function );

		return( -1 );
	}
	if( amount_of_date_elements != 6 )
	{
		notify_warning_printf( "%s: unsupported amount of date elements in header value.\n",
		 function );

		libewf_string_split_values_free(
		 date_elements,
		 amount_of_date_elements );

		return( -1 );
	}
	/* Set the year
	 */
	time_elements.tm_year = (int) ( ( ( date_elements[ 0 ][ 0 ] - (character_t) '0' ) * 1000 )
	                      + ( ( date_elements[ 0 ][ 1 ] - (character_t) '0' ) * 100 )
	                      + ( ( date_elements[ 0 ][ 2 ] - (character_t) '0' ) * 10 )
	                      + ( date_elements[ 0 ][ 3 ] - (character_t) '0' )
	                      - 1900 );

	/* Set the month
	 */
	if( date_elements[ 1 ][ 1 ] == 0 )
	{
		time_elements.tm_mon = (int) ( ( date_elements[ 1 ][ 0 ] - (character_t) '0' )
		                     - 1 );
	}
	else
	{
		time_elements.tm_mon = (int) ( ( ( date_elements[ 1 ][ 0 ] - (character_t) '0' ) * 10 )
		                     + ( date_elements[ 1 ][ 1 ] - (character_t) '0' )
		                     - 1 );
	}
	/* Set the day of the month
	 */
	if( date_elements[ 2 ][ 1 ] == 0 )
	{
		time_elements.tm_mday = (int) ( date_elements[ 2 ][ 0 ] - (character_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( date_elements[ 2 ][ 0 ] - (character_t) '0' ) * 10 )
		                      + ( date_elements[ 2 ][ 1 ] - (character_t) '0' ) );
	}
	/* Set the hour
	 */
	if( date_elements[ 3 ][ 1 ] == 0 )
	{
		time_elements.tm_hour = (int) ( date_elements[ 3 ][ 0 ] - (character_t) '0' );
	}
	else
	{
		time_elements.tm_hour = (int) ( ( ( date_elements[ 3 ][ 0 ] - (character_t) '0' ) * 10 )
		                      + ( date_elements[ 3 ][ 1 ] - (character_t) '0' ) );
	}
	/* Set the minutes
	 */
	if( date_elements[ 4 ][ 1 ] == 0 )
	{
		time_elements.tm_min = (int) ( date_elements[ 4 ][ 0 ] - (character_t) '0' );
	}
	else
	{
		time_elements.tm_min = (int) ( ( ( date_elements[ 4 ][ 0 ] - (character_t) '0' ) * 10 )
		                     + ( date_elements[ 4 ][ 1 ] - (character_t) '0' ) );
	}
	/* Set the seconds
	 */
	if( date_elements[ 5 ][ 1 ] == 0 )
	{
		time_elements.tm_sec = (int) ( date_elements[ 5 ][ 0 ] - (character_t) '0' );
	}
	else
	{
		time_elements.tm_sec = (int) ( ( ( date_elements[ 5 ][ 0 ] - (character_t) '0' ) * 10 )
		                     + ( date_elements[ 5 ][ 1 ] - (character_t) '0' ) );
	}
	/* Set to ignore the daylight saving time
	 */
	time_elements.tm_isdst = -1;

	/* Create a timestamp
	 */
	timestamp = date_time_mktime(
	             &time_elements );

	if( libewf_string_split_values_free(
	     date_elements,
	     amount_of_date_elements ) != 1 )
	{
		notify_warning_printf( "%s: unable to free split date elements.\n",
		 function );

		return( -1 );
	}
	if( timestamp == (time_t) -1 )
	{
		notify_warning_printf( "%s: unable to create timestamp.\n",
		 function );

		return( -1 );
	}
	*date_string_length = 32;

	*date_string = (character_t *) memory_allocate(
	                                sizeof( character_t ) * *date_string_length );

	if( *date_string == NULL )
	{
		notify_warning_printf( "%s: unable to create date string.\n",
		 function );

		*date_string_length = 0;

		return( -1 );
	}
	if( libewf_convert_timestamp(
	     timestamp,
	     date_format,
	     *date_string,
	     *date_string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to convert timestamp.\n",
		 function );

		memory_free(
		 *date_string );

		*date_string        = NULL;
		*date_string_length = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate date string within a header value
 * Sets date string and date string size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header_value(
     time_t timestamp,
     character_t **date_string,
     size_t *date_string_size )
{
	struct tm *time_elements = NULL;
	static char *function    = "libewf_generate_date_header_value";

	if( date_string == NULL )
	{
		notify_warning_printf( "%s: invalid date string.\n",
		 function );

		return( -1 );
	}
	if( *date_string != NULL )
	{
		notify_warning_printf( "%s: date string already created.\n",
		 function );

		return( -1 );
	}
	if( date_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid date string size.\n",
		 function );

		return( -1 );
	}
	time_elements = date_time_localtime(
	                 &timestamp );

	if( time_elements == NULL )
	{
		notify_warning_printf( "%s: unable to create time elements.\n",
		 function );

		return( -1 );
	}
	*date_string_size = 20;

	*date_string = (character_t *) memory_allocate(
	                                sizeof( character_t ) * *date_string_size );

	if( *date_string == NULL )
	{
		notify_warning_printf( "%s: unable to create date string.\n",
		 function );

		memory_free(
		 time_elements );

		*date_string_size = 0;

		return( -1 );
	}
	if( string_snprintf(
	     *date_string,
	     *date_string_size,
	     _CHARACTER_T_STRING( "%4d %d %d %d %d %d" ),
	     ( time_elements->tm_year + 1900 ),
	     ( time_elements->tm_mon + 1 ),
	     time_elements->tm_mday,
	     time_elements->tm_hour,
	     time_elements->tm_min,
	     time_elements->tm_sec ) <= -1 )
	{
		notify_warning_printf( "%s: unable to fill date string.\n",
		 function );

		memory_free(
		 *date_string );
		memory_free(
		 time_elements );

		*date_string      = NULL;
		*date_string_size = 0;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	( *date_string )[ *date_string_size - 1 ] = 0;

	memory_free(
	 time_elements );

	return( 1 );
}

/* Convert a header2 value into a date string
 * Sets date string and date string length
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_header2_value(
     character_t *header_value,
     size_t header_value_length,
     uint8_t date_format,
     character_t **date_string,
     size_t *date_string_length )
{
	static char *function   = "libewf_convert_date_header2_value";
	int64_t timestamp_value = 0;
	time_t timestamp        = 0;

	if( header_value == NULL )
	{
		notify_warning_printf( "%s: invalid header value.\n",
		 function );

		return( -1 );
	}
	if( string_to_int64(
	     header_value,
	     header_value_length,
	     &timestamp_value ) != 1 )
	{
		notify_warning_printf( "%s: unable to convert header value to int64.\n",
		 function );

		return( -1 );
	}
	if( timestamp_value > (int64_t) LONG_MAX )
	{
		notify_warning_printf( "%s: timestamp value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	timestamp = (time_t) timestamp_value;

	*date_string_length = 32;

	*date_string = (character_t *) memory_allocate(
	                                sizeof( character_t ) * *date_string_length );

	if( *date_string == NULL )
	{
		notify_warning_printf( "%s: unable to create date string.\n",
		 function );

		*date_string_length = 0;

		return( -1 );
	}
	if( libewf_convert_timestamp(
	     timestamp,
	     date_format,
	     *date_string,
	     *date_string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to convert timestamp.\n",
		 function );

		memory_free(
		 *date_string );

		*date_string        = NULL;
		*date_string_length = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate date string within a header2 value
 * Sets date string and date string size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_header2_value(
     time_t timestamp,
     character_t **date_string,
     size_t *date_string_size )
{
	static char *function = "libewf_generate_date_header2_value";

	if( date_string == NULL )
	{
		notify_warning_printf( "%s: invalid date string.\n",
		 function );

		return( -1 );
	}
	if( *date_string != NULL )
	{
		notify_warning_printf( "%s: date string already created.\n",
		 function );

		return( -1 );
	}
	if( date_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid date string size.\n",
		 function );

		return( -1 );
	}
	*date_string_size = 11;

	*date_string = (character_t *) memory_allocate(
	                                sizeof( character_t ) * *date_string_size );

	if( *date_string == NULL )
	{
		notify_warning_printf( "%s: unable to create date string.\n",
		 function );

		*date_string_size = 0;

		return( -1 );
	}
	if( string_snprintf(
	     *date_string,
	     *date_string_size,
	     _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIu32 ),
	     (uint32_t) timestamp ) <= -1 )
	{
		notify_warning_printf( "%s: unable to create date string.\n",
		 function );

		memory_free(
		 *date_string );

		*date_string      = NULL;
		*date_string_size = 0;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	( *date_string )[ *date_string_size - 1 ] = 0;

	return( 1 );
}

/* Copies the header values from the source to the destination
 * Returns 1 if successful -1 on error
 */
int libewf_header_values_copy(
     libewf_values_table_t *destination_header_values,
     libewf_values_table_t *source_header_values )
{
	static char *function  = "libewf_header_values_copy";
	int values_table_index = 0;

	if( destination_header_values == NULL )
	{
		notify_warning_printf( "%s: invalid destination header values.\n",
		 function );

		return( -1 );
	}
	if( source_header_values == NULL )
	{
		notify_warning_printf( "%s: invalid source header values.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->amount_of_values < 0 )
	{
		notify_warning_printf( "%s: invalid source header values - amount is less than zero.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->identifier == NULL )
	{
		notify_warning_printf( "%s: invalid source header values - missing identifiers.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->identifier_length == NULL )
	{
		notify_warning_printf( "%s: invalid source header values - missing identifier lengths.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid source header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid source header values - missing value lengths.\n",
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
			notify_verbose_printf( "%s: missing identifier for index: %d.\n",
			 function, values_table_index );
#endif

			continue;
		}
		/* Skip empty values
		 */
		if( ( source_header_values->value[ values_table_index ] == NULL )
		 || ( source_header_values->value_length[ values_table_index ] == 0 ) )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: missing value for index: %d.\n",
			 function, values_table_index );
#endif

			continue;
		}
		if( libewf_values_table_set_value(
		     destination_header_values,
		     source_header_values->identifier[ values_table_index ],
		     source_header_values->identifier_length[ values_table_index ],
		     source_header_values->value[ values_table_index ],
		     source_header_values->value_length[ values_table_index ] ) != 1 )
		{
			notify_warning_printf( "%s: unable to set value for index: %d.\n",
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
     libewf_values_table_t **header_values,
     character_t *header_string,
     size_t header_string_size,
     uint8_t date_format )
{
	character_t **lines        = NULL;
	character_t **types        = NULL;
	character_t **values       = NULL;
	character_t *date_string   = NULL;
	character_t *value_string  = NULL;
	static char *function      = "libewf_header_values_parse_header_string";
	size_t amount_of_lines     = 0;
	size_t amount_of_types     = 0;
	size_t amount_of_values    = 0;
	size_t date_string_size    = 0;
	size_t iterator            = 0;
	size_t type_string_length  = 0;
	size_t value_string_length = 0;
	int result                 = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_initialize(
	     header_values,
	     LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_initialize(
	     *header_values ) != 1 )
	{
		notify_warning_printf( "%s: unable to initialize the header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_split(
	     header_string,
	     header_string_size,
	     (character_t) '\n',
	     &lines,
	     &amount_of_lines ) != 1 )
	{
		notify_warning_printf( "%s: unable to split header string into lines.\n",
		 function );

		return( -1 );
	}
	if( amount_of_lines > 0 )
	{
		if( libewf_string_split(
		     lines[ 2 ],
		     string_size(
		      lines[ 2 ] ),
		     (character_t) '\t',
		     &types,
		     &amount_of_types ) != 1 )
		{
			notify_warning_printf( "%s: unable to split header string into types.\n",
			 function );

			libewf_string_split_values_free(
			 lines,
			 amount_of_lines );

			return( -1 );
		}
		if(libewf_string_split(
		    lines[ 3 ],
		    string_size(
		     lines[ 3 ] ),
		    (character_t) '\t',
		    &values,
		    &amount_of_values ) != 1 )
		{
			notify_warning_printf( "%s: unable to split header string into values.\n",
			 function );

			libewf_string_split_values_free(
			 lines,
			 amount_of_lines );
			libewf_string_split_values_free(
			 types,
			 amount_of_types );

			return( -1 );
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( amount_of_types != amount_of_values )
		{
			notify_warning_printf( "%s: mismatch in amount of types and values.\n",
			 function );
		}
#endif
		for( iterator = 0; iterator < amount_of_types; iterator++ )
		{
			type_string_length = string_length(
					      types[ iterator ] );

			if( ( iterator >= amount_of_values )
			 || ( values[ iterator ] == NULL )
			 || ( values[ iterator ] == (character_t *) _CHARACTER_T_STRING( "" ) ) )
			{
				value_string        = NULL;
				value_string_length = 0;
			}
			else
			{
				value_string        = values[ iterator ];
				value_string_length = string_length(
						       value_string );
			}
			/* Remove trailing carriage return
			 */
			if( ( type_string_length > 0 )
			 && ( types[ iterator ][ type_string_length - 1 ] == (character_t) '\r' ) )
			{
				type_string_length -= 1;
			}
			if( ( value_string_length > 0 )
			 && ( value_string[ value_string_length - 1 ] == (character_t) '\r' ) )
			{
				value_string_length -= 1;
			}
			if( type_string_length == 3 )
			{
				if( string_compare(
				     types[ iterator ],
				     _CHARACTER_T_STRING( "ext" ),
				     type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "unknown_ext" ),
					     11,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set unknown: ext.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "pid" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "unknown_pid" ),
					     11,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set unknown: pid.\n",
						 function );
#endif
					}
				}
			}
			else if( type_string_length == 2 )
			{
				if( string_compare(
				     types[ iterator ],
				     _CHARACTER_T_STRING( "av" ),
				     type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "acquiry_software_version" ),
					     24,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set acquiry software version.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "dc" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "unknown_dc" ),
					     10,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set unknown: dc.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "md" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "model" ),
					     5,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set model.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "ov" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "acquiry_operating_system" ),
					     24,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set acquiry operating system.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "sn" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "serial_number" ),
					     13,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set serial_number.\n",
						 function );
#endif
					}
				}
			}
			else if( type_string_length == 1 )
			{
				if( ( string_compare(
				       types[ iterator ],
				       _CHARACTER_T_STRING( "m" ),
				       type_string_length ) == 0 )
				 || ( string_compare(
				       types[ iterator ],
				       _CHARACTER_T_STRING( "u" ),
				       type_string_length ) == 0 ) )
				{
					/* If the date string contains spaces it's in the old header
					 * format otherwise is in new header2 format
					 */
					if( string_search(
					     value_string,
					     (character_t) ' ',
					     value_string_length ) != NULL )
					{
						result = libewf_convert_date_header_value(
							  value_string,
							  value_string_length,
							  date_format,
							  &date_string,
							  &date_string_size );
					}
					else if( value_string_length != 0 )
					{
						result = libewf_convert_date_header2_value(
							  value_string,
							  value_string_length,
							  date_format,
							  &date_string,
							  &date_string_size );
					}
					if( ( value_string_length != 0 )
					 && ( result != 1 ) )
					{
						notify_warning_printf( "%s: unable to create date string.\n",
						 function );
					}
					else
					{
						/* The effective size of the date string is needed
						 */
						if( value_string_length != 0 )
						{
							value_string_length = string_length(
									       date_string );

						}
						if( string_compare(
						     types[ iterator ],
						     _CHARACTER_T_STRING( "m" ),
						     1 ) == 0 )
						{
							if( libewf_values_table_set_value(
							     *header_values,
							     _CHARACTER_T_STRING( "acquiry_date" ),
							     12,
							     date_string,
							     value_string_length ) != 1 )
							{
#if defined( HAVE_VERBOSE_OUTPUT )
								notify_verbose_printf( "%s: unable to set acquiry date.\n",
								 function );
#endif
							}
						}
						else if( string_compare(
							  types[ iterator ],
							  _CHARACTER_T_STRING( "u" ),
							  1 ) == 0 )
						{
							if( libewf_values_table_set_value(
							     *header_values,
							     _CHARACTER_T_STRING( "system_date" ),
							     11,
							     date_string,
							     value_string_length ) != 1 )
							{
#if defined( HAVE_VERBOSE_OUTPUT )
								notify_verbose_printf( "%s: unable to set system date.\n",
								 function );
#endif
							}
						}
						memory_free(
						 date_string );

						date_string = NULL;
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "p" ),
					  type_string_length ) == 0 )
				{
					if( value_string_length == 0 )
					{
						/* Empty hash do nothing
						 */
					}
					else if( ( value_string_length == 1 )
					 && ( value_string[ 0 ] == (character_t) '0' ) )
					{
						/* Empty hash do nothing
						 */
					}
					else
					{
						if( libewf_values_table_set_value(
						     *header_values,
						     _CHARACTER_T_STRING( "password" ),
						     8,
						     value_string,
						     value_string_length ) != 1 )
						{
#if defined( HAVE_VERBOSE_OUTPUT )
							notify_verbose_printf( "%s: unable to set password.\n",
							 function );
#endif
						}
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "a" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "description" ),
					     11,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set description.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "c" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "case_number" ),
					     11,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set case number.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "n" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "evidence_number" ),
					     15,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set evidence number.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "e" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "examiner_name" ),
					     13,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set examiner name.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "t" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "notes" ),
					     5,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set notes.\n",
						 function );
#endif
					}
				}
				else if( string_compare(
					  types[ iterator ],
					  _CHARACTER_T_STRING( "r" ),
					  type_string_length ) == 0 )
				{
					if( libewf_values_table_set_value(
					     *header_values,
					     _CHARACTER_T_STRING( "compression_type" ),
					     16,
					     value_string,
					     value_string_length ) != 1 )
					{
#if defined( HAVE_VERBOSE_OUTPUT )
						notify_verbose_printf( "%s: unable to set compression type.\n",
						 function );
#endif
					}
				}
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			else
			{
				notify_warning_printf( "%s: unsupported type: %d with value: %" PRIs ".\n",
				 function, types[ iterator ], value_string );
			}
#endif
		}
		if( libewf_string_split_values_free(
		     values,
		     amount_of_values ) != 1 )
		{
			notify_warning_printf( "%s: unable to free split values.\n",
			 function );

			libewf_string_split_values_free(
			 types,
			 amount_of_types );
			libewf_string_split_values_free(
			 values,
			 amount_of_values );

			return( -1 );
		}
		if( libewf_string_split_values_free(
		     types,
		     amount_of_types ) != 1 )
		{
			notify_warning_printf( "%s: unable to free split types.\n",
			 function );

			libewf_string_split_values_free(
			 lines,
			 amount_of_lines );

			return( -1 );
		}
	}
	if( libewf_string_split_values_free(
	     lines,
	     amount_of_lines ) != 1 )
	{
		notify_warning_printf( "%s: unable to free split lines.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parse an EWF header for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header(
     libewf_values_table_t **header_values,
     uint8_t *header,
     size_t header_size,
     int codepage,
     uint8_t date_format )
{
	character_t *header_string = NULL;
	static char *function      = "libewf_header_values_parse_header";
	ssize_t header_string_size = 0;
	int result                 = 0;

	if( header == NULL )
	{
		notify_warning_printf( "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	header_string_size = string_size_from_byte_stream(
	                      header,
	                      header_size,
	                      codepage );

	if( header_string_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine header string size.\n",
		 function );

		return( -1 );
	}
	header_string = (character_t *) memory_allocate(
	                                 sizeof( character_t ) * (size_t) header_string_size );

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	if( string_copy_from_byte_stream(
	     header_string,
	     (size_t) header_string_size,
	     header,
	     header_size,
	     codepage ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	result = libewf_header_values_parse_header_string(
	          header_values,
	          header_string,
	          (size_t) header_string_size,
	          date_format );

	memory_free(
	 header_string );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to parse header string.\n",
		 function );
	}
	return( result );
}

/* Parse an EWF header2 for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header2(
     libewf_values_table_t **header_values,
     uint8_t *header2,
     size_t header2_size,
     uint8_t date_format )
{
	character_t *header_string = NULL;
	static char *function      = "libewf_header_values_parse_header2";
	ssize_t header_string_size = 0;
	int result                 = 0;

	if( header2 == NULL )
	{
		notify_warning_printf( "%s: invalid header2.\n",
		 function );

		return( -1 );
	}
	header_string_size = string_size_from_utf16_stream(
	                      header2,
	                      header2_size,
	                      LIBUCA_ENDIAN_LITTLE );

	if( header_string_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine header string size.\n",
		 function );

		return( -1 );
	}
	header_string = (character_t *) memory_allocate(
	                                 sizeof( character_t ) * (size_t) header_string_size );

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	if( string_copy_from_utf16_stream(
	     header_string,
	     (size_t) header_string_size,
	     header2,
	     header2_size,
	     LIBUCA_ENDIAN_LITTLE ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
		 function );

		memory_free(
		 header_string );

		return( -1 );
	}
	result = libewf_header_values_parse_header_string(
	          header_values,
	          header_string,
	          (size_t) header_string_size,
	          date_format );

	memory_free(
	 header_string );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to parse header string.\n",
		 function );
	}
	return( result );
}

/* Converts a header string into a header
 * Sets the header and header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_convert_header_string_to_header(
     character_t *header_string,
     size_t header_string_size,
     uint8_t **header,
     size_t *header_size,
     int codepage )
{
	static char *function    = "libewf_header_values_convert_header_string_to_header";
	ssize_t byte_stream_size = 0;

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( header == NULL )
	{
		notify_warning_printf( "%s: invalid header.\n",
		 function );

		return( -1 );
	}
	if( *header != NULL )
	{
		notify_warning_printf( "%s: header already created.\n",
		 function );

		return( -1 );
	}
	if( header_size == NULL )
	{
		notify_warning_printf( "%s: invalid header size.\n",
		 function );

		return( -1 );
	}
	byte_stream_size = byte_stream_size_from_string(
	                    header_string,
	                    header_string_size,
	                    codepage );

	if( byte_stream_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine header size.\n",
		 function );

		return( -1 );
	}
	*header = (uint8_t *) memory_allocate(
	                       sizeof( uint8_t ) * (size_t) byte_stream_size );

	if( *header == NULL )
	{
		notify_warning_printf( "%s: unable to create header.\n",
		 function );

		return( -1 );
	}
	if( byte_stream_copy_from_string(
	     *header,
	     (size_t) byte_stream_size,
	     codepage,
	     header_string,
	     header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header.\n",
		 function );

		memory_free(
		 header );

		*header = NULL;

		return( -1 );
	}
	*header_size = (size_t) byte_stream_size;

	return( 1 );
}

/* Converts a header string into a header2
 * Sets the header and header size
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_convert_header_string_to_header2(
     character_t *header_string,
     size_t header_string_size,
     uint8_t **header2,
     size_t *header2_size )
{
	static char *function     = "libewf_header_values_convert_header_string_to_header2";
	ssize_t utf16_stream_size = 0;

	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( header2 == NULL )
	{
		notify_warning_printf( "%s: invalid header2.\n",
		 function );

		return( -1 );
	}
	if( *header2 != NULL )
	{
		notify_warning_printf( "%s: header2 already created.\n",
		 function );

		return( -1 );
	}
	if( header2_size == NULL )
	{
		notify_warning_printf( "%s: invalid header2 size.\n",
		 function );

		return( -1 );
	}
	utf16_stream_size = utf16_stream_size_from_string(
	                     header_string,
	                     header_string_size );

	if( utf16_stream_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine header2 size.\n",
		 function );

		return( -1 );
	}
	*header2 = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * (size_t) utf16_stream_size );

	if( *header2 == NULL )
	{
		notify_warning_printf( "%s: unable to create header2.\n",
		 function );

		return( -1 );
	}
	if( utf16_stream_copy_from_string(
	     *header2,
	     (size_t) utf16_stream_size,
	     LIBUCA_ENDIAN_LITTLE,
	     header_string,
	     header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set header2.\n",
		 function );

		memory_free(
		 header2 );

		*header2 = NULL;

		return( -1 );
	}
	*header2_size = (size_t) utf16_stream_size;

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
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *case_number                = _CHARACTER_T_STRING( "" );
	character_t *description                = _CHARACTER_T_STRING( "" );
	character_t *examiner_name              = _CHARACTER_T_STRING( "" );
	character_t *evidence_number            = _CHARACTER_T_STRING( "" );
	character_t *notes                      = _CHARACTER_T_STRING( "" );
	character_t *system_date                = NULL;
	character_t *acquiry_date               = NULL;
	character_t *password_hash              = _CHARACTER_T_STRING( "" );
	character_t *compression_type           = _CHARACTER_T_STRING( "" );
	const character_t *header_format_string = NULL;
	static char *function                   = "libewf_header_values_generate_header_string_type1";
	size_t system_date_size                 = 0;
	size_t acquiry_date_size                = 0;
	ssize_t print_count                     = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		notify_warning_printf( "%s: missing default header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		notify_warning_printf( "%s: invalid header string head.\n",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		notify_warning_printf( "%s: invalid header string tail.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid header string size.\n",
		 function );

		return( -1 );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		notify_warning_printf( "%s: compression level not supported.\n",
		 function );

		return( -1 );
	}
	*header_string_size = string_length(
	                       header_string_head );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &system_date,
		     &system_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			 *header_string_size += string_length(
			                         system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*header_string_size += 1;

		password_hash = _CHARACTER_T_STRING( "0" );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];

		compression_type = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];
	}
	else
	{
		if( compression_level == EWF_COMPRESSION_NONE )
		{
			compression_type = (character_t *) LIBEWF_COMPRESSION_TYPE_NONE;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = (character_t *) LIBEWF_COMPRESSION_TYPE_FAST;
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = (character_t *) LIBEWF_COMPRESSION_TYPE_BEST;
		}
		*header_string_size += string_length(
		                        compression_type );
	}
	*header_string_size += string_length(
	                        header_string_tail );

	/* allow for 8x \t and 1x \0
	 */
	*header_string_size += 9;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	header_format_string = _CHARACTER_T_STRING( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" );
#else
	header_format_string = _CHARACTER_T_STRING( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" ),
#endif

	print_count = string_snprintf(
	               *header_string,
	               *header_string_size,
	               header_format_string,
	               header_string_head,
	               case_number,
	               evidence_number,
	               description,
	               examiner_name,
	               notes,
	               acquiry_date,
	               system_date,
	               password_hash,
	               compression_type,
	               header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( print_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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

/* Generate a header format type 2 (EnCase2, EnCase3, FTK Imager 2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type2(
     libewf_values_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *case_number                = _CHARACTER_T_STRING( "" );
	character_t *description                = _CHARACTER_T_STRING( "" );
	character_t *examiner_name              = _CHARACTER_T_STRING( "" );
	character_t *evidence_number            = _CHARACTER_T_STRING( "" );
	character_t *notes                      = _CHARACTER_T_STRING( "" );
	character_t *system_date                = NULL;
	character_t *acquiry_date               = NULL;
	character_t *acquiry_operating_system   = _CHARACTER_T_STRING( "" );
	character_t *acquiry_software_version   = _CHARACTER_T_STRING( "" );
	character_t *password_hash              = _CHARACTER_T_STRING( "" );
	character_t *compression_type           = _CHARACTER_T_STRING( "" );
	const character_t *header_format_string = NULL;
	static char *function                   = "libewf_header_values_generate_header_string_type2";
	size_t system_date_size                 = 0;
	size_t acquiry_date_size                = 0;
	ssize_t print_count                     = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		notify_warning_printf( "%s: missing default header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		notify_warning_printf( "%s: invalid header string head.\n",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		notify_warning_printf( "%s: invalid header string tail.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid header string size.\n",
		 function );

		return( -1 );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		notify_warning_printf( "%s: compression level not supported.\n",
		 function );

		return( -1 );
	}
	*header_string_size = string_length(
	                       header_string_head );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &system_date,
		     &system_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*header_string_size += 1;

		password_hash = _CHARACTER_T_STRING( "0" );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];

		compression_type = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];
	}
	else
	{
		if( compression_level == EWF_COMPRESSION_NONE )
		{
			compression_type = (character_t *) LIBEWF_COMPRESSION_TYPE_NONE;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = (character_t *) LIBEWF_COMPRESSION_TYPE_FAST;
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = (character_t *) LIBEWF_COMPRESSION_TYPE_BEST;
		}
		*header_string_size += string_length(
		                        compression_type );
	}
	*header_string_size += string_length(
	                        header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*header_string_size += 11;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	header_format_string = _CHARACTER_T_STRING( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" );
#else
	header_format_string = _CHARACTER_T_STRING( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" );
#endif

	print_count = string_snprintf(
	               *header_string,
	               *header_string_size,
	               header_format_string,
	               header_string_head,
	               case_number,
	               evidence_number,
	               description,
	               examiner_name,
	               notes,
	               acquiry_software_version,
	               acquiry_operating_system,
	               acquiry_date,
	               system_date,
	               password_hash,
	               compression_type,
	               header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( print_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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

/* Generate a header format type 3 (EnCase4, EnCase5)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type3(
     libewf_values_table_t *header_values,
     time_t timestamp, 
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *case_number                = _CHARACTER_T_STRING( "" );
	character_t *description                = _CHARACTER_T_STRING( "" );
	character_t *examiner_name              = _CHARACTER_T_STRING( "" );
	character_t *evidence_number            = _CHARACTER_T_STRING( "" );
	character_t *notes                      = _CHARACTER_T_STRING( "" );
	character_t *system_date                = NULL;
	character_t *acquiry_date               = NULL;
	character_t *acquiry_operating_system   = _CHARACTER_T_STRING( "" );
	character_t *acquiry_software_version   = _CHARACTER_T_STRING( "" );
	character_t *password_hash              = _CHARACTER_T_STRING( "" );
	const character_t *header_format_string = NULL;
	static char *function                   = "libewf_header_values_generate_header_string_type3";
	size_t system_date_size                 = 0;
	size_t acquiry_date_size                = 0;
	ssize_t print_count                     = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		notify_warning_printf( "%s: missing default header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		notify_warning_printf( "%s: invalid header string head.\n",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		notify_warning_printf( "%s: invalid header string tail.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid header string size.\n",
		 function );

		return( -1 );
	}
	*header_string_size = string_length(
	                       header_string_head );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header_value(
		     timestamp,
		     &system_date,
		     &system_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*header_string_size += 1;

		password_hash = _CHARACTER_T_STRING( "0" );
	}
	*header_string_size += string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	header_format_string = _CHARACTER_T_STRING( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" );
#else
	header_format_string = _CHARACTER_T_STRING( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" );
#endif

	print_count = string_snprintf(
	               *header_string,
	               *header_string_size,
	               header_format_string,
	               header_string_head,
	               case_number,
	               evidence_number,
	               description,
	               examiner_name,
	               notes,
	               acquiry_software_version,
	               acquiry_operating_system,
	               acquiry_date,
	               system_date,
	               password_hash,
	               header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( print_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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

/* Generate a header format type 4 (EnCase4 header2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type4(
     libewf_values_table_t *header_values,
     time_t timestamp, 
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *case_number                = _CHARACTER_T_STRING( "" );
	character_t *description                = _CHARACTER_T_STRING( "" );
	character_t *examiner_name              = _CHARACTER_T_STRING( "" );
	character_t *evidence_number            = _CHARACTER_T_STRING( "" );
	character_t *notes                      = _CHARACTER_T_STRING( "" );
	character_t *system_date                = NULL;
	character_t *acquiry_date               = NULL;
	character_t *acquiry_operating_system   = _CHARACTER_T_STRING( "" );
	character_t *acquiry_software_version   = _CHARACTER_T_STRING( "" );
	character_t *password_hash              = _CHARACTER_T_STRING( "" );
	const character_t *header_format_string = NULL;
	static char *function                   = "libewf_header_values_generate_header_string_type4";
	size_t system_date_size                 = 0;
	size_t acquiry_date_size                = 0;
	ssize_t print_count                     = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		notify_warning_printf( "%s: missing default header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		notify_warning_printf( "%s: invalid header string head.\n",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		notify_warning_printf( "%s: invalid header string tail.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid header string size.\n",
		 function );

		return( -1 );
	}
	*header_string_size = string_length(
	                       header_string_head );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	*header_string_size += string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 10;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	header_format_string = _CHARACTER_T_STRING( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" );
#else
	header_format_string = _CHARACTER_T_STRING( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" );
#endif

	print_count = string_snprintf(
	               *header_string,
	               *header_string_size,
	               header_format_string,
	               header_string_head,
	               description,
	               case_number,
	               evidence_number,
	               examiner_name,
	               notes,
	               acquiry_software_version,
	               acquiry_operating_system,
	               acquiry_date,
	               system_date,
	               password_hash,
	               header_string_tail );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( print_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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

/* Generate a header format type 5 (EnCase5 header2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type5(
     libewf_values_table_t *header_values,
     time_t timestamp, 
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *header_string_srce         = _CHARACTER_T_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	character_t *header_string_sub          = _CHARACTER_T_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	character_t *case_number                = _CHARACTER_T_STRING( "" );
	character_t *description                = _CHARACTER_T_STRING( "" );
	character_t *examiner_name              = _CHARACTER_T_STRING( "" );
	character_t *evidence_number            = _CHARACTER_T_STRING( "" );
	character_t *notes                      = _CHARACTER_T_STRING( "" );
	character_t *system_date                = NULL;
	character_t *acquiry_date               = NULL;
	character_t *acquiry_operating_system   = _CHARACTER_T_STRING( "" );
	character_t *acquiry_software_version   = _CHARACTER_T_STRING( "" );
	character_t *password_hash              = _CHARACTER_T_STRING( "" );
	character_t *unknown_dc                 = _CHARACTER_T_STRING( "" );
	const character_t *header_format_string = NULL;
	static char *function                   = "libewf_header_values_generate_header_string_type5";
	size_t system_date_size                 = 0;
	size_t acquiry_date_size                = 0;
	ssize_t print_count                     = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		notify_warning_printf( "%s: missing default header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		notify_warning_printf( "%s: invalid header string head.\n",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		notify_warning_printf( "%s: invalid header string tail.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid header string size.\n",
		 function );

		return( -1 );
	}
	*header_string_size = string_length(
	                       header_string_head );

	*header_string_size += string_length(
	                        header_string_srce );

	*header_string_size += string_length(
	                        header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];

		unknown_dc = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];
	}
	*header_string_size += string_length(
	                        header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*header_string_size += 11;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	header_format_string = _CHARACTER_T_STRING( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls%ls%ls" );
#else
	header_format_string = _CHARACTER_T_STRING( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s" );
#endif

	print_count = string_snprintf(
	               *header_string,
	               *header_string_size,
	               header_format_string,
	               header_string_head,
	               description,
	               case_number,
	               evidence_number,
	               examiner_name,
	               notes,
	               acquiry_software_version,
	               acquiry_operating_system,
	               acquiry_date,
	               system_date,
	               password_hash,
	               unknown_dc,
	               header_string_tail,
	               header_string_srce,
	               header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( print_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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

/* Generate a header format type 6 (EnCase6 header2)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type6(
     libewf_values_table_t *header_values,
     time_t timestamp, 
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *header_string_srce         = _CHARACTER_T_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	character_t *header_string_sub          = _CHARACTER_T_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	character_t *case_number                = _CHARACTER_T_STRING( "" );
	character_t *description                = _CHARACTER_T_STRING( "" );
	character_t *examiner_name              = _CHARACTER_T_STRING( "" );
	character_t *evidence_number            = _CHARACTER_T_STRING( "" );
	character_t *notes                      = _CHARACTER_T_STRING( "" );
	character_t *system_date                = NULL;
	character_t *acquiry_date               = NULL;
	character_t *acquiry_operating_system   = _CHARACTER_T_STRING( "" );
	character_t *acquiry_software_version   = _CHARACTER_T_STRING( "" );
	character_t *password_hash              = _CHARACTER_T_STRING( "" );
	character_t *model                      = _CHARACTER_T_STRING( "" );
	character_t *serial_number              = _CHARACTER_T_STRING( "" );
	character_t *unknown_dc                 = _CHARACTER_T_STRING( "" );
	const character_t *header_format_string = NULL;
	static char *function                   = "libewf_header_values_generate_header_string_type6";
	size_t system_date_size                 = 0;
	size_t acquiry_date_size                = 0;
	ssize_t print_count                     = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		notify_warning_printf( "%s: missing default header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		notify_warning_printf( "%s: invalid header string head.\n",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		notify_warning_printf( "%s: invalid header string tail.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid header string size.\n",
		 function );

		return( -1 );
	}
	*header_string_size = string_length(
	                       header_string_head );

	*header_string_size += string_length(
	                        header_string_srce );

	*header_string_size += string_length(
	                        header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_MODEL ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_MODEL ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_MODEL ];

		model = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_MODEL ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ];

		serial_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];

		unknown_dc = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];
	}
	*header_string_size += string_length(
	                        header_string_tail );

	/* allow for 12x \t and 1x \0
	 */
	*header_string_size += 13;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	header_format_string = _CHARACTER_T_STRING( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls%ls%ls" );
#else
	header_format_string = _CHARACTER_T_STRING( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s" );
#endif

	print_count = string_snprintf(
	               *header_string,
	               *header_string_size,
	               header_format_string,
	               header_string_head,
	               description,
	               case_number,
	               evidence_number,
	               examiner_name,
	               notes,
	               model,
	               serial_number,
	               acquiry_software_version,
	               acquiry_operating_system,
	               acquiry_date,
	               system_date,
	               password_hash,
	               unknown_dc,
	               header_string_tail,
	               header_string_srce,
	               header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( print_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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

/* Generate a header format type 7 (EnCase5 linen header)
 * Sets header string and header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_type7(
     libewf_values_table_t *header_values,
     time_t timestamp, 
     character_t *header_string_head,
     character_t *header_string_tail,
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *header_string_srce         = _CHARACTER_T_STRING( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	character_t *header_string_sub          = _CHARACTER_T_STRING( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	character_t *case_number                = _CHARACTER_T_STRING( "" );
	character_t *description                = _CHARACTER_T_STRING( "" );
	character_t *examiner_name              = _CHARACTER_T_STRING( "" );
	character_t *evidence_number            = _CHARACTER_T_STRING( "" );
	character_t *notes                      = _CHARACTER_T_STRING( "" );
	character_t *system_date                = NULL;
	character_t *acquiry_date               = NULL;
	character_t *acquiry_operating_system   = _CHARACTER_T_STRING( "" );
	character_t *acquiry_software_version   = _CHARACTER_T_STRING( "" );
	character_t *password_hash              = _CHARACTER_T_STRING( "" );
	const character_t *header_format_string = NULL;
	static char *function                   = "libewf_header_values_generate_header_string_type7";
	size_t system_date_size                 = 0;
	size_t acquiry_date_size                = 0;
	ssize_t print_count                     = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT )
	{
		notify_warning_printf( "%s: missing default header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string_head == NULL )
	{
		notify_warning_printf( "%s: invalid header string head.\n",
		 function );

		return( -1 );
	}
	if( header_string_tail == NULL )
	{
		notify_warning_printf( "%s: invalid header string tail.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid header string size.\n",
		 function );

		return( -1 );
	}
	*header_string_size = string_length(
	                       header_string_head );

	*header_string_size += string_length(
	                        header_string_srce );

	*header_string_size += string_length(
	                        header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];

		case_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];

		description = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];

		examiner_name = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];

		evidence_number = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];

		notes =  header_values->value[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];

		acquiry_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &acquiry_date,
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        acquiry_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];

		system_date = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		if( libewf_generate_date_header2_value(
		     timestamp,
		     &system_date,
		     &system_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Make sure to determine the actual length of the date string
			 */
			*header_string_size += string_length(
			                        system_date );
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];

		acquiry_operating_system = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];

		acquiry_software_version = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	 && ( header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] > 0 ) )
	{
		/* Do not include end of string character
		 */
		*header_string_size += header_values->value_length[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];

		password_hash = header_values->value[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	*header_string_size += string_length(
	                        header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*header_string_size += 11;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 system_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
#if defined( LIBEWF_WIDE_CHARACTER_TYPE )
	header_format_string = _CHARACTER_T_STRING( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls%ls%ls" );
#else
	header_format_string = _CHARACTER_T_STRING( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s" );
#endif

	print_count = string_snprintf(
	               *header_string,
	               *header_string_size,
	               header_format_string,
		       header_string_head,
	               description,
	               case_number,
	               evidence_number,
	               examiner_name,
		       notes,
	               acquiry_software_version,
	               acquiry_operating_system,
	               acquiry_date,
		       system_date,
	               password_hash,
	               header_string_tail,
		       header_string_srce,
	               header_string_sub );

	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 acquiry_date );
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		memory_free(
		 system_date );
	}
	if( print_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\nmain\nc\tn\ta\te\tt\tm\tu\tp\tr\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header_ewf";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type1(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tm\tu\tp\tr\r\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\r\n\r\n" );
	static char *function           = "libewf_header_values_generate_header_encase1";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type1(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header_string_ftk";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type2(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\r\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\r\n\r\n" );
	static char *function           = "libewf_header_values_generate_header_encase2";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type2(
	     header_values,
	     timestamp,
	     compression_level,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\r\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\r\n\r\n" );
	static char *function           = "libewf_header_values_generate_header_encase4";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type3(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header_linen5";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type7(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header_linen6";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type6(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EnCase4 header2
 * Sets header2 and header2 length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase4(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_length )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header2_encase4";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type4(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_length );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header2.\n",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EnCase5 header2
 * Sets header2 and header2 length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase5(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_length )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\tdc\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header2_encase5";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type5(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_length );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header2.\n",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Generate an EnCase6 header2
 * Sets header2 and header2 length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header2_encase6(
     libewf_values_table_t *header_values,
     time_t timestamp,
     uint8_t **header2,
     size_t *header2_length )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header2_encase6";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type6(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_length );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header2.\n",
		 function );
	}
	memory_free(
	 header_string );

	return( result );
}

/* Convert date string within an xheader value
 * Sets date string and date string length
 * Returns 1 if successful or -1 on error
 */
int libewf_convert_date_xheader_value(
     character_t *header_value,
     size_t header_value_length,
     uint8_t date_format,
     character_t **date_string,
     size_t *date_string_length )
{
	struct tm time_elements;

	character_t **date_elements  = NULL;
	static char *function          = "libewf_convert_date_xheader_value";
	time_t timestamp               = 0;
	size_t amount_of_date_elements = 0;

	if( header_value == NULL )
	{
		notify_warning_printf( "%s: invalid header value.\n",
		 function );

		return( -1 );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH)
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		notify_warning_printf( "%s: unsupported date format.\n",
		 function );

		return( -1 );
	}
	if( date_string == NULL )
	{
		notify_warning_printf( "%s: invalid date string.\n",
		 function );

		return( -1 );
	}
	if( *date_string != NULL )
	{
		notify_warning_printf( "%s: date string already created.\n",
		 function );

		return( -1 );
	}
	if( date_string_length == NULL )
	{
		notify_warning_printf( "%s: invalid date string length.\n",
		 function );

		return( -1 );
	}
	if( date_format == LIBEWF_DATE_FORMAT_CTIME )
	{
		*date_string = (character_t *) memory_allocate(
		                                sizeof( character_t ) * ( header_value_length + 1 ) );

		if( *date_string == NULL )
		{
			notify_warning_printf( "%s: unable to create date string.\n",
			 function );

			return( -1 );
		}
		if( string_copy(
		     *date_string,
		     header_value,
		     header_value_length ) == NULL )
		{
			notify_warning_printf( "%s: unable to set date string.\n",
			 function );

			memory_free(
			 *date_string );

			*date_string = NULL;

			return( -1 );
		}
		( *date_string )[ header_value_length ] = 0;

		*date_string_length = header_value_length;
	}
	else
	{
		if( libewf_string_split(
		     header_value,
		     header_value_length,
		     (character_t) ' ',
		     &date_elements,
		     &amount_of_date_elements ) != 1 )
		{
			notify_warning_printf( "%s: unable to split header value into date elements.\n",
			 function );

			return( -1 );
		}
		if( amount_of_date_elements != 6 )
		{
			notify_warning_printf( "%s: unsupported amount of date elements in header value.\n",
			 function );

			libewf_string_split_values_free(
			 date_elements,
			 amount_of_date_elements );

			return( -1 );
		}
		/* Set the year
		 */
		time_elements.tm_year = (int) ( ( ( date_elements[ 4 ][ 0 ] - (character_t) '0' ) * 1000 )
				      + ( ( date_elements[ 4 ][ 1 ] - (character_t) '0' ) * 100 )
				      + ( ( date_elements[ 4 ][ 2 ] - (character_t) '0' ) * 10 )
				      + ( date_elements[ 4 ][ 3 ] - (character_t) '0' )
				      - 1900 );

		/* Set the month
		 */
		if( string_compare(
		     date_elements[ 1 ],
		     _CHARACTER_T_STRING( "Jan" ),
		     3 ) == 0 )
		{
			time_elements.tm_mon = 0;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Feb" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 1;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Mar" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 2;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Apr" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 3;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "May" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 4;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Jun" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 5;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Jul" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 6;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Aug" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 7;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Sep" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 8;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Oct" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 9;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Nov" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 10;
		}
		else if( string_compare(
		          date_elements[ 1 ],
		          _CHARACTER_T_STRING( "Dec" ),
		          3 ) == 0 )
		{
			time_elements.tm_mon = 11;
		}
		/* Set the day of the month
		 */
		if( date_elements[ 2 ][ 1 ] == 0 )
		{
			time_elements.tm_mday = (int) ( date_elements[ 2 ][ 0 ] - (character_t) '0' );
		}
		else
		{
			time_elements.tm_mday = (int) ( ( ( date_elements[ 2 ][ 0 ] - (character_t) '0' ) * 10 )
					      + ( date_elements[ 2 ][ 1 ] - (character_t) '0' ) );
		}
		/* Set the hour
		 */
		time_elements.tm_hour = (int) ( ( ( date_elements[ 3 ][ 0 ] - (character_t) '0' ) * 10 )
				      + ( date_elements[ 3 ][ 1 ] - (character_t) '0' ) );

		/* Set the minutes
		 */
		time_elements.tm_min  = (int) ( ( ( date_elements[ 3 ][ 3 ] - (character_t) '0' ) * 10 )
				      + ( date_elements[ 3 ][ 4 ] - (character_t) '0' ) );

		/* Set the seconds
		 */
		time_elements.tm_sec = (int) ( ( ( date_elements[ 3 ][ 6 ] - (character_t) '0' ) * 10 )
				     + ( date_elements[ 3 ][ 7 ] - (character_t) '0' ) );

		/* Set to ignore the daylight saving time
		 */
		time_elements.tm_isdst = -1;

		/* Create a timestamp
		 */
		timestamp = date_time_mktime(
		             &time_elements );

		if( libewf_string_split_values_free(
		     date_elements,
		     amount_of_date_elements ) != 1 )
		{
			notify_warning_printf( "%s: unable to free split date elements.\n",
			 function );

			return( -1 );
		}
		if( timestamp == (time_t) -1 )
		{
			notify_warning_printf( "%s: unable to create timestamp.\n",
			 function );

			return( -1 );
		}
		*date_string_length = 32;

		*date_string = (character_t *) memory_allocate(
						sizeof( character_t ) * *date_string_length );

		if( *date_string == NULL )
		{
			notify_warning_printf( "%s: unable to create date string.\n",
			 function );

			*date_string_length = 0;

			return( -1 );
		}
		if( libewf_convert_timestamp(
		     timestamp,
		     date_format,
		     *date_string,
		     *date_string_length ) != 1 )
		{
			notify_warning_printf( "%s: unable to convert timestamp.\n",
			 function );

			memory_free(
			 *date_string );

			*date_string        = NULL;
			*date_string_length = 0;

			return( -1 );
		}
	}
	return( 1 );
}

/* Generate date string within a xheader value
 * Sets date string and date string size
 * Returns 1 if successful or -1 on error
 */
int libewf_generate_date_xheader_value(
     time_t timestamp, 
     character_t **date_string,
     size_t *date_string_size )
{
	character_t *newline  = NULL;
	static char *function = "libewf_generate_date_xheader_value";

	if( date_string == NULL )
	{
		notify_warning_printf( "%s: invalid date string.\n",
		 function );

		return( -1 );
	}
	if( *date_string != NULL )
	{
		notify_warning_printf( "%s: date string already created.\n",
		 function );

		return( -1 );
	}
	if( date_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid date string size.\n",
		 function );

		return( -1 );
	}
	*date_string_size = 32;

	*date_string = (character_t *) memory_allocate(
	                                sizeof( character_t ) * *date_string_size );

	if( *date_string == NULL )
	{
		notify_warning_printf( "%s: unable to create date string.\n",
		 function );

		*date_string_size = 0;

		return( -1 );
	}
	if( string_ctime(
	     &timestamp,
	     *date_string,
	     *date_string_size ) == NULL )
	{
		notify_warning_printf( "%s: unable to set ctime string.\n",
		 function );

		memory_free(
		 *date_string );

		*date_string      = NULL;
		*date_string_size = 0;

		return( -1 );
	}
	newline = string_search(
	           *date_string,
	           (character_t) '\n',
	           *date_string_size );

	if( newline != NULL )
	{
		newline[ 0 ] = (character_t) ' ';
		newline[ 1 ] = (character_t) tzname[ 0 ][ 0 ];
		newline[ 2 ] = (character_t) tzname[ 0 ][ 1 ];
		newline[ 3 ] = (character_t) tzname[ 0 ][ 2 ];
		newline[ 4 ] = 0;
	}
	return( 1 );
}

/* Parse a XML header string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_header_string_xml(
     libewf_values_table_t **header_values,
     character_t *header_string_xml,
     size_t header_string_xml_size,
     uint8_t date_format )
{
	character_t **lines          = NULL;
	character_t *open_tag_start  = NULL;
	character_t *open_tag_end    = NULL;
	character_t *close_tag_start = NULL;
	character_t *close_tag_end   = NULL;
	character_t *date_string     = NULL;
	static char *function        = "libewf_header_values_parse_header_string_xml";
	size_t amount_of_lines       = 0;
	size_t date_string_size      = 0;
	size_t identifier_length     = 0;
	size_t iterator              = 0;
	size_t string_length         = 0;
	size_t value_length          = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_string_xml == NULL )
	{
		notify_warning_printf( "%s: invalid header string\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_initialize(
	     header_values,
	     LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_header_values_initialize(
	     *header_values ) != 1 )
	{
		notify_warning_printf( "%s: unable to initialize the header values.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_split(
	     header_string_xml,
	     header_string_xml_size,
	     (character_t) '\n',
	     &lines,
	     &amount_of_lines ) != 1 )
	{
		notify_warning_printf( "%s: unable to split header string into lines.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < amount_of_lines; iterator++ )
	{
		if( ( lines[ iterator ] == NULL )
		 || ( lines[ iterator ] == (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			continue;
		}
		string_length = string_length(
		                 lines[ iterator ] );

		/* Ignore empty lines
		 */
		if( string_length == 0 )
		{
			continue;
		}
		open_tag_start = string_search(
		                  lines[ iterator ],
		                  (character_t) '<',
		                  string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = string_search(
		                lines[ iterator ],
		                (character_t) '>',
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
		close_tag_start = string_search_reverse(
		                   &open_tag_end[ 1 ],
		                   (character_t) '<',
		                   string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = string_search_reverse(
		                 &open_tag_end[ 1 ],
		                 (character_t) '>',
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

		if( ( string_compare(
		       &open_tag_start[ 1 ],
		       _CHARACTER_T_STRING( "acquiry_date" ),
		       12 ) == 0 )
		 || ( string_compare(
		       &open_tag_start[ 1 ],
		       _CHARACTER_T_STRING( "system_date" ),
		       11 ) == 0 ) )
		{
			if( libewf_convert_date_xheader_value(
			     &open_tag_end[ 1 ],
			     value_length,
			     date_format,
			     &date_string,
			     &date_string_size ) != 1 )
			{
				notify_warning_printf( "%s: unable to create date string.\n",
				 function );
			}
			else
			{
				/* Make sure to determine the actual length of the date string
				 */
				if( libewf_values_table_set_value(
				     *header_values,
				     &open_tag_start[ 1 ],
				     identifier_length,
				     date_string,
				     string_length(
				      date_string ) ) != 1 )
				{
#if defined( HAVE_VERBOSE_OUTPUT )
					notify_verbose_printf( "%s: unable to set value with identifier: %" PRIs ".\n",
					 function, &open_tag_start[ 1 ] );
#endif
				}
				memory_free(
				 date_string );
			}
		}
		else
		{
			if( libewf_values_table_set_value(
			     *header_values,
			     &open_tag_start[ 1 ],
			     identifier_length,
			     &open_tag_end[ 1 ],
			     value_length ) != 1 )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				notify_verbose_printf( "%s: unable to set value with identifier: %" PRIs ".\n",
				 function, &open_tag_start[ 1 ] );
#endif
			}
		}
	}
	if( libewf_string_split_values_free(
	     lines,
	     amount_of_lines ) != 1 )
	{
		notify_warning_printf( "%s: unable to free split lines.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parse an EWF xheader for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_parse_xheader(
     libewf_values_table_t **header_values,
     uint8_t *xheader,
     size_t xheader_size,
     uint8_t date_format )
{
	character_t *xml_header_string = NULL;
	static char *function          = "libewf_header_values_parse_xheader";
	ssize_t xml_header_string_size = 0;
	int result                     = 0;

	if( xheader == NULL )
	{
		notify_warning_printf( "%s: invalid xheader.\n",
		 function );

		return( -1 );
	}
	xml_header_string_size = string_size_from_utf8_stream(
	                          xheader,
	                          xheader_size );

	if( xml_header_string_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine XML header string size.\n",
		 function );

		return( -1 );
	}
	xml_header_string = (character_t *) memory_allocate(
	                                     sizeof( character_t ) * (size_t) xml_header_string_size );

	if( xml_header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create XML header string.\n",
		 function );

		return( -1 );
	}
	if( string_copy_from_utf8_stream(
             xml_header_string,
             (size_t) xml_header_string_size,
             xheader,
             xheader_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set XML header string.\n",
		 function );

		memory_free(
		 xml_header_string );

		return( -1 );
	}
	result = libewf_header_values_parse_header_string_xml(
                  header_values,
                  xml_header_string,
                  (size_t) xml_header_string_size,
                  date_format );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to parse XML header string.\n",
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
     character_t *header_string_xml,
     size_t header_string_xml_size,
     uint8_t **xheader,
     size_t *xheader_size )
{
	static char *function    = "libewf_header_values_convert_header_string_xml_to_xheader";
	ssize_t utf8_stream_size = 0;

	if( header_string_xml == NULL )
	{
		notify_warning_printf( "%s: invalid XML header string.\n",
		 function );

		return( -1 );
	}
	if( xheader == NULL )
	{
		notify_warning_printf( "%s: invalid xheader.\n",
		 function );

		return( -1 );
	}
	if( *xheader != NULL )
	{
		notify_warning_printf( "%s: xheader already created.\n",
		 function );

		return( -1 );
	}
	if( xheader_size == NULL )
	{
		notify_warning_printf( "%s: invalid xheader size.\n",
		 function );

		return( -1 );
	}
	utf8_stream_size = utf8_stream_size_from_string(
	                    header_string_xml,
	                    header_string_xml_size );

	if( utf8_stream_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine xheader size.\n",
		 function );

		return( -1 );
	}
	*xheader = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * (size_t) utf8_stream_size );

	if( *xheader == NULL )
	{
		notify_warning_printf( "%s: unable to create xheader.\n",
		 function );

		return( -1 );
	}
	if( utf8_stream_copy_from_string(
	     *xheader,
	     (size_t) utf8_stream_size,
	     header_string_xml,
	     header_string_xml_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set xheader.\n",
		 function );

		memory_free(
		 xheader );

		*xheader = NULL;

		return( -1 );
	}
	*xheader_size = (size_t) utf8_stream_size;

	return( 1 );
}
/* Generate a header format in XML
 * Sets header string and the header string length
 * Returns 1 if successful or -1 on error
 */
int libewf_header_values_generate_header_string_xml(
     libewf_values_table_t *header_values,
     time_t timestamp, 
     character_t **header_string,
     size_t *header_string_size )
{
	character_t *xml_head              = _CHARACTER_T_STRING( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
	character_t *xml_open_tag_xheader  = _CHARACTER_T_STRING( "<xheader>" );
	character_t *xml_close_tag_xheader = _CHARACTER_T_STRING( "</xheader>" );
	character_t *acquiry_date          = NULL;
	static char *function              = "libewf_header_values_generate_header_string_xml";
	size_t acquiry_date_size           = 0;
	int character_count                = 0;
	int string_offset                  = 0;
	int values_table_iterator          = 0;

	if( header_values == NULL )
	{
		notify_warning_printf( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	if( header_values->amount_of_values < 0 )
	{
		notify_warning_printf( "%s: invalid header values - amount of values less than zero.\n",
		 function );

		return( -1 );
	}
	if( header_values->identifier == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing identifiers.\n",
		 function );

		return( -1 );
	}
	if( header_values->identifier_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing identifier lengths.\n",
		 function );

		return( -1 );
	}
	if( header_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( header_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid header values - missing value lengths.\n",
		 function );

		return( -1 );
	}
	if( header_string == NULL )
	{
		notify_warning_printf( "%s: invalid header string.\n",
		 function );

		return( -1 );
	}
	if( *header_string != NULL )
	{
		notify_warning_printf( "%s: header string already created.\n",
		 function );

		return( -1 );
	}
	if( header_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid string size.\n",
		 function );

		return( -1 );
	}
	/* Add space for the XML data and an end of line
	 */
	*header_string_size = 1 + string_length(
	                           xml_head );

	*header_string_size += 1 + string_length(
	                            xml_open_tag_xheader );

	*header_string_size += 1 + string_length(
	                            xml_close_tag_xheader );

	for( values_table_iterator = 0; values_table_iterator < header_values->amount_of_values; values_table_iterator++ )
	{
		if( ( header_values->identifier[ values_table_iterator ] == NULL )
		 && ( header_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			notify_warning_printf( "%s: invalid header value - missing identifier.\n",
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
		     &acquiry_date_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _CHARACTER_T_STRING( "" );
		}
		else
		{
			/* Add space for a leading tab, <acquiry_date>, header value, </acquiry_date> and an end of line
			 * Make sure to determine the effective length of the acquiry date string
			 */
			*header_string_size += 31 + string_length(
			                             acquiry_date );
		}
	}
	/* allow for an empty line and an end of string
	 */
	*header_string_size += 2;

	*header_string = (character_t *) memory_allocate(
	                                  sizeof( character_t ) * *header_string_size );

	if( *header_string == NULL )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		*header_string_size = 0;

		return( -1 );
	}
	character_count = string_snprintf(
	                   *header_string,
	                   *header_string_size,
	                   _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "\n%" )
	                   _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "\n" ),
	                   xml_head,
	                   xml_open_tag_xheader );

	if( character_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
		 function );

		memory_free(
		 *header_string );

		if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			memory_free(
			 acquiry_date );
		}
		*header_string      = NULL;
		*header_string_size = 0;

		return( -1 );
	}
	string_offset = character_count;

	for( values_table_iterator = 0; values_table_iterator < header_values->amount_of_values; values_table_iterator++ )
	{
		if( ( header_values->identifier[ values_table_iterator ] == NULL )
		 && ( header_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			notify_warning_printf( "%s: invalid header value - missing identifier.\n",
			 function );

			continue;
		}
                if( ( header_values->value[ values_table_iterator ] != NULL )
                 && ( header_values->value_length[ values_table_iterator ] > 0 ) )
		{
			character_count = string_snprintf(
			                   &( ( *header_string )[ string_offset ] ),
			                   ( *header_string_size - string_offset ),
			                   _CHARACTER_T_STRING( "\t<%" ) _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( ">%" )
			                   _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "</%" ) _CHARACTER_T_STRING( PRIs )
			                   _CHARACTER_T_STRING( ">\n" ),
			                   header_values->identifier[ values_table_iterator ],
			                   header_values->value[ values_table_iterator ],
			                   header_values->identifier[ values_table_iterator ] );

			if( character_count <= -1 )
			{
				notify_warning_printf( "%s: unable to set header string.\n",
				 function );

				memory_free(
				 *header_string );

				if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
				 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
				{
					memory_free(
					 acquiry_date );
				}
				*header_string      = NULL;
				*header_string_size = 0;

				return( -1 );
			}
			string_offset += character_count;
		}
	}
	if( ( header_values->value[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (character_t *) _CHARACTER_T_STRING( "" ) ) )
	{
		character_count = string_snprintf(
		                   &( ( *header_string )[ string_offset ] ),
		                   ( *header_string_size - string_offset ),
		                   _CHARACTER_T_STRING( "\t<acquiry_date>%" ) _CHARACTER_T_STRING( PRIs )
		                   _CHARACTER_T_STRING( "</acquiry_date>\n" ),
		                   acquiry_date );

		memory_free(
		 acquiry_date );

		if( character_count <= -1 )
		{
			notify_warning_printf( "%s: unable to set header string.\n",
			 function );

			memory_free(
			 *header_string );

			*header_string      = NULL;
			*header_string_size = 0;

			return( -1 );
		}
		string_offset += character_count;
	}
	character_count = string_snprintf(
	                   &( ( *header_string )[ string_offset ] ),
	                   ( *header_string_size - string_offset ),
	                   _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "\n\n" ),
	                   xml_close_tag_xheader );

	if( character_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set header string.\n",
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
     int codepage )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header_ewfx";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type3(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          header_string_size,
	          header,
	          header_size,
	          codepage );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header.\n",
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
     size_t *header2_size )
{
	character_t *header_string      = NULL;
	character_t *header_string_head = _CHARACTER_T_STRING( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	character_t *header_string_tail = _CHARACTER_T_STRING( "\n\n" );
	static char *function           = "libewf_header_values_generate_header2_ewfx";
	size_t header_string_size       = 0;
	int result                      = 0;

	if( libewf_header_values_generate_header_string_type4(
	     header_values,
	     timestamp,
	     header_string_head,
	     header_string_tail,
	     &header_string,
	     &header_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create header string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_to_header2(
	          header_string,
	          header_string_size,
	          header2,
	          header2_size );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create header2.\n",
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
     size_t *xheader_size )
{
	character_t *header_string_xml = NULL;
	static char *function          = "libewf_header_values_generate_xheader_ewfx";
	size_t header_string_xml_size  = 0;
	int result                     = 0;

	if( libewf_header_values_generate_header_string_xml(
	     header_values,
	     timestamp,
	     &header_string_xml,
	     &header_string_xml_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create xheader string.\n",
		 function );

		return( -1 );
	}
	result = libewf_header_values_convert_header_string_xml_to_xheader(
	          header_string_xml,
	          header_string_xml_size,
	          xheader,
	          xheader_size );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create xheader.\n",
		 function );
	}
	memory_free(
	 header_string_xml );

	return( result );
}


/*
 * Date and time values functions
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
#include "libewf_libfvalue.h"
#include "libewf_split_values.h"

/* Copies date and time values string from a timestamp
 * The string must be at least 20 characters + the length of the timezone string and/or timezone name of size including the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_values_copy_from_timestamp(
     uint8_t *date_time_values_string,
     size_t date_time_values_string_size,
     time_t timestamp,
     liberror_error_t **error )
{
	struct tm time_elements;

	static char *function                = "libewf_date_time_values_copy_from_timestamp";
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
	if( date_time_values_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid date time values string size.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_size < 20 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: date time values string too small.",
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
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
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
	if( ( time_elements.tm_mon < 0 )
	 || ( time_elements.tm_mon > 11 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported month.",
		 function );

		return( -1 );
	}
	if( ( time_elements.tm_mday < 1 )
	 || ( time_elements.tm_mday > 31 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported day of month.",
		 function );

		return( -1 );
	}
	if( ( time_elements.tm_hour < 0 )
	 || ( time_elements.tm_hour > 23 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported hours.",
		 function );

		return( -1 );
	}
	if( ( time_elements.tm_min < 0 )
	 || ( time_elements.tm_min > 59 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported minutes.",
		 function );

		return( -1 );
	}
	/* Note a value of 60 is used for a leap-second
	 */
	if( ( time_elements.tm_sec < 0 )
	 || ( time_elements.tm_sec > 60 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported seconds.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_decimal_copy_from_16bit(
	     date_time_values_string,
	     date_time_values_string_size,
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

		return( -1 );
	}
	date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( ( time_elements.tm_mon + 1 ) < 10 )
	{
		date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) '0';
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     date_time_values_string,
	     date_time_values_string_size,
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

		return( -1 );
	}
	date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( time_elements.tm_mday < 10 )
	{
		date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) '0';
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     date_time_values_string,
	     date_time_values_string_size,
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

		return( -1 );
	}
	date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( time_elements.tm_hour < 10 )
	{
		date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) '0';
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     date_time_values_string,
	     date_time_values_string_size,
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

		return( -1 );
	}
	date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( time_elements.tm_min < 10 )
	{
		date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) '0';
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     date_time_values_string,
	     date_time_values_string_size,
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

		return( -1 );
	}
	date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) ' ';

	if( time_elements.tm_sec < 10 )
	{
		date_time_values_string[ date_time_values_string_index++ ] = (uint8_t) '0';
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     date_time_values_string,
	     date_time_values_string_size,
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

		return( -1 );
	}
	date_time_values_string[ date_time_values_string_index ] = 0;

	return( 1 );
}

/* Copies date and time values string to a timestamp
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_values_copy_to_timestamp(
     const uint8_t *date_time_values_string,
     size_t date_time_values_string_length,
     time_t *timestamp,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_split_values_t *date_time_elements = NULL;
	static char *function                     = "libewf_date_time_values_copy_to_timestamp";

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
	if( date_time_values_string_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid date time values string length.",
		 function );

		return( -1 );
	}
	if( timestamp == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid timestamp.",
		 function );

		return( -1 );
	}
	if( libewf_split_values_parse_string(
	     &date_time_elements,
	     date_time_values_string,
	     date_time_values_string_length,
	     (uint8_t) ' ',
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split date time values string into elements.",
		 function );

		return( -1 );
	}
	if( date_time_elements->number_of_values < 6 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of elements in date time values string.",
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
	time_elements.tm_mon = (int) ( ( ( ( date_time_elements->values[ 1 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
	                     + ( ( date_time_elements->values[ 1 ] )[ 1 ] - (uint8_t) '0' )
	                     - 1 );

	/* Set the day of the month
	 */
	time_elements.tm_mday = (int) ( ( ( ( date_time_elements->values[ 2 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
	                      + ( ( date_time_elements->values[ 2 ] )[ 1 ] - (uint8_t) '0' ) );

	/* Set the hour
	 */
	time_elements.tm_hour = (int) ( ( ( ( date_time_elements->values[ 3 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
	                      + ( ( date_time_elements->values[ 3 ] )[ 1 ] - (uint8_t) '0' ) );

	/* Set the minutes
	 */
	time_elements.tm_min = (int) ( ( ( ( date_time_elements->values[ 4 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
	                     + ( ( date_time_elements->values[ 4 ] )[ 1 ] - (uint8_t) '0' ) );

	/* Set the seconds
	 */
	time_elements.tm_sec = (int) ( ( ( ( date_time_elements->values[ 5 ] )[ 0 ] - (uint8_t) '0' ) * 10 )
	                     + ( ( date_time_elements->values[ 5 ] )[ 1 ] - (uint8_t) '0' ) );

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
	*timestamp = libewf_date_time_mktime(
	              &time_elements );

	if( *timestamp == (time_t) -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-8 encoded string of a day of week from time elements
 * Returns 1 if successful or -1 on error
 */
int libewf_utf8_string_day_of_week_copy_from_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     struct tm *time_elements,
     liberror_error_t **error )
{
	const char *day_of_week = NULL;
	static char *function   = "libewf_utf8_string_day_of_week_copy_from_time_elements";

	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
       	if( *utf8_string_index >= utf8_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string index value out of bounds.",
		 function );

		return( -1 );
       	}
	if( ( *utf8_string_index + 3 ) >= utf8_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_wday < 0 )
	 || ( time_elements->tm_wday > 6 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported day of week.",
		 function );

		return( -1 );
	}
	switch( time_elements->tm_wday )
	{
		case 0:
			day_of_week = "Sun";
			break;
		case 1:
			day_of_week = "Mon";
			break;
		case 2:
			day_of_week = "Tue";
			break;
		case 3:
			day_of_week = "Wed";
			break;
		case 4:
			day_of_week = "Thu";
			break;
		case 5:
			day_of_week = "Fri";
			break;
		case 6:
			day_of_week = "Sat";
			break;
	}
	utf8_string[ *utf8_string_index     ] = (uint8_t) day_of_week[ 0 ];
	utf8_string[ *utf8_string_index + 1 ] = (uint8_t) day_of_week[ 1 ];
	utf8_string[ *utf8_string_index + 2 ] = (uint8_t) day_of_week[ 2 ];

	*utf8_string_index += 3;

	return( 1 );
}

/* Copies an UTF-8 encoded string of a month from time elements
 * Returns 1 if successful or -1 on error
 */
int libewf_utf8_string_month_copy_from_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     struct tm *time_elements,
     liberror_error_t **error )
{
	const char *month     = NULL;
	static char *function = "libewf_utf8_string_month_copy_from_time_elements";

	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
       	if( *utf8_string_index >= utf8_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string index value out of bounds.",
		 function );

		return( -1 );
       	}
	if( ( *utf8_string_index + 3 ) >= utf8_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_mon < 0 )
	 || ( time_elements->tm_mon > 11 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported month.",
		 function );

		return( -1 );
	}
	switch( time_elements->tm_mon )
	{
		case 0:
			month = "Jan";
			break;
		case 1:
			month = "Feb";
			break;
		case 2:
			month = "Mar";
			break;
		case 3:
			month = "Apr";
			break;
		case 4:
			month = "May";
			break;
		case 5:
			month = "Jun";
			break;
		case 6:
			month = "Jul";
			break;
		case 7:
			month = "Aug";
			break;
		case 8:
			month = "Sep";
			break;
		case 9:
			month = "Oct";
			break;
		case 10:
			month = "Nov";
			break;
		case 11:
			month = "Dec";
			break;
	}
	utf8_string[ *utf8_string_index     ] = (uint8_t) month[ 0 ];
	utf8_string[ *utf8_string_index + 1 ] = (uint8_t) month[ 1 ];
	utf8_string[ *utf8_string_index + 2 ] = (uint8_t) month[ 2 ];

	*utf8_string_index += 3;

	return( 1 );
}

/* Copies an UTF-8 encoded string of a time value in the format (HH:MM:SS) from time elements
 * Returns 1 if successful or -1 on error
 */
int libewf_utf8_string_time_copy_from_time_elements(
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     struct tm *time_elements,
     liberror_error_t **error )
{
	static char *function = "libewf_utf8_string_time_copy_from_time_elements";

	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string index.",
		 function );

		return( -1 );
	}
       	if( *utf8_string_index >= utf8_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string index value out of bounds.",
		 function );

		return( -1 );
       	}
	if( ( *utf8_string_index + 8 ) >= utf8_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_hour < 0 )
	 || ( time_elements->tm_hour > 23 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported hours.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_min < 0 )
	 || ( time_elements->tm_min > 59 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported minutes.",
		 function );

		return( -1 );
	}
	/* Note a value of 60 is used for a leap-second
	 */
	if( ( time_elements->tm_sec < 0 )
	 || ( time_elements->tm_sec > 60 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported seconds.",
		 function );

		return( -1 );
	}
	if( time_elements->tm_hour < 10 )
	{
		utf8_string[ *utf8_string_index ] = (uint8_t) '0';

		*utf8_string_index += 1;
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     utf8_string,
	     utf8_string_size,
	     utf8_string_index,
	     (uint8_t) time_elements->tm_hour,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hours to UTF-8 string.",
		 function );

		return( -1 );
	}
	utf8_string[ *utf8_string_index ] = (uint8_t) ':';

	*utf8_string_index += 1;

	if( time_elements->tm_min < 10 )
	{
		utf8_string[ *utf8_string_index ] = (uint8_t) '0';

		*utf8_string_index += 1;
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     utf8_string,
	     utf8_string_size,
	     utf8_string_index,
	     (uint8_t) time_elements->tm_min,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy minutes to UTF-8 string.",
		 function );

		return( -1 );
	}
	utf8_string[ *utf8_string_index ] = (uint8_t) ':';

	*utf8_string_index += 1;

	if( time_elements->tm_sec < 10 )
	{
		utf8_string[ *utf8_string_index ] = (uint8_t) '0';

		*utf8_string_index += 1;
	}
	if( libfvalue_utf8_string_decimal_copy_from_8bit(
	     utf8_string,
	     utf8_string_size,
	     utf8_string_index,
	     (uint8_t) time_elements->tm_sec,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy seconds to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies an UTF-16 encoded string of a day of week from time elements
 * Returns 1 if successful or -1 on error
 */
int libewf_utf16_string_day_of_week_copy_from_time_elements(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     struct tm *time_elements,
     liberror_error_t **error )
{
	const char *day_of_week = NULL;
	static char *function   = "libewf_utf16_string_day_of_week_copy_from_time_elements";

	if( utf16_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string index.",
		 function );

		return( -1 );
	}
       	if( *utf16_string_index >= utf16_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string index value out of bounds.",
		 function );

		return( -1 );
       	}
	if( ( *utf16_string_index + 3 ) >= utf16_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_wday < 0 )
	 || ( time_elements->tm_wday > 6 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported day of week.",
		 function );

		return( -1 );
	}
	switch( time_elements->tm_wday )
	{
		case 0:
			day_of_week = "Sun";
			break;
		case 1:
			day_of_week = "Mon";
			break;
		case 2:
			day_of_week = "Tue";
			break;
		case 3:
			day_of_week = "Wed";
			break;
		case 4:
			day_of_week = "Thu";
			break;
		case 5:
			day_of_week = "Fri";
			break;
		case 6:
			day_of_week = "Sat";
			break;
	}
	utf16_string[ *utf16_string_index     ] = (uint16_t) day_of_week[ 0 ];
	utf16_string[ *utf16_string_index + 1 ] = (uint16_t) day_of_week[ 1 ];
	utf16_string[ *utf16_string_index + 2 ] = (uint16_t) day_of_week[ 2 ];

	*utf16_string_index += 3;

	return( 1 );
}

/* Copies an UTF-16 encoded string of a month from time elements
 * Returns 1 if successful or -1 on error
 */
int libewf_utf16_string_month_copy_from_time_elements(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     struct tm *time_elements,
     liberror_error_t **error )
{
	const char *month     = NULL;
	static char *function = "libewf_utf16_string_month_copy_from_time_elements";

	if( utf16_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string index.",
		 function );

		return( -1 );
	}
       	if( *utf16_string_index >= utf16_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string index value out of bounds.",
		 function );

		return( -1 );
       	}
	if( ( *utf16_string_index + 3 ) >= utf16_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_mon < 0 )
	 || ( time_elements->tm_mon > 11 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported month.",
		 function );

		return( -1 );
	}
	switch( time_elements->tm_mon )
	{
		case 0:
			month = "Jan";
			break;
		case 1:
			month = "Feb";
			break;
		case 2:
			month = "Mar";
			break;
		case 3:
			month = "Apr";
			break;
		case 4:
			month = "May";
			break;
		case 5:
			month = "Jun";
			break;
		case 6:
			month = "Jul";
			break;
		case 7:
			month = "Aug";
			break;
		case 16:
			month = "Sep";
			break;
		case 9:
			month = "Oct";
			break;
		case 10:
			month = "Nov";
			break;
		case 11:
			month = "Dec";
			break;
	}
	utf16_string[ *utf16_string_index     ] = (uint16_t) month[ 0 ];
	utf16_string[ *utf16_string_index + 1 ] = (uint16_t) month[ 1 ];
	utf16_string[ *utf16_string_index + 2 ] = (uint16_t) month[ 2 ];

	*utf16_string_index += 3;

	return( 1 );
}

/* Copies an UTF-16 encoded string of a time value in the format (HH:MM:SS) from time elements
 * Returns 1 if successful or -1 on error
 */
int libewf_utf16_string_time_copy_from_time_elements(
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     struct tm *time_elements,
     liberror_error_t **error )
{
	static char *function = "libewf_utf16_string_time_copy_from_time_elements";

	if( utf16_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string index.",
		 function );

		return( -1 );
	}
       	if( *utf16_string_index >= utf16_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 string index value out of bounds.",
		 function );

		return( -1 );
       	}
	if( ( *utf16_string_index + 16 ) >= utf16_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( time_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid time elements.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_hour < 0 )
	 || ( time_elements->tm_hour > 23 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported hours.",
		 function );

		return( -1 );
	}
	if( ( time_elements->tm_min < 0 )
	 || ( time_elements->tm_min > 59 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported minutes.",
		 function );

		return( -1 );
	}
	/* Note a value of 60 is used for a leap-second
	 */
	if( ( time_elements->tm_sec < 0 )
	 || ( time_elements->tm_sec > 60 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported seconds.",
		 function );

		return( -1 );
	}
	if( time_elements->tm_hour < 10 )
	{
		utf16_string[ *utf16_string_index ] = (uint16_t) '0';

		*utf16_string_index += 1;
	}
	if( libfvalue_utf16_string_decimal_copy_from_8bit(
	     utf16_string,
	     utf16_string_size,
	     utf16_string_index,
	     (uint8_t) time_elements->tm_hour,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy hours to UTF-16 string.",
		 function );

		return( -1 );
	}
	utf16_string[ *utf16_string_index ] = (uint16_t) ':';

	*utf16_string_index += 1;

	if( time_elements->tm_min < 10 )
	{
		utf16_string[ *utf16_string_index ] = (uint16_t) '0';

		*utf16_string_index += 1;
	}
	if( libfvalue_utf16_string_decimal_copy_from_8bit(
	     utf16_string,
	     utf16_string_size,
	     utf16_string_index,
	     (uint8_t) time_elements->tm_min,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy minutes to UTF-16 string.",
		 function );

		return( -1 );
	}
	utf16_string[ *utf16_string_index ] = (uint16_t) ':';

	*utf16_string_index += 1;

	if( time_elements->tm_sec < 10 )
	{
		utf16_string[ *utf16_string_index ] = (uint16_t) '0';

		*utf16_string_index += 1;
	}
	if( libfvalue_utf16_string_decimal_copy_from_8bit(
	     utf16_string,
	     utf16_string_size,
	     utf16_string_index,
	     (uint8_t) time_elements->tm_sec,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy seconds to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies a date time values string to a string in a specific formatted UTF-8 encoded date and time string
 * The string must be at least 32 characters of size this includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_values_copy_to_utf8_string(
     const uint8_t *date_time_values_string,
     size_t date_time_values_string_length,
     int date_format,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	static char *function                = "libewf_date_time_values_copy_to_utf8_string";
	size_t date_time_values_string_index = 0;
	size_t utf8_string_index             = 0;
	time_t timestamp                     = 0;

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
	if( date_time_values_string_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid date time values string length.",
		 function );

		return( -1 );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date format.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < 20 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	if( libewf_date_time_values_copy_to_timestamp(
	     date_time_values_string,
	     date_time_values_string_length,
	     &timestamp,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create timestamp.",
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
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
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
	if( date_format == LIBEWF_DATE_FORMAT_CTIME )
	{
		if( utf8_string_size < 25 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string too small.",
			 function );

			return( -1 );
		}
		if( libewf_utf8_string_day_of_week_copy_from_time_elements(
		     utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy day of week to UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ utf8_string_index++ ] = (uint8_t) ' ';

		if( libewf_utf8_string_month_copy_from_time_elements(
		     utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy month to UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ utf8_string_index++ ] = (uint8_t) ' ';

		if( time_elements.tm_mday < 10 )
		{
			utf8_string[ utf8_string_index++ ] = (uint8_t) ' ';
		}
		if( libfvalue_utf8_string_decimal_copy_from_8bit(
		     utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     (uint8_t) time_elements.tm_mday,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy day of month to UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ utf8_string_index++ ] = (uint8_t) ' ';

		if( libewf_utf8_string_time_copy_from_time_elements(
		     utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy time to UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ utf8_string_index++ ] = (uint8_t) ' ';

		if( libfvalue_utf8_string_decimal_copy_from_16bit(
		     utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     (uint16_t) time_elements.tm_year + 1900,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy year to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	else if( ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
	      || ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	      || ( date_format == LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		if( ( time_elements.tm_mon < 0 )
		 || ( time_elements.tm_mon > 11 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported month.",
			 function );

			return( -1 );
		}
		if( ( time_elements.tm_mday < 1 )
		 || ( time_elements.tm_mday > 31 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported day of month.",
			 function );

			return( -1 );
		}
		if( ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
		 || ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH ) )
		{
			if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
			{
				if( ( time_elements.tm_mon + 1 ) < 10 )
				{
					utf8_string[ utf8_string_index++ ] = (uint8_t) '0';
				}
				if( libfvalue_utf8_string_decimal_copy_from_8bit(
				     utf8_string,
				     utf8_string_size,
				     &utf8_string_index,
				     (uint8_t) time_elements.tm_mon + 1,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy month to UTF-8 string.",
					 function );

					return( -1 );
				}
				utf8_string[ utf8_string_index++ ] = (uint8_t) '/';

				if( time_elements.tm_mday < 10 )
				{
					utf8_string[ utf8_string_index++ ] = (uint8_t) '0';
				}
				if( libfvalue_utf8_string_decimal_copy_from_8bit(
				     utf8_string,
				     utf8_string_size,
				     &utf8_string_index,
				     (uint8_t) time_elements.tm_mday,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy day of month to UTF-8 string.",
					 function );

					return( -1 );
				}
			}
			else if( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
			{
				if( time_elements.tm_mday < 10 )
				{
					utf8_string[ utf8_string_index++ ] = (uint8_t) '0';
				}
				if( libfvalue_utf8_string_decimal_copy_from_8bit(
				     utf8_string,
				     utf8_string_size,
				     &utf8_string_index,
				     (uint8_t) time_elements.tm_mday,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy day of month to UTF-8 string.",
					 function );

					return( -1 );
				}
				utf8_string[ utf8_string_index++ ] = (uint8_t) '/';

				if( ( time_elements.tm_mon + 1 ) < 10 )
				{
					utf8_string[ utf8_string_index++ ] = (uint8_t) '0';
				}
				if( libfvalue_utf8_string_decimal_copy_from_8bit(
				     utf8_string,
				     utf8_string_size,
				     &utf8_string_index,
				     (uint8_t) time_elements.tm_mon + 1,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy month to UTF-8 string.",
					 function );

					return( -1 );
				}
			}
			utf8_string[ utf8_string_index++ ] = (uint8_t) '/';

			if( libfvalue_utf8_string_decimal_copy_from_16bit(
			     utf8_string,
			     utf8_string_size,
			     &utf8_string_index,
			     (uint16_t) time_elements.tm_year + 1900,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy year to UTF-8 string.",
				 function );

				return( -1 );
			}
			utf8_string[ utf8_string_index++ ] = (uint8_t) ' ';
		}
		else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
		{
			if( libfvalue_utf8_string_decimal_copy_from_16bit(
			     utf8_string,
			     utf8_string_size,
			     &utf8_string_index,
			     (uint16_t) time_elements.tm_year + 1900,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy year to UTF-8 string.",
				 function );

				return( -1 );
			}
			utf8_string[ utf8_string_index++ ] = (uint8_t) '-';

			if( ( time_elements.tm_mon + 1 ) < 10 )
			{
				utf8_string[ utf8_string_index++ ] = (uint8_t) '0';
			}
			if( libfvalue_utf8_string_decimal_copy_from_8bit(
			     utf8_string,
			     utf8_string_size,
			     &utf8_string_index,
			     (uint8_t) time_elements.tm_mon + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy month to UTF-8 string.",
				 function );

				return( -1 );
			}
			utf8_string[ utf8_string_index++ ] = (uint8_t) '-';

			if( time_elements.tm_mday < 10 )
			{
				utf8_string[ utf8_string_index++ ] = (uint8_t) '0';
			}
			if( libfvalue_utf8_string_decimal_copy_from_8bit(
			     utf8_string,
			     utf8_string_size,
			     &utf8_string_index,
			     (uint8_t) time_elements.tm_mday,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy day of month to UTF-8 string.",
				 function );

				return( -1 );
			}
			utf8_string[ utf8_string_index++ ] = (uint8_t) 'T';
		}
		if( libewf_utf8_string_time_copy_from_time_elements(
		     utf8_string,
		     utf8_string_size,
		     &utf8_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy time to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		if( date_time_values_string_length >= 27 )
		{
			if( ( utf8_string_index + 7 ) >= utf8_string_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: UTF-8 string too small.",
				 function );

				return( -1 );
			}
			if( ( date_time_values_string[ 20 ] == (uint8_t ) '+' )
			 || ( date_time_values_string[ 20 ] == (uint8_t ) '-' ) )
			{
				utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ 20 ];
				utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ 21 ];
				utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ 22 ];
				utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ 23 ];
				utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ 24 ];
				utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ 25 ];
				utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ 26 ];
			}
		}
	}
	else if( date_time_values_string_length > 19 )
	{
		if( ( utf8_string_index + ( date_time_values_string_length - 19 ) ) >= utf8_string_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string too small.",
			 function );

			return( -1 );
		}
		for( date_time_values_string_index = 19;
		     date_time_values_string_index <= date_time_values_string_length;
		     date_time_values_string_index++ )
		{
			utf8_string[ utf8_string_index++ ] = (uint8_t) date_time_values_string[ date_time_values_string_index ];
		}
	}
	if( utf8_string_index >= utf8_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	utf8_string[ utf8_string_index ] = 0;

	return( 1 );
}

/* Copies a date time values string to a string in a specific formatted UTF-16 encoded date and time string
 * The string must be at least 32 characters of size this includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_values_copy_to_utf16_string(
     const uint8_t *date_time_values_string,
     size_t date_time_values_string_length,
     int date_format,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	static char *function                = "libewf_date_time_values_copy_to_utf16_string";
	size_t date_time_values_string_index = 0;
	size_t utf16_string_index            = 0;
	time_t timestamp                     = 0;

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
	if( date_time_values_string_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid date time values string length.",
		 function );

		return( -1 );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported date format.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < 20 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	if( libewf_date_time_values_copy_to_timestamp(
	     date_time_values_string,
	     date_time_values_string_length,
	     &timestamp,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create timestamp.",
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
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
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
	if( date_format == LIBEWF_DATE_FORMAT_CTIME )
	{
		if( utf16_string_size < 25 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-16 string too small.",
			 function );

			return( -1 );
		}
		if( libewf_utf16_string_day_of_week_copy_from_time_elements(
		     utf16_string,
		     utf16_string_size,
		     &utf16_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy day of week to UTF-16 string.",
			 function );

			return( -1 );
		}
		utf16_string[ utf16_string_index++ ] = (uint16_t) ' ';

		if( libewf_utf16_string_month_copy_from_time_elements(
		     utf16_string,
		     utf16_string_size,
		     &utf16_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy month to UTF-16 string.",
			 function );

			return( -1 );
		}
		utf16_string[ utf16_string_index++ ] = (uint16_t) ' ';

		if( time_elements.tm_mday < 10 )
		{
			utf16_string[ utf16_string_index++ ] = (uint16_t) ' ';
		}
		if( libfvalue_utf16_string_decimal_copy_from_8bit(
		     utf16_string,
		     utf16_string_size,
		     &utf16_string_index,
		     (uint8_t) time_elements.tm_mday,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy day of month to UTF-16 string.",
			 function );

			return( -1 );
		}
		utf16_string[ utf16_string_index++ ] = (uint16_t) ' ';

		if( libewf_utf16_string_time_copy_from_time_elements(
		     utf16_string,
		     utf16_string_size,
		     &utf16_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy time to UTF-16 string.",
			 function );

			return( -1 );
		}
		utf16_string[ utf16_string_index++ ] = (uint16_t) ' ';

		if( libfvalue_utf16_string_decimal_copy_from_16bit(
		     utf16_string,
		     utf16_string_size,
		     &utf16_string_index,
		     (uint16_t) time_elements.tm_year + 1900,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy year to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	else if( ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
	      || ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	      || ( date_format == LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		if( ( time_elements.tm_mon < 0 )
		 || ( time_elements.tm_mon > 11 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported month.",
			 function );

			return( -1 );
		}
		if( ( time_elements.tm_mday < 1 )
		 || ( time_elements.tm_mday > 31 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported day of month.",
			 function );

			return( -1 );
		}
		if( ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
		 || ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH ) )
		{
			if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
			{
				if( ( time_elements.tm_mon + 1 ) < 10 )
				{
					utf16_string[ utf16_string_index++ ] = (uint16_t) '0';
				}
				if( libfvalue_utf16_string_decimal_copy_from_8bit(
				     utf16_string,
				     utf16_string_size,
				     &utf16_string_index,
				     (uint8_t) time_elements.tm_mon + 1,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy month to UTF-16 string.",
					 function );

					return( -1 );
				}
				utf16_string[ utf16_string_index++ ] = (uint16_t) '/';

				if( time_elements.tm_mday < 10 )
				{
					utf16_string[ utf16_string_index++ ] = (uint16_t) '0';
				}
				if( libfvalue_utf16_string_decimal_copy_from_8bit(
				     utf16_string,
				     utf16_string_size,
				     &utf16_string_index,
				     (uint8_t) time_elements.tm_mday,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy day of month to UTF-16 string.",
					 function );

					return( -1 );
				}
			}
			else if( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
			{
				if( time_elements.tm_mday < 10 )
				{
					utf16_string[ utf16_string_index++ ] = (uint16_t) '0';
				}
				if( libfvalue_utf16_string_decimal_copy_from_8bit(
				     utf16_string,
				     utf16_string_size,
				     &utf16_string_index,
				     (uint8_t) time_elements.tm_mday,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy day of month to UTF-16 string.",
					 function );

					return( -1 );
				}
				utf16_string[ utf16_string_index++ ] = (uint16_t) '/';

				if( ( time_elements.tm_mon + 1 ) < 10 )
				{
					utf16_string[ utf16_string_index++ ] = (uint16_t) '0';
				}
				if( libfvalue_utf16_string_decimal_copy_from_8bit(
				     utf16_string,
				     utf16_string_size,
				     &utf16_string_index,
				     (uint8_t) time_elements.tm_mon + 1,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy month to UTF-16 string.",
					 function );

					return( -1 );
				}
			}
			utf16_string[ utf16_string_index++ ] = (uint16_t) '/';

			if( libfvalue_utf16_string_decimal_copy_from_16bit(
			     utf16_string,
			     utf16_string_size,
			     &utf16_string_index,
			     (uint16_t) time_elements.tm_year + 1900,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy year to UTF-16 string.",
				 function );

				return( -1 );
			}
			utf16_string[ utf16_string_index++ ] = (uint16_t) ' ';
		}
		else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
		{
			if( libfvalue_utf16_string_decimal_copy_from_16bit(
			     utf16_string,
			     utf16_string_size,
			     &utf16_string_index,
			     (uint16_t) time_elements.tm_year + 1900,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy year to UTF-16 string.",
				 function );

				return( -1 );
			}
			utf16_string[ utf16_string_index++ ] = (uint16_t) '-';

			if( ( time_elements.tm_mon + 1 ) < 10 )
			{
				utf16_string[ utf16_string_index++ ] = (uint16_t) '0';
			}
			if( libfvalue_utf16_string_decimal_copy_from_8bit(
			     utf16_string,
			     utf16_string_size,
			     &utf16_string_index,
			     (uint8_t) time_elements.tm_mon + 1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy month to UTF-16 string.",
				 function );

				return( -1 );
			}
			utf16_string[ utf16_string_index++ ] = (uint16_t) '-';

			if( time_elements.tm_mday < 10 )
			{
				utf16_string[ utf16_string_index++ ] = (uint16_t) '0';
			}
			if( libfvalue_utf16_string_decimal_copy_from_8bit(
			     utf16_string,
			     utf16_string_size,
			     &utf16_string_index,
			     (uint8_t) time_elements.tm_mday,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy day of month to UTF-16 string.",
				 function );

				return( -1 );
			}
			utf16_string[ utf16_string_index++ ] = (uint16_t) 'T';
		}
		if( libewf_utf16_string_time_copy_from_time_elements(
		     utf16_string,
		     utf16_string_size,
		     &utf16_string_index,
		     &time_elements,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy time to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		if( date_time_values_string_length >= 27 )
		{
			if( ( utf16_string_index + 7 ) >= utf16_string_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: UTF-16 string too small.",
				 function );

				return( -1 );
			}
			if( ( date_time_values_string[ 20 ] == (uint16_t ) '+' )
			 || ( date_time_values_string[ 20 ] == (uint16_t ) '-' ) )
			{
				utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ 20 ];
				utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ 21 ];
				utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ 22 ];
				utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ 23 ];
				utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ 24 ];
				utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ 25 ];
				utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ 26 ];
			}
		}
	}
	else if( date_time_values_string_length > 19 )
	{
		if( ( utf16_string_index + ( date_time_values_string_length - 19 ) ) >= utf16_string_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-16 string too small.",
			 function );

			return( -1 );
		}
		for( date_time_values_string_index = 19;
		     date_time_values_string_index < date_time_values_string_length;
		     date_time_values_string_index++ )
		{
			utf16_string[ utf16_string_index++ ] = (uint16_t) date_time_values_string[ date_time_values_string_index ];
		}
	}
	if( utf16_string_index >= utf16_string_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	utf16_string[ utf16_string_index ] = 0;

	return( 1 );
}


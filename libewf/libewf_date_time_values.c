/*
 * Date and time values functions
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
#include <narrow_string.h>
#include <memory.h>
#include <wide_string.h>
#include <types.h>

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

/* Copies date and time values string from a timestamp
 * The string must be at least 20 characters + the length of the timezone string and/or timezone name of size including the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_values_copy_from_timestamp(
     libewf_character_t *date_time_values_string,
     size_t date_time_values_string_size,
     time_t timestamp,
     liberror_error_t **error )
{
	struct tm time_elements;

	static char *function    = "libewf_date_time_values_copy_from_timestamp";
	int print_count          = 0;

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
	print_count = libewf_string_snprintf(
		       date_time_values_string,
		       date_time_values_string_size,
		       "%04d %02d %02d %02d %02d %02d",
		       time_elements.tm_year + 1900,
		       time_elements.tm_mon + 1,
		       time_elements.tm_mday,
		       time_elements.tm_hour,
		       time_elements.tm_min,
		       time_elements.tm_sec );

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > date_time_values_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set date time values string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies date and time values string to a timestamp
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_values_copy_to_timestamp(
     libewf_character_t *date_time_values_string,
     size_t date_time_values_string_length,
     time_t *timestamp,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_character_t **date_time_elements = NULL;
	static char *function                   = "libewf_convert_date_header_value";
	size_t amount_of_date_time_elements     = 0;

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
	if( libewf_string_split(
	     date_time_values_string,
	     date_time_values_string_length,
	     (libewf_character_t) ' ',
	     &date_time_elements,
	     &amount_of_date_time_elements,
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
	if( amount_of_date_time_elements < 6 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported amount of elements in date time values string.",
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
	time_elements.tm_mon = (int) ( ( ( date_time_elements[ 1 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
	                     + ( date_time_elements[ 1 ][ 1 ] - (libewf_character_t) '0' )
	                     - 1 );

	/* Set the day of the month
	 */
	time_elements.tm_mday = (int) ( ( ( date_time_elements[ 2 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
	                      + ( date_time_elements[ 2 ][ 1 ] - (libewf_character_t) '0' ) );

	/* Set the hour
	 */
	time_elements.tm_hour = (int) ( ( ( date_time_elements[ 3 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
	                      + ( date_time_elements[ 3 ][ 1 ] - (libewf_character_t) '0' ) );

	/* Set the minutes
	 */
	time_elements.tm_min = (int) ( ( ( date_time_elements[ 4 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
	                     + ( date_time_elements[ 4 ][ 1 ] - (libewf_character_t) '0' ) );

	/* Set the seconds
	 */
	time_elements.tm_sec = (int) ( ( ( date_time_elements[ 5 ][ 0 ] - (libewf_character_t) '0' ) * 10 )
	                     + ( date_time_elements[ 5 ][ 1 ] - (libewf_character_t) '0' ) );

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

/* Copies a date time values string to a string in a specific date format
 * The string must be at least 32 characters of size this includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_date_time_values_copy_to_string(
     libewf_character_t *date_time_values_string,
     size_t date_time_values_string_length,
     int date_format,
     libewf_character_t *string,
     size_t string_size,
     liberror_error_t **error )
{
	struct tm time_elements;

	libewf_character_t *day_of_week = NULL;
	libewf_character_t *month       = NULL;
	static char *function           = "libewf_date_time_values_copy_to_string";
	time_t timestamp                = 0;
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
	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( string_size < 20 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: string too small.",
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
	if( date_format == LIBEWF_DATE_FORMAT_CTIME )
	{
		if( string_size < 25 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: string too small.",
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
		print_count = libewf_string_snprintf(
			       string,
			       string_size,
			       "%s %s %2d %02d:%02d:%02d %04d",
			       (char *) day_of_week,
			       (char *) month,
			       time_elements.tm_mday,
			       time_elements.tm_hour,
			       time_elements.tm_min,
			       time_elements.tm_sec,
			       time_elements.tm_year + 1900 );
	}
	else if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
	{
		print_count = libewf_string_snprintf(
			       string,
			       string_size,
			       "%02d/%02d/%04d %02d:%02d:%02d",
			       time_elements.tm_mon + 1,
			       time_elements.tm_mday,
			       time_elements.tm_year + 1900,
			       time_elements.tm_hour,
			       time_elements.tm_min,
			       time_elements.tm_sec );
	}
	else if( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	{
		print_count = libewf_string_snprintf(
			       string,
			       string_size,
			       "%02d/%02d/%04d %02d:%02d:%02d",
			       time_elements.tm_mday,
			       time_elements.tm_mon + 1,
			       time_elements.tm_year + 1900,
			       time_elements.tm_hour,
			       time_elements.tm_min,
			       time_elements.tm_sec );
	}
	else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		print_count = libewf_string_snprintf(
			       string,
			       string_size,
			       "%04d-%02d-%02dT%02d:%02d:%02d",
			       time_elements.tm_year + 1900,
			       time_elements.tm_mon + 1,
			       time_elements.tm_mday,
			       time_elements.tm_hour,
			       time_elements.tm_min,
			       time_elements.tm_sec );
	}
	if( ( print_count <= -1 )
	 || ( (size_t) print_count > string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
	if( date_time_values_string_length > 19 )
	{
		if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
		{
			if( ( date_time_values_string[ 20 ] == (libewf_character_t ) '+' )
			 || ( date_time_values_string[ 20 ] == (libewf_character_t ) '-' ) )
			{
				print_count = libewf_string_snprintf(
					       &( string[ 19 ] ),
					       7,
					       "%" PRIs_LIBEWF "",
					       &( date_time_values_string[ 20 ] ) );
			}
		}
		else
		{
			print_count = libewf_string_snprintf(
				       &( string[ print_count ] ),
				       string_size - print_count,
				       "%" PRIs_LIBEWF "",
				       &( date_time_values_string[ 19 ] ) );
		}
		if( ( print_count <= -1 )
		 || ( (size_t) print_count > string_size ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}


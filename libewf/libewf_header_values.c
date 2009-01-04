/*
 * libewf header values
 *
 * Copyright (c) 2006-2008, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "libewf_includes.h"

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_header_values.h"
#include "libewf_notify.h"
#include "libewf_string.h"

#include "ewf_definitions.h"

/* Initializes the header values
 * Returns 1 if successful, or -1 otherwise
 */
int libewf_header_values_initialize( LIBEWF_VALUES_TABLE *header_values )
{
	static char *function = "libewf_header_values_initialize";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( -1 );
	}
	header_values->identifiers[ 0 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "case_number" ), 11 );
	header_values->identifiers[ 1 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "description" ), 11 );
	header_values->identifiers[ 2 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "examiner_name" ), 13 );
	header_values->identifiers[ 3 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "evidence_number" ), 15 );
	header_values->identifiers[ 4 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "notes" ), 5 );
	header_values->identifiers[ 5 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "acquiry_date" ), 12 );
	header_values->identifiers[ 6 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "system_date" ), 11 );
	header_values->identifiers[ 7 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "acquiry_operating_system" ), 24 );
	header_values->identifiers[ 8 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "acquiry_software_version" ), 24 );
	header_values->identifiers[ 9 ]  = libewf_string_duplicate( _S_LIBEWF_CHAR( "password" ), 8 );
	header_values->identifiers[ 10 ] = libewf_string_duplicate( _S_LIBEWF_CHAR( "compression_type" ), 16 );
	header_values->identifiers[ 11 ] = libewf_string_duplicate( _S_LIBEWF_CHAR( "model" ), 5 );
	header_values->identifiers[ 12 ] = libewf_string_duplicate( _S_LIBEWF_CHAR( "serial_number" ), 13 );
	header_values->identifiers[ 13 ] = libewf_string_duplicate( _S_LIBEWF_CHAR( "unknown_dc" ), 10 );

	return( 1 );
}

/* Convert a time stamp into a date string
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_convert_date_timestamp( time_t timestamp, uint8_t date_format )
{
	struct tm *time_elements   = NULL;
	libewf_char_t *date_string = NULL;
	libewf_char_t *newline     = NULL;
	static char *function      = "libewf_convert_date_timestamp";
	size_t date_string_length  = 20;

	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported date format.\n",
		 function );

		return( NULL );
	}
	if( date_format == LIBEWF_DATE_FORMAT_CTIME )
	{
		/* The libewf_string_ctime function returns a string of length 32
		 */
		date_string = libewf_string_ctime( &timestamp );

		if( date_string == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
			 function );

			return( NULL );
		}
		newline = libewf_string_search( date_string, (libewf_char_t) '\n', 32 );

		if( newline != NULL )
		{
			newline[ 0 ] = (libewf_char_t) '\0';
		}
	}
	else
	{
		time_elements = libewf_common_localtime( &timestamp );

		if( time_elements == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create time elements.\n",
			 function );

			return( NULL );
		}
		date_string = (libewf_char_t *) libewf_common_alloc(
                                                 sizeof( libewf_char_t ) * date_string_length );

		if( date_string == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
			 function );

			libewf_common_free( time_elements );

			return( NULL );
		}
		if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
		{
			if( libewf_string_snprintf(
			     date_string,
			     date_string_length,
			     _S_LIBEWF_CHAR( "%02d/%02d/%04d %02d:%02d:%02d" ),
			     ( time_elements->tm_mon + 1 ),
			     time_elements->tm_mday,
			     ( time_elements->tm_year + 1900 ),
			     time_elements->tm_hour,
			     time_elements->tm_min,
			     time_elements->tm_sec ) <= -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to fill date string.\n",
				 function );

				libewf_common_free( time_elements );
				libewf_common_free( date_string );

				return( NULL );
			}
		}
		else if( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
		{
			if( libewf_string_snprintf(
			     date_string,
			     date_string_length,
			     _S_LIBEWF_CHAR( "%02d/%02d/%04d %02d:%02d:%02d" ),
			     time_elements->tm_mday,
			     ( time_elements->tm_mon + 1 ),
			     ( time_elements->tm_year + 1900 ),
			     time_elements->tm_hour,
			     time_elements->tm_min,
			     time_elements->tm_sec ) <= -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to fill date string.\n",
				 function );

				libewf_common_free( time_elements );
				libewf_common_free( date_string );

				return( NULL );
			}
		}
		else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
		{
			if( libewf_string_snprintf(
			     date_string,
			     date_string_length,
			     _S_LIBEWF_CHAR( "%04d-%02d-%02dT%02d:%02d:%02d" ),
			     ( time_elements->tm_year + 1900 ),
			     ( time_elements->tm_mon + 1 ),
			     time_elements->tm_mday,
			     time_elements->tm_hour,
			     time_elements->tm_min,
			     time_elements->tm_sec ) <= -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to fill date string.\n",
				 function );

				libewf_common_free( time_elements );
				libewf_common_free( date_string );

				return( NULL );
			}
		}
		libewf_common_free( time_elements );
	}
	return( date_string );
}

/* Convert a header value into a date string
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_convert_date_header_value( libewf_char_t *header_value, size_t header_value_length, uint8_t date_format )
{
	struct tm time_elements;

	libewf_char_t **date_elements = NULL;
	libewf_char_t *date_string    = NULL;
	static char *function         = "libewf_convert_date_header_value";
	time_t timestamp              = 0;
	uint32_t date_element_count   = 0;

	if( header_value == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header value.\n",
		 function );

		return( NULL );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported date format.\n",
		 function );

		return( NULL );
	}
	date_elements = libewf_string_split(
			 header_value,
			 header_value_length,
			 (libewf_char_t) ' ',
			 &date_element_count );

	if( date_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to split date elements in header value.\n",
		 function );

		return( NULL );
	}
	if( date_element_count != 6 )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported amount of date elements in header value.\n",
		 function );

		libewf_string_split_values_free( date_elements, date_element_count );

		return( NULL );
	}
	/* Set the year
	 */
	time_elements.tm_year = (int) ( ( ( date_elements[ 0 ][ 0 ] - (libewf_char_t) '0' ) * 1000 )
	                      + ( ( date_elements[ 0 ][ 1 ] - (libewf_char_t) '0' ) * 100 )
	                      + ( ( date_elements[ 0 ][ 2 ] - (libewf_char_t) '0' ) * 10 )
	                      + ( date_elements[ 0 ][ 3 ] - (libewf_char_t) '0' )
	                      - 1900 );

	/* Set the month
	 */
	if( date_elements[ 1 ][ 1 ] == (libewf_char_t) '\0' )
	{
		time_elements.tm_mon = (int) ( ( date_elements[ 1 ][ 0 ] - (libewf_char_t) '0' )
		                     - 1 );
	}
	else
	{
		time_elements.tm_mon = (int) ( ( ( date_elements[ 1 ][ 0 ] - (libewf_char_t) '0' ) * 10 )
		                     + ( date_elements[ 1 ][ 1 ] - (libewf_char_t) '0' )
		                     - 1 );
	}
	/* Set the day of the month
	 */
	if( date_elements[ 2 ][ 1 ] == (libewf_char_t) '\0' )
	{
		time_elements.tm_mday = (int) ( date_elements[ 2 ][ 0 ] - (libewf_char_t) '0' );
	}
	else
	{
		time_elements.tm_mday = (int) ( ( ( date_elements[ 2 ][ 0 ] - (libewf_char_t) '0' ) * 10 )
		                      + ( date_elements[ 2 ][ 1 ] - (libewf_char_t) '0' ) );
	}
	/* Set the hour
	 */
	if( date_elements[ 3 ][ 1 ] == (libewf_char_t) '\0' )
	{
		time_elements.tm_hour = (int) ( date_elements[ 3 ][ 0 ] - (libewf_char_t) '0' );
	}
	else
	{
		time_elements.tm_hour = (int) ( ( ( date_elements[ 3 ][ 0 ] - (libewf_char_t) '0' ) * 10 )
		                      + ( date_elements[ 3 ][ 1 ] - (libewf_char_t) '0' ) );
	}
	/* Set the minutes
	 */
	if( date_elements[ 4 ][ 1 ] == (libewf_char_t) '\0' )
	{
		time_elements.tm_min = (int) ( date_elements[ 4 ][ 0 ] - (libewf_char_t) '0' );
	}
	else
	{
		time_elements.tm_min = (int) ( ( ( date_elements[ 4 ][ 0 ] - (libewf_char_t) '0' ) * 10 )
		                     + ( date_elements[ 4 ][ 1 ] - (libewf_char_t) '0' ) );
	}
	/* Set the seconds
	 */
	if( date_elements[ 5 ][ 1 ] == (libewf_char_t) '\0' )
	{
		time_elements.tm_sec = (int) ( date_elements[ 5 ][ 0 ] - (libewf_char_t) '0' );
	}
	else
	{
		time_elements.tm_sec = (int) ( ( ( date_elements[ 5 ][ 0 ] - (libewf_char_t) '0' ) * 10 )
		                     + ( date_elements[ 5 ][ 1 ] - (libewf_char_t) '0' ) );
	}
	/* Set to ignore the daylight saving time
	 */
	time_elements.tm_isdst = -1;

	/* Create a timestamp
	 */
	timestamp = libewf_common_mktime( &time_elements );

	libewf_string_split_values_free( date_elements, date_element_count );

	if( timestamp == (time_t) -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create timestamp.\n",
		 function );

		return( NULL );
	}
	date_string = libewf_convert_date_timestamp( timestamp, date_format );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
		 function );

		return( NULL );
	}
	return( date_string );
}

/* Generate date string within a header value
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_generate_date_header_value( time_t timestamp )
{
	libewf_char_t *date_string = NULL;
	struct tm *time_elements   = NULL;
	static char *function      = "libewf_generate_date_header_value";

	time_elements = libewf_common_localtime( &timestamp );

	if( time_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create time elements.\n",
		 function );

		return( NULL );
	}
	date_string = (libewf_char_t *) libewf_common_alloc(
	                                 sizeof( libewf_char_t ) * 20 );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
		 function );

		libewf_common_free( time_elements );

		return( NULL );
	}
	if( libewf_string_snprintf(
	     date_string,
	     20,
	     _S_LIBEWF_CHAR( "%4d %d %d %d %d %d" ),
	     ( time_elements->tm_year + 1900 ),
	     ( time_elements->tm_mon + 1 ),
	     time_elements->tm_mday,
	     time_elements->tm_hour,
	     time_elements->tm_min,
	     time_elements->tm_sec ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to fill date string.\n",
		 function );

		libewf_common_free( date_string );
		libewf_common_free( time_elements );

		return( NULL );
	}
	/* Make sure the string is terminated correctly
	 */
	date_string[ 19 ] = (libewf_char_t) '\0';

	libewf_common_free( time_elements );

	return( date_string );
}

/* Convert a header2 value into a date string
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_convert_date_header2_value( libewf_char_t *header_value, size_t header_value_length, uint8_t date_format )
{
	libewf_char_t *date_string = NULL;
	static char *function      = "libewf_convert_date_header2_value";
	time_t timestamp           = 0;

	if( header_value == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header value.\n",
		 function );

		return( NULL );
	}
	timestamp   = (time_t) libewf_string_to_int64( header_value, header_value_length );
	date_string = libewf_convert_date_timestamp( timestamp, date_format );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
		 function );

		return( NULL );
	}
	return( date_string );
}

/* Generate date string within a header2 value
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_generate_date_header2_value( time_t timestamp )
{
	libewf_char_t *date_string = NULL;
	static char *function      = "libewf_generate_date_header2_value";

	date_string = (libewf_char_t *) libewf_common_alloc(
	                                 sizeof( libewf_char_t ) * 11 );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
		 function );

		return( NULL );
	}
	if( libewf_string_snprintf(
	     date_string,
	     11,
	     _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIu32 ),
	     (uint32_t) timestamp ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
		 function );

		libewf_common_free( date_string );

		return( NULL );
	}
	/* Make sure the string is terminated correctly
	 */
	date_string[ 10 ] = (libewf_char_t) '\0';

	return( date_string );
}

/* Copies the header values from the source to the destination
 * Returns 1 if successful, -1 on error
 */
int libewf_header_values_copy( LIBEWF_VALUES_TABLE *destination_header_values, LIBEWF_VALUES_TABLE *source_header_values )
{
	static char *function = "libewf_header_values_copy";
	size_t string_length  = 0;
	int32_t index         = 0;

	if( destination_header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid destination header values.\n",
		 function );

		return( -1 );
	}
	if( source_header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source header values.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->identifiers == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source header values - missing identifiers.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source header values - missing values.\n",
		 function );

		return( -1 );
	}
	if( source_header_values->amount > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid source header values amount value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( index = 0; index < (int32_t) source_header_values->amount; index++ )
	{
		/* Skip the acquiry and system date
		 */
		if( ( index == LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE )
		 || ( index == LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ) )
		{
			continue;
		}
		/* Skip empty values
		 */
		if( source_header_values->values[ index ] == NULL )
		{
			LIBEWF_VERBOSE_PRINT( "%s: missing header value for index: %" PRIi32 ".\n",
			 function, index );

			continue;
		}
		string_length = libewf_string_length( source_header_values->values[ index ] );

		/* Skip empty values
		 */
		if( string_length == 0 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: empty header value for index: %" PRIi32 ".\n",
			 function, index );

			continue;
		}
		if( libewf_values_table_set_value(
		     destination_header_values,
		     source_header_values->identifiers[ index ],
		     source_header_values->values[ index ],
		     string_length ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set value for index: %" PRIi32 ".\n",
			 function, index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Parse a header string for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_VALUES_TABLE *libewf_header_values_parse_header_string( libewf_char_t *header_string, size_t length, uint8_t date_format )
{
	libewf_values_table_t *header_values = NULL;
	libewf_char_t **lines                = NULL;
	libewf_char_t **types                = NULL;
	libewf_char_t **values               = NULL;
	libewf_char_t *date_string           = NULL;
	static char *function                = "libewf_header_values_parse_header_string";
	size_t string_length                 = 0;
	uint32_t line_count                  = 0;
	uint32_t type_count                  = 0;
	uint32_t value_count                 = 0;
	uint32_t iterator                    = 0;

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string.\n",
		 function );

		return( NULL );
	}
	lines = libewf_string_split(
	         (libewf_char_t *) header_string,
	         length,
	         (libewf_char_t) '\n',
	         &line_count );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to split lines in header string.\n",
		 function );

		return( NULL );
	}
	types = libewf_string_split(
	         lines[ 2 ],
	         libewf_string_length( lines[ 2 ] ),
	         (libewf_char_t) '\t',
	         &type_count );

	if( types == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to split types in header string.\n",
		 function );

		libewf_string_split_values_free( lines, line_count );

		return( NULL );
	}
	values = libewf_string_split(
	          lines[ 3 ],
	          libewf_string_length( lines[ 3 ] ),
	          (libewf_char_t) '\t',
	          &value_count );

	if( values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to split values in header string.\n",
		 function );

		libewf_string_split_values_free( lines, line_count );
		libewf_string_split_values_free( types, type_count );

		return( NULL );
	}
	libewf_string_split_values_free( lines, line_count );

	header_values = libewf_values_table_alloc( LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
		 function );

		libewf_string_split_values_free( types, type_count );
		libewf_string_split_values_free( values, value_count );

		return( NULL );
	}
	if( libewf_header_values_initialize( header_values ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize the header values.\n",
		 function );

		libewf_string_split_values_free( types, type_count );
		libewf_string_split_values_free( values, value_count );

		return( NULL );
	}
	for( iterator = 0; iterator < type_count; iterator++ )
	{
		if( ( values[ iterator ] == NULL )
		 || ( values[ iterator ] == (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			continue;
		}
		string_length = libewf_string_length( values[ iterator ] );

		/* Remove trailing white space
		 */
		if( ( string_length > 0 )
		 && ( values[ iterator ][ string_length - 1 ] == (libewf_char_t) '\r' ) )
		{
			string_length -= 1;
		}
		if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "av" ), 2 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "acquiry_software_version" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set acquiry software version.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "dc" ), 2 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "unknown_dc" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set unknown: dc.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "md" ), 2 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "model" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set model.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "ov" ), 2 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "acquiry_operating_system" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set acquiry operating system.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "sn" ), 2 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "serial_number" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set serial_number.\n",
				 function );
			}
		}
		else if( ( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "m" ), 1 ) == 0 )
		      || ( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "u" ), 1 ) == 0 ) )
		{
			/* If the date string contains spaces it's in the old header
			 * format otherwise is in new header2 format
			 */
			if( libewf_string_search(
			     values[ iterator ],
			     (libewf_char_t) ' ',
			     string_length ) != NULL )
			{
				date_string = libewf_convert_date_header_value( values[ iterator ], string_length, date_format );
			}
			else
			{
				date_string = libewf_convert_date_header2_value( values[ iterator ], string_length, date_format );
			}
			if( date_string == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
				 function );
			}
			else
			{
				/* The string length of the date string is needed
				 */
				string_length = libewf_string_length( date_string );

				if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "m" ), 1 ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _S_LIBEWF_CHAR( "acquiry_date" ),
					     date_string,
					     string_length ) != 1 )
					{
						LIBEWF_VERBOSE_PRINT( "%s: unable to set acquiry date.\n",
						 function );
					}
				}
				else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "u" ), 1 ) == 0 )
				{
					if( libewf_values_table_set_value(
					     header_values,
					     _S_LIBEWF_CHAR( "system_date" ),
					     date_string,
					     string_length ) != 1 )
					{
						LIBEWF_VERBOSE_PRINT( "%s: unable to set system date.\n",
						 function );
					}
				}
				libewf_common_free( date_string );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "p" ), 1 ) == 0 )
		{
			if( string_length == 0 )
			{
				/* Empty hash do nothing
				 */
			}
			else if( ( string_length == 1 ) && ( values[ iterator ][ 0 ] == (libewf_char_t) '0' ) )
			{
				/* Empty hash do nothing
				 */
			}
			else
			{
				if( libewf_values_table_set_value(
				     header_values,
				     _S_LIBEWF_CHAR( "password" ),
				     values[ iterator ],
				     string_length ) != 1 )
				{
					LIBEWF_VERBOSE_PRINT( "%s: unable to set password.\n",
					 function );
				}
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "a" ), 1 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "description" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set description.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "c" ), 1 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "case_number" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set case number.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "n" ), 1 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "evidence_number" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set evidence number.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "e" ), 1 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "examiner_name" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set examiner name.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "t" ), 1 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "notes" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set notes.\n",
				 function );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "r" ), 1 ) == 0 )
		{
			if( libewf_values_table_set_value(
			     header_values,
			     _S_LIBEWF_CHAR( "compression_type" ),
			     values[ iterator ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set compression type.\n",
				 function );
			}
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: unsupported type: %d with value: %" PRIs_EWF ".\n",
			 function, types[ iterator ], values[ iterator ] );

			libewf_string_split_values_free( types, type_count );
			libewf_string_split_values_free( values, value_count );

			return( NULL );
		}
	}
	libewf_string_split_values_free( types, type_count );
	libewf_string_split_values_free( values, value_count );

	return( header_values );
}

/* Parse an EWF header for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_VALUES_TABLE *libewf_header_values_parse_header( ewf_char_t *header, size_t size, uint8_t date_format )
{
	libewf_values_table_t *header_values = NULL;
	libewf_char_t *header_string         = NULL;
	static char *function                = "libewf_header_values_parse_header";

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header.\n",
		 function );

		return( NULL );
	}
	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * ( size + 1 ) );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		return( NULL );
	}
	if( libewf_string_copy_from_header( header_string, size, header, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy header to header string.\n",
		 function );

		libewf_common_free( header_string );

		return( NULL );
	}
	header_values = libewf_header_values_parse_header_string( header_string, size, date_format );

	libewf_common_free( header_string );

	return( header_values );
}

/* Parse an EWF header for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_VALUES_TABLE *libewf_header_values_parse_header2( ewf_char_t *header2, size_t size, uint8_t date_format )
{
	libewf_values_table_t *header_values = NULL;
	libewf_char_t *header_string         = NULL;
	static char *function                = "libewf_header_values_parse_header2";
	size_t header_size                   = 0;

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header2.\n",
		 function );

		return( NULL );
	}
	header_size   = ( size - 2 ) / 2;
	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * ( header_size + 1 ) );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		return( NULL );
	}
	if( libewf_string_copy_from_header2( header_string, header_size, header2, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy header2 to header string.\n",
		 function );

		libewf_common_free( header_string );

		return( NULL );
	}
	header_values = libewf_header_values_parse_header_string( header_string, header_size, date_format );

	libewf_common_free( header_string );

	return( header_values );
}

/* Converts a header string into a header
 * Cleans the header string
 * Sets header length
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_convert_header_string_to_header( libewf_char_t *header_string, size_t string_length, size_t *header_length )
{
	ewf_char_t *header    = NULL;
	static char *function = "libewf_header_values_convert_header_string_to_header";

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string.\n",
		 function );

		return( NULL );
	}
	if( header_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header length.\n",
		 function );

		return( NULL );
	}
	header = (ewf_char_t *) libewf_common_alloc(
	                         sizeof( ewf_char_t ) * string_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		libewf_common_free( header_string );

		return( NULL );
	}
	*header_length = string_length;

	if( libewf_string_copy_to_header( header_string, string_length, header, *header_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header.\n",
		 function );

		libewf_common_free( header );
		libewf_common_free( header_string );

		*header_length = 0;

		return( NULL );
	}
	libewf_common_free( header_string );

	return( header );
}

/* Converts a header string into a header2
 * Cleans the header string
 * Sets header2 length
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_convert_header_string_to_header2( libewf_char_t *header_string, size_t string_length, size_t *header2_length )
{
	ewf_char_t *header2   = NULL;
	static char *function = "libewf_header_values_convert_header_string_to_header2";

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string.\n",
		 function );

		return( NULL );
	}
	if( header2_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header2 length.\n",
		 function );

		return( NULL );
	}
	/* Add a character for the UTF16 endian type
	 */
	*header2_length = ( string_length + 1 ) * 2;

	header2 = (ewf_char_t *) libewf_common_alloc(
	                          sizeof( ewf_char_t ) * *header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header2.\n",
		 function );

		libewf_common_free( header_string );

		*header2_length = 0;

		return( NULL );
	}
	if( libewf_string_copy_to_header2( header_string, string_length, header2, *header2_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header2.\n",
		 function );

		libewf_common_free( header2 );
		libewf_common_free( header_string );

		*header2_length = 0;

		return( NULL );
	}
	libewf_common_free( header_string );

	return( header2 );
}

/* Generate a header string format type 1 (original EWF, EnCase1)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_type1( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, int8_t compression_level, libewf_char_t *header_string_head, libewf_char_t *header_string_tail, size_t *string_length )
{
	libewf_char_t *header_string    = NULL;
	libewf_char_t *case_number      = _S_LIBEWF_CHAR( "" );
	libewf_char_t *description      = _S_LIBEWF_CHAR( "" );
	libewf_char_t *examiner_name    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *evidence_number  = _S_LIBEWF_CHAR( "" );
	libewf_char_t *notes            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *system_date      = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_date     = _S_LIBEWF_CHAR( "" );
	libewf_char_t *password_hash    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *compression_type = _S_LIBEWF_CHAR( "" );
	static char *function           = "libewf_header_values_generate_header_string_type1";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string head.\n",
		 function );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string tail.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		LIBEWF_WARNING_PRINT( "%s: compression level not supported.\n",
		 function );

		return( NULL );
	}
	*string_length = libewf_string_length( header_string_head );

	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] );
		case_number     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] );
		description     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] );
		examiner_name   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] );
		evidence_number = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	{
		*string_length += libewf_string_length(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] );
		notes           =  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] );
		acquiry_date    = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		acquiry_date = libewf_generate_date_header_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( acquiry_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] );
		system_date     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		system_date = libewf_generate_date_header_value( timestamp );

		if( system_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			 *string_length += libewf_string_length( system_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] );
		password_hash   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*string_length += 1;
		password_hash   = _S_LIBEWF_CHAR( "0" );
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] != NULL )
	{
		*string_length  += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] );
		compression_type = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];
	}
	else
	{
		if( compression_level == EWF_COMPRESSION_NONE )
		{
			compression_type = (libewf_char_t *) LIBEWF_COMPRESSION_TYPE_NONE;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = (libewf_char_t *) LIBEWF_COMPRESSION_TYPE_FAST;
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = (libewf_char_t *) LIBEWF_COMPRESSION_TYPE_BEST;
		}
		*string_length += libewf_string_length( compression_type );
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*string_length += 11;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" ),
		header_string_head, case_number, evidence_number, description, examiner_name,
		notes, acquiry_date, system_date, password_hash, compression_type, header_string_tail ) <= -1 )
#else
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" ),
		header_string_head, case_number, evidence_number, description, examiner_name,
		notes, acquiry_date, system_date, password_hash, compression_type, header_string_tail ) <= -1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the header string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}

/* Generate a header format type 2 (EnCase2, EnCase3, FTK Imager 2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_type2( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, int8_t compression_level, libewf_char_t *header_string_head, libewf_char_t *header_string_tail, size_t *string_length )
{
	libewf_char_t *header_string            = NULL;
	libewf_char_t *case_number              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *description              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *examiner_name            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *evidence_number          = _S_LIBEWF_CHAR( "" );
	libewf_char_t *notes                    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *system_date              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_date             = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	libewf_char_t *password_hash            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *compression_type         = _S_LIBEWF_CHAR( "" );
	static char *function                   = "libewf_header_values_generate_header_string_type2";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string head.\n",
		 function );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string tail.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		LIBEWF_WARNING_PRINT( "%s: compression level not supported.\n",
		 function );

		return( NULL );
	}
	*string_length = libewf_string_length( header_string_head );

	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] );
		case_number     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] );
		description     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] );
		examiner_name   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] );
		evidence_number = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	{
		*string_length += libewf_string_length(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] );
		notes           =  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] );
		acquiry_date    = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		acquiry_date = libewf_generate_date_header_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( acquiry_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] );
		system_date     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		system_date = libewf_generate_date_header_value( timestamp );

		if( system_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( system_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] );
		acquiry_operating_system = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] );
		acquiry_software_version = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] );
		password_hash   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*string_length += 1;
		password_hash   = _S_LIBEWF_CHAR( "0" );
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] != NULL )
	{
		*string_length  += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ] );
		compression_type = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_COMPRESSION_TYPE ];
	}
	else
	{
		if( compression_level == EWF_COMPRESSION_NONE )
		{
			compression_type = (libewf_char_t *) LIBEWF_COMPRESSION_TYPE_NONE;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = (libewf_char_t *) LIBEWF_COMPRESSION_TYPE_FAST;
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = (libewf_char_t *) LIBEWF_COMPRESSION_TYPE_BEST;
		}
		*string_length += libewf_string_length( compression_type );
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*string_length += 11;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" ),
		header_string_head, case_number, evidence_number, description, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, compression_type, header_string_tail ) <= -1 )
#else
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" ),
		header_string_head, case_number, evidence_number, description, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, compression_type, header_string_tail ) <= -1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the header string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}

/* Generate a header format type 3 (EnCase4, EnCase5)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_type3( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, libewf_char_t *header_string_head, libewf_char_t *header_string_tail, size_t *string_length )
{
	libewf_char_t *header_string            = NULL;
	libewf_char_t *case_number              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *description              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *examiner_name            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *evidence_number          = _S_LIBEWF_CHAR( "" );
	libewf_char_t *notes                    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *system_date              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_date             = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	libewf_char_t *password_hash            = _S_LIBEWF_CHAR( "" );
	static char *function                   = "libewf_header_values_generate_header_string_type3";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string head.\n",
		 function );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string tail.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	*string_length = libewf_string_length( header_string_head );

	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] );
		case_number     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] );
		description     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] );
		examiner_name   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] );
		evidence_number = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	{
		*string_length += libewf_string_length(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] );
		notes           =  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] );
		acquiry_date    = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		acquiry_date = libewf_generate_date_header_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( acquiry_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] );
		system_date     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		system_date = libewf_generate_date_header_value( timestamp );

		if( system_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( system_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] );
		acquiry_operating_system = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] );
		acquiry_software_version = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] );
		password_hash   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	else
	{
		*string_length += 1;
		password_hash   = _S_LIBEWF_CHAR( "0" );
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*string_length += 10;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" ),
		header_string_head, case_number, evidence_number, description, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, header_string_tail ) <= -1 )
#else
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" ),
		header_string_head, case_number, evidence_number, description, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, header_string_tail ) <= -1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}

/* Generate a header format type 4 (EnCase4 header2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_type4( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, libewf_char_t *header_string_head, libewf_char_t *header_string_tail, size_t *string_length )
{
	libewf_char_t *header_string            = NULL;
	libewf_char_t *case_number              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *description              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *examiner_name            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *evidence_number          = _S_LIBEWF_CHAR( "" );
	libewf_char_t *notes                    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *system_date              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_date             = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	libewf_char_t *password_hash            = _S_LIBEWF_CHAR( "" );
	static char *function                   = "libewf_header_values_generate_header_string_type4";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string head.\n",
		 function );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string tail.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	*string_length = libewf_string_length( header_string_head );

	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] );
		case_number     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] );
		description     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] );
		examiner_name   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] );
		evidence_number = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	{
		*string_length += libewf_string_length(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] );
		notes           =  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] );
		acquiry_date    = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		acquiry_date = libewf_generate_date_header2_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( acquiry_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] );
		system_date     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		system_date = libewf_generate_date_header2_value( timestamp );

		if( system_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( system_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] );
		acquiry_operating_system = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] );
		acquiry_software_version = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] );
		password_hash   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*string_length += 10;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, header_string_tail ) <= -1 )
#else
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, header_string_tail ) <= -1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}

/* Generate a header format type 5 (EnCase5 header2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_type5( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, libewf_char_t *header_string_head, libewf_char_t *header_string_tail, size_t *string_length )
{
	libewf_char_t *header_string            = NULL;
	libewf_char_t *header_string_srce       = _S_LIBEWF_CHAR( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libewf_char_t *header_string_sub        = _S_LIBEWF_CHAR( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libewf_char_t *case_number              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *description              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *examiner_name            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *evidence_number          = _S_LIBEWF_CHAR( "" );
	libewf_char_t *notes                    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *system_date              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_date             = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	libewf_char_t *password_hash            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *unknown_dc               = _S_LIBEWF_CHAR( "" );
	static char *function                   = "libewf_header_values_generate_header_string_type5";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string head.\n",
		 function );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string tail.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	*string_length  = libewf_string_length( header_string_head );
	*string_length += libewf_string_length( header_string_srce );
	*string_length += libewf_string_length( header_string_sub );

	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] );
		case_number     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] );
		description     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] );
		examiner_name   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] );
		evidence_number = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	{
		*string_length += libewf_string_length(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] );
		notes           =  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] );
		acquiry_date    = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		acquiry_date = libewf_generate_date_header2_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( acquiry_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] );
		system_date     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		system_date = libewf_generate_date_header2_value( timestamp );

		if( system_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( system_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] );
		acquiry_operating_system = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] );
		acquiry_software_version = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] );
		password_hash   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] );
		unknown_dc      = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*string_length += 11;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls%ls%ls" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, unknown_dc, header_string_tail,
		header_string_srce, header_string_sub ) <= -1 )
#else
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, unknown_dc, header_string_tail,
		header_string_srce, header_string_sub ) <= -1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}

/* Generate a header format type 6 (EnCase6 header2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_type6( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, libewf_char_t *header_string_head, libewf_char_t *header_string_tail, size_t *string_length )
{
	libewf_char_t *header_string            = NULL;
	libewf_char_t *header_string_srce       = _S_LIBEWF_CHAR( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libewf_char_t *header_string_sub        = _S_LIBEWF_CHAR( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libewf_char_t *case_number              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *description              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *examiner_name            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *evidence_number          = _S_LIBEWF_CHAR( "" );
	libewf_char_t *notes                    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *system_date              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_date             = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	libewf_char_t *password_hash            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *model                    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *serial_number            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *unknown_dc               = _S_LIBEWF_CHAR( "" );
	static char *function                   = "libewf_header_values_generate_header_string_type6";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string head.\n",
		 function );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string tail.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	*string_length  = libewf_string_length( header_string_head );
	*string_length += libewf_string_length( header_string_srce );
	*string_length += libewf_string_length( header_string_sub );

	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] );
		case_number     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] );
		description     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] );
		examiner_name   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] );
		evidence_number = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	{
		*string_length += libewf_string_length(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] );
		notes           =  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] );
		acquiry_date    = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		acquiry_date = libewf_generate_date_header2_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( acquiry_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] );
		system_date     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		system_date = libewf_generate_date_header2_value( timestamp );

		if( system_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( system_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] );
		acquiry_operating_system = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] );
		acquiry_software_version = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] );
		password_hash   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_MODEL ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_MODEL ] );
		model           = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_MODEL ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ] );
		serial_number   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SERIAL_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ] );
		unknown_dc      = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_UNKNOWN_DC ];
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 12x \t and 1x \0
	 */
	*string_length += 13;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls%ls%ls" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, model, serial_number, acquiry_software_version, acquiry_operating_system,
		acquiry_date, system_date, password_hash, unknown_dc, header_string_tail,
		header_string_srce, header_string_sub ) <= -1 )
#else
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, model, serial_number, acquiry_software_version, acquiry_operating_system,
		acquiry_date, system_date, password_hash, unknown_dc, header_string_tail,
		header_string_srce, header_string_sub ) <= -1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}


/* Generate a header format type 7 (EnCase5 linen header)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_type7( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, libewf_char_t *header_string_head, libewf_char_t *header_string_tail, size_t *string_length )
{
	libewf_char_t *header_string            = NULL;
	libewf_char_t *header_string_srce       = _S_LIBEWF_CHAR( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	libewf_char_t *header_string_sub        = _S_LIBEWF_CHAR( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	libewf_char_t *case_number              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *description              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *examiner_name            = _S_LIBEWF_CHAR( "" );
	libewf_char_t *evidence_number          = _S_LIBEWF_CHAR( "" );
	libewf_char_t *notes                    = _S_LIBEWF_CHAR( "" );
	libewf_char_t *system_date              = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_date             = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	libewf_char_t *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	libewf_char_t *password_hash            = _S_LIBEWF_CHAR( "" );
	static char *function                   = "libewf_header_values_generate_header_string_type7";

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string head.\n",
		 function );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string tail.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	*string_length  = libewf_string_length( header_string_head );
	*string_length += libewf_string_length( header_string_srce );
	*string_length += libewf_string_length( header_string_sub );

	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ] );
		case_number     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_CASE_NUMBER ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ] );
		description     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_DESCRIPTION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ] );
		examiner_name   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EXAMINER_NAME ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ] );
		evidence_number = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_EVIDENCE_NUMBER ];
	}
	if(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] != NULL )
	{
		*string_length += libewf_string_length(  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ] );
		notes           =  header_values->values[ LIBEWF_HEADER_VALUES_INDEX_NOTES ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] );
		acquiry_date    = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ];
	}
	else
	{
		acquiry_date = libewf_generate_date_header2_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( acquiry_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] );
		system_date     = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ];
	}
	else
	{
		system_date = libewf_generate_date_header2_value( timestamp );

		if( system_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate system date header value.\n",
			 function );

			system_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			*string_length += libewf_string_length( system_date );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ] );
		acquiry_operating_system = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_OPERATING_SYSTEM ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] != NULL )
	{
		*string_length          += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ] );
		acquiry_software_version = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_SOFTWARE_VERSION ];
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] != NULL )
	{
		*string_length += libewf_string_length( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ] );
		password_hash   = header_values->values[ LIBEWF_HEADER_VALUES_INDEX_PASSWORD ];
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 9x \t and 1x \0
	 */
	*string_length += 11;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE )
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%ls%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls\t%ls%ls%ls%ls" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, header_string_tail,
		header_string_srce, header_string_sub ) <= -1 )
#else
	if( libewf_string_snprintf( header_string, *string_length,
		_S_LIBEWF_CHAR( "%s%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s%s%s%s" ),
		header_string_head, description, case_number, evidence_number, examiner_name,
		notes, acquiry_software_version, acquiry_operating_system, acquiry_date,
		system_date, password_hash, header_string_tail,
		header_string_srce, header_string_sub ) <= -1 )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}

/* Generate an EWF header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_ewf( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	ewf_char_t *header                = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\nmain\nc\tn\ta\te\tt\tm\tu\tp\tr\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header_string_ewf";

	header_string = libewf_header_values_generate_header_string_type1(
	                 header_values,
	                 timestamp,
	                 compression_level,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase1 header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_encase1( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	ewf_char_t *header                = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\r\nmain\r\nc\tn\ta\te\tt\tm\tu\tp\tr\r\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\r\n\r\n" );
	static char *function             = "libewf_header_values_generate_header_string_encase1";

	header_string = libewf_header_values_generate_header_string_type1(
	                 header_values,
	                 timestamp,
	                 compression_level,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an FTK Imager header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_ftk( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	ewf_char_t *header                = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header_string_ftk";

	header_string = libewf_header_values_generate_header_string_type2(
	                 header_values,
	                 timestamp,
	                 compression_level,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase2 and EnCase3 header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_encase2( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	ewf_char_t *header                = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\r\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\r\n\r\n" );
	static char *function             = "libewf_header_values_generate_header_string_encase2";

	header_string = libewf_header_values_generate_header_string_type2(
	                 header_values,
	                 timestamp,
	                 compression_level,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase4 header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_encase4( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header_length )
{
	ewf_char_t *header                = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\r\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\r\n\r\n" );
	static char *function             = "libewf_header_values_generate_header_string_encase4";

	header_string = libewf_header_values_generate_header_string_type3(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase5 linen header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_encase5_linen( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header_length )
{
	ewf_char_t *header                = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header_string_encase5_linen";

	header_string = libewf_header_values_generate_header_string_type7(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase6 linen header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_encase6_linen( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header_length )
{
	ewf_char_t *header                = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header_string_encase6_linen";

	header_string = libewf_header_values_generate_header_string_type6(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase4 header2
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header2_string_encase4( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header2_length )
{
	ewf_char_t *header2               = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header2_string_encase4";

	header_string = libewf_header_values_generate_header_string_type4(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header2_length );

	header2 = libewf_header_values_convert_header_string_to_header2(
	           header_string,
	           *header2_length,
	           header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header2.\n",
		 function );

		return( NULL );
	}
	return( header2 );
}

/* Generate an EnCase5 header2
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header2_string_encase5( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header2_length )
{
	ewf_char_t *header2               = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\tdc\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function               = "libewf_header_values_generate_header2_string_encase5";

	header_string = libewf_header_values_generate_header_string_type5(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header2_length );

	header2 = libewf_header_values_convert_header_string_to_header2(
	           header_string,
	           *header2_length,
	           header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header2.\n",
		 function );

		return( NULL );
	}
	return( header2 );
}

/* Generate an EnCase6 header2
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header2_string_encase6( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header2_length )
{
	ewf_char_t *header2               = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header2_string_encase6";

	header_string = libewf_header_values_generate_header_string_type6(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header2_length );

	header2 = libewf_header_values_convert_header_string_to_header2(
	           header_string,
	           *header2_length,
	           header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header2.\n",
		 function );

		return( NULL );
	}
	return( header2 );
}

/* Convert date string within an xheader value
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_convert_date_xheader_value( libewf_char_t *header_value, size_t header_value_length, uint8_t date_format )
{
	struct tm time_elements;

	libewf_char_t **date_elements = NULL;
	libewf_char_t *date_string    = NULL;
	static char *function         = "libewf_convert_date_xheader_value";
	time_t timestamp              = 0;
	uint32_t date_element_count   = 0;

	if( header_value == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header value.\n",
		 function );

		return( NULL );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_CTIME )
	 && ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH)
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported date format.\n",
		 function );

		return( NULL );
	}
	if( date_format == LIBEWF_DATE_FORMAT_CTIME )
	{
		date_string = libewf_string_duplicate( header_value, header_value_length );
	}
	else
	{
		date_elements = libewf_string_split(
				 header_value,
				 header_value_length,
				 (libewf_char_t) ' ',
				 &date_element_count );

		if( date_elements == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to split date elements in header value.\n",
			 function );

			libewf_common_free( date_string );

			return( NULL );
		}
		if( date_element_count != 6 )
		{
			LIBEWF_WARNING_PRINT( "%s: unsupported amount of date elements in header value.\n",
			 function );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		/* Set the year
		 */
		time_elements.tm_year = (int) ( ( ( date_elements[ 4 ][ 0 ] - (libewf_char_t) '0' ) * 1000 )
				      + ( ( date_elements[ 4 ][ 1 ] - (libewf_char_t) '0' ) * 100 )
				      + ( ( date_elements[ 4 ][ 2 ] - (libewf_char_t) '0' ) * 10 )
				      + ( date_elements[ 4 ][ 3 ] - (libewf_char_t) '0' )
				      - 1900 );

		/* Set the month
		 */
		if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Jan" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 0;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Feb" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 1;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Mar" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 2;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Apr" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 3;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "May" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 4;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Jun" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 5;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Jul" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 6;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Aug" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 7;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Sep" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 8;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Oct" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 9;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Nov" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 10;
		}
		else if( libewf_string_compare( date_elements[ 1 ], _S_LIBEWF_CHAR( "Dec" ), 3 ) == 0 )
		{
			time_elements.tm_mon = 11;
		}
		/* Set the day of the month
		 */
		if( date_elements[ 2 ][ 1 ] == (libewf_char_t) '\0' )
		{
			time_elements.tm_mday = (int) ( date_elements[ 2 ][ 0 ] - (libewf_char_t) '0' );
		}
		else
		{
			time_elements.tm_mday = (int) ( ( ( date_elements[ 2 ][ 0 ] - (libewf_char_t) '0' ) * 10 )
					      + ( date_elements[ 2 ][ 1 ] - (libewf_char_t) '0' ) );
		}
		/* Set the hour
		 */
		time_elements.tm_hour = (int) ( ( ( date_elements[ 3 ][ 0 ] - (libewf_char_t) '0' ) * 10 )
				      + ( date_elements[ 3 ][ 1 ] - (libewf_char_t) '0' ) );

		/* Set the minutes
		 */
		time_elements.tm_min  = (int) ( ( ( date_elements[ 3 ][ 3 ] - (libewf_char_t) '0' ) * 10 )
				      + ( date_elements[ 3 ][ 4 ] - (libewf_char_t) '0' ) );

		/* Set the seconds
		 */
		time_elements.tm_sec = (int) ( ( ( date_elements[ 3 ][ 6 ] - (libewf_char_t) '0' ) * 10 )
				     + ( date_elements[ 3 ][ 7 ] - (libewf_char_t) '0' ) );

		/* Set to ignore the daylight saving time
		 */
		time_elements.tm_isdst = -1;

		/* Create a timestamp
		 */
		timestamp = libewf_common_mktime( &time_elements );

		libewf_string_split_values_free( date_elements, date_element_count );

		if( timestamp == (time_t) -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create timestamp.\n",
			 function );

			return( NULL );
		}
		date_string = libewf_convert_date_timestamp( timestamp, date_format );
	}
	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
		 function );

		return( NULL );
	}
	return( date_string );
}

/* Generate date string within a xheader value
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_generate_date_xheader_value( time_t timestamp )
{
	libewf_char_t *date_string = NULL;
	libewf_char_t *newline     = NULL;
	static char *function      = "libewf_generate_date_xheader_value";

	/* The libewf_string_ctime function returns a string of length 32
	 */
	date_string = libewf_string_ctime( &timestamp );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
		 function );

		return( NULL );
	}
	newline = libewf_string_search( date_string, (libewf_char_t) '\n', 32 );

	if( newline != NULL )
	{
		newline[ 0 ] = (libewf_char_t) ' ';
		newline[ 1 ] = (libewf_char_t) tzname[ 0 ][ 0 ];
		newline[ 2 ] = (libewf_char_t) tzname[ 0 ][ 1 ];
		newline[ 3 ] = (libewf_char_t) tzname[ 0 ][ 2 ];
		newline[ 4 ] = (libewf_char_t) '\0';
	}
	return( date_string );
}

/* Parse a xml header string for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_VALUES_TABLE *libewf_header_values_parse_header_string_xml( libewf_char_t *header_string_xml, size_t length, uint8_t date_format )
{
	LIBEWF_VALUES_TABLE *header_values = NULL;
	libewf_char_t **lines              = NULL;
	libewf_char_t *open_tag_start      = NULL;
	libewf_char_t *open_tag_end        = NULL;
	libewf_char_t *close_tag_start     = NULL;
	libewf_char_t *close_tag_end       = NULL;
	libewf_char_t *date_string         = NULL;
	static char *function              = "libewf_header_values_parse_header_string_xml";
	size_t string_length               = 0;
	uint32_t line_count                = 0;
	uint32_t iterator                  = 0;

	if( header_string_xml == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header string\n",
		 function );

		return( NULL );
	}
	lines = libewf_string_split(
	         (libewf_char_t *) header_string_xml,
	         length,
	         (libewf_char_t) '\n',
	         &line_count );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to split lines in header string.\n",
		 function );

		return( NULL );
	}
	header_values = libewf_values_table_alloc( LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header values.\n",
		 function );

		libewf_string_split_values_free( lines, line_count );

		return( NULL );
	}
	if( libewf_header_values_initialize( header_values ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize the header values.\n",
		 function );

		libewf_string_split_values_free( lines, line_count );

		return( NULL );
	}
	for( iterator = 0; iterator < line_count; iterator++ )
	{
		if( ( lines[ iterator ] == NULL )
		 || ( lines[ iterator ] == (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			continue;
		}
		string_length = libewf_string_length( lines[ iterator ] );

		/* Ignore empty lines
		 */
		if( string_length == 0 )
		{
			continue;
		}
		open_tag_start = libewf_string_search(
		                  lines[ iterator ],
		                  (libewf_char_t) '<',
		                  string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = libewf_string_search(
		                lines[ iterator ],
		                (libewf_char_t) '>',
		                string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_end == NULL )
		{
			continue;
		}
		/* Ignore the first part of the xml string
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
		                   (libewf_char_t) '<',
		                   string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = libewf_string_search_reverse(
		                 &open_tag_end[ 1 ],
		                 (libewf_char_t) '>',
		                 string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_end == NULL )
		{
			continue;
		}
		/* Ignore the second part of the xml string
		 */
		string_length = (size_t) ( close_tag_start - open_tag_end ) - 1;

		/* Make sure the identifier string will be terminated
		 */
		*open_tag_end = (libewf_char_t) '\0';

		if( ( libewf_string_compare( &open_tag_start[ 1 ], _S_LIBEWF_CHAR( "acquiry_date" ), 12 ) == 0 )
		 || ( libewf_string_compare( &open_tag_start[ 1 ], _S_LIBEWF_CHAR( "system_date" ), 11 ) == 0 ) )
		{
			date_string = libewf_convert_date_xheader_value( &open_tag_end[ 1 ], string_length, date_format );

			if( date_string == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to create date string.\n",
				 function );
			}
			else
			{
				if( libewf_values_table_set_value(
				     header_values,
				     &open_tag_start[ 1 ],
				     date_string,
				     libewf_string_length( date_string ) ) != 1 )
				{
					LIBEWF_VERBOSE_PRINT( "%s: unable to set value with identifier: %" PRIs_EWF ".\n",
					 function, &open_tag_start[ 1 ] );
				}
				libewf_common_free( date_string );
			}
		}
		else
		{
			if( libewf_values_table_set_value(
			     header_values,
			     &open_tag_start[ 1 ],
			     &open_tag_end[ 1 ],
			     string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "%s: unable to set value with identifier: %" PRIs_EWF ".\n",
				 function, &open_tag_start[ 1 ] );
			}
		}
	}
	libewf_string_split_values_free( lines, line_count );

	return( header_values );
}

/* Parse an EWF xheader for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_VALUES_TABLE *libewf_header_values_parse_xheader( ewf_char_t *xheader, size_t size, uint8_t date_format )
{
	libewf_values_table_t *header_values = NULL;
	libewf_char_t *xml_header_string     = NULL;
	static char *function                = "libewf_header_values_parse_xheader";

	if( xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid xheader.\n",
		 function );

		return( NULL );
	}
	xml_header_string = (libewf_char_t *) libewf_common_alloc(
	                                       sizeof( libewf_char_t ) * ( size + 1 ) );

	if( xml_header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create XML header string.\n",
		 function );

		return( NULL );
	}
	if( libewf_string_copy_from_header( xml_header_string, size, xheader, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy xheader to xml header string.\n",
		 function );

		libewf_common_free( xml_header_string );

		return( NULL );
	}
	header_values = libewf_header_values_parse_header_string_xml( xml_header_string, size, date_format );

	libewf_common_free( xml_header_string );

	return( header_values );
}

/* Generate a header format in XML
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
libewf_char_t *libewf_header_values_generate_header_string_xml( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *string_length )
{
	libewf_char_t *header_string         = NULL;
	libewf_char_t *xml_head              = _S_LIBEWF_CHAR( "<?xml version=\"1.0\"?>" );
	libewf_char_t *xml_open_tag_xheader  = _S_LIBEWF_CHAR( "<xheader>" );
	libewf_char_t *xml_close_tag_xheader = _S_LIBEWF_CHAR( "</xheader>" );
	libewf_char_t *acquiry_date          = _S_LIBEWF_CHAR( "" );
	static char *function                = "libewf_header_values_generate_header_string_xml";
	uint32_t iterator                    = 0;
	int string_offset                    = 0;
	int character_count                  = 0;

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header values.\n",
		 function );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid string length.\n",
		 function );

		return( NULL );
	}
	/* Add space for the xml data and an end of line
	 */
	*string_length  = libewf_string_length( xml_head ) + 1;
	*string_length += libewf_string_length( xml_open_tag_xheader ) + 1;
	*string_length += libewf_string_length( xml_close_tag_xheader ) + 1;

	for( iterator = 0; iterator < header_values->amount; iterator++ )
	{
		if( header_values->identifiers[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid header value - missing identifier.\n",
			 function );

			continue;
		}
		if( header_values->values[ iterator ] != NULL )
		{
			/* Add space for a leading tab, <identifier></identifier> and an end of line
			 */
			*string_length += ( 2 * libewf_string_length( header_values->identifiers[ iterator ] ) ) + 7;

			/* Add space for the header value
			 */
			*string_length += libewf_string_length( header_values->values[ iterator ] );
		}
	}
	if( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	{
		acquiry_date = libewf_generate_date_xheader_value( timestamp );

		if( acquiry_date == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to generate acquiry date header value.\n",
			 function );

			acquiry_date = _S_LIBEWF_CHAR( "" );
		}
		else
		{
			/* Add space for a leading tab, <acquiry_date>, header value, </acquiry_date> and an end of line
			 */
			*string_length += libewf_string_length( acquiry_date ) + 31;
		}
	}
	/* allow for an empty line and an end of string
	 */
	*string_length += 2;

	header_string = (libewf_char_t *) libewf_common_alloc(
	                                   sizeof( libewf_char_t ) * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header string.\n",
		 function );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		*string_length = 0;

		return( NULL );
	}
	character_count = libewf_string_snprintf( header_string, *string_length,
						  _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n%" )
						  _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n" ), xml_head, xml_open_tag_xheader );

	if( character_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		*string_length = 0;

		return( NULL );
	}
	string_offset = character_count;

	for( iterator = 0; iterator < header_values->amount; iterator++ )
	{
		if( header_values->identifiers[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid header value - missing identifier.\n",
			 function );

			continue;
		}
		if( header_values->values[ iterator ] != NULL )
		{
			character_count = libewf_string_snprintf( &header_string[ string_offset ], ( *string_length - string_offset ),
			                                          _S_LIBEWF_CHAR( "\t<%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( ">%" )
								  _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "</%" ) _S_LIBEWF_CHAR( PRIs_EWF )
								  _S_LIBEWF_CHAR( ">\n" ), header_values->identifiers[ iterator ],
			                                          header_values->values[ iterator ], header_values->identifiers[ iterator ] );

			if( character_count <= -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
				 function );

				libewf_common_free( header_string );

				if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
				 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
				{
					libewf_common_free( acquiry_date );
				}
				*string_length = 0;

				return( NULL );
			}
			string_offset += character_count;
		}
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
	{
		character_count = libewf_string_snprintf( &header_string[ string_offset ], ( *string_length - string_offset ),
							  _S_LIBEWF_CHAR( "\t<acquiry_date>%" ) _S_LIBEWF_CHAR( PRIs_EWF )
							  _S_LIBEWF_CHAR( "</acquiry_date>\n" ), acquiry_date );

		libewf_common_free( acquiry_date );

		if( character_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
			 function );

			libewf_common_free( header_string );

			*string_length = 0;

			return( NULL );
		}
		string_offset += character_count;
	}
	character_count = libewf_string_snprintf( &header_string[ string_offset ], ( *string_length - string_offset ),
	                                          _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n\n" ),
						  xml_close_tag_xheader );

	if( character_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set header string.\n",
		 function );

		libewf_common_free( header_string );

		*string_length = 0;

		return( NULL );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (libewf_char_t) '\0';

	return( header_string );
}

/* Generate an EWFX header
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header_string_ewfx( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header_length )
{
	ewf_char_t  *header               = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header_string_ewfx";

	header_string = libewf_header_values_generate_header_string_type3(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header_length );

	header = libewf_header_values_convert_header_string_to_header(
	          header_string,
	          *header_length,
	          header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header.\n",
		 function );

		return( NULL );
	}
	return( header );
}

/* Generate an EWFX header2
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_header2_string_ewfx( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header2_length )
{
	ewf_char_t  *header2              = NULL;
	libewf_char_t *header_string      = NULL;
	libewf_char_t *header_string_head = _S_LIBEWF_CHAR( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	libewf_char_t *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );
	static char *function             = "libewf_header_values_generate_header2_string_ewfx";

	header_string = libewf_header_values_generate_header_string_type4(
	                 header_values,
	                 timestamp,
	                 header_string_head,
	                 header_string_tail,
	                 header2_length );

	header2 = libewf_header_values_convert_header_string_to_header2(
	           header_string,
	           *header2_length,
	           header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create header2.\n",
		 function );

		return( NULL );
	}
	return( header2 );
}

/* Generate an EWFX xheader
 * Returns a pointer to the new instance, NULL on error
 */
ewf_char_t *libewf_header_values_generate_xheader_string_ewfx( LIBEWF_VALUES_TABLE *header_values, time_t timestamp, size_t *header_length )
{
	ewf_char_t  *xheader         = NULL;
	libewf_char_t *header_string = NULL;
	static char *function        = "libewf_header_values_generate_xheader_string_ewfx";

	header_string = libewf_header_values_generate_header_string_xml(
	                 header_values,
	                 timestamp,
	                 header_length );

	xheader = libewf_header_values_convert_header_string_to_header(
	           header_string,
	           *header_length,
	           header_length );

	if( xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create xheader.\n",
		 function );

		return( NULL );
	}
	return( xheader );
}


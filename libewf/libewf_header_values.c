/*
 * libewf header values
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
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
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
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

#include "ewf_compress.h"
#include "ewf_definitions.h"

/* Allocates memory for a new header values struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HEADER_VALUES *libewf_header_values_alloc( void )
{
	LIBEWF_HEADER_VALUES *header_values = NULL;
	size_t header_values_size           = 0;

	header_values = (LIBEWF_HEADER_VALUES *) libewf_common_alloc_cleared( LIBEWF_HEADER_VALUES_SIZE, 0 );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_alloc: unable to allocate header values.\n" );

		return( NULL );
	}
	header_values->amount = LIBEWF_HEADER_VALUES_DEFAULT_AMOUNT;
	header_values_size    = header_values->amount * sizeof( LIBEWF_CHAR* );

	if( header_values_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_alloc: invalid size value exceeds maximum.\n" );

		libewf_common_free( header_values );

		return( NULL );
	}
	header_values->identifiers = (LIBEWF_CHAR **) libewf_common_alloc_cleared( header_values_size, 0 );

	if( header_values->identifiers == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_alloc: unable to allocate identifiers.\n" );

		libewf_common_free( header_values );

		return( NULL );
	}
	header_values->values = (LIBEWF_CHAR **) libewf_common_alloc_cleared( header_values_size, 0 );

	if( header_values->values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_alloc: unable to allocate values.\n" );

		libewf_common_free( header_values->identifiers );
		libewf_common_free( header_values );

		return( NULL );
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

	return( header_values );
}

/* Reallocates memory for the header values
 * Returns a pointer to the instance, NULL on error
 */
LIBEWF_HEADER_VALUES *libewf_header_values_realloc( LIBEWF_HEADER_VALUES *header_values, uint32_t previous_amount, uint32_t new_amount )
{
	LIBEWF_CHAR **reallocation = NULL;
	size_t previous_size       = previous_amount * sizeof( LIBEWF_CHAR* );
	size_t new_size            = new_amount * sizeof( LIBEWF_CHAR* );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_realloc: invalid header values.\n" );

		return( NULL );
	}
	if( ( previous_amount > (uint32_t) INT32_MAX ) || ( new_amount > (uint32_t) INT32_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_realloc: invalid amount only values below 2^32 are supported.\n" );

		return( NULL );
	}
	if( previous_amount >= new_amount )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_realloc: new amount smaller than previous amount.\n" );

		return( NULL );
	}
	if( ( previous_size > (size_t) SSIZE_MAX ) || ( new_size > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_realloc: invalid size value exceeds maximum.\n" );

		return( NULL );
	}
	reallocation = (LIBEWF_CHAR **) libewf_common_realloc_new_cleared( header_values->identifiers, previous_size, new_size, 0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_realloc: unable to reallocate identifiers.\n" );

		return( NULL );
	}
	header_values->identifiers = reallocation;
	reallocation               = (LIBEWF_CHAR **) libewf_common_realloc_new_cleared( header_values->values, previous_size, new_size, 0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_realloc: unable to reallocate values.\n" );

		return( NULL );
	}
	header_values->values = reallocation;
	header_values->amount = new_amount;

	return( header_values );
}

/* Frees memory of a header values struct including elements
 */
void libewf_header_values_free( LIBEWF_HEADER_VALUES *header_values )
{
	uint32_t iterator = 0;

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_free: invalid header values.\n" );

		return;
	}
	if( header_values->values != NULL )
	{
		for( iterator = 0; iterator < header_values->amount; iterator++ )
		{
			libewf_common_free( header_values->identifiers[ iterator ] );
			libewf_common_free( header_values->values[ iterator ] );
		}
		libewf_common_free( header_values->values );
	}
	libewf_common_free( header_values );
}

/* Sets the year value in the date element at the start of the date string
 * Returns 1 if successful, -1 otherwise
 */
int8_t libewf_date_string_set_year( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements )
{
	size_t string_length = 0;

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_year: invalid date string.\n" );

		return( -1 );
	}
	if( date_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_year: invalid date elements.\n" );

		return( -1 );
	}
	string_length = libewf_string_length( date_elements[ 0 ] );

	if( string_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_year: empty year string in date elements.\n" );

		return( -1 );
	}
	if( string_length < 4 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_year: invalid year string in date elements.\n" );

		return( -1 );
	}
	date_string[ 0 ] = date_elements[ 0 ][ 0 ];
	date_string[ 1 ] = date_elements[ 0 ][ 1 ];
	date_string[ 2 ] = date_elements[ 0 ][ 2 ];
	date_string[ 3 ] = date_elements[ 0 ][ 3 ];

	return( 1 );
}

/* Sets a 2 digit value for a certain index in the date element at the start of the date string
 * Returns 1 if successful, -1 otherwise
 */
int8_t libewf_date_string_set_2digit_value( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements, uint8_t index )
{
	size_t string_length = 0;

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_2digit_value: invalid date string.\n" );

		return( -1 );
	}
	if( date_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_2digit_value: invalid date elements.\n" );

		return( -1 );
	}
	string_length = libewf_string_length( date_elements[ index ] );

	if( string_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_2digit_value: empty string at index: %" PRIu8 " in date elements.\n", index );

		return( -1 );
	}
	else if( string_length == 1 )
	{
		date_string[ 0 ] = (LIBEWF_CHAR) '0';
		date_string[ 1 ] = (LIBEWF_CHAR) date_elements[ index ][ 0 ];
	}
	else
	{
		date_string[ 0 ] = (LIBEWF_CHAR) date_elements[ index ][ 0 ];
		date_string[ 1 ] = (LIBEWF_CHAR) date_elements[ index ][ 1 ];
	}
	return( 1 );
}

/* Sets the month value in the date element at the start of the date string
 * Returns 1 if successful, -1 otherwise
 */
int8_t libewf_date_string_set_month( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements )
{
	if( libewf_date_string_set_2digit_value( date_string, date_elements, 1 ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_month: unable to set month.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Sets the day of the month value in the date element at the start of the date string
 * Returns 1 if successful, -1 otherwise
 */
int8_t libewf_date_string_set_day_of_month( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements )
{
	if( libewf_date_string_set_2digit_value( date_string, date_elements, 2 ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_month: unable to set day of the month.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Sets the hours value in the date element at the start of the date string
 * Returns 1 if successful, -1 otherwise
 */
int8_t libewf_date_string_set_hours( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements )
{
	if( libewf_date_string_set_2digit_value( date_string, date_elements, 3 ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_month: unable to set hours.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Sets the minutes value in the date element at the start of the date string
 * Returns 1 if successful, -1 otherwise
 */
int8_t libewf_date_string_set_minutes( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements )
{
	if( libewf_date_string_set_2digit_value( date_string, date_elements, 4 ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_month: unable to set minutes.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Sets the seconds value in the date element at the start of the date string
 * Returns 1 if successful, -1 otherwise
 */
int8_t libewf_date_string_set_seconds( LIBEWF_CHAR *date_string, LIBEWF_CHAR **date_elements )
{
	if( libewf_date_string_set_2digit_value( date_string, date_elements, 5 ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_date_string_set_month: unable to set seconds.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Convert date string within a header value
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_convert_date_header_value( LIBEWF_CHAR *header_value, uint8_t date_format )
{
	LIBEWF_CHAR **date_elements  = NULL;
	LIBEWF_CHAR *date_string     = NULL;
	uint32_t date_element_count  = 0;
	uint8_t date_string_iterator = 0;

	if( header_value == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: invalid header value.\n" );

		return( NULL );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unsupported date format.\n" );

		return( NULL );
	}
	date_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * 20 );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to create date string.\n" );

		return( NULL );
	}
	date_elements = libewf_string_split( header_value, libewf_string_length( header_value ), (LIBEWF_CHAR) ' ', &date_element_count );

	if( date_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to split date elements in header value.\n" );

		libewf_common_free( date_string );

		return( NULL );
	}
	if( date_element_count != 6 )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unsupported amount of date elements in header value.\n" );

		libewf_common_free( date_string );
		libewf_string_split_values_free( date_elements, date_element_count );

		return( NULL );
	}
	/* Set first value
	 */
	if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
	{
		if( libewf_date_string_set_month( &date_string[ date_string_iterator ], date_elements ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set month value.\n" );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		date_string_iterator += 2;
	}
	else if( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	{
		if( libewf_date_string_set_day_of_month( &date_string[ date_string_iterator ], date_elements ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set day of the month value.\n" );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		date_string_iterator += 2;
	}
	else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		if( libewf_date_string_set_year( &date_string[ date_string_iterator ], date_elements ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set year value.\n" );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		date_string_iterator += 4;
	}
	/* Set first seperator
	 */
	if( ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	 || ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY ) )
	{
		date_string[ date_string_iterator ] = '/';

		date_string_iterator++;
	}
	else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		date_string[ date_string_iterator ] = '-';

		date_string_iterator++;
	}
	/* Set second value
	 */
	if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
	{
		if( libewf_date_string_set_day_of_month( &date_string[ date_string_iterator ], date_elements ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set day of the month value.\n" );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		date_string_iterator += 2;
	}
	else if( ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	 || ( date_format == LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		if( libewf_date_string_set_month( &date_string[ date_string_iterator ], date_elements ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set month value.\n" );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		date_string_iterator += 2;
	}
	/* Set second seperator
	 */
	if( ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	 || ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY ) )
	{
		date_string[ date_string_iterator ] = '/';

		date_string_iterator++;
	}
	else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		date_string[ date_string_iterator ] = '-';

		date_string_iterator++;
	}
	/* Set third value
	 */
	if( ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	 || ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY ) )
	{
		if( libewf_date_string_set_year( &date_string[ date_string_iterator ], date_elements ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set year value.\n" );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		date_string_iterator += 4;
	}
	else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		if( libewf_date_string_set_day_of_month( &date_string[ date_string_iterator ], date_elements ) == -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set day of the month value.\n" );

			libewf_common_free( date_string );
			libewf_string_split_values_free( date_elements, date_element_count );

			return( NULL );
		}
		date_string_iterator += 2;
	}
	/* Set third seperator
	 */
	if( ( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	 || ( date_format == LIBEWF_DATE_FORMAT_MONTHDAY ) )
	{
		date_string[ date_string_iterator ] = ' ';

		date_string_iterator++;
	}
	else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		date_string[ date_string_iterator ] = 'T';

		date_string_iterator++;
	}
	/* Set fourth value
	 */
	if( libewf_date_string_set_hours( &date_string[ date_string_iterator ], date_elements ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set hours value.\n" );

		libewf_common_free( date_string );
		libewf_string_split_values_free( date_elements, date_element_count );

		return( NULL );
	}
	date_string_iterator += 2;

	/* Set fourth seperator
	 */
	date_string[ date_string_iterator ] = ':';

	date_string_iterator++;

	/* Set fifth value
	 */
	if( libewf_date_string_set_minutes( &date_string[ date_string_iterator ], date_elements ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set minutes value.\n" );

		libewf_common_free( date_string );
		libewf_string_split_values_free( date_elements, date_element_count );

		return( NULL );
	}
	date_string_iterator += 2;

	/* Set fifth seperator
	 */
	date_string[ date_string_iterator ] = ':';

	date_string_iterator++;

	/* Set sixth value
	 */
	if( libewf_date_string_set_seconds( &date_string[ date_string_iterator ], date_elements ) == -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header_value: unable to set seconds value.\n" );

		libewf_common_free( date_string );
		libewf_string_split_values_free( date_elements, date_element_count );

		return( NULL );
	}
	date_string_iterator += 2;

	date_string[ date_string_iterator ] = (LIBEWF_CHAR) '\0';

	libewf_string_split_values_free( date_elements, date_element_count );

	return( date_string );
}

/* Generate date string within a header value
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_generate_date_header_value( time_t timestamp )
{
	LIBEWF_CHAR *date_string = NULL;
	struct tm *time_elements = NULL;

	time_elements = libewf_common_localtime( &timestamp );

	if( time_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_header_value: unable to create time elements.\n" );

		return( NULL );
	}
	date_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * 20 );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_header_value: unable to create date string.\n" );

		libewf_common_free( time_elements );

		return( NULL );
	}
	if( libewf_string_snprintf( date_string, 20, _S_LIBEWF_CHAR( "%4d %d %d %d %d %d" ),
		( time_elements->tm_year + 1900 ), ( time_elements->tm_mon + 1 ), time_elements->tm_mday,
		time_elements->tm_hour, time_elements->tm_min, time_elements->tm_sec ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_header_value: unable to fill date string.\n" );

		libewf_common_free( date_string );
		libewf_common_free( time_elements );

		return( NULL );
	}
	/* Make sure the string is terminated correctly
	 */
	date_string[ 19 ] = (LIBEWF_CHAR) '\0';

	libewf_common_free( time_elements );

	return( date_string );
}

/* Convert date string within a header2 value
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_convert_date_header2_value( LIBEWF_CHAR *header_value, uint8_t date_format )
{
	struct tm *time_elements = NULL;
	LIBEWF_CHAR *date_string = NULL;
	time_t timestamp         = 0;

	if( header_value == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header2_value: invalid header value.\n" );

		return( NULL );
	}
	if( ( date_format != LIBEWF_DATE_FORMAT_DAYMONTH )
	 && ( date_format != LIBEWF_DATE_FORMAT_MONTHDAY )
	 && ( date_format != LIBEWF_DATE_FORMAT_ISO8601 ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header2_value: unsupported date format.\n" );

		return( NULL );
	}
	timestamp     = libewf_string_to_int64( header_value, libewf_string_length( header_value ) );
	time_elements = libewf_common_localtime( &timestamp );

	if( time_elements == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header2_value: unable to create time elements.\n" );

		return( NULL );
	}
	date_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * 20 );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_convert_date_header2_value: unable to create date string.\n" );

		libewf_common_free( time_elements );

		return( NULL );
	}
	if( date_format == LIBEWF_DATE_FORMAT_MONTHDAY )
	{
		if( libewf_string_snprintf( date_string, 20, _S_LIBEWF_CHAR( "%02d/%02d/%04d %02d:%02d:%02d" ),
	                ( time_elements->tm_mon + 1 ), time_elements->tm_mday, ( time_elements->tm_year + 1900 ),
	                time_elements->tm_hour, time_elements->tm_min, time_elements->tm_sec ) <= -1 )
        	{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header2_value: unable to fill date string.\n" );

			libewf_common_free( time_elements );
			libewf_common_free( date_string );

			return( NULL );
		}
	}
	else if( date_format == LIBEWF_DATE_FORMAT_DAYMONTH )
	{
		if( libewf_string_snprintf( date_string, 20, _S_LIBEWF_CHAR( "%02d/%02d/%04d %02d:%02d:%02d" ),
	                time_elements->tm_mday, ( time_elements->tm_mon + 1 ), ( time_elements->tm_year + 1900 ),
	                time_elements->tm_hour, time_elements->tm_min, time_elements->tm_sec ) <= -1 )
        	{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header2_value: unable to fill date string.\n" );

			libewf_common_free( time_elements );
			libewf_common_free( date_string );

			return( NULL );
		}
	}
	else if( date_format == LIBEWF_DATE_FORMAT_ISO8601 )
	{
		if( libewf_string_snprintf( date_string, 20, _S_LIBEWF_CHAR( "%04d-%02d-%02dT%02d:%02d:%02d" ),
	                ( time_elements->tm_year + 1900 ), ( time_elements->tm_mon + 1 ), time_elements->tm_mday,
	                time_elements->tm_hour, time_elements->tm_min, time_elements->tm_sec ) <= -1 )
        	{
			LIBEWF_WARNING_PRINT( "libewf_convert_date_header2_value: unable to fill date string.\n" );

			libewf_common_free( time_elements );
			libewf_common_free( date_string );

			return( NULL );
		}
	}
	libewf_common_free( time_elements );

	return( date_string );
}

/* Generate date string within a header2 value
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_generate_date_header2_value( time_t timestamp )
{
	LIBEWF_CHAR *date_string = NULL;

	date_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * 11 );

	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_header2_value: unable to create date string.\n" );

		return( NULL );
	}
	if( libewf_string_snprintf( date_string, 11, _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIu32 ), (uint32_t) timestamp ) <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_header2_value: unable to create date string.\n" );

		libewf_common_free( date_string );

		return( NULL );
	}
	/* Make sure the string is terminated correctly
	 */
	date_string[ 10 ] = (LIBEWF_CHAR) '\0';

	return( date_string );
}

/* Retrieves the header value index number, or -1 on error
 * The index number will be larger than the amount when the identifier is not present in the header values
 */
int32_t libewf_header_values_get_index( LIBEWF_HEADER_VALUES *header_values, LIBEWF_CHAR *identifier )
{
	size_t identifier_length = 0;
	size_t string_length     = 0;
	int32_t iterator         = 0;

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_index: invalid header values.\n" );

		return( -1 );
	}
	if( identifier == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_index: invalid identifier.\n" );

		return( -1 );
	}
	identifier_length = libewf_string_length( identifier );

	if( identifier_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_index: invalid identifier.\n" );

		return( -1 );
	}
	if( header_values->amount > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_index: invalid header values amount only values below 2^32 are supported.\n" );

		return( -1 );
	}
	for( iterator = 0; iterator < (int32_t) header_values->amount; iterator++ )
	{
		if( header_values->identifiers[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_header_values_get_index: missing identifier for index: %" PRIi32 ".\n", iterator );

			continue;
		}
		string_length = libewf_string_length( header_values->identifiers[ iterator ] );

		if( string_length == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_header_values_get_index: unable to determine length of identifier of index: %" PRIi32 ".\n", iterator );

			continue;
		}
		if( string_length != identifier_length )
		{
			continue;
		}
		if( libewf_string_compare( identifier, header_values->identifiers[ iterator ], identifier_length ) == 0 )
		{
			return( iterator );
		}
	}
	return( iterator );
}

/* Retrieves a header value in value
 * Length should contain the amount of characters in the string
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int8_t libewf_header_values_get_value( LIBEWF_HEADER_VALUES *header_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	size_t header_value_length = 0;
	int32_t index              = 0;

	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_value: invalid value.\n" );

		return( -1 );
	}
	index = libewf_header_values_get_index( header_values, identifier );

	if( index <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_value: unable to find index for: %" PRIs_EWF ".\n", identifier );

		return( -1 );
	}
	if( (uint32_t) index > header_values->amount )
	{
		return( 0 );
	}
	if( header_values->values[ index ] == NULL )
	{
		return( 0 );
	}
	header_value_length = libewf_string_length( header_values->values[ index ] );

	/* Don't bother with empty values
	 */
	if( header_value_length == 0 )
	{
		return( 0 );
	}
	/* Add 1 additional byte required for the end of string character
	 */
	header_value_length += 1;

	if( header_value_length > length )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_value: value too small.\n" );

		return( -1 );
	}
	if( libewf_string_copy( value, header_values->values[ index ], header_value_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_get_value: unable to set header value.\n" );

		return( -1 );
	}
	value[ header_value_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( 1 );
}

/* Set a header value
 * Length should contain the amount of characters in the string
 * Frees the previous header value if necessary
 * Returns 1 if successful, -1 on error
 */
int8_t libewf_header_values_set_value( LIBEWF_HEADER_VALUES *header_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	size_t string_length = 0;
	int32_t index        = 0;

	index = libewf_header_values_get_index( header_values, identifier );

	if( index <= -1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_set_value: unable to find index for: %" PRIs_EWF ".\n", identifier );

		return( -1 );
	}
	if( (uint32_t) index >= header_values->amount )
	{
		string_length = libewf_string_length( identifier );

		if( string_length == 0 )
		{
			LIBEWF_WARNING_PRINT( "libewf_header_values_set_value: unable to determine length of identifier.\n" );

			return( -1 );
		}
		if( libewf_header_values_realloc( header_values, header_values->amount, ( index + 1 ) ) == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_header_values_set_value: unable to reallocate header values.\n" );

			return( -1 );
		}
		header_values->identifiers[ index ] = libewf_string_duplicate( identifier, string_length );

		if( header_values->identifiers[ index ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "libewf_header_values_set_value: unable to set identifier.\n" );

			libewf_common_free( header_values->identifiers[ index ] );

			header_values->identifiers[ index ] = NULL;

			return( -1 );
		}
	}
	/* Clear the buffer of the previous header value
	 */
	if( header_values->values[ index ] != NULL )
	{
		libewf_common_free( header_values->values[ index ] );

		header_values->values[ index ] = NULL;
	}
	/* Don't bother with empty values
	 */
	if( value == NULL )
	{
		return( 1 );
	}
	if( length == 0 )
	{
		return( 1 );
	}
	header_values->values[ index ] = libewf_string_duplicate( value, length );

	if( header_values->values[ index ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_set_value: unable to set value.\n" );

		libewf_common_free( header_values->values[ index ] );

		header_values->values[ index ] = NULL;

		return( -1 );
	}
	return( 1 );
}

/* Parse a header string for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HEADER_VALUES *libewf_header_values_parse_header_string( LIBEWF_CHAR *header_string, size_t length, uint8_t date_format )
{
	LIBEWF_HEADER_VALUES *header_values = NULL;
	LIBEWF_CHAR **lines                 = NULL;
	LIBEWF_CHAR **types                 = NULL;
	LIBEWF_CHAR **values                = NULL;
	LIBEWF_CHAR *date_string            = NULL;
	size_t string_length                = 0;
	uint32_t line_count                 = 0;
	uint32_t type_count                 = 0;
	uint32_t value_count                = 0;
	uint32_t iterator                   = 0;

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string: invalid header string\n" );

		return( NULL );
	}
	lines = libewf_string_split( (LIBEWF_CHAR *) header_string, length, (LIBEWF_CHAR) '\n', &line_count );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string: unable to split lines in header string.\n" );

		return( NULL );
	}
	types = libewf_string_split( lines[ 2 ], libewf_string_length( lines[ 2 ] ), (LIBEWF_CHAR) '\t', &type_count );

	if( types == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string: unable to split types in header string.\n" );

		libewf_string_split_values_free( lines, line_count );

		return( NULL );
	}
	values = libewf_string_split( lines[ 3 ], libewf_string_length( lines[ 3 ] ), (LIBEWF_CHAR) '\t', &value_count );

	if( values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string: unable to split values in header string.\n" );

		libewf_string_split_values_free( lines, line_count );
		libewf_string_split_values_free( types, type_count );

		return( NULL );
	}
	libewf_string_split_values_free( lines, line_count );

	header_values = libewf_header_values_alloc();

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string: unable to create header values.\n" );

		libewf_string_split_values_free( types, type_count );
		libewf_string_split_values_free( values, value_count );

		return( NULL );
	}
	for( iterator = 0; iterator < type_count; iterator++ )
	{
		if( ( values[ iterator ] == NULL )
		 || ( values[ iterator ] == (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			continue;
		}
		string_length = libewf_string_length( values[ iterator ] );

		/* Remove trailing white space
		 */
		if( ( string_length > 0 ) && ( values[ iterator ][ string_length - 1 ] == (LIBEWF_CHAR) '\r' ) )
		{
			string_length -= 1;
		}
		if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "av" ), 2 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "acquiry_software_version" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set acquiry software version.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "dc" ), 2 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "unknown_dc" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set unknown: dc.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "md" ), 2 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "model" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set model.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "ov" ), 2 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "acquiry_operating_system" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set acquiry operating system.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "sn" ), 2 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "serial_number" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set serial_number.\n" );
			}
		}
		else if( ( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "m" ), 1 ) == 0 )
		      || ( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "u" ), 1 ) == 0 ) )
		{
			/* If the date string contains spaces it's in the old header
			 * format otherwise is in new header2 format
			 */
			if( libewf_string_search( values[ iterator ] , (LIBEWF_CHAR) ' ', string_length ) != NULL )
			{
				date_string = libewf_convert_date_header_value( values[ iterator ], date_format );
			}
			else
			{
				date_string = libewf_convert_date_header2_value( values[ iterator ], date_format );
			}
			if( date_string == NULL )
			{
				LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string: unable to create date string.\n" );
			}
			else
			{
				/* The string length of the date string is needed
				 */
				string_length = libewf_string_length( date_string );

				if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "m" ), 1 ) == 0 )
				{
					if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "acquiry_date" ), date_string, string_length ) != 1 )
					{
						LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set acquiry date.\n" );
					}
				}
				else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "u" ), 1 ) == 0 )
				{
					if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "system_date" ), date_string, string_length ) != 1 )
					{
						LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set system date.\n" );
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
			else if( ( string_length == 1 ) && ( values[ iterator ][ 0 ] == (LIBEWF_CHAR) '0' ) )
			{
				/* Empty hash do nothing
				 */
			}
			else
			{
				if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "password" ), values[ iterator ], string_length ) != 1 )
				{
					LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set password.\n" );
				}
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "a" ), 1 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "description" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set description.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "c" ), 1 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "case_number" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set case number.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "n" ), 1 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "evidence_number" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set evidence number.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "e" ), 1 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "examiner_name" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set examiner name.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "t" ), 1 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "notes" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set notes.\n" );
			}
		}
		else if( libewf_string_compare( types[ iterator ], _S_LIBEWF_CHAR( "r" ), 1 ) == 0 )
		{
			if( libewf_header_values_set_value( header_values, _S_LIBEWF_CHAR( "compression_type" ), values[ iterator ], string_length ) != 1 )
			{
				LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string: unable to set compression type.\n" );
			}
		}
		else
		{
			LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string: unsupported type: %d with value: %" PRIs_EWF ".\n", types[ iterator ], values[ iterator ] );

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
LIBEWF_HEADER_VALUES *libewf_header_values_parse_header( EWF_HEADER *header, size_t size, uint8_t date_format )
{
	LIBEWF_HEADER_VALUES* header_values = NULL;
	LIBEWF_CHAR *header_string          = NULL;

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header: invalid header.\n" );

		return( NULL );
	}
	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * ( size + 1 ) );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header: unable to create header string.\n" );

		return( NULL );
	}
	if( libewf_string_copy_from_header( header_string, size, header, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header: unable to copy header to header string.\n" );

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
LIBEWF_HEADER_VALUES *libewf_header_values_parse_header2( EWF_HEADER2 *header2, size_t size, uint8_t date_format )
{
	LIBEWF_HEADER_VALUES* header_values = NULL;
	LIBEWF_CHAR *header_string          = NULL;
	size_t header_size                  = 0;

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header2: invalid header2.\n" );

		return( NULL );
	}
	header_size   = ( size - 2 ) / 2;
	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * ( header_size + 1 ) );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header2: unable to create header string.\n" );

		return( NULL );
	}
	if( libewf_string_copy_from_header2( header_string, header_size, header2, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header2: unable to copy header2 to header string.\n" );

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
EWF_HEADER *libewf_header_values_convert_header_string_to_header( LIBEWF_CHAR *header_string, size_t string_length, size_t *header_length )
{
	EWF_HEADER *header = NULL;

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_convert_header_string_to_header: invalid header string.\n" );

		return( NULL );
	}
	if( header_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_convert_header_string_to_header: invalid header length.\n" );

		return( NULL );
	}
	header = (EWF_HEADER *) libewf_common_alloc( EWF_HEADER_SIZE * string_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_convert_header_string_to_header: unable to create header.\n" );

		libewf_common_free( header_string );

		return( NULL );
	}
	*header_length = string_length;

	if( libewf_string_copy_to_header( header_string, string_length, header, *header_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_convert_header_string_to_header: unable to set header.\n" );

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
EWF_HEADER2 *libewf_header_values_convert_header_string_to_header2( LIBEWF_CHAR *header_string, size_t string_length, size_t *header2_length )
{
	EWF_HEADER2 *header2 = NULL;

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_convert_header_string_to_header2: invalid header string.\n" );

		return( NULL );
	}
	if( header2_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_convert_header_string_to_header2: invalid header2 length.\n" );

		return( NULL );
	}
	/* Add a character for the UTF16 endian type
	 */
	*header2_length = ( string_length + 1 ) * 2;

	header2 = (EWF_HEADER2 *) libewf_common_alloc( EWF_HEADER2_SIZE * *header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header2_string_encase4: unable to create header2.\n" );

		libewf_common_free( header_string );

		*header2_length = 0;

		return( NULL );
	}
	if( libewf_string_copy_to_header2( header_string, string_length, header2, *header2_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header2_string_encase4: unable to set header2.\n" );

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
LIBEWF_CHAR *libewf_header_values_generate_header_string_type1( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length )
{
	LIBEWF_CHAR *header_string    = NULL;
	LIBEWF_CHAR *case_number      = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *description      = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *examiner_name    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *evidence_number  = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *notes            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *system_date      = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_date     = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *password_hash    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *compression_type = _S_LIBEWF_CHAR( "" );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: invalid header values.\n" );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: invalid header string head.\n" );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: invalid header string tail.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: invalid string length.\n" );

		return( NULL );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE )
	 && ( compression_level != EWF_COMPRESSION_FAST )
	 && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: compression level not supported.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: unable to generate acquiry date header value.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: unable to generate system date header value.\n" );

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
			compression_type = (LIBEWF_CHAR *) LIBEWF_COMPRESSION_TYPE_NONE;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = (LIBEWF_CHAR *) LIBEWF_COMPRESSION_TYPE_FAST;
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = (LIBEWF_CHAR *) LIBEWF_COMPRESSION_TYPE_BEST;
		}
		*string_length += libewf_string_length( compression_type );
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*string_length += 11;

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type1: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the header string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}

/* Generate a header format type 2 (EnCase2, EnCase3, FTK Imager 2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_header_values_generate_header_string_type2( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length )
{
	LIBEWF_CHAR *header_string            = NULL;
	LIBEWF_CHAR *case_number              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *description              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *examiner_name            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *evidence_number          = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *notes                    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *system_date              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_date             = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *password_hash            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *compression_type         = _S_LIBEWF_CHAR( "" );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: invalid header values.\n" );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: invalid header string head.\n" );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: invalid header string tail.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: invalid string length.\n" );

		return( NULL );
	}
	if( ( compression_level != EWF_COMPRESSION_NONE ) && ( compression_level != EWF_COMPRESSION_FAST ) && ( compression_level != EWF_COMPRESSION_BEST ) )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: compression level not supported.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: unable to generate acquiry date header value.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: unable to generate system date header value.\n" );

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
			compression_type = (LIBEWF_CHAR *) LIBEWF_COMPRESSION_TYPE_NONE;
		}
		else if( compression_level == EWF_COMPRESSION_FAST )
		{
			compression_type = (LIBEWF_CHAR *) LIBEWF_COMPRESSION_TYPE_FAST;
		}
		else if( compression_level == EWF_COMPRESSION_BEST )
		{
			compression_type = (LIBEWF_CHAR *) LIBEWF_COMPRESSION_TYPE_BEST;
		}
		*string_length += libewf_string_length( compression_type );
	}
	*string_length += libewf_string_length( header_string_tail );

	/* allow for 10x \t and 1x \0
	 */
	*string_length += 11;

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type2: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the header string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}

/* Generate a header format type 3 (EnCase4, EnCase5)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_header_values_generate_header_string_type3( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length )
{
	LIBEWF_CHAR *header_string            = NULL;
	LIBEWF_CHAR *case_number              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *description              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *examiner_name            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *evidence_number          = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *notes                    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *system_date              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_date             = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *password_hash            = _S_LIBEWF_CHAR( "" );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: invalid header values.\n" );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: invalid header string head.\n" );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: invalid header string tail.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: invalid string length.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: unable to generate acquiry date header value.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: unable to generate system date header value.\n" );

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

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type3: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}

/* Generate a header format type 4 (EnCase4 header2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_header_values_generate_header_string_type4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length )
{
	LIBEWF_CHAR *header_string            = NULL;
	LIBEWF_CHAR *case_number              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *description              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *examiner_name            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *evidence_number          = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *notes                    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *system_date              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_date             = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *password_hash            = _S_LIBEWF_CHAR( "" );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: invalid header values.\n" );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: invalid header string head.\n" );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: invalid header string tail.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: invalid string length.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: unable to generate acquiry date header value.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: unable to generate system date header value.\n" );

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

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type4: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		*string_length = 0;

		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}

/* Generate a header format type 5 (EnCase5 header2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_header_values_generate_header_string_type5( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length )
{
	LIBEWF_CHAR *header_string            = NULL;
	LIBEWF_CHAR *header_string_srce       = _S_LIBEWF_CHAR( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	LIBEWF_CHAR *header_string_sub        = _S_LIBEWF_CHAR( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	LIBEWF_CHAR *case_number              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *description              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *examiner_name            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *evidence_number          = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *notes                    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *system_date              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_date             = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *password_hash            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *unknown_dc               = _S_LIBEWF_CHAR( "" );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: invalid header values.\n" );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: invalid header string head.\n" );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: invalid header string tail.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: invalid string length.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: unable to generate acquiry date header value.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: unable to generate system date header value.\n" );

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

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type5: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}

/* Generate a header format type 6 (EnCase6 header2)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_header_values_generate_header_string_type6( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length )
{
	LIBEWF_CHAR *header_string            = NULL;
	LIBEWF_CHAR *header_string_srce       = _S_LIBEWF_CHAR( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	LIBEWF_CHAR *header_string_sub        = _S_LIBEWF_CHAR( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	LIBEWF_CHAR *case_number              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *description              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *examiner_name            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *evidence_number          = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *notes                    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *system_date              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_date             = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *password_hash            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *model                    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *serial_number            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *unknown_dc               = _S_LIBEWF_CHAR( "" );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: invalid header values.\n" );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: invalid header string head.\n" );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: invalid header string tail.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: invalid string length.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: unable to generate acquiry date header value.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: unable to generate system date header value.\n" );

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

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type6: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}


/* Generate a header format type 7 (EnCase5 linen header)
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_header_values_generate_header_string_type7( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, LIBEWF_CHAR *header_string_head, LIBEWF_CHAR *header_string_tail, size_t *string_length )
{
	LIBEWF_CHAR *header_string            = NULL;
	LIBEWF_CHAR *header_string_srce       = _S_LIBEWF_CHAR( "srce\n0\t1\np\tn\tid\tev\ttb\tlo\tpo\tah\tgu\taq\n0\t0\n\t\t\t\t\t-1\t-1\t\t\t\n\n" );
	LIBEWF_CHAR *header_string_sub        = _S_LIBEWF_CHAR( "sub\n0\t1\np\tn\tid\tnu\tco\tgu\n0\t0\n\t\t\t\t1\t\n\n" );
	LIBEWF_CHAR *case_number              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *description              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *examiner_name            = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *evidence_number          = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *notes                    = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *system_date              = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_date             = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_operating_system = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *acquiry_software_version = _S_LIBEWF_CHAR( "" );
	LIBEWF_CHAR *password_hash            = _S_LIBEWF_CHAR( "" );

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: invalid header values.\n" );

		return( NULL );
	}
	if( header_string_head == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: invalid header string head.\n" );

		return( NULL );
	}
	if( header_string_tail == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: invalid header string tail.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: invalid string length.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: unable to generate acquiry date header value.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: unable to generate system date header value.\n" );

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

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
#ifdef HAVE_WIDE_CHARACTER_TYPE
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_type7: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( acquiry_date );
		}
		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
		 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
		{
			libewf_common_free( system_date );
		}
		return( NULL );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( acquiry_date );
	}
	if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_SYSTEM_DATE ] == NULL )
	 && ( system_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		libewf_common_free( system_date );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}

/* Generate an EWF header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_ewf( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	EWF_HEADER *header              = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\nmain\nc\tn\ta\te\tt\tm\tu\tp\tr\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type1( header_values, timestamp, compression_level, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_ewf: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase1 header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_encase1( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	EWF_HEADER *header              = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\r\nmain\r\nc\tn\ta\te\tt\tm\tu\tp\tr\r\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\r\n\r\n" );

	header_string = libewf_header_values_generate_header_string_type1( header_values, timestamp, compression_level, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_encase1: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an FTK Imager header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_ftk( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	EWF_HEADER *header              = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type2( header_values, timestamp, compression_level, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_ftk: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase2 and EnCase3 header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_encase2( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, int8_t compression_level, size_t *header_length )
{
	EWF_HEADER *header              = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\tr\r\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\r\n\r\n" );

	header_string = libewf_header_values_generate_header_string_type2( header_values, timestamp, compression_level, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_encase2: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase4 header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_encase4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length )
{
	EWF_HEADER *header              = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\r\nmain\r\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\r\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\r\n\r\n" );

	header_string = libewf_header_values_generate_header_string_type3( header_values, timestamp, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_encase4: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase5 linen header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_encase5_linen( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length )
{
	EWF_HEADER *header              = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type7( header_values, timestamp, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_encase5_linen: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase6 linen header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_encase6_linen( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length )
{
	EWF_HEADER *header              = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type6( header_values, timestamp, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_encase6_linen: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an EnCase4 header2
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER2 *libewf_header_values_generate_header2_string_encase4( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header2_length )
{
	EWF_HEADER2 *header2            = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type4( header_values, timestamp, header_string_head, header_string_tail, header2_length );
	header2       = libewf_header_values_convert_header_string_to_header2( header_string, *header2_length, header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header2_string_encase4: unable to create header2.\n" );

		return( NULL );
	}
	return( header2 );
}

/* Generate an EnCase5 header2
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER2 *libewf_header_values_generate_header2_string_encase5( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header2_length )
{
	EWF_HEADER2 *header2            = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\tdc\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type5( header_values, timestamp, header_string_head, header_string_tail, header2_length );
	header2       = libewf_header_values_convert_header_string_to_header2( header_string, *header2_length, header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header2_string_encase5: unable to create header2.\n" );

		return( NULL );
	}
	return( header2 );
}

/* Generate an EnCase6 header2
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER2 *libewf_header_values_generate_header2_string_encase6( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header2_length )
{
	EWF_HEADER2 *header2            = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "3\nmain\na\tc\tn\te\tt\tmd\tsn\tav\tov\tm\tu\tp\tdc\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type6( header_values, timestamp, header_string_head, header_string_tail, header2_length );
	header2       = libewf_header_values_convert_header_string_to_header2( header_string, *header2_length, header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header2_string_encase6: unable to create header2.\n" );

		return( NULL );
	}
	return( header2 );
}

/* Generate date string within a xheader value
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_generate_date_xheader_value( time_t timestamp )
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && !defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	char *ctime_string       = NULL;
#endif
	LIBEWF_CHAR *date_string = NULL;
	LIBEWF_CHAR *newline     = NULL;

	/* The libewf_common_ctime function returns a string of length 32
	 */
#if !defined( HAVE_WIDE_CHARACTER_TYPE )
	date_string = libewf_common_ctime( &timestamp );

#elif defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	date_string = libewf_common_wide_ctime( &timestamp );
#else
	ctime_string = libewf_common_ctime( &timestamp );

	if( ctime_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_xheader_value: unable to create ctime string.\n" );

		return( NULL );
	}
	date_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * 32 );

	if( libewf_common_copy_char_to_wchar( date_string, ctime_string, 32 ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_xheader_value: unable to copy ctime string to date string.\n" );

		return( NULL );
	}
#endif
	if( date_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_generate_date_xheader_value: unable to create date string.\n" );

		return( NULL );
	}
	newline = libewf_string_search( date_string, (LIBEWF_CHAR) '\n', 32 );

	if( newline != NULL )
	{
		newline[ 0 ] = (LIBEWF_CHAR) ' ';
		newline[ 1 ] = (LIBEWF_CHAR) tzname[ 0 ][ 0 ];
		newline[ 2 ] = (LIBEWF_CHAR) tzname[ 0 ][ 1 ];
		newline[ 3 ] = (LIBEWF_CHAR) tzname[ 0 ][ 2 ];
		newline[ 4 ] = (LIBEWF_CHAR) '\0';
	}
	return( date_string );
}

/* Parse a xml header string for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HEADER_VALUES *libewf_header_values_parse_header_string_xml( LIBEWF_CHAR *header_string_xml, size_t length, uint8_t date_format )
{
	LIBEWF_HEADER_VALUES *header_values = NULL;
	LIBEWF_CHAR **lines                 = NULL;
	LIBEWF_CHAR *open_tag_start         = NULL;
	LIBEWF_CHAR *open_tag_end           = NULL;
	LIBEWF_CHAR *close_tag_start        = NULL;
	LIBEWF_CHAR *close_tag_end          = NULL;
	size_t string_length                = 0;
	uint32_t line_count                 = 0;
	uint32_t iterator                   = 0;

	if( header_string_xml == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string_xml: invalid header string\n" );

		return( NULL );
	}
	lines = libewf_string_split( (LIBEWF_CHAR *) header_string_xml, length, (LIBEWF_CHAR) '\n', &line_count );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string_xml: unable to split lines in header string.\n" );

		return( NULL );
	}
	header_values = libewf_header_values_alloc();

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_header_string_xml: unable to create header values.\n" );

		libewf_string_split_values_free( lines, line_count );

		return( NULL );
	}
	for( iterator = 0; iterator < line_count; iterator++ )
	{
		if( ( lines[ iterator ] == NULL )
		 || ( lines[ iterator ] == (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
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
		open_tag_start = libewf_string_search( lines[ iterator ], (LIBEWF_CHAR) '<', string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = libewf_string_search( lines[ iterator ], (LIBEWF_CHAR) '>', string_length );

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
		close_tag_start = libewf_string_search_reverse( &open_tag_end[ 1 ], (LIBEWF_CHAR) '<', string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = libewf_string_search_reverse( &open_tag_end[ 1 ], (LIBEWF_CHAR) '>', string_length );

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
		*open_tag_end = (LIBEWF_CHAR) '\0';

		if( libewf_header_values_set_value( header_values, &open_tag_start[ 1 ] , &open_tag_end[ 1 ], string_length ) != 1 )
		{
			LIBEWF_VERBOSE_PRINT( "libewf_header_values_parse_header_string_xml: unable to set value with identifier: %" PRIs_EWF ".\n", &open_tag_start[ 1 ] );
		}
	}
	libewf_string_split_values_free( lines, line_count );

	return( header_values );
}

/* Parse an EWF xheader for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HEADER_VALUES *libewf_header_values_parse_xheader( EWF_HEADER *xheader, size_t size, uint8_t date_format )
{
	LIBEWF_HEADER_VALUES* header_values = NULL;
	LIBEWF_CHAR *xml_header_string      = NULL;

	if( xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_xheader: invalid xheader.\n" );

		return( NULL );
	}
	xml_header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * ( size + 1 ) );

	if( xml_header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_xheader: unable to create XML header string.\n" );

		return( NULL );
	}
	if( libewf_string_copy_from_header( xml_header_string, size, xheader, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_parse_xheader: unable to copy xheader to xml header string.\n" );

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
LIBEWF_CHAR *libewf_header_values_generate_header_string_xml( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *string_length )
{
	LIBEWF_CHAR *header_string         = NULL;
	LIBEWF_CHAR *xml_head              = _S_LIBEWF_CHAR( "<?xml version=\"1.0\"?>" );
	LIBEWF_CHAR *xml_open_tag_xheader  = _S_LIBEWF_CHAR( "<xheader>" );
	LIBEWF_CHAR *xml_close_tag_xheader = _S_LIBEWF_CHAR( "</xheader>" );
	LIBEWF_CHAR *acquiry_date          = _S_LIBEWF_CHAR( "" );
	uint32_t iterator                  = 0;
	int string_offset                  = 0;
	int character_count                = 0;

	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: invalid header values.\n" );

		return( NULL );
	}
	if( header_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: invalid header values.\n" );

		return( NULL );
	}
	if( string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: invalid string length.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: invalid header value - missing identifier.\n" );

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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: unable to generate acquiry date header value.\n" );

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

	header_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( header_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: unable to create header string.\n" );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: unable to set header string.\n" );

		libewf_common_free( header_string );

		if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
		 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
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
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: invalid header value - missing identifier.\n" );

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
				LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: unable to set header string.\n" );

				libewf_common_free( header_string );

				if( ( header_values->values[ LIBEWF_HEADER_VALUES_INDEX_ACQUIRY_DATE ] == NULL )
				 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
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
	 && ( acquiry_date != (LIBEWF_CHAR *) _S_LIBEWF_CHAR( "" ) ) )
	{
		character_count = libewf_string_snprintf( &header_string[ string_offset ], ( *string_length - string_offset ),
							  _S_LIBEWF_CHAR( "\t<acquiry_date>%" ) _S_LIBEWF_CHAR( PRIs_EWF )
							  _S_LIBEWF_CHAR( "</acquiry_date>\n" ), acquiry_date );

		libewf_common_free( acquiry_date );

		if( character_count <= -1 )
		{
			LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: unable to set header string.\n" );

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
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_xml: unable to set header string.\n" );

		libewf_common_free( header_string );

		*string_length = 0;

		return( NULL );
	}
	/* Make sure the string is terminated
	 */
	header_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( header_string );
}

/* Generate an EWFX header
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_header_string_ewfx( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length )
{
	EWF_HEADER  *header             = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\nmain\nc\tn\ta\te\tt\tav\tov\tm\tu\tp\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type3( header_values, timestamp, header_string_head, header_string_tail, header_length );
	header        = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( header == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header_string_ewfx: unable to create header.\n" );

		return( NULL );
	}
	return( header );
}

/* Generate an EWFX header2
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER2 *libewf_header_values_generate_header2_string_ewfx( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header2_length )
{
	EWF_HEADER  *header2            = NULL;
	LIBEWF_CHAR *header_string      = NULL;
	LIBEWF_CHAR *header_string_head = _S_LIBEWF_CHAR( "1\nmain\na\tc\tn\te\tt\tav\tov\tm\tu\tp\n" );
	LIBEWF_CHAR *header_string_tail = _S_LIBEWF_CHAR( "\n\n" );

	header_string = libewf_header_values_generate_header_string_type4( header_values, timestamp, header_string_head, header_string_tail, header2_length );
	header2       = libewf_header_values_convert_header_string_to_header2( header_string, *header2_length, header2_length );

	if( header2 == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_header2_string_ewfx: unable to create header2.\n" );

		return( NULL );
	}
	return( header2 );
}

/* Generate an EWFX xheader
 * Returns a pointer to the new instance, NULL on error
 */
EWF_HEADER *libewf_header_values_generate_xheader_string_ewfx( LIBEWF_HEADER_VALUES *header_values, time_t timestamp, size_t *header_length )
{
	EWF_HEADER  *xheader       = NULL;
	LIBEWF_CHAR *header_string = NULL;

	header_string = libewf_header_values_generate_header_string_xml( header_values, timestamp, header_length );
	xheader       = libewf_header_values_convert_header_string_to_header( header_string, *header_length, header_length );

	if( xheader == NULL )
	{
		LIBEWF_WARNING_PRINT( "libewf_header_values_generate_xheader_string_ewfx: unable to create xheader.\n" );

		return( NULL );
	}
	return( xheader );
}


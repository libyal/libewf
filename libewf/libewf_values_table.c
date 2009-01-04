/*
 * libewf values table
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
#include "libewf_values_table.h"
#include "libewf_notify.h"
#include "libewf_string.h"

#include "ewf_definitions.h"

/* Allocates memory for a new values table struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_VALUES_TABLE *libewf_values_table_alloc( uint32_t amount )
{
	LIBEWF_VALUES_TABLE *values_table = NULL;
	static char *function           = "libewf_values_table_alloc";
	size_t values_table_size         = 0;

	values_table = (LIBEWF_VALUES_TABLE *) libewf_common_alloc( LIBEWF_VALUES_TABLE_SIZE );

	if( values_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate values table.\n",
		 function );

		return( NULL );
	}
	values_table_size = amount * sizeof( libewf_char_t * );

	if( values_table_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		libewf_common_free( values_table );

		return( NULL );
	}
	values_table->identifiers = (libewf_char_t **) libewf_common_alloc( values_table_size );

	if( values_table->identifiers == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate identifiers.\n",
		 function );

		libewf_common_free( values_table );

		return( NULL );
	}
	if( libewf_common_memset( values_table->identifiers, 0, values_table_size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear identifiers.\n",
		 function );

		libewf_common_free( values_table->identifiers );
		libewf_common_free( values_table );

		return( NULL );
	}
	values_table->values = (libewf_char_t **) libewf_common_alloc( values_table_size );

	if( values_table->values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate values.\n",
		 function );

		libewf_common_free( values_table->identifiers );
		libewf_common_free( values_table );

		return( NULL );
	}
	if( libewf_common_memset( values_table->values, 0, values_table_size ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear values.\n",
		 function );

		libewf_common_free( values_table->identifiers );
		libewf_common_free( values_table->values );
		libewf_common_free( values_table );

		return( NULL );
	}
	values_table->amount = amount;

	return( values_table );
}

/* Reallocates memory for the values table
 * Returns 1 if successful, or -1 on error
 */
int libewf_values_table_realloc( LIBEWF_VALUES_TABLE *values_table, uint32_t previous_amount, uint32_t new_amount )
{
	libewf_char_t **reallocation = NULL;
	static char *function        = "libewf_values_table_realloc";
	size_t previous_size         = previous_amount * sizeof( libewf_char_t * );
	size_t new_size              = new_amount * sizeof( libewf_char_t * );

	if( values_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( ( previous_amount > (uint32_t) INT32_MAX )
	 || ( new_amount > (uint32_t) INT32_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid amount value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( previous_amount >= new_amount )
	{
		LIBEWF_WARNING_PRINT( "%s: new amount smaller than previous amount.\n",
		 function );

		return( -1 );
	}
	if( ( previous_size > (size_t) SSIZE_MAX )
	 || ( new_size > (ssize_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	reallocation = (libewf_char_t **) libewf_common_realloc( values_table->identifiers, new_size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate identifiers.\n",
		 function );

		return( -1 );
	}
	values_table->identifiers = reallocation;

        if( libewf_common_memset(
             &( values_table->identifiers[ previous_amount ] ),
             0,
             ( new_size - previous_size ) ) == NULL )
        {
                LIBEWF_WARNING_PRINT( "%s: unable to clear identifiers.\n",
                 function );

                return( -1 );
        }
	reallocation = (libewf_char_t **) libewf_common_realloc( values_table->values, new_size );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate values.\n",
		 function );

		return( -1 );
	}
	values_table->values = reallocation;

        if( libewf_common_memset(
             &( values_table->values[ previous_amount ] ),
             0,
             ( new_size - previous_size ) ) == NULL )
        {
                LIBEWF_WARNING_PRINT( "%s: unable to clear values.\n",
                 function );

                return( -1 );
        }
	values_table->amount = new_amount;

	return( 1 );
}

/* Frees memory of a values table struct including elements
 */
void libewf_values_table_free( LIBEWF_VALUES_TABLE *values_table )
{
	static char *function = "libewf_values_table_free";
	uint32_t iterator     = 0;

	if( values_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid values table.\n",
		 function );

		return;
	}
	if( values_table->values != NULL )
	{
		for( iterator = 0; iterator < values_table->amount; iterator++ )
		{
			libewf_common_free( values_table->identifiers[ iterator ] );
			libewf_common_free( values_table->values[ iterator ] );
		}
		libewf_common_free( values_table->values );
	}
	libewf_common_free( values_table );
}

/* Retrieves the value index number, or -1 on error
 * The index number will be larger than the amount when the identifier is not present in the values table
 */
int32_t libewf_values_table_get_index( LIBEWF_VALUES_TABLE *values_table, libewf_char_t *identifier )
{
	static char *function    = "libewf_values_table_get_index";
	size_t string_length     = 0;
	size_t identifier_length = 0;
	int32_t iterator         = 0;

	if( values_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	identifier_length = libewf_string_length( identifier );

	if( identifier_length == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( values_table->amount > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid values table amount value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < (int32_t) values_table->amount; iterator++ )
	{
		if( values_table->identifiers[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: missing identifier for index: %" PRIi32 ".\n",
			 function, iterator );

			continue;
		}
		string_length = libewf_string_length( values_table->identifiers[ iterator ] );

		if( string_length == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine length of identifier of index: %" PRIi32 ".\n",
			 function, iterator );

			continue;
		}
		if( string_length != identifier_length )
		{
			continue;
		}
		if( libewf_string_compare( identifier, values_table->identifiers[ iterator ], identifier_length ) == 0 )
		{
			return( iterator );
		}
	}
	return( iterator );
}

/* Retrieves an identifier in the values table
 * Length should contain the amount of characters in the string
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int libewf_values_table_get_identifier( LIBEWF_VALUES_TABLE *values_table, uint32_t index, libewf_char_t *identifier, size_t length )
{
	static char *function    = "libewf_values_table_get_identifier";
	size_t identifier_length = 0;

	if( values_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid values table.\n",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid identifier.\n",
		 function );

		return( -1 );
	}
	if( values_table->amount == 0 )
	{
		return( 0 );
	}
	if( index >= values_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid index out of range.\n",
		 function );

		return( -1 );
	}
	if( values_table->identifiers[ index ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: missing identifier for index: %" PRIu32 ".\n",
		 function, index );

		return( -1 );
	}
	identifier_length = libewf_string_length( values_table->identifiers[ index ] );

	/* Don't bother with empty values
	 */
	if( identifier_length == 0 )
	{
		return( 0 );
	}
	/* Add 1 additional byte required for the end of string character
	 */
	identifier_length += 1;

	if( identifier_length > length )
	{
		LIBEWF_WARNING_PRINT( "%s: identifier too small.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy( identifier, values_table->identifiers[ index ], identifier_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set identifier.\n",
		 function );

		return( -1 );
	}
	identifier[ identifier_length - 1 ] = (libewf_char_t) '\0';

	return( 1 );
}

/* Retrieves a value in the values table
 * Length should contain the amount of characters in the string
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int libewf_values_table_get_value( LIBEWF_VALUES_TABLE *values_table, libewf_char_t *identifier, libewf_char_t *value, size_t length )
{
	static char *function = "libewf_values_table_get_value";
	size_t value_length   = 0;
	int32_t index         = 0;

	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	index = libewf_values_table_get_index( values_table, identifier );

	if( index <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find index for: %" PRIs_EWF ".\n",
		 function, identifier );

		return( -1 );
	}
	if( (uint32_t) index >= values_table->amount )
	{
		return( 0 );
	}
	if( values_table->values[ index ] == NULL )
	{
		return( 0 );
	}
	value_length = libewf_string_length( values_table->values[ index ] );

	/* Don't bother with empty values
	 */
	if( value_length == 0 )
	{
		return( 0 );
	}
	/* Add 1 additional byte required for the end of string character
	 */
	value_length += 1;

	if( value_length > length )
	{
		LIBEWF_WARNING_PRINT( "%s: value too small.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy( value, values_table->values[ index ], value_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set value.\n",
		 function );

		return( -1 );
	}
	value[ value_length - 1 ] = (libewf_char_t) '\0';

	return( 1 );
}

/* Set a value in the values table
 * Length should contain the amount of characters in the string
 * Frees the previous value if necessary
 * Returns 1 if successful, -1 on error
 */
int libewf_values_table_set_value( LIBEWF_VALUES_TABLE *values_table, libewf_char_t *identifier, libewf_char_t *value, size_t length )
{
	static char *function = "libewf_values_table_set_value";
	size_t string_length  = 0;
	int32_t index         = 0;

	index = libewf_values_table_get_index( values_table, identifier );

	if( index <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find index for: %" PRIs_EWF ".\n",
		 function, identifier );

		return( -1 );
	}
	if( (uint32_t) index >= values_table->amount )
	{
		string_length = libewf_string_length( identifier );

		if( string_length == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine length of identifier.\n",
			 function );

			return( -1 );
		}
		if( libewf_values_table_realloc( values_table, values_table->amount, ( index + 1 ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate values table.\n",
			 function );

			return( -1 );
		}
		values_table->identifiers[ index ] = libewf_string_duplicate( identifier, string_length );

		if( values_table->identifiers[ index ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set identifier.\n",
			 function );

			libewf_common_free( values_table->identifiers[ index ] );

			values_table->identifiers[ index ] = NULL;

			return( -1 );
		}
	}
	/* Clear the buffer of the previous value
	 */
	if( values_table->values[ index ] != NULL )
	{
		libewf_common_free( values_table->values[ index ] );

		values_table->values[ index ] = NULL;
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
	values_table->values[ index ] = libewf_string_duplicate( value, length );

	if( values_table->values[ index ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set value.\n",
		 function );

		libewf_common_free( values_table->values[ index ] );

		values_table->values[ index ] = NULL;

		return( -1 );
	}
	return( 1 );
}


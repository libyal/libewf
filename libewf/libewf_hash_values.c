/*
 * libewf hash values
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
#include "libewf_hash_values.h"
#include "libewf_notify.h"
#include "libewf_string.h"

#include "ewf_compress.h"
#include "ewf_definitions.h"

/* Allocates memory for a new hash values struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HASH_VALUES *libewf_hash_values_alloc( void )
{
	LIBEWF_HASH_VALUES *hash_values = NULL;
	static char *function           = "libewf_hash_values_alloc";
	size_t hash_values_size         = 0;

	hash_values = (LIBEWF_HASH_VALUES *) libewf_common_alloc_cleared( LIBEWF_HASH_VALUES_SIZE, 0 );

	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate hash values.\n",
		 function );

		return( NULL );
	}
	hash_values->amount = LIBEWF_HASH_VALUES_DEFAULT_AMOUNT;
	hash_values_size    = hash_values->amount * sizeof( LIBEWF_CHAR* );

	if( hash_values_size > (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid size value exceeds maximum.\n",
		 function );

		libewf_common_free( hash_values );

		return( NULL );
	}
	hash_values->identifiers = (LIBEWF_CHAR **) libewf_common_alloc_cleared( hash_values_size, 0 );

	if( hash_values->identifiers == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate identifiers.\n",
		 function );

		libewf_common_free( hash_values );

		return( NULL );
	}
	hash_values->values = (LIBEWF_CHAR **) libewf_common_alloc_cleared( hash_values_size, 0 );

	if( hash_values->values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate values.\n",
		 function );

		libewf_common_free( hash_values->identifiers );
		libewf_common_free( hash_values );

		return( NULL );
	}
	hash_values->identifiers[ 0 ] = libewf_string_duplicate( _S_LIBEWF_CHAR( "MD5" ), 3 );

	return( hash_values );
}

/* Reallocates memory for the hash values
 * Returns 1 if successful, or -1 on error
 */
int libewf_hash_values_realloc( LIBEWF_HASH_VALUES *hash_values, uint32_t previous_amount, uint32_t new_amount )
{
	LIBEWF_CHAR **reallocation = NULL;
	static char *function      = "libewf_hash_values_realloc";
	size_t previous_size       = previous_amount * sizeof( LIBEWF_CHAR* );
	size_t new_size            = new_amount * sizeof( LIBEWF_CHAR* );

	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash values.\n",
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
	reallocation = (LIBEWF_CHAR **) libewf_common_realloc_new_cleared( hash_values->identifiers, previous_size, new_size, 0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate identifiers.\n",
		 function );

		return( -1 );
	}
	hash_values->identifiers = reallocation;
	reallocation             = (LIBEWF_CHAR **) libewf_common_realloc_new_cleared( hash_values->values, previous_size, new_size, 0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate values.\n",
		 function );

		return( -1 );
	}
	hash_values->values = reallocation;
	hash_values->amount = new_amount;

	return( 1 );
}

/* Frees memory of a hash values struct including elements
 */
void libewf_hash_values_free( LIBEWF_HASH_VALUES *hash_values )
{
	static char *function = "libewf_hash_values_free";
	uint32_t iterator     = 0;

	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash values.\n",
		 function );

		return;
	}
	if( hash_values->values != NULL )
	{
		for( iterator = 0; iterator < hash_values->amount; iterator++ )
		{
			libewf_common_free( hash_values->identifiers[ iterator ] );
			libewf_common_free( hash_values->values[ iterator ] );
		}
		libewf_common_free( hash_values->values );
	}
	libewf_common_free( hash_values );
}

/* Retrieves the hash value index number, or -1 on error
 * The index number will be larger than the amount when the identifier is not present in the hash values
 */
int32_t libewf_hash_values_get_index( LIBEWF_HASH_VALUES *hash_values, LIBEWF_CHAR *identifier )
{
	static char *function    = "libewf_hash_values_get_index";
	size_t string_length     = 0;
	size_t identifier_length = 0;
	int32_t iterator         = 0;

	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash values.\n",
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
	if( hash_values->amount > (uint32_t) INT32_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash values amount value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < (int32_t) hash_values->amount; iterator++ )
	{
		if( hash_values->identifiers[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: missing identifier for index: %" PRIi32 ".\n",
			 function, iterator );

			continue;
		}
		string_length = libewf_string_length( hash_values->identifiers[ iterator ] );

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
		if( libewf_string_compare( identifier, hash_values->identifiers[ iterator ], identifier_length ) == 0 )
		{
			return( iterator );
		}
	}
	return( iterator );
}

/* Retrieves a hash value in value
 * Length should contain the amount of characters in the string
 * Returns 1 if successful, 0 if value not present, -1 on error
 */
int libewf_hash_values_get_value( LIBEWF_HASH_VALUES *hash_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	static char *function    = "libewf_hash_values_get_value";
	size_t hash_value_length = 0;
	int32_t index            = 0;

	if( value == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid value.\n",
		 function );

		return( -1 );
	}
	index = libewf_hash_values_get_index( hash_values, identifier );

	if( index <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find index for: %" PRIs_EWF ".\n",
		 function, identifier );

		return( -1 );
	}
	if( (uint32_t) index > hash_values->amount )
	{
		return( 0 );
	}
	if( hash_values->values[ index ] == NULL )
	{
		return( 0 );
	}
	hash_value_length = libewf_string_length( hash_values->values[ index ] );

	/* Don't bother with empty values
	 */
	if( hash_value_length == 0 )
	{
		return( 0 );
	}
	/* Add 1 additional byte required for the end of string character
	 */
	hash_value_length += 1;

	if( hash_value_length > length )
	{
		LIBEWF_WARNING_PRINT( "%s: value too small.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy( value, hash_values->values[ index ], hash_value_length ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash value.\n",
		 function );

		return( -1 );
	}
	value[ hash_value_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( 1 );
}

/* Set a hash value
 * Length should contain the amount of characters in the string
 * Frees the previous hash value if necessary
 * Returns 1 if successful, -1 on error
 */
int libewf_hash_values_set_value( LIBEWF_HASH_VALUES *hash_values, LIBEWF_CHAR *identifier, LIBEWF_CHAR *value, size_t length )
{
	static char *function = "libewf_hash_values_set_value";
	size_t string_length  = 0;
	int32_t index         = 0;

	index = libewf_hash_values_get_index( hash_values, identifier );

	if( index <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to find index for: %" PRIs_EWF ".\n",
		 function, identifier );

		return( -1 );
	}
	if( (uint32_t) index >= hash_values->amount )
	{
		string_length = libewf_string_length( identifier );

		if( string_length == 0 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to determine length of identifier.\n",
			 function );

			return( -1 );
		}
		if( libewf_hash_values_realloc( hash_values, hash_values->amount, ( index + 1 ) ) != 1 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to reallocate hash values.\n",
			 function );

			return( -1 );
		}
		hash_values->identifiers[ index ] = libewf_string_duplicate( identifier, string_length );

		if( hash_values->identifiers[ index ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to set identifier.\n",
			 function );

			libewf_common_free( hash_values->identifiers[ index ] );

			hash_values->identifiers[ index ] = NULL;

			return( -1 );
		}
	}
	/* Clear the buffer of the previous hash value
	 */
	if( hash_values->values[ index ] != NULL )
	{
		libewf_common_free( hash_values->values[ index ] );

		hash_values->values[ index ] = NULL;
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
	hash_values->values[ index ] = libewf_string_duplicate( value, length );

	if( hash_values->values[ index ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set value.\n",
		 function );

		libewf_common_free( hash_values->values[ index ] );

		hash_values->values[ index ] = NULL;

		return( -1 );
	}
	return( 1 );
}

/* Parse a xml hash string for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HASH_VALUES *libewf_hash_values_parse_hash_string_xml( LIBEWF_CHAR *hash_string_xml, size_t length )
{
	LIBEWF_HASH_VALUES *hash_values = NULL;
	LIBEWF_CHAR **lines             = NULL;
	LIBEWF_CHAR *open_tag_start     = NULL;
	LIBEWF_CHAR *open_tag_end       = NULL;
	LIBEWF_CHAR *close_tag_start    = NULL;
	LIBEWF_CHAR *close_tag_end      = NULL;
	static char *function           = "libewf_hash_values_parse_hash_string_xml";
	size_t string_length            = 0;
	uint32_t line_count             = 0;
	uint32_t iterator               = 0;

	if( hash_string_xml == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash string.\n",
		 function );

		return( NULL );
	}
	lines = libewf_string_split( (LIBEWF_CHAR *) hash_string_xml, length, (LIBEWF_CHAR) '\n', &line_count );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to split lines in hash string.\n",
		 function );

		return( NULL );
	}
	hash_values = libewf_hash_values_alloc();

	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create hash values.\n",
		 function );

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

		if( libewf_hash_values_set_value( hash_values, &open_tag_start[ 1 ] , &open_tag_end[ 1 ], string_length ) != 1 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unable to set value with identifier: %" PRIs_EWF ".\n",
			 function, &open_tag_start[ 1 ] );
		}
	}
	libewf_string_split_values_free( lines, line_count );

	return( hash_values );
}

/* Parse an EWF xhash for the values
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HASH_VALUES *libewf_hash_values_parse_xhash( EWF_CHAR *xhash, size_t size )
{
	LIBEWF_HASH_VALUES* hash_values = NULL;
	LIBEWF_CHAR *xml_hash_string    = NULL;
	static char *function           = "libewf_hash_values_parse_xhash";

	if( xhash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid xhash.\n",
		 function );

		return( NULL );
	}
	xml_hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * ( size + 1 ) );

	if( xml_hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create XML hash string.\n",
		 function );

		return( NULL );
	}
	if( libewf_string_copy_from_ewf_char( xml_hash_string, size, xhash, size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy xhash to xml hash string.\n",
		 function );

		libewf_common_free( xml_hash_string );

		return( NULL );
	}
	hash_values = libewf_hash_values_parse_hash_string_xml( xml_hash_string, size );

	libewf_common_free( xml_hash_string );

	return( hash_values );
}

/* Converts a hash string into a hash
 * Cleans the hash string
 * Sets hash length
 * Returns a pointer to the new instance, NULL on error
 */
EWF_CHAR *libewf_hash_values_convert_hash_string_to_hash( LIBEWF_CHAR *hash_string, size_t string_length, size_t *hash_length )
{
	EWF_CHAR *hash        = NULL;
	static char *function = "libewf_hash_values_convert_hash_string_to_hash";

	if( hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash string.\n",
		 function );

		return( NULL );
	}
	if( hash_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash length.\n",
		 function );

		return( NULL );
	}
	hash = (EWF_CHAR *) libewf_common_alloc( EWF_CHAR_SIZE * string_length );

	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create hash.\n",
		 function );

		libewf_common_free( hash_string );

		return( NULL );
	}
	*hash_length = string_length;

	if( libewf_string_copy_to_ewf_char( hash_string, string_length, hash, *hash_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash.\n",
		 function );

		libewf_common_free( hash );
		libewf_common_free( hash_string );

		*hash_length = 0;

		return( NULL );
	}
	libewf_common_free( hash_string );

	return( hash );
}

/* Generate a hash format in XML
 * Sets string length
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_CHAR *libewf_hash_values_generate_hash_string_xml( LIBEWF_HASH_VALUES *hash_values, size_t *string_length )
{
	LIBEWF_CHAR *hash_string         = NULL;
	LIBEWF_CHAR *xml_head            = _S_LIBEWF_CHAR( "<?xml version=\"1.0\"?>" );
	LIBEWF_CHAR *xml_open_tag_xhash  = _S_LIBEWF_CHAR( "<xhash>" );
	LIBEWF_CHAR *xml_close_tag_xhash = _S_LIBEWF_CHAR( "</xhash>" );
	static char *function            = "libewf_hash_values_generate_hash_string_xml";
	uint32_t iterator                = 0;
	int string_offset                = 0;
	int character_count              = 0;

	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash values.\n",
		 function );

		return( NULL );
	}
	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash values.\n",
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
	*string_length += libewf_string_length( xml_open_tag_xhash ) + 1;
	*string_length += libewf_string_length( xml_close_tag_xhash ) + 1;

	for( iterator = 0; iterator < hash_values->amount; iterator++ )
	{
		if( hash_values->identifiers[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid hash value - missing identifier.\n",
			 function );

			continue;
		}
		if( hash_values->values[ iterator ] != NULL )
		{
			/* Add space for a leading tab, <identifier></identifier> and an end of line
			 */
			*string_length += ( 2 * libewf_string_length( hash_values->identifiers[ iterator ] ) ) + 7;

			/* Add space for the hash value
			 */
			*string_length += libewf_string_length( hash_values->values[ iterator ] );
		}
	}
	/* allow for an empty line and an end of string
	 */
	*string_length += 2;

	hash_string = (LIBEWF_CHAR *) libewf_common_alloc( LIBEWF_CHAR_SIZE * *string_length );

	if( hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create hash string.\n",
		 function );

		*string_length = 0;

		return( NULL );
	}
	character_count = libewf_string_snprintf(
	                   hash_string,
	                   *string_length,
	                   _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n" ),
	                   xml_head,
	                   xml_open_tag_xhash );

	if( character_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash string.\n",
		 function );

		libewf_common_free( hash_string );

		*string_length = 0;

		return( NULL );
	}
	string_offset = character_count;

	for( iterator = 0; iterator < hash_values->amount; iterator++ )
	{
		if( hash_values->identifiers[ iterator ] == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: invalid hash value - missing identifier.\n",
			 function );

			continue;
		}
		if( hash_values->values[ iterator ] != NULL )
		{
			character_count = libewf_string_snprintf(
			                   &hash_string[ string_offset ],
			                   ( *string_length - string_offset ),
			                   _S_LIBEWF_CHAR( "\t<%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( ">%" )
			                   _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "</%" ) _S_LIBEWF_CHAR( PRIs_EWF )
			                   _S_LIBEWF_CHAR( ">\n" ),
			                   hash_values->identifiers[ iterator ],
			                   hash_values->values[ iterator ],
			                   hash_values->identifiers[ iterator ] );

			if( character_count <= -1 )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to set hash string.\n",
				 function );

				libewf_common_free( hash_string );

				*string_length = 0;

				return( NULL );
			}
			string_offset += character_count;
		}
	}
	character_count = libewf_string_snprintf(
	                   &hash_string[ string_offset ],
	                   ( *string_length - string_offset ),
	                   _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n\n" ),
	                   xml_close_tag_xhash );

	if( character_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash string.\n",
		 function );

		libewf_common_free( hash_string );

		*string_length = 0;

		return( NULL );
	}
	/* Make sure the string is terminated
	 */
	hash_string[ *string_length - 1 ] = (LIBEWF_CHAR) '\0';

	return( hash_string );
}

/* Generate an EWFX xhash
 * Returns a pointer to the new instance, NULL on error
 */
EWF_CHAR *libewf_hash_values_generate_xhash_string_ewfx( LIBEWF_HASH_VALUES *hash_values, size_t *hash_length )
{
	EWF_CHAR *xhash          = NULL;
	LIBEWF_CHAR *hash_string = NULL;
	static char *function    = "libewf_hash_values_generate_xhash_string_ewfx";

	hash_string = libewf_hash_values_generate_hash_string_xml( hash_values, hash_length );
	xhash       = libewf_hash_values_convert_hash_string_to_hash( hash_string, *hash_length, hash_length );

	if( xhash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create xhash.\n",
		 function );

		return( NULL );
	}
	return( xhash );
}


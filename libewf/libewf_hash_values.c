/*
 * Hash values functions
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
#include <memory.h>
#include <notify.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_hash_values.h"
#include "libewf_string.h"

#include "ewf_definitions.h"

/* Initializes the hash values
 * Returns 1 if successful, or -1 otherwise
 */
int libewf_hash_values_initialize(
     libewf_values_table_t *hash_values )
{
	static char *function = "libewf_hash_values_initialize";

	if( hash_values == NULL )
	{
		notify_warning_printf( "%s: invalid hash values.\n",
		 function );

		return( -1 );
	}
	hash_values->identifiers[ 0 ] = string_duplicate(
	                                 _CHARACTER_T_STRING( "MD5" ),
	                                 3 );

	return( 1 );
}

/* Parse a XML hash string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_hash_string_xml(
     libewf_values_table_t **hash_values,
     character_t *hash_string_xml,
     size_t hash_string_xml_size )
{
	character_t **lines          = NULL;
	character_t *open_tag_start  = NULL;
	character_t *open_tag_end    = NULL;
	character_t *close_tag_start = NULL;
	character_t *close_tag_end   = NULL;
	static char *function        = "libewf_hash_values_parse_hash_string_xml";
	size_t string_length         = 0;
	size_t amount_of_lines       = 0;
	size_t line_iterator         = 0;

	if( hash_string_xml == NULL )
	{
		notify_warning_printf( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_split(
	     hash_string_xml,
	     hash_string_xml_size,
	     (character_t) '\n',
	     &lines,
	     &amount_of_lines ) != 1 )
	{
		notify_warning_printf( "%s: unable to split hash string into lines.\n",
		 function );

		return( -1 );
	}
	*hash_values = libewf_values_table_alloc(
	                LIBEWF_HASH_VALUES_DEFAULT_AMOUNT );

	if( *hash_values == NULL )
	{
		notify_warning_printf( "%s: unable to create hash values.\n",
		 function );

		libewf_string_split_values_free(
		 lines,
		 amount_of_lines );

		return( -1 );
	}
	if( libewf_hash_values_initialize(
	     *hash_values ) != 1 )
	{
		notify_warning_printf( "%s: unable to initialize the hash values.\n",
		 function );

		libewf_string_split_values_free(
		 lines,
		 amount_of_lines );

		return( -1 );
	}
	for( line_iterator = 0; line_iterator < amount_of_lines; line_iterator++ )
	{
		if( ( lines[ line_iterator ] == NULL )
		 || ( lines[ line_iterator ] == (character_t *) _CHARACTER_T_STRING( "" ) ) )
		{
			continue;
		}
		string_length = string_length(
		                 lines[ line_iterator ] );

		/* Ignore empty lines
		 */
		if( string_length == 0 )
		{
			continue;
		}
		open_tag_start = string_search(
		                  lines[ line_iterator ],
		                  (character_t) '<',
		                  string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = string_search(
		                lines[ line_iterator ],
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
		string_length -= (size_t) ( open_tag_end - lines[ line_iterator ] );

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
		string_length = (size_t) ( close_tag_start - open_tag_end ) - 1;

		/* Make sure the identifier string will be terminated
		 */
		*open_tag_end = (character_t) '\0';

		if( libewf_values_table_set_value(
		     *hash_values,
		     &open_tag_start[ 1 ],
		     &open_tag_end[ 1 ],
		     string_length ) != 1 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: unable to set value with identifier: %" PRIs ".\n",
			 function, &open_tag_start[ 1 ] );
#endif
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

/* Parse an EWF xhash for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_xhash(
     libewf_values_table_t **hash_values,
     uint8_t *xhash,
     size_t xhash_size )
{
	character_t *xml_hash_string = NULL;
	static char *function        = "libewf_hash_values_parse_xhash";
	ssize_t xml_hash_string_size = 0;
	int result                   = 0;

	if( xhash == NULL )
	{
		notify_warning_printf( "%s: invalid xhash.\n",
		 function );

		return( -1 );
	}
	xml_hash_string_size = string_size_from_byte_stream(
	                        (uint8_t *) xhash,
	                        xhash_size,
	                        LIBUCA_CODEPAGE_ASCII );

	if( xml_hash_string_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine XML hash string size.\n",
		 function );

		return( -1 );
	}
	xml_hash_string = (character_t *) memory_allocate(
	                                   sizeof( character_t ) * (size_t) xml_hash_string_size );

	if( xml_hash_string == NULL )
	{
		notify_warning_printf( "%s: unable to create XML hash string.\n",
		 function );

		return( -1 );
	}
	if( string_copy_from_byte_stream(
	     xml_hash_string,
	     (size_t) xml_hash_string_size,
	     xhash,
	     xhash_size,
	     LIBUCA_CODEPAGE_ASCII ) != 1 )
	{
		notify_warning_printf( "%s: unable to XML set hash string.\n",
		 function );

		memory_free(
		 xml_hash_string );

		return( -1 );
	}
	result = libewf_hash_values_parse_hash_string_xml(
	          hash_values,
	          xml_hash_string,
	          (size_t) xml_hash_string_size );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to parse XML hash string.\n",
		 function );
	}
	memory_free(
	 xml_hash_string );

	return( result );
}

/* Converts a hash string into a hash
 * Sets the hash and hash size
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_convert_hash_string_to_xhash(
     character_t *hash_string,
     size_t hash_string_size,
     uint8_t **xhash,
     size_t *xhash_size )
{
	static char *function    = "libewf_hash_values_convert_hash_string_to_xhash";
	ssize_t utf8_stream_size = 0;

	if( hash_string == NULL )
	{
		notify_warning_printf( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	if( ( hash_string_size == 0 )
	 || ( hash_string_size > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid hash string size.\n",
		 function );

		return( -1 );
	}
	if( xhash == NULL )
	{
		notify_warning_printf( "%s: invalid xhash.\n",
		 function );

		return( -1 );
	}
	if( xhash_size == NULL )
	{
		notify_warning_printf( "%s: invalid xhash size.\n",
		 function );

		return( -1 );
	}
	if( *xhash != NULL )
	{
		notify_warning_printf( "%s: xhash already created .\n",
		 function );

		return( -1 );
	}
	utf8_stream_size = utf8_stream_size_from_string(
	                    hash_string,
	                    hash_string_size );

	if( utf8_stream_size < 0 )
	{
		notify_warning_printf( "%s: unable to determine xhash size.\n",
		 function );

		return( -1 );
	}
	*xhash = (uint8_t *) memory_allocate(
	                      sizeof( uint8_t ) * (size_t) utf8_stream_size );

	if( *xhash == NULL )
	{
		notify_warning_printf( "%s: unable to create xhash.\n",
		 function );

		return( -1 );
	}
	if( utf8_stream_copy_from_string(
	     *xhash,
	     (size_t) utf8_stream_size,
	     hash_string,
	     hash_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to set xhash.\n",
		 function );

		memory_free(
		 *xhash );

		*xhash = NULL;

		return( -1 );
	}
	*xhash_size = (size_t) utf8_stream_size;

	return( 1 );
}

/* Generate a hash format in XML
 * Sets hash sting and hash string size
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_hash_string_xml(
     libewf_values_table_t *hash_values,
     character_t **hash_string,
     size_t *hash_string_size )
{
	character_t *xml_head            = _CHARACTER_T_STRING( "<?xml version=\"1.0\"?>" );
	character_t *xml_open_tag_xhash  = _CHARACTER_T_STRING( "<xhash>" );
	character_t *xml_close_tag_xhash = _CHARACTER_T_STRING( "</xhash>" );
	static char *function            = "libewf_hash_values_generate_hash_string_xml";
	uint32_t iterator                = 0;
	int string_offset                = 0;
	int character_count              = 0;

	if( hash_values == NULL )
	{
		notify_warning_printf( "%s: invalid hash values.\n",
		 function );

		return( -1 );
	}
	if( hash_string == NULL )
	{
		notify_warning_printf( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	if( *hash_string != NULL )
	{
		notify_warning_printf( "%s: hash string already created.\n",
		 function );

		return( -1 );
	}
	if( hash_string_size == NULL )
	{
		notify_warning_printf( "%s: invalid hash string size.\n",
		 function );

		return( -1 );
	}
	/* Add space for the XML data and an end of line
	 */
	*hash_string_size = 1 + string_length(
	                         xml_head );

	*hash_string_size += 1 + string_length(
	                          xml_open_tag_xhash );

	*hash_string_size += 1 + string_length(
	                          xml_close_tag_xhash );

	for( iterator = 0; iterator < hash_values->amount; iterator++ )
	{
		if( hash_values->identifiers[ iterator ] == NULL )
		{
			notify_warning_printf( "%s: invalid hash value - missing identifier.\n",
			 function );

			continue;
		}
		if( hash_values->values[ iterator ] != NULL )
		{
			/* Add space for a leading tab, <identifier></identifier> and an end of line
			 */
			*hash_string_size += 7 + ( 2 * string_length(
			                                hash_values->identifiers[ iterator ] ) );

			/* Add space for the hash value
			 */
			*hash_string_size += string_length(
			                      hash_values->values[ iterator ] );
		}
	}
	/* allow for an empty line and an end of string
	 */
	*hash_string_size += 2;

	*hash_string = (character_t *) memory_allocate(
                                        sizeof( character_t ) * *hash_string_size );

	if( *hash_string == NULL )
	{
		notify_warning_printf( "%s: unable to create hash string.\n",
		 function );

		*hash_string_size = 0;

		return( -1 );
	}
	character_count = string_snprintf(
	                   *hash_string,
	                   *hash_string_size,
	                   _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "\n%" )
	                   _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "\n" ),
	                   xml_head,
	                   xml_open_tag_xhash );

	if( character_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set hash string.\n",
		 function );

		memory_free(
		 *hash_string );

		*hash_string      = NULL;
		*hash_string_size = 0;

		return( -1 );
	}
	string_offset = character_count;

	for( iterator = 0; iterator < hash_values->amount; iterator++ )
	{
		if( hash_values->identifiers[ iterator ] == NULL )
		{
			notify_warning_printf( "%s: invalid hash value - missing identifier.\n",
			 function );

			continue;
		}
		if( hash_values->values[ iterator ] != NULL )
		{
			character_count = string_snprintf(
			                   &( ( *hash_string) [ string_offset ] ),
			                   ( *hash_string_size - string_offset ),
			                   _CHARACTER_T_STRING( "\t<%" ) _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( ">%" )
			                   _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "</%" ) _CHARACTER_T_STRING( PRIs )
			                   _CHARACTER_T_STRING( ">\n" ),
			                   hash_values->identifiers[ iterator ],
			                   hash_values->values[ iterator ],
			                   hash_values->identifiers[ iterator ] );

			if( character_count <= -1 )
			{
				notify_warning_printf( "%s: unable to set hash string.\n",
				 function );

				memory_free(
				 *hash_string );

				*hash_string      = NULL;
				*hash_string_size = 0;

				return( -1 );
			}
			string_offset += character_count;
		}
	}
	character_count = string_snprintf(
	                   &( ( *hash_string )[ string_offset ] ),
	                   ( *hash_string_size - string_offset ),
	                   _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "\n\n" ),
	                   xml_close_tag_xhash );

	if( character_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set hash string.\n",
		 function );

		memory_free(
		 *hash_string );

		*hash_string      = NULL;
		*hash_string_size = 0;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	( *hash_string )[ *hash_string_size - 1 ] = 0;

	return( 1 );
}

/* Generate an EWFX xhash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_xhash_string_ewfx(
     libewf_values_table_t *hash_values,
     uint8_t **xhash,
     size_t *xhash_size )
{
	character_t *xml_hash_string = NULL;
	static char *function        = "libewf_hash_values_generate_xhash_string_ewfx";
	size_t xml_hash_string_size  = 0;
	int result                   = 0;

	if( libewf_hash_values_generate_hash_string_xml(
	     hash_values,
	     &xml_hash_string,
	     &xml_hash_string_size ) != 1 )
	{
		notify_warning_printf( "%s: unable to create XML hash string.\n",
		 function );

		return( -1 );
	}
	result = libewf_hash_values_convert_hash_string_to_xhash(
	          xml_hash_string,
	          xml_hash_string_size,
	          xhash,
	          xhash_size );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create xhash.\n",
		 function );
	}
	memory_free(
	 xml_hash_string );

	return( result );
}


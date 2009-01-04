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
#include <memory.h>
#include <notify.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_hash_values.h"
#include "libewf_libuna.h"
#include "libewf_string.h"

#include "ewf_definitions.h"
#include "ewf_digest_hash.h"

/* Initializes the hash values
 * Returns 1 if successful or -1 on error
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
	if( libewf_values_table_set_identifier(
	     hash_values,
	     LIBEWF_HASH_VALUES_INDEX_MD5,
	     _LIBEWF_CHARACTER_T_STRING( "MD5" ),
	     3 ) != 1 )
	{
		notify_warning_printf( "%s: unable to set MD5 identifier.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parses an MD5 hash for its value
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_md5_hash(
     libewf_values_table_t **hash_values,
     uint8_t *md5_hash,
     size_t md5_hash_size )
{
	libewf_character_t md5_hash_string[ 33 ];

	static char *function           = "libewf_hash_values_parse_md5_hash";
	size_t md5_hash_iterator        = 0;
	size_t md5_hash_string_iterator = 0;
	int result                      = 0;
	uint8_t md5_digit               = 0;

	if( hash_values == NULL )
	{
		notify_warning_printf( "%s: invalid hash values.\n",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		notify_warning_printf( "%s: invalid MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( md5_hash_size < EWF_DIGEST_HASH_SIZE_MD5 )
	{
		notify_warning_printf( "%s: invalid MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( *hash_values == NULL )
	{
		if( libewf_values_table_initialize(
		     hash_values,
		     LIBEWF_HASH_VALUES_DEFAULT_AMOUNT ) != 1 )
		{
			notify_warning_printf( "%s: unable to create hash values.\n",
			 function );

			return( -1 );
		}
		if( libewf_hash_values_initialize(
		     *hash_values ) != 1 )
		{
			notify_warning_printf( "%s: unable to initialize the hash values.\n",
			 function );

			return( -1 );
		}
	}
	result = libewf_values_table_get_value(
	          *hash_values,
	          _LIBEWF_CHARACTER_T_STRING( "MD5" ),
	          3,
	          md5_hash_string,
	          33 );

	if( result == -1 )
	{
		notify_warning_printf( "%s: unable to determine if MD5 hash value was set.\n",
		 function );

		return( -1 );
	}
	/* The MD5 hash values has been set
	 */
	else if( result == 1 )
	{
		return( 1 );
	}
	for( md5_hash_iterator = 0; md5_hash_iterator < md5_hash_size; md5_hash_iterator++ )
	{
		md5_digit = md5_hash[ md5_hash_iterator ] / 16;

		if( md5_digit <= 9 )
		{
			md5_hash_string[ md5_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) '0' + md5_digit );
		}
		else
		{
			md5_hash_string[ md5_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) 'a' + ( md5_digit - 10 ) );
		}
		md5_digit = md5_hash[ md5_hash_iterator ] % 16;

		if( md5_digit <= 9 )
		{
			md5_hash_string[ md5_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) '0' + md5_digit );
		}
		else
		{
			md5_hash_string[ md5_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) 'a' + ( md5_digit - 10 ) );
		}
	}
	result = libewf_values_table_set_value(
		  *hash_values,
		  _LIBEWF_CHARACTER_T_STRING( "MD5" ),
		  3,
		  md5_hash_string,
		  32 );

	if( result != 1 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: unable to set value with identifier: MD5.\n",
		 function );
#endif
	}
	return( result );
}

/* Parse a XML hash string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_hash_string_xml(
     libewf_values_table_t **hash_values,
     libewf_character_t *hash_string_xml,
     size_t hash_string_xml_size )
{
	libewf_character_t **lines          = NULL;
	libewf_character_t *open_tag_start  = NULL;
	libewf_character_t *open_tag_end    = NULL;
	libewf_character_t *close_tag_start = NULL;
	libewf_character_t *close_tag_end   = NULL;
	static char *function               = "libewf_hash_values_parse_hash_string_xml";
	size_t amount_of_lines              = 0;
	size_t identifier_length            = 0;
	size_t line_iterator                = 0;
	size_t string_length                = 0;
	size_t value_length                 = 0;

	if( hash_string_xml == NULL )
	{
		notify_warning_printf( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	if( libewf_values_table_initialize(
	     hash_values,
	     LIBEWF_HASH_VALUES_DEFAULT_AMOUNT ) != 1 )
	{
		notify_warning_printf( "%s: unable to create hash values.\n",
		 function );

		return( -1 );
	}
	if( libewf_hash_values_initialize(
	     *hash_values ) != 1 )
	{
		notify_warning_printf( "%s: unable to initialize the hash values.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_split(
	     hash_string_xml,
	     hash_string_xml_size,
	     (libewf_character_t) '\n',
	     &lines,
	     &amount_of_lines ) != 1 )
	{
		notify_warning_printf( "%s: unable to split hash string into lines.\n",
		 function );

		return( -1 );
	}
	for( line_iterator = 0; line_iterator < amount_of_lines; line_iterator++ )
	{
		if( ( lines[ line_iterator ] == NULL )
		 || ( lines[ line_iterator ] == (libewf_character_t *) _LIBEWF_CHARACTER_T_STRING( "" ) ) )
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
		                  (libewf_character_t) '<',
		                  string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = string_search(
		                lines[ line_iterator ],
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
		string_length -= (size_t) ( open_tag_end - lines[ line_iterator ] );

		/* Ignore lines only containing a single tag
		 */
		if( string_length <= 1 )
		{
			continue;
		}
		close_tag_start = string_search_reverse(
		                   &open_tag_end[ 1 ],
		                   (libewf_character_t) '<',
		                   string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = string_search_reverse(
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

		/* Make sure the identifier string will be terminated
		 */
		*open_tag_end = 0;

		if( libewf_values_table_set_value(
		     *hash_values,
		     &open_tag_start[ 1 ],
		     identifier_length,
		     &open_tag_end[ 1 ],
		     value_length ) != 1 )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: unable to set value with identifier: %" PRIs_LIBEWF ".\n",
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
	libewf_character_t *xml_hash_string = NULL;
	libuna_error_t *error               = NULL;
	static char *function               = "libewf_hash_values_parse_xhash";
	size_t xml_hash_string_size         = 0;
	int result                          = 0;

	if( xhash == NULL )
	{
		notify_warning_printf( "%s: invalid xhash.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_size_from_utf8_stream(
	     xhash,
	     xhash_size,
	     &xml_hash_string_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine XML hash string size.\n",
		 function );

		libuna_error_free(
		 &error );

		return( -1 );
	}
	xml_hash_string = (libewf_character_t *) memory_allocate(
	                                          sizeof( libewf_character_t ) * xml_hash_string_size );

	if( xml_hash_string == NULL )
	{
		notify_warning_printf( "%s: unable to create XML hash string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_utf8_stream(
	     xml_hash_string,
	     xml_hash_string_size,
	     xhash,
	     xhash_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to XML set hash string.\n",
		 function );

		libuna_error_free(
		 &error );
		memory_free(
		 xml_hash_string );

		return( -1 );
	}
	result = libewf_hash_values_parse_hash_string_xml(
	          hash_values,
	          xml_hash_string,
	          xml_hash_string_size );

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
     libewf_character_t *hash_string,
     size_t hash_string_size,
     uint8_t **xhash,
     size_t *xhash_size )
{
	libuna_error_t *error = NULL;
	static char *function = "libewf_hash_values_convert_hash_string_to_xhash";

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
	if( utf8_stream_size_from_libewf_string(
	     hash_string,
	     hash_string_size,
	     xhash_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to determine xhash size.\n",
		 function );

		libuna_error_free(
		 &error );

		return( -1 );
	}
	*xhash = (uint8_t *) memory_allocate(
	                      sizeof( uint8_t ) * *xhash_size );

	if( *xhash == NULL )
	{
		notify_warning_printf( "%s: unable to create xhash.\n",
		 function );

		*xhash_size = 0;

		return( -1 );
	}
	if( utf8_stream_copy_from_libewf_string(
	     *xhash,
	     *xhash_size,
	     hash_string,
	     hash_string_size,
	     &error ) != 1 )
	{
		notify_warning_printf( "%s: unable to set xhash.\n",
		 function );

		libuna_error_free(
		 &error );
		memory_free(
		 *xhash );

		*xhash      = NULL;
		*xhash_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a hash format in XML
 * Sets hash sting and hash string size
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_hash_string_xml(
     libewf_values_table_t *hash_values,
     libewf_character_t **hash_string,
     size_t *hash_string_size )
{
	libewf_character_t *xml_head            = _LIBEWF_CHARACTER_T_STRING( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
	libewf_character_t *xml_open_tag_xhash  = _LIBEWF_CHARACTER_T_STRING( "<xhash>" );
	libewf_character_t *xml_close_tag_xhash = _LIBEWF_CHARACTER_T_STRING( "</xhash>" );
	static char *function                   = "libewf_hash_values_generate_hash_string_xml";
	int character_count                     = 0;
	int string_offset                       = 0;
	int values_table_iterator               = 0;

	if( hash_values == NULL )
	{
		notify_warning_printf( "%s: invalid hash values.\n",
		 function );

		return( -1 );
	}
	if( hash_values->amount_of_values < 0 )
	{
		notify_warning_printf( "%s: invalid hash values - amount of value less than zero.\n",
		 function );

		return( -1 );
	}
	if( hash_values->identifier == NULL )
	{
		notify_warning_printf( "%s: invalid hash values - missing identifiers.\n",
		 function );

		return( -1 );
	}
	if( hash_values->identifier_length == NULL )
	{
		notify_warning_printf( "%s: invalid hash values - missing identifier lengths.\n",
		 function );

		return( -1 );
	}
	if( hash_values->value == NULL )
	{
		notify_warning_printf( "%s: invalid hash values - missing values.\n",
		 function );

		return( -1 );
	}
	if( hash_values->value_length == NULL )
	{
		notify_warning_printf( "%s: invalid hash values - missing value lengths.\n",
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

	for( values_table_iterator = 0; values_table_iterator < hash_values->amount_of_values; values_table_iterator++ )
	{
		if( ( hash_values->identifier[ values_table_iterator ] == NULL )
		 || ( hash_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			notify_warning_printf( "%s: invalid hash value - missing identifier.\n",
			 function );

			continue;
		}
		if( ( hash_values->value[ values_table_iterator ] != NULL )
		 && ( hash_values->value_length[ values_table_iterator ] > 0 ) )
		{
			/* Add space for a leading tab, <identifier>value</identifier> and an end of line
			 */
			*hash_string_size += ( 2 * hash_values->identifier_length[ values_table_iterator ] )
			                   + hash_values->value_length[ values_table_iterator ] + 7;
		}
	}
	/* allow for an empty line and an end of string
	 */
	*hash_string_size += 2;

	*hash_string = (libewf_character_t *) memory_allocate(
                                               sizeof( libewf_character_t ) * *hash_string_size );

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
	                   _LIBEWF_CHARACTER_T_STRING( "%" ) _LIBEWF_CHARACTER_T_STRING( PRIs_LIBEWF ) _LIBEWF_CHARACTER_T_STRING( "\n%" )
	                   _LIBEWF_CHARACTER_T_STRING( PRIs_LIBEWF ) _LIBEWF_CHARACTER_T_STRING( "\n" ),
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

	for( values_table_iterator = 0; values_table_iterator < hash_values->amount_of_values; values_table_iterator++ )
	{
		if( ( hash_values->identifier[ values_table_iterator ] == NULL )
		 || ( hash_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			notify_warning_printf( "%s: invalid hash value - missing identifier.\n",
			 function );

			continue;
		}
		if( ( hash_values->value[ values_table_iterator ] != NULL )
		 && ( hash_values->value_length[ values_table_iterator ] > 0 ) )
		{
			character_count = string_snprintf(
			                   &( ( *hash_string) [ string_offset ] ),
			                   ( *hash_string_size - string_offset ),
			                   _LIBEWF_CHARACTER_T_STRING( "\t<%" ) _LIBEWF_CHARACTER_T_STRING( PRIs_LIBEWF ) _LIBEWF_CHARACTER_T_STRING( ">%" )
			                   _LIBEWF_CHARACTER_T_STRING( PRIs_LIBEWF ) _LIBEWF_CHARACTER_T_STRING( "</%" ) _LIBEWF_CHARACTER_T_STRING( PRIs_LIBEWF )
			                   _LIBEWF_CHARACTER_T_STRING( ">\n" ),
			                   hash_values->identifier[ values_table_iterator ],
			                   hash_values->value[ values_table_iterator ],
			                   hash_values->identifier[ values_table_iterator ] );

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
	                   _LIBEWF_CHARACTER_T_STRING( "%" ) _LIBEWF_CHARACTER_T_STRING( PRIs_LIBEWF ) _LIBEWF_CHARACTER_T_STRING( "\n\n" ),
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

/* Generate an MD5 xhash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_md5_hash(
     libewf_values_table_t *hash_values,
     uint8_t *md5_hash,
     size_t md5_hash_size,
     uint8_t *md5_hash_set )
{
	libewf_character_t md5_hash_string[ 33 ];

	static char *function           = "libewf_hash_values_generate_md5_hash";
	size_t md5_hash_iterator        = 0;
	size_t md5_hash_string_iterator = 0;
	int result                      = 0;
	uint8_t md5_digit               = 0;

	if( hash_values == NULL )
	{
		notify_warning_printf( "%s: invalid hash values.\n",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		notify_warning_printf( "%s: invalid MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( md5_hash_size < EWF_DIGEST_HASH_SIZE_MD5 )
	{
		notify_warning_printf( "%s: invalid MD5 hash.\n",
		 function );

		return( -1 );
	}
	if( md5_hash_set == NULL )
	{
		notify_warning_printf( "%s: invalid MD5 hash set.\n",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_value(
	          hash_values,
	          _LIBEWF_CHARACTER_T_STRING( "MD5" ),
	          3,
	          md5_hash_string,
	          33 );

	if( result == -1 )
	{
		notify_warning_printf( "%s: unable to determine if MD5 hash value was set.\n",
		 function );

		return( -1 );
	}
	/* No need to generate the MD5 hash
	 */
	else if( result == 0 )
	{
		*md5_hash_set = 0;

		return( 1 );
	}
	for( md5_hash_string_iterator = 0; md5_hash_string_iterator < 33; md5_hash_string_iterator++ )
	{
		if( ( md5_hash_string[ md5_hash_string_iterator ] >= (libewf_character_t) '0' )
		 && ( md5_hash_string[ md5_hash_string_iterator ] <= (libewf_character_t) '9' ) )
		{
			md5_digit = (uint8_t) ( md5_hash_string[ md5_hash_string_iterator ] - (libewf_character_t) '0' );
		}
		else if( ( md5_hash_string[ md5_hash_string_iterator ] >= (libewf_character_t) 'A' )
		      && ( md5_hash_string[ md5_hash_string_iterator ] <= (libewf_character_t) 'F' ) )
		{
			md5_digit = 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_iterator ] - (libewf_character_t) 'A' );
		}
		else if( ( md5_hash_string[ md5_hash_string_iterator ] >= (libewf_character_t) 'a' )
		      && ( md5_hash_string[ md5_hash_string_iterator ] <= (libewf_character_t) 'f' ) )
		{
			md5_digit = 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_iterator ] - (libewf_character_t) 'a' );
		}
		md5_hash_string_iterator++;

		md5_digit *= 16;

		if( ( md5_hash_string[ md5_hash_string_iterator ] >= (libewf_character_t) '0' )
		 && ( md5_hash_string[ md5_hash_string_iterator ] <= (libewf_character_t) '9' ) )
		{
			md5_digit += (uint8_t) ( md5_hash_string[ md5_hash_string_iterator ] - (libewf_character_t) '0' );
		}
		else if( ( md5_hash_string[ md5_hash_string_iterator ] >= (libewf_character_t) 'A' )
		      && ( md5_hash_string[ md5_hash_string_iterator ] <= (libewf_character_t) 'F' ) )
		{
			md5_digit += 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_iterator ] - (libewf_character_t) 'A' );
		}
		else if( ( md5_hash_string[ md5_hash_string_iterator ] >= (libewf_character_t) 'a' )
		      && ( md5_hash_string[ md5_hash_string_iterator ] <= (libewf_character_t) 'f' ) )
		{
			md5_digit += 10 + (uint8_t) ( md5_hash_string[ md5_hash_string_iterator ] - (libewf_character_t) 'a' );
		}
		md5_hash[ md5_hash_iterator++ ] = md5_digit;
	}
	*md5_hash_set = 1;

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
	libewf_character_t *xml_hash_string = NULL;
	static char *function               = "libewf_hash_values_generate_xhash_string_ewfx";
	size_t xml_hash_string_size         = 0;
	int result                          = 0;

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


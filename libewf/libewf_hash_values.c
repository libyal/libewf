/*
 * Hash values functions
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
#include <types.h>

#include <liberror.h>

#include "libewf_definitions.h"
#include "libewf_hash_values.h"
#include "libewf_libuna.h"
#include "libewf_string.h"

#include "ewf_definitions.h"

/* Initializes the hash values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_initialize(
     libewf_values_table_t **hash_values,
     liberror_error_t **error )
{
	static char *function = "libewf_hash_values_initialize";

	if( libewf_values_table_initialize(
	     hash_values,
	     LIBEWF_HASH_VALUES_DEFAULT_AMOUNT,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parses an MD5 hash for its value
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_md5_hash(
     libewf_values_table_t *hash_values,
     uint8_t *md5_hash,
     size_t md5_hash_size,
     liberror_error_t **error )
{
	libewf_character_t md5_hash_string[ 33 ];

	static char *function           = "libewf_hash_values_parse_md5_hash";
	size_t md5_hash_iterator        = 0;
	size_t md5_hash_string_iterator = 0;
	int result                      = 0;
	uint8_t md5_digit               = 0;

	if( hash_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( md5_hash_size < 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_value(
	          hash_values,
	          _LIBEWF_STRING( "MD5" ),
	          3,
	          md5_hash_string,
	          33,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MD5 hash value was set.",
		 function );

		return( -1 );
	}
	/* The MD5 hash values has been set
	 */
	else if( result == 1 )
	{
		return( 1 );
	}
	for( md5_hash_iterator = 0;
	     md5_hash_iterator < md5_hash_size;
	     md5_hash_iterator++ )
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
		  hash_values,
		  _LIBEWF_STRING( "MD5" ),
		  3,
		  md5_hash_string,
		  32,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value with identifier: MD5.",
		 function );
	}
	return( result );
}

/* Parses an SHA1 hash for its value
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_sha1_hash(
     libewf_values_table_t *hash_values,
     uint8_t *sha1_hash,
     size_t sha1_hash_size,
     liberror_error_t **error )
{
	libewf_character_t sha1_hash_string[ 41 ];

	static char *function            = "libewf_hash_values_parse_sha1_hash";
	size_t sha1_hash_iterator        = 0;
	size_t sha1_hash_string_iterator = 0;
	int result                       = 0;
	uint8_t sha1_digit               = 0;

	if( hash_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( sha1_hash_size < 20 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_value(
	          hash_values,
	          _LIBEWF_STRING( "SHA1" ),
	          4,
	          sha1_hash_string,
	          41,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if SHA1 hash value was set.",
		 function );

		return( -1 );
	}
	/* The MD5 hash values has been set
	 */
	else if( result == 1 )
	{
		return( 1 );
	}
	for( sha1_hash_iterator = 0;
	     sha1_hash_iterator < sha1_hash_size;
	     sha1_hash_iterator++ )
	{
		sha1_digit = sha1_hash[ sha1_hash_iterator ] / 16;

		if( sha1_digit <= 9 )
		{
			sha1_hash_string[ sha1_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) '0' + sha1_digit );
		}
		else
		{
			sha1_hash_string[ sha1_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) 'a' + ( sha1_digit - 10 ) );
		}
		sha1_digit = sha1_hash[ sha1_hash_iterator ] % 16;

		if( sha1_digit <= 9 )
		{
			sha1_hash_string[ sha1_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) '0' + sha1_digit );
		}
		else
		{
			sha1_hash_string[ sha1_hash_string_iterator++ ] = (libewf_character_t) ( (uint8_t) 'a' + ( sha1_digit - 10 ) );
		}
	}
	result = libewf_values_table_set_value(
		  hash_values,
		  _LIBEWF_STRING( "SHA1" ),
		  4,
		  sha1_hash_string,
		  40,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value with identifier: SHA1.",
		 function );
	}
	return( result );
}

/* Parse a XML hash string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_hash_string_xml(
     libewf_values_table_t *hash_values,
     libewf_character_t *hash_string_xml,
     size_t hash_string_xml_size,
     liberror_error_t **error )
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash string.",
		 function );

		return( -1 );
	}
	if( libewf_string_split(
	     hash_string_xml,
	     hash_string_xml_size,
	     (libewf_character_t) '\n',
	     &lines,
	     &amount_of_lines,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split hash string into lines.",
		 function );

		return( -1 );
	}
	for( line_iterator = 0;
	     line_iterator < amount_of_lines;
	     line_iterator++ )
	{
		if( ( lines[ line_iterator ] == NULL )
		 || ( lines[ line_iterator ] == (libewf_character_t *) _LIBEWF_STRING( "" ) ) )
		{
			continue;
		}
		string_length = libewf_string_length(
		                 lines[ line_iterator ] );

		/* Ignore empty lines
		 */
		if( string_length == 0 )
		{
			continue;
		}
		open_tag_start = libewf_string_search(
		                  lines[ line_iterator ],
		                  '<',
		                  string_length );

		/* Ignore lines without an open tag
		 */
		if( open_tag_start == NULL )
		{
			continue;
		}
		open_tag_end = libewf_string_search(
		                lines[ line_iterator ],
		                '>',
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
		close_tag_start = libewf_string_search_reverse(
		                   &open_tag_end[ 1 ],
		                   '<',
		                   string_length );

		/* Ignore lines without a close tag
		 */
		if( close_tag_start == NULL )
		{
			continue;
		}
		close_tag_end = libewf_string_search_reverse(
		                 &open_tag_end[ 1 ],
		                 '>',
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
		     hash_values,
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

/* Parse an EWF xhash for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_xhash(
     libewf_values_table_t *hash_values,
     uint8_t *xhash,
     size_t xhash_size,
     liberror_error_t **error )
{
	libewf_character_t *xml_hash_string = NULL;
	static char *function               = "libewf_hash_values_parse_xhash";
	size_t xml_hash_string_size         = 0;
	int result                          = 0;

	if( xhash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xhash.",
		 function );

		return( -1 );
	}
	if( libewf_string_size_from_utf8_stream(
	     xhash,
	     xhash_size,
	     &xml_hash_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine XML hash string size.",
		 function );

		return( -1 );
	}
	xml_hash_string = (libewf_character_t *) memory_allocate(
	                                          sizeof( libewf_character_t ) * xml_hash_string_size );

	if( xml_hash_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to create XML hash string.",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_utf8_stream(
	     xml_hash_string,
	     xml_hash_string_size,
	     xhash,
	     xhash_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to XML set hash string.",
		 function );

		memory_free(
		 xml_hash_string );

		return( -1 );
	}
	result = libewf_hash_values_parse_hash_string_xml(
	          hash_values,
	          xml_hash_string,
	          xml_hash_string_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
	         LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse XML hash string.",
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
     size_t *xhash_size,
     liberror_error_t **error )
{
	static char *function = "libewf_hash_values_convert_hash_string_to_xhash";

	if( hash_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash string.",
		 function );

		return( -1 );
	}
	if( ( hash_string_size == 0 )
	 || ( hash_string_size > (size_t) SSIZE_MAX ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash string size.",
		 function );

		return( -1 );
	}
	if( xhash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xhash.",
		 function );

		return( -1 );
	}
	if( xhash_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid xhash size.",
		 function );

		return( -1 );
	}
	if( *xhash != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: xhash already created.",
		 function );

		return( -1 );
	}
	if( utf8_stream_size_from_libewf_string(
	     hash_string,
	     hash_string_size,
	     xhash_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine xhash size.",
		 function );

		return( -1 );
	}
	*xhash = (uint8_t *) memory_allocate(
	                      sizeof( uint8_t ) * *xhash_size );

	if( *xhash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to create xhash.",
		 function );

		*xhash_size = 0;

		return( -1 );
	}
	if( utf8_stream_copy_from_libewf_string(
	     *xhash,
	     *xhash_size,
	     hash_string,
	     hash_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set xhash.",
		 function );

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
     size_t *hash_string_size,
     liberror_error_t **error )
{
	libewf_character_t *xml_head            = _LIBEWF_STRING( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
	libewf_character_t *xml_open_tag_xhash  = _LIBEWF_STRING( "<xhash>" );
	libewf_character_t *xml_close_tag_xhash = _LIBEWF_STRING( "</xhash>" );
	static char *function                   = "libewf_hash_values_generate_hash_string_xml";
	int print_count                         = 0;
	int string_offset                       = 0;
	int values_table_iterator               = 0;

	if( hash_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( hash_values->amount_of_values < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid hash values - amount of value less than zero.",
		 function );

		return( -1 );
	}
	if( hash_values->identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values - missing identifiers.",
		 function );

		return( -1 );
	}
	if( hash_values->identifier_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values - missing identifier lengths.",
		 function );

		return( -1 );
	}
	if( hash_values->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values - missing values.",
		 function );

		return( -1 );
	}
	if( hash_values->value_length == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values - missing value lengths.",
		 function );

		return( -1 );
	}
	if( hash_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash string.",
		 function );

		return( -1 );
	}
	if( *hash_string != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: hash string already created.",
		 function );

		return( -1 );
	}
	if( hash_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash string size.",
		 function );

		return( -1 );
	}
	/* Add space for the XML data and an end of line
	 */
	*hash_string_size = 1 + libewf_string_length(
	                         xml_head );

	*hash_string_size += 1 + libewf_string_length(
	                          xml_open_tag_xhash );

	*hash_string_size += 1 + libewf_string_length(
	                          xml_close_tag_xhash );

	for( values_table_iterator = 0;
	     values_table_iterator < hash_values->amount_of_values;
	     values_table_iterator++ )
	{
		if( ( hash_values->identifier[ values_table_iterator ] == NULL )
		 || ( hash_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid hash value - missing identifier.",
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unable to create hash string.",
		 function );

		*hash_string_size = 0;

		return( -1 );
	}
	print_count = libewf_string_snprintf(
	               *hash_string,
	               *hash_string_size,
	               "%s\n%s\n",
	               (char *) xml_head,
	               (char *) xml_open_tag_xhash );

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > *hash_string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set hash string.",
		 function );

		memory_free(
		 *hash_string );

		*hash_string      = NULL;
		*hash_string_size = 0;

		return( -1 );
	}
	string_offset = print_count;

	for( values_table_iterator = 0;
	     values_table_iterator < hash_values->amount_of_values;
	     values_table_iterator++ )
	{
		if( ( hash_values->identifier[ values_table_iterator ] == NULL )
		 || ( hash_values->identifier_length[ values_table_iterator ] == 0 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid hash value - missing identifier.",
			 function );

			continue;
		}
		if( ( hash_values->value[ values_table_iterator ] != NULL )
		 && ( hash_values->value_length[ values_table_iterator ] > 0 ) )
		{
			print_count = libewf_string_snprintf(
			               &( ( *hash_string) [ string_offset ] ),
			               ( *hash_string_size - string_offset ),
			               "\t<%s>%s</%s>\n",
			               (char *) hash_values->identifier[ values_table_iterator ],
			               (char *) hash_values->value[ values_table_iterator ],
			               (char *) hash_values->identifier[ values_table_iterator ] );

			if( ( print_count <= -1 )
			 || ( (size_t) print_count > ( *hash_string_size - string_offset ) ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set hash string.",
				 function );

				memory_free(
				 *hash_string );

				*hash_string      = NULL;
				*hash_string_size = 0;

				return( -1 );
			}
			string_offset += print_count;
		}
	}
	print_count = libewf_string_snprintf(
	               &( ( *hash_string )[ string_offset ] ),
	               ( *hash_string_size - string_offset ),
	               "%s\n\n",
	               (char *) xml_close_tag_xhash );

	if( ( print_count <= -1 )
	 || ( (size_t) print_count > ( *hash_string_size - string_offset ) ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set hash string.",
		 function );

		memory_free(
		 *hash_string );

		*hash_string      = NULL;
		*hash_string_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate an MD5 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_md5_hash(
     libewf_values_table_t *hash_values,
     uint8_t *md5_hash,
     size_t md5_hash_size,
     uint8_t *md5_hash_set,
     liberror_error_t **error )
{
	libewf_character_t md5_hash_string[ 33 ];

	static char *function           = "libewf_hash_values_generate_md5_hash";
	size_t md5_hash_iterator        = 0;
	size_t md5_hash_string_iterator = 0;
	int result                      = 0;
	uint8_t md5_digit               = 0;

	if( hash_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( md5_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash.",
		 function );

		return( -1 );
	}
	if( md5_hash_size < 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: MD5 hash too small.",
		 function );

		return( -1 );
	}
	if( md5_hash_set == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MD5 hash set.",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_value(
	          hash_values,
	          _LIBEWF_STRING( "MD5" ),
	          3,
	          md5_hash_string,
	          33,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if MD5 hash value was set.",
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
	for( md5_hash_string_iterator = 0;
	     md5_hash_string_iterator < 33;
	     md5_hash_string_iterator++ )
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

/* Generate an SHA1 hash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_sha1_hash(
     libewf_values_table_t *hash_values,
     uint8_t *sha1_hash,
     size_t sha1_hash_size,
     uint8_t *sha1_hash_set,
     liberror_error_t **error )
{
	libewf_character_t sha1_hash_string[ 41 ];

	static char *function            = "libewf_hash_values_generate_sha1_hash";
	size_t sha1_hash_iterator        = 0;
	size_t sha1_hash_string_iterator = 0;
	int result                       = 0;
	uint8_t sha1_digit               = 0;

	if( hash_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hash values.",
		 function );

		return( -1 );
	}
	if( sha1_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash.",
		 function );

		return( -1 );
	}
	if( sha1_hash_size < 20 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: SHA1 hash too small.",
		 function );

		return( -1 );
	}
	if( sha1_hash_set == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid SHA1 hash set.",
		 function );

		return( -1 );
	}
	result = libewf_values_table_get_value(
	          hash_values,
	          _LIBEWF_STRING( "SHA1" ),
	          4,
	          sha1_hash_string,
	          41,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if SHA1 hash value was set.",
		 function );

		return( -1 );
	}
	/* No need to generate the SHA1 hash
	 */
	else if( result == 0 )
	{
		*sha1_hash_set = 0;

		return( 1 );
	}
	for( sha1_hash_string_iterator = 0;
	     sha1_hash_string_iterator < 41;
	     sha1_hash_string_iterator++ )
	{
		if( ( sha1_hash_string[ sha1_hash_string_iterator ] >= (libewf_character_t) '0' )
		 && ( sha1_hash_string[ sha1_hash_string_iterator ] <= (libewf_character_t) '9' ) )
		{
			sha1_digit = (uint8_t) ( sha1_hash_string[ sha1_hash_string_iterator ] - (libewf_character_t) '0' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_iterator ] >= (libewf_character_t) 'A' )
		      && ( sha1_hash_string[ sha1_hash_string_iterator ] <= (libewf_character_t) 'F' ) )
		{
			sha1_digit = 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_iterator ] - (libewf_character_t) 'A' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_iterator ] >= (libewf_character_t) 'a' )
		      && ( sha1_hash_string[ sha1_hash_string_iterator ] <= (libewf_character_t) 'f' ) )
		{
			sha1_digit = 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_iterator ] - (libewf_character_t) 'a' );
		}
		sha1_hash_string_iterator++;

		sha1_digit *= 16;

		if( ( sha1_hash_string[ sha1_hash_string_iterator ] >= (libewf_character_t) '0' )
		 && ( sha1_hash_string[ sha1_hash_string_iterator ] <= (libewf_character_t) '9' ) )
		{
			sha1_digit += (uint8_t) ( sha1_hash_string[ sha1_hash_string_iterator ] - (libewf_character_t) '0' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_iterator ] >= (libewf_character_t) 'A' )
		      && ( sha1_hash_string[ sha1_hash_string_iterator ] <= (libewf_character_t) 'F' ) )
		{
			sha1_digit += 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_iterator ] - (libewf_character_t) 'A' );
		}
		else if( ( sha1_hash_string[ sha1_hash_string_iterator ] >= (libewf_character_t) 'a' )
		      && ( sha1_hash_string[ sha1_hash_string_iterator ] <= (libewf_character_t) 'f' ) )
		{
			sha1_digit += 10 + (uint8_t) ( sha1_hash_string[ sha1_hash_string_iterator ] - (libewf_character_t) 'a' );
		}
		sha1_hash[ sha1_hash_iterator++ ] = sha1_digit;
	}
	*sha1_hash_set = 1;

	return( 1 );
}

/* Generate an EWFX xhash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_xhash_string_ewfx(
     libewf_values_table_t *hash_values,
     uint8_t **xhash,
     size_t *xhash_size,
     liberror_error_t **error )
{
	libewf_character_t *xml_hash_string = NULL;
	static char *function               = "libewf_hash_values_generate_xhash_string_ewfx";
	size_t xml_hash_string_size         = 0;
	int result                          = 0;

	if( libewf_hash_values_generate_hash_string_xml(
	     hash_values,
	     &xml_hash_string,
	     &xml_hash_string_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create XML hash string.",
		 function );

		return( -1 );
	}
	result = libewf_hash_values_convert_hash_string_to_xhash(
	          xml_hash_string,
	          xml_hash_string_size,
	          xhash,
	          xhash_size,
	          error );

	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to create xhash.",
		 function );
	}
	memory_free(
	 xml_hash_string );

	return( result );
}


/*
 * libewf hash values
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

/* Parse a xml hash string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_hash_string_xml(
     libewf_values_table_t **hash_values,
     character_t *hash_string_xml,
     size_t length )
{
	character_t **lines          = NULL;
	character_t *open_tag_start  = NULL;
	character_t *open_tag_end    = NULL;
	character_t *close_tag_start = NULL;
	character_t *close_tag_end   = NULL;
	static char *function        = "libewf_hash_values_parse_hash_string_xml";
	size_t string_length         = 0;
	uint32_t line_count          = 0;
	uint32_t iterator            = 0;

	if( hash_string_xml == NULL )
	{
		notify_warning_printf( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	lines = libewf_string_split(
	         (character_t *) hash_string_xml,
	         length,
	         (character_t) '\n',
	         &line_count );

	if( lines == NULL )
	{
		notify_warning_printf( "%s: unable to split lines in hash string.\n",
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
		 line_count );

		return( -1 );
	}
	if( libewf_hash_values_initialize(
	     *hash_values ) != 1 )
	{
		notify_warning_printf( "%s: unable to initialize the hash values.\n",
		 function );

		libewf_string_split_values_free(
		 lines,
		 line_count );

		return( -1 );
	}
	for( iterator = 0; iterator < line_count; iterator++ )
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
		/* Ignore the first part of the xml string
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
		/* Ignore the second part of the xml string
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
	libewf_string_split_values_free(
	 lines,
	 line_count );

	return( 1 );
}

/* Parse an EWF xhash for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_xhash(
     libewf_values_table_t **hash_values,
     ewf_char_t *xhash,
     size_t size )
{
	character_t *xml_hash_string = NULL;
	static char *function        = "libewf_hash_values_parse_xhash";
	int result                   = 0;

	if( xhash == NULL )
	{
		notify_warning_printf( "%s: invalid xhash.\n",
		 function );

		return( -1 );
	}
	xml_hash_string = (character_t *) memory_allocate(
	                                   sizeof( character_t ) * ( size + 1 ) );

	if( xml_hash_string == NULL )
	{
		notify_warning_printf( "%s: unable to create XML hash string.\n",
		 function );

		return( -1 );
	}
	if( string_copy_from_char(
	     xml_hash_string,
	     (char *) xhash,
	     size ) != 1 )
	{
		notify_warning_printf( "%s: unable to copy xhash to xml hash string.\n",
		 function );

		memory_free(
		 xml_hash_string );

		return( -1 );
	}
	result = libewf_hash_values_parse_hash_string_xml(
	          hash_values,
	          xml_hash_string,
	          size );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to parse xml hash string.\n",
		 function );
	}
	memory_free(
	 xml_hash_string );

	return( result );
}

/* Converts a hash string into a hash
 * Sets the hash and hash length
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_convert_hash_string_to_hash(
     character_t *hash_string,
     size_t hash_string_length,
     ewf_char_t **hash,
     size_t *hash_length )
{
	static char *function = "libewf_hash_values_convert_hash_string_to_hash";

	if( hash_string == NULL )
	{
		notify_warning_printf( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	if( ( hash_string_length == 0 )
	 || ( hash_string_length > (size_t) SSIZE_MAX ) )
	{
		notify_warning_printf( "%s: invalid hash string length.\n",
		 function );

		return( -1 );
	}
	if( hash == NULL )
	{
		notify_warning_printf( "%s: invalid hash.\n",
		 function );

		return( -1 );
	}
	if( hash_length == NULL )
	{
		notify_warning_printf( "%s: invalid hash length.\n",
		 function );

		return( -1 );
	}
	if( *hash != NULL )
	{
		notify_warning_printf( "%s: hash already created .\n",
		 function );

		return( -1 );
	}
	*hash_length = hash_string_length;

	*hash = (ewf_char_t *) memory_allocate(
	                        sizeof( ewf_char_t ) * *hash_length );

	if( *hash == NULL )
	{
		notify_warning_printf( "%s: unable to create hash.\n",
		 function );

		*hash_length = 0;

		return( -1 );
	}
	if( string_copy_to_char(
	     (char *) *hash,
	     hash_string,
	     hash_string_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to set hash.\n",
		 function );

		memory_free(
		 *hash );

		*hash        = NULL;
		*hash_length = 0;

		return( -1 );
	}
	return( 1 );
}

/* Generate a hash format in XML
 * Sets hash sting and hash string length
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_hash_string_xml(
     libewf_values_table_t *hash_values,
     character_t **hash_string,
     size_t *hash_string_length )
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
	if( hash_string_length == NULL )
	{
		notify_warning_printf( "%s: invalid hash string length.\n",
		 function );

		return( -1 );
	}
	/* Add space for the xml data and an end of line
	 */
	*hash_string_length = 1 + string_length(
	                           xml_head );

	*hash_string_length += 1 + string_length(
	                            xml_open_tag_xhash );

	*hash_string_length += 1 + string_length(
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
			*hash_string_length += 7 + ( 2 * string_length(
			                                  hash_values->identifiers[ iterator ] ) );

			/* Add space for the hash value
			 */
			*hash_string_length += string_length(
			                        hash_values->values[ iterator ] );
		}
	}
	/* allow for an empty line and an end of string
	 */
	*hash_string_length += 2;

	*hash_string = (character_t *) memory_allocate(
                                        sizeof( character_t ) * *hash_string_length );

	if( *hash_string == NULL )
	{
		notify_warning_printf( "%s: unable to create hash string.\n",
		 function );

		*hash_string_length = 0;

		return( -1 );
	}
	character_count = string_snprintf(
	                   *hash_string,
	                   *hash_string_length,
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

		*hash_string        = NULL;
		*hash_string_length = 0;

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
			                   ( *hash_string_length - string_offset ),
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

				*hash_string        = NULL;
				*hash_string_length = 0;

				return( -1 );
			}
			string_offset += character_count;
		}
	}
	character_count = string_snprintf(
	                   &( ( *hash_string )[ string_offset ] ),
	                   ( *hash_string_length - string_offset ),
	                   _CHARACTER_T_STRING( "%" ) _CHARACTER_T_STRING( PRIs ) _CHARACTER_T_STRING( "\n\n" ),
	                   xml_close_tag_xhash );

	if( character_count <= -1 )
	{
		notify_warning_printf( "%s: unable to set hash string.\n",
		 function );

		memory_free(
		 *hash_string );

		*hash_string        = NULL;
		*hash_string_length = 0;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	( *hash_string )[ *hash_string_length - 1 ] = (character_t) '\0';

	return( 1 );
}

/* Generate an EWFX xhash
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_generate_xhash_string_ewfx(
     libewf_values_table_t *hash_values,
     ewf_char_t **hash,
     size_t *hash_length )
{
	character_t *hash_string = NULL;
	static char *function    = "libewf_hash_values_generate_xhash_string_ewfx";
	int result               = 0;

	if( libewf_hash_values_generate_hash_string_xml(
	     hash_values,
	     &hash_string,
	     hash_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to create xhash string.\n",
		 function );

		return( -1 );
	}
	result = libewf_hash_values_convert_hash_string_to_hash(
	          hash_string,
	          *hash_length,
	          hash,
	          hash_length );

	if( result != 1 )
	{
		notify_warning_printf( "%s: unable to create xhash.\n",
		 function );
	}
	memory_free(
	 hash_string );

	return( result );
}


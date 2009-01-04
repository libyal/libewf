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

#include "libewf_includes.h"

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_hash_values.h"
#include "libewf_notify.h"
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
		LIBEWF_WARNING_PRINT( "%s: invalid hash values.\n",
		 function );

		return( -1 );
	}
	hash_values->identifiers[ 0 ] = libewf_string_duplicate(
	                                 _S_LIBEWF_CHAR( "MD5" ),
	                                 3 );

	return( 1 );
}

/* Parse a xml hash string for the values
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_parse_hash_string_xml(
     libewf_values_table_t **hash_values,
     libewf_char_t *hash_string_xml,
     size_t length )
{
	libewf_char_t **lines          = NULL;
	libewf_char_t *open_tag_start  = NULL;
	libewf_char_t *open_tag_end    = NULL;
	libewf_char_t *close_tag_start = NULL;
	libewf_char_t *close_tag_end   = NULL;
	static char *function          = "libewf_hash_values_parse_hash_string_xml";
	size_t string_length           = 0;
	uint32_t line_count            = 0;
	uint32_t iterator              = 0;

	if( hash_string_xml == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	lines = libewf_string_split(
	         (libewf_char_t *) hash_string_xml,
	         length,
	         (libewf_char_t) '\n',
	         &line_count );

	if( lines == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to split lines in hash string.\n",
		 function );

		return( -1 );
	}
	*hash_values = libewf_values_table_alloc(
	                LIBEWF_HASH_VALUES_DEFAULT_AMOUNT );

	if( *hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create hash values.\n",
		 function );

		libewf_string_split_values_free(
		 lines,
		 line_count );

		return( -1 );
	}
	if( libewf_hash_values_initialize(
	     *hash_values ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to initialize the hash values.\n",
		 function );

		libewf_string_split_values_free(
		 lines,
		 line_count );

		return( -1 );
	}
	for( iterator = 0; iterator < line_count; iterator++ )
	{
		if( ( lines[ iterator ] == NULL )
		 || ( lines[ iterator ] == (libewf_char_t *) _S_LIBEWF_CHAR( "" ) ) )
		{
			continue;
		}
		string_length = libewf_string_length(
		                 lines[ iterator ] );

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

		if( libewf_values_table_set_value(
		     *hash_values,
		     &open_tag_start[ 1 ],
		     &open_tag_end[ 1 ],
		     string_length ) != 1 )
		{
			LIBEWF_VERBOSE_PRINT( "%s: unable to set value with identifier: %" PRIs_EWF ".\n",
			 function, &open_tag_start[ 1 ] );
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
	libewf_char_t *xml_hash_string = NULL;
	static char *function          = "libewf_hash_values_parse_xhash";
	int result                     = 0;

	if( xhash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid xhash.\n",
		 function );

		return( -1 );
	}
	xml_hash_string = (libewf_char_t *) libewf_common_alloc(
	                                     sizeof( libewf_char_t ) * ( size + 1 ) );

	if( xml_hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create XML hash string.\n",
		 function );

		return( -1 );
	}
	if( libewf_string_copy_from_ewf_char(
	     xml_hash_string,
	     size,
	     xhash,
	     size ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy xhash to xml hash string.\n",
		 function );

		libewf_common_free(
		 xml_hash_string );

		return( -1 );
	}
	result = libewf_hash_values_parse_hash_string_xml(
	          hash_values,
	          xml_hash_string,
	          size );

	if( result != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to parse xml hash string.\n",
		 function );
	}
	libewf_common_free(
	 xml_hash_string );

	return( result );
}

/* Converts a hash string into a hash
 * Sets the hash and hash length
 * Returns 1 if successful or -1 on error
 */
int libewf_hash_values_convert_hash_string_to_hash(
     libewf_char_t *hash_string,
     size_t hash_string_length,
     ewf_char_t **hash,
     size_t *hash_length )
{
	static char *function = "libewf_hash_values_convert_hash_string_to_hash";

	if( hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	if( ( hash_string_length == 0 )
	 || ( hash_string_length > (size_t) SSIZE_MAX ) )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash string length.\n",
		 function );

		return( -1 );
	}
	if( hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash.\n",
		 function );

		return( -1 );
	}
	if( hash_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash length.\n",
		 function );

		return( -1 );
	}
	if( *hash != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: hash already created .\n",
		 function );

		return( -1 );
	}
	*hash = (ewf_char_t *) libewf_common_alloc(
	                        sizeof( ewf_char_t ) * hash_string_length );

	if( *hash == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create hash.\n",
		 function );

		*hash_length = 0;

		return( -1 );
	}
	*hash_length = hash_string_length;

	if( libewf_string_copy_to_ewf_char(
	     hash_string,
	     hash_string_length,
	     *hash,
	     *hash_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash.\n",
		 function );

		libewf_common_free(
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
     libewf_char_t **hash_string,
     size_t *hash_string_length )
{
	libewf_char_t *xml_head            = _S_LIBEWF_CHAR( "<?xml version=\"1.0\"?>" );
	libewf_char_t *xml_open_tag_xhash  = _S_LIBEWF_CHAR( "<xhash>" );
	libewf_char_t *xml_close_tag_xhash = _S_LIBEWF_CHAR( "</xhash>" );
	static char *function              = "libewf_hash_values_generate_hash_string_xml";
	uint32_t iterator                  = 0;
	int string_offset                  = 0;
	int character_count                = 0;

	if( hash_values == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash values.\n",
		 function );

		return( -1 );
	}
	if( hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash string.\n",
		 function );

		return( -1 );
	}
	if( *hash_string != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: hash string already created.\n",
		 function );

		return( -1 );
	}
	if( hash_string_length == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid hash string length.\n",
		 function );

		return( -1 );
	}
	/* Add space for the xml data and an end of line
	 */
	*hash_string_length = 1 + libewf_string_length(
	                           xml_head );

	*hash_string_length += 1 + libewf_string_length(
	                            xml_open_tag_xhash );

	*hash_string_length += 1 + libewf_string_length(
	                            xml_close_tag_xhash );

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
			*hash_string_length += 7 + ( 2 * libewf_string_length(
			                                  hash_values->identifiers[ iterator ] ) );

			/* Add space for the hash value
			 */
			*hash_string_length += libewf_string_length(
			                        hash_values->values[ iterator ] );
		}
	}
	/* allow for an empty line and an end of string
	 */
	*hash_string_length += 2;

	*hash_string = (libewf_char_t *) libewf_common_alloc(
                                         sizeof( libewf_char_t ) * *hash_string_length );

	if( *hash_string == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create hash string.\n",
		 function );

		*hash_string_length = 0;

		return( -1 );
	}
	character_count = libewf_string_snprintf(
	                   *hash_string,
	                   *hash_string_length,
	                   _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n%" )
	                   _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n" ),
	                   xml_head,
	                   xml_open_tag_xhash );

	if( character_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash string.\n",
		 function );

		libewf_common_free(
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
			LIBEWF_WARNING_PRINT( "%s: invalid hash value - missing identifier.\n",
			 function );

			continue;
		}
		if( hash_values->values[ iterator ] != NULL )
		{
			character_count = libewf_string_snprintf(
			                   &( ( *hash_string) [ string_offset ] ),
			                   ( *hash_string_length - string_offset ),
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

				libewf_common_free(
				 *hash_string );

				*hash_string        = NULL;
				*hash_string_length = 0;

				return( -1 );
			}
			string_offset += character_count;
		}
	}
	character_count = libewf_string_snprintf(
	                   &( ( *hash_string )[ string_offset ] ),
	                   ( *hash_string_length - string_offset ),
	                   _S_LIBEWF_CHAR( "%" ) _S_LIBEWF_CHAR( PRIs_EWF ) _S_LIBEWF_CHAR( "\n\n" ),
	                   xml_close_tag_xhash );

	if( character_count <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set hash string.\n",
		 function );

		libewf_common_free(
		 *hash_string );

		*hash_string        = NULL;
		*hash_string_length = 0;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	( *hash_string )[ *hash_string_length - 1 ] = (libewf_char_t) '\0';

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
	libewf_char_t *hash_string = NULL;
	static char *function      = "libewf_hash_values_generate_xhash_string_ewfx";
	int result                 = 0;

	if( libewf_hash_values_generate_hash_string_xml(
	     hash_values,
	     &hash_string,
	     hash_length ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create xhash string.\n",
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
		LIBEWF_WARNING_PRINT( "%s: unable to create xhash.\n",
		 function );
	}
	libewf_common_free(
	 hash_string );

	return( result );
}


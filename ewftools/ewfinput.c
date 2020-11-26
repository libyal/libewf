/*
 * User input functions for the ewftools
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#include "byte_size_string.h"
#include "ewfinput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libewf.h"
#include "ewftools_system_string.h"

/* Input selection defintions
 */
#if defined( HAVE_BZIP2_SUPPORT )
system_character_t *ewfinput_compression_methods[ 2 ] = {
	_SYSTEM_STRING( "deflate" ),
	_SYSTEM_STRING( "bzip2" ) };
#else
system_character_t *ewfinput_compression_methods[ 1 ] = {
	_SYSTEM_STRING( "deflate" ) };
#endif

system_character_t *ewfinput_compression_levels[ 4 ] = {
	_SYSTEM_STRING( "none" ),
	_SYSTEM_STRING( "empty-block" ),
	_SYSTEM_STRING( "fast" ),
	_SYSTEM_STRING( "best" ) };

system_character_t *ewfinput_format_types[ 15 ] = {
	_SYSTEM_STRING( "ewf" ),
	_SYSTEM_STRING( "smart" ),
	_SYSTEM_STRING( "ftk" ),
	_SYSTEM_STRING( "encase1" ),
	_SYSTEM_STRING( "encase2" ),
	_SYSTEM_STRING( "encase3" ),
	_SYSTEM_STRING( "encase4" ),
	_SYSTEM_STRING( "encase5" ),
	_SYSTEM_STRING( "encase6" ),
	_SYSTEM_STRING( "encase7" ),
	_SYSTEM_STRING( "encase7-v2" ),
	_SYSTEM_STRING( "linen5" ),
	_SYSTEM_STRING( "linen6" ),
	_SYSTEM_STRING( "linen7" ),
	_SYSTEM_STRING( "ewfx" ) };

system_character_t *ewfinput_media_types[ 4 ] = {
	_SYSTEM_STRING( "fixed" ),
	_SYSTEM_STRING( "removable" ),
	_SYSTEM_STRING( "optical" ),
	_SYSTEM_STRING( "memory" ) };

system_character_t *ewfinput_media_flags[ 2 ] = {
	_SYSTEM_STRING( "logical" ),
	_SYSTEM_STRING( "physical" ) };

system_character_t *ewfinput_sector_per_block_sizes[ 12 ] = {
	_SYSTEM_STRING( "16" ),
	_SYSTEM_STRING( "32" ),
	_SYSTEM_STRING( "64" ),
	_SYSTEM_STRING( "128" ),
	_SYSTEM_STRING( "256" ),
	_SYSTEM_STRING( "512" ),
	_SYSTEM_STRING( "1024" ),
	_SYSTEM_STRING( "2048" ),
	_SYSTEM_STRING( "4096" ),
	_SYSTEM_STRING( "8192" ),
	_SYSTEM_STRING( "16384" ),
	_SYSTEM_STRING( "32768" ) };

system_character_t *ewfinput_yes_no[ 2 ] = {
	_SYSTEM_STRING( "yes" ),
	_SYSTEM_STRING( "no" ) };

/* Determines the EWF format from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int ewfinput_determine_ewf_format(
     const system_character_t *string,
     uint8_t *ewf_format,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_ewf_format";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( ewf_format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid libewf format.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "ewf" ),
		     3 ) == 0 )
		{
			*ewf_format = LIBEWF_FORMAT_EWF;
			result      = 1;
		}
		else if( system_string_compare(
		          string,
		          _SYSTEM_STRING( "ftk" ),
		          3 ) == 0 )
		{
			*ewf_format = LIBEWF_FORMAT_FTK_IMAGER;
			result      = 1;
		}
	}
	else if( string_length == 4 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "ewfx" ),
		     4 ) == 0 )
		{
			*ewf_format = LIBEWF_FORMAT_EWFX;
			result      = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "smart" ),
		     5 ) == 0 )
		{
			*ewf_format = LIBEWF_FORMAT_SMART;
			result      = 1;
		}
	}
	else if( string_length == 6 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "linen" ),
		     5 ) == 0 )
		{
			if( string[ 5 ] == (system_character_t) '5' )
			{
				*ewf_format = LIBEWF_FORMAT_LINEN5;
				result      = 1;
			}
			else if( string[ 5 ] == (system_character_t) '6' )
			{
				*ewf_format = LIBEWF_FORMAT_LINEN6;
				result      = 1;
			}
			else if( string[ 5 ] == (system_character_t) '7' )
			{
				*ewf_format = LIBEWF_FORMAT_LINEN7;
				result      = 1;
			}
		}
	}
	else if( string_length == 7 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "encase" ),
		     6 ) == 0 )
		{
			if( string[ 6 ] == (system_character_t) '1' )
			{
				*ewf_format = LIBEWF_FORMAT_ENCASE1;
				result      = 1;
			}
			else if( string[ 6 ] == (system_character_t) '2' )
			{
				*ewf_format = LIBEWF_FORMAT_ENCASE2;
				result      = 1;
			}
			else if( string[ 6 ] == (system_character_t) '3' )
			{
				*ewf_format = LIBEWF_FORMAT_ENCASE3;
				result      = 1;
			}
			else if( string[ 6 ] == (system_character_t) '4' )
			{
				*ewf_format = LIBEWF_FORMAT_ENCASE4;
				result      = 1;
			}
			else if( string[ 6 ] == (system_character_t) '5' )
			{
				*ewf_format = LIBEWF_FORMAT_ENCASE5;
				result      = 1;
			}
			else if( string[ 6 ] == (system_character_t) '6' )
			{
				*ewf_format = LIBEWF_FORMAT_ENCASE6;
				result      = 1;
			}
			else if( string[ 6 ] == (system_character_t) '7' )
			{
				*ewf_format = LIBEWF_FORMAT_ENCASE7;
				result      = 1;
			}
		}
	}
	else if( string_length == 10 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "encase" ),
		     6 ) == 0 )
		{
			if( ( string[ 8 ] == (system_character_t) 'v' )
			 && ( string[ 9 ] == (system_character_t) '2' ) )
			{
				if( ( string[ 7 ] == (system_character_t) '-' )
				 || ( string[ 7 ] == (system_character_t) '_' ) )
				{
					if( string[ 6 ] == (system_character_t) '7' )
					{
						*ewf_format = LIBEWF_FORMAT_V2_ENCASE7;
						result      = 1;
					}
				}
			}
		}
	}
	return( result );
}

/* Determines the sectors per chunk value from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int ewfinput_determine_sectors_per_chunk(
     const system_character_t *string,
     uint32_t *sectors_per_chunk,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_sectors_per_chunk";
	size_t string_length  = 0;
	int result            = -1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( sectors_per_chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sectors per chunk.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 2 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "16" ),
		     2 ) == 0 )
		{
			*sectors_per_chunk = 16;
			result             = 1;
		}
		else if( system_string_compare(
		          string,
		          _SYSTEM_STRING( "32" ),
		          2 ) == 0 )
		{
			*sectors_per_chunk = 32;
			result             = 1;
		}
		else if( system_string_compare(
		          string,
		          _SYSTEM_STRING( "64" ),
		          2 ) == 0 )
		{
			*sectors_per_chunk = 64;
			result             = 1;
		}
	}
	else if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "128" ),
		     3 ) == 0 )
		{
			*sectors_per_chunk = 128;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "256" ),
			  3 ) == 0 )
		{
			*sectors_per_chunk = 256;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "512" ),
			  3 ) == 0 )
		{
			*sectors_per_chunk = 512;
			result             = 1;
		}
	}
	else if( string_length == 4 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "1024" ),
		     4 ) == 0 )
		{
			*sectors_per_chunk = 1024;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "2048" ),
			  4 ) == 0 )
		{
			*sectors_per_chunk = 2048;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "4096" ),
			  4 ) == 0 )
		{
			*sectors_per_chunk = 4096;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "8192" ),
			  4 ) == 0 )
		{
			*sectors_per_chunk = 8192;
			result             = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "16384" ),
		     5 ) == 0 )
		{
			*sectors_per_chunk = 16384;
			result             = 1;
		}
		else if( system_string_compare(
		          string,
		          _SYSTEM_STRING( "32768" ),
		          5 ) == 0 )
		{
			*sectors_per_chunk = 32768;
			result             = 1;
		}
	}
	return( result );
}

/* Determines the compression method from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int ewfinput_determine_compression_method(
     const system_character_t *string,
     uint16_t *compression_method,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_compression_method";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( compression_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression method.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

#if defined( HAVE_BZIP2_SUPPORT )
	if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "bzip2" ),
		     5 ) == 0 )
		{
			*compression_method = LIBEWF_COMPRESSION_METHOD_BZIP2;
			result              = 1;
		}
	}
	else if( string_length == 7 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "deflate" ),
		     7 ) == 0 )
		{
			*compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
			result              = 1;
		}
	}
#else
	if( string_length == 7 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "deflate" ),
		     7 ) == 0 )
		{
			*compression_method = LIBEWF_COMPRESSION_METHOD_DEFLATE;
			result              = 1;
		}
	}
#endif
	return( result );
}

/* Determines the compression values from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int ewfinput_determine_compression_values(
     const system_character_t *string,
     int8_t *compression_level,
     uint8_t *compression_flags,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_compression_values";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( compression_level == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression level.",
		 function );

		return( -1 );
	}
	if( compression_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression flags.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 4 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "none" ),
		     4 ) == 0 )
		{
			*compression_level = LIBEWF_COMPRESSION_LEVEL_NONE;
			*compression_flags = 0;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "fast" ),
			  4 ) == 0 )
		{
			*compression_level = LIBEWF_COMPRESSION_LEVEL_FAST;
			*compression_flags = 0;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "best" ),
			  4 ) == 0 )
		{
			*compression_level = LIBEWF_COMPRESSION_LEVEL_BEST;
			*compression_flags = 0;
			result             = 1;
		}
	}
	else if( string_length == 11 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "empty-block" ),
		     11 ) == 0 )
		{
			*compression_level = LIBEWF_COMPRESSION_LEVEL_NONE;
			*compression_flags = LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
			result             = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "empty_block" ),
			  11 ) == 0 )
		{
			*compression_level = LIBEWF_COMPRESSION_LEVEL_NONE;
			*compression_flags = LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
			result             = 1;
		}
	}
	return( result );
}

/* Determines the media type value from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int ewfinput_determine_media_type(
     const system_character_t *string,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_media_type";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "fixed" ),
			  5 ) == 0 )
		{
			*media_type = LIBEWF_MEDIA_TYPE_FIXED;
			result      = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "memory" ),
		     6 ) == 0 )
		{
			*media_type = LIBEWF_MEDIA_TYPE_MEMORY;
			result      = 1;
		}
	}
	else if( string_length == 7 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "optical" ),
		     7 ) == 0 )
		{
			*media_type = LIBEWF_MEDIA_TYPE_OPTICAL;
			result      = 1;
		}
	}
	else if( string_length == 9 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "removable" ),
		     9 ) == 0 )
		{
			*media_type = LIBEWF_MEDIA_TYPE_REMOVABLE;
			result      = 1;
		}
	}
	return( result );
}

/* Determines the media flags value from a string
 * Returns 1 if successful or -1 on error
 */
int ewfinput_determine_media_flags(
     const system_character_t *string,
     uint8_t *media_flags,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_media_flags";
	int result            = -1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( media_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media flags.",
		 function );

		return( -1 );
	}
	if( system_string_compare(
	     string,
	     _SYSTEM_STRING( "logical" ),
	     7 ) == 0 )
	{
		*media_flags &= ~LIBEWF_MEDIA_FLAG_PHYSICAL;
		result        = 1;
	}
	else if( system_string_compare(
	          string,
	          _SYSTEM_STRING( "physical" ),
	          8 ) == 0 )
	{
		*media_flags |= LIBEWF_MEDIA_FLAG_PHYSICAL;
		result        = 1;
	}
	else if( system_string_compare(
	          string,
	          _SYSTEM_STRING( "fastbloc" ),
	          8 ) == 0 )
	{
		*media_flags |= LIBEWF_MEDIA_FLAG_FASTBLOC;
		result        = 1;
	}
	else if( system_string_compare(
	          string,
	          _SYSTEM_STRING( "tableau" ),
	          8 ) == 0 )
	{
		*media_flags |= LIBEWF_MEDIA_FLAG_TABLEAU;
		result        = 1;
	}
	return( result );
}

/* Determines the codepage from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int ewfinput_determine_header_codepage(
     const system_character_t *string,
     int *header_codepage,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_header_codepage";
	size_t string_length  = 0;
	int result            = -1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( header_codepage == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header codepage.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "ascii" ),
		     5 ) == 0 )
		{
			*header_codepage = LIBEWF_CODEPAGE_ASCII;
			result           = 1;
		}
	}
#if defined( HAVE_ISO_CODEPAGES )
	if( ( string_length == 10 )
	 || ( string_length == 11 ) )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "iso" ),
		     3 ) == 0 )
		{
			if( ( string[ 3 ] != '-' )
			 && ( string[ 3 ] != '_' ) )
			{
			}
			else if( system_string_compare(
				  &( string[ 4 ] ),
				  _SYSTEM_STRING( "8859" ),
				  4 ) == 0 )
			{
				if( ( string[ 8 ] != '-' )
				 && ( string[ 8 ] != '_' ) )
				{
				}
				else if( string_length == 10 )
				{
					if( system_string_compare(
					     &( string[ 9 ] ),
					     _SYSTEM_STRING( "1" ),
					     1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_1;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "2" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_2;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "3" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_3;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "4" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_4;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "5" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_5;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "6" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_6;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "7" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_7;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "8" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_8;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "9" ),
						  1 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_9;
						result           = 1;
					}
				}
				else if( string_length == 11 )
				{
					if( system_string_compare(
					     &( string[ 9 ] ),
					     _SYSTEM_STRING( "10" ),
					     2 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_10;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "11" ),
						  2 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_11;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "13" ),
						  2 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_13;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "14" ),
						  2 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_14;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "15" ),
						  2 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_15;
						result           = 1;
					}
					else if( system_string_compare(
						  &( string[ 9 ] ),
						  _SYSTEM_STRING( "16" ),
						  2 ) == 0 )
					{
						*header_codepage = LIBEWF_CODEPAGE_ISO_8859_16;
						result           = 1;
					}
				}
			}
		}
	}
#endif
	if( ( string_length == 11 )
	 || ( string_length == 12 ) )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "windows" ),
		     7 ) == 0 )
		{
			if( ( string[ 7 ] != '-' )
			 && ( string[ 7 ] != '_' ) )
			{
			}
			else if( string_length == 11 )
			{
				if( system_string_compare(
				     &( string[ 8 ] ),
				     _SYSTEM_STRING( "874" ),
				     3 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_874;
					result           = 1;
				}
				else if( system_string_compare(
				          &( string[ 8 ] ),
				          _SYSTEM_STRING( "932" ),
				          3 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_932;
					result           = 1;
				}
				else if( system_string_compare(
				          &( string[ 8 ] ),
				          _SYSTEM_STRING( "936" ),
				          3 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_936;
					result           = 1;
				}
			}
			else if( string_length == 12 )
			{
				if( system_string_compare(
				     &( string[ 8 ] ),
				     _SYSTEM_STRING( "1250" ),
				     4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1250;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1251" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1251;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1252" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1252;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1253" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1253;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1253" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1253;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1254" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1254;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1255" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1255;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1256" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1256;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1257" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1257;
					result           = 1;
				}
				else if( system_string_compare(
					  &( string[ 8 ] ),
					  _SYSTEM_STRING( "1258" ),
					  4 ) == 0 )
				{
					*header_codepage = LIBEWF_CODEPAGE_WINDOWS_1258;
					result           = 1;
				}
			}
		}
	}
	return( result );
}

/* Determines the yes or no value from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int ewfinput_determine_yes_no(
     const system_character_t *string,
     uint8_t *yes_no_value,
     libcerror_error_t **error )
{
	static char *function = "ewfinput_determine_yes_no";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( yes_no_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid yes no value.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 2 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "no" ),
		     2 ) == 0 )
		{
			*yes_no_value = 0;
			result        = 1;
		}
	}
	else if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "yes" ),
		     3 ) == 0 )
		{
			*yes_no_value = 1;
			result        = 1;
		}
	}
	return( result );
}

/* Retrieves a string variable
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int ewfinput_get_string_variable(
     FILE *stream,
     const system_character_t *request_string,
     system_character_t *string_variable,
     size_t string_variable_size,
     libcerror_error_t **error )
{
	system_character_t *end_of_input  = NULL;
	system_character_t *result_string = NULL;
	static char *function             = "ewfinput_get_string_variable";
	ssize_t input_length              = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output stream.",
		 function );

		return( -1 );
	}
	if( request_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid request string.",
		 function );

		return( -1 );
	}
	if( string_variable == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string variable.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T > SIZEOF_INT
	if( string_variable_size > (size_t) INT_MAX )
#else
	if( string_variable_size > (size_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string variable size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Safe guard the end of the input string
	 */
	string_variable[ string_variable_size - 1 ] = 0;

	while( 1 )
	{
		fprintf(
		 stream,
		 "%" PRIs_SYSTEM ": ",
		 request_string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result_string = file_stream_get_string_wide(
		                 stdin,
		                 string_variable,
		                 (int) ( string_variable_size - 1 ) );
#else
		result_string = file_stream_get_string(
		                 stdin,
		                 string_variable,
		                 (int) ( string_variable_size - 1 ) );
#endif
		if( result_string != NULL )
		{
			end_of_input = system_string_search_character(
			                string_variable,
			                (system_character_t) '\n',
			                string_variable_size );

			/* Input was larger than size of buffer
			 */
			if( end_of_input == NULL )
			{
				/* Flush the stdin stream
				 */
				while( end_of_input == NULL )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result_string = file_stream_get_string_wide(
					                 stdin,
					                 string_variable,
					                 (int) ( string_variable_size - 1 ) );
#else
					result_string = file_stream_get_string(
					                 stdin,
					                 string_variable,
					                 (int) ( string_variable_size - 1 ) );
#endif
					end_of_input = system_string_search_character(
					                string_variable,
					                (system_character_t) '\n',
					                string_variable_size );

				}
				return( -1 );
			}
			input_length = (ssize_t) ( end_of_input - string_variable );

			if( input_length < 0 )
			{
				return( -1 );
			}
			/* Make sure the string is terminated with an end of string character
			 */
			string_variable[ input_length ] = 0;

			break;
		}
		else
		{
			fprintf(
			 stream,
			 "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	if( input_length == 0 )
	{
		return( 0 );
	}
	return( 1 );
}

/* Retrieves a size variable
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int ewfinput_get_size_variable(
     FILE *stream,
     system_character_t *input_buffer,
     size_t input_buffer_size,
     const system_character_t *request_string,
     uint64_t minimum_size,
     uint64_t maximum_size,
     uint64_t default_size,
     uint64_t *size_variable,
     libcerror_error_t **error )
{
	system_character_t *end_of_input  = NULL;
	system_character_t *result_string = NULL;
	static char *function             = "ewfinput_get_size_variable";
	ssize_t input_length              = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output stream.",
		 function );

		return( -1 );
	}
	if( input_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input buffer.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T > SIZEOF_INT
	if( input_buffer_size > (size_t) INT_MAX )
#else
	if( input_buffer_size > (size_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( request_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid request string.",
		 function );

		return( -1 );
	}
	if( size_variable == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size variable.",
		 function );

		return( -1 );
	}
	/* Safe guard the end of the input buffer
	 */
	input_buffer[ input_buffer_size - 1 ] = 0;

	while( 1 )
	{
		fprintf(
		 stream,
		 "%" PRIs_SYSTEM " (%" PRIu64 " <= value <= %" PRIu64 ") [%" PRIu64 "]: ",
		 request_string,
		 minimum_size,
		 maximum_size,
		 default_size );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result_string = file_stream_get_string_wide(
		                 stdin,
		                 input_buffer,
		                 (int)( input_buffer_size - 1 ) );
#else
		result_string = file_stream_get_string(
		                 stdin,
		                 input_buffer,
		                 (int)( input_buffer_size - 1 ) );
#endif
		if( result_string != NULL )
		{
			end_of_input = system_string_search_character(
			                input_buffer,
			                (system_character_t) '\n',
			                input_buffer_size );

			/* Input was larger than size of buffer
			 */
			if( end_of_input == NULL )
			{
				/* Flush the stdin stream
				 */
				while( end_of_input == NULL )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result_string = file_stream_get_string_wide(
					                 stdin,
					                 input_buffer,
					                 (int) ( input_buffer_size - 1 ) );
#else
					result_string = file_stream_get_string(
					                 stdin,
					                 input_buffer,
					                 (int) ( input_buffer_size - 1 ) );
#endif
					end_of_input = system_string_search_character(
					                input_buffer,
					                (system_character_t) '\n',
					                input_buffer_size );

				}
				return( -1 );
			}
			input_length = (ssize_t) ( end_of_input - input_buffer );

			if( input_length < 0 )
			{
				return( -1 );
			}
			else if( input_length == 0 )
			{
				*size_variable = default_size;

				return( 0 );
			}
			if( ewftools_system_string_decimal_copy_to_64_bit(
			     input_buffer,
			     input_length,
			     size_variable,
			     NULL ) != 1 )
			{
				fprintf(
				 stream,
				 "Unable to convert value into number, please try again or terminate using Ctrl^C.\n" );
			}
			else if( ( *size_variable >= minimum_size )
			      && ( *size_variable <= maximum_size ) )
			{
				break;
			}
			else
			{
				fprintf(
				 stream,
				 "Value not within specified range, please try again or terminate using Ctrl^C.\n" );
			}
		}
		else
		{
			fprintf(
			 stream,
			 "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( 1 );
}

/* Retrieves a byte size variable
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int ewfinput_get_byte_size_variable(
     FILE *stream,
     system_character_t *input_buffer,
     size_t input_buffer_size,
     const system_character_t *request_string,
     uint64_t minimum_size,
     uint64_t maximum_size,
     uint64_t default_size,
     uint64_t *byte_size_variable,
     libcerror_error_t **error )
{
	system_character_t minimum_size_string[ 16 ];
	system_character_t maximum_size_string[ 16 ];
	system_character_t default_size_string[ 16 ];

	system_character_t *end_of_input  = NULL;
	system_character_t *result_string = NULL;
	static char *function             = "ewfinput_get_byte_size_variable";
	ssize_t input_length              = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output stream.",
		 function );

		return( -1 );
	}
	if( input_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input buffer.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T > SIZEOF_INT
	if( input_buffer_size > (size_t) INT_MAX )
#else
	if( input_buffer_size > (size_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( request_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid request string.",
		 function );

		return( -1 );
	}
	if( byte_size_variable == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte size variable.",
		 function );

		return( -1 );
	}
	if( byte_size_string_create(
	     minimum_size_string,
	     16,
	     minimum_size,
	     BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create minimum byte size string.",
		 function );

		return( -1 );
	}
	if( byte_size_string_create(
	     default_size_string,
	     16,
	     default_size,
	     BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create default byte size string.",
		 function );

		return( -1 );
	}
	if( byte_size_string_create(
	     maximum_size_string,
	     16,
	     maximum_size,
	     BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create maximum byte size string.",
		 function );

		return( -1 );
	}
	/* Safe guard the end of the input buffer
	 */
	input_buffer[ input_buffer_size - 1 ] = 0;

	while( 1 )
	{
		fprintf(
		 stream,
		 "%" PRIs_SYSTEM " (%" PRIs_SYSTEM " <= value <= %" PRIs_SYSTEM ") [%" PRIs_SYSTEM "]: ",
		 request_string,
		 minimum_size_string,
		 maximum_size_string,
		 default_size_string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result_string = file_stream_get_string_wide(
		                 stdin,
		                 input_buffer,
		                 (int) ( input_buffer_size - 1 ) );
#else
		result_string = file_stream_get_string(
		                 stdin,
		                 input_buffer,
		                 (int) ( input_buffer_size - 1 ) );
#endif
		if( result_string != NULL )
		{
			end_of_input = system_string_search_character(
			                input_buffer,
			                (system_character_t) '\n',
			                input_buffer_size );

			/* Input was larger than size of buffer
			 */
			if( end_of_input == NULL )
			{
				/* Flush the stdin stream
				 */
				while( end_of_input == NULL )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result_string = file_stream_get_string_wide(
					                 stdin,
					                 input_buffer,
					                 (int) ( input_buffer_size - 1 ) );
#else
					result_string = file_stream_get_string(
					                 stdin,
					                 input_buffer,
					                 (int) ( input_buffer_size - 1 ) );
#endif
					end_of_input = system_string_search_character(
					                input_buffer,
					                (system_character_t) '\n',
					                input_buffer_size );

				}
				return( -1 );
			}
			input_length = (ssize_t) ( end_of_input - input_buffer );

			if( input_length < 0 )
			{
				return( -1 );
			}
			else if( input_length == 0 )
			{
				*byte_size_variable = default_size;

				return( 0 );
			}
			if( byte_size_string_convert(
			     input_buffer,
			     (size_t) input_length,
			     byte_size_variable,
			     NULL ) != 1 )
			{
				fprintf(
				 stream,
				 "Invalid value, please try again or terminate using Ctrl^C.\n" );
			}
			else if( ( *byte_size_variable >= minimum_size )
			      && ( *byte_size_variable <= maximum_size ) )
			{
				break;
			}
			else
			{
				fprintf(
				 stream,
				 "Value not within specified range, please try again or terminate using Ctrl^C.\n" );
			}
		}
		else
		{
			fprintf(
			 stream,
			 "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( 1 );
}

/* Retrieves a fixed value string variable
 * Returns 1 if successful, 0 if no input was provided or -1 on error
 */
int ewfinput_get_fixed_string_variable(
     FILE *stream,
     system_character_t *input_buffer,
     size_t input_buffer_size,
     const system_character_t *request_string,
     system_character_t **values,
     uint8_t number_of_values,
     uint8_t default_value,
     system_character_t **fixed_string_variable,
     libcerror_error_t **error )
{

	system_character_t *end_of_input  = NULL;
	system_character_t *result_string = NULL;
	static char *function             = "ewfinput_get_fixed_string_variable";
	size_t value_length               = 0;
	ssize_t input_length              = 0;
	uint8_t value_iterator            = 0;

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output stream.",
		 function );

		return( -1 );
	}
	if( input_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input buffer.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T > SIZEOF_INT
	if( input_buffer_size > (size_t) INT_MAX )
#else
	if( input_buffer_size > (size_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( request_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid request string.",
		 function );

		return( -1 );
	}
	if( default_value >= number_of_values )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: default value exceeds number of values.",
		 function );

		return( -1 );
	}
	if( fixed_string_variable == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fixed string variable.",
		 function );

		return( -1 );
	}
	while( 1 )
	{
		fprintf(
		 stream,
		 "%" PRIs_SYSTEM " (",
		 request_string );

		for( value_iterator = 0;
		     value_iterator < number_of_values;
		     value_iterator++ )
		{
			if( value_iterator > 0 )
			{
				fprintf(
				 stream,
				 ", " );
			}
			fprintf(
			 stream,
			 "%" PRIs_SYSTEM "",
			 values[ value_iterator ] );
		}
		fprintf(
		 stream,
		 ") [%" PRIs_SYSTEM "]: ",
		 values[ default_value ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result_string = file_stream_get_string_wide(
		                 stdin,
		                 input_buffer,
		                 (int) ( input_buffer_size - 1 ) );
#else
		result_string = file_stream_get_string(
		                 stdin,
		                 input_buffer,
		                 (int) ( input_buffer_size - 1 ) );
#endif
		if( result_string != NULL )
		{
			end_of_input = system_string_search_character(
			                input_buffer,
			                (system_character_t) '\n',
			                input_buffer_size );

			/* Input was larger than size of buffer
			 */
			if( end_of_input == NULL )
			{
				/* Flush the stdin stream
				 */
				while( end_of_input == NULL )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result_string = file_stream_get_string_wide(
					                 stdin,
					                 input_buffer,
					                 (int) ( input_buffer_size - 1 ) );
#else
					result_string = file_stream_get_string(
					                 stdin,
					                 input_buffer,
					                 (int) ( input_buffer_size - 1 ) );
#endif
					end_of_input = system_string_search_character(
					                input_buffer,
					                (system_character_t) '\n',
					                input_buffer_size );

				}
				return( -1 );
			}
			input_length = (ssize_t) ( end_of_input - input_buffer );

			if( input_length < 0 )
			{
				return( -1 );
			}
			else if( input_length == 0 )
			{
				*fixed_string_variable = values[ default_value ];

				return( 0 );
			}
			for( value_iterator = 0;
			     value_iterator < number_of_values;
			     value_iterator++ )
			{
				value_length = system_string_length(
						values[ value_iterator ] );

				if( ( value_length == (size_t) input_length )
				 && ( system_string_compare(
				       input_buffer,
				       values[ value_iterator ],
				       value_length ) == 0 ) )
				{
					break;
				}
			}
			if( value_iterator < number_of_values )
			{
				*fixed_string_variable = values[ value_iterator ];

				break;
			}
			fprintf(
			 stream,
			 "Selected option not supported, please try again or terminate using Ctrl^C.\n" );
		}
		else
		{
			fprintf(
			 stream,
			 "Error reading input, please try again or terminate using Ctrl^C.\n" );
		}
	}
	return( 1 );
}


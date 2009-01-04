/*
 * libewf filename
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

#include "libewf_filename.h"
#include "libewf_notify.h"

#include "ewf_definitions.h"

#ifdef REFACTOR
/* Retrieves an internally used filename
 * Returns 1 if succesful, or -1 on error
 */
int libewf_filename_get( LIBEWF_FILENAME *internal_filename, LIBEWF_FILENAME *external_filename, size_t length_external_filename )
{
	static char *function           = "libewf_filename_get";
	size_t length_internal_filename = 0;

	if( internal_filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid internal filename.\n",
		 function );

		return( -1 );
	}
	if( external_filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid external filename.\n",
		 function );

		return( -1 );
	}
	length_internal_filename = libewf_filename_length( internal_filename );

	/* Add one additional character for the end of line
	 */
	length_internal_filename += 1;

	if( length_external_filename < length_internal_filename )
	{
		LIBEWF_WARNING_PRINT( "%s: external filename too small.\n",
		 function );

		return( -1 );
	}
	if( libewf_filename_copy(
	     external_filename,
	     internal_filename,
	     length_internal_filename ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set external filename.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets an internal filename, creates a duplicate of the string
 * Returns 1 if succesful, or -1 on error
 */
int libewf_filename_set( LIBEWF_FILENAME **internal_filename, const LIBEWF_FILENAME *external_filename, size_t length_external_filename )
{
	static char *function = "libewf_filename_set";

	if( internal_filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid internal filename.\n",
		 function );

		return( -1 );
	}
	if( external_filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid external filename.\n",
		 function );

		return( -1 );
	}
	if( *internal_filename != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: filename already set: %" PRIs_EWF_filename ".\n",
		 function, *internal_filename );

		return( -1 );
	}
	if( length_external_filename == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid external filename length is zero.\n",
		 function );

		return( -1 );
	}
	if( length_external_filename >= (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid external filename length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* One additional byte for the end of string character is needed
	 */
	*internal_filename = (LIBEWF_FILENAME *) libewf_common_alloc( LIBEWF_FILENAME_SIZE * ( length_external_filename + 1 ) );

	if( *internal_filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create internal filename.\n",
		 function );

		return( -1 );
	}
	if( libewf_filename_copy( *internal_filename, external_filename, length_external_filename ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set internal filename.\n",
		 function );

		libewf_common_free( *internal_filename );

		*internal_filename = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	( *internal_filename )[ length_external_filename ] = '\0';

	return( 1 );
}
#endif

/* Creates the filename extension for a certain segment file
 * For EWF-E01, EWF-S01 segment file extension naming scheme
 * Returns 1 on success, -1 on error
 */
int libewf_filename_set_extension( LIBEWF_FILENAME *extension, uint16_t segment_number, int16_t maximum_amount_of_segments, uint8_t segment_file_type, uint8_t format, uint8_t ewf_format )
{
	static char *function                           = "libewf_filename_set_extension";
	LIBEWF_FILENAME extension_first_character       = (LIBEWF_FILENAME) '\0';
	LIBEWF_FILENAME extension_additional_characters = (LIBEWF_FILENAME) '\0';

	if( extension == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid extension.\n",
		 function );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment 0.\n",
		 function );

		return( -1 );
	}
	if( maximum_amount_of_segments <= -1 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid maximum amount of segment files.\n",
		 function );

		return( -1 );
	}
	if( segment_number > (uint16_t) maximum_amount_of_segments )
	{
		LIBEWF_WARNING_PRINT( "%s: segment number: %" PRIu16 " exceeds the maximum amount of segment files: %" PRIu16 ".\n",
		 function, segment_number, maximum_amount_of_segments );

		return( -1 );
	}
	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
	{
		if( ( format == LIBEWF_FORMAT_EWF )
		 || ( format == LIBEWF_FORMAT_EWFX ) )
		{
			extension_first_character       = (LIBEWF_FILENAME) 'e';
			extension_additional_characters = (LIBEWF_FILENAME) 'a';
		}
		else if( ewf_format == EWF_FORMAT_S01 )
		{
			extension_first_character       = (LIBEWF_FILENAME) 's';
			extension_additional_characters = (LIBEWF_FILENAME) 'a';
		}
		else
		{
			extension_first_character       = (LIBEWF_FILENAME) 'E';
			extension_additional_characters = (LIBEWF_FILENAME) 'A';
		}
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF )
	{
		extension_first_character       = (LIBEWF_FILENAME) 'L';
		extension_additional_characters = (LIBEWF_FILENAME) 'A';
	}
	else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		extension_first_character       = (LIBEWF_FILENAME) 'd';
		extension_additional_characters = (LIBEWF_FILENAME) 'a';
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported segment file type.\n",
		 function );

		return( -1 );
	}
	extension[ 0 ] = extension_first_character;

	if( segment_number <= 99 )
	{
		extension[ 2 ] = (LIBEWF_FILENAME) '0' + (LIBEWF_FILENAME) ( segment_number % 10 );
		extension[ 1 ] = (LIBEWF_FILENAME) '0' + (LIBEWF_FILENAME) ( segment_number / 10 );
	}
	else if( segment_number >= 100 )
	{
		segment_number -= 100;
		extension[ 2 ]  = extension_additional_characters + (LIBEWF_FILENAME) ( segment_number % 26 );
		segment_number /= 26;
		extension[ 1 ]  = extension_additional_characters + (LIBEWF_FILENAME) ( segment_number % 26 );
		segment_number /= 26;

		if( segment_number >= 26 )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to support for more segment files.\n",
			 function );

			return( -1 );
		}
		extension[ 0 ] = extension_first_character + (LIBEWF_FILENAME) segment_number;
	}
	/* Safety check
	 */
	if( ( extension[ 0 ] > (LIBEWF_FILENAME) 'z' )
	 || ( ( extension[ 0 ] > (LIBEWF_FILENAME) 'Z' )
	  && ( extension[ 0 ] < (LIBEWF_FILENAME) 'a' ) ) )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to support for more segment files.\n",
		 function );

		return( -1 );
	}
	extension[ 3 ] = (LIBEWF_FILENAME) '\0';

	return( 1 );
}

/* Creates a filename for a certain segment file
 * Returns 1 if successful, or -1 on error
 */
int libewf_filename_create( LIBEWF_FILENAME **filename, size_t *length_filename, LIBEWF_FILENAME *basename, size_t length_basename, uint16_t segment_number, int16_t maximum_amount_of_segments, uint8_t segment_file_type, uint8_t format, uint8_t ewf_format )
{
	LIBEWF_FILENAME *new_filename = NULL;
	static char *function         = "libewf_filename_create";

	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( *filename != NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: filename already set.\n",
		 function );

		return( -1 );
	}
	if( length_filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid length filename.\n",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid basename.\n",
		 function );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment 0.\n",
		 function );

		return( -1 );
	}
	/* The actual filename also contains a '.', 3 character extension and a end of string byte
	 */
	new_filename = libewf_common_alloc( LIBEWF_FILENAME_SIZE * ( length_basename + 5 ) );

	if( new_filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate filename.\n",
		 function );

		return( -1 );
	}
	/* Add one additional character for the end of line
	 */
	if( libewf_filename_copy( new_filename, basename, ( length_basename + 1 ) ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to copy basename.\n",
		 function );

		libewf_common_free( new_filename );

		return( -1 );
	}
	new_filename[ length_basename ] = (LIBEWF_FILENAME) '.';

	if( libewf_filename_set_extension(
	     &( new_filename[ length_basename + 1 ] ),
	     segment_number,
	     maximum_amount_of_segments,
	     segment_file_type,
	     format,
	     ewf_format ) != 1 )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set extension.\n",
		 function );

		libewf_common_free( new_filename );

		return( -1 );
	}
	*filename        = new_filename;
	*length_filename = length_basename + 5;

	return( 1 );
}


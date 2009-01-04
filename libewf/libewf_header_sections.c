/*
 * libewf header sections
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
#include "libewf_header_sections.h"
#include "libewf_header_values.h"
#include "libewf_notify.h"
#include "libewf_string.h"

/* Allocates memory for a new header sections struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_HEADER_SECTIONS *libewf_header_sections_alloc( void )
{
	LIBEWF_HEADER_SECTIONS *header_sections = NULL;
	static char *function                   = "libewf_header_sections_alloc";

	header_sections = (LIBEWF_HEADER_SECTIONS *) libewf_common_alloc( LIBEWF_HEADER_SECTIONS_SIZE );

	if( header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate header sections.\n",
		 function );

		return( NULL );
	}
	if( libewf_common_memset(
	     header_sections,
	     0,
	     LIBEWF_HEADER_SECTIONS_SIZE ) == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to clear header sections.\n",
		 function );

		libewf_common_free( header_sections );

		return( NULL );
	}
	return( header_sections );
}

/* Frees memory of a header sections struct including elements
 */
void libewf_header_sections_free( LIBEWF_HEADER_SECTIONS *header_sections )
{
        static char *function = "libewf_header_sections_free";

	if( header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header sections.\n",
		 function );

		return;
	}
	if( header_sections->header != NULL )
	{
		libewf_common_free( header_sections->header );
	}
	if( header_sections->header2 != NULL )
	{
		libewf_common_free( header_sections->header2 );
	}
	if( header_sections->xheader != NULL )
	{
		libewf_common_free( header_sections->xheader );
	}
	libewf_common_free( header_sections );
}

/* Create the header sections from the header values
 * Returns 1 on success, -1 on error
 */
int libewf_header_sections_create( LIBEWF_HEADER_SECTIONS *header_sections, libewf_values_table_t *header_values, int8_t compression_level, uint8_t format )
{
	static char *function = "libewf_header_sections_create";
	time_t timestamp      = time( NULL );

	if( header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_EWF )
	{
		header_sections->header = libewf_header_values_generate_header_string_ewf(
		                           header_values,
		                           timestamp,
		                           compression_level,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE1 )
	{
		header_sections->header = libewf_header_values_generate_header_string_encase1(
		                           header_values,
		                           timestamp,
		                           compression_level,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( ( format == LIBEWF_FORMAT_ENCASE2 )
	 || ( format == LIBEWF_FORMAT_ENCASE3 ) )
	{
		header_sections->header = libewf_header_values_generate_header_string_encase2(
		                           header_values,
		                           timestamp,
		                           compression_level,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( ( format == LIBEWF_FORMAT_FTK )
	 || ( format == LIBEWF_FORMAT_SMART ) )
	{
		header_sections->header = libewf_header_values_generate_header_string_ftk(
		                           header_values,
		                           timestamp,
		                           compression_level,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE4 )
	{
		header_sections->header = libewf_header_values_generate_header_string_encase4(
		                           header_values,
		                           timestamp,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		header_sections->header2 = libewf_header_values_generate_header2_string_encase4(
		                            header_values,
		                            timestamp,
		                            &( header_sections->header2_size ) );

		if( header_sections->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 sections.\n",
			 function );

			libewf_common_free( header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE5 )
	{
		header_sections->header = libewf_header_values_generate_header_string_encase4(
		                           header_values,
		                           timestamp,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		header_sections->header2 = libewf_header_values_generate_header2_string_encase5(
		                            header_values,
		                            timestamp,
		                            &( header_sections->header2_size ) );

		if( header_sections->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 sections.\n",
			 function );

			libewf_common_free( header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE6 )
	{
		header_sections->header = libewf_header_values_generate_header_string_encase4(
		                           header_values,
		                           timestamp,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		header_sections->header2 = libewf_header_values_generate_header2_string_encase6(
		                            header_values,
		                            timestamp,
		                            &( header_sections->header2_size ) );

		if( header_sections->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 sections.\n",
			 function );

			libewf_common_free( header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
	}
	else if( ( format == LIBEWF_FORMAT_LINEN5 )
	 || ( format == LIBEWF_FORMAT_LINEN6 ) )
	{
		header_sections->header = libewf_header_values_generate_header_string_encase5_linen(
		                           header_values,
		                           timestamp,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_EWFX )
	{
		header_sections->header = libewf_header_values_generate_header_string_ewfx(
		                           header_values,
		                           timestamp,
		                           &( header_sections->header_size ) );

		if( header_sections->header == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		header_sections->header2 = libewf_header_values_generate_header2_string_ewfx(
		                            header_values,
		                            timestamp,
		                            &( header_sections->header2_size ) );

		if( header_sections->header2 == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create header2 sections.\n",
			 function );

			libewf_common_free( header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
		header_sections->xheader = libewf_header_values_generate_xheader_string_ewfx(
		                            header_values,
		                            timestamp,
		                            &( header_sections->xheader_size ) );

		if( header_sections->xheader == NULL )
		{
			LIBEWF_WARNING_PRINT( "%s: unable to create xheader section.\n",
			 function );

			libewf_common_free( header_sections->header );
			libewf_common_free( header_sections->header2 );

			header_sections->header       = NULL;
			header_sections->header_size  = 0;
			header_sections->header2      = NULL;
			header_sections->header2_size = 0;

			return( -1 );
		}
	}
	return( 1 );
}


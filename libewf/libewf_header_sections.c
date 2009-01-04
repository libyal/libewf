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

#include "ewf_definitions.h"

/* Allocates memory for a new header sections struct
 * Returns a pointer to the new instance, NULL on error
 */
libewf_header_sections_t *libewf_header_sections_alloc(
                           void )
{
	libewf_header_sections_t *header_sections = NULL;
	static char *function                     = "libewf_header_sections_alloc";

	header_sections = (libewf_header_sections_t *) libewf_common_alloc(
	                                                sizeof( libewf_header_sections_t ) );

	if( header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate header sections.\n",
		 function );

		return( NULL );
	}
	if( libewf_common_memset(
	     header_sections,
	     0,
	     sizeof( libewf_header_sections_t ) ) == NULL )
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
void libewf_header_sections_free(
      libewf_header_sections_t *header_sections )
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
int libewf_header_sections_create(
     libewf_header_sections_t *header_sections,
     libewf_values_table_t *header_values,
     int8_t compression_level,
     uint8_t format )
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

/* Determines the EWF file format based on known characteristics
 * Returns 1 if the format was determined, -1 on errror
 */
int libewf_header_sections_determine_format(
     libewf_header_sections_t *header_sections,
     uint8_t ewf_format,
     uint8_t *format )
{
	static char *function = "libewf_header_sections_determine_format";

	if( header_sections == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid format.\n",
		 function );

		return( -1 );
	}
	if( ewf_format == EWF_FORMAT_S01 )
	{
		/* The format identifier for the EWF-S01 format was already set
		 * while reading the volume section
		 */
	}
	else if( ewf_format == EWF_FORMAT_E01 )
	{
		if( header_sections->xheader != NULL )
		{
			*format = LIBEWF_FORMAT_EWFX;
		}
		/* The header2 in raw format starts with 0xff 0xfe <number>
		 */
		else if( header_sections->header2 != NULL )
		{
			if( header_sections->header2[ 2 ] == (ewf_char_t) '3' )
			{
				/* The EnCase5 header2 contains av on the 6th position (36 ... 38 ...)
				 * the header2 is an UTF16 string
				 */
				if( ( header_sections->header2[ 36 ] == (ewf_char_t) 'a' )
				 && ( header_sections->header2[ 38 ] == (ewf_char_t) 'v' ) )
				{
					*format = LIBEWF_FORMAT_ENCASE5;
				}
				else if( ( header_sections->header2[ 36 ] == (ewf_char_t) 'm' )
				 && ( header_sections->header2[ 38 ] == (ewf_char_t) 'd' ) )
				{
					*format = LIBEWF_FORMAT_ENCASE6;
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unsupported header2 format: %c%c.\n",
					 function, (char) header_sections->header2[ 36 ],
					 (char) header_sections->header2[ 38 ] );

					return( -1 );
				}
			}
			else if( header_sections->header2[ 2 ] == (ewf_char_t) '1' )
			{
				*format = LIBEWF_FORMAT_ENCASE4;
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: unsupported header2 version: %c.\n",
				 function, (char) header_sections->header2[ 2 ] );

				return( -1 );
			}
		}
		else if( header_sections->header != NULL )
		{
			if( header_sections->header[ 0 ] == (ewf_char_t) '3' )
			{
				/* The linen5 header2 contains av on the 6th position (17 18)
				 * the header2 is an UTF16 string
				 */
				if( ( header_sections->header[ 17 ] == (ewf_char_t) 'a' )
				 && ( header_sections->header[ 18 ] == (ewf_char_t) 'v' ) )
				{
					*format = LIBEWF_FORMAT_LINEN5;
				}
				else if( ( header_sections->header[ 17 ] == (ewf_char_t) 'm' )
				 && ( header_sections->header[ 18 ] == (ewf_char_t) 'd' ) )
				{
					*format = LIBEWF_FORMAT_LINEN6;
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unsupported header format: %c%c.\n",
					 function, (char) header_sections->header[ 17 ],
					 (char) header_sections->header[ 18 ] );

					return( -1 );
				}
			}
			else if( header_sections->header[ 0 ] == (ewf_char_t) '1' )
			{
				/* EnCase uses \r\n
				 */
				if( header_sections->header[ 1 ] == (ewf_char_t) '\r' )
				{
					if( header_sections->header[ 25 ] == (ewf_char_t) 'r' )
					{
						*format = LIBEWF_FORMAT_ENCASE1;

						if( header_sections->amount_of_header_sections != 1 )
						{
							LIBEWF_VERBOSE_PRINT( "%s: multiple header sections found.\n",
							 function );
						}
					}
					else if( header_sections->header[ 31 ] == (ewf_char_t) 'r' )
					{
						*format = LIBEWF_FORMAT_ENCASE2;
					}
					else
					{
						LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
						 function );

						return( -1 );
					}
				}
				/* FTK Imager uses \n
				 */
				else if( header_sections->header[ 1 ] == (ewf_char_t) '\n' )
				{
					if( header_sections->header[ 29 ] == (ewf_char_t) 'r' )
					{
						*format = LIBEWF_FORMAT_FTK;
					}
					else
					{
						LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
						 function );

						return( -1 );
					}
				}
				else
				{
					LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
					 function );

					return( -1 );
				}
			}
			else
			{
				LIBEWF_WARNING_PRINT( "%s: unsupported header version.\n",
				 function );

				return( -1 );
			}
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: missing header information.\n",
			 function );

			return( -1 );
		}
	}
	else if( ewf_format == EWF_FORMAT_L01 )
	{
		*format = LIBEWF_FORMAT_LVF;
	}
	else
	{
		LIBEWF_WARNING_PRINT( "%s: unsupported EWF file format.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}


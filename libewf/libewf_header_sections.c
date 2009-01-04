/*
 * Header sections functions
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

#include <libewf/definitions.h>

#include "libewf_header_sections.h"
#include "libewf_header_values.h"
#include "libewf_string.h"

#include "ewf_definitions.h"

/* Initialize the header sections
 * Returns 1 if successful or -1 on error
 */
int libewf_header_sections_initialize(
     libewf_header_sections_t **header_sections )
{
	static char *function = "libewf_header_sections_initialize";

	if( header_sections == NULL )
	{
		notify_warning_printf( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( *header_sections == NULL )
	{
		*header_sections = (libewf_header_sections_t *) memory_allocate(
		                                                 sizeof( libewf_header_sections_t ) );

		if( *header_sections == NULL )
		{
			notify_warning_printf( "%s: unable to create header sections.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *header_sections,
		     0,
		     sizeof( libewf_header_sections_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear header sections.\n",
			 function );

			memory_free(
			 *header_sections );

			*header_sections = NULL;

			return( -1 );
		}
		( *header_sections )->header_codepage = LIBEWF_CODEPAGE_ASCII;
	}
	return( 1 );
}

/* Frees the header sections including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_header_sections_free(
      libewf_header_sections_t **header_sections )
{
        static char *function = "libewf_header_sections_free";

	if( header_sections == NULL )
	{
		notify_warning_printf( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( *header_sections != NULL )
	{
		if( ( *header_sections )->header != NULL )
		{
			memory_free(
			 ( *header_sections )->header );
		}
		if( ( *header_sections )->header2 != NULL )
		{
			memory_free(
			 ( *header_sections )->header2 );
		}
		if( ( *header_sections )->xheader != NULL )
		{
			memory_free(
			 ( *header_sections )->xheader );
		}
		memory_free(
		 *header_sections );

		*header_sections = NULL;
	}
	return( 1 );
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
		notify_warning_printf( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_EWF )
	{
		if( libewf_header_values_generate_header_ewf(
		     header_values,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE1 )
	{
		if( libewf_header_values_generate_header_encase1(
		     header_values,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( ( format == LIBEWF_FORMAT_ENCASE2 )
	 || ( format == LIBEWF_FORMAT_ENCASE3 ) )
	{
		if( libewf_header_values_generate_header_encase2(
		     header_values,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( ( format == LIBEWF_FORMAT_FTK )
	 || ( format == LIBEWF_FORMAT_SMART ) )
	{
		if( libewf_header_values_generate_header_ftk(
		     header_values,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE4 )
	{
		if( libewf_header_values_generate_header_encase4(
		     header_values,
		     timestamp,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		if( libewf_header_values_generate_header2_encase4(
		     header_values,
		     timestamp,
		     &( header_sections->header2 ),
		     &( header_sections->header2_size ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header2 section.\n",
			 function );

			memory_free(
			 header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE5 )
	{
		if( libewf_header_values_generate_header_encase4(
		     header_values,
		     timestamp,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		if( libewf_header_values_generate_header2_encase5(
		     header_values,
		     timestamp,
		     &( header_sections->header2 ),
		     &( header_sections->header2_size ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header2 sections.\n",
			 function );

			memory_free(
			 header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_ENCASE6 )
	{
		if( libewf_header_values_generate_header_encase4(
		     header_values,
		     timestamp,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		if( libewf_header_values_generate_header2_encase6(
		     header_values,
		     timestamp,
		     &( header_sections->header2 ),
		     &( header_sections->header2_size ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header2 sections.\n",
			 function );

			memory_free(
			 header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_LINEN5 )
	{
		if( libewf_header_values_generate_header_linen5(
		     header_values,
		     timestamp,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_LINEN6 )
	{
		if( libewf_header_values_generate_header_linen6(
		     header_values,
		     timestamp,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
	}
	else if( format == LIBEWF_FORMAT_EWFX )
	{
		if( libewf_header_values_generate_header_ewfx(
		     header_values,
		     timestamp,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_sections->header_codepage ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header section.\n",
			 function );

			return( -1 );
		}
		if( libewf_header_values_generate_header2_ewfx(
		     header_values,
		     timestamp,
		     &( header_sections->header2 ),
		     &( header_sections->header2_size ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create header2 section.\n",
			 function );

			memory_free(
			 header_sections->header );

			header_sections->header      = NULL;
			header_sections->header_size = 0;

			return( -1 );
		}
		if( libewf_header_values_generate_xheader_ewfx(
		     header_values,
		     timestamp,
		     &( header_sections->xheader ),
		     &( header_sections->xheader_size ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to create xheader section.\n",
			 function );

			memory_free(
			 header_sections->header );
			memory_free(
			 header_sections->header2 );

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
		notify_warning_printf( "%s: invalid header sections.\n",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		notify_warning_printf( "%s: invalid format.\n",
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
			if( header_sections->header2[ 2 ] == (uint8_t) '3' )
			{
				/* The EnCase5 header2 contains av on the 6th position (36 ... 38 ...)
				 * the header2 is an UTF16 string
				 */
				if( ( header_sections->header2[ 36 ] == (uint8_t) 'a' )
				 && ( header_sections->header2[ 38 ] == (uint8_t) 'v' ) )
				{
					*format = LIBEWF_FORMAT_ENCASE5;
				}
				else if( ( header_sections->header2[ 36 ] == (uint8_t) 'm' )
				 && ( header_sections->header2[ 38 ] == (uint8_t) 'd' ) )
				{
					*format = LIBEWF_FORMAT_ENCASE6;
				}
				else
				{
					notify_warning_printf( "%s: unsupported header2 format: %c%c.\n",
					 function, (char) header_sections->header2[ 36 ],
					 (char) header_sections->header2[ 38 ] );

					return( -1 );
				}
			}
			else if( header_sections->header2[ 2 ] == (uint8_t) '1' )
			{
				*format = LIBEWF_FORMAT_ENCASE4;
			}
			else
			{
				notify_warning_printf( "%s: unsupported header2 version: %c.\n",
				 function, (char) header_sections->header2[ 2 ] );

				return( -1 );
			}
		}
		else if( header_sections->header != NULL )
		{
			if( header_sections->header[ 0 ] == (uint8_t) '3' )
			{
				/* The linen5 header2 contains av on the 6th position (17 18)
				 * the header2 is an UTF16 string
				 */
				if( ( header_sections->header[ 17 ] == (uint8_t) 'a' )
				 && ( header_sections->header[ 18 ] == (uint8_t) 'v' ) )
				{
					*format = LIBEWF_FORMAT_LINEN5;
				}
				else if( ( header_sections->header[ 17 ] == (uint8_t) 'm' )
				 && ( header_sections->header[ 18 ] == (uint8_t) 'd' ) )
				{
					*format = LIBEWF_FORMAT_LINEN6;
				}
				else
				{
					notify_warning_printf( "%s: unsupported header format: %c%c.\n",
					 function, (char) header_sections->header[ 17 ],
					 (char) header_sections->header[ 18 ] );

					return( -1 );
				}
			}
			else if( header_sections->header[ 0 ] == (uint8_t) '1' )
			{
				/* EnCase uses \r\n
				 */
				if( header_sections->header[ 1 ] == (uint8_t) '\r' )
				{
					if( header_sections->header[ 25 ] == (uint8_t) 'r' )
					{
						*format = LIBEWF_FORMAT_ENCASE1;

						if( header_sections->amount_of_header_sections != 1 )
						{
							notify_verbose_printf( "%s: multiple header sections found.\n",
							 function );
						}
					}
					else if( header_sections->header[ 31 ] == (uint8_t) 'r' )
					{
						*format = LIBEWF_FORMAT_ENCASE2;
					}
					else
					{
						notify_warning_printf( "%s: unsupported header version.\n",
						 function );

						return( -1 );
					}
				}
				/* FTK Imager uses \n
				 */
				else if( header_sections->header[ 1 ] == (uint8_t) '\n' )
				{
					if( header_sections->header[ 29 ] == (uint8_t) 'r' )
					{
						*format = LIBEWF_FORMAT_FTK;
					}
					else
					{
						notify_warning_printf( "%s: unsupported header version.\n",
						 function );

						return( -1 );
					}
				}
				else
				{
					notify_warning_printf( "%s: unsupported header version.\n",
					 function );

					return( -1 );
				}
			}
			else
			{
				notify_warning_printf( "%s: unsupported header version.\n",
				 function );

				return( -1 );
			}
		}
		else
		{
			notify_warning_printf( "%s: missing header information.\n",
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
		notify_warning_printf( "%s: unsupported EWF file format.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}


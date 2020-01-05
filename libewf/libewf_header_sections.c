/*
 * Header sections functions
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
#include <memory.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_io_handle.h"
#include "libewf_header_sections.h"
#include "libewf_header_values.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"

/* Creates header sections
 * Make sure the value header_sections is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_header_sections_initialize(
     libewf_header_sections_t **header_sections,
     libcerror_error_t **error )
{
	static char *function = "libewf_header_sections_initialize";

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	if( *header_sections != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid header sections value already set.",
		 function );

		return( -1 );
	}
	*header_sections = memory_allocate_structure(
	                    libewf_header_sections_t );

	if( *header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create header sections.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *header_sections,
	     0,
	     sizeof( libewf_header_sections_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear header sections.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *header_sections != NULL )
	{
		memory_free(
		 *header_sections );

		*header_sections = NULL;
	}
	return( -1 );
}

/* Frees header sections
 * Returns 1 if successful or -1 on error
 */
int libewf_header_sections_free(
      libewf_header_sections_t **header_sections,
      libcerror_error_t **error )
{
        static char *function = "libewf_header_sections_free";

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
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

/* Clones the header sections
 * Returns 1 if successful or -1 on error
 */
int libewf_header_sections_clone(
     libewf_header_sections_t **destination_header_sections,
     libewf_header_sections_t *source_header_sections,
     libcerror_error_t **error )
{
	static char *function = "libewf_header_sections_clone";

	if( destination_header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination header sections.",
		 function );

		return( -1 );
	}
	if( *destination_header_sections != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination header sections already set.",
		 function );

		return( -1 );
	}
	if( source_header_sections == NULL )
	{
		*destination_header_sections = NULL;

		return( 1 );
	}
	if( libewf_header_sections_initialize(
	     destination_header_sections,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination header sections.",
		 function );

		goto on_error;
	}
	if( *destination_header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination header sections.",
		 function );

		goto on_error;
	}
	if( source_header_sections->header != NULL )
	{
		( *destination_header_sections )->header = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_header_sections->header_size );

		if( ( *destination_header_sections )->header == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination header.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_header_sections )->header,
		     source_header_sections->header,
		     sizeof( uint8_t ) * source_header_sections->header_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination header.",
			 function );

			goto on_error;
		}
		( *destination_header_sections )->header_size = source_header_sections->header_size;
	}
	if( source_header_sections->header2 != NULL )
	{
		( *destination_header_sections )->header2 = (uint8_t *) memory_allocate(
		                                                         sizeof( uint8_t ) * source_header_sections->header2_size );

		if( ( *destination_header_sections )->header2 == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination header2.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_header_sections )->header2,
		     source_header_sections->header2,
		     sizeof( uint8_t ) * source_header_sections->header2_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination header2.",
			 function );

			goto on_error;
		}
		( *destination_header_sections )->header2_size = source_header_sections->header2_size;
	}
	if( source_header_sections->xheader != NULL )
	{
		( *destination_header_sections )->xheader = (uint8_t *) memory_allocate(
		                                                         sizeof( uint8_t ) * source_header_sections->xheader_size );

		if( ( *destination_header_sections )->xheader == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination xheader.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_header_sections )->xheader,
		     source_header_sections->xheader,
		     sizeof( uint8_t ) * source_header_sections->xheader_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination xheader.",
			 function );

			goto on_error;
		}
		( *destination_header_sections )->xheader_size = source_header_sections->xheader_size;
	}
	( *destination_header_sections )->number_of_header_sections = source_header_sections->number_of_header_sections;

	return( 1 );

on_error:
	if( *destination_header_sections != NULL )
	{
		if( ( *destination_header_sections )->xheader != NULL )
		{
			memory_free(
			 ( *destination_header_sections )->xheader );
		}
		if( ( *destination_header_sections )->header2 != NULL )
		{
			memory_free(
			 ( *destination_header_sections )->header2 );
		}
		if( ( *destination_header_sections )->header != NULL )
		{
			memory_free(
			 ( *destination_header_sections )->header );
		}
		memory_free(
		 *destination_header_sections );

		*destination_header_sections = NULL;
	}
	return( -1 );
}

/* Create the header sections from the header values
 * Returns 1 on success or -1 on error
 */
int libewf_header_sections_generate(
     libewf_header_sections_t *header_sections,
     libfvalue_table_t *header_values,
     time_t timestamp,
     int8_t compression_level,
     uint8_t format,
     int header_codepage,
     libcerror_error_t **error )
{
	static char *function = "libewf_header_sections_generate";

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
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
		     header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header section.",
			 function );

			goto on_error;
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
		     header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header section.",
			 function );

			goto on_error;
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
		     header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header section.",
			 function );

			goto on_error;
		}
	}
	else if( ( format == LIBEWF_FORMAT_FTK_IMAGER )
	      || ( format == LIBEWF_FORMAT_SMART ) )
	{
		if( libewf_header_values_generate_header_ftk(
		     header_values,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header section.",
			 function );

			goto on_error;
		}
	}
	else if( ( format == LIBEWF_FORMAT_ENCASE4 )
	      || ( format == LIBEWF_FORMAT_ENCASE5 )
	      || ( format == LIBEWF_FORMAT_ENCASE6 )
	      || ( format == LIBEWF_FORMAT_ENCASE7 ) )
	{
		if( libewf_header_values_generate_header_encase4(
		     header_values,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header section.",
			 function );

			goto on_error;
		}
		if( libewf_header_values_generate_header2(
		     header_values,
		     format,
		     timestamp,
		     compression_level,
		     &( header_sections->header2 ),
		     &( header_sections->header2_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header2 section.",
			 function );

			goto on_error;
		}
	}
	else if( ( format == LIBEWF_FORMAT_LINEN5 )
	      || ( format == LIBEWF_FORMAT_LINEN6 )
	      || ( format == LIBEWF_FORMAT_LINEN7 ) )
	{
		if( libewf_header_values_generate_header_linen(
		     header_values,
		     format,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header section.",
			 function );

			goto on_error;
		}
	}
	else if( format == LIBEWF_FORMAT_EWFX )
	{
		if( libewf_header_values_generate_header_ewfx(
		     header_values,
		     timestamp,
		     compression_level,
		     &( header_sections->header ),
		     &( header_sections->header_size ),
		     header_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header section.",
			 function );

			goto on_error;
		}
		if( libewf_header_values_generate_header2(
		     header_values,
		     format,
		     timestamp,
		     compression_level,
		     &( header_sections->header2 ),
		     &( header_sections->header2_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create header2 section.",
			 function );

			goto on_error;
		}
		if( libewf_header_values_generate_xheader(
		     header_values,
		     timestamp,
		     &( header_sections->xheader ),
		     &( header_sections->xheader_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create xheader section.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( header_sections->header2 != NULL )
	{
		memory_free(
		 header_sections->header2 );

		header_sections->header2      = NULL;
		header_sections->header2_size = 0;
	}
	if( header_sections->header != NULL )
	{
		memory_free(
		 header_sections->header );

		header_sections->header      = NULL;
		header_sections->header_size = 0;
	}
	return( -1 );
}

/* Parses the header, header2 and/or xheader section for header values
 * Returns 1 if successful or -1 on error
 */
int libewf_header_sections_parse(
     libewf_header_sections_t *header_sections,
     libewf_io_handle_t *io_handle,
     libfvalue_table_t *header_values,
     uint8_t *format,
     libcerror_error_t **error )
{
	static char *function = "libewf_header_sections_parse";
	int result_header     = 1;
	int result_header2    = 1;
	int result_xheader    = 1;

	if( header_sections == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header sections.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format.",
		 function );

		return( -1 );
	}
	/* For EWF version 1 format read all the header sections
	 * and overwrite values by the most specific data
	 * This also applied to the format
	 */
	if( header_sections->header != NULL )
	{
		if( libewf_header_values_parse_header(
		     header_values,
		     header_sections->header,
		     header_sections->header_size,
		     io_handle->header_codepage,
		     format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse header.",
			 function );

			result_header = -1;
		}
	}
	if( header_sections->header2 != NULL )
	{
		if( libewf_header_values_parse_header2(
		     header_values,
		     header_sections->header2,
		     header_sections->header2_size,
		     format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse header2.",
			 function );

			result_header2 = -1;
		}
	}
	if( header_sections->xheader != NULL )
	{
		if( libewf_header_values_parse_xheader(
		     header_values,
		     header_sections->xheader,
		     header_sections->xheader_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to parse xheader.",
			 function );

			result_xheader = -1;
		}
		*format = LIBEWF_FORMAT_EWFX;
	}
	if( ( result_header != 1 )
	 && ( result_header2 != 1 )
	 && ( result_xheader != 1 ) )
	{
		return( -1 );
	}
	if( ( result_header != 1 )
	 || ( result_header2 != 1 )
	 || ( result_xheader != 1 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
		}
#endif
		libcerror_error_free(
		 error );
	}
	return( 1 );
}


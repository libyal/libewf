/*
 * Device information functions
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
#include "libewf_device_information.h"
#include "libewf_header_values.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_media_values.h"
#include "libewf_value_table.h"

/* Generate an UTF-8 encoded device information string
 * Sets utf8_string and utf8_string_size
 * Returns 1 if successful or -1 on error
 */
int libewf_device_information_generate_utf8_string(
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error )
{
	uint8_t *safe_utf8_string               = NULL;
	static char *function                   = "libewf_device_information_generate_utf8_string";
	const char *newline_string              = "\n";
	size_t bytes_per_sector_string_length   = 0;
	size_t device_label_string_length       = 0;
	size_t model_string_length              = 0;
	size_t newline_string_length            = 1;
	size_t number_of_sectors_string_length  = 0;
	size_t process_identifier_string_length = 0;
	size_t safe_utf8_string_size            = 0;
	size_t serial_number_string_length      = 0;
	size_t utf8_string_index                = 0;
	uint64_t value_64bit                    = 0;
	int number_of_characters                = 0;
	int number_of_tabs                      = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( *utf8_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: UTF-8 string already set.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "model",
	     6,
	     &model_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: model length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "serial_number",
	     14,
	     &serial_number_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: serial_number length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "device_label",
	     13,
	     &device_label_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: device_label length.",
		 function );

		goto on_error;
	}
	if( libewf_value_table_get_value_utf8_string_length(
	     header_values,
	     (uint8_t *) "process_identifier",
	     19,
	     &process_identifier_string_length,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve header value: process_identifier length.",
		 function );

		goto on_error;
	}
	/* Determine the string size
	 * Reserve space for:
	 * 1 <newline>
	 * main <newline>
	 */
	safe_utf8_string_size = 5 + ( 2 * newline_string_length );

	/* Reserve space for:
	 * sn <tab> md <tab> lb <tab> ts <tab> hs <tab> dc <tab> dt <tab> pid <tab> rs <tab> ls <tab> bp <tab> ph <newline>
	 */
	number_of_characters = 25;
	number_of_tabs       = 11;

	safe_utf8_string_size += number_of_characters + number_of_tabs + newline_string_length;

	/* Reserve space for the values
	 */
	safe_utf8_string_size += serial_number_string_length
	                       + model_string_length
	                       + device_label_string_length;

	/* If we do a streamed write reserve space for the final value
	 */
	if( media_values->media_size == 0 )
	{
		value_64bit   = 1;
		value_64bit <<= 63;
		value_64bit  -= 1;
	}
	else
	{
		value_64bit = media_values->number_of_sectors;
	}
	if( libfvalue_string_size_from_integer(
	     &number_of_sectors_string_length,
	     value_64bit,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string size of number of sectors.",
		 function );

		goto on_error;
	}
	if( number_of_sectors_string_length > 0 )
	{
		number_of_sectors_string_length -= 1;

		safe_utf8_string_size += number_of_sectors_string_length;
	}
/* TODO: hs, dc */

	/* Reserve space for the media (or drive) type */
	safe_utf8_string_size += 1;

	safe_utf8_string_size += process_identifier_string_length;

/* TODO: rs, ls */

	if( libfvalue_string_size_from_integer(
	     &bytes_per_sector_string_length,
	     media_values->bytes_per_sector,
	     32,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string size of bytes per sector.",
		 function );

		goto on_error;
	}
	if( bytes_per_sector_string_length > 0 )
	{
		bytes_per_sector_string_length -= 1;

		safe_utf8_string_size += bytes_per_sector_string_length;
	}
	if( ( media_values->media_flags & LIBEWF_MEDIA_FLAG_PHYSICAL ) != 0 )
	{
		safe_utf8_string_size += 1;
	}
	/* Reserve space for the tabs and 2 newlines
	 */
	safe_utf8_string_size += number_of_tabs + ( 2 * newline_string_length );

	/* Reserve space for the end-of-string character
	 */
	safe_utf8_string_size += 1;

	if( safe_utf8_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		goto on_error;
	}
	/* Determine the string
	 */
	safe_utf8_string = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * safe_utf8_string_size );

	if( safe_utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '1';

	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'm';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'a';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'i';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';

	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'n';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'm';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'd';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'l';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'b';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'h';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'd';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'c';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'd';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 't';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'p';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'i';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'd';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'r';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'l';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 's';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'b';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'p';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'p';
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'h';

	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	if( serial_number_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "serial_number",
		     14,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: serial_number to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( model_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "model",
		     6,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: model to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( device_label_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "device_label",
		     13,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: device_label to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( number_of_sectors_string_length > 0 )
	{
		/* If we do a streamed write reserve space for the final value
		 */
		if( media_values->media_size == 0 )
		{
			value_64bit   = 1;
			value_64bit <<= 63;
			value_64bit  -= 1;
		}
		else
		{
			value_64bit = media_values->number_of_sectors;
		}
		if( libfvalue_utf8_string_with_index_copy_from_integer(
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     value_64bit,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy number of sectors to string.",
			 function );

			goto on_error;
		}
		utf8_string_index--;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

/* TODO: set hs */
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

/* TODO: set dc */
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

/* TODO add support for media types: RAM disk, PALM */
	switch( media_values->media_type )
	{
		case LIBEWF_MEDIA_TYPE_REMOVABLE:
			safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'r';
			break;

		case LIBEWF_MEDIA_TYPE_FIXED:
			safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'f';
			break;

		case LIBEWF_MEDIA_TYPE_OPTICAL:
			safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'c';
			break;

		case LIBEWF_MEDIA_TYPE_SINGLE_FILES:
			safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'l';
			break;

		case LIBEWF_MEDIA_TYPE_MEMORY:
			safe_utf8_string[ utf8_string_index++ ] = (uint8_t) 'm';
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported media type.",
			 function );

			goto on_error;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( process_identifier_string_length > 0 )
	{
		if( libewf_value_table_get_value_copy_to_utf8_string_with_index(
		     header_values,
		     (uint8_t *) "process_identifier",
		     19,
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy header value: process_identifier to string.",
			 function );

			goto on_error;
		}
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

/* TODO: rs */
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

/* TODO: ls */
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( bytes_per_sector_string_length > 0 )
	{
		if( libfvalue_utf8_string_with_index_copy_from_integer(
		     safe_utf8_string,
		     safe_utf8_string_size,
		     &utf8_string_index,
		     media_values->bytes_per_sector,
		     32,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy bytes per sector to string.",
			 function );

			goto on_error;
		}
		utf8_string_index--;
	}
	safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '\t';

	if( ( media_values->media_flags & LIBEWF_MEDIA_FLAG_PHYSICAL ) != 0 )
	{
		safe_utf8_string[ utf8_string_index++ ] = (uint8_t) '1';
	}
	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = newline_string[ 0 ];

	if( newline_string_length == 2 )
	{
		safe_utf8_string[ utf8_string_index++ ] = newline_string[ 1 ];
	}
	safe_utf8_string[ utf8_string_index++ ] = 0;

	*utf8_string      = safe_utf8_string;
	*utf8_string_size = safe_utf8_string_size;

	return( 1 );

on_error:
	if( safe_utf8_string != NULL )
	{
		memory_free(
		 safe_utf8_string );
	}
	return( -1 );
}

/* Generate device information
 * Sets device_information and device_information_size
 * Returns 1 if successful or -1 on error
 */
int libewf_device_information_generate(
     uint8_t **device_information,
     size_t *device_information_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error )
{
	uint8_t *safe_device_information    = NULL;
	uint8_t *utf8_string                = NULL;
	static char *function               = "libewf_device_information_generate";
	size_t safe_device_information_size = 0;
	size_t utf8_string_size             = 0;

	if( device_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information.",
		 function );

		return( -1 );
	}
	if( *device_information != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: device information already created.",
		 function );

		return( -1 );
	}
	if( device_information_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information size.",
		 function );

		return( -1 );
	}
	if( libewf_device_information_generate_utf8_string(
	     &utf8_string,
	     &utf8_string_size,
	     media_values,
	     header_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create UTF-8 device information string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: device information string:\n%s",
		 function,
		 utf8_string );
	}
#endif
	if( libuna_utf16_stream_size_from_utf8(
	     utf8_string,
	     utf8_string_size,
	     &safe_device_information_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine device information size.",
		 function );

		goto on_error;
	}
	if( ( safe_device_information_size == 0 )
	 || ( safe_device_information_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid device information size value out of bounds.",
		 function );

		goto on_error;
	}
	safe_device_information = (uint8_t *) memory_allocate(
	                                       sizeof( uint8_t ) * safe_device_information_size );

	if( safe_device_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create device information.",
		 function );

		goto on_error;
	}
	if( libuna_utf16_stream_copy_from_utf8(
	     safe_device_information,
	     safe_device_information_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set device information.",
		 function );

		goto on_error;
	}
	memory_free(
	 utf8_string );

	*device_information      = safe_device_information;
	*device_information_size = safe_device_information_size;

	return( 1 );

on_error:
	if( utf8_string != NULL )
	{
		memory_free(
		 utf8_string );
	}
	if( safe_device_information != NULL )
	{
		memory_free(
		 safe_device_information );
	}
	return( -1 );
}

/* Parses an UTF-8 encoded device information string
 * Returns 1 if successful or -1 on error
 */
int libewf_device_information_parse_utf8_string(
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *lines  = NULL;
	libfvalue_split_utf8_string_t *types  = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_device_information_parse_utf8_string";
	size_t line_string_size               = 0;
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	int number_of_lines                   = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     utf8_string,
	     utf8_string_size,
	     (uint8_t) '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split UTF-8 string into lines.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     lines,
	     &number_of_lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of lines.",
		 function );

		goto on_error;
	}
	if( number_of_lines > 0 )
	{
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     0,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 0.",
			 function );

			goto on_error;
		}
		if( line_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 0.",
			 function );

			goto on_error;
		}
		if( ( line_string == NULL )
		 || ( line_string_size < 2 )
		 || ( line_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 0.",
			 function );

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if( line_string[ line_string_size - 2 ] == (uint8_t) '\r' )
		{
			line_string[ line_string_size - 2 ] = 0;

			line_string_size -= 1;
		}
		if( line_string_size != 2 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 0.",
			 function );

			goto on_error;
		}
		if( line_string[ 0 ] < (uint8_t) '1' )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 0.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     1,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 1.",
			 function );

			goto on_error;
		}
		if( line_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 1.",
			 function );

			goto on_error;
		}
		if( ( line_string == NULL )
		 || ( line_string_size < 5 )
		 || ( line_string[ 0 ] == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing line string: 1.",
			 function );

			goto on_error;
		}
		/* Remove trailing carriage return
		 */
		else if( line_string[ line_string_size - 2 ] == (uint8_t) '\r' )
		{
			line_string[ line_string_size - 2 ] = 0;

			line_string_size -= 1;
		}
		if( line_string_size != 5 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 1.",
			 function );

			goto on_error;
		}
		if( ( line_string[ 0 ] != (uint8_t) 'm' )
		 || ( line_string[ 1 ] != (uint8_t) 'a' )
		 || ( line_string[ 2 ] != (uint8_t) 'i' )
		 || ( line_string[ 3 ] != (uint8_t) 'n' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported line string: 1.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     2,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 2.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_split(
		     line_string,
		     line_string_size,
		     (uint8_t) '\t',
		     &types,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split device information string into types.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_number_of_segments(
		     types,
		     &number_of_types,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of types.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_segment_by_index(
		     lines,
		     3,
		     &line_string,
		     &line_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve line string: 3.",
			 function );

			goto on_error;
		}
		if( libfvalue_utf8_string_split(
		     line_string,
		     line_string_size,
		     (uint8_t) '\t',
		     &values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to split device information string into values.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_get_number_of_segments(
		     values,
		     &number_of_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of values.",
			 function );

			goto on_error;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( number_of_types != number_of_values )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
			 	"%s: mismatch in number of types and values.\n",
				 function );
			}
		}
#endif
		for( value_index = 0;
		     value_index < number_of_types;
		     value_index++ )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     types,
			     value_index,
			     &type_string,
			     &type_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve type string: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( value_index >= number_of_values )
			{
				value_string      = NULL;
				value_string_size = 0;
			}
			else if( libfvalue_split_utf8_string_get_segment_by_index(
			          values,
			          value_index,
			          &value_string,
			          &value_string_size,
			          error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( libewf_device_information_parse_utf8_string_value(
			     type_string,
			     type_string_size,
			     value_string,
			     value_string_size,
			     value_index,
			     media_values,
			     header_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to parse UTF-8 string value: %d.",
				 function,
				 value_index );

				goto on_error;
			}
		}
		if( libfvalue_split_utf8_string_free(
		     &values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split values.",
			 function );

			goto on_error;
		}
		if( libfvalue_split_utf8_string_free(
		     &types,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free split types.",
			 function );

			goto on_error;
		}
	}
	if( libfvalue_split_utf8_string_free(
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split lines.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"\n" );
	}
#endif
	return( 1 );

on_error:
	if( values != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &values,
		 NULL );
	}
	if( types != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &types,
		 NULL );
	}
	if( lines != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &lines,
		 NULL );
	}
	return( -1 );
}

/* Parses an UTF-8 encoded device information string value
 * Returns 1 if successful or -1 on error
 */
int libewf_device_information_parse_utf8_string_value(
     uint8_t *type_string,
     size_t type_string_size,
     uint8_t *value_string,
     size_t value_string_size,
     int value_index,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error )
{
	uint8_t *identifier    = NULL;
	static char *function  = "libewf_device_information_parse_utf8_string_value";
	size_t identifier_size = 0;
	uint64_t value_64bit   = 0;

	if( ( type_string == NULL )
	 || ( type_string_size < 2 )
	 || ( type_string[ 0 ] == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing type string: %d.",
		 function,
		 value_index );

		return( -1 );
	}
	if( media_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media values.",
		 function );

		return( -1 );
	}
	if( header_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header values.",
		 function );

		return( -1 );
	}
	/* Remove trailing carriage return
	 */
	if( type_string[ type_string_size - 2 ] == (uint8_t) '\r' )
	{
		type_string[ type_string_size - 2 ] = 0;

		type_string_size -= 1;
	}
	if( ( value_string == NULL )
	 || ( value_string_size < 2 )
	 || ( value_string[ 0 ] == 0 ) )
	{
		value_string      = NULL;
		value_string_size = 0;
	}
	/* Remove trailing carriage return
	 */
	else if( value_string[ value_string_size - 2 ] == (uint8_t) '\r' )
	{
		value_string[ value_string_size - 2 ] = 0;

		value_string_size -= 1;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type: %s with value: %s.\n",
		 function,
		 (char *) type_string,
		 (char *) value_string );
	}
#endif
	/* Ignore empty values
	 */
	if( value_string == NULL )
	{
		return( 1 );
	}
	identifier      = NULL;
	identifier_size = 0;

	if( type_string_size == 4 )
	{
		if( ( type_string[ 0 ] == (uint8_t) 'p' )
		 && ( type_string[ 1 ] == (uint8_t) 'i' )
		 && ( type_string[ 2 ] == (uint8_t) 'd' ) )
		{
			identifier      = (uint8_t *) "process_identifier";
			identifier_size = 19;
		}
	}
	else if( type_string_size == 3 )
	{
		if( ( type_string[ 0 ] == (uint8_t) 'b' )
		 && ( type_string[ 1 ] == (uint8_t) 'p' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     32,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set bytes per sector.",
				 function );

				return( -1 );
			}
			media_values->bytes_per_sector = (uint32_t) value_64bit;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'd' )
		      && ( type_string[ 1 ] == (uint8_t) 'c' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     64,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set number of DCO protected sectors.",
				 function );

				return( -1 );
			}
/* TODO number of DCO protected sectors */
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'd' )
		      && ( type_string[ 1 ] == (uint8_t) 't' ) )
		{
			if( value_string_size == 2 )
			{
				switch( value_string[ 0 ] )
				{
/* TODO need the EWF1 equivalent value
					case (uint8_t) 'a':
						media_values->media_type = LIBEWF_MEDIA_TYPE_RAM_DISK;
						break;
*/

					case (uint8_t) 'c':
						media_values->media_type = LIBEWF_MEDIA_TYPE_OPTICAL;
						break;

					case (uint8_t) 'f':
						media_values->media_type = LIBEWF_MEDIA_TYPE_FIXED;
						break;

					case (uint8_t) 'l':
						media_values->media_type = LIBEWF_MEDIA_TYPE_SINGLE_FILES;
						break;

					case (uint8_t) 'm':
						media_values->media_type = LIBEWF_MEDIA_TYPE_MEMORY;
						break;

/* TODO need the EWF1 equivalent value
					case (uint8_t) 'p':
						media_values->media_type = LIBEWF_MEDIA_TYPE_PALM;
						break;
*/

					case (uint8_t) 'r':
						media_values->media_type = LIBEWF_MEDIA_TYPE_REMOVABLE;
						break;

#if defined( HAVE_DEBUG_OUTPUT )
					default:
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							"%s: unsupported drive type.\n",
							 function );
						}
						break;
#endif
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				"%s: unsupported drive type.\n",
				 function );
			}
#endif
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'h' )
		      && ( type_string[ 1 ] == (uint8_t) 's' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     64,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set number of HPA protected sectors.",
				 function );

				return( -1 );
			}
/* TODO number of HPA protected sectors */
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'l' )
		      && ( type_string[ 1 ] == (uint8_t) 'b' ) )
		{
			identifier      = (uint8_t *) "device_label";
			identifier_size = 13;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'l' )
		      && ( type_string[ 1 ] == (uint8_t) 's' ) )
		{
/* TODO number of sectors of SMART logs */
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'm' )
		      && ( type_string[ 1 ] == (uint8_t) 'd' ) )
		{
			identifier      = (uint8_t *) "model";
			identifier_size = 6;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'p' )
		      && ( type_string[ 1 ] == (uint8_t) 'h' ) )
		{
			if( ( value_string_size == 2 )
			 && ( value_string[ 0 ] == (uint8_t) '1' ) )
			{
				media_values->media_flags |= LIBEWF_MEDIA_FLAG_PHYSICAL;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			else if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				"%s: unsupported is physical.\n",
				 function );
			}
#endif
		}
		else if( ( type_string[ 0 ] == (uint8_t) 'r' )
		      && ( type_string[ 1 ] == (uint8_t) 's' ) )
		{
/* TODO number of sectors of PALM RAM device */
		}
		else if( ( type_string[ 0 ] == (uint8_t) 's' )
		      && ( type_string[ 1 ] == (uint8_t) 'n' ) )
		{
			identifier      = (uint8_t *) "serial_number";
			identifier_size = 14;
		}
		else if( ( type_string[ 0 ] == (uint8_t) 't' )
		      && ( type_string[ 1 ] == (uint8_t) 's' ) )
		{
			if( libfvalue_utf8_string_copy_to_integer(
			     value_string,
			     value_string_size,
			     &value_64bit,
			     64,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set number of sectors.",
				 function );

				return( -1 );
			}
			media_values->number_of_sectors = value_64bit;
		}
	}
	if( identifier != NULL )
	{
		if( libewf_value_table_set_value_by_identifier(
		     header_values,
		     identifier,
		     identifier_size,
		     value_string,
		     value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set header value: %s.",
			 function,
			 (char *) identifier );

			return( -1 );
		}
	}
	return( 1 );
}

/* Parses EWF version 2 device information
 * Returns 1 if successful or -1 on error
 */
int libewf_device_information_parse(
     const uint8_t *device_information,
     size_t device_information_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error )
{
	uint8_t *utf8_string    = NULL;
	static char *function   = "libewf_device_information_parse";
	size_t utf8_string_size = 0;

	if( device_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     device_information,
	     device_information_size,
	     0,
	     &utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 string size.",
		 function );

		goto on_error;
	}
	if( ( utf8_string_size == 0 )
	 || ( utf8_string_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-8 string size value out of bounds.",
		 function );

		goto on_error;
	}
	utf8_string = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     device_information,
	     device_information_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy device information to UTF-8 string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: device information string:\n%s",
		 function,
		 utf8_string );
	}
#endif
	if( libewf_device_information_parse_utf8_string(
	     utf8_string,
	     utf8_string_size,
	     media_values,
	     header_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse UTF-8 string.",
		 function );

		goto on_error;
	}
	memory_free(
	 utf8_string );

	return( 1 );

on_error:
	if( utf8_string != NULL )
	{
		memory_free(
		 utf8_string );
	}
	return( -1 );
}


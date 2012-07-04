/*
 * Device information functions
 *
 * Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libewf_definitions.h"
#include "libewf_device_information.h"
#include "libewf_header_values.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_media_values.h"

/* Parses an UTF-8 encoded device information string
 * Returns 1 if successful or -1 on error
 */
int libewf_device_information_parse_string(
     const uint8_t *device_information_string,
     size_t device_information_string_size,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     libcerror_error_t **error )
{
	libfvalue_split_utf8_string_t *lines  = NULL;
	libfvalue_split_utf8_string_t *types  = NULL;
	libfvalue_split_utf8_string_t *values = NULL;
	libfvalue_value_t *header_value       = NULL;
	uint8_t *identifier                   = NULL;
	uint8_t *line_string                  = NULL;
	uint8_t *type_string                  = NULL;
	uint8_t *value_string                 = NULL;
	static char *function                 = "libewf_device_information_parse_string";
	size_t identifier_size                = 0;
	size_t line_string_size               = 0;
	size_t type_string_size               = 0;
	size_t value_string_size              = 0;
	uint64_t value_64bit                  = 0;
	int number_of_lines                   = 0;
	int number_of_types                   = 0;
	int number_of_values                  = 0;
	int value_index                       = 0;

	if( device_information_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device information string.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     device_information_string,
	     device_information_string_size,
	     (uint8_t) '\n',
	     &lines,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split device information string into lines.",
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
		 "%s: unable to retrieve number of lines",
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
			 "%s: unsupported header values string.",
			 function );

			goto on_error;
		}
		if( ( line_string[ 0 ] < (uint8_t) '0' )
		 || ( line_string[ 0 ] > (uint8_t) '9' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported header values string.",
			 function );

			goto on_error;
		}
/* TODO validate line 1 => "main" */
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
			 "%s: unable to split header values string into types.",
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
			 "%s: unable to retrieve number of types",
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
			 "%s: unable to split header values string into values.",
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
			 "%s: unable to retrieve number of values",
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

				goto on_error;
			}
			/* Remove trailing carriage return
			 */
			else if( type_string[ type_string_size - 2 ] == (uint8_t) '\r' )
			{
				type_string[ type_string_size - 2 ] = 0;

				type_string_size -= 1;
			}
			if( value_index < number_of_values )
			{
				if( libfvalue_split_utf8_string_get_segment_by_index(
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
			}
			else
			{
				value_string      = NULL;
				value_string_size = 0;
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
				continue;
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
				if( ( type_string[ 0 ] == (uint8_t) 'p' )
				 && ( type_string[ 1 ] == (uint8_t) 'b' ) )
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

						goto on_error;
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

						goto on_error;
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
								 	"%s: unsupported is drive type.\n",
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

						goto on_error;
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
					else
					{
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
						 	"%s: unsupported is physical.\n",
							 function );
						}
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

						goto on_error;
					}
					media_values->number_of_sectors = value_64bit;
				}
			}
			if( identifier != NULL )
			{
				if( libfvalue_value_type_initialize(
				     &header_value,
				     LIBFVALUE_VALUE_TYPE_STRING_UTF8,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create header value.",
					 function );

					goto on_error;
				}
				if( libfvalue_value_set_identifier(
				     header_value,
				     identifier,
				     identifier_size,
				     LIBFVALUE_VALUE_IDENTIFIER_FLAG_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set header value: %s identifier.",
					 function,
					 (char *) identifier );

					goto on_error;
				}
				if( libfvalue_value_set_data(
				     header_value,
				     value_string,
				     value_string_size,
				     LIBFVALUE_CODEPAGE_UTF8,
				     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set header value: %s data.",
					 function,
					 (char *) identifier );

					goto on_error;
				}
				if( libfvalue_table_set_value(
				     header_values,
				     header_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set header value: %s in table.",
					 function,
					 (char *) identifier );

					goto on_error;
				}
				header_value = NULL;
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
	if( header_value != NULL )
	{
		libfvalue_value_free(
		 &header_value,
		 NULL );
	}
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
	uint8_t *device_information_string    = NULL;
	static char *function        = "libewf_device_information_parse";
	size_t device_information_string_size = 0;

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
	     &device_information_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine device information string size.",
		 function );

		goto on_error;
	}
	device_information_string = (uint8_t *) memory_allocate(
	                                        sizeof( uint8_t ) * device_information_string_size );

	if( device_information_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create device information string.",
		 function );

		goto on_error;
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     device_information_string,
	     device_information_string_size,
	     device_information,
	     device_information_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to set device information string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	"%s: device information string:\n%s",
		 function,
		 device_information_string );
	}
#endif
	if( libewf_device_information_parse_string(
	     device_information_string,
	     device_information_string_size,
	     media_values,
	     header_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to parse device information string.",
		 function );

		goto on_error;
	}
	memory_free(
	 device_information_string );

	return( 1 );

on_error:
	if( device_information_string != NULL )
	{
		memory_free(
		 device_information_string );
	}
	return( -1 );
}


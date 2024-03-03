/*
 * The value reader functions
 *
 * Copyright (C) 2006-2024, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#include "libewf_definitions.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_serialized_string.h"
#include "libewf_value_reader.h"

/* Creates a value reader
 * Make sure the value value_reader is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_value_reader_initialize(
     libewf_value_reader_t **value_reader,
     libcerror_error_t **error )
{
	static char *function = "libewf_value_reader_initialize";

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( *value_reader != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value reader value already set.",
		 function );

		return( -1 );
	}
	*value_reader = memory_allocate_structure(
	                 libewf_value_reader_t );

	if( *value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value reader.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *value_reader,
	     0,
	     sizeof( libewf_value_reader_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear value reader.",
		 function );

		memory_free(
		 *value_reader );

		*value_reader = NULL;

		return( -1 );
	}
	( *value_reader )->value_data_size = 1 * 1024 * 1024;

	( *value_reader )->value_data = (uint8_t *) memory_allocate(
	                                             ( *value_reader )->value_data_size );

	if( ( *value_reader )->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *value_reader != NULL )
	{
		memory_free(
		 *value_reader );

		*value_reader = NULL;
	}
	return( -1 );
}

/* Frees a value reader
 * Returns 1 if successful or -1 on error
 */
int libewf_value_reader_free(
     libewf_value_reader_t **value_reader,
     libcerror_error_t **error )
{
	static char *function = "libewf_value_reader_free";
	int result            = 1;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( *value_reader != NULL )
	{
		/* The buffer reference is freed elsewhere
		 */
		memory_free(
		 ( *value_reader )->value_data );

		memory_free(
		 *value_reader );

		*value_reader = NULL;
	}
	return( result );
}

/* Sets the buffer
 * Returns 1 if successful or -1 on error
 */
int libewf_value_reader_set_buffer(
     libewf_value_reader_t *value_reader,
     off64_t line_offset,
     const uint8_t *buffer,
     size_t buffer_size,
     int data_type,
     libcerror_error_t **error )
{
	static char *function = "libewf_value_reader_set_buffer";

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	value_reader->line_offset   = line_offset;
	value_reader->buffer        = buffer;
	value_reader->buffer_size   = buffer_size;
	value_reader->buffer_offset = 0;

/* TODO remove after refactor */
	value_reader->data_type = data_type;

	return( 1 );
}

/* Reads a value as data
 * Returns 1 if successful or -1 on error
 */
int libewf_value_reader_read_data(
     libewf_value_reader_t *value_reader,
     const uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	const uint8_t *safe_value_data = NULL;
	static char *function          = "libewf_value_reader_read_data";
	size_t end_of_value_offset     = 0;
	size_t read_size               = 0;
	size_t safe_value_data_size    = 0;
	int safe_value_index           = 0;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	if( value_reader->buffer_offset < value_reader->buffer_size )
	{
		safe_value_index = value_reader->value_index;

/* TODO remove after refactor */
		if( value_reader->data_type == LIBEWF_VALUE_DATA_TYPE_UTF8 )
		{
			for( end_of_value_offset = value_reader->buffer_offset;
			     end_of_value_offset < value_reader->buffer_size;
			     end_of_value_offset++ )
			{
				if( value_reader->buffer[ end_of_value_offset ] == (uint8_t) '\t' )
				{
					read_size = ( end_of_value_offset + 1 ) - value_reader->buffer_offset;

					safe_value_index++;

					break;
				}
			}
		}
		else
		{
			for( end_of_value_offset = value_reader->buffer_offset;
			     end_of_value_offset < value_reader->buffer_size;
			     end_of_value_offset += 2 )
			{
				if( ( value_reader->buffer[ end_of_value_offset ] == (uint8_t) '\t' )
				 && ( value_reader->buffer[ end_of_value_offset + 1 ] == 0 ) )
				{
					read_size = ( end_of_value_offset + 2 ) - value_reader->buffer_offset;

					safe_value_index++;

					break;
				}
			}
		}
		safe_value_data_size = end_of_value_offset - value_reader->buffer_offset;

		if( read_size == 0 )
		{
			read_size = safe_value_data_size;
		}
		if( safe_value_data_size > 0 )
		{
			safe_value_data = &( value_reader->buffer[ value_reader->buffer_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: value: %d data at offset: %" PRIi64 " (0x%08" PRIx64 "):\n",
				 function,
				 value_reader->value_index,
				 value_reader->line_offset,
				 value_reader->line_offset );
				libcnotify_print_data(
				 safe_value_data,
				 safe_value_data_size,
				 0 );
			}
#endif
		}
		value_reader->line_offset   += read_size;
		value_reader->buffer_offset += read_size;
		value_reader->value_index    = safe_value_index;
	}
	*value_data      = safe_value_data;
	*value_data_size = safe_value_data_size;

	return( 1 );
}

/* Reads a base-16 encoded data
 * Returns 1 if successful or -1 on error
 */
int libewf_value_reader_read_byte_stream_base16(
     libewf_value_reader_t *value_reader,
     uint8_t **byte_stream,
     size_t *byte_stream_size,
     libcerror_error_t **error )
{
	uint8_t *safe_byte_stream    = NULL;
	const uint8_t *value_data    = NULL;
	static char *function        = "libewf_value_reader_read_byte_stream_base16";
	size_t safe_byte_stream_size = 0;
	size_t value_data_size       = 0;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream size.",
		 function );

		return( -1 );
	}
	if( libewf_value_reader_read_data(
	     value_reader,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value: %d data.",
		 function,
		 value_reader->value_index );

		return( -1 );
	}
	if( value_data_size > 0 )
	{
		if( libuna_base16_stream_size_to_byte_stream(
		     value_data,
		     value_data_size,
		     &safe_byte_stream_size,
		     LIBUNA_BASE16_VARIANT_RFC4648 | LIBUNA_BASE16_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine byte stream size of base16 encoded data.",
			 function );

			return( -1 );
		}
		if( safe_byte_stream_size > value_reader->value_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid byte stream size value out of bounds.",
			 function );

			return( -1 );
		}
		safe_byte_stream = value_reader->value_data;

		if( libuna_base16_stream_copy_to_byte_stream(
		     value_data,
		     value_data_size,
		     safe_byte_stream,
		     safe_byte_stream_size,
		     LIBUNA_BASE16_VARIANT_RFC4648 | LIBUNA_BASE16_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to copy base16 encoded data to byte stream.",
			 function );

			return( -1 );
		}
	}
	*byte_stream      = safe_byte_stream;
	*byte_stream_size = safe_byte_stream_size;

	return( 1 );
}

/* Reads a signed integer value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libewf_value_reader_read_integer_signed(
     libewf_value_reader_t *value_reader,
     int64_t *integer_value,
     libcerror_error_t **error )
{
	const uint8_t *value_data = NULL;
	static char *function     = "libewf_value_reader_read_integer_signed";
	size_t value_data_offset  = 0;
	size_t value_data_size    = 0;
	uint64_t value_64bit      = 0;
	uint16_t character_value  = 0;
	int8_t sign               = 1;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( integer_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value.",
		 function );

		return( -1 );
	}
	if( libewf_value_reader_read_data(
	     value_reader,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value: %d data.",
		 function,
		 value_reader->value_index );

		return( -1 );
	}
	if( ( value_data == NULL )
	 || ( value_data_size == 0 ) )
	{
		return( 0 );
	}
	if( value_reader->data_type == LIBEWF_VALUE_DATA_TYPE_UTF8 )
	{
		if( libfvalue_utf8_string_copy_to_integer(
		     value_data,
		     value_data_size,
		     (uint64_t *) integer_value,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_SIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to integer.",
			 function );

			return( -1 );
		}
	}
	else
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( value_data[ value_data_offset ] ),
		 character_value );

		if( character_value == (uint16_t) '-' )
		{
			sign = -1;

			value_data_offset += 2;
		}
		else if( character_value == (uint16_t) '+' )
		{
			value_data_offset += 2;
		}
		if( ( value_data_size - value_data_offset ) > (size_t) ( 19 * 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data size value out of bounds.",
			 function );

			return( -1 );
		}
		while( value_data_offset < value_data_size )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( value_data[ value_data_offset ] ),
			 character_value );

			if( character_value == 0 )
			{
				break;
			}
			value_64bit *= 10;

			if( ( character_value < (uint16_t) '0' )
			 || ( character_value > (uint16_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%04" PRIx8 " at offset: %" PRIzd ".",
				 function,
				 character_value,
				 value_data_offset );

				return( -1 );
			}
			character_value -= (uint16_t) '0';

			value_64bit += character_value;

			value_data_offset += 2;
		}
		if( ( sign == -1 )
		 && ( value_64bit != 0 ) )
		{
			value_64bit = ~( value_64bit - 1 );
		}
		*integer_value = (int64_t) value_64bit;
	}
	return( 1 );
}

/* Reads an unsigned integer value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libewf_value_reader_read_integer_unsigned(
     libewf_value_reader_t *value_reader,
     uint64_t *integer_value,
     libcerror_error_t **error )
{
	const uint8_t *value_data = NULL;
	static char *function     = "libewf_value_reader_read_integer_unsigned";
	size_t value_data_offset  = 0;
	size_t value_data_size    = 0;
	uint64_t value_64bit      = 0;
	uint16_t character_value  = 0;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( integer_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer value.",
		 function );

		return( -1 );
	}
	if( libewf_value_reader_read_data(
	     value_reader,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value: %d data.",
		 function,
		 value_reader->value_index );

		return( -1 );
	}
	if( ( value_data == NULL )
	 || ( value_data_size == 0 ) )
	{
		return( 0 );
	}
	if( value_reader->data_type == LIBEWF_VALUE_DATA_TYPE_UTF8 )
	{
		if( libfvalue_utf8_string_copy_to_integer(
		     value_data,
		     value_data_size,
		     (uint64_t *) integer_value,
		     64,
		     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to integer.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( ( value_data_size - value_data_offset ) > (size_t) ( 20 * 2 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data size value out of bounds.",
			 function );

			return( -1 );
		}
		while( value_data_offset < value_data_size )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( value_data[ value_data_offset ] ),
			 character_value );

			if( character_value == 0 )
			{
				break;
			}
			value_64bit *= 10;

			if( ( character_value < (uint16_t) '0' )
			 || ( character_value > (uint16_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported character value: 0x%04" PRIx8 " at offset: %" PRIzd ".",
				 function,
				 character_value,
				 value_data_offset );

				return( -1 );
			}
			character_value -= (uint16_t) '0';

			value_64bit += character_value;

			value_data_offset += 2;
		}
		*integer_value = value_64bit;
	}
	return( 1 );
}

/* Reads a serialized string value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libewf_value_reader_read_serialized_string(
     libewf_value_reader_t *value_reader,
     libewf_serialized_string_t *serialized_string,
     libcerror_error_t **error )
{
	const uint8_t *value_data = NULL;
	static char *function     = "libewf_value_reader_read_serialized_string";
	size_t value_data_size    = 0;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( libewf_value_reader_read_data(
	     value_reader,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value: %d data.",
		 function,
		 value_reader->value_index );

		return( -1 );
	}
	if( ( value_data == NULL )
	 || ( value_data_size == 0 ) )
	{
		return( 0 );
	}
	if( libewf_serialized_string_read_data(
	     serialized_string,
	     value_data,
	     value_data_size,
	     value_reader->data_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read serialized string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a base-16 encoded serialized string value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libewf_value_reader_read_serialized_string_base16(
     libewf_value_reader_t *value_reader,
     libewf_serialized_string_t *serialized_string,
     libcerror_error_t **error )
{
	const uint8_t *value_data = NULL;
	static char *function     = "libewf_value_reader_read_serialized_string_base16";
	size_t value_data_size    = 0;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
	if( libewf_value_reader_read_data(
	     value_reader,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value: %d data.",
		 function,
		 value_reader->value_index );

		return( -1 );
	}
	if( ( value_data == NULL )
	 || ( value_data_size == 0 ) )
	{
		return( 0 );
	}
	if( libewf_serialized_string_read_hexadecimal_data(
	     serialized_string,
	     value_data,
	     value_data_size,
	     value_reader->data_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read hexadecimal data serialized string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a value as UTF-8 string
 * Returns 1 if successful or -1 on error
 */
int libewf_value_reader_read_utf8_string(
     libewf_value_reader_t *value_reader,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *safe_utf8_string    = NULL;
	const uint8_t *value_data    = NULL;
	static char *function        = "libewf_value_reader_read_utf8_string";
	size_t safe_utf8_string_size = 0;
	size_t value_data_size       = 0;

	if( value_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value reader.",
		 function );

		return( -1 );
	}
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
	if( libewf_value_reader_read_data(
	     value_reader,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value: %d data.",
		 function,
		 value_reader->value_index );

		return( -1 );
	}
	if( value_data_size > 0 )
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     value_data,
		     value_data_size,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     &safe_utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		if( safe_utf8_string_size > value_reader->value_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-8 string size value out of bounds.",
			 function );

			return( -1 );
		}
		safe_utf8_string = value_reader->value_data;

		if( libuna_utf8_string_copy_from_utf16_stream(
		     safe_utf8_string,
		     safe_utf8_string_size,
		     value_data,
		     value_data_size,
		     LIBUNA_ENDIAN_LITTLE | LIBUNA_UTF16_STREAM_ALLOW_UNPAIRED_SURROGATE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	*utf8_string      = safe_utf8_string;
	*utf8_string_size = safe_utf8_string_size;

	return( 1 );
}


/*
 * The line reader functions
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libfdata.h"
#include "libewf_libhmac.h"
#include "libewf_libuna.h"
#include "libewf_line_reader.h"

/* Creates a line reader
 * Make sure the value line_reader is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_line_reader_initialize(
     libewf_line_reader_t **line_reader,
     libfdata_stream_t *data_stream,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	static char *function = "libewf_line_reader_initialize";

	if( line_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line reader.",
		 function );

		return( -1 );
	}
	if( *line_reader != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid line reader value already set.",
		 function );

		return( -1 );
	}
	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	*line_reader = memory_allocate_structure(
	                libewf_line_reader_t );

	if( *line_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create line reader.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *line_reader,
	     0,
	     sizeof( libewf_line_reader_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear line reader.",
		 function );

		memory_free(
		 *line_reader );

		*line_reader = NULL;

		return( -1 );
	}
	if( libfdata_stream_get_size(
	     data_stream,
	     &( ( *line_reader )->stream_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data stream size.",
		 function );

		goto on_error;
	}
	( *line_reader )->buffer_size = 16 * 1024 * 1024;

	( *line_reader )->buffer = (uint8_t *) memory_allocate(
	                                        ( *line_reader )->buffer_size );

	if( ( *line_reader )->buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create buffer.",
		 function );

		goto on_error;
	}
	( *line_reader )->utf8_string_size = 1 * 1024 * 1024;

	( *line_reader )->utf8_string = (uint8_t *) memory_allocate(
	                                             ( *line_reader )->utf8_string_size );

	if( ( *line_reader )->utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	if( libhmac_md5_initialize(
	     &( ( *line_reader )->md5_context ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize MD5 context.",
		 function );

		goto on_error;
	}
	( *line_reader )->data_stream  = data_stream;
	( *line_reader )->file_io_pool = file_io_pool;

	return( 1 );

on_error:
	if( *line_reader != NULL )
	{
		if( ( *line_reader )->utf8_string != NULL )
		{
			memory_free(
			 ( *line_reader )->utf8_string );
		}
		if( ( *line_reader )->buffer != NULL )
		{
			memory_free(
			 ( *line_reader )->buffer );
		}
		memory_free(
		 *line_reader );

		*line_reader = NULL;
	}
	return( -1 );
}

/* Frees a line reader
 * Returns 1 if successful or -1 on error
 */
int libewf_line_reader_free(
     libewf_line_reader_t **line_reader,
     libcerror_error_t **error )
{
	static char *function = "libewf_line_reader_free";
	int result            = 1;

	if( line_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line reader.",
		 function );

		return( -1 );
	}
	if( *line_reader != NULL )
	{
		/* The data_stream reference is freed elsewhere
		 */
		if( libhmac_md5_free(
		     &( ( *line_reader )->md5_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MD5 context.",
			 function );

			result = -1;
		}
		memory_free(
		 ( *line_reader )->utf8_string );

		memory_free(
		 ( *line_reader )->buffer );

		memory_free(
		 *line_reader );

		*line_reader = NULL;
	}
	return( result );
}

/* Reads a line as data
 * Returns 1 if successful or -1 on error
 */
int libewf_line_reader_read_data(
     libewf_line_reader_t *line_reader,
     const uint8_t **line_data,
     size_t *line_data_size,
     libcerror_error_t **error )
{
	const uint8_t *safe_line_data = NULL;
	static char *function         = "libewf_line_reader_read_data";
	size_t end_of_line_offset     = 0;
	size_t read_size              = 0;
	size_t safe_line_data_size    = 0;
	ssize_t read_count            = 0;
	int safe_line_index           = 0;

	if( line_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line reader.",
		 function );

		return( -1 );
	}
	if( line_reader->buffer_offset >= line_reader->buffer_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid line reader - buffer offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( line_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line data.",
		 function );

		return( -1 );
	}
	if( line_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line data size.",
		 function );

		return( -1 );
	}
	read_size = line_reader->buffer_size - line_reader->buffer_offset;

	if( (size64_t) line_reader->stream_offset < line_reader->stream_size )
	{
		if( ( line_reader->stream_offset == 0 )
		 || ( read_size < line_reader->utf8_string_size ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: additional line data needed at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
				 function,
				 line_reader->line_offset,
				 line_reader->line_offset );
			}
#endif
			if( line_reader->buffer_offset > 0 )
			{
				if( memmove(
				     line_reader->buffer,
				     &( line_reader->buffer[ line_reader->buffer_offset ] ),
				     read_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to move buffer content to start.",
					 function );

					return( -1 );
				}
				line_reader->buffer_offset = read_size;

				read_size = line_reader->buffer_size - read_size;
			}
			if( read_size > ( line_reader->stream_size - line_reader->stream_offset ) )
			{
				read_size = (size_t) ( line_reader->stream_size - line_reader->stream_offset );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading %" PRIzd " bytes of section data at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
				 function,
				 read_size,
				 line_reader->stream_offset,
				 line_reader->stream_offset );
			}
#endif
			read_count = libfdata_stream_read_buffer_at_offset(
				      line_reader->data_stream,
				      (intptr_t *) line_reader->file_io_pool,
				      &( line_reader->buffer[ line_reader->buffer_offset ] ),
				      read_size,
				      line_reader->stream_offset,
				      0,
				      error );

			if( read_count != (ssize_t) read_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read section data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 line_reader->stream_offset,
				 line_reader->stream_offset );

				return( -1 );
			}
			line_reader->stream_offset += read_count;

			if( libhmac_md5_update(
			     line_reader->md5_context,
			     &( line_reader->buffer[ line_reader->buffer_offset ] ),
			     read_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to update MD5 digest hash.",
				 function );

				return( -1 );
			}
			line_reader->buffer_offset = 0;
		}
	}
	read_size       = 0;
	safe_line_index = line_reader->line_index;

	for( end_of_line_offset = line_reader->buffer_offset;
	     end_of_line_offset < line_reader->buffer_size;
	     end_of_line_offset += 2 )
	{
		if( ( line_reader->buffer[ end_of_line_offset ] == (uint8_t) '\n' )
		 && ( line_reader->buffer[ end_of_line_offset + 1 ] == 0 ) )
		{
			read_size = ( end_of_line_offset + 2 ) - line_reader->buffer_offset;

			safe_line_index++;

			break;
		}
	}
	/* Remove trailing carriage return
	 */
	if( end_of_line_offset >= 2 )
	{
		if( ( line_reader->buffer[ end_of_line_offset - 2 ] == (uint8_t) '\r' )
		 && ( line_reader->buffer[ end_of_line_offset - 1 ] == 0 ) )
		{
			end_of_line_offset -= 2;
		}
	}
	safe_line_data_size = end_of_line_offset - line_reader->buffer_offset;

	if( read_size == 0 )
	{
		read_size = safe_line_data_size;
	}
	if( safe_line_data_size > 0 )
	{
		safe_line_data = &( line_reader->buffer[ line_reader->buffer_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: line: %d data at offset: %" PRIi64 " (0x%08" PRIx64 "):\n",
			 function,
			 line_reader->line_index,
			 line_reader->line_offset,
			 line_reader->line_offset );
			libcnotify_print_data(
			 safe_line_data,
			 safe_line_data_size,
			 0 );
		}
#endif
	}
	line_reader->buffer_offset += read_size;
	line_reader->line_offset   += read_size;
	line_reader->line_index     = safe_line_index;

	*line_data      = safe_line_data;
	*line_data_size = safe_line_data_size;

	return( 1 );
}

/* Reads a line as UTF-8 string
 * Returns 1 if successful or -1 on error
 */
int libewf_line_reader_read_utf8_string(
     libewf_line_reader_t *line_reader,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	const uint8_t *utf16_stream  = NULL;
	static char *function        = "libewf_line_reader_read_utf8_string";
	size_t safe_utf8_string_size = 0;
	size_t utf16_stream_size     = 0;

	if( line_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line reader.",
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
	if( libewf_line_reader_read_data(
	     line_reader,
	     &utf16_stream,
	     &utf16_stream_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read line data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 line_reader->stream_offset,
		 line_reader->stream_offset );

		return( -1 );
	}
	if( utf16_stream_size == 0 )
	{
		line_reader->utf8_string[ 0 ] = 0;
		safe_utf8_string_size         = 1;
	}
	else
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     utf16_stream,
		     utf16_stream_size,
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
		if( safe_utf8_string_size > line_reader->utf8_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-8 string size value out of bounds.",
			 function );

			return( -1 );
		}
		if( libuna_utf8_string_copy_from_utf16_stream(
		     line_reader->utf8_string,
		     safe_utf8_string_size,
		     utf16_stream,
		     utf16_stream_size,
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
	*utf8_string      = line_reader->utf8_string;
	*utf8_string_size = safe_utf8_string_size;

	return( 1 );
}

/* Finalizes the line reader
 * Returns 1 if successful or -1 on error
 */
int libewf_line_reader_finalize(
     libewf_line_reader_t *line_reader,
     libcerror_error_t **error )
{
	uint8_t calculated_md5_hash[ LIBHMAC_MD5_HASH_SIZE ];

	static char *function = "libewf_line_reader_finalize";
	size_t read_size      = 0;
	ssize_t read_count    = 0;

	if( line_reader == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid line reader.",
		 function );

		return( -1 );
	}
	while( (size64_t) line_reader->stream_offset < line_reader->stream_size )
	{
		read_size = line_reader->buffer_size;

		if( read_size > ( line_reader->stream_size - line_reader->stream_offset ) )
		{
			read_size = (size_t) ( line_reader->stream_size - line_reader->stream_offset );
		}
		read_count = libfdata_stream_read_buffer_at_offset(
		              line_reader->data_stream,
		              (intptr_t *) line_reader->file_io_pool,
		              line_reader->buffer,
		              read_size,
		              line_reader->stream_offset,
		              0,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read section data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 line_reader->stream_offset,
			 line_reader->stream_offset );

			return( -1 );
		}
		line_reader->stream_offset += read_count;

		if( libhmac_md5_update(
		     line_reader->md5_context,
		     line_reader->buffer,
		     read_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update MD5 digest hash.",
			 function );

			return( -1 );
		}
	}
	if( libhmac_md5_finalize(
	     line_reader->md5_context,
	     calculated_md5_hash,
	     LIBHMAC_MD5_HASH_SIZE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to finalize MD5 hash.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: calculated MD5 hash:\n",
		 function );
		libcnotify_print_data(
		 calculated_md5_hash,
		 16,
		 0 );
	}
#endif
/* TODO
	if( memory_compare(
	     stored_md5_hash,
	     calculated_md5_hash,
	     16 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: mismatch in integrity hash.",
		 function );

		return( -1 );
	}
*/
	return( 1 );
}


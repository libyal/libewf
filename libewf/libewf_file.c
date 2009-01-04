/*
 * File handling functions
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
#include <narrow_string.h>
#include <memory.h>
#include <notify.h>
#include <types.h>
#include <wide_string.h>

#include "libewf_definitions.h"
#include "libewf_error.h"
#include "libewf_file.h"
#include "libewf_filename.h"
#include "libewf_file_io.h"
#include "libewf_offset_table.h"
#include "libewf_read.h"
#include "libewf_section_list.h"
#include "libewf_segment_file.h"
#include "libewf_segment_file_handle.h"
#include "libewf_segment_table.h"
#include "libewf_write.h"

#include "ewf_definitions.h"

/* Detects if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
int libewf_check_file_signature(
     const char *filename )
{
	uint8_t signature[ 8 ];

	libewf_error_t *error = NULL;
	static char *function = "libewf_check_file_signature";
	ssize_t read_count    = 0;
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	file_descriptor = libewf_file_io_open(
	                   filename,
	                   LIBEWF_FILE_IO_O_RDONLY,
	                   &error );

	if( file_descriptor == -1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	read_count = libewf_file_io_read(
	              file_descriptor,
	              signature,
	              8 );

	if( read_count != 8 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature.\n",
		 function );

		libewf_file_io_close(
		 file_descriptor );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( libewf_file_io_close(
	     file_descriptor ) != 0 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	/* The amount of EWF segment files will be the largest
	 */
	if( memory_compare(
	     evf_file_signature,
	     signature,
	     8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          lvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          dvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Detects if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_check_file_signature_wide(
     const wchar_t *filename )
{
	uint8_t signature[ 8 ];

	libewf_error_t *error = NULL;
	static char *function = "libewf_check_file_signature";
	ssize_t read_count    = 0;
	int file_descriptor   = 0;

	if( filename == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	file_descriptor = libewf_file_io_open_wide(
	                   filename,
	                   LIBEWF_FILE_IO_O_RDONLY,
	                   &error );

	if( file_descriptor == -1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	read_count = libewf_file_io_read(
	              file_descriptor,
	              signature,
	              8 );

	if( read_count != 8 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature.\n",
		 function );

		libewf_file_io_close(
		 file_descriptor );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( libewf_file_io_close(
	     file_descriptor ) != 0 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	/* The amount of EWF segment files will be the largest
	 */
	if( memory_compare(
	     evf_file_signature,
	     signature,
	     8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          lvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          dvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}
#endif

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns the amount of filenames if successful or -1 on error
 */
int libewf_glob(
     const char *filename,
     size_t length,
     uint8_t format,
     char **filenames[] )
{
	libewf_error_t *error     = NULL;
	char *segment_filename    = NULL;
	void *reallocation        = NULL;
	static char *function     = "libewf_glob";
	size_t additional_length  = 4;
	int amount_of_files       = 0;
	int result                = 0;
	uint8_t segment_file_type = 0;
	uint8_t ewf_format        = 0;

	if( filename == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( length == 0 )
	 || ( length > (size_t) SSIZE_MAX ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid filename length.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_UNKNOWN )
	 && ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK )
	 && ( format != LIBEWF_FORMAT_LVF )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( filename[ length - 4 ] != (char) '.' )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - missing extension.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		additional_length = 0;

		if( filename[ length - 3 ] == (char) 'E' )
		{
			format = LIBEWF_FORMAT_ENCASE5;
		}
		else if( filename[ length - 3 ] == (char) 'e' )
		{
			format = LIBEWF_FORMAT_EWF;
		}
		else if( filename[ length - 3 ] == (char) 'L' )
		{
			format = LIBEWF_FORMAT_LVF;
		}
		else if( filename[ length - 3 ] == (char) 's' )
		{
			format = LIBEWF_FORMAT_SMART;
		}
		else
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - unsupported extension: %s.\n",
			 function,
			 &( filename[ length - 4 ] ) );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
	}
	if( format == LIBEWF_FORMAT_LVF )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
		ewf_format        = EWF_FORMAT_L01;
	}
	else if( format == LIBEWF_FORMAT_SMART )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_S01;
	}
	else
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_E01;
	}
	while( amount_of_files < (int) UINT16_MAX )
	{
		segment_filename = (char * ) memory_allocate(
			                      sizeof( char ) * ( length + additional_length + 1 ) );

		if( segment_filename == NULL )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		if( narrow_string_copy(
		     segment_filename,
		     filename,
		     length ) == NULL )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy filename.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		if( additional_length > 0 )
		{
			segment_filename[ length ] = (char) '.';
		}
		if( libewf_filename_set_extension(
		     &( segment_filename[ length + additional_length - 3 ] ),
		     (uint16_t) ( amount_of_files + 1 ),
		     UINT16_MAX,
		     segment_file_type,
		     format,
		     ewf_format,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set extension.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		segment_filename[ length + additional_length ] = 0;

		result = libewf_file_io_exists(
		          segment_filename,
		          &error );

		if( result == -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_GENERIC,
			 "%s: unable to test if file exists.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		amount_of_files++;

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( char * ) * amount_of_files );

		if( reallocation == NULL )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize filenames.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		*filenames = (char **) reallocation;

		( *filenames )[ amount_of_files - 1 ] = segment_filename;
	}
	return( amount_of_files );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns the amount of filenames if successful or -1 on error
 */
int libewf_glob_wide(
     const wchar_t *filename,
     size_t length,
     uint8_t format,
     wchar_t **filenames[] )
{
	libewf_error_t *error     = NULL;
	wchar_t *segment_filename = NULL;
	void *reallocation        = NULL;
	static char *function     = "libewf_glob_wide";
	size_t additional_length  = 4;
	int amount_of_files       = 0;
	int result                = 0;
	uint8_t segment_file_type = 0;
	uint8_t ewf_format        = 0;

	if( filename == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( length == 0 )
	 || ( length > (size_t) SSIZE_MAX ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid filename length.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_UNKNOWN )
	 && ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK )
	 && ( format != LIBEWF_FORMAT_LVF )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( filename[ length - 4 ] != (wchar_t) '.' )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - missing extension.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		additional_length = 0;

		if( filename[ length - 3 ] == (wchar_t) 'E' )
		{
			format = LIBEWF_FORMAT_ENCASE5;
		}
		else if( filename[ length - 3 ] == (wchar_t) 'e' )
		{
			format = LIBEWF_FORMAT_EWF;
		}
		else if( filename[ length - 3 ] == (wchar_t) 'L' )
		{
			format = LIBEWF_FORMAT_LVF;
		}
		else if( filename[ length - 3 ] == (wchar_t) 's' )
		{
			format = LIBEWF_FORMAT_SMART;
		}
		else
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
			 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - unsupported extension: %s.\n",
			 function,
			 &( filename[ length - 4 ] ) );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
	}
	if( format == LIBEWF_FORMAT_LVF )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_LWF;
		ewf_format        = EWF_FORMAT_L01;
	}
	else if( format == LIBEWF_FORMAT_SMART )
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_S01;
	}
	else
	{
		segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF;
		ewf_format        = EWF_FORMAT_E01;
	}
	while( amount_of_files < (int) UINT16_MAX )
	{
		segment_filename = memory_allocate(
			            sizeof( wchar_t ) * ( length + additional_length + 1 ) );

		if( segment_filename == NULL )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		if( wide_string_copy(
		     segment_filename,
		     filename,
		     length ) == NULL )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy filename.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		if( additional_length > 0 )
		{
			segment_filename[ length ] = (wchar_t) '.';
		}
		if( libewf_filename_set_extension_wide(
		     &( segment_filename[ length + additional_length - 3 ] ),
		     (uint16_t) ( amount_of_files + 1 ),
		     UINT16_MAX,
		     segment_file_type,
		     format,
		     ewf_format,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set extension.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		segment_filename[ length + additional_length ] = 0;

		result = libewf_file_io_exists_wide(
		          segment_filename,
		          &error );

		if( result == -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_GENERIC,
			 "%s: unable to test if file exists.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		amount_of_files++;

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( wchar_t * ) * amount_of_files );

		if( reallocation == NULL )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_MEMORY,
			 LIBEWF_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize filenames.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			memory_free(
			 segment_filename );

			return( -1 );
		}
		*filenames = (wchar_t **) reallocation;

		( *filenames )[ amount_of_files - 1 ] = segment_filename;
	}
	return( amount_of_files );
}
#endif

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 */
libewf_handle_t *libewf_open(
                  char * const filenames[],
                  int amount_of_files,
                  uint8_t flags )
{
	libewf_error_t *error                     = NULL;
	libewf_handle_t *handle                   = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	char *first_delta_segment_filename        = NULL;
	char *first_segment_filename              = NULL;
	static char *function                     = "libewf_open";
	size64_t *segment_file_size               = NULL;
	size_t filename_length                    = 0;
	uint16_t segment_number                   = 0;
	int filename_iterator                     = 0;
	int file_io_pool_entry                    = 0;
	int result                                = 0;
	uint8_t segment_file_type                 = 0;

	if( filenames == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	if( amount_of_files <= 0 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of files zero or less.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) != LIBEWF_FLAG_READ )
	 && ( ( flags & LIBEWF_FLAG_WRITE ) != LIBEWF_FLAG_WRITE ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	if( libewf_handle_initialize(
	     &handle,
	     flags,
	     &error ) != 1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		for( filename_iterator = 0; filename_iterator < amount_of_files; filename_iterator++ )
		{
			filename_length = narrow_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
				 LIBEWF_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %s is too small.\n",
				 function,
				 filenames[ filename_iterator ] );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			if( libewf_file_io_pool_add_file(
			     internal_handle->file_io_pool,
			     filenames[ filename_iterator ],
			     LIBEWF_FILE_IO_O_RDONLY,
			     &file_io_pool_entry,
			     &error ) != 1 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to add file to file io pool.\n",
				 function );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			result = libewf_internal_handle_add_segment_file(
			          internal_handle,
			          file_io_pool_entry,
			          flags,
			          &segment_number,
			          &segment_file_type,
			          &error );

			if( result == -1 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add segment file.\n",
				 function );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			else if( result == 0 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_SET_FAILED,
				 "%s: segment file: %" PRIu16 " already exists.\n",
				 function,
				 segment_number );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			if( (int) segment_number > amount_of_files )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_INPUT,
				 LIBEWF_INPUT_ERROR_INVALID_DATA,
				 "%s: invalid segment number: %" PRIu16 " - value out of range or missing segment files.\n",
				 function,
				 segment_number );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
			 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF ) )
			{
				if( segment_number == 1 )
				{
					first_segment_filename = filenames[ filename_iterator ];

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_iterator ];
					}
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				notify_verbose_printf(
				 "%s: added segment file: %" PRIu16 " (%s).\n",
				 function,
				 segment_number,
				 filenames[ filename_iterator ] );
#endif
			}
			else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
			{
				if( segment_number == 1 )
				{
					first_delta_segment_filename = filenames[ filename_iterator ];
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				notify_verbose_printf(
				 "%s: added delta segment file: %" PRIu16 " (%s).\n",
				 function,
				 segment_number,
				 filenames[ filename_iterator ] );
#endif
			}
		}
		/* Get the basename of the first segment file
		 */
		filename_length = narrow_string_length(
				   first_segment_filename );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename(
		     internal_handle->segment_table,
		     first_segment_filename,
		     filename_length - 4 + 1,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		/* Get the basename of the first segment file
		 */
		filename_length = narrow_string_length(
				   first_delta_segment_filename );

		/* Set delta segment table basename
		 */
		if( libewf_segment_table_set_basename(
		     internal_handle->delta_segment_table,
		     first_delta_segment_filename,
		     filename_length - 4 + 1,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in delta segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		if( libewf_segment_table_build(
		     internal_handle->segment_table,
		     internal_handle->file_io_pool,
		     internal_handle->header_sections,
		     internal_handle->hash_sections,
		     internal_handle->media_values,
		     internal_handle->offset_table,
		     internal_handle->sessions,
		     internal_handle->acquiry_errors,
		     &( internal_handle->compression_level ),
		     &( internal_handle->format ),
		     &( internal_handle->ewf_format ),
		     segment_file_size,
		     &( internal_handle->abort ),
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to build segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		if( ( internal_handle->delta_segment_table->amount > 1 )
		 && ( libewf_segment_table_build(
		       internal_handle->delta_segment_table,
		       internal_handle->file_io_pool,
		       internal_handle->header_sections,
		       internal_handle->hash_sections,
		       internal_handle->media_values,
		       internal_handle->offset_table,
		       internal_handle->sessions,
		       internal_handle->acquiry_errors,
		       &( internal_handle->compression_level ),
		       &( internal_handle->format ),
		       &( internal_handle->ewf_format ),
		       segment_file_size,
		       &( internal_handle->abort ),
		       &error ) != 1 ) )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to build delta segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL);
		}
		/* Determine the EWF format
		 */
		if( libewf_header_sections_determine_format(
		     internal_handle->header_sections,
		     internal_handle->ewf_format,
		     &( internal_handle->format ),
		     &error ) == -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine format.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		/* Calculate the media size
		 */
		internal_handle->media_values->media_size = (size64_t) internal_handle->media_values->amount_of_sectors
		                                          * (size64_t) internal_handle->media_values->bytes_per_sector;
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = narrow_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length + 1,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
	}
	/* Make sure format specific values are set
	 */
	if( ( internal_handle->format != 0 )
	 && ( libewf_internal_handle_set_format(
	       internal_handle,
	       internal_handle->format,
	       &error ) != 1 ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		libewf_handle_free(
		 &handle,
		 NULL );

		return( NULL );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: open successful.\n",
	 function );
#endif

	return( handle );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of EWF file(s)
 * For reading files should contain all filenames that make up an EWF image
 * For writing files should contain the base of the filename, extentions like .e01 will be automatically added
 * Returns a pointer to the new instance of handle, NULL on error
 */
libewf_handle_t *libewf_open_wide(
                  wchar_t * const filenames[],
                  int amount_of_files,
                  uint8_t flags )
{
	libewf_error_t *error                     = NULL;
	libewf_handle_t *handle                   = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	wchar_t *first_delta_segment_filename     = NULL;
	wchar_t *first_segment_filename           = NULL;
	static char *function                     = "libewf_open_wide";
	size64_t *segment_file_size               = NULL;
	size_t filename_length                    = 0;
	uint16_t segment_number                   = 0;
	int filename_iterator                     = 0;
	int file_io_pool_entry                    = 0;
	int result                                = 0;
	uint8_t segment_file_type                 = 0;

	if( filenames == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	if( amount_of_files <= 0 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid amount of files zero or less.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	if( ( ( flags & LIBEWF_FLAG_READ ) != LIBEWF_FLAG_READ )
	 && ( ( flags & LIBEWF_FLAG_WRITE ) != LIBEWF_FLAG_WRITE ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	if( libewf_handle_initialize(
	     &handle,
	     flags,
	     &error ) != 1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( NULL );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( flags & LIBEWF_FLAG_READ ) == LIBEWF_FLAG_READ )
	{
		for( filename_iterator = 0; filename_iterator < amount_of_files; filename_iterator++ )
		{
			filename_length = wide_string_length(
					   filenames[ filename_iterator ] );

			/* Make sure there is more to the filename than the extension
			 */
			if( filename_length <= 4 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
				 LIBEWF_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: filename: %ls is too small.\n",
				 function,
				 filenames[ filename_iterator ] );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			if( libewf_file_io_pool_add_file_wide(
			     internal_handle->file_io_pool,
			     filenames[ filename_iterator ],
			     LIBEWF_FILE_IO_O_RDONLY,
			     &file_io_pool_entry,
			     &error ) != 1 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to add file to file io pool.\n",
				 function );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			result = libewf_internal_handle_add_segment_file(
			          internal_handle,
			          file_io_pool_entry,
			          flags,
			          &segment_number,
			          &segment_file_type,
			          &error );

			if( result == -1 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to add segment file.\n",
				 function );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			else if( result == 0 )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_RUNTIME,
				 LIBEWF_RUNTIME_ERROR_SET_FAILED,
				 "%s: segment file: %" PRIu16 " already exists.\n",
				 function,
				 segment_number );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			if( (int) segment_number > amount_of_files )
			{
				libewf_error_set(
				 &error,
				 LIBEWF_ERROR_DOMAIN_INPUT,
				 LIBEWF_INPUT_ERROR_INVALID_DATA,
				 "%s: invalid segment number: %" PRIu16 " - value out of range or missing segment files.\n",
				 function,
				 segment_number );

				libewf_error_backtrace_notify(
				 error );
				libewf_error_free(
				 &error );

				libewf_handle_free(
				 &handle,
				 NULL );

				return( NULL );
			}
			if( ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
			 || ( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF ) )
			{
				if( segment_number == 1 )
				{
					first_segment_filename = filenames[ filename_iterator ];

					if( first_delta_segment_filename == NULL )
					{
						first_delta_segment_filename = filenames[ filename_iterator ];
					}
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				notify_verbose_printf(
				 "%s: added segment file: %" PRIu16 " (%ls).\n",
				 function,
				 segment_number,
				 filenames[ filename_iterator ] );
#endif
			}
			else if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
			{
				if( segment_number == 1 )
				{
					first_delta_segment_filename = filenames[ filename_iterator ];
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				notify_verbose_printf(
				 "%s: added delta segment file: %" PRIu16 " (%ls).\n",
				 function,
				 segment_number,
				 filenames[ filename_iterator ] );
#endif
			}
		}
		/* Get the basename of the first segment file
		 */
		filename_length = wide_string_length(
				   first_segment_filename );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename_wide(
		     internal_handle->segment_table,
		     first_segment_filename,
		     filename_length - 4 + 1,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		/* Get the basename of the first segment file
		 */
		filename_length = wide_string_length(
				   first_delta_segment_filename );

		/* Set delta segment table basename
		 */
		if( libewf_segment_table_set_basename_wide(
		     internal_handle->delta_segment_table,
		     first_delta_segment_filename,
		     filename_length - 4 + 1,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in delta segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		if( libewf_segment_table_build(
		     internal_handle->segment_table,
		     internal_handle->file_io_pool,
		     internal_handle->header_sections,
		     internal_handle->hash_sections,
		     internal_handle->media_values,
		     internal_handle->offset_table,
		     internal_handle->sessions,
		     internal_handle->acquiry_errors,
		     &( internal_handle->compression_level ),
		     &( internal_handle->format ),
		     &( internal_handle->ewf_format ),
		     segment_file_size,
		     &( internal_handle->abort ),
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to build segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		if( ( internal_handle->delta_segment_table->amount > 1 )
		 && ( libewf_segment_table_build(
		       internal_handle->delta_segment_table,
		       internal_handle->file_io_pool,
		       internal_handle->header_sections,
		       internal_handle->hash_sections,
		       internal_handle->media_values,
		       internal_handle->offset_table,
		       internal_handle->sessions,
		       internal_handle->acquiry_errors,
		       &( internal_handle->compression_level ),
		       &( internal_handle->format ),
		       &( internal_handle->ewf_format ),
		       segment_file_size,
		       &( internal_handle->abort ),
		       &error ) != 1 ) )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to build delta segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL);
		}
		/* Determine the EWF format
		 */
		if( libewf_header_sections_determine_format(
		     internal_handle->header_sections,
		     internal_handle->ewf_format,
		     &( internal_handle->format ),
		     &error ) == -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine format.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
		/* Calculate the media size
		 */
		internal_handle->media_values->media_size = (size64_t) internal_handle->media_values->amount_of_sectors
		                                          * (size64_t) internal_handle->media_values->bytes_per_sector;
	}
	else if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
	{
		/* Get the basename and store it in the segment tables
		 */
		filename_length = wide_string_length(
				   filenames[ 0 ] );

		/* Set segment table basename
		 */
		if( libewf_segment_table_set_basename_wide(
		     internal_handle->segment_table,
		     filenames[ 0 ],
		     filename_length + 1,
		     &error ) != 1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename in segment table.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			libewf_handle_free(
			 &handle,
			 NULL );

			return( NULL );
		}
	}
	/* Make sure format specific values are set
	 */
	if( ( internal_handle->format != 0 )
	 && ( libewf_internal_handle_set_format(
	       internal_handle,
	       internal_handle->format,
	       &error ) != 1 ) )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set format.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		libewf_handle_free(
		 &handle,
		 NULL );

		return( NULL );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf(
	 "%s: open successful.\n",
	 function );
#endif
	return( handle );
}

#endif

/* Closes the EWF handle and frees memory used within the handle
 * Returns 0 if successful or -1 on error
 */
int libewf_close(
     libewf_handle_t *handle )
{
	libewf_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_close";

	if( handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( ( internal_handle->write != NULL )
	 && ( internal_handle->write->write_finalized == 0 ) )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf(
		 "%s: finalizing write.\n",
		 function );
#endif

		libewf_write_finalize(
		 handle );
	}
	if( libewf_file_io_pool_close_all(
	     internal_handle->file_io_pool,
	     &error ) != 0 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_IO,
		 LIBEWF_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close all segment files.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		libewf_handle_free(
		 &handle,
		 NULL );

		return( -1 );
	}
	if( libewf_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_FREE_FAILED,
		 "%s: unable to free handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Seeks a certain offset of the media data within the EWF file(s)
 * It will set the related file offset to the specific chunk offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libewf_seek_offset(
         libewf_handle_t *handle,
         off64_t offset )
{
	libewf_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_seek_offset";
	uint64_t chunk                            = 0;
	uint64_t chunk_offset                     = 0;

	if( handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( offset <= -1 )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid offset value cannot be less than zero.\n",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) internal_handle->media_values->media_size )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_VALUE_TOO_LARGE,
		 "%s: attempting to read past the end of the file.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	if( offset < (off64_t) internal_handle->media_values->media_size )
	{
		/* Determine the chunk that is requested
		 */
		chunk = offset / internal_handle->media_values->chunk_size;

		if( chunk >= (uint64_t) INT32_MAX )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk value exceeds maximum.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		if( libewf_offset_table_seek_chunk_offset(
		     internal_handle->offset_table,
		     (uint32_t) chunk,
		     internal_handle->file_io_pool,
		     &error ) == -1 )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_IO,
			 LIBEWF_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek chunk offset.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		internal_handle->current_chunk = (uint32_t) chunk;

		/* Determine the offset within the decompressed chunk that is requested
		 */
		chunk_offset = offset % internal_handle->media_values->chunk_size;

		if( chunk_offset >= (uint64_t) INT32_MAX )
		{
			libewf_error_set(
			 &error,
			 LIBEWF_ERROR_DOMAIN_RUNTIME,
			 LIBEWF_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid chunk offset value exceeds maximum.\n",
			 function );

			libewf_error_backtrace_notify(
			 error );
			libewf_error_free(
			 &error );

			return( -1 );
		}
		internal_handle->current_chunk_offset = (uint32_t) chunk_offset;
	}
	else
	{
		internal_handle->current_chunk        = internal_handle->offset_table->amount_of_chunk_offsets;
		internal_handle->current_chunk_offset = 0;
	}
	return( offset );
}

/* Retrieves the current offset of the media data within the EWF file(s)
 * Returns the offset if successful or -1 on error
 */
off64_t libewf_get_offset(
         libewf_handle_t *handle )
{
	libewf_error_t *error                     = NULL;
	libewf_internal_handle_t *internal_handle = NULL;
	static char *function                     = "libewf_seek_offset";
	off64_t current_offset                    = 0;

	if( handle == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_ARGUMENTS,
		 LIBEWF_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	internal_handle = (libewf_internal_handle_t *) handle;

	if( internal_handle->media_values == NULL )
	{
		libewf_error_set(
		 &error,
		 LIBEWF_ERROR_DOMAIN_RUNTIME,
		 LIBEWF_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing media values.\n",
		 function );

		libewf_error_backtrace_notify(
		 error );
		libewf_error_free(
		 &error );

		return( -1 );
	}
	current_offset  = internal_handle->current_chunk * internal_handle->media_values->chunk_size;
	current_offset += internal_handle->current_chunk_offset;

	return( current_offset );
}


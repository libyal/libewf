/*
 * Segment file handle
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
#include <file_io.h>
#include <memory.h>
#include <notify.h>
#include <types.h>
#include <system_string.h>

#include "libewf_filename.h"
#include "libewf_segment_file_handle.h"

/* Initialize the segment file handle
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_initialize(
     libewf_segment_file_handle_t **segment_file_handle )
{
	static char *function = "libewf_segment_file_handle_initialize";

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( *segment_file_handle == NULL )
	{
		*segment_file_handle = (libewf_segment_file_handle_t *) memory_allocate(
		                                                         sizeof( libewf_segment_file_handle_t ) );

		if( *segment_file_handle == NULL )
		{
			notify_warning_printf( "%s: unable to create segment file handle.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *segment_file_handle,
		     0,
		     sizeof( libewf_segment_file_handle_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear segment file handle.\n",
			 function );

			memory_free(
			 *segment_file_handle );

			*segment_file_handle = NULL;

			return( -1 );
		}
		( *segment_file_handle )->section_list = (libewf_section_list_t *) memory_allocate(
		                                                                    sizeof( libewf_section_list_t ) );

		if( ( *segment_file_handle )->section_list == NULL )
		{
			notify_warning_printf( "%s: unable to create section list.\n",
			 function );

			memory_free(
			 *segment_file_handle );

			*segment_file_handle = NULL;

			return( -1 );
		}
		if( memory_set(
		     ( *segment_file_handle )->section_list,
		     0,
		     sizeof( libewf_section_list_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear section list.\n",
			 function );

			memory_free(
			 ( *segment_file_handle )->section_list );
			memory_free(
			 *segment_file_handle );

			*segment_file_handle = NULL;

			return( -1 );
		}
		( *segment_file_handle )->file_descriptor = -1;
	}
	return( 1 );
}

/* Frees the segment file handle including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_free(
     libewf_segment_file_handle_t **segment_file_handle )
{
        libewf_section_list_entry_t *section_list_entry         = NULL;
        libewf_section_list_entry_t *current_section_list_entry = NULL;
	static char *function                                   = "libewf_segment_file_handle_free";

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( *segment_file_handle != NULL )
	{
		if( ( *segment_file_handle )->filename != NULL )
		{
			memory_free(
			 ( *segment_file_handle )->filename );
		}
		if( ( *segment_file_handle )->section_list != NULL )
		{
			section_list_entry = ( *segment_file_handle )->section_list->first;

			while( section_list_entry != NULL )
			{
				current_section_list_entry = section_list_entry;
				section_list_entry         = section_list_entry->next;

				memory_free(
				 current_section_list_entry );
			}
			memory_free(
			 ( *segment_file_handle )->section_list );
		}
		memory_free(
		 *segment_file_handle );

		*segment_file_handle = NULL;
	}
	return( 1 );
}

/* Retrieves a filename of a certain segment file handle
 * Returns 1 if succesful or -1 on error
 */
int libewf_segment_file_handle_get_filename(
     libewf_segment_file_handle_t *segment_file_handle,
     system_character_t *filename,
     size_t length_filename )
{
	static char *function  = "libewf_segment_file_handle_get_filename";
	size_t filename_length = 0;

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	filename_length = segment_file_handle->length_filename;

	/* Add one additional character for the end of line
	 */
	filename_length += 1;

	if( length_filename < filename_length )
	{
		notify_warning_printf( "%s: filename too small.\n",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     filename,
	     segment_file_handle->filename,
	     filename_length ) == NULL )
	{
		notify_warning_printf( "%s: unable to set filename.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets a filename for a specific segment file handle
 * Creates a duplicate of the string
 * Returns 1 if succesful or -1 on error
 */
int libewf_segment_file_handle_set_filename(
     libewf_segment_file_handle_t *segment_file_handle,
     const system_character_t *filename,
     size_t length_filename )
{
	static char *function = "libewf_segment_file_handle_set_filename";

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename != NULL )
	{
		notify_warning_printf( "%s: filename already set: %" PRIs_SYSTEM ".\n",
		 function, segment_file_handle->filename );

		return( -1 );
	}
	if( length_filename == 0 )
	{
		notify_warning_printf( "%s: invalid filename length is zero.\n",
		 function );

		return( -1 );
	}
	if( length_filename >= (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid filename length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* One additional byte for the end of string character is needed
	 */
	segment_file_handle->filename = (system_character_t *) memory_allocate(
	                                                        sizeof( system_character_t ) * ( length_filename + 1 ) );

	if( segment_file_handle->filename == NULL )
	{
		notify_warning_printf( "%s: unable to create filename.\n",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     segment_file_handle->filename,
	     filename,
	     length_filename ) == NULL )
	{
		notify_warning_printf( "%s: unable to set filename.\n",
		 function );

		memory_free(
		 segment_file_handle->filename );

		segment_file_handle->filename = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	segment_file_handle->filename[ length_filename ] = '\0';
	segment_file_handle->length_filename             = length_filename;

	return( 1 );
}

/* Opens a segment file handle
 * Sets the filename and the file descriptor in the segment file handle struct
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_open(
     libewf_segment_file_handle_t *segment_file_handle,
     int flags )
{
	static char *function = "libewf_segment_file_handle_open";

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	segment_file_handle->file_descriptor = libewf_filename_open(
	                                        segment_file_handle->filename,
	                                        flags );

	if( segment_file_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: unable to open segment file: %" PRIs_SYSTEM ".\n",
		 function, segment_file_handle->filename );

		return( -1 );
	}
	return( 1 );
}

/* Re-opens a segment file handle
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_file_handle_reopen(
     libewf_segment_file_handle_t *segment_file_handle,
     int flags )
{
	static char *function = "libewf_segment_file_handle_reopen";

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( file_io_close(
	     segment_file_handle->file_descriptor ) != 0 )
	{
		notify_warning_printf( "%s: unable to close segment file: %" PRIs_SYSTEM ".\n",
		 function, segment_file_handle->filename );
	}
	segment_file_handle->file_descriptor = libewf_filename_open(
	                                        segment_file_handle->filename,
	                                        flags );

	if( segment_file_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: unable to open file: %" PRIs_SYSTEM ".\n",
		 function, segment_file_handle->filename );

		return( -1 );
	}
	/* Seek the previous file offset
	 */
	if( file_io_lseek(
	     segment_file_handle->file_descriptor,
	     segment_file_handle->file_offset,
	     SEEK_CUR ) == -1 )
	{
		notify_warning_printf( "%s: unable to seek in file: %" PRIs_SYSTEM ".\n",
		 function, segment_file_handle->filename );

		return( -1 );
	}
	return( 1 );
}

/* Reads a buffer from a segment file handle
 * Updates the file offset
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_segment_file_handle_read(
         libewf_segment_file_handle_t *segment_file_handle,
         void *buffer,
         size_t size )
{
	static char *function = "libewf_segment_file_handle_read";
	ssize_t read_count    = 0;

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid segment file handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		notify_warning_printf( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	read_count = file_io_read(
	              segment_file_handle->file_descriptor,
	              buffer,
	              size );

	if( read_count > 0 )
	{
		segment_file_handle->file_offset += (off64_t) read_count;
	}
	if( read_count != (ssize_t) size )
	{
		notify_warning_printf( "%s: unable to read from segment file: %" PRIs_SYSTEM ".\n",
		 function, segment_file_handle->filename );
	}
	return( read_count );
}

/* Writes a buffer to a segment file handle
 * Updates the file offset
 * Returns the amount of bytes written if successful, or -1 on errror
 */
ssize_t libewf_segment_file_handle_write(
         libewf_segment_file_handle_t *segment_file_handle,
         void *buffer,
         size_t size )
{
	static char *function = "libewf_segment_file_handle_write";
	ssize_t write_count   = 0;

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid segment file handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		notify_warning_printf( "%s: invalid buffer.\n",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	write_count = file_io_write(
	               segment_file_handle->file_descriptor,
	               buffer,
	               size );

	if( write_count > 0 )
	{
		segment_file_handle->file_offset += (off64_t) write_count;
	}
	if( write_count != (ssize_t) size )
	{
		notify_warning_printf( "%s: unable to write to segment file: %" PRIs_SYSTEM ".\n",
		 function, segment_file_handle->filename );
	}
	return( write_count );
}

/* Seeks a certain offset within the a segment file handle
 * Returns 1 if the seek is successful or -1 on error
 */
off64_t libewf_segment_file_handle_seek_offset(
         libewf_segment_file_handle_t *segment_file_handle,
         off64_t offset )
{
	static char *function = "libewf_segment_file_handle_seek_offset";

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid segment file handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) INT64_MAX )
	{
		notify_warning_printf( "%s: invalid offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_offset != offset )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: seeking offset: %" PRIjd " in segment file: %" PRIs_SYSTEM " with file descriptor: %d.\n",
		 function, offset, segment_file_handle->filename, segment_file_handle->file_descriptor );
#endif

		if( file_io_lseek(
		     segment_file_handle->file_descriptor,
		     offset,
		     SEEK_SET ) == -1 )
		{
			notify_warning_printf( "%s: unable to find offset: %" PRIjd " in segment file: %" PRIs_SYSTEM ".\n",
			 function, offset, segment_file_handle->filename );

			return( -1 );
		}
		segment_file_handle->file_offset = offset;
	}
	return( offset );
}

/* Closes a segment file handle
 * Returns 0 if successful or -1 on error
 */
int libewf_segment_file_handle_close(
     libewf_segment_file_handle_t *segment_file_handle )
{
	static char *function = "libewf_segment_file_handle_close";

	if( segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment file handle.\n",
		 function );

		return( -1 );
	}
	if( segment_file_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid segment file handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( file_io_close(
	     segment_file_handle->file_descriptor ) != 0 )
	{
		notify_warning_printf( "%s: unable to close segment file handle.\n",
		 function );

		return( -1 );
	}
	segment_file_handle->file_descriptor = -1;

	return( 0 );
}


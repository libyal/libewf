/*
 * File Input/Output (IO) handle
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
#include <notify.h>
#include <types.h>
#include <system_string.h>

#include "libewf_definitions.h"
#include "libewf_file_io_handle.h"
#include "libewf_filename.h"

/* Retrieves a filename of a certain file io handle
 * Returns 1 if succesful or -1 on error
 */
int libewf_file_io_handle_get_filename(
     libewf_file_io_handle_t *file_io_handle,
     system_character_t *filename,
     size_t filename_size )
{
	static char *function = "libewf_file_io_handle_get_filename";

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( filename_size < file_io_handle->filename_size )
	{
		notify_warning_printf( "%s: filename too small.\n",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     filename,
	     file_io_handle->filename,
	     file_io_handle->filename_size ) == NULL )
	{
		notify_warning_printf( "%s: unable to set filename.\n",
		 function );

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	filename[ file_io_handle->filename_size - 1 ] = 0;

	return( 1 );
}

/* Sets a filename for a specific file io handle
 * Creates a duplicate of the filename string
 * Returns 1 if succesful or -1 on error
 */
int libewf_file_io_handle_set_filename(
     libewf_file_io_handle_t *file_io_handle,
     const system_character_t *filename,
     size_t filename_size )
{
	static char *function = "libewf_file_io_handle_set_filename";

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		notify_warning_printf( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename != NULL )
	{
		notify_warning_printf( "%s: filename already set: %" PRIs_SYSTEM ".\n",
		 function, file_io_handle->filename );

		return( -1 );
	}
	if( filename_size == 0 )
	{
		notify_warning_printf( "%s: invalid filename size is zero.\n",
		 function );

		return( -1 );
	}
	if( filename_size >= (size_t) SSIZE_MAX )
	{
		notify_warning_printf( "%s: invalid filename size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	file_io_handle->filename = (system_character_t *) memory_allocate(
	                                                   sizeof( system_character_t ) * filename_size );

	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: unable to create filename.\n",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     file_io_handle->filename,
	     filename,
	     filename_size ) == NULL )
	{
		notify_warning_printf( "%s: unable to set filename.\n",
		 function );

		memory_free(
		 file_io_handle->filename );

		file_io_handle->filename = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	file_io_handle->filename[ filename_size - 1 ] = 0;

	file_io_handle->filename_size = filename_size;

	return( 1 );
}

/* Opens a file io handle
 * Sets the filename and the file descriptor in the file io handle struct
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_handle_open(
     libewf_file_io_handle_t *file_io_handle,
     int flags )
{
	static char *function = "libewf_file_io_handle_open";

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_descriptor == -1 )
	{
		file_io_handle->file_descriptor = libewf_filename_open(
						   file_io_handle->filename,
						   flags );

		if( file_io_handle->file_descriptor == -1 )
		{
			notify_warning_printf( "%s: unable to open file: %" PRIs_SYSTEM ".\n",
			 function, file_io_handle->filename );

			return( -1 );
		}
		file_io_handle->flags       = flags;
		file_io_handle->file_offset = 0;
	}
	return( 1 );
}

/* Reopens a file io handle
 * Sets the filename and the file descriptor in the file io handle struct
 * Returns 1 if successful or -1 on error
 */
int libewf_file_io_handle_reopen(
     libewf_file_io_handle_t *file_io_handle,
     int flags )
{
	static char *function = "libewf_file_io_handle_reopen";

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	/* Do not bother to reopen files if the flags are the same
	 */
	if( file_io_handle->flags == flags )
	{
		return( 1 );
	}
	if( file_io_handle->file_descriptor >= 0 )
	{
		if( file_io_close(
		     file_io_handle->file_descriptor ) != 0 )
		{
			notify_warning_printf( "%s: unable to close file: %" PRIs_SYSTEM ".\n",
			 function, file_io_handle->filename );

			return( -1 );
		}
	}
	file_io_handle->file_descriptor = libewf_filename_open(
					   file_io_handle->filename,
					   flags );

	if( file_io_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: unable to open file: %" PRIs_SYSTEM ".\n",
		 function, file_io_handle->filename );

		return( -1 );
	}
	file_io_handle->flags = flags;

	/* Seek the previous file offset
	 */
	if( file_io_lseek(
	     file_io_handle->file_descriptor,
	     file_io_handle->file_offset,
	     SEEK_CUR ) == -1 )
	{
		notify_warning_printf( "%s: unable to seek offset in file: %" PRIs_SYSTEM ".\n",
		 function, file_io_handle->filename );

		return( -1 );
	}
	return( 1 );
}

/* Closes a file io handle
 * Returns 0 if successful or -1 on error
 */
int libewf_file_io_handle_close(
     libewf_file_io_handle_t *file_io_handle )
{
	static char *function = "libewf_file_io_handle_close";

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_descriptor >= 0 )
	{
		if( file_io_close(
		     file_io_handle->file_descriptor ) != 0 )
		{
			notify_warning_printf( "%s: unable to close file: %" PRIs_SYSTEM ".\n",
			 function, file_io_handle->filename );

			return( -1 );
		}
		file_io_handle->file_descriptor = -1;
	}
	return( 0 );
}

/* Reads a buffer from a file io handle
 * Updates the file io handle offset
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_file_io_handle_read(
         libewf_file_io_handle_t *file_io_handle,
         uint8_t *buffer,
         size_t size )
{
	static char *function = "libewf_file_io_handle_read";
	ssize_t read_count    = 0;

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid file io handle - invalid file descriptor.\n",
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
	              file_io_handle->file_descriptor,
	              buffer,
	              size );

	if( read_count > 0 )
	{
		file_io_handle->file_offset += (off64_t) read_count;
	}
	if( read_count != (ssize_t) size )
	{
		notify_warning_printf( "%s: unable to read from file: %" PRIs_SYSTEM ".\n",
		 function, file_io_handle->filename );
	}
	return( read_count );
}

/* Writes a buffer to a file io handle
 * Updates the file io handle offset
 * Returns the amount of bytes written if successful, or -1 on errror
 */
ssize_t libewf_file_io_handle_write(
         libewf_file_io_handle_t *file_io_handle,
         uint8_t *buffer,
         size_t size )
{
	static char *function = "libewf_file_io_handle_write";
	ssize_t write_count   = 0;

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid file io handle - invalid file descriptor.\n",
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
	               file_io_handle->file_descriptor,
	               buffer,
	               size );

	if( write_count > 0 )
	{
		file_io_handle->file_offset += (off64_t) write_count;
	}
	if( write_count != (ssize_t) size )
	{
		notify_warning_printf( "%s: unable to write to file: %" PRIs_SYSTEM ".\n",
		 function, file_io_handle->filename );
	}
	return( write_count );
}

/* Seeks a certain offset within the a file io handle
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t libewf_file_io_handle_seek_offset(
         libewf_file_io_handle_t *file_io_handle,
         off64_t offset,
         int whence )
{
	static char *function = "libewf_file_io_handle_seek_offset";

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid file io handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( offset > (off64_t) INT64_MAX )
	{
		notify_warning_printf( "%s: invalid offset value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_SET ) )
	{
		notify_warning_printf( "%s: unsupported whence.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_offset != offset )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( whence == SEEK_CUR )
		{	
			notify_verbose_printf( "%s: seeking offset: %" PRIjd " in file: %" PRIs_SYSTEM " with file descriptor: %d.\n",
			 function, ( file_io_handle->file_offset + offset ), file_io_handle->filename, file_io_handle->file_descriptor );
		}
		else if( whence == SEEK_SET )
		{
			notify_verbose_printf( "%s: seeking offset: %" PRIjd " in file: %" PRIs_SYSTEM " with file descriptor: %d.\n",
			 function, offset, file_io_handle->filename, file_io_handle->file_descriptor );
		}
#endif

		if( file_io_lseek(
		     file_io_handle->file_descriptor,
		     offset,
		     whence ) == -1 )
		{
			notify_warning_printf( "%s: unable to find offset: %" PRIjd " in file: %" PRIs_SYSTEM ".\n",
			 function, offset, file_io_handle->filename );

			return( -1 );
		}
		file_io_handle->file_offset = offset;
	}
	return( offset );
}

/* Retrieves the current offset within the a file io handle
 * Returns the offset if the seek is successful or -1 on error
 */
int libewf_file_io_handle_get_offset(
     libewf_file_io_handle_t *file_io_handle,
     off64_t *offset )
{
	static char *function = "libewf_file_io_handle_get_offset";

	if( file_io_handle == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: invalid file io handle - invalid file descriptor.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: invalid file io handle - missing filename.\n",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		notify_warning_printf( "%s: invalid offset.\n",
		 function );

		return( -1 );
	}
	*offset = file_io_handle->file_offset;

	return( 1 );
}


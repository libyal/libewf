/*
 * libewf file io pool
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

#include <common.h>
#include <file_io.h>
#include <notify.h>
#include <types.h>
#include <system_string.h>

#include <libewf/definitions.h>

#include "libewf_file_io_handle.h"
#include "libewf_filename.h"

/* Opens a file io handle
 * Sets the filename and the file descriptor in the file io handle struct
 * Returns 1 if successful, or -1 on error
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
	file_io_handle->file_descriptor = libewf_filename_open(
	                                   file_io_handle->filename,
	                                   flags );

	if( file_io_handle->file_descriptor == -1 )
	{
		notify_warning_printf( "%s: unable to open file io handle: %" PRIs_SYSTEM ".\n",
		 function, file_io_handle->filename );

		return( -1 );
	}
	file_io_handle->flags = flags;

	return( 1 );
}

/* Reads a buffer from a file io handle
 * Updates the file io handle offset
 * Returns the amount of bytes read if successful, or -1 on errror
 */
ssize_t libewf_file_io_handle_read(
         libewf_file_io_handle_t *file_io_handle,
         void *buffer,
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
		notify_warning_printf( "%s: unable to read from file io handle: %" PRIs_SYSTEM ".\n",
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
         void *buffer,
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
		notify_warning_printf( "%s: unable to write to file io handle: %" PRIs_SYSTEM ".\n",
		 function, file_io_handle->filename );
	}
	return( write_count );
}

/* Seeks a certain offset within the a file io handle
 * Returns 1 if the seek is successful, or -1 on error
 */
off64_t libewf_file_io_handle_seek_offset(
         libewf_file_io_handle_t *file_io_handle,
         off64_t offset )
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
	if( file_io_handle->file_offset != offset )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: seeking offset: %" PRIjd " in file io handle: %" PRIs_SYSTEM " with file descriptor: %d.\n",
		 function, offset, file_io_handle->filename, file_io_handle->file_descriptor );
#endif

		if( file_io_lseek(
		     file_io_handle->file_descriptor,
		     offset,
		     SEEK_SET ) == -1 )
		{
			notify_warning_printf( "%s: unable to find offset: %" PRIjd " in file io handle: %" PRIs_SYSTEM ".\n",
			 function, offset, file_io_handle->filename );

			return( -1 );
		}
		file_io_handle->file_offset = offset;
	}
	return( offset );
}


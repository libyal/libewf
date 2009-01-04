/*
 * Segment table functions
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
#include <system_string.h>
#include <types.h>

#include <libewf/definitions.h>

#include "libewf_filename.h"
#include "libewf_file_io_handle.h"
#include "libewf_file_io_pool.h"
#include "libewf_segment_file.h"
#include "libewf_segment_table.h"

/* Initialize the hash sections
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_initialize(
     libewf_segment_table_t **segment_table,
     uint16_t amount )
{
	static char *function     = "libewf_segment_table_initialize";
	size_t segment_table_size = 0;

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( amount == 0 )
	{
		notify_warning_printf( "%s: invalid amount value cannot be zero.\n",
		 function );

		return( -1 );
	}
	if( *segment_table == NULL )
	{
		segment_table_size = sizeof( libewf_segment_file_handle_t * ) * amount;

		if( segment_table_size > (size_t) SSIZE_MAX )
		{
			notify_warning_printf( "%s: invalid segment table size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		*segment_table = (libewf_segment_table_t *) memory_allocate(
		                                             sizeof( libewf_segment_table_t ) );

		if( *segment_table == NULL )
		{
			notify_warning_printf( "%s: unable to create segment table.\n",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *segment_table,
		     0,
		     sizeof( libewf_segment_table_t ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear segment table.\n",
			 function );

			memory_free(
			 *segment_table );

			*segment_table = NULL;

			return( -1 );
		}
		( *segment_table )->segment_file_handle = (libewf_segment_file_handle_t **) memory_allocate(
		                                                                             segment_table_size );

		if( ( *segment_table )->segment_file_handle == NULL )
		{
			notify_warning_printf( "%s: unable to create segment file array.\n",
			 function );

			memory_free(
			 segment_table );

			*segment_table = NULL;

			return( -1 );
		}
		if( memory_set(
		     ( *segment_table )->segment_file_handle,
		     0, 
		     segment_table_size ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear segment file array.\n",
			 function );

			memory_free(
			 ( *segment_table )->segment_file_handle );
			memory_free(
			 *segment_table );

			*segment_table = NULL;

			return( -1 );
		} 
		( *segment_table )->amount = amount;
	}
	return( 1 );
}

/* Frees the hash sections including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_free(
     libewf_segment_table_t **segment_table )
{
	static char *function = "libewf_segment_table_free";
	uint16_t iterator     = 0;

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( *segment_table != NULL )
	{
		for( iterator = 0; iterator < ( *segment_table )->amount; iterator++ )
		{
			if( libewf_segment_file_handle_free(
			     &( ( *segment_table )->segment_file_handle[ iterator ] ) ) != 1 )
			{
				notify_warning_printf( "%s: unable to free segment file handle: %" PRIu16 ".\n",
				 function, ( iterator + 1 ) );
			}
		}
		memory_free(
		 ( *segment_table )->segment_file_handle );

		if( ( *segment_table )->basename != NULL )
		{
			memory_free(
			 ( *segment_table )->basename );
		}
		memory_free(
		 *segment_table );

		*segment_table = NULL;
	}
	return( 1 );
}

/* Resizes the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_resize(
     libewf_segment_table_t *segment_table,
     uint16_t amount )
{
	void *reallocation        = NULL;
	static char *function     = "libewf_segment_table_resize";
	size_t segment_table_size = 0;

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( segment_table->amount < amount )
	{
		segment_table_size = sizeof( libewf_segment_file_handle_t * ) * amount;

		if( segment_table_size > (size_t) SSIZE_MAX )
		{
			notify_warning_printf( "%s: invalid segment table size value exceeds maximum.\n",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
				segment_table->segment_file_handle,
				segment_table_size );

		if( reallocation == NULL )
		{
			notify_warning_printf( "%s: unable to resize file handle array.\n",
			 function );

			return( -1 );
		}
		segment_table->segment_file_handle = (libewf_segment_file_handle_t **) reallocation;

		if( memory_set(
		     &( segment_table->segment_file_handle[ segment_table->amount ] ),
		     0, 
		     ( ( amount - segment_table->amount ) * sizeof( libewf_segment_file_handle_t * ) ) ) == NULL )
		{
			notify_warning_printf( "%s: unable to clear file handle array.\n",
			 function );

			return( 1 );
		} 
		segment_table->amount = amount;
	}
	return( 1 );
}

/* Builds the segment table from all segment files
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_table_build(
     libewf_segment_table_t *segment_table,
     libewf_file_io_pool_t *file_io_pool,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     uint8_t error_tollerance,
     int *abort )
{
	static char *function   = "libewf_segment_table_build";
	uint16_t segment_number = 0;
	int last_segment_file   = 0;
	int result              = 0;

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment table - missing segment file handles.\n",
		 function );

		return( -1 );
	}
	/* Read the segment and offset table from the segment file(s)
	 */
	for( segment_number = 1; segment_number < segment_table->amount; segment_number++ )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: reading section list for segment number: %" PRIu16 ".\n",
		 function, segment_number );
#endif

		result = libewf_segment_file_read_sections(
		          segment_table->segment_file_handle[ segment_number ],
		          file_io_pool,
		          &last_segment_file,
		          header_sections,
		          hash_sections,
		          media_values,
		          offset_table,
		          sessions,
		          acquiry_errors,
		          compression_level,
		          format,
		          ewf_format,
		          segment_file_size,
		          error_tollerance );

		if( result == -1 )
		{
			notify_warning_printf( "%s: unable to read sections.\n",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			notify_warning_printf( "%s: unable to missing next or done section.\n",
			 function );

			return( 0 );
		}
		if( *abort == 1 )
		{
			break;
		}
	}
	/* Check to see if the done section has been found in the last segment file
	 */
	if( last_segment_file != 1 )
	{
		notify_warning_printf( "%s: unable to find the last segment file.\n",
		 function );

		return( 0 );
	}
	return( 1 );
}

/* retrieves the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libewf_segment_table_get_basename(
     libewf_segment_table_t *segment_table,
     system_character_t *basename,
     size_t length )
{
	static char *function = "libewf_segment_table_get_basename";

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		notify_warning_printf( "%s: invalid basename.\n",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
	/* Make sure to include the end of string character
	 */
	if( length < ( segment_table->basename_length + 1 ) )
	{
		notify_warning_printf( "%s: basename too small.\n",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     basename,
	     segment_table->basename,
	     ( segment_table->basename_length + 1 ) ) == NULL )
	{
		notify_warning_printf( "%s: unable to set basename.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the basename in the segment table
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_set_basename(
     libewf_segment_table_t *segment_table,
     system_character_t *basename,
     size_t basename_length )
{
	static char *function = "libewf_segment_table_set_basename";

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		notify_warning_printf( "%s: invalid basename.\n",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename        = NULL;
		segment_table->basename_length = 0;
	}
	segment_table->basename = system_string_duplicate(
	                           basename,
	                           basename_length + 1 );

	if( segment_table->basename == NULL )
	{
		notify_warning_printf( "%s: unable to set basename.\n",
		 function );

		return( -1 );
	}
	segment_table->basename_length = basename_length;

	return( 1 );
}

/* Initializes the segment table
 * Opens EWF segment files for reading and EWF delta segment files for reading and writing
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_table_read_open(
     libewf_segment_table_t *segment_table,
     libewf_segment_table_t *delta_segment_table,
     libewf_file_io_pool_t *file_io_pool,
     system_character_t * const filenames[],
     uint16_t file_amount, uint8_t flags,
     libewf_header_sections_t *header_sections,
     libewf_hash_sections_t *hash_sections,
     libewf_media_values_t *media_values,
     libewf_offset_table_t *offset_table,
     libewf_sector_table_t *sessions,
     libewf_sector_table_t *acquiry_errors,
     int8_t *compression_level,
     uint8_t *format,
     uint8_t *ewf_format,
     size64_t *segment_file_size,
     uint8_t error_tollerance,
     int *abort )
{
	libewf_file_io_handle_t *file_io_handle           = NULL;
	libewf_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                             = "libewf_segment_table_read_open";
	size_t filename_size                              = 0;
	uint32_t iterator                                 = 0;
	uint16_t segment_number                           = 0;
	int file_io_pool_entry                            = 0;

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment table - missing segment file handles.\n",
		 function );

		return( -1 );
	}
	if( delta_segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid delta segment table.\n",
		 function );

		return( -1 );
	}
	if( delta_segment_table->segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid delta segment table - missing segment file handles.\n",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		notify_warning_printf( "%s: invalid filenames.\n",
		 function );

		return( -1 );
	}
	if( file_amount < 1 )
	{
		notify_warning_printf( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( -1 );
	}
	/* Get the basename of the first segment file
	 * and store it in the segment tables
	 */
	filename_size = system_string_length(
	                 filenames[ 0 ] );

        /* Make sure there is more to the filename than the extension
	 */
	if( filename_size <= 4 )
	{
		notify_warning_printf( "%s: filename is too small.\n",
		 function );

		return( -1 );
	}
	/* Set segment table basename
	 */
	if( libewf_segment_table_set_basename(
	     segment_table,
	     filenames[ 0 ],
	     ( filename_size - 4 ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set basename in segment table.\n",
		 function );

		return( -1 );
	}
	/* Set delta segment table basename
	 */
	if( libewf_segment_table_set_basename(
	     delta_segment_table,
	     filenames[ 0 ],
	     ( filename_size - 4 ) ) != 1 )
	{
		notify_warning_printf( "%s: unable to set basename in delta segment table.\n",
		 function );

		return( -1 );
	}
	/* Open the segment files
	 */
	for( iterator = 0; iterator < file_amount; iterator++ )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		notify_verbose_printf( "%s: trying to open file: %" PRIs_SYSTEM ".\n",
		 function, filenames[ iterator ] );
#endif

		if( libewf_file_io_pool_create_file_io_handle(
		     file_io_pool,
		     &file_io_handle,
		     &file_io_pool_entry ) != 1 )
		{
			notify_warning_printf( "%s: unable to create file io pool entry.\n",
			 function );

			return( -1 );
		}
		filename_size = 1 + system_string_length(
		                     filenames[ iterator ] );

		if( libewf_file_io_handle_set_filename(
		     file_io_handle,
		     filenames[ iterator ],
		     filename_size ) != 1 )
		{
			notify_warning_printf( "%s: unable to set filename in file io handle.\n",
			 function );

			return( -1 );
		}
		if( libewf_file_io_pool_open(
		     file_io_pool,
		     file_io_pool_entry,
		     FILE_IO_O_RDONLY ) != 1 )
		{
			notify_warning_printf( "%s: unable to open segment file: %" PRIs_SYSTEM ".\n",
			 function, filenames[ iterator ] );

			return( -1 );
		}
		if( libewf_segment_file_handle_initialize(
		     &segment_file_handle,
		     file_io_pool_entry ) != 1 )
		{
			notify_warning_printf( "%s: unable to create segment file handle.\n",
			 function );

			return( -1 );
		}
		if( libewf_segment_file_read_file_header(
		     segment_file_handle,
		     &segment_number,
		     file_io_pool ) <= -1 )
		{
			notify_warning_printf( "%s: unable to read file header in: %" PRIs_SYSTEM ".\n",
			 function, filenames[ iterator ] );

			libewf_segment_file_handle_free(
			 &segment_file_handle );

			return( -1 );
		}
		if( segment_number == 0 )
		{
			notify_warning_printf( "%s: invalid segment number: 0.\n",
			 function );

			libewf_segment_file_handle_free(
			 &segment_file_handle );

			return( -1 );
		}
		if( segment_number > file_amount )
		{
			notify_warning_printf( "%s: invalid segment number, value out of range or missing segment files.\n",
			 function );

			libewf_segment_file_handle_free(
			 &segment_file_handle );

			return( -1 );
		}
		if( ( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_EWF )
		 || ( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_LWF ) )
		{
			if( segment_number >= segment_table->amount )
			{
				if( libewf_segment_table_resize(
				     segment_table,
				     ( segment_number + 1 ) ) != 1 )
				{
					notify_warning_printf( "%s: unable to resize the segment table.\n",
					 function );

					return( -1 );
				}
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: added segment file: %" PRIs_SYSTEM " with file descriptor: %d with segment number: %" PRIu16 ".\n",
			 function, file_io_handle->filename, file_io_handle->file_descriptor, segment_number );
#endif

			segment_table->segment_file_handle[ segment_number ] = segment_file_handle;
		}
		else if( segment_file_handle->file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
		{
			/* Make sure to re-open the delta segment file with write access
			 */
			if( ( flags & LIBEWF_FLAG_WRITE ) == LIBEWF_FLAG_WRITE )
			{
				if( libewf_file_io_pool_reopen(
				     file_io_pool,
				     file_io_pool_entry,
				     FILE_IO_O_RDWR ) != 1 )
				{
					notify_warning_printf( "%s: unable to reopen segment file: %" PRIs_SYSTEM ".\n",
					 function, filenames[ iterator ] );

					return( -1 );
				}
			}
			if( segment_number >= delta_segment_table->amount )
			{
				if( libewf_segment_table_resize(
				     delta_segment_table,
				     ( segment_number + 1 ) ) != 1 )
				{
					notify_warning_printf( "%s: unable to resize the delta segment table.\n",
					 function );

					return( -1 );
				}
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			notify_verbose_printf( "%s: added delta segment file: %" PRIs_SYSTEM " with file descriptor: %d with segment number: %" PRIu16 ".\n",
			 function, file_io_handle->filename, file_io_handle->file_descriptor, segment_number );
#endif

			delta_segment_table->segment_file_handle[ segment_number ] = segment_file_handle;
		}
		else
		{
			notify_warning_printf( "%s: unsupported segment file type.\n",
			 function );

			return( -1 );
		}
		segment_file_handle = NULL;
	}
	if( libewf_segment_table_build(
	     segment_table,
	     file_io_pool,
	     header_sections,
	     hash_sections,
	     media_values,
	     offset_table,
	     sessions,
	     acquiry_errors,
	     compression_level,
	     format,
	     ewf_format,
	     segment_file_size,
	     error_tollerance,
	     abort ) != 1 )
	{
		notify_warning_printf( "%s: unable to build segment table.\n",
		 function );

		return( -1 );
	}
	if( ( delta_segment_table->amount > 1 )
	 && ( libewf_segment_table_build(
	       delta_segment_table,
	       file_io_pool,
	       header_sections,
	       hash_sections,
	       media_values,
	       offset_table,
	       sessions,
	       acquiry_errors,
	       compression_level,
	       format,
	       ewf_format,
	       segment_file_size,
	       error_tollerance,
	       abort ) != 1 ) )
	{
		notify_warning_printf( "%s: unable to build delta segment table.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Initializes the segment table
 * Opens EWF segment file(s) for writing
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libewf_segment_table_write_open(
     libewf_segment_table_t *segment_table,
     system_character_t * const filenames[],
     uint16_t file_amount )
{
	static char *function  = "libewf_segment_table_write_open";
	size_t filename_length = 0;

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		notify_warning_printf( "%s: invalid filenames.\n",
		 function );

		return( -1 );
	}
	if( file_amount < 1 )
	{
		notify_warning_printf( "%s: invalid file amount at least 1 is required.\n",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filenames[ 0 ] );

	if( filename_length == 0 )
	{
		notify_warning_printf( "%s: filename is empty.\n",
		 function );

		return( -1 );
	}
	/* Set segment table basename
	 */
	if( libewf_segment_table_set_basename(
	     segment_table,
	     filenames[ 0 ],
	     filename_length ) != 1 )
	{
		notify_warning_printf( "%s: unable to set basename in segment table.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Creates a new segment file and opens it for writing
 * The necessary sections at the start of the segment file are written
 * Returns 1 if successful or -1 on error
 */
int libewf_segment_table_create_segment_file(
     libewf_segment_table_t *segment_table,
     uint16_t segment_number,
     libewf_file_io_pool_t *file_io_pool,
     int16_t maximum_amount_of_segments,
     uint8_t segment_file_type,
     uint8_t format,
     uint8_t ewf_format )
{
	libewf_file_io_handle_t *file_io_handle = NULL;
	static char *function                   = "libewf_segment_table_create_segment_file";
	int file_io_pool_entry                  = 0;
	int flags                               = 0;

	if( segment_table == NULL )
	{
		notify_warning_printf( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( segment_table->segment_file_handle == NULL )
	{
		notify_warning_printf( "%s: invalid segment table - missing segment file handles.\n",
		 function );

		return( -1 );
	}
	if( segment_number == 0 )
	{
		notify_warning_printf( "%s: invalid segment number: 0.\n",
		 function );

		return( -1 );
	}
	if( segment_number > segment_table->amount )
	{
		notify_warning_printf( "%s: invalid segment number value out of range.\n",
		 function );

		return( -1 );
	}
	/* Check if one additional entries in the segment table are needed
	 */
	if( segment_number >= segment_table->amount )
	{
		/* Add one additional entry because the 0 entry is used for the basename
		 */
		if( libewf_segment_table_resize(
		     segment_table,
		     ( segment_number + 1 ) ) != 1 )
		{
			notify_warning_printf( "%s: unable to resize segment table.\n",
			 function );

			return( -1 );
		}
	}
	/* Check if the entry has already been filled
	 */
	if( segment_table->segment_file_handle[ segment_number ] != NULL )
	{
		notify_warning_printf( "%s: segment file has already been created.\n",
		 function );

		return( -1 );
	}
	if( libewf_file_io_pool_create_file_io_handle(
	     file_io_pool,
	     &file_io_handle,
	     &file_io_pool_entry ) != 1 )
	{
		notify_warning_printf( "%s: unable to create file io pool entry.\n",
		 function );

		return( -1 );
	}
	if( libewf_filename_create(
	     &( file_io_handle->filename ),
	     &( file_io_handle->filename_size ),
	     segment_table->basename,
	     segment_table->basename_length,
	     segment_number,
	     maximum_amount_of_segments,
	     segment_file_type,
	     format,
	     ewf_format ) != 1 )
	{
		notify_warning_printf( "%s: unable to create segment file filename.\n",
		 function );

		return( -1 );
	}
	if( file_io_handle->filename == NULL )
	{
		notify_warning_printf( "%s: filename is empty.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	notify_verbose_printf( "%s: created segment file: %" PRIu16 " with filename: %" PRIs_SYSTEM ".\n",
	 function, segment_number, file_io_handle->filename );
#endif

	if( segment_file_type == LIBEWF_SEGMENT_FILE_TYPE_DWF )
	{
		flags = FILE_IO_O_RDWR | FILE_IO_O_CREAT | FILE_IO_O_TRUNC;
	}
	else
	{
		flags = FILE_IO_O_WRONLY | FILE_IO_O_CREAT | FILE_IO_O_TRUNC;
	}
	if( libewf_file_io_pool_open(
	     file_io_pool,
	     file_io_pool_entry,
	     flags ) != 1 )
	{
		notify_warning_printf( "%s: unable to open segment file: %" PRIu16 ".\n",
		 function, segment_number );

		return( -1 );
	}
	if( libewf_segment_file_handle_initialize(
	     &( segment_table->segment_file_handle[ segment_number ] ),
	     file_io_pool_entry ) != 1 )
	{
		notify_warning_printf( "%s: unable to create segment file handle.\n",
		 function );

		return( -1 );
	}
	return( 1 );
}


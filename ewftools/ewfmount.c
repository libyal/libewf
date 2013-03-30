/*
 * Mounts an EWF file
 *
 * Copyright (C) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_SYS_RESOURCE_H )
#include <sys/resource.h>
#endif

#if !defined( WINAPI ) || defined( USE_CRT_FUNCTIONS )
#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
#define FUSE_USE_VERSION	26

#if defined( HAVE_LIBFUSE )
#include <fuse.h>

#elif defined( HAVE_LIBOSXFUSE )
#include <osxfuse/fuse.h>
#endif

#elif defined( HAVE_LIBDOKAN )
#include <dokan.h>
#endif

#include "ewfoutput.h"
#include "ewftools_libcerror.h"
#include "ewftools_libclocale.h"
#include "ewftools_libcnotify.h"
#include "ewftools_libcstring.h"
#include "ewftools_libcsystem.h"
#include "ewftools_libewf.h"
#include "mount_handle.h"

mount_handle_t *ewfmount_mount_handle = NULL;
int ewfmount_abort                    = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use ewfmount to mount the EWF format (Expert Witness\n"
                         "Compression Format)\n\n" );

	fprintf( stream, "Usage: ewfmount [ -f format ] [ -X extended_options ] [ -hvV ]\n"
	                 "                ewf_files mount_point\n\n" );

	fprintf( stream, "\tewf_files:   the first or the entire set of EWF segment files\n\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-f:          specify the input format, options: raw (default),\n"
	                 "\t             files (restricted to logical volume files)\n" );
	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-v:          verbose output to stderr\n"
	                 "\t             ewfmount will remain running in the foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for ewfmount
 */
void ewfmount_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "ewfmount_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

	ewfmount_abort = 1;

	if( ewfmount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     ewfmount_mount_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal mount handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libcsystem_file_io_close(
	     0 ) != 0 )
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )

#if ( SIZEOF_OFF_T != 8 ) && ( SIZEOF_OFF_T != 4 )
#error Size of off_t not supported
#endif

static char *ewfmount_fuse_path         = "/ewf1";
static size_t ewfmount_fuse_path_length = 5;

/* Opens a file or directory
 * Returns 0 if successful or a negative errno value otherwise
 */
int ewfmount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info )
{
	libcerror_error_t *error        = NULL;
	libewf_file_entry_t *file_entry = NULL;
	static char *function           = "ewfmount_fuse_open";
	size_t path_length              = 0;
	int result                      = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( mount_handle_get_file_entry_by_path(
		     ewfmount_mount_handle,
		     path,
		     path_length,
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry for: %s.",
			 function,
			 path );

			result = -ENOENT;

			goto on_error;
		}
		if( libewf_file_entry_free(
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( path_length != ewfmount_fuse_path_length )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path length.",
			 function );

			result = -ENOENT;

			goto on_error;
		}
		if( libcstring_narrow_string_compare(
		     path,
		     ewfmount_fuse_path,
		     ewfmount_fuse_path_length ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path.",
			 function );

			result = -ENOENT;

			goto on_error;
		}
	}
	if( ( file_info->flags & 0x03 ) != O_RDONLY )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		result = -EACCES;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns number of bytes read if successful or a negative errno value otherwise
 */
int ewfmount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error        = NULL;
	libewf_file_entry_t *file_entry = NULL;
	static char *function           = "ewfmount_fuse_read";
	size_t path_length              = 0;
	ssize_t read_count              = 0;
	int result                      = 0;

	LIBCSYSTEM_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( size > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( mount_handle_get_file_entry_by_path(
		     ewfmount_mount_handle,
		     path,
		     path_length,
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry for: %s.",
			 function,
			 path );

			result = -ENOENT;

			goto on_error;
		}
		if( libewf_file_entry_seek_offset(
		     file_entry,
		     (off64_t) offset,
		     SEEK_SET,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in file entry.",
			 function );

			result = -EIO;

			goto on_error;
		}
		read_count = libewf_file_entry_read_buffer(
			      file_entry,
			      (uint8_t *) buffer,
			      size,
			      &error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from file entry.",
			 function );

			result = -EIO;

			goto on_error;
		}
		if( libewf_file_entry_free(
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( path_length != ewfmount_fuse_path_length )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path length.",
			 function );

			result = -ENOENT;

			goto on_error;
		}
		if( libcstring_narrow_string_compare(
		     path,
		     ewfmount_fuse_path,
		     ewfmount_fuse_path_length ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path.",
			 function );

			result = -ENOENT;

			goto on_error;
		}
		if( mount_handle_seek_offset(
		     ewfmount_mount_handle,
		     (off64_t) offset,
		     SEEK_SET,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in mount handle.",
			 function );

			result = -EIO;

			goto on_error;
		}
		read_count = mount_handle_read_buffer(
			      ewfmount_mount_handle,
			      (uint8_t *) buffer,
			      size,
			      &error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from mount handle.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	return( (int) read_count );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a directory
 * Returns 0 if successful or a negative errno value otherwise
 */
int ewfmount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset LIBCSYSTEM_ATTRIBUTE_UNUSED,
     struct fuse_file_info *file_info LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error            = NULL;
	libewf_file_entry_t *file_entry     = NULL;
	libewf_file_entry_t *sub_file_entry = NULL;
	char *name                          = NULL;
	static char *function               = "ewfmount_fuse_readdir";
	size_t name_index                   = 0;
	size_t name_size                    = 0;
	size_t path_length                  = 0;
	int number_of_sub_file_entries      = 0;
	int result                          = 0;
	int sub_file_entry_index            = 0;

	LIBCSYSTEM_UNREFERENCED_PARAMETER( offset )
	LIBCSYSTEM_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( mount_handle_get_file_entry_by_path(
		     ewfmount_mount_handle,
		     path,
		     path_length,
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry for: %s.",
			 function,
			 path );

			result = -ENOENT;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( ( path_length != 1 )
		 || ( path[ 0 ] != '/' ) )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path.",
			 function );

			result = -ENOENT;

			goto on_error;
		}
	}
	if( filler(
	     buffer,
	     ".",
	     NULL,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( filler(
	     buffer,
	     "..",
	     NULL,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( libewf_file_entry_get_number_of_sub_file_entries(
		     file_entry,
		     &number_of_sub_file_entries,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub file entries.",
			 function );

			result = -EIO;

			goto on_error;
		}
		for( sub_file_entry_index = 0;
		     sub_file_entry_index < number_of_sub_file_entries;
		     sub_file_entry_index++ )
		{
			if( libewf_file_entry_get_sub_file_entry(
			     file_entry,
			     sub_file_entry_index,
			     &sub_file_entry,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to free retrieve sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				result = -EIO;

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libewf_file_entry_get_utf16_name_size(
				  sub_file_entry,
				  &name_size,
				  &error );
#else
			result = libewf_file_entry_get_utf8_name_size(
				  sub_file_entry,
				  &name_size,
				  &error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the sub file entry name size.",
				 function );

				result = -EIO;

				goto on_error;
			}
			if( name_size > 0 )
			{
				name = libcstring_system_string_allocate(
					name_size );

				if( name == NULL )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create sub file entry name.",
					 function );

					result = -EIO;

					goto on_error;
				}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libewf_file_entry_get_utf16_name(
					  sub_file_entry,
					  (uint16_t *) name,
					  name_size,
					  &error );
#else
				result = libewf_file_entry_get_utf8_name(
					  sub_file_entry,
					  (uint8_t *) name,
					  name_size,
					  &error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the sub file entry name.",
					 function );

					result = -EIO;

					goto on_error;
				}
				/* Exchange / for \
				 */
				for( name_index = 0;
				     name_index < name_size;
				     name_index++ )
				{
					if( name[ name_index ] == (libcstring_system_character_t) '/' )
					{
						 name[ name_index ] = (libcstring_system_character_t) '\\';
					}
				}
				if( filler(
				     buffer,
				     name,
				     NULL,
				     0 ) == 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set directory entry.",
					 function );

					result = -EIO;

					goto on_error;
				}
				memory_free(
				 name );

				name = NULL;
			}
			if( libewf_file_entry_free(
			     &sub_file_entry,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				result = -EIO;

				goto on_error;
			}
		}
		if( libewf_file_entry_free(
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( filler(
		     buffer,
		     &( ewfmount_fuse_path[ 1 ] ),
		     NULL,
		     0 ) == 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set directory entry.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	if( sub_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( file_entry != NULL )
	{
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( result );
}

/* Retrieves the file stat info
 * Returns 0 if successful or a negative errno value otherwise
 */
int ewfmount_fuse_getattr(
     const char *path,
     struct stat *stat_info )
{
	libcerror_error_t *error        = NULL;
	libewf_file_entry_t *file_entry = NULL;
	static char *function           = "ewfmount_fuse_getattr";
	size64_t file_size              = 0;
	size_t path_length              = 0;
	uint32_t value_32bit            = 0;
	int number_of_sub_file_entries  = 0;
	int result                      = -ENOENT;

#if defined( HAVE_TIME )
	time_t timestamp                = 0;
#endif

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( stat_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( memory_set(
	     stat_info,
	     0,
	     sizeof( struct stat ) ) == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear stat info.",
		 function );

		result = errno;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] == '/' )
		{
			stat_info->st_mode  = S_IFDIR | 0755;
			stat_info->st_nlink = 2;

			result = 0;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		result = mount_handle_get_file_entry_by_path(
		          ewfmount_mount_handle,
		          path,
		          path_length,
		          &file_entry,
		          &error );

		if( result == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry for: %s.",
			 function,
			 path );

			result = -ENOENT;

			goto on_error;
		}
		else if( result == 1 )
		{
			if( libewf_file_entry_get_number_of_sub_file_entries(
			     file_entry,
			     &number_of_sub_file_entries,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of sub file entries.",
				 function );

				result = -EIO;

				goto on_error;
			}
			if( number_of_sub_file_entries == 0 )
			{
				stat_info->st_mode = S_IFREG | 0444;
			}
			else
			{
				stat_info->st_mode = S_IFDIR | 0555;
			}
			stat_info->st_nlink = 1;

			if( libewf_file_entry_get_size(
			     file_entry,
			     &file_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file entry size.",
				 function );

				result = -EIO;

				goto on_error;
			}
#if SIZEOF_OFF_T == 4
			if( file_size > (size64_t) UINT32_MAX )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid to file entry size value out of bounds.",
				 function );

				result = -ERANGE;

				goto on_error;
			}
#endif
			stat_info->st_size = (off_t) file_size;

			if( libewf_file_entry_get_modification_time(
			     file_entry,
			     &value_32bit,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file entry modification time.",
				 function );

				result = -EIO;

				goto on_error;
			}
			stat_info->st_mtime = value_32bit;

			if( libewf_file_entry_get_access_time(
			     file_entry,
			     &value_32bit,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file entry access time.",
				 function );

				result = -EIO;

				goto on_error;
			}
			stat_info->st_atime = value_32bit;

			if( libewf_file_entry_get_entry_modification_time(
			     file_entry,
			     &value_32bit,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve file entry entry modification time.",
				 function );

				result = -EIO;

				goto on_error;
			}
			stat_info->st_ctime = value_32bit;

			if( libewf_file_entry_free(
			     &file_entry,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file entry.",
				 function );

				result = -EIO;

				goto on_error;
			}
			result = 0;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( path_length == ewfmount_fuse_path_length )
		{
			if( libcstring_narrow_string_compare(
			     path,
			     ewfmount_fuse_path,
			     ewfmount_fuse_path_length ) == 0 )
			{
				stat_info->st_mode  = S_IFREG | 0444;
				stat_info->st_nlink = 1;

				if( mount_handle_get_media_size(
				     ewfmount_mount_handle,
				     &file_size,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve media size.",
					 function );

					result = -EIO;

					goto on_error;
				}
#if SIZEOF_OFF_T == 4
				if( file_size > (size64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid media size value out of bounds.",
					 function );

					result = -ERANGE;

					goto on_error;
				}
#endif
				stat_info->st_size = (off_t) file_size;
#if defined( HAVE_TIME )
				if( time( &timestamp ) == (time_t) -1 )
				{
					timestamp = 0;
				}
				stat_info->st_atime = timestamp;
				stat_info->st_mtime = timestamp;
				stat_info->st_ctime = timestamp;
#else
				stat_info->st_atime = 0;
				stat_info->st_mtime = 0;
				stat_info->st_ctime = 0;
#endif
				result = 0;
			}
		}
	}
	if( result == 0 )
	{
#if defined( HAVE_GETEUID )
		stat_info->st_uid = geteuid();
#else
		stat_info->st_uid = 0;
#endif
#if defined( HAVE_GETEGID )
		stat_info->st_gid = getegid();
#else
		stat_info->st_gid = 0;
#endif
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( file_entry != NULL )
	{
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( result );
}

/* Cleans up when fuse is done
 */
void ewfmount_fuse_destroy(
      void *private_data LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "ewfmount_fuse_destroy";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( private_data )

	if( ewfmount_mount_handle != NULL )
	{
		if( mount_handle_free(
		     &ewfmount_mount_handle,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free mount handle.",
			 function );

			goto on_error;
		}
	}
	return;

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return;
}

#elif defined( HAVE_LIBDOKAN )

static wchar_t *ewfmount_dokan_path      = L"EWF1";
static size_t ewfmount_dokan_path_length = 4;

/* Opens a file or directory
 * Returns 0 if successful or a negative error code otherwise
 */
int ewfmount_dokan_CreateFile(
     const wchar_t *path,
     DWORD desired_access,
     DWORD share_mode,
     DWORD creation_disposition,
     DWORD attribute_flags LIBCSYSTEM_ATTRIBUTE_UNUSED,
     DOKAN_FILE_INFO *file_info )
{
	libcerror_error_t *error        = NULL;
	libewf_file_entry_t *file_entry = NULL;
	static char *function           = "ewfmount_dokan_CreateFile";
	size_t path_length              = 0;
	int result                      = 0;

	LIBCSYSTEM_UNREFERENCED_PARAMETER( attribute_flags )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( ( desired_access & GENERIC_WRITE ) != 0 )
	{
		return( -ERROR_WRITE_PROTECT );
	}
	if( ( share_mode != 0 )
	 && ( share_mode != FILE_SHARE_READ ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported share mode.",
		 function );

		result = -ERROR_SHARING_VIOLATION;

		goto on_error;
	}
	if( creation_disposition == CREATE_NEW )
	{
		return( -ERROR_FILE_EXISTS );
	}
	else if( creation_disposition == CREATE_ALWAYS )
	{
		return( -ERROR_ALREADY_EXISTS );
	}
	else if( creation_disposition == OPEN_ALWAYS )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition == TRUNCATE_EXISTING )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition != OPEN_EXISTING )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid createion disposition.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = libcstring_wide_string_length(
	               path );

	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( mount_handle_get_file_entry_by_path(
		     ewfmount_mount_handle,
		     path,
		     path_length,
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry for: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
/* TODO: if directory */
	file_info->IsDirectory = TRUE;

		if( libewf_file_entry_free(
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( path_length != ewfmount_dokan_path_length )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path length.",
			 function );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		if( libcstring_wide_string_compare(
		     path,
		     ewfmount_dokan_path,
		     ewfmount_dokan_path_length ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path.",
			 function );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns 0 if successful or a negative error code otherwise
 */
int ewfmount_dokan_ReadFile(
     const wchar_t *path,
     void *buffer,
     DWORD number_of_bytes_to_read,
     DWORD *number_of_bytes_read,
     LONGLONG offset,
     DOKAN_FILE_INFO *file_info LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error        = NULL;
	libewf_file_entry_t *file_entry = NULL;
	static char *function           = "ewfmount_dokan_ReadFile";
	size_t path_length              = 0;
	ssize_t read_count              = 0;
	int result                      = 0;

	LIBCSYSTEM_UNREFERENCED_PARAMETER( file_info )

/* TODO what about end of file behavior ? */

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_to_read > (DWORD) INT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of bytes to read value exceeds maximum.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_read == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of bytes read.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = libcstring_wide_string_length(
	               path );

	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( mount_handle_get_file_entry_by_path(
		     ewfmount_mount_handle,
		     path,
		     path_length,
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry for: %s.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		if( libewf_file_entry_seek_offset(
		     file_entry,
		     (off64_t) offset,
		     SEEK_SET,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in file entry.",
			 function );

			result = -ERROR_SEEK_ON_DEVICE;

			goto on_error;
		}
		read_count = libewf_file_entry_read_buffer(
			      file_entry,
			      (uint8_t *) buffer,
			      (size_t) number_of_bytes_to_read,
			      &error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from file entry.",
			 function );

			result = -ERROR_READ_FAULT;

			goto on_error;
		}
		if( libewf_file_entry_free(
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( path_length != ewfmount_fuse_path_length )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path length.",
			 function );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		if( libcstring_wide_string_compare(
		     path,
		     ewfmount_fuse_path,
		     ewfmount_fuse_path_length ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path.",
			 function );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		if( mount_handle_seek_offset(
		     ewfmount_mount_handle,
		     (off64_t) offset,
		     SEEK_SET,
		     &error ) == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in mount handle.",
			 function );

			result = -ERROR_SEEK_ON_DEVICE;

			goto on_error;
		}
		read_count = mount_handle_read_buffer(
			      ewfmount_mount_handle,
			      (uint8_t *) buffer,
			      (size_t) number_of_bytes_to_read,
			      &error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from mount handle.",
			 function );

			result = -ERROR_READ_FAULT;

			goto on_error;
		}
	}
	if( read_count > (size_t) INT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid read count value exceeds maximum.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	*number_of_bytes_read = (DWOWD) read_count;

	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a directory
 * Returns 0 if successful or a negative error code otherwise
 */
int ewfmount_dokan_FindFiles(
     const wchar_t *path,
     PFillFindData fill_find_data,
     DOKAN_FILE_INFO *file_info LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error            = NULL;
	libewf_file_entry_t *file_entry     = NULL;
	libewf_file_entry_t *sub_file_entry = NULL;
	char *name                          = NULL;
	static char *function               = "ewfmount_fuse_FindFiles";
	size_t name_index                   = 0;
	size_t name_size                    = 0;
	size_t path_length                  = 0;
	int number_of_sub_file_entries      = 0;
	int result                          = 0;
	int sub_file_entry_index            = 0;

	LIBCSYSTEM_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( mount_handle_get_file_entry_by_path(
		     ewfmount_mount_handle,
		     path,
		     path_length,
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry for: %s.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( ( path_length != 1 )
		 || ( path[ 0 ] != '/' ) )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path.",
			 function );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
	}

/* TODO */
	WIN32_FIND_DATAW
  DWORD    dwFileAttributes;
  FILETIME ftCreationTime;
  FILETIME ftLastAccessTime;
  FILETIME ftLastWriteTime;
  DWORD    nFileSizeHigh;
  DWORD    nFileSizeLow;
  DWORD    dwReserved0;
  DWORD    dwReserved1;
  TCHAR    cFileName[MAX_PATH];
  TCHAR    cAlternateFileName[14];


	if( fill_find_data(
	     buffer,
	     ".",
	     NULL,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( fill_find_data(
	     buffer,
	     "..",
	     NULL,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_FILES )
	{
		if( libewf_file_entry_get_number_of_sub_file_entries(
		     file_entry,
		     &number_of_sub_file_entries,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub file entries.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
		for( sub_file_entry_index = 0;
		     sub_file_entry_index < number_of_sub_file_entries;
		     sub_file_entry_index++ )
		{
			if( libewf_file_entry_get_sub_file_entry(
			     file_entry,
			     sub_file_entry_index,
			     &sub_file_entry,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to free retrieve sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libewf_file_entry_get_utf16_name_size(
				  sub_file_entry,
				  &name_size,
				  &error );
#else
			result = libewf_file_entry_get_utf8_name_size(
				  sub_file_entry,
				  &name_size,
				  &error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the sub file entry name size.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
			if( name_size > 0 )
			{
				name = libcstring_system_string_allocate(
					name_size );

				if( name == NULL )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create sub file entry name.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libewf_file_entry_get_utf16_name(
					  sub_file_entry,
					  (uint16_t *) name,
					  name_size,
					  &error );
#else
				result = libewf_file_entry_get_utf8_name(
					  sub_file_entry,
					  (uint8_t *) name,
					  name_size,
					  &error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the sub file entry name.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				if( fill_find_data(
				     buffer,
				     name,
				     NULL,
				     0 ) == 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set directory entry.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				memory_free(
				 name );

				name = NULL;
			}
			if( libewf_file_entry_free(
			     &sub_file_entry,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
		if( libewf_file_entry_free(
		     &file_entry,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entry.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	else if( ewfmount_mount_handle->input_format == MOUNT_HANDLE_INPUT_FORMAT_RAW )
	{
		if( fill_find_data(
		     buffer,
		     &( ewfmount_fuse_path[ 1 ] ),
		     NULL,
		     0 ) == 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set directory entry.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	if( sub_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( file_entry != NULL )
	{
		libewf_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( result );
}

int ewfmount_dokan_GetFileInformation(
     const wchar_t *path,
     BY_HANDLE_FILE_INFORMATION *file_information,
     DOKAN_FILE_INFO *file_info )
{
/* TODO */
}

#endif

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
#if defined( HAVE_GETRLIMIT )
	struct rlimit limit_data;
#endif
	libcstring_system_character_t * const *argv_filenames  = NULL;

	libewf_error_t *error                                  = NULL;
	libcstring_system_character_t *option_extended_options = NULL;
	libcstring_system_character_t *option_format           = NULL;
	libcstring_system_character_t *mount_point             = NULL;
	char *program                                          = "ewfmount";
	libcstring_system_integer_t option                     = 0;
	int number_of_filenames                                = 0;
	int result                                             = 0;
	int verbose                                            = 0;

#if !defined( LIBCSYSTEM_HAVE_GLOB )
	libcsystem_glob_t *glob                                = NULL;
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations ewfmount_fuse_operations;

	struct fuse_args ewfmount_fuse_arguments               = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *ewfmount_fuse_channel                = NULL;
	struct fuse *ewfmount_fuse_handle                      = NULL;

#elif defined( HAVE_LIBDOKAN )
	DOKAN_OPERATIONS ewfmount_dokan_operations;
	DOKAN_OPTIONS ewfmount_dokan_options;

#endif

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "ewftools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( libcsystem_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	ewfoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "f:hvVX:" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'f':
				option_format = optarg;

				break;

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				ewfoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'X':
				option_extended_options = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing EWF image file(s).\n" );


		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	if( ( optind + 1 ) == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ argc - 1 ];

	libcnotify_verbose_set(
	 verbose );

#if !defined( HAVE_LOCAL_LIBEWF )
	libewf_notify_set_verbose(
	 verbose );
	libewf_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if !defined( LIBCSYSTEM_HAVE_GLOB )
	if( libcsystem_glob_initialize(
	     &glob,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize glob.\n" );

		goto on_error;
	}
	if( libcsystem_glob_resolve(
	     glob,
	     &( argv[ optind ] ),
	     argc - optind - 1,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to resolve glob.\n" );

		goto on_error;
	}
	argv_filenames      = glob->result;
	number_of_filenames = glob->number_of_results;
#else
	argv_filenames      = &( argv[ optind ] );
	number_of_filenames = argc - optind - 1;
#endif

	if( mount_handle_initialize(
	     &ewfmount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_format != NULL )
	{
		result = mount_handle_set_format(
			  ewfmount_mount_handle,
			  option_format,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported input format defaulting to: raw.\n" );
		}
	}
#if defined( HAVE_GETRLIMIT )
	if( getrlimit(
            RLIMIT_NOFILE,
            &limit_data ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to determine limit: number of open file descriptors.\n" );
	}
	if( limit_data.rlim_max > (rlim_t) INT_MAX )
	{
		limit_data.rlim_max = (rlim_t) INT_MAX;
	}
	if( limit_data.rlim_max > 0 )
	{
		limit_data.rlim_max /= 2;
	}
	if( mount_handle_set_maximum_number_of_open_handles(
	     ewfmount_mount_handle,
	     (int) limit_data.rlim_max,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set maximum number of open file handles.\n" );

		goto on_error;
	}
#endif
	if( mount_handle_open_input(
	     ewfmount_mount_handle,
	     argv_filenames,
	     number_of_filenames,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open EWF file(s).\n" );

		goto on_error;
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( memory_set(
	     &ewfmount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	if( option_extended_options != NULL )
	{
		/* This argument is required but ignored
		 */
		if( fuse_opt_add_arg(
		     &ewfmount_fuse_arguments,
		     "" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &ewfmount_fuse_arguments,
		     "-o" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &ewfmount_fuse_arguments,
		     option_extended_options ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
	}
	ewfmount_fuse_operations.open    = &ewfmount_fuse_open;
	ewfmount_fuse_operations.read    = &ewfmount_fuse_read;
	ewfmount_fuse_operations.readdir = &ewfmount_fuse_readdir;
	ewfmount_fuse_operations.getattr = &ewfmount_fuse_getattr;
	ewfmount_fuse_operations.destroy = &ewfmount_fuse_destroy;

	ewfmount_fuse_channel = fuse_mount(
	                         mount_point,
	                         &ewfmount_fuse_arguments );

	if( ewfmount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	ewfmount_fuse_handle = fuse_new(
	                        ewfmount_fuse_channel,
	                        &ewfmount_fuse_arguments,
	                        &ewfmount_fuse_operations,
	                        sizeof( struct fuse_operations ),
	                        ewfmount_mount_handle );
	
	if( ewfmount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          ewfmount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 ewfmount_fuse_handle );

	fuse_opt_free_args(
	 &ewfmount_fuse_arguments );

	return( EXIT_SUCCESS );

#elif defined( HAVE_LIBDOKAN )
	if( memory_set(
	     &ewfmount_dokan_operations,
	     0,
	     sizeof( DOKAN_OPERATIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan operations.\n" );

		goto on_error;
	}
	if( memory_set(
	     &ewfmount_dokan_options,
	     0,
	     sizeof( DOKAN_OPTIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan options.\n" );

		goto on_error;
	}
	ewfmount_dokan_options.Version     = 600;
	ewfmount_dokan_options.ThreadCount = 1;

	DokanMain(
	 &ewfmount_dokan_options,
	 &ewfmount_dokan_operations );

#else
	fprintf(
	 stderr,
	 "No sub system to mount EWF format.\n" );

	return( EXIT_FAILURE );
#endif

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( ewfmount_fuse_handle != NULL )
	{
		fuse_destroy(
		 ewfmount_fuse_handle );
	}
	fuse_opt_free_args(
	 &ewfmount_fuse_arguments );
#endif
	if( ewfmount_mount_handle != NULL )
	{
		mount_handle_free(
		 &ewfmount_mount_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}


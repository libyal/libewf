/* 
 * Device handle
 *
 * Copyright (C) 2007-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#include <liberror.h>

/* If libtool DLL support is enabled set LIBEWF_DLL_IMPORT
 * before including libewf.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBEWF_DLL_IMPORT
#endif

#include <libewf.h>

#if defined( HAVE_LOCAL_LIBSMDEV )
#include <libsmdev_definitions.h>
#include <libsmdev_handle.h>
#include <libsmdev_metadata.h>
#include <libsmdev_support.h>
#include <libsmdev_types.h>
#elif defined( HAVE_LIBSMDEV_H )
#include <libsmdev.h>
#endif

#include <libsystem.h>

#include "byte_size_string.h"
#include "device_handle.h"
#include "io_ata.h"
#include "io_bus.h"
#include "io_optical_disk.h"
#include "io_scsi.h"
#include "io_usb.h"
#include "storage_media_buffer.h"

/* Initializes the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_initialize(
     device_handle_t **device_handle,
     liberror_error_t **error )
{
	static char *function = "device_handle_initialize";

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( *device_handle == NULL )
	{
		*device_handle = (device_handle_t *) memory_allocate(
		                                      sizeof( device_handle_t ) );

		if( *device_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create device handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *device_handle,
		     0,
		     sizeof( device_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear device handle.",
			 function );

			memory_free(
			 *device_handle );

			*device_handle = NULL;

			return( -1 );
		}
#if defined( WINAPI )
		( *device_handle )->file_handle     = INVALID_HANDLE_VALUE;
#else
		( *device_handle )->file_descriptor = -1;
#endif
	}
	return( 1 );
}

/* Frees the device handle and its elements
 * Returns 1 if successful or -1 on error
 */
int device_handle_free(
     device_handle_t **device_handle,
     liberror_error_t **error )
{
	static char *function = "device_handle_free";
	int result            = 1;

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( *device_handle != NULL )
	{
		if( ( *device_handle )->type == DEVICE_HANDLE_TYPE_DEVICE )
		{
			if( ( *device_handle )->device_handle == NULL )
			{
				if( libsmdev_handle_free(
				     &( ( *device_handle )->device_handle ),
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free device handle.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 *device_handle );

		*device_handle = NULL;
	}
	return( result );
}

/* Opens the input of the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_open_input(
     device_handle_t *device_handle,
     const libsystem_character_t *filename,
     liberror_error_t **error )
{
	libsystem_character_t *filenames[ 1 ] = { NULL };

#if defined( WINAPI )
	BY_HANDLE_FILE_INFORMATION file_information;

	PVOID error_string                    = NULL;
	LARGE_INTEGER large_integer_size      = DEVICE_HANDLE_LARGE_INTEGER_ZERO;
	DWORD dword_size                      = 0;
	DWORD error_code                      = 0;
	DWORD file_type                       = 0;
	DWORD windows_version                 = 0;
#else
	struct stat file_stat;
#endif

	static char *function                 = "device_handle_open_input";
	size64_t file_size                    = 0;
	int result                            = 0;

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	result = libsmdev_check_device(
	          filename,
                  error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if filename is a device.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( device_handle->device_handle != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid device handle - device handle already set.",
			 function );

			return( -1 );
		}
		device_handle->type = DEVICE_HANDLE_TYPE_DEVICE;

		filenames[ 0 ] = (libsystem_character_t *) filename;

		if( libsmdev_handle_initialize(
		     &( device_handle->device_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create device handle.",
			 function );

			return( -1 );
		}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
		if( libsmdev_handle_open_wide(
		     device_handle->device_handle,
		     (wchar_t * const *) filenames,
		     1,
		     LIBSMDEV_OPEN_READ,
		     error ) != 1 )
#else
		if( libsmdev_handle_open(
		     device_handle->device_handle,
		     (char * const *) filenames,
		     1,
		     LIBSMDEV_OPEN_READ,
		     error ) != 1 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open device handle.",
			 function );

			return( -1 );
		}
	}
	else
	{
#if defined( WINAPI )
		if( device_handle->file_handle != INVALID_HANDLE_VALUE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid device handle - file handle already set.",
			 function );

			return( -1 );
		}
		device_handle->file_handle = CreateFile(
					      (LPCTSTR) filename,
					      GENERIC_READ,
					      FILE_SHARE_READ,
					      NULL,
					      OPEN_EXISTING,
					      FILE_ATTRIBUTE_NORMAL,
					      NULL );

		if( device_handle->file_handle == INVALID_HANDLE_VALUE )
		{
			error_code = GetLastError();

			if( FormatMessage(
			     FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			     NULL,
			     error_code,
			     MAKELANGID(
			      LANG_NEUTRAL,
			      SUBLANG_DEFAULT ),
			     (LPTSTR) &error_string,
			     0,
			     NULL ) != 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open file: %" PRIs_LIBSYSTEM " with error: %" PRIs_LIBSYSTEM "",
				 function,
				 filename,
				 error_string );

				LocalFree(
				 error_string );
			}
			else
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_OPEN_FAILED,
				 "%s: unable to open file: %" PRIs_LIBSYSTEM ".",
				 function,
				 filename );
			}
			return( -1 );
		}
		/* Use the GetFileType function to rule out certain file types
		 * like pipes, sockets, etc.
		 */
		file_type = GetFileType(
			     device_handle->file_handle );

		if( file_type != FILE_TYPE_DISK )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported file type.",
			 function );

			return( -1 );
		}
		if( ( filename[ 0 ] == '\\' )
		 && ( filename[ 1 ] == '\\' )
		 && ( filename[ 2 ] == '.' )
		 && ( filename[ 3 ] == '\\' ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: filename is a device.",
			 function );

			return( -1 );
		}
		else
		{
			/* This function fails on a device
			 */
			if( GetFileInformationByHandle(
			     device_handle->file_handle,
			     &file_information ) == 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_GENERIC,
				 "%s: unable to retrieve file information.",
				 function );

				return( -1 );
			}
			if( file_information.dwFileAttributes == INVALID_FILE_ATTRIBUTES )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid file attributes returned.",
				 function );

				return( -1 );
			}
			if( ( file_information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: filename is a directory.",
				 function );

				return( -1 );
			}
			device_handle->type = DEVICE_HANDLE_TYPE_FILE;

			windows_version = GetVersion();

			if( windows_version >= 0x80000000 )
			{
				if( GetFileSize(
				     device_handle->file_handle,
				     &dword_size ) == 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine file size.",
					 function );

					return( -1 );
				}
				file_size = (size64_t) dword_size;
			}
			else
			{
				if( GetFileSizeEx(
				     device_handle->file_handle,
				     &large_integer_size ) == 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine file size.",
					 function );

					return( -1 );
				}
				file_size = ( (size64_t) large_integer_size.HighPart << 32 ) + large_integer_size.LowPart;
			}
		}
#else
		if( device_handle->file_descriptor != -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid device handle - file descriptor already set.",
			 function );

			return( -1 );
		}
		device_handle->file_descriptor = open(
						  filename,
						  O_RDONLY );

		if( device_handle->file_descriptor == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file: %" PRIs_LIBSYSTEM ".",
			 function,
			 filename );

			return( -1 );
		}
#if defined( HAVE_POSIX_FADVISE )
		/* Use this function to double the read-ahead system buffer
		 * This provides for some additional performance
		 */
		if( posix_fadvise(
		     device_handle->file_descriptor,
		     0,
		     0,
		     POSIX_FADV_SEQUENTIAL ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_GENERIC,
			 "%s: unable to advice file handle.",
			 function );

			return( -1 );
		}
#endif
		if( fstat(
		     device_handle->file_descriptor,
		     &file_stat ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine file status information.",
			 function );

			return( -1 );
		}
		device_handle->type = DEVICE_HANDLE_TYPE_FILE;

		if( S_ISDIR( file_stat.st_mode ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: filename is a directory.",
			 function );

			return( -1 );
		}
		if( S_ISBLK( file_stat.st_mode )
		 || S_ISCHR( file_stat.st_mode ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: filename is a device.",
			 function );

			return( -1 );
		}
		file_size = file_stat.st_size;
#endif

		if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
		{
			device_handle->media_size           = file_size;
			device_handle->media_size_set       = 1;
			device_handle->bytes_per_sector     = 512;
			device_handle->bytes_per_sector_set = 1;
		}
	}
	return( 1 );
}

/* Closes the device handle
 * Returns the 0 if succesful or -1 on error
 */
int device_handle_close(
     device_handle_t *device_handle,
     liberror_error_t **error )
{
	static char *function = "device_handle_close";

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_close(
		     device_handle->device_handle,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close device handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
#if defined( WINAPI )
		if( device_handle->file_handle == INVALID_HANDLE_VALUE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid device handle - missing file handle.",
			 function );

			return( -1 );
		}
		if( CloseHandle(
		     device_handle->file_handle ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file handle.",
			 function );

			return( -1 );
		}
#else
		if( device_handle->file_descriptor == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid device handle - missing file descriptor.",
			 function );

			return( -1 );
		}
		if( close(
		     device_handle->file_descriptor ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file descriptor.",
			 function );

			return( -1 );
		}
#endif
	}
	return( 0 );
}

/* Reads a buffer from the input of the device handle
 * Returns the amount of bytes written or -1 on error
 */
ssize_t device_handle_read_buffer(
         device_handle_t *device_handle,
         uint8_t *buffer,
         size_t read_size,
         liberror_error_t **error )
{
	static char *function = "device_handle_read_buffer";
	ssize_t read_count    = 0;

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		read_count = libsmdev_handle_read_buffer(
			      device_handle->device_handle,
			      buffer,
			      read_size,
		              error );

		if( read_count < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
#if defined( WINAPI )
		if( device_handle->file_handle == INVALID_HANDLE_VALUE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid device handle - missing file handle.",
			 function );

			return( -1 );
		}
#else
		if( device_handle->file_descriptor == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid device handle - missing file descriptor.",
			 function );

			return( -1 );
		}
#endif

#if defined( WINAPI )
		if( ReadFile(
		     device_handle->file_handle,
		     buffer,
		     read_size,
		     (LPDWORD) &read_count,
		     NULL ) == 0 )
#else
		read_count = read(
			      device_handle->file_descriptor,
			      buffer,
			      read_size );

		if( read_count < 0 )
#endif
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer.",
			 function );

			return( -1 );
		}
	}
	return( read_count );
}

/* Seeks the offset in the input file
 * Returns the new offset if successful or -1 on error
 */
off64_t device_handle_seek_offset(
         device_handle_t *device_handle,
         off64_t offset,
         int whence,
         liberror_error_t **error )
{
	static char *function              = "device_handle_seek_offset";

#if defined( WINAPI )
	LARGE_INTEGER large_integer_offset = DEVICE_HANDLE_LARGE_INTEGER_ZERO;
	DWORD move_method                  = 0;
#endif

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		offset = libsmdev_handle_seek_offset(
		          device_handle->device_handle,
		          offset,
		          whence,
		          error );

		if( offset < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in device handle.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
#if defined( WINAPI )
		if( device_handle->file_handle == INVALID_HANDLE_VALUE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid device handle - missing file handle.",
			 function );

			return( -1 );
		}
#else
		if( device_handle->file_descriptor == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid device handle - missing file descriptor.",
			 function );

			return( -1 );
		}
#endif
		if( ( whence != SEEK_CUR )
		 && ( whence != SEEK_END )
		 && ( whence != SEEK_SET ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported whence.",
			 function );

			return( -1 );
		}
#if defined( WINAPI )
		if( whence == SEEK_SET )
		{
			move_method = FILE_BEGIN;
		}
		else if( whence == SEEK_CUR )
		{
			move_method = FILE_CURRENT;
		}
		else if( whence == SEEK_END )
		{
			move_method = FILE_END;
		}
		large_integer_offset.LowPart  = (DWORD) ( 0x0ffffffff & offset );
		large_integer_offset.HighPart = (LONG) ( offset >> 32 );

		if( SetFilePointerEx(
		     device_handle->file_handle,
		     large_integer_offset,
		     NULL,
		     move_method ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: %" PRIi64 " in input handle.",
			 function,
			 offset );

			return( -1 );
		}
		offset = ( (off64_t) large_integer_offset.HighPart << 32 ) + large_integer_offset.LowPart;

		if( offset < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: invalid offset: %" PRIi64 " returned.",
			 function,
			 offset );

			return( -1 );
		}
#else
		offset = lseek(
			  device_handle->file_descriptor,
			  offset,
			  whence );

		if( offset < 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in input handle.",
			 function );

			return( -1 );
		}
#endif
	}
	return( offset );
}

/* Retrieves the media size
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_media_size(
     device_handle_t *device_handle,
     size64_t *media_size,
     liberror_error_t **error )
{
	static char *function = "device_handle_get_media_size";

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_media_size(
		     device_handle->device_handle,
		     media_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media size.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( media_size == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid media size.",
			 function );

			return( -1 );
		}
		*media_size = device_handle->media_size;
	}
	return( 1 );
}

/* Retrieves the media type
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_media_type(
     device_handle_t *device_handle,
     uint8_t *media_type,
     liberror_error_t **error )
{
	static char *function = "device_handle_get_media_type";

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_media_type(
		     device_handle->device_handle,
		     media_type,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media type.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( media_type == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid media type.",
			 function );

			return( -1 );
		}
		*media_type = 0;
	}
	return( 1 );
}

/* Retrieves the amount of bytes per sector
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_bytes_per_sector(
     device_handle_t *device_handle,
     uint32_t *bytes_per_sector,
     liberror_error_t **error )
{
	static char *function = "device_handle_get_bytes_per_sector";

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_bytes_per_sector(
		     device_handle->device_handle,
		     bytes_per_sector,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bytes per sector.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		if( bytes_per_sector == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid bytes per sector.",
			 function );

			return( -1 );
		}
		*bytes_per_sector = device_handle->bytes_per_sector;
	}
	return( 1 );
}

/* Retrieves the media information value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int device_handle_get_media_information_value(
     device_handle_t *device_handle,
     char *media_information_value_identifier,
     size_t media_information_value_identifier_length,
     libsystem_character_t *media_information_value,
     size_t media_information_value_size,
     liberror_error_t **error )
{
	static char *function = "device_handle_get_media_information_value";

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		if( libsmdev_handle_get_media_information_value(
		     device_handle->device_handle,
		     (uint8_t *) media_information_value_identifier,
		     media_information_value_identifier_length,
		     media_information_value,
		     media_information_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media information value.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		return( 0 );
	}
	return( 1 );
}

/* Print the media information to a stream
 * Returns 1 if successful or -1 on error
 */
int device_handle_media_information_fprint(
     device_handle_t *device_handle,
     FILE *stream,
     liberror_error_t **error )
{
	uint8_t media_information_value[ 64 ];

        libsystem_character_t media_size_string[ 16 ];

	static char *function = "device_handle_media_information_fprint";
	size64_t media_size   = 0;
	uint8_t device_type   = 0;
	uint8_t bus_type      = 0;
	int result            = 0;

	if( device_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( device_handle->type == DEVICE_HANDLE_TYPE_DEVICE )
	{
		fprintf(
		 stream,
		 "Media information:\n" );

#ifdef TODO
		if( libsmdev_handle_get_device_type(
		     device_handle->device_handle,
		     device_type,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve device type.",
			 function );

			return( -1 );
		}
		fprintf(
		 stream,
		 "Device type:\t\t" );

		switch( device_handle->device_type )
		{
			case 0x00:
				fprintf(
				 stream,
				 "Direct access" );
				break;

			case 0x01:
				fprintf(
				 stream,
				 "Sequential access" );
				break;

			case 0x02:
				fprintf(
				 stream,
				 "Printer" );
				break;

			case 0x03:
				fprintf(
				 stream,
				 "Processor" );
				break;

			case 0x04:
				fprintf(
				 stream,
				 "Write-once" );
				break;

			case 0x05:
				fprintf(
				 stream,
				 "Optical disk (CD/DVD/BD)" );
				break;

			case 0x06:
				fprintf(
				 stream,
				 "Scanner" );
				break;

			case 0x07:
				fprintf(
				 stream,
				 "Optical memory" );
				break;

			case 0x08:
				fprintf(
				 stream,
				 "Medium changer" );
				break;

			case 0x09:
				fprintf(
				 stream,
				 "Communications" );
				break;

			case 0x0a:
			case 0x0b:
				fprintf(
				 stream,
				 "Graphic arts pre-press" );
				break;

			case 0x0c:
				fprintf(
				 stream,
				 "Storage array controller" );
				break;

			case 0x0d:
				fprintf(
				 stream,
				 "Enclosure services" );
				break;

			case 0x0e:
				fprintf(
				 stream,
				 "Simplified direct-access" );
				break;

			case 0x0f:
				fprintf(
				 stream,
				 "Optical card reader/writer" );
				break;

			case 0x10:
				fprintf(
				 stream,
				 "Bridging expander" );
				break;

			case 0x11:
				fprintf(
				 stream,
				 "Object-based Storage" );
				break;

			case 0x12:
				fprintf(
				 stream,
				 "Automation/Drive Interface" );
				break;

			case 0x13:
			case 0x14:
			case 0x15:
			case 0x16:
			case 0x17:
			case 0x18:
			case 0x1a:
			case 0x1b:
			case 0x1c:
			case 0x1d:
				fprintf(
				 stream,
				 "Reserved: %d",
				 device_handle->device_type );
				break;

			case 0x1e:
				fprintf(
				 stream,
				 "Well known logical unit" );
				break;

			default:
				fprintf(
				 stream,
				 "Unknown: %d",
				 device_handle->device_type );
				break;
		}
		fprintf(
		 stream,
		 "\n" );

		if( libsmdev_handle_get_bus_type(
		     device_handle->device_handle,
		     &bus_type,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bus type.",
			 function );

			return( -1 );
		}
#endif
		fprintf(
		 stream,
		 "Bus type:\t\t" );

		switch( bus_type )
		{
			case LIBSMDEV_BUS_TYPE_ATA:
				fprintf(
				 stream,
				 "ATA/ATAPI" );
				break;

			case LIBSMDEV_BUS_TYPE_FIREWIRE:
				fprintf(
				 stream,
				 "FireWire (IEEE1394)" );
				break;

			case LIBSMDEV_BUS_TYPE_SCSI:
				fprintf(
				 stream,
				 "SCSI" );
				break;

			case LIBSMDEV_BUS_TYPE_USB:
				fprintf(
				 stream,
				 "USB" );
				break;
		}
		fprintf(
		 stream,
		 "\n" );

#ifdef TODO
		if( device_handle->removable != 0 )
		{
			fprintf(
			 stream,
			 "Removable:\t\tyes\n" );
		}
#endif
		result = libsmdev_handle_get_media_information_value(
		          device_handle->device_handle,
		          (uint8_t *) "vendor",
		          6,
		          media_information_value,
		          64,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media information value: vendor.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			media_information_value[ 0 ] = 0;
		}
		fprintf(
		 stream,
		 "Vendor:\t\t\t%s\n",
		 (char *) media_information_value );

		result = libsmdev_handle_get_media_information_value(
		          device_handle->device_handle,
		          (uint8_t *) "model",
		          5,
		          media_information_value,
		          64,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media information value: model.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			media_information_value[ 0 ] = 0;
		}
		fprintf(
		 stream,
		 "Model:\t\t\t%s\n",
		 (char *) media_information_value );

		result = libsmdev_handle_get_media_information_value(
		          device_handle->device_handle,
		          (uint8_t *) "serial_number",
		          13,
		          media_information_value,
		          64,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media information value: serial_number.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			media_information_value[ 0 ] = 0;
		}
		fprintf(
		 stream,
		 "Serial:\t\t\t%s\n",
		 (char *) media_information_value );

		if( libsmdev_handle_get_media_size(
		     device_handle->device_handle,
		     &media_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media size.",
			 function );

			return( -1 );
		}
	}
	else if( device_handle->type == DEVICE_HANDLE_TYPE_FILE )
	{
		media_size = device_handle->media_size;
	}
	result = byte_size_string_create(
		  media_size_string,
		  16,
		  media_size,
		  BYTE_SIZE_STRING_UNIT_MEGABYTE,
		  NULL );

	if( result == 1 )
	{
		fprintf(
		 stream,
		 "Media size:\t\t%" PRIs_LIBSYSTEM " (%" PRIu64 " bytes)\n",
		 media_size_string,
		 media_size );
	}
	else
	{
		fprintf(
		 stream,
		 "Media size:\t\t%" PRIu64 " bytes\n",
		 media_size );
	}
	fprintf(
	 stream,
	 "\n" );

	return( 1 );
}


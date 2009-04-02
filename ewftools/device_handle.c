/* 
 * Device handle
 *
 * Copyright (C) 2007-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_CYGWIN_FS_H )
#include <cygwin/fs.h>
#endif

#if defined( HAVE_LINUX_FS_H )
/* Required for Linux platforms that use a sizeof( u64 )
 * in linux/fs.h but have no typedef of it
 */
#if ! defined( HAVE_U64 )
typedef size_t u64;
#endif

#include <linux/fs.h>
#endif

#if defined( HAVE_SYS_DISK_H )
#include <sys/disk.h>
#endif

#if defined( HAVE_SYS_DISKLABEL_H )
#include <sys/disklabel.h>
#endif

#if defined( WINAPI )
#include <winbase.h>
#endif

#include "device_handle.h"
#include "notify.h"
#include "storage_media_buffer.h"
#include "system_string.h"

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
		memory_free(
		 *device_handle );

		*device_handle = NULL;
	}
	return( 1 );
}

/* Opens the input of the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_open_input(
     device_handle_t *device_handle,
     const system_character_t *filename,
     liberror_error_t **error )
{
#if defined( WINAPI )
	BY_HANDLE_FILE_INFORMATION file_information;
#endif

	static char *function            = "device_handle_open_input";
	size64_t file_size               = 0;

#if defined( WINAPI )
	PVOID error_string               = NULL;
	LARGE_INTEGER large_integer_size = { 0, 0 };
	DWORD error_code                 = 0;
	DWORD file_type                  = 0;
#else
	struct stat file_stat;
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
#endif
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
#if defined( WINAPI )
	device_handle->file_handle = CreateFile(
	                              (LPCTSTR) filename,
	                              GENERIC_READ,
	                              0,
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
			 "%s: unable to open file or device: %" PRIs_SYSTEM " with error: %" PRIs_SYSTEM "",
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
			 "%s: unable to open file or device: %" PRIs_SYSTEM ".",
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
		device_handle->type = DEVICE_HANDLE_TYPE_DEVICE;
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
			 "%s: file or device is a directory.",
			 function );

			return( -1 );
		}
		device_handle->type = DEVICE_HANDLE_TYPE_FILE;

		if( GetFileSizeEx(
		     device_handle->file_handle,
		     &large_integer_size ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine file or device size.",
			 function );

			return( -1 );
		}
		file_size = ( (size64_t) large_integer_size.HighPart << 32 ) + large_integer_size.LowPart;
	}
#else
	/* TODO refactor */
	device_handle->file_descriptor = system_string_open(
	                                  filename,
	                                  FILE_IO_O_RDONLY );

	if( device_handle->file_descriptor == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file or device: %" PRIs_SYSTEM ".",
		 function,
		 filename );

		return( -1 );
	}
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
		 "%s: file or device is a directory.",
		 function );

		return( -1 );
	}
	if( S_ISBLK( file_stat.st_mode )
	 || S_ISCHR( file_stat.st_mode ) )
	{
		device_handle->type = DEVICE_HANDLE_TYPE_DEVICE;
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
	return( 1 );
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
	LARGE_INTEGER large_integer_offset = { 0, 0 };
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
	lseek(
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
	return( offset );
}

/* Reads a buffer from the input of the device handle
 * Returns the amount of bytes written or -1 on error
 */
#ifdef TODO
ssize_t device_handle_read_buffer(
         device_handle_t *device_handle,
         storage_media_buffer_t *storage_media_buffer,
         size_t read_size,
         liberror_error_t **error )
#else
ssize_t device_handle_read_buffer(
         device_handle_t *device_handle,
         uint8_t *buffer,
         size_t read_size,
         liberror_error_t **error )
#endif
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

#ifdef TODO
	if( storage_media_buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid storage media buffer.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( ReadFile(
	     device_handle->file_handle,
	     storage_media_buffer->raw_buffer,
	     read_size,
	     (LPDWORD) &read_count,
	     NULL ) == 0 )
#else
	read_count = file_io_read(
	              device_handle->file_descriptor,
	              storage_media_buffer->raw_buffer,
	              read_size );

	if( read_count < 0 )
#endif
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read storage media buffer.",
		 function );

		return( -1 );
	}
	storage_media_buffer->raw_buffer_amount = read_count;
#else
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
#if defined( WINAPI )
	if( ReadFile(
	     device_handle->file_handle,
	     buffer,
	     read_size,
	     (LPDWORD) &read_count,
	     NULL ) == 0 )
#else
	read_count = file_io_read(
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
#endif

	return( read_count );
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
	return( 0 );
}

/* Retrieves the media size
 * Returns 1 if successful or -1 on error
 */
int device_handle_get_media_size(
     device_handle_t *device_handle,
     size64_t *media_size,
     liberror_error_t **error )
{
#if defined( WINAPI )
	GET_LENGTH_INFORMATION length_information;

	DWORD result_count    = 0;
#else
#if !defined( DIOCGMEDIASIZE ) && defined( DIOCGDINFO )
	struct disklabel disk_label;
#endif
#if defined( DKIOCGETBLOCKCOUNT )
	uint64_t block_count  = 0;
#endif
#endif
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
	if( device_handle->media_size_set == 0 )
	{
#if defined( WINAPI )
		if( DeviceIoControl(
		     device_handle->file_handle,
		     IOCTL_DISK_GET_LENGTH_INFO,
		     NULL,
		     0,
		     &length_information,
		     sizeof( GET_LENGTH_INFORMATION ),
		     &result_count,
		     NULL ) == 0 )
		{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_IOCTL_FAILED,
				 "%s: unable to query device for: IOCTL_DISK_GET_LENGTH_INFO.",
				 function );

				return( -1 );
		}
		device_handle->media_size     = ( (size64_t) length_information.Length.HighPart << 32 ) + length_information.Length.LowPart;
		device_handle->media_size_set = 1;

#elif defined( BLKGETSIZE64 )
		if( ioctl(
		     device_handle->file_descriptor,
		     BLKGETSIZE64,
		     &( device_handle->media_size ) ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: BLKGETSIZE64.",
			 function );

			return( -1 );
		}
		device_handle->media_size_set = 1;

#elif defined( DIOCGMEDIASIZE )
		if( ioctl(
		     device_handle->file_descriptor,
		     DIOCGMEDIASIZE,
		     &( device_handle->media_size ) ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: DIOCGMEDIASIZE.",
			 function );

			return( -1 );
		}
		device_handle->media_size_set = 1;

#elif defined( DIOCGDINFO )
		if( ioctl(
		     device_handle->file_descriptor,
		     DIOCGDINFO,
		     &disk_label ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: DIOCGDINFO.",
			 function );

			return( -1 );
		}
		device_handle->media_size     = disk_label.d_secperunit * disk_label.d_secsize;
		device_handle->media_size_set = 1;

#elif defined( DKIOCGETBLOCKCOUNT )
		if( device_handle->bytes_per_sector_set == 0 )
		{
			if( ioctl(
			     device_handle->file_descriptor,
			     DKIOCGETBLOCKSIZE,
			     &( device_handle->bytes_per_sector ) ) == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_IOCTL_FAILED,
				 "%s: unable to query device for: DKIOCGETBLOCKSIZE.",
				 function );

				return( -1 );
			}
			device_handle->bytes_per_sector_set = 1;
		}
		if( ioctl(
		     device_handle->file_descriptor,
		     DKIOCGETBLOCKCOUNT,
		     &block_count ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: DKIOCGETBLOCKCOUNT.",
			 function );

			return( -1 );
		}
		device_handle->media_size     = (size64_t) block_count * (size64_t) device_handle->bytes_per_sector;
		device_handle->media_size_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
		notify_verbose_printf(
		 "%s: block size: %" PRIu32 " block count: %" PRIu64 " ",
		 function,
		 device_handle->bytes_per_sector,
		 block_count );
#endif
#endif
	}
	if( device_handle->media_size_set == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported platform.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_verbose_printf(
	 "%s: device size: %" PRIu64 "\n",
	 function,
	 device_handle->media_size );
#endif

	*media_size = device_handle->media_size;

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
#if defined( WINAPI )
	DISK_GEOMETRY_EX disk_geometry;

	DWORD result_count    = 0;
#endif
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
	if( device_handle->bytes_per_sector_set == 0 )
	{
#if defined( WINAPI )
		if( DeviceIoControl(
		     device_handle->file_handle,
		     IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
		     NULL,
		     0,
		     &disk_geometry,
		     sizeof( DISK_GEOMETRY_EX ),
		     &result_count,
		     NULL ) == 0 )
		{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_IOCTL_FAILED,
				 "%s: unable to query device for: IOCTL_DISK_GET_DRIVE_GEOMETRY_EX.",
				 function );

				return( -1 );
		}
		device_handle->bytes_per_sector     = (uint32_t) disk_geometry.Geometry.BytesPerSector; 
		device_handle->bytes_per_sector_set = 1;

#elif defined( BLKSSZGET )
		if( ioctl(
		     device_handle->file_descriptor,
		     BLKSSZGET,
		     &( device_handle->bytes_per_sector ) ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: BLKSSZGET.",
			 function );

			return( -1 );
		}
		device_handle->bytes_per_sector_set = 1;

#elif defined( DKIOCGETBLOCKCOUNT )
		if( ioctl(
		     device_handle->file_descriptor,
		     DKIOCGETBLOCKSIZE,
		     &( device_handle->bytes_per_sector ) ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: DKIOCGETBLOCKSIZE.",
			 function );

			return( -1 );
		}
		device_handle->bytes_per_sector_set = 1;
#endif
	}
	if( device_handle->bytes_per_sector_set == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported platform.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	notify_verbose_printf(
	 "%s: sector size: %" PRIu32 "\n",
	 function,
	 device_handle->bytes_per_sector );
#endif

	*bytes_per_sector = device_handle->bytes_per_sector;

	return( 1 );
}


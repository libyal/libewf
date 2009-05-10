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

#if defined( WINAPI )
#include <windows.h>

#elif defined( HAVE_CYGWIN_FS_H )
#include <cygwin/fs.h>

#elif defined( HAVE_LINUX_FS_H )
/* Required for Linux platforms that use a sizeof( u64 )
 * in linux/fs.h but have no typedef of it
 */
#if !defined( HAVE_U64 )
typedef size_t u64;
#endif

#include <linux/fs.h>

#else

#if defined( HAVE_SYS_DISK_H )
#include <sys/disk.h>
#endif

#if defined( HAVE_SYS_DISKLABEL_H )
#include <sys/disklabel.h>
#endif

#endif

#include "byte_size_string.h"
#include "device_handle.h"
#include "io_ata.h"
#include "io_bus.h"
#include "io_optical_disk.h"
#include "io_scsi.h"
#include "io_usb.h"
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
	device_handle->file_descriptor = open(
	                                  filename,
	                                  O_RDONLY );

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
	read_count = read(
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
#endif

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
#if defined( WINAPI )
	GET_LENGTH_INFORMATION length_information;

	DWORD response_count  = 0;
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
		     &response_count,
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

	DWORD response_count  = 0;
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
		     &response_count,
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

/* Determines the media information
 * Returns 1 if successful, 0 if no media information available or -1 on error
 */
int device_handle_determine_media_information(
     device_handle_t *device_handle,
     liberror_error_t **error )
{
#if defined( WINAPI )
	STORAGE_PROPERTY_QUERY query;

	uint8_t *response      = NULL;
	size_t response_size   = 1024;
	size_t string_length   = 0;
	DWORD response_count   = 0;

#else
#if defined( HAVE_IO_SCSI )
	uint8_t response[ 255 ];
	ssize_t response_count = 0;
#endif
#if defined( HAVE_IO_ATA )
	struct hd_driveid device_configuration;
#endif
#endif

	static char *function  = "device_handle_determine_media_information";
	int result             = 0;

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
	if( device_handle->type != DEVICE_HANDLE_TYPE_DEVICE )
	{
		return( 0 );
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
#if defined( WINAPI )
	if( device_handle->media_information_set == 0 )
	{
		if( memset(
		     &query,
		     0,
		     sizeof( STORAGE_PROPERTY_QUERY ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear storage property query.",
			 function );

			return( -1 );
		}
		query.PropertyId = StorageDeviceProperty;
		query.QueryType  = PropertyStandardQuery;

		response         = (uint8_t *) memory_allocate(
						response_size );

		if( response == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to response.",
			 function );

			return( -1 );
		}
		if( DeviceIoControl(
		     device_handle->file_handle,
		     IOCTL_STORAGE_QUERY_PROPERTY,
		     &query,
		     sizeof( STORAGE_PROPERTY_QUERY ),
		     response,
		     response_size,
		     &response_count,
		     NULL ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: IOCTL_STORAGE_QUERY_PROPERTY.",
			 function );

			memory_free(
			 response );

			return( -1 );
		}
		if( (size_t) ( (STORAGE_DESCRIPTOR_HEADER *) response )->Size > response_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: response buffer too small.\n",
			 function );

			memory_free(
			 response );

			return( -1 );
		}
		if( (size_t) ( (STORAGE_DESCRIPTOR_HEADER *) response )->Size > sizeof( STORAGE_DEVICE_DESCRIPTOR ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			notify_verbose_dump_data(
			 response,
			 (size_t) response_count );
#endif

			if( ( (STORAGE_DEVICE_DESCRIPTOR *) response )->VendorIdOffset > 0 )
			{
				string_length = narrow_string_length(
				                 (char *) &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->VendorIdOffset ] ) );

				result = system_string_trim_copy_from_byte_stream(
					  device_handle->vendor,
					  64,
					  &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->VendorIdOffset ] ),
					  string_length,
					  error );

				if( result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set vendor.",
					 function );

					return( -1 );
				}
				else if( result == 0 )
				{
					device_handle->vendor[ 0 ] = 0;
				}
			}
			if( ( (STORAGE_DEVICE_DESCRIPTOR *) response )->ProductIdOffset > 0 )
			{
				string_length = narrow_string_length(
				                 (char *) &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->ProductIdOffset ] ) );

				result = system_string_trim_copy_from_byte_stream(
					  device_handle->model,
					  64,
					  &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->ProductIdOffset ] ),
					  string_length,
					  error );

				if( result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set model.",
					 function );

					return( -1 );
				}
				else if( result == 0 )
				{
					device_handle->model[ 0 ] = 0;
				}
			}
			if( ( (STORAGE_DEVICE_DESCRIPTOR *) response )->SerialNumberOffset > 0 )
			{
				string_length = narrow_string_length(
				                 (char *) &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->SerialNumberOffset ] ) );

				result = system_string_trim_copy_from_byte_stream(
					  device_handle->serial_number,
					  64,
					  &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->SerialNumberOffset ] ),
					  string_length,
					  error );

				if( result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set serial number.",
					 function );

					return( -1 );
				}
				else if( result == 0 )
				{
					device_handle->serial_number[ 0 ] = 0;
				}
			}
			device_handle->removable             = ( (STORAGE_DEVICE_DESCRIPTOR *) response )->RemovableMedia;
			device_handle->media_information_set = 1;

			switch( ( ( STORAGE_DEVICE_DESCRIPTOR *) response )->BusType )
			{
				case BusTypeScsi:
					device_handle->bus_type = IO_BUS_TYPE_SCSI;
					break;

				case BusTypeAtapi:
				case BusTypeAta:
					device_handle->bus_type = IO_BUS_TYPE_ATA;
					break;

				case BusType1394:
					device_handle->bus_type = IO_BUS_TYPE_FIREWIRE;
					break;

				case BusTypeUsb:
					device_handle->bus_type = IO_BUS_TYPE_USB;
					break;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			fprintf(
			 stderr,
			 "Bus type:\t\t" );

			switch( ( ( STORAGE_DEVICE_DESCRIPTOR *) response )->BusType )
			{
				case BusTypeScsi:
					fprintf(
					 stderr,
					 "SCSI" );
					break;

				case BusTypeAtapi:
					fprintf(
					 stderr,
					 "ATAPI" );
					break;

				case BusTypeAta:
					fprintf(
					 stderr,
					 "ATA" );
					break;

				case BusType1394:
					fprintf(
					 stderr,
					 "FireWire (IEEE1394)" );
					break;

				case BusTypeSsa:
					fprintf(
					 stderr,
					 "Serial Storage Architecture (SSA)" );
					break;

				case BusTypeFibre:
					fprintf(
					 stderr,
					 "Fibre Channel" );
					break;

				case BusTypeUsb:
					fprintf(
					 stderr,
					 "USB" );
					break;

				case BusTypeRAID:
					fprintf(
					 stderr,
					 "RAID" );
					break;

				case BusTypeiScsi:
					fprintf(
					 stderr,
					 "iSCSI" );
					break;

				case BusTypeSas:
					fprintf(
					 stderr,
					 "SAS" );
					break;

				case BusTypeSata:
					fprintf(
					 stderr,
					 "SATA" );
					break;

				case BusTypeSd:
					fprintf(
					 stderr,
					 "Secure Digital (SD)" );
					break;

				case BusTypeMmc:
					fprintf(
					 stderr,
					 "Multi Media Card (MMC)" );
					break;

				default:
					fprintf(
					 stderr,
					 "Unknown: %d",
					 ( ( STORAGE_DEVICE_DESCRIPTOR *) response )->BusType );
					break;
			}
			fprintf(
			 stderr,
			 "\n" );
#endif
		}
		memory_free(
		 response );
	}
#else
#if defined( HAVE_IO_SCSI )
	/* Use the Linux sg (generic SCSI) driver to determine device information
	 */
	if( io_scsi_get_bus_type(
	     device_handle->file_descriptor,
	     &( device_handle->bus_type ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine bus type.",
		 function );

		return( -1 );
	}
	if( io_scsi_get_identiier(
	     device_handle->file_descriptor,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine SCSI identifier.",
		 function );

		return( -1 );
	}
	uint8_t pci_bus_address[ 64 ];
	size_t pci_bus_address_size = 64;

	if( io_scsi_get_pci_bus_address(
	     device_handle->file_descriptor,
	     pci_bus_address,
	     pci_bus_address_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine PCI bus address.",
		 function );

		return( -1 );
	}
	if( device_handle->media_information_set == 0 )
	{
		response_count = io_scsi_inquiry(
		                  device_handle->file_descriptor,
		                  0x00,
		                  0x00,
		                  response,
		                  255,
		                  NULL );

		if( response_count > 32 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			notify_verbose_dump_data(
			 response,
			 response_count );
#endif
			result = system_string_trim_copy_from_byte_stream(
				  device_handle->vendor,
				  64,
				  &( response[ 8 ] ),
				  15 - 8,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set vendor.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				device_handle->vendor[ 0 ] = 0;
			}
			result = system_string_trim_copy_from_byte_stream(
				  device_handle->model,
				  64,
				  &( response[ 16 ] ),
				  31 - 16,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set model.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				device_handle->model[ 0 ] = 0;
			}
			device_handle->removable             = ( response[ 1 ] & 0x80 ) >> 7;
			device_handle->device_type           = ( response[ 0 ] & 0x1f );
			device_handle->media_information_set = 1;
		}
	}
	if( device_handle->serial_number[ 0 ] == 0 )
	{
		response_count = io_scsi_inquiry(
		                  device_handle->file_descriptor,
		                  0x01,
		                  0x80,
		                  response,
		                  255,
		                  NULL );

		if( response_count > 4 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			notify_verbose_dump_data(
			 response,
			 response_count );
#endif
			result = system_string_trim_copy_from_byte_stream(
				  device_handle->serial_number,
				  64,
				  &( response[ 4 ] ),
				  response_count - 4,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set serial number.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
				device_handle->serial_number[ 0 ] = 0;
			}
		}
	}
#endif
#if defined( HAVE_IO_ATA )
	if( device_handle->bus_type == IO_BUS_TYPE_ATA )
	{
		if( io_ata_get_device_configuration(
		     device_handle->file_descriptor,
		     &device_configuration,
		     error ) == -1 )
		{
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				notify_error_backtrace(
				 *error );
			}
			liberror_error_free(
			 error );
		}
		else
		{
			if( device_handle->serial_number[ 0 ] == 0 )
			{
				result = system_string_trim_copy_from_byte_stream(
					  device_handle->serial_number,
					  64,
					  device_configuration.serial_no,
					  20,
					  error );

				if( result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set serial number.",
					 function );

					return( -1 );
				}
				else if( result == 0 )
				{
					device_handle->serial_number[ 0 ] = 0;
				}
			}
			if( device_handle->model[ 0 ] == 0 )
			{
				result = system_string_trim_copy_from_byte_stream(
					  device_handle->model,
					  64,
					  device_configuration.model,
					  40,
					  error );

				if( result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set model.",
					 function );

					return( -1 );
				}
				else if( result == 0 )
				{
					device_handle->model[ 0 ] = 0;
				}
			}
			if( device_handle->media_information_set == 0 )
			{
				device_handle->removable             = ( device_configuration.config & 0x0080 ) >> 7;
				device_handle->device_type           = ( device_configuration.config & 0x1f00 ) >> 8;
				device_handle->media_information_set = 1;
			}
		}
	}
#endif
#if defined( HAVE_IO_OPTICAL_DISK )
	if( device_handle->device_type == 0x05 )
	{
		if( io_optical_disk_get_table_of_contents(
		     device_handle->file_descriptor,
		     error ) != 1 )
		{
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				notify_error_backtrace(
				 *error );
			}
			liberror_error_free(
			 error );
		}
	}
#endif
/* Disabled for now
#if defined( HAVE_IO_USB )
	if( device_handle->bus_type == IO_BUS_TYPE_USB )
	{
		if( io_usb_test(
		     device_handle->file_descriptor,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to test USB.",
			 function );

			return( -1 );
		}
	}
#endif
*/
#endif
	return( 1 );
}

/* Retrieves the media information value
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int device_handle_get_media_information_value(
     device_handle_t *device_handle,
     char *media_information_value_identifier,
     size_t media_information_value_identifier_length,
     system_character_t *media_information_value,
     size_t media_information_value_size,
     liberror_error_t **error )
{
	uint8_t *utf8_media_information_value          = NULL;
	static char *function                          = "device_handle_get_media_information_value";
	size_t calculated_media_information_value_size = 0;
	size_t utf8_media_information_value_size       = 0;

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
	if( media_information_value_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media information value identifier.",
		 function );

		return( -1 );
	}
	if( media_information_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media information value.",
		 function );

		return( -1 );
	}
	if( ( media_information_value_identifier_length == 5 )
	 && ( narrow_string_compare(
	       "model",
	       media_information_value_identifier,
	       media_information_value_identifier_length ) == 0 ) )
	{
		utf8_media_information_value = (uint8_t *) device_handle->model;
	}
	else if( ( media_information_value_identifier_length == 6 )
	      && ( narrow_string_compare(
	            "vendor",
	            media_information_value_identifier,
	            media_information_value_identifier_length ) == 0 ) )
	{
		utf8_media_information_value = (uint8_t *) device_handle->vendor;
	}
	else if( ( media_information_value_identifier_length == 13 )
	      && ( narrow_string_compare(
	            "serial_number",
	            media_information_value_identifier,
	            media_information_value_identifier_length ) == 0 ) )
	{
		utf8_media_information_value = (uint8_t *) device_handle->serial_number;
	}
	else
	{
		return( 0 );
	}
	if( utf8_media_information_value != NULL )
	{
		if( utf8_media_information_value[ 0 ] == 0 )
		{
			return( 0 );
		}
		/* Determine the header value size
		 */
		utf8_media_information_value_size = 1 + narrow_string_length(
		                                         (char *) utf8_media_information_value );

		if( system_string_size_from_utf8_string(
		     utf8_media_information_value,
		     utf8_media_information_value_size,
		     &calculated_media_information_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine media information value size.",
			 function );

			return( -1 );
		}
		if( media_information_value_size < calculated_media_information_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: media information value too small.",
			 function );

			return( -1 );
		}
		if( system_string_copy_from_utf8_string(
		     media_information_value,
		     media_information_value_size,
		     utf8_media_information_value,
		     utf8_media_information_value_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set media information value.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Set the read error values in the device handle
 * Returns 1 if successful or -1 on error
 */
int device_handle_set_read_error_values(
     device_handle_t *device_handle,
     int8_t read_error_retry,
     uint32_t byte_error_granularity,
     uint8_t wipe_block_on_read_error,
     liberror_error_t **error )
{
	static char *function = "device_handle_set_read_error_values";

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
	device_handle->read_error_retry         = read_error_retry;
	device_handle->byte_error_granularity   = byte_error_granularity;
	device_handle->wipe_block_on_read_error = wipe_block_on_read_error;

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
        system_character_t media_size_string[ 16 ];

	static char *function = "device_handle_media_information_fprint";
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
	fprintf(
	 stream,
	 "Media information:\n" );

	if( device_handle->media_information_set != 0 )
	{
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

		fprintf(
		 stream,
		 "Bus type:\t\t" );

		switch( device_handle->bus_type )
		{
			case IO_BUS_TYPE_ATA:
				fprintf(
				 stream,
				 "ATA/ATAPI" );
				break;

			case IO_BUS_TYPE_FIREWIRE:
				fprintf(
				 stream,
				 "FireWire (IEEE1394)" );
				break;

			case IO_BUS_TYPE_SCSI:
				fprintf(
				 stream,
				 "SCSI" );
				break;

			case IO_BUS_TYPE_USB:
				fprintf(
				 stream,
				 "USB" );
				break;
		}
		fprintf(
		 stream,
		 "\n" );

		if( device_handle->removable != 0 )
		{
			fprintf(
			 stream,
			 "Removable:\t\tyes\n" );
		}
		fprintf(
		 stream,
		 "Vendor:\t\t\t%" PRIs_SYSTEM "\n",
		 device_handle->vendor );
		fprintf(
		 stream,
		 "Model:\t\t\t%" PRIs_SYSTEM "\n",
		 device_handle->model );
		fprintf(
		 stream,
		 "Serial:\t\t\t%" PRIs_SYSTEM "\n",
		 device_handle->serial_number );
	}
	if( device_handle->media_size_set != 0 )
	{
		result = byte_size_string_create(
			  media_size_string,
			  16,
			  device_handle->media_size,
			  BYTE_SIZE_STRING_UNIT_MEGABYTE,
			  NULL );

		if( result == 1 )
		{
			fprintf(
			 stream,
			 "Media size:\t\t%" PRIs_SYSTEM " (%" PRIu64 " bytes)\n",
			 media_size_string,
			 device_handle->media_size );
		}
		else
		{
			fprintf(
			 stream,
			 "Media size:\t\t%" PRIu64 " bytes\n",
			 device_handle->media_size );
		}
	}
	fprintf(
	 stream,
	 "\n" );

	return( 1 );
}


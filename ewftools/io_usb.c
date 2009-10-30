/*
 * USB IO functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include <liberror.h>

#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_LINUX_USBDEVICE_FS_H )
#include <linux/usbdevice_fs.h>
#endif

#if defined( HAVE_LINUX_USB_CH9_H )
#include <linux/usb/ch9.h>
#endif

#include <libsystem.h>

#include "io_usb.h"
#include "io_scsi.h"

#define IO_USB_CONTROL_COMMAND_TIMEOUT	5000

#if defined( HAVE_LINUX_USB_CH9_H )

/* Sends an USB ioctl to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int io_usb_ioctl(
     int file_descriptor,
     int interface_number,
     int request,
     void *request_data,
     liberror_error_t **error )
{
	libsystem_character_t error_string[ 128 ];

#if defined( USBDEVFS_IOCTL )
	struct usbdevfs_ioctl ioctl_request;
#endif

	static char *function = "io_usb_ioctl";

	if( file_descriptor == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file descriptor.",
		 function );

		return( -1 );
	}
#if defined( USBDEVFS_IOCTL )
	ioctl_request.ifno       = interface_number;
	ioctl_request.ioctl_code = request;
	ioctl_request.data       = request_data;

	if( ioctl(
	     file_descriptor,
	     USBDEVFS_IOCTL,
	     &ioctl_request ) == -1 )
	{
		if( libsystem_error_copy_to_string(
		     errno,
		     error_string,
		     128,
		     error ) == 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: USBDEVFS_IOCTL with error: %" PRIs_LIBSYSTEM ".",
			 function,
			 error_string );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: USBDEVFS_IOCTL.",
			 function );
		}
		return( -1 );
	}
#endif
	return( 1 );
}

/* Sends a USB control command to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int io_usb_control_command(
     int file_descriptor,
     uint8_t request_type,
     uint8_t request,
     uint16_t value,
     uint16_t index,
     uint8_t *buffer,
     size_t buffer_size,
     liberror_error_t **error )
{
	libsystem_character_t error_string[ 128 ];

#if defined( USBDEVFS_CONTROL )
	struct usbdevfs_ctrltransfer control_request;
#endif

	static char *function = "io_usb_control_command";

	if( file_descriptor == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file descriptor.",
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
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( USBDEVFS_CONTROL )
	control_request.bRequestType = request_type;
	control_request.bRequest     = request;
	control_request.wValue       = value;
	control_request.wIndex       = index;
	control_request.wLength      = buffer_size;
	control_request.timeout      = IO_USB_CONTROL_COMMAND_TIMEOUT;
	control_request.data         = buffer;

	if( ioctl(
	     file_descriptor,
	     USBDEVFS_CONTROL,
	     (void *) &control_request ) == -1 )
	{
		if( libsystem_error_copy_to_string(
		     errno,
		     error_string,
		     128,
		     error ) == 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: USBDEVFS_CONTROL with error: %" PRIs_LIBSYSTEM ".",
			 function,
			 error_string );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: USBDEVFS_CONTROL.",
			 function );
		}
		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	libsystem_notify_verbose_print_data(
	 buffer,
	 buffer_size );
#endif
	return( 1 );
}

/* Sends a USB ioctl to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int io_usb_test(
     int file_descriptor,
     liberror_error_t **error )
{
	libsystem_character_t error_string[ 128 ];

#if defined( USBDEVFS_CONNECTINFO )
	struct usbdevfs_connectinfo connection_information;
#endif

	static char *function = "io_usb_test";

	if( file_descriptor == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file descriptor.",
		 function );

		return( -1 );
	}
#if defined( USBDEVFS_CONNECTINFO )
	if( ioctl(
	     file_descriptor,
	     USBDEVFS_CONNECTINFO,
	     &connection_information ) == -1 )
	{
		if( libsystem_error_copy_to_string(
		     errno,
		     error_string,
		     128,
		     error ) == 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: USBDEVFS_CONNECTINFO with error: %" PRIs_LIBSYSTEM ".",
			 function,
			 error_string );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: USBDEVFS_CONNECTINFO.",
			 function );
		}
		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libsystem_notify_verbose_print_data(
	 (uint8_t *) &connection_information,
	 sizeof( struct usbdevfs_connectinfo ) );
#endif
#endif
	return( 1 );
}

#ifdef X

/* Sends a USB control command to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int io_usb_test(
     int file_descriptor,
     liberror_error_t **error )
{
	struct usb_device_descriptor device_descriptor;

	static char *function = "io_usb_test";
	uint8_t buffer[ 255 ];

	if( file_descriptor == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file descriptor.",
		 function );

		return( -1 );
	}
	if( io_usb_control_command(
	     file_descriptor,
	     USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE,
	     USB_REQ_GET_DESCRIPTOR,
	     USB_DT_DEVICE << 8,
	     0,
	     buffer,
	     255,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to send control command.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif

#endif


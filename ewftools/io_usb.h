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

#if !defined( _IO_USB_H )
#define _IO_USB_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LINUX_USB_CH9_H )

int io_usb_ioctl(
     int file_descriptor,
     int interface_number,
     int request,
     void *request_data,
     liberror_error_t **error );

int io_usb_control_command(
     int file_descriptor,
     uint8_t request_type,
     uint8_t request,
     uint16_t value,
     uint16_t index,
     uint8_t *buffer,
     size_t buffer_size,
     liberror_error_t **error );

int io_usb_test(
     int file_descriptor,
     liberror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif


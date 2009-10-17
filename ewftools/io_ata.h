/*
 * ATA/ATAPI IO functions
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

#if !defined( _IO_ATA_H )
#define _IO_ATA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( HAVE_CYGWIN_HDREG_H )
#include <cygwin/hdreg.h>
#endif

#if defined( HAVE_LINUX_HDREG_H )
#include <linux/hdreg.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HDIO_GET_IDENTITY )
int io_ata_get_device_configuration(
     int file_descriptor,
     struct hd_driveid *device_configuration,
     liberror_error_t **error );
#endif

#if defined( __cplusplus )
}
#endif

#endif


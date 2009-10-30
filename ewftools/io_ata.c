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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include <liberror.h>

#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_CYGWIN_HDREG_H )
#include <cygwin/hdreg.h>
#endif

#if defined( HAVE_LINUX_HDREG_H )
#include <linux/hdreg.h>
#endif

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#include <libsystem.h>

#include "io_ata.h"

#if defined( HDIO_GET_IDENTITY )

/* Sends a ATA DEVICE CONFIGURATION IDENTIFY to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int io_ata_get_device_configuration(
     int file_descriptor,
     struct hd_driveid *device_configuration,
     liberror_error_t **error )
{
	libsystem_character_t error_string[ 128 ];

	static char *function = "io_ata_get_device_configuration";

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
	if( device_configuration == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device configuration.",
		 function );

		return( -1 );
	}
#if defined( HDIO_GET_IDENTITY )
	if( ioctl(
	     file_descriptor,
	     HDIO_GET_IDENTITY,
	     device_configuration ) == -1 )
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
			 "%s: unable to query device for: HDIO_GET_IDENTITY with error: %" PRIs_LIBSYSTEM ".",
			 function,
			 error_string );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: HDIO_GET_IDENTITY.",
			 function );
		}
		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libsystem_notify_verbose_print_data(
	 (uint8_t *) device_configuration,
	 sizeof( struct hd_driveid ) );

	libsystem_notify_verbose_printf(
	 "Feature sets:\n" );
	libsystem_notify_verbose_printf(
	 "SMART:\t\t\t%d\n",
	 ( device_configuration->command_set_1 & 0x0001 ) );
	libsystem_notify_verbose_printf(
	 "Security Mode:\t\t%d (%d)\n",
	 ( device_configuration->command_set_1 & 0x0002 ) >> 1,
	 ( device_configuration->dlf & 0x0001 ) );
	libsystem_notify_verbose_printf(
	 "Security Mode enabled:\t%d\n",
	 ( device_configuration->dlf & 0x0002 ) >> 1 );
	libsystem_notify_verbose_printf(
	 "Removable Media:\t%d\n",
	 ( device_configuration->command_set_1 & 0x0004 ) >> 2 );
	libsystem_notify_verbose_printf(
	 "HPA:\t\t\t%d\n",
	 ( device_configuration->command_set_1 & 0x0400 ) >> 10 );
	libsystem_notify_verbose_printf(
	 "DCO:\t\t\t%d\n",
	 ( device_configuration->command_set_2 & 0x0800 ) >> 11 );
	libsystem_notify_verbose_printf(
	 "Media serial:\t\t%d\n",
	 ( device_configuration->cfsse & 0x0004 ) >> 2 );
	libsystem_notify_verbose_printf(
	 "\n" );
#endif
#endif
	return( 1 );
}

#endif


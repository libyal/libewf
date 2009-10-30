/*
 * Optical disk functions
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
#include <types.h>

#include <liberror.h>

#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_LINUX_CDROM_H )
#include <linux/cdrom.h>
#endif

#include <libsystem.h>

#include "io_optical_disk.h"

#if defined( HAVE_LINUX_CDROM_H )

/* Retrieves the table of contents (toc) from the optical disk
 * Returns 1 if successful or -1 on error
 */
int io_optical_disk_get_table_of_contents(
     int file_descriptor,
     liberror_error_t **error )
{
	struct cdrom_tochdr toc_header;
	struct cdrom_tocentry toc_entry;

	static char *function   = "io_optical_disk_get_table_of_contents";
	uint16_t entry_iterator = 0;
	uint8_t first_entry     = 0;
	uint8_t last_entry      = 0;

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
	if( ioctl(
	     file_descriptor,
	     CDROMREADTOCHDR,
	     &toc_header ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: CDROMREADTOCHDR.",
		 function );

		return( -1 );
	}
	first_entry = toc_header.cdth_trk0;
	last_entry  = toc_header.cdth_trk1;

#if defined( HAVE_DEBUG_OUTPUT )
	libsystem_notify_verbose_printf(
	 "Table of contents (TOC):\n" );
	libsystem_notify_verbose_printf(
	 "\tAmount of entries:\t%d\n",
	 last_entry );
#endif

	for( entry_iterator = (uint16_t) first_entry;
	     entry_iterator <= (uint16_t) last_entry;
	     entry_iterator++ )
	{
		if( memory_set(
		     &toc_entry,
		     0,
		     sizeof( struct cdrom_tocentry ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable clear toc entry.",
			 function );

			return( -1 );
		}
		toc_entry.cdte_track  = (uint8_t) entry_iterator;
		toc_entry.cdte_format = CDROM_LBA;

		if( ioctl(
		     file_descriptor,
		     CDROMREADTOCENTRY,
		     &toc_entry ) == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: CDROMREADTOCENTRY.",
			 function );

			return( -1 );
		}
		libsystem_notify_verbose_printf(
		 "\tTrack: %" PRIu16 "",
		 entry_iterator );

		if( toc_entry.cdte_format == CDROM_MSF )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			libsystem_notify_verbose_printf(
			 " start:\t\t%02" PRIu8 ":%02" PRIu8 ".%" PRIu8 "",
			 toc_entry.cdte_addr.msf.minute,
			 toc_entry.cdte_addr.msf.second,
			 toc_entry.cdte_addr.msf.frame );
#endif
		}
		else if( toc_entry.cdte_format == CDROM_LBA )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			libsystem_notify_verbose_printf(
			 " start:\t\t%" PRIu32 "",
			 toc_entry.cdte_addr.lba );
#endif
		}
		libsystem_notify_verbose_printf(
		 "\n" );

		if( ( toc_entry.cdte_ctrl & CDROM_DATA_TRACK ) == CDROM_DATA_TRACK )
		{
		}
	}
	if( memory_set(
	     &toc_entry,
	     0,
	     sizeof( struct cdrom_tocentry ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable clear toc entry.",
		 function );

		return( -1 );
	}
	toc_entry.cdte_track  = CDROM_LEADOUT;
	toc_entry.cdte_format = CDROM_LBA;

	if( ioctl(
	     file_descriptor,
	     CDROMREADTOCENTRY,
	     &toc_entry ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: CDROMREADTOCENTRY.",
		 function );

		return( -1 );
	}
	libsystem_notify_verbose_printf(
	 "\tLead out" );

	if( toc_entry.cdte_format == CDROM_MSF )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		libsystem_notify_verbose_printf(
		 " start:\t\t%02" PRIu8 ":%02" PRIu8 ".%" PRIu8 "",
		 toc_entry.cdte_addr.msf.minute,
		 toc_entry.cdte_addr.msf.second,
		 toc_entry.cdte_addr.msf.frame );
#endif
	}
	else if( toc_entry.cdte_format == CDROM_LBA )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		libsystem_notify_verbose_printf(
		 " start:\t\t%" PRIu32 "",
		 toc_entry.cdte_addr.lba );
#endif
	}
	libsystem_notify_verbose_printf(
	 "\n\n" );

	return( 1 );
}

/* CDROM_DISC_STATUS
 * CDROM_CHANGER_NSLOTS
 * CDROMMULTISESSION
 * CDROM_GET_MCN
 */

#endif


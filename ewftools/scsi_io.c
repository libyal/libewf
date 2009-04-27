/*
 * SCSI IO functions
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <notify.h>
#include <types.h>

#include <liberror.h>

#if defined( HAVE_SYS_IOCTL_H )
#include <sys/ioctl.h>
#endif

#if defined( HAVE_SCSI_SG_H )
#include <scsi/sg.h>
#endif

#include "notify.h"
#include "scsi_io.h"

#if defined( HAVE_SCSI_IO )

/* Sends a SCSI command to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int scsi_io_command(
     int file_descriptor,
     uint8_t *command,
     size_t command_size,
     uint8_t *response,
     size_t response_size,
     uint8_t *sense,
     size_t sense_size,
     liberror_error_t **error )
{
	struct sg_io_hdr sg_io_header;

	static char *function = "scsi_io_command";

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
	if( command == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid command.",
		 function );

		return( -1 );
	}
	if( command_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid command size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( response == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid response.",
		 function );

		return( -1 );
	}
	if( response_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid response size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sense == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sense.",
		 function );

		return( -1 );
	}
	if( sense_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid sense size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &sg_io_header,
	     0,
	     sizeof( struct sg_io_hdr ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear SG IO header.",
		 function );

		return( -1 );
	}

	/* Timeout in milli seconds: 30 seconds
	 */
	sg_io_header.interface_id    = 'S';
	sg_io_header.cmdp            = command;
	sg_io_header.cmd_len         = command_size;
	sg_io_header.sbp             = sense;
	sg_io_header.mx_sb_len       = sense_size;
	sg_io_header.dxferp          = response;
	sg_io_header.dxfer_len       = response_size;
	sg_io_header.dxfer_direction = SG_DXFER_FROM_DEV;
	sg_io_header.timeout         = 30000;

	if( ioctl(
	     file_descriptor,
	     SG_IO,
	     &sg_io_header ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: SG_IO.",
		 function );

		return( -1 );
	}
	/* Check if command returned an error
	 */
	if( ( sg_io_header.info & SG_INFO_OK_MASK ) != SG_INFO_OK )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: error while querying device: scsi status: %X, host status: %X, driver status: %X.\n",
		 function,
		 sg_io_header.status,
		 sg_io_header.host_status,
		 sg_io_header.driver_status );

		return( -1 );
	}
	return( 1 );
}

/* Sends a SCSI inquiry to the file descriptor
 * Returns the amount of bytes read if successful or -1 on error
 */
ssize_t scsi_io_inquiry(
         int file_descriptor,
         uint8_t inquiry_vital_product_data,
         uint8_t code_page,
         uint8_t *response,
         size_t response_size,
         liberror_error_t **error )
{
	scsi_io_command_descriptor_t command;

	uint8_t sense[ SCSI_IO_SENSE_SIZE ];

	static char *function  = "scsi_io_inquiry";
	ssize_t response_count = 0;

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
	if( response == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid response.",
		 function );

		return( -1 );
	}
	if( response_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid response size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &command,
	     0,
	     sizeof( scsi_io_command_descriptor_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear command.",
		 function );

		return( -1 );
	}
	command.operation_code = SCSI_IO_OPERATION_CODE_INQUIRY;

	if( inquiry_vital_product_data != 0 )
	{
		command.lun      |= 0x01;
		command.reserved1 = code_page;
	}
	if( scsi_io_command(
	     file_descriptor,
	     (uint8_t *) &command,
	     sizeof( scsi_io_command_descriptor_t ),
	     response,
	     response_size,
	     sense,
	     SCSI_IO_SENSE_SIZE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: SCSI command failed.",
		 function );

		return( -1 );
	}
	/* In standard inquiry mode the additional size is in the 5th byte
	 * in vital produce data inquiry mode it is in the 4th byte
	 */
	if( inquiry_vital_product_data == 0 )
	{
		response_count = (ssize_t) ( response[ 4 ] + 5 );
	}
	else
	{
		response_count = (ssize_t) ( response[ 3 ] + 4 );
	}
	if( response_count > (ssize_t) response_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: response too small.",
		 function );

		return( -1 );
	}
	return( response_count );
}

#endif


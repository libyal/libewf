/*
 * SCSI IO functions
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if defined( HAVE_SCSI_SCSI_H )
#include <scsi/scsi.h>
#endif

#if defined( HAVE_SCSI_SCSI_IOCTL_H )
#include <scsi/scsi_ioctl.h>
#endif

#if defined( HAVE_SCSI_SG_H )
#include <scsi/sg.h>
#endif

#include <libsystem.h>

#include "io_bus.h"
#include "io_scsi.h"

#if defined( HAVE_SCSI_SG_H )

/* Sends a SCSI command to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int io_scsi_command(
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

	static char *function = "io_scsi_command";

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
		 "%s: unable to clear SCSI SG IO header.",
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
		 "%s: error while querying device: scsi status: %X, host status: %X, driver status: %X.",
		 function,
		 sg_io_header.status,
		 sg_io_header.host_status,
		 sg_io_header.driver_status );

		return( -1 );
	}
	return( 1 );
}

/* Sends a SCSI ioctl to the file descriptor
 * Returns 1 if successful or -1 on error
 */
int io_scsi_ioctrl(
     int file_descriptor,
     void *request_data,
     size_t request_data_size,
     liberror_error_t **error )
{
	uint8_t *ioctrl_request    = 0;
	static char *function      = "io_scsi_ioctrl";
	size_t ioctrl_request_size = 0;

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
	ioctrl_request_size = sizeof( io_scsi_ioctrl_header_t ) + request_data_size;

	ioctrl_request = (uint8_t *) memory_allocate(
	                              ioctrl_request_size );

	if( ioctrl_request == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create SCSI ioctrl request.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     ioctrl_request,
	     0,
	     ioctrl_request_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear SCSI ioctrl request.",
		 function );

		memory_free(
		 ioctrl_request );

		return( -1 );
	}
	( (io_scsi_ioctrl_header_t *) ioctrl_request )->request_size = request_data_size;

	if( memory_copy(
	     &( ioctrl_request[ sizeof( io_scsi_ioctrl_header_t ) ] ),
	     request_data,
	     request_data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set SCSI ioctrl request.",
		 function );

		memory_free(
		 ioctrl_request );

		return( -1 );
	}
	if( ioctl(
	     file_descriptor,
	     SCSI_IOCTL_SEND_COMMAND,
	     ioctrl_request ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: SCSI_IOCTL_SEND_COMMAND.",
		 function );

		memory_free(
		 ioctrl_request );

		return( -1 );
	}
	memory_free(
	 ioctrl_request );

	return( 1 );
}

/* Sends a SCSI inquiry to the file descriptor
 * Returns the amount of bytes read if successful or -1 on error
 */
ssize_t io_scsi_inquiry(
         int file_descriptor,
         uint8_t inquiry_vital_product_data,
         uint8_t code_page,
         uint8_t *response,
         size_t response_size,
         liberror_error_t **error )
{
	io_scsi_command_descriptor_t command;

	uint8_t sense[ IO_SCSI_SENSE_SIZE ];

	static char *function  = "io_scsi_inquiry";
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
	     sizeof( io_scsi_command_descriptor_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear command.",
		 function );

		return( -1 );
	}
	command.operation_code = IO_SCSI_OPERATION_CODE_INQUIRY;

	if( inquiry_vital_product_data != 0 )
	{
		command.lun      |= 0x01;
		command.reserved1 = code_page;
	}
	if( io_scsi_command(
	     file_descriptor,
	     (uint8_t *) &command,
	     sizeof( io_scsi_command_descriptor_t ),
	     response,
	     response_size,
	     sense,
	     IO_SCSI_SENSE_SIZE,
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

/* Retrieves the SCSI identifier
 * Returns 1 if successful or -1 on error
 */
int io_scsi_get_identier(
     int file_descriptor,
     liberror_error_t **error )
{
#if defined( SG_GET_SCSI_ID )
	struct
	{
		int four_in_one;
		int host_unique_id;
	} identifier;
#endif

	static char *function = "io_scsi_get_identifier";

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
#if defined( SG_GET_SCSI_ID )
	if( ioctl(
	     file_descriptor,
	     SCSI_IOCTL_GET_IDLUN,
	     &identifier ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: SCSI_IOCTL_GET_IDLUN.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libsystem_notify_verbose_print_data(
	 (uint8_t *) &identifier,
	 sizeof( identifier ) );
#endif
#endif
	return( 1 );
}

/* Determines and retrieves the bus type
 * Returns 1 if successful or -1 on error
 */
int io_scsi_get_bus_type(
     int file_descriptor,
     uint8_t *bus_type,
     liberror_error_t **error )
{
#if defined( SCSI_IOCTL_PROBE_HOST )
	union 
	{
		int length;
		char buffer[ 128 ];
	} sg_probe_host;

	size_t sg_probe_host_length = 0;
#endif

	static char *function       = "io_scsi_get_bus_type";

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
	if( bus_type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bus type.",
		 function );

		return( -1 );
	}
	*bus_type = IO_BUS_TYPE_UNKNOWN;

#if defined( SCSI_IOCTL_PROBE_HOST )
	sg_probe_host.length = 127;

	if( ioctl(
	     file_descriptor,
	     SCSI_IOCTL_PROBE_HOST,
	     &sg_probe_host ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: SCSI_IOCTL_PROBE_HOST.",
		 function );

		return( -1 );
	}
	sg_probe_host.buffer[ 127 ] = 0;

	sg_probe_host_length = narrow_string_length(
	                        sg_probe_host.buffer );

#if defined( HAVE_DEBUG_OUTPUT )
	libsystem_notify_verbose_printf(
	 "SCSI_IOCTL_PROBE_HOST (%d): %s\n",
	 sg_probe_host_length,
	 sg_probe_host.buffer );
#endif

	if( sg_probe_host_length >= 4 )
	{
		if( narrow_string_compare(
		     sg_probe_host.buffer,
		     "ahci",
		     4 ) == 0 )
		{
			*bus_type = IO_BUS_TYPE_ATA;
		}
		else if( narrow_string_compare(
		          sg_probe_host.buffer,
		          "pata",
		          4 ) == 0 )
		{
			*bus_type = IO_BUS_TYPE_ATA;
		}
		else if( narrow_string_compare(
		          sg_probe_host.buffer,
		          "sata",
		          4 ) == 0 )
		{
			*bus_type = IO_BUS_TYPE_ATA;
		}
	}
	/* Serial Bus Protocol (SBP-2)
	 */
	else if( ( sg_probe_host_length == 15 )
	      && ( narrow_string_compare(
	            sg_probe_host.buffer,
	            "SBP-2 IEEE-1394",
	            15 ) == 0 ) )
	{
		*bus_type = IO_BUS_TYPE_FIREWIRE;
	}
	else if( ( sg_probe_host_length == 43 )
	      && ( narrow_string_compare(
	            sg_probe_host.buffer,
	            "SCSI emulation for USB Mass Storage devices",
	            43 ) == 0 ) )
	{
		*bus_type = IO_BUS_TYPE_USB;
	}
#endif
	return( 1 );
}

#if !defined( SCSI_IOCTL_GET_PCI )
#define SCSI_IOCTL_GET_PCI	0x5387
#endif

/* Determines and retrieves the PCI bus address
 * Returns 1 if successful or -1 on error
 */
int io_scsi_get_pci_bus_address(
     int file_descriptor,
     uint8_t *pci_bus_address,
     size_t pci_bus_address_size,
     liberror_error_t **error )
{
	static char *function = "io_scsi_get_bus_type";

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
	if( pci_bus_address == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid PCI bus address.",
		 function );

		return( -1 );
	}
	if( pci_bus_address_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid PCI bus address size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( pci_bus_address_size <= 8 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid PCI bus address size value too small.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     pci_bus_address,
	     0,
	     pci_bus_address_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear PCI bus address.",
		 function );

		return( -1 );
	}
#if defined( SCSI_IOCTL_GET_PCI )
	if( ioctl(
	     file_descriptor,
	     SCSI_IOCTL_GET_PCI,
	     pci_bus_address ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: SCSI_IOCTL_GET_PCI.",
		 function );

		return( -1 );
	}
	pci_bus_address[ pci_bus_address_size - 1 ] = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libsystem_notify_verbose_printf(
	 "SCSI_IOCTL_GET_PCI: %s\n",
	 pci_bus_address );
#endif

#endif
	return( 1 );
}

#endif


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

#if !defined( _IO_SCSI_H )
#define _IO_SCSI_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_SCSI_SG_H )

enum IO_SCSI_OPERATION_CODES
{
	IO_SCSI_OPERATION_CODE_INQUIRY	= 0x12
};

/* SCSI device types:
 * 0x00      - direct-access device (e.g., magnetic disk)
 * 0x01      - sequential-access device (e.g., magnetic tape)
 * 0x02      - printer device
 * 0x03      - processor device
 * 0x04      - write-once device
 * 0x05      - optical disk (CDROM) device
 * 0x06      - scanner device
 * 0x07      - optical memory device (e.g., some optical disks)
 * 0x08      - medium Changer (e.g. jukeboxes)
 * 0x09      - communications device
 * 0x0a-0x0b - defined by ASC IT8 (Graphic arts pre-press devices)
 * 0x0c      - Storage array controller device (e.g., RAID)
 * 0x0d      - Enclosure services device
 * 0x0e      - Simplified direct-access device (e.g., magnetic disk)
 * 0x0f      - Optical card reader/writer device
 * 0x10      - Reserved for bridging expanders
 * 0x11      - Object-based Storage Device
 * 0x12      - Automation/Drive Interface
 * 0x13-0x1d - reserved
 * 0x1e      - Well known logical unit
 * 0x1f      - unknown or no device type
 */

/* The SCSI command descriptor block (CDB)
 */
typedef struct io_scsi_command_descriptor io_scsi_command_descriptor_t;

struct io_scsi_command_descriptor
{
	/* The operation code
	 */
	uint8_t operation_code;

	/* The LUN and reserved bytes
	 * Bits:
	 * 0 - 4 reserved
	 * 5 - 7 LUN
	 */
	uint8_t lun;

	/* Reserved
	 */
	uint8_t reserved1;

	/* Reserved
	 */
	uint8_t reserved2;

	/* The size of the receive buffer (allocation length)
	 */
	uint8_t receive_size;

	/* The control
	 */
	uint8_t control;
};

/* The SCSI ioctrl header
 */
typedef struct io_scsi_ioctrl_header io_scsi_ioctrl_header_t;

struct io_scsi_ioctrl_header
{
	/* The size of the request (input data)
	 */
	unsigned int request_size;

	/* The size out the response (output data)
	 */
	unsigned int response_size;

	/* The request/response data
	 */
};

#define IO_SCSI_RESPONSE_SIZE_INQUIRY	96
#define IO_SCSI_SENSE_SIZE		32

int io_scsi_command(
     int file_descriptor,
     uint8_t *command,
     size_t command_size,
     uint8_t *response,
     size_t response_size,
     uint8_t *sense,
     size_t sense_size,
     liberror_error_t **error );

int io_scsi_ioctrl(
     int file_descriptor,
     void *request_data,
     size_t request_data_size,
     liberror_error_t **error );

ssize_t io_scsi_inquiry(
         int file_descriptor,
         uint8_t inquiry_vital_product_data,
         uint8_t code_page,
         uint8_t *response,
         size_t response_size,
         liberror_error_t **error );

int io_scsi_get_identier(
     int file_descriptor,
     liberror_error_t **error );

int io_scsi_get_bus_type(
     int file_descriptor,
     uint8_t *bus_type,
     liberror_error_t **error );

int io_scsi_get_pci_bus_address(
     int file_descriptor,
     uint8_t *pci_bus_address,
     size_t pci_bus_address_size,
     liberror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif


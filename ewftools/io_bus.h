/*
 * IO bus functions
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

#if !defined( _IO_BUS_H )
#define _IO_BUS_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

enum IO_BUS_TYPES
{
	IO_BUS_TYPE_UNKNOWN	= 0,
	IO_BUS_TYPE_ATA		= (uint8_t) 'a',
	IO_BUS_TYPE_FIREWIRE	= (uint8_t) 'f',
	IO_BUS_TYPE_SCSI	= (uint8_t) 's',
	IO_BUS_TYPE_USB		= (uint8_t) 'u'
};

#if defined( __cplusplus )
}
#endif

#endif


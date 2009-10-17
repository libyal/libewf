/*
 * Definitions for libewf
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _EWF_DEFINIONS_H )
#define _EWF_DEFINIONS_H

#include <common.h>
#include <types.h>

/* The minimum chunk size is 32k (32 * 1024)
 * 64 * 512 (64 sectors)
 */
#define EWF_MINIMUM_CHUNK_SIZE			32768

/* The possible EWF file formats
 */
#define EWF_FORMAT_UNKNOWN			(uint8_t) 'u'
#define EWF_FORMAT_E01				(uint8_t) 'e'
#define EWF_FORMAT_L01				(uint8_t) 'l'
#define EWF_FORMAT_S01				(uint8_t) 's'

/* The EWF compression levels
 */
#define EWF_COMPRESSION_UNKNOWN 		-2
#define EWF_COMPRESSION_DEFAULT			-1
#define EWF_COMPRESSION_NONE			 0
#define EWF_COMPRESSION_FAST			 1
#define EWF_COMPRESSION_BEST			 2

/* The EWF offset masks
 */
#define EWF_OFFSET_COMPRESSED_READ_MASK 	0x7fffffff
#define EWF_OFFSET_COMPRESSED_WRITE_MASK 	0x80000000

/* EWF the initial maximum amount of offsets in a table section
 */
#define EWF_MAXIMUM_OFFSETS_IN_TABLE		16375

/* EWF maximum amount of offsets in a table section
 * as of EnCase 6
 */
#define EWF_MAXIMUM_OFFSETS_IN_TABLE_ENCASE6	65534

#endif


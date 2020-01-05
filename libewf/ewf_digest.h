/*
 * EWF degest section
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _EWF_DIGEST_H )
#define _EWF_DIGEST_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct ewf_digest ewf_digest_t;

struct ewf_digest
{
	/* The MD5 hash of the aquired data
	 * consists of 16 bytes
	 */
	uint8_t md5_hash[ 16 ];

	/* The SHA1 hash of the aquired data
	 * consists of 20 bytes
	 */
	uint8_t sha1_hash[ 20 ];

	/* Padding
	 * consists of 40 bytes
	 * value should be 0x00
	 */
	uint8_t padding1[ 40 ];

	/* The section checksum of all (previous) data
	 * consists of 4 bytes (32 bits)
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_DIGEST_H ) */


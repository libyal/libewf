/*
 * EWF hash sections
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

#if !defined( _EWF_HASH_H )
#define _EWF_HASH_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The version 1 hash section
 */
typedef struct ewf_hash ewf_hash_t;

struct ewf_hash
{
	/* The MD5 hash of the aquired data
	 * Consists of 16 bytes
	 */
	uint8_t md5_hash[ 16 ];

	/* Unknown
	 * Consists of 16 bytes
	 */
	uint8_t unknown1[ 16 ];

	/* The section checksum of all previous data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

/* The version 2 MD5 hash section
 */
typedef struct ewf_md5_hash ewf_md5_hash_t;

struct ewf_md5_hash
{
	/* The MD5 hash of the aquired data
	 * Consists of 16 bytes
	 */
	uint8_t md5_hash[ 16 ];

	/* The section checksum of all previous data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Padding
	 * Consists of 12 bytes
	 */
	uint8_t padding[ 12 ];
};

/* The version 2 SHA1 hash section
 */
typedef struct ewf_sha1_hash ewf_sha1_hash_t;

struct ewf_sha1_hash
{
	/* The SHA1 hash of the aquired data
	 * Consists of 20 bytes
	 */
	uint8_t sha1_hash[ 20 ];

	/* The section checksum of all previous data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Padding
	 * Consists of 8 bytes
	 */
	uint8_t padding[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EWF_HASH_H ) */


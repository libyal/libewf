/*
 * Hash sections functions
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

#if !defined( _LIBEWF_HASH_SECTIONS_H )
#define _LIBEWF_HASH_SECTIONS_H

#include <common.h>
#include <types.h>

#include "libewf_values_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_hash_sections libewf_hash_sections_t;

/* Additional subhandle for media specific parameters
 */
struct libewf_hash_sections
{
	/* The stored xhash
	 */
	uint8_t *xhash;

	/* The size of the stored xhash
	 */
	size_t xhash_size;

	/* The MD5 hash of the data as found in the hash section
	 */
	uint8_t md5_hash[ 16 ];

	/* Value to indicate if the MD5 hash was set
	 */
	uint8_t md5_hash_set;

	/* The MD5 hash of the data as found in the digest section
	 */
	uint8_t md5_digest[ 16 ];

	/* Value to indicate if the MD5 digest was set
	 */
	uint8_t md5_digest_set;

	/* The SHA1 hash of the data as found in the digest section
	 */
	uint8_t sha1_digest[ 20 ];

	/* Value to indicate if the SHA1 digest was set
	 */
	uint8_t sha1_digest_set;
};

int libewf_hash_sections_initialize(
     libewf_hash_sections_t **hash_sections,
     liberror_error_t **error );

int libewf_hash_sections_free(
     libewf_hash_sections_t **hash_sections,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


/*
 * Hash sections functions
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

#if !defined( _LIBEWF_HASH_SECTIONS_H )
#define _LIBEWF_HASH_SECTIONS_H

#include <common.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"

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

	/* The MD5 hash of the data as found in the MD5 hash section
	 */
	uint8_t md5_hash[ 16 ];

	/* Value to indicate if the MD5 hash was set
	 */
	uint8_t md5_hash_set;

	/* The SHA1 hash of the data as found in the SHA1 hash section
	 */
	uint8_t sha1_hash[ 20 ];

	/* Value to indicate if the SHA1 hash was set
	 */
	uint8_t sha1_hash_set;

#ifdef TODO
	/* The SHA256 hash of the data as found in the SHA256 hash section
	 */
	uint8_t sha256_hash[ 32 ];

	/* Value to indicate if the SHA256 hash was set
	 */
	uint8_t sha256_hash_set;
#endif

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
     libcerror_error_t **error );

int libewf_hash_sections_free(
     libewf_hash_sections_t **hash_sections,
     libcerror_error_t **error );

int libewf_hash_sections_clone(
     libewf_hash_sections_t **destination_hash_sections,
     libewf_hash_sections_t *source_hash_sections,
     libcerror_error_t **error );

int libewf_hash_sections_set_digest_from_hash_values(
     libewf_hash_sections_t *hash_sections,
     const uint8_t *identifier,
     size_t identifier_length,
     libfvalue_table_t *hash_values,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_HASH_SECTIONS_H ) */


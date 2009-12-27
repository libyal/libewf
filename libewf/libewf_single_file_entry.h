/*
 * Single file entry functions
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

#if !defined( _LIBEWF_SINGLE_FILE_ENTRY_H )
#define _LIBEWF_SINGLE_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libewf_date_time.h"
#include "libewf_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_single_file_entry libewf_single_file_entry_t;

struct libewf_single_file_entry
{
	/* The data segment file number
	 */
	uint16_t data_segment_number;

	/* The data offset
	 */
	off64_t data_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The name
	 */
	libewf_character_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The (file) size
	 */
	size64_t size;

	/* The creation date and time
	 */
	time_t creation_time;

	/* The last modification date and time
	 */
	time_t modification_time;

	/* The last access date and time
	 */
	time_t access_time;

	/* The last (file system) entry modification date and time
	 */
	time_t entry_modification_time;

	/* The MD5 digest hash
	 */
	libewf_character_t *md5_hash;

	/* The MD5 digest hash size
	 */
	size_t md5_hash_size;
};

int libewf_single_file_entry_initialize(
     libewf_single_file_entry_t **single_file_entry,
     liberror_error_t **error );

int libewf_single_file_entry_free(
     intptr_t *single_file_entry,
     liberror_error_t **error );

int libewf_single_file_entry_clone(
     intptr_t **destination,
     intptr_t *source,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


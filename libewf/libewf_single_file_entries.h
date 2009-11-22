/*
 * Single file entries functions
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

#if !defined( _LIBEWF_SINGLE_FILE_ENTRIES_H )
#define _LIBEWF_SINGLE_FILE_ENTRIES_H

#include <common.h>
#include <types.h>

#include "libewf_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_single_file_entries_parse_file_entries_string(
     libewf_character_t *file_entries_string,
     size_t file_entries_string_size,
     liberror_error_t **error );

int libewf_single_file_entries_parse_ltree(
     uint8_t *ltree,
     size_t ltree_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


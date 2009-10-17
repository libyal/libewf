/*
 * Compression handling functions
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

#if !defined( _LIBEWF_COMPRESS_H )
#define _LIBEWF_COMPRESS_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

int libewf_compress(
     uint8_t *compressed_data,
     size_t *compressed_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_size,
     int8_t compression_level,
     liberror_error_t **error );

int libewf_uncompress(
     uint8_t *uncompressed_data,
     size_t *uncompressed_size,
     uint8_t *compressed_data,
     size_t compressed_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


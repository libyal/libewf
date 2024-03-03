/*
 * The line reader functions
 *
 * Copyright (C) 2006-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_LINE_READER_H )
#define _LIBEWF_LINE_READER_H

#include <common.h>
#include <types.h>

#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libfdata.h"
#include "libewf_libhmac.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libewf_line_reader libewf_line_reader_t;

struct libewf_line_reader
{
	/* The data stream
	 */
	libfdata_stream_t *data_stream;

	/* The file IO pool
	 */
	libbfio_pool_t *file_io_pool;

	/* The stream offset
	 */
	off64_t stream_offset;

	/* The stream size
	 */
	size64_t stream_size;

	/* The buffer
	 */
	uint8_t *buffer;

	/* The buffer size
	 */
	size_t buffer_size;

	/* The buffer offset
	 */
	size_t buffer_offset;

	/* The UTF-8 string
	 */
	uint8_t *utf8_string;

	/* The UTF-8 string size
	 */
	size_t utf8_string_size;

	/* The line offset
	 */
	off64_t line_offset;

	/* The line index
	 */
	int line_index;

	/* The MD5 context
	 */
	libhmac_md5_context_t *md5_context;
};

int libewf_line_reader_initialize(
     libewf_line_reader_t **line_reader,
     libfdata_stream_t *data_stream,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

int libewf_line_reader_free(
     libewf_line_reader_t **line_reader,
     libcerror_error_t **error );

int libewf_line_reader_read_data(
     libewf_line_reader_t *line_reader,
     const uint8_t **line_data,
     size_t *line_data_size,
     libcerror_error_t **error );

int libewf_line_reader_read_utf8_string(
     libewf_line_reader_t *line_reader,
     uint8_t **utf8_string,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libewf_line_reader_finalize(
     libewf_line_reader_t *line_reader,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEWF_LINE_READER_H ) */


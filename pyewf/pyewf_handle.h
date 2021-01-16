/*
 * Python object wrapper of libewf_handle_t
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (C) 2008-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYEWF_HANDLE_H )
#define _PYEWF_HANDLE_H

#include <common.h>
#include <types.h>

#include "pyewf_libbfio.h"
#include "pyewf_libcerror.h"
#include "pyewf_libewf.h"
#include "pyewf_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyewf_handle pyewf_handle_t;

struct pyewf_handle
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libewf handle
	 */
	libewf_handle_t *handle;

	/* The file IO pool
	 */
	libbfio_pool_t *file_io_pool;
};

extern PyMethodDef pyewf_handle_object_methods[];
extern PyTypeObject pyewf_handle_type_object;

int pyewf_handle_init(
     pyewf_handle_t *pyewf_handle );

void pyewf_handle_free(
      pyewf_handle_t *pyewf_handle );

PyObject *pyewf_handle_signal_abort(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_open(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_open_file_objects(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_close(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_read_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_read_buffer_at_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_write_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_write_buffer_at_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_seek_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_get_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_root_file_entry(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_media_size(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_sectors_per_chunk(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_bytes_per_sector(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_number_of_sectors(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_chunk_size(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_error_granularity(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_compression_method(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_media_type(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_media_flags(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_format(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_header_codepage(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

int pyewf_handle_set_header_codepage_from_string(
     pyewf_handle_t *pyewf_handle,
     const char *codepage_string );

PyObject *pyewf_handle_set_header_codepage(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

int pyewf_handle_set_header_codepage_setter(
     pyewf_handle_t *pyewf_handle,
     PyObject *string_object,
     void *closure );

PyObject *pyewf_handle_get_header_value(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_get_header_values(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

PyObject *pyewf_handle_get_hash_value(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_handle_get_hash_values(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEWF_HANDLE_H ) */


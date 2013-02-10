/*
 * Python object definition of the libewf file entry
 *
 * Copyright (c) 2008-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYEWF_FILE_ENTRY_H )
#define _PYEWF_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "pyewf_handle.h"
#include "pyewf_libcerror.h"
#include "pyewf_libewf.h"
#include "pyewf_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyewf_file_entry pyewf_file_entry_t;

struct pyewf_file_entry
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libewf file entry
	 */
	libewf_file_entry_t *file_entry;

	/* The handle object
	 */
	pyewf_handle_t *handle_object;
};

extern PyMethodDef pyewf_file_entry_object_methods[];
extern PyTypeObject pyewf_file_entry_type_object;

PyObject *pyewf_file_entry_new(
           libewf_file_entry_t *file_entry,
           pyewf_handle_t *handle_object );

int pyewf_file_entry_init(
     pyewf_file_entry_t *pyewf_file_entry );

void pyewf_file_entry_free(
      pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_read_buffer(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_file_entry_read_random(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_file_entry_seek_offset(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyewf_file_entry_get_offset(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_size(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_creation_time(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_creation_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_modification_time(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_modification_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_access_time(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_access_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_entry_modification_time(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_entry_modification_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_name(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_hash_value_md5(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_number_of_sub_file_entries(
           pyewf_file_entry_t *pyewf_file_entry );

PyObject *pyewf_file_entry_get_sub_file_entry_by_index(
           pyewf_file_entry_t *pyewf_file_entry,
           int sub_file_entry_index );

PyObject *pyewf_file_entry_get_sub_file_entry(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif


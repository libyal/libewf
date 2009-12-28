/*
 * Python object definition of the libewf handle
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>
 *
 * Integration into libewf package by Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _PYEWF_HANDLE_H )
#define _PYEWF_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

/* Fix HAVE_FSTAT define in pyport.h
 */
#undef HAVE_FSTAT

#include <Python.h>

#include <libewf.h>

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
};

extern PyMethodDef pyewf_handle_object_methods[];
extern PyTypeObject pyewf_handle_type_object;

int pyewf_handle_initialize(
     pyewf_handle_t *pyewf_handle,
     liberror_error_t **error );

void pyewf_handle_free(
      pyewf_handle_t *pyewf_handle );

PyObject* pyewf_handle_signal_abort(
           pyewf_handle_t *pyewf_handle );

PyObject* pyewf_handle_open(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_handle_close(
           pyewf_handle_t *pyewf_handle );

PyObject* pyewf_handle_read_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_handle_read_random(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_handle_write_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_handle_write_random(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_handle_seek_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_handle_get_offset(
           pyewf_handle_t *pyewf_handle );

#if defined( __cplusplus )
}
#endif

#endif

/*
 * File Object definition for libewf Python bindings
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

#if !defined( _PYEWF_FILE_H )
#define _PYEWF_FILE_H

#include <common.h>
#include <types.h>

/* Fix HAVE_FSTAT define in pyport.h
 */
#undef HAVE_FSTAT

#include <Python.h>

#include <libewf.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyewf_file pyewf_file_t;

struct pyewf_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libewf handle
	 */
	libewf_handle_t *handle;

	/* The read offset
	 */
	off64_t read_offset;

	/* The media size
	 */
	size64_t media_size;
};

int pyewf_file_initialize(
     pyewf_file_t *pyewf_file,
     PyObject *arguments,
     PyObject *keywords );

void pyewf_file_free(
      pyewf_file_t *pyewf_file );

PyObject* pyewf_file_close(
           pyewf_file_t *pyewf_file );

PyObject* pyewf_file_read(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_file_seek_offset(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_file_get_offset(
           pyewf_file_t *pyewf_file );

PyObject* pyewf_file_get_header_value(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject* pyewf_file_get_header_values(
           pyewf_file_t *pyewf_file );

#if defined( __cplusplus )
}
#endif

#endif


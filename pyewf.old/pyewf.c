/*
 * Python bindings for libewf (pyewf)
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* Fix HAVE_FSTAT define in pyport.h
 */
#undef HAVE_FSTAT

#include <Python.h>

#include "pyewf.h"
#include "pyewf_file.h"

PyMethodDef pyewf_object_methods[] = {
	{ "close",
	  (PyCFunction) pyewf_file_close,
	  METH_NOARGS,
	 "Close the EWF file(s)" },

	{ "read",
	  (PyCFunction) pyewf_file_read,
	  METH_VARARGS | METH_KEYWORDS,
	  "Read media data from EWF file(s)" },

	{ "seek",
	  (PyCFunction) pyewf_file_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "Seek within the media data" },

	{ "tell",
	  (PyCFunction) pyewf_file_get_offset,
	  METH_NOARGS,
	  "Return the current offset within the media data" },

	{ "get_header_value",
	  (PyCFunction) pyewf_file_get_header_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "Retrieve a header value by its name" },

	{ "get_header_values",
	  (PyCFunction) pyewf_file_get_header_values,
	  METH_NOARGS,
	  "Retrieve all header values" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyTypeObject pyewf_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyewf.file",
	/* tp_basicsize */
	sizeof(pyewf_file_t),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyewf_file_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"libewf File Object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyewf_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyewf_file_initialize,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

PyObject* pyewf_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *files             = NULL;
	PyObject *file_arguments    = NULL;
	PyObject *file_keywords     = NULL;
	pyewf_file_t *pyewf_file    = NULL;
	static char *keyword_list[] = { "files", NULL };
	int result                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &files ) == 0 )
	{
		return( NULL );
	}
	/* Create an pyewf_file object and return it
	 */
	file_arguments = PyTuple_New( 0 );

	if( file_arguments == NULL )
	{
		return( NULL );
	}
	file_keywords = Py_BuildValue(
	                 "{sO}",
	                 "files",
	                 files );

	if( file_keywords == NULL )
	{
		return( NULL );
	}
	pyewf_file = PyObject_New(
	              struct pyewf_file,
	              &pyewf_type_object );

	result = pyewf_file_initialize(
	          pyewf_file,
	          file_arguments,
	          file_keywords );

	Py_DECREF(
	 file_arguments );
	Py_DECREF(
	 file_keywords );

	if( result == -1 )
	{
		Py_DECREF(
		 pyewf_file );

		return( NULL );
	}
	return( (PyObject *) pyewf_file );
}

/* These are the module methods
 */
PyMethodDef pyewf_module_methods[] = {
	{ "open",
	  (PyCFunction)pyewf_open,
	  METH_VARARGS|METH_KEYWORDS,
	  "Open Expert Wittness Compression Format (EWF) file(s)" },

	/* Sentinel */
	{ NULL,
	  NULL,
	  0,
	  NULL}
};

/* Declarations for DLL import/export
 */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC initpyewf(
                void ) 
{
	PyObject* module          = NULL;
	PyTypeObject *type_object = NULL;

	/* Create module
	 */
	module = Py_InitModule3(
	          "pyewf",
	           pyewf_module_methods,
	           "Python libewf module." );

	/* Setup pyewf_file type
	 */
	pyewf_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyewf_type_object ) < 0 )
	{
		return;
	}
	Py_INCREF(
	 &pyewf_type_object );

	type_object = &pyewf_type_object;

	PyModule_AddObject(
	 module,
	"pyewf_file",
	(PyObject *) type_object );

	libewf_set_notify_values(
	 stderr,
	 1 );
}


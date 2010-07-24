/*
 * Python bindings module for libewf (pyewf)
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* Fix HAVE_FSTAT and HAVE_STAT defines in pyport.h
 */
#undef HAVE_FSTAT
#undef HAVE_STAT

#include <Python.h>

#include <libewf.h>

#include "pyewf.h"
#include "pyewf_handle.h"

/* The pyewf module methods
 */
PyMethodDef pyewf_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyewf_get_version,
	  METH_NOARGS,
	  "Retrieves the pyewf (libewf) version" },

	{ "get_access_flags_read",
	  (PyCFunction) pyewf_get_access_flags_read,
	  METH_NOARGS,
	  "Retrieves the read access flags" },

	{ "get_access_flags_read_write",
	  (PyCFunction) pyewf_get_access_flags_read_write,
	  METH_NOARGS,
	  "Retrieves the read and write access flags" },

	{ "get_access_flags_write",
	  (PyCFunction) pyewf_get_access_flags_write,
	  METH_NOARGS,
	  "Retrieves the write access flags" },

	{ "get_access_flags_write_resume",
	  (PyCFunction) pyewf_get_access_flags_write_resume,
	  METH_NOARGS,
	  "Retrieves the resume write access flags" },

	{ "check_file_signature",
	  (PyCFunction) pyewf_check_file_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "Checks if a file has an EWF signature" },

	{ "glob",
	  (PyCFunction) pyewf_glob,
	  METH_VARARGS | METH_KEYWORDS,
	  "Globs filenames according to the EWF segment file naming schema" },

	{ "set_notify_values",
	  (PyCFunction) pyewf_set_notify_values,
	  METH_VARARGS | METH_KEYWORDS,
	  "Sets pyewf (libewf) notification values" },

	{ "new_handle",
	  (PyCFunction) pyewf_new_handle,
	  METH_NOARGS,
	  "Creates a new pyewf handle object" },

	/* TODO these function are deprecated and will be removed after a while */

	{ "get_flags_read",
	  (PyCFunction) pyewf_get_access_flags_read,
	  METH_NOARGS,
	  "Retrieves the read open flags" },

	{ "get_flags_read_write",
	  (PyCFunction) pyewf_get_access_flags_read_write,
	  METH_NOARGS,
	  "Retrieves the read write open flags" },

	{ "get_flags_write",
	  (PyCFunction) pyewf_get_access_flags_write,
	  METH_NOARGS,
	  "Retrieves the write open flags" },

	{ "get_flags_write_resume",
	  (PyCFunction) pyewf_get_access_flags_write_resume,
	  METH_NOARGS,
	  "Retrieves the resume write open flags" },

	/* Sentinel */
	{ NULL,
	  NULL,
	  0,
	  NULL}
};

/* Retrieves the pyewf/libewf version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_get_version(
           PyObject *self )
{
	const char *errors         = NULL;
	const char *version_string = NULL;
	size_t version_string_size = 0;

	version_string = libewf_get_version();

	version_string_size = 1 + libcstring_narrow_string_length(
	                           version_string );

	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_size,
	         errors ) );
}

/* Retrieves the pyewf/libewf read access flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_get_access_flags_read(
           PyObject *self )
{
	return( PyInt_FromLong(
	         (long) libewf_get_access_flags_read() ) );
}

/* Retrieves the pyewf/libewf read write access flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_get_access_flags_read_write(
           PyObject *self )
{
	return( PyInt_FromLong(
	         (long) libewf_get_access_flags_read_write() ) );
}

/* Retrieves the pyewf/libewf write access flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_get_access_flags_write(
           PyObject *self )
{
	return( PyInt_FromLong(
	         (long) libewf_get_access_flags_write() ) );
}

/* Retrieves the pyewf/libewf resume write access flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_get_access_flags_write_resume(
           PyObject *self )
{
	return( PyInt_FromLong(
	         (long) libewf_get_access_flags_write_resume() ) );
}

/* Checks if the file has an EWF signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_check_file_signature(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	liberror_error_t *error     = NULL;
	static char *function       = "pyewf_check_file_signature";
	static char *keyword_list[] = { "filename", NULL };
	const char *filename        = NULL;
	int result                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	result = libewf_check_file_signature(
	          filename,
	          &error );

	if( result == -1 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to check file signature.",
		 function );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		return( Py_True );
	}
	return( Py_False );
}

/* Globs filenames according to the EWF segment file naming schema
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_glob(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	liberror_error_t *error     = NULL;
	static char *function       = "pyewf_glob";
	static char *keyword_list[] = { "filename", NULL };
	const char *filename        = NULL;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	/* TODO implement function */

	return( Py_None );
}

/* Sets the pyewf (libewf) notification values
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_set_notify_values(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	static char *function       = "pyewf_set_notify_values";
	static char *keyword_list[] = { "stream", "verbose", NULL };

	/* TODO implement function */

	return( Py_None );
}

/* Creates a new pyewf handle object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_new_handle(
           PyObject *self )
{
	static char *function       = "pyewf_new_handle";
	pyewf_handle_t *pyewf_handle = NULL;
	liberror_error_t *error      = NULL;

	pyewf_handle = PyObject_New(
	                struct pyewf_handle,
	                &pyewf_handle_type_object );

	if( pyewf_handle_initialize(
	     pyewf_handle,
	     &error ) != 1 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to initialize pyewf handle.",
		 function );

		liberror_error_free(
		 &error );
		Py_DECREF(
		 pyewf_handle );

		return( NULL );
	}
	return( (PyObject *) pyewf_handle );
}

/* Declarations for DLL import/export
 */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

/* Initializes the pyewf module
 */
PyMODINIT_FUNC initpyewf(
                void ) 
{
	PyObject *module                 = NULL;
	PyTypeObject *handle_type_object = NULL;

	/* Create the module
	 */
	module = Py_InitModule3(
	          "pyewf",
	           pyewf_module_methods,
	           "Python libewf module (pyewf)." );

	/* Setup the handle type object
	 */
	pyewf_handle_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyewf_handle_type_object ) < 0 )
	{
		return;
	}
	Py_INCREF(
	 &pyewf_handle_type_object );

	handle_type_object = &pyewf_handle_type_object;

	PyModule_AddObject(
	 module,
	"pyewf_handle",
	(PyObject *) handle_type_object );
}


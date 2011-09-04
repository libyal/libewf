/*
 * Python bindings module for libewf (pyewf)
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (c) 2009-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libewf.h>

#include "pyewf.h"
#include "pyewf_handle.h"
#include "pyewf_python.h"

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
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

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
		if( liberror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to check file signature.\n%s",
			 function,
			 error_string );
		}
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
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	char **filenames            = NULL;
	liberror_error_t *error     = NULL;
	PyObject *list_object       = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyewf_glob";
	static char *keyword_list[] = { "filename", NULL };
	const char *errors          = NULL;
	const char *filename        = NULL;
	size_t filename_length      = 0;
	int filename_index          = 0;
	int number_of_filenames     = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	filename_length = libcstring_narrow_string_length(
	                   filename );

	if( libewf_glob(
	     filename,
	     filename_length,
	     LIBEWF_FORMAT_UNKNOWN,
	     &filenames,
	     &number_of_filenames,
	     &error ) != 1 )
	{
		if( liberror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to glob filenames.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to glob filenames.\n%s",
			 function,
			 error_string );
		}
		liberror_error_free(
		 &error );

		return( NULL );
	}
	list_object = PyList_New(
	               (Py_ssize_t) number_of_filenames );

	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		filename_length = libcstring_narrow_string_length(
		                   filenames[ filename_index ] );

		string_object = PyUnicode_DecodeUTF8(
		                 filenames[ filename_index ],
		                 filename_length,
		                 errors );

		if( string_object == NULL )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to convert UTF-8 filename: %d into Unicode.",
			 function,
			 filename_index );

			libewf_glob_free(
			 filenames,
			 number_of_filenames,
			 NULL );

			Py_DecRef(
			 list_object );

			return( NULL );
		}
		if( PyList_SetItem(
		     list_object,
		     (Py_ssize_t) filename_index,
		     string_object ) != 0 )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to set filename: %d in list.",
			 function,
			 filename_index );

			libewf_glob_free(
			 filenames,
			 number_of_filenames,
			 NULL );

			Py_DecRef(
			 string_object );
			Py_DecRef(
			 list_object );

			return( NULL );
		}
	}
	if( libewf_glob_free(
	     filenames,
	     number_of_filenames,
	     &error ) != 1 )
	{
		if( liberror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free globbed filenames.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free globbed filenames.\n%s",
			 function,
			 error_string );
		}
		liberror_error_free(
		 &error );

		Py_DecRef(
		 list_object );

		return( NULL );
	}
	return( list_object );
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
	Py_IncRef(
	 (PyObject * ) &pyewf_handle_type_object );

	handle_type_object = &pyewf_handle_type_object;

	PyModule_AddObject(
	 module,
	"pyewf_handle",
	(PyObject *) handle_type_object );
}


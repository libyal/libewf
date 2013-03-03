/*
 * Python bindings module for libewf (pyewf)
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include "pyewf.h"
#include "pyewf_file_entries.h"
#include "pyewf_file_entry.h"
#include "pyewf_file_object_io_handle.h"
#include "pyewf_handle.h"
#include "pyewf_libcerror.h"
#include "pyewf_libcstring.h"
#include "pyewf_libewf.h"
#include "pyewf_python.h"

#if !defined( LIBEWF_HAVE_BFIO )
LIBEWF_EXTERN \
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libewf_error_t **error );
#endif

/* The pyewf module methods
 */
PyMethodDef pyewf_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyewf_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_file_signature",
	  (PyCFunction) pyewf_check_file_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a file has an Expert Witness Compression Format (EWF) signature." },

	{ "check_file_signature_file_object",
	  (PyCFunction) pyewf_check_file_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature_file_object(filename) -> Boolean\n"
	  "\n"
	  "Checks if a file has an Expert Witness Compression Format (EWF) signature using a file-like object." },

	{ "glob",
	  (PyCFunction) pyewf_glob,
	  METH_VARARGS | METH_KEYWORDS,
	  "glob(filename) -> Object\n"
	  "\n"
	  "Globs filenames according to the Expert Witness Compression Format (EWF) segment file naming schema\n"
	  "based on the extension (e.g. E01) and returns a sequence (list) of all the segment filenames." },

	{ "open",
	  (PyCFunction) pyewf_handle_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filenames, mode='r') -> Object\n"
	  "\n"
          "Opens file(s) from a sequence (list) of all the segment filenames.\n"
          "Use pyewf.glob() to determine the segment filenames from first (E01)." },

/* TODO: open file-like object using pool - list of file objects */

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
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	Py_BEGIN_ALLOW_THREADS

	version_string = libewf_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = libcstring_narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Checks if the file has an Expert Witness Compression Format (EWF) signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_check_file_signature(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
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
	Py_BEGIN_ALLOW_THREADS

	result = libewf_check_file_signature(
	          filename,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		if( libcerror_error_backtrace_sprint(
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
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		return( Py_True );
	}
	return( Py_False );
}

/* Checks if the file has a Windows Event Log (EWF) file signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_check_file_signature_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyewf_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pyewf_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		if( libcerror_error_backtrace_sprint(
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
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		return( Py_True );
	}
	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Globs filenames according to the Expert Witness Compression Format (EWF) segment file naming schema
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_glob(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	char **filenames            = NULL;
	libcerror_error_t *error     = NULL;
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
		if( libcerror_error_backtrace_sprint(
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
		libcerror_error_free(
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

		/* Pass the string length to PyUnicode_DecodeUTF8
		 * otherwise it makes the end of string character is part
		 * of the string
		 */
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
		if( libcerror_error_backtrace_sprint(
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
		libcerror_error_free(
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
	PyObject *module                       = NULL;
	PyTypeObject *file_entries_type_object = NULL;
	PyTypeObject *file_entry_type_object   = NULL;
	PyTypeObject *handle_type_object       = NULL;
	PyGILState_STATE gil_state             = 0;

        /* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
	module = Py_InitModule3(
	          "pyewf",
	          pyewf_module_methods,
	          "Python libewf module (pyewf)." );

	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the handle type object
	 */
	pyewf_handle_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyewf_handle_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject * ) &pyewf_handle_type_object );

	handle_type_object = &pyewf_handle_type_object;

	PyModule_AddObject(
	 module,
	 "handle",
	 (PyObject *) handle_type_object );

	/* Setup the file entry type object
	 */
	pyewf_file_entry_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyewf_file_entry_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyewf_file_entry_type_object );

	file_entry_type_object = &pyewf_file_entry_type_object;

	PyModule_AddObject(
	 module,
	"file_entry",
	(PyObject *) file_entry_type_object );

	/* Setup the file entries type object
	 */
	pyewf_file_entries_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyewf_file_entries_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyewf_file_entries_type_object );

	file_entries_type_object = &pyewf_file_entries_type_object;

	PyModule_AddObject(
	 module,
	"_file_entries",
	(PyObject *) file_entries_type_object );

on_error:
	PyGILState_Release(
	 gil_state );
}


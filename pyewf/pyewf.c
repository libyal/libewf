/*
 * Python bindings module for libewf (pyewf)
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (C) 2008-2020, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyewf.h"
#include "pyewf_compression_methods.h"
#include "pyewf_error.h"
#include "pyewf_file_entries.h"
#include "pyewf_file_entry.h"
#include "pyewf_file_object_io_handle.h"
#include "pyewf_handle.h"
#include "pyewf_libcerror.h"
#include "pyewf_libewf.h"
#include "pyewf_media_flags.h"
#include "pyewf_media_types.h"
#include "pyewf_python.h"
#include "pyewf_unused.h"

#if !defined( LIBEWF_HAVE_BFIO )

LIBEWF_EXTERN \
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libewf_error_t **error );

#endif /* !defined( LIBEWF_HAVE_BFIO ) */

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
	  "check_file_signature_file_object(file_object) -> Boolean\n"
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
	  (PyCFunction) pyewf_open_new_handle,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filenames, mode='r') -> Object\n"
	  "\n"
          "Opens file(s) from a sequence (list) of all the segment filenames.\n"
          "Use pyewf.glob() to determine the segment filenames from first (E01)." },

	{ "open_file_objects",
	  (PyCFunction) pyewf_open_new_handle_with_file_objects,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_objects(file_objects, mode='r') -> Object\n"
	  "\n"
          "Opens file(s) from a sequence (list) of file-like objects." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pyewf/libewf version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_get_version(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYEWF_UNREFERENCED_PARAMETER( self )
	PYEWF_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libewf_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
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
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	const char *filename_narrow  = NULL;
	static char *function        = "pyewf_check_file_signature";
	static char *keyword_list[]  = { "filename", NULL };
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyewf_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libewf_check_file_signature_wide(
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyewf_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libewf_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

		if( result == -1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyewf_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libewf_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if the file has an Expert Witness Compression Format (EWF) signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_check_file_signature_file_object(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	static char *function            = "pyewf_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYEWF_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
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
		pyewf_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

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
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check file signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

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
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	char **filenames_narrow          = NULL;
	libcerror_error_t *error         = NULL;
	PyObject *filename_string_object = NULL;
	PyObject *list_object            = NULL;
	PyObject *string_object          = NULL;
	static char *function            = "pyewf_glob";
	static char *keyword_list[]      = { "filename", NULL };
	const char *errors               = NULL;
	const char *filename_narrow      = NULL;
	size_t filename_length           = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames_wide         = NULL;
	const wchar_t *filename_wide     = NULL;
#else
	PyObject *utf8_string_object     = NULL;
#endif

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyewf_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );

		filename_length = wide_string_length(
		                   filename_wide );

		Py_BEGIN_ALLOW_THREADS

		result = libewf_glob_wide(
			  filename_wide,
			  filename_length,
			  LIBEWF_FORMAT_UNKNOWN,
			  &filenames_wide,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyewf_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		filename_length = narrow_string_length(
		                   filename_narrow );

		Py_BEGIN_ALLOW_THREADS

		result = libewf_glob(
			  filename_narrow,
			  filename_length,
			  LIBEWF_FORMAT_UNKNOWN,
			  &filenames_narrow,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to glob filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		list_object = PyList_New(
			       (Py_ssize_t) number_of_filenames );

		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			filename_length = wide_string_length(
					   filenames_wide[ filename_index ] );

			filename_string_object = PyUnicode_FromWideChar(
						  filenames_wide[ filename_index ],
						  filename_length );
#else
			filename_length = narrow_string_length(
			                   filenames_narrow[ filename_index ] );

			/* Pass the string length to PyUnicode_DecodeUTF8
			 * otherwise it makes the end of string character is part
			 * of the string
			 */
			filename_string_object = PyUnicode_DecodeUTF8(
			                          filenames_narrow[ filename_index ],
			                          filename_length,
			                          errors );
#endif
			if( filename_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert filename: %d into Unicode.",
				 function,
				 filename_index );

				goto on_error;
			}
			if( PyList_SetItem(
			     list_object,
			     (Py_ssize_t) filename_index,
			     filename_string_object ) != 0 )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to set filename: %d in list.",
				 function,
				 filename_index );

				goto on_error;
			}
			filename_string_object = NULL;
		}
		Py_BEGIN_ALLOW_THREADS

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libewf_glob_wide_free(
			  filenames_wide,
			  number_of_filenames,
			  &error );
#else
		result = libewf_glob_free(
			  filenames_narrow,
			  number_of_filenames,
			  &error );
#endif

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free globbed filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		return( list_object );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyewf_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );
#endif
		filename_length = narrow_string_length(
		                   filename_narrow );

		Py_BEGIN_ALLOW_THREADS

		result = libewf_glob(
			  filename_narrow,
			  filename_length,
			  LIBEWF_FORMAT_UNKNOWN,
			  &filenames_narrow,
			  &number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to glob filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		list_object = PyList_New(
			       (Py_ssize_t) number_of_filenames );

		for( filename_index = 0;
		     filename_index < number_of_filenames;
		     filename_index++ )
		{
			filename_length = narrow_string_length(
					   filenames_narrow[ filename_index ] );

			filename_string_object = PyUnicode_Decode(
						  filenames_narrow[ filename_index ],
						  filename_length,
						  PyUnicode_GetDefaultEncoding(),
						  errors );

			if( filename_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert filename: %d into Unicode.",
				 function,
				 filename_index );

				goto on_error;
			}
			if( PyList_SetItem(
			     list_object,
			     (Py_ssize_t) filename_index,
			     filename_string_object ) != 0 )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to set filename: %d in list.",
				 function,
				 filename_index );

				goto on_error;
			}
			filename_string_object = NULL;
		}
		Py_BEGIN_ALLOW_THREADS

		result = libewf_glob_free(
			  filenames_narrow,
			  number_of_filenames,
			  &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free globbed filenames.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		return( list_object );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

on_error:
	if( filename_string_object != NULL )
	{
		Py_DecRef(
		 filename_string_object );
	}
	if( list_object != NULL )
	{
		Py_DecRef(
		 list_object );
	}
	if( filenames_narrow != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libewf_glob_free(
		 filenames_narrow,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( filenames_wide != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libewf_glob_wide_free(
		 filenames_wide,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
#endif
	return( NULL );
}

/* Creates a new handle object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_open_new_handle(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pyewf_handle_t *pyewf_handle = NULL;
	static char *function        = "pyewf_open_new_handle";

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pyewf_handle = PyObject_New(
	              struct pyewf_handle,
	              &pyewf_handle_type_object );

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( pyewf_handle_init(
	     pyewf_handle ) != 0 )
	{
		goto on_error;
	}
	if( pyewf_handle_open(
	     pyewf_handle,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pyewf_handle );

on_error:
	if( pyewf_handle != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyewf_handle );
	}
	return( NULL );
}

/* Creates a new handle object and opens it using file-like objects
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_open_new_handle_with_file_objects(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pyewf_handle_t *pyewf_handle = NULL;
	static char *function        = "pyewf_open_new_handle_with_file_objects";

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pyewf_handle = PyObject_New(
	                struct pyewf_handle,
	                &pyewf_handle_type_object );

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	if( pyewf_handle_init(
	     pyewf_handle ) != 0 )
	{
		goto on_error;
	}
	if( pyewf_handle_open_file_objects(
	     pyewf_handle,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pyewf_handle );

on_error:
	if( pyewf_handle != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyewf_handle );
	}
	return( NULL );
}

#if PY_MAJOR_VERSION >= 3

/* The pyewf module definition
 */
PyModuleDef pyewf_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pyewf",
	/* m_doc */
	"Python libewf module (pyewf).",
	/* m_size */
	-1,
	/* m_methods */
	pyewf_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pyewf module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pyewf(
                void )
#else
PyMODINIT_FUNC initpyewf(
                void )
#endif
{
	PyObject *module           = NULL;
	PyGILState_STATE gil_state = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libewf_notify_set_stream(
	 stderr,
	 NULL );
	libewf_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pyewf_module_definition );
#else
	module = Py_InitModule3(
	          "pyewf",
	          pyewf_module_methods,
	          "Python libewf module (pyewf)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
#if PY_VERSION_HEX < 0x03070000
	PyEval_InitThreads();
#endif
	gil_state = PyGILState_Ensure();

	/* Setup the compression methods type object
	 */
	pyewf_compression_methods_type_object.tp_new = PyType_GenericNew;

	if( pyewf_compression_methods_init_type(
	     &pyewf_compression_methods_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyewf_compression_methods_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyewf_compression_methods_type_object );

	PyModule_AddObject(
	 module,
	 "compression_methods",
	 (PyObject *) &pyewf_compression_methods_type_object );

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

	PyModule_AddObject(
	 module,
	 "file_entries",
	 (PyObject *) &pyewf_file_entries_type_object );

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

	PyModule_AddObject(
	 module,
	 "file_entry",
	 (PyObject *) &pyewf_file_entry_type_object );

	/* Setup the handle type object
	 */
	pyewf_handle_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyewf_handle_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyewf_handle_type_object );

	PyModule_AddObject(
	 module,
	 "handle",
	 (PyObject *) &pyewf_handle_type_object );

	/* Setup the media flags type object
	 */
	pyewf_media_flags_type_object.tp_new = PyType_GenericNew;

	if( pyewf_media_flags_init_type(
	     &pyewf_media_flags_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyewf_media_flags_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyewf_media_flags_type_object );

	PyModule_AddObject(
	 module,
	 "media_flags",
	 (PyObject *) &pyewf_media_flags_type_object );

	/* Setup the media types type object
	 */
	pyewf_media_types_type_object.tp_new = PyType_GenericNew;

	if( pyewf_media_types_init_type(
	     &pyewf_media_types_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyewf_media_types_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyewf_media_types_type_object );

	PyModule_AddObject(
	 module,
	 "media_types",
	 (PyObject *) &pyewf_media_types_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}


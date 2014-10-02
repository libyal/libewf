/*
 * Python bindings module for libewf (pyewf)
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (c) 2008-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include "pyewf_compression_methods.h"
#include "pyewf_error.h"
#include "pyewf_file_entries.h"
#include "pyewf_file_entry.h"
#include "pyewf_file_object_io_handle.h"
#include "pyewf_handle.h"
#include "pyewf_libcerror.h"
#include "pyewf_libcstring.h"
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

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )

/* Checks if the file has an Expert Witness Compression Format (EWF) signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_check_file_signature(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *exception_string    = NULL;
	PyObject *exception_traceback = NULL;
	PyObject *exception_type      = NULL;
	PyObject *exception_value     = NULL;
	PyObject *string_object       = NULL;
	libcerror_error_t *error      = NULL;
	static char *function         = "pyewf_check_file_signature";
	static char *keyword_list[]   = { "filename", NULL };
	const wchar_t *filename_wide  = NULL;
	const char *filename_narrow   = NULL;
	char *error_string            = NULL;
	int result                    = 0;

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
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
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
		                    exception_value );

		error_string = PyString_AsString(
		                exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libewf_check_file_signature_wide(
		          filename_wide,
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
	PyErr_Clear();

	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
				    exception_value );

		error_string = PyString_AsString(
				exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_narrow = PyString_AsString(
				   string_object );

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
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Checks if the file has an Expert Witness Compression Format (EWF) signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_check_file_signature(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyewf_check_file_signature";
	static char *keyword_list[] = { "filename", NULL };
	const char *filename        = NULL;
	int result                  = 0;

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
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

#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

/* Checks if the file has a Windows Event Log (EWF) file signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_check_file_signature_file_object(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyewf_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYEWF_UNREFERENCED_PARAMETER( self )

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

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )

/* Globs filenames according to the Expert Witness Compression Format (EWF) segment file naming schema
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_glob(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	wchar_t **filenames_wide         = NULL;
	char **filenames_narrow          = NULL;
	libcerror_error_t *error         = NULL;
	PyObject *exception_string       = NULL;
	PyObject *exception_traceback    = NULL;
	PyObject *exception_type         = NULL;
	PyObject *exception_value        = NULL;
	PyObject *filename_string_object = NULL;
	PyObject *list_object            = NULL;
	PyObject *string_object          = NULL;
	const wchar_t *filename_wide     = NULL;
	static char *function            = "pyewf_glob";
	static char *keyword_list[]      = { "filename", NULL };
	const char *errors               = NULL;
	const char *filename_narrow      = NULL;
	char *error_string               = NULL;
	size_t filename_length           = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
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
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
		                    exception_value );

		error_string = PyString_AsString(
		                exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );

		filename_length = libcstring_wide_string_length(
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
			filename_length = libcstring_wide_string_length(
					   filenames_wide[ filename_index ] );

			filename_string_object = PyUnicode_FromWideChar(
						  filenames_wide[ filename_index ],
						  filename_length );

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

		result = libewf_glob_wide_free(
			  filenames_wide,
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
	PyErr_Clear();

	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
				    exception_value );

		error_string = PyString_AsString(
				exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		goto on_error;
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_narrow = PyString_AsString(
				   string_object );

		filename_length = libcstring_narrow_string_length(
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
			filename_length = libcstring_narrow_string_length(
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
	 "%s: unsupported string object type",
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
	if( filenames_wide != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libewf_glob_wide_free(
		 filenames_wide,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
	return( NULL );
}

#else

/* Globs filenames according to the Expert Witness Compression Format (EWF) segment file naming schema
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_glob(
           PyObject *self PYEWF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	char **filenames                 = NULL;
	libcerror_error_t *error         = NULL;
	PyObject *filename_string_object = NULL;
	PyObject *list_object            = NULL;
	static char *function            = "pyewf_glob";
	static char *keyword_list[]      = { "filename", NULL };
	const char *errors               = NULL;
	const char *filename             = NULL;
	size_t filename_length           = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	PYEWF_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
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

	Py_BEGIN_ALLOW_THREADS

	result = libewf_glob(
	          filename,
	          filename_length,
	          LIBEWF_FORMAT_UNKNOWN,
	          &filenames,
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
		filename_length = libcstring_narrow_string_length(
		                   filenames[ filename_index ] );

		/* Pass the string length to PyUnicode_DecodeUTF8
		 * otherwise it makes the end of string character is part
		 * of the string
		 */
		filename_string_object = PyUnicode_DecodeUTF8(
		                          filenames[ filename_index ],
		                          filename_length,
		                          errors );

		if( filename_string_object == NULL )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to convert UTF-8 filename: %d into Unicode.",
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
	          filenames,
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
	if( filenames != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		libewf_glob_free(
		 filenames,
		 number_of_filenames,
		 NULL );

		Py_END_ALLOW_THREADS
	}
	return( NULL );
}

#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

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
	PyObject *module                              = NULL;
	PyTypeObject *compression_methods_type_object = NULL;
	PyTypeObject *file_entries_type_object        = NULL;
	PyTypeObject *file_entry_type_object          = NULL;
	PyTypeObject *handle_type_object              = NULL;
	PyTypeObject *media_flags_type_object         = NULL;
	PyTypeObject *media_types_type_object         = NULL;
	PyGILState_STATE gil_state                    = 0;

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

	compression_methods_type_object = &pyewf_compression_methods_type_object;

	PyModule_AddObject(
	 module,
	 "compression_methods",
	 (PyObject *) compression_methods_type_object );

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

	media_types_type_object = &pyewf_media_types_type_object;

	PyModule_AddObject(
	 module,
	 "media_types",
	 (PyObject *) media_types_type_object );

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

	media_flags_type_object = &pyewf_media_flags_type_object;

	PyModule_AddObject(
	 module,
	 "media_flags",
	 (PyObject *) media_flags_type_object );

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


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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

#include "pyewf.h"
#include "pyewf_datetime.h"
#include "pyewf_file_entries.h"
#include "pyewf_file_entry.h"
#include "pyewf_libcerror.h"
#include "pyewf_libcstring.h"
#include "pyewf_libewf.h"
#include "pyewf_metadata.h"
#include "pyewf_python.h"

PyMethodDef pyewf_file_entry_object_methods[] = {

	/* Functions to access the file entry data */

	{ "read_buffer",
	  (PyCFunction) pyewf_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of file entry data." },

	{ "read_random",
	  (PyCFunction) pyewf_file_entry_read_random,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_random(size, offset) -> String\n"
	  "\n"
	  "Reads a buffer of file entry data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyewf_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the file entry data." },

	{ "get_offset",
	  (PyCFunction) pyewf_file_entry_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Long\n"
	  "\n"
	  "Returns the current offset within the file entry data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pyewf_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> String\n"
	  "\n"
	  "Reads a buffer of file entry data." },

	{ "seek",
	  (PyCFunction) pyewf_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the file entry data." },

	{ "tell",
	  (PyCFunction) pyewf_file_entry_get_offset,
	  METH_NOARGS,
	  "tell() -> Long\n"
	  "\n"
	  "Returns the current offset within the file entry data." },

	/* Functions to access the metadata */

	{ "get_size",
	  (PyCFunction) pyewf_file_entry_get_size,
	  METH_NOARGS,
	  "get_size() -> Long\n"
	  "\n"
	  "Returns the size of the file entry data." },

	{ "get_creation_time",
	  (PyCFunction) pyewf_file_entry_get_creation_time,
	  METH_NOARGS,
	  "get_creation_time() -> Datetime\n"
	  "\n"
	  "Returns the creation date and time of the file entry." },

	{ "get_creation_time_as_integer",
	  (PyCFunction) pyewf_file_entry_get_creation_time_as_integer,
	  METH_NOARGS,
	  "get_creation_time_as_integer() -> Long\n"
	  "\n"
	  "Returns the creation date and time as a 32-bit integer containing a POSIX timestamp value." },

	{ "get_modification_time",
	  (PyCFunction) pyewf_file_entry_get_modification_time,
	  METH_NOARGS,
	  "get_modification_time() -> Datetime\n"
	  "\n"
	  "Returns the modification date and time of the file entry." },

	{ "get_modification_time_as_integer",
	  (PyCFunction) pyewf_file_entry_get_modification_time_as_integer,
	  METH_NOARGS,
	  "get_modification_time_as_integer() -> Long\n"
	  "\n"
	  "Returns the modification date and time as a 32-bit integer containing a POSIX timestamp value." },

	{ "get_access_time",
	  (PyCFunction) pyewf_file_entry_get_access_time,
	  METH_NOARGS,
	  "get_access_time() -> Datetime\n"
	  "\n"
	  "Returns the access date and time of the file entry." },

	{ "get_access_time_as_integer",
	  (PyCFunction) pyewf_file_entry_get_access_time_as_integer,
	  METH_NOARGS,
	  "get_access_time_as_integer() -> Long\n"
	  "\n"
	  "Returns the access date and time as a 32-bit integer containing a POSIX timestamp value." },

	{ "get_entry_modification_time",
	  (PyCFunction) pyewf_file_entry_get_entry_modification_time,
	  METH_NOARGS,
	  "get_entry_modification_time() -> Datetime\n"
	  "\n"
	  "Returns the entry modification date and time of the file entry." },

	{ "get_entry_modification_time_as_integer",
	  (PyCFunction) pyewf_file_entry_get_entry_modification_time_as_integer,
	  METH_NOARGS,
	  "get_entry_modification_time_as_integer() -> Long\n"
	  "\n"
	  "Returns the entry modification date and time as a 32-bit integer containing a POSIX timestamp value." },

	{ "get_name",
	  (PyCFunction) pyewf_file_entry_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Returns the name of the file entry." },

	{ "get_hash_value_md5",
	  (PyCFunction) pyewf_file_entry_get_hash_value_md5,
	  METH_NOARGS,
	  "get_hash_value_md5() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the MD5 hash of the file entry data." },

	/* Functions to access the sub file entries */

	{ "get_number_of_sub_file_entries",
	  (PyCFunction) pyewf_file_entry_get_number_of_sub_file_entries,
	  METH_NOARGS,
	  "get_number_of_sub_file_entries() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub file entries." },

	{ "get_sub_file_entry",
	  (PyCFunction) pyewf_file_entry_get_sub_file_entry,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_file_entry() -> Object\n"
	  "\n"
	  "Retrieves a specific sub file entry." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyewf_file_entry_object_get_set_definitions[] = {

	{ "size",
	  (getter) pyewf_file_entry_get_size,
	  (setter) 0,
	  "The size of the file entry data.",
	  NULL },

	{ "creation_time",
	  (getter) pyewf_file_entry_get_creation_time,
	  (setter) 0,
	  "The creation date and time of the file entry.",
	  NULL },

	{ "modification_time",
	  (getter) pyewf_file_entry_get_modification_time,
	  (setter) 0,
	  "The modification date and time of the file entry.",
	  NULL },

	{ "access_time",
	  (getter) pyewf_file_entry_get_access_time,
	  (setter) 0,
	  "The access date and time of the file entry.",
	  NULL },

	{ "entry_modification_time",
	  (getter) pyewf_file_entry_get_entry_modification_time,
	  (setter) 0,
	  "The entry modification date and time of the file entry.",
	  NULL },

	{ "name",
	  (getter) pyewf_file_entry_get_name,
	  (setter) 0,
	  "The name of the file entry.",
	  NULL },

	{ "md5_hash_value",
	  (getter) pyewf_file_entry_get_hash_value_md5,
	  (setter) 0,
	  "The MD5 hash of the file entry data.",
	  NULL },

	{ "number_of_sub_file_entries",
	  (getter) pyewf_file_entry_get_number_of_sub_file_entries,
	  (setter) 0,
	  "The number of sub file entries.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyewf_file_entry_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyewf.file_entry",
	/* tp_basicsize */
	sizeof( pyewf_file_entry_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyewf_file_entry_free,
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
	"pyewf file entry object (wraps libewf_file_entry_t)",
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
	pyewf_file_entry_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyewf_file_entry_object_get_set_definitions,
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
	(initproc) pyewf_file_entry_init,
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

/* Creates a new pyewf file_entry object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_new(
           libewf_file_entry_t *file_entry,
           pyewf_handle_t *handle_object )
{
	pyewf_file_entry_t *pyewf_file_entry = NULL;
	static char *function                = "pyewf_file_entry_new";

	if( file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	pyewf_file_entry = PyObject_New(
	                    struct pyewf_file_entry,
	                    &pyewf_file_entry_type_object );

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file_entry.",
		 function );

		goto on_error;
	}
	if( pyewf_file_entry_init(
	     pyewf_file_entry ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file_entry.",
		 function );

		goto on_error;
	}
	pyewf_file_entry->file_entry    = file_entry;
	pyewf_file_entry->handle_object = handle_object;

	Py_IncRef(
	 (PyObject *) pyewf_file_entry->handle_object );

	return( (PyObject *) pyewf_file_entry );

on_error:
	if( pyewf_file_entry != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyewf_file_entry );
	}
	return( NULL );
}

/* Intializes a file_entry object
 * Returns 0 if successful or -1 on error
 */
int pyewf_file_entry_init(
     pyewf_file_entry_t *pyewf_file_entry )
{
	static char *function = "pyewf_file_entry_init";

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file_entry.",
		 function );

		return( -1 );
	}
	/* Make sure libewf file_entry is set to NULL
	 */
	pyewf_file_entry->file_entry = NULL;

	return( 0 );
}

/* Frees a file_entry object
 */
void pyewf_file_entry_free(
      pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_file_entry_free";
	int result               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file_entry.",
		 function );

		return;
	}
	if( pyewf_file_entry->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file_entry - missing ob_type.",
		 function );

		return;
	}
	if( pyewf_file_entry->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file_entry - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyewf_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file_entry - missing libewf file_entry.",
		 function );

		return;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_free(
	          &( pyewf_file_entry->file_entry ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free file_entry.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free file_entry.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );
	}
        if( pyewf_file_entry->handle_object != NULL )
        {
                Py_DecRef(
                 (PyObject *) pyewf_file_entry->handle_object );
        }
	pyewf_file_entry->ob_type->tp_free(
	 (PyObject*) pyewf_file_entry );
}

/* Reads a buffer of file entry data from EWF file(s)
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_file_entry_read_buffer(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *result_data       = NULL;
	static char *function       = "pyewf_file_entry_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int read_size               = -1;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf file_entry.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf file_entry - missing libewf file_entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	result_data = PyString_FromStringAndSize(
	               NULL,
	               read_size );

	Py_BEGIN_ALLOW_THREADS

	read_count = libewf_file_entry_read_buffer(
	              pyewf_file_entry->file_entry,
	              PyString_AsString(
	               result_data ),
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count != (ssize_t) read_size )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( result_data );
}

/* Reads a buffer of file entry data at a specific offset from EWF file(s)
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_file_entry_read_random(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *result_data       = NULL;
	static char *function       = "pyewf_file_entry_read_random";
	static char *keyword_list[] = { "size", "offset", NULL };
	off64_t read_offset         = 0;
	ssize_t read_count          = 0;
	int read_size               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf file_entry.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf file_entry - missing libewf file_entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i|L",
	     keyword_list,
	     &read_size,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read offset value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	result_data = PyString_FromStringAndSize(
	               NULL,
	               read_size );

	Py_BEGIN_ALLOW_THREADS

	read_count = libewf_file_entry_read_random(
	              pyewf_file_entry->file_entry,
	              PyString_AsString(
	               result_data ),
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count != (ssize_t) read_size )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to read data.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( result_data );
}

/* Seeks a certain offset in the file entry data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_file_entry_seek_offset(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	static char *function       = "pyewf_file_entry_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf file_entry.",
		 function );

		return( NULL );
	}
	if( pyewf_file_entry->file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf file_entry - missing libewf file_entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list, 
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libewf_file_entry_seek_offset(
	          pyewf_file_entry->file_entry,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

	if( offset == -1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to seek offset.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to seek offset.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_offset(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_file_entry_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_offset(
	          pyewf_file_entry->file_entry,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve offset.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve offset.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if defined( HAVE_LONG_LONG )
	if( offset > (off64_t) LLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: offset value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         (long long) offset ) );
#else
	if( offset > (off64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: offset value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLong(
	         (long) offset ) );
#endif
}

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_size(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_file_entry_get_size";
	size64_t size            = 0;
	int result               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_size(
	          pyewf_file_entry->file_entry,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve size.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if defined( HAVE_LONG_LONG )
	if( size > (size64_t) LLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: size value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         (long long) size ) );
#else
	if( size > (size64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: size value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLong(
	         (long) size ) );
#endif
}

/* Retrieves the creation date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_creation_time(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyewf_file_entry_get_creation_time";
	uint32_t posix_time        = 0;
	int result                 = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_creation_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve creation time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve creation time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	date_time_object = pyewf_datetime_new_from_posix_time(
	                    posix_time );

	return( date_time_object );
}

/* Retrieves the creation date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_creation_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_file_entry_get_creation_time_as_integer";
	uint32_t posix_time      = 0;
	int result               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_creation_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve creation time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve creation time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if defined( HAVE_LONG_LONG )
	if( (uint64_t) posix_time > (uint64_t) LLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         (long long) posix_time ) );
#else
	if( (uint64_t) posix_time > (uint64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLong(
	         (long) posix_time ) );
#endif
}

/* Retrieves the modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_modification_time(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyewf_file_entry_get_modification_time";
	uint32_t posix_time        = 0;
	int result                 = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_modification_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve modification time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve modification time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	date_time_object = pyewf_datetime_new_from_posix_time(
	                    posix_time );

	return( date_time_object );
}

/* Retrieves the modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_modification_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_file_entry_get_modification_time_as_integer";
	uint32_t posix_time      = 0;
	int result               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_modification_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve modification time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve modification time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if defined( HAVE_LONG_LONG )
	if( (uint64_t) posix_time > (uint64_t) LLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         (long long) posix_time ) );
#else
	if( (uint64_t) posix_time > (uint64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLong(
	         (long) posix_time ) );
#endif
}

/* Retrieves the access date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_access_time(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyewf_file_entry_get_access_time";
	uint32_t posix_time        = 0;
	int result                 = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_access_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve access time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve access time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	date_time_object = pyewf_datetime_new_from_posix_time(
	                    posix_time );

	return( date_time_object );
}

/* Retrieves the access date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_access_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_file_entry_get_access_time_as_integer";
	uint32_t posix_time      = 0;
	int result               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_access_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve access time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve access time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if defined( HAVE_LONG_LONG )
	if( (uint64_t) posix_time > (uint64_t) LLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         (long long) posix_time ) );
#else
	if( (uint64_t) posix_time > (uint64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLong(
	         (long) posix_time ) );
#endif
}

/* Retrieves the entry modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_entry_modification_time(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyewf_file_entry_get_entry_modification_time";
	uint32_t posix_time        = 0;
	int result                 = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_entry_modification_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve entry modification time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve entry modification time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	date_time_object = pyewf_datetime_new_from_posix_time(
	                    posix_time );

	return( date_time_object );
}

/* Retrieves the entry modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_entry_modification_time_as_integer(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_file_entry_get_entry_modification_time_as_integer";
	uint32_t posix_time      = 0;
	int result               = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_entry_modification_time(
	          pyewf_file_entry->file_entry,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve entry modification time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve entry modification time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if defined( HAVE_LONG_LONG )
	if( (uint64_t) posix_time > (uint64_t) LLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         (long long) posix_time ) );
#else
	if( (uint64_t) posix_time > (uint64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: POSIX time value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLong(
	         (long) posix_time ) );
#endif
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_name(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error  = NULL;
	PyObject *string_object   = NULL;
	const char *errors        = NULL;
	uint8_t *name             = NULL;
	static char *function     = "pyewf_file_entry_get_name";
	size_t name_size          = 0;
	int result                = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_utf8_name_size(
	          pyewf_file_entry->file_entry,
	          &name_size,
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
			 "%s: unable to retrieve name size.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve name size.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	name = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * name_size );

	if( name == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_utf8_name(
		  pyewf_file_entry->file_entry,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve name.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve name.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) name,
			 (Py_ssize_t) name_size - 1,
			 errors );

	PyMem_Free(
	 name );

	return( string_object );

on_error:
	if( name != NULL )
	{
		PyMem_Free(
		 name );
	}
	return( NULL );
}

/* Retrieves the MD5 hash
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_hash_value_md5(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error  = NULL;
	PyObject *string_object   = NULL;
	const char *errors        = NULL;
	uint8_t *hash_value       = NULL;
	static char *function     = "pyewf_file_entry_get_hash_value_md5";
	size_t hash_value_size    = 33;
	int result                = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	hash_value = (uint8_t *) PyMem_Malloc(
	                          sizeof( uint8_t ) * hash_value_size );

	if( hash_value == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create hash value.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_utf8_hash_value_md5(
		  pyewf_file_entry->file_entry,
		  hash_value,
		  hash_value_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve hash value MD5.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve hash value MD5.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) hash_value,
			 (Py_ssize_t) hash_value_size - 1,
			 errors );

	PyMem_Free(
	 hash_value );

	return( string_object );

on_error:
	if( hash_value != NULL )
	{
		PyMem_Free(
		 hash_value );
	}
	return( NULL );
}

/* Retrieves the number of sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_number_of_sub_file_entries(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error       = NULL;
	static char *function          = "pyewf_file_entry_get_number_of_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_number_of_sub_file_entries(
	          pyewf_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub file entries.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub file entries.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_sub_file_entries ) );
}

/* Retrieves a specific sub file entry by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_sub_file_entry_by_index(
           pyewf_file_entry_t *pyewf_file_entry,
           int sub_file_entry_index )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error            = NULL;
	libewf_file_entry_t *sub_file_entry = NULL;
	PyObject *file_entry_object         = NULL;
	static char *function               = "pyewf_file_entry_get_sub_file_entry_by_index";
	int result                          = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_sub_file_entry(
	          pyewf_file_entry->file_entry,
	          sub_file_entry_index,
	          &sub_file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub file entry: %d.",
			 function,
			 sub_file_entry_index );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve sub file entry: %d.\n%s",
			 function,
			 sub_file_entry_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	file_entry_object = pyewf_file_entry_new(
	                     sub_file_entry,
	                     pyewf_file_entry->handle_object );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( sub_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific sub file entry
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_sub_file_entry(
           pyewf_file_entry_t *pyewf_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object = NULL;
	static char *keyword_list[] = { "sub_file_entry_index", NULL };
	int sub_file_entry_index    = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_file_entry_index ) == 0 )
        {
		return( NULL );
        }
	file_entry_object = pyewf_file_entry_get_sub_file_entry_by_index(
	                     pyewf_file_entry,
	                     sub_file_entry_index );

	return( file_entry_object );
}

/* Retrieves a file entries sequence and iterator object for the sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_file_entry_get_sub_file_entries(
           pyewf_file_entry_t *pyewf_file_entry )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error       = NULL;
	PyObject *file_entries_object  = NULL;
	static char *function          = "pyewf_file_entry_get_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	if( pyewf_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_file_entry_get_number_of_sub_file_entries(
	          pyewf_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub file entries.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of sub file entries.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	file_entries_object = pyewf_file_entries_new(
	                       pyewf_file_entry,
	                       &pyewf_file_entry_get_sub_file_entry_by_index,
	                       number_of_sub_file_entries );

	if( file_entries_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entries object.",
		 function );

		return( NULL );
	}
	return( file_entries_object );
}


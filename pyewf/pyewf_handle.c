/*
 * Python object definition of the libewf handle
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* Fix HAVE_FSTAT define in pyport.h
 */
#undef HAVE_FSTAT

#include <Python.h>

#include <libewf.h>

#include "pyewf_handle.h"
#include "pyewf_metadata.h"

PyMethodDef pyewf_handle_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyewf_handle_signal_abort,
	  METH_NOARGS,
	  "Signals the handle to abort the current activity" },

	/* Functions to access the media data */

	{ "open",
	  (PyCFunction) pyewf_handle_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "Opens EWF file(s)" },

	{ "close",
	  (PyCFunction) pyewf_handle_close,
	  METH_NOARGS,
	  "Closes EWF file(s)" },

	{ "read_buffer",
	  (PyCFunction) pyewf_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "Reads a buffer of media data from EWF file(s)" },

	{ "read_random",
	  (PyCFunction) pyewf_handle_read_random,
	  METH_VARARGS | METH_KEYWORDS,
	  "Reads a buffer of media data at a specific offset from EWF file(s)" },

	{ "write_buffer",
	  (PyCFunction) pyewf_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "Writes a buffer of media data to EWF file(s)" },

	{ "write_random",
	  (PyCFunction) pyewf_handle_write_random,
	  METH_VARARGS | METH_KEYWORDS,
	  "Writes a buffer of media data at a specific offset to EWF file(s)" },

	{ "seek_offset",
	  (PyCFunction) pyewf_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "Seeks an offset within the media data" },

	{ "get_offset",
	  (PyCFunction) pyewf_handle_get_offset,
	  METH_NOARGS,
	  "Return the current offset within the media data" },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pyewf_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "Reads a buffer of media data from EWF file(s)" },

	{ "write",
	  (PyCFunction) pyewf_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "Writes a buffer of media data to EWF file(s)" },

	{ "seek",
	  (PyCFunction) pyewf_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "Seeks an offset within the media data" },

	{ "tell",
	  (PyCFunction) pyewf_handle_get_offset,
	  METH_NOARGS,
	  "Return the current offset within the media data" },

	/* Functions to access the metadata */

	{ "get_media_size",
	  (PyCFunction) pyewf_handle_get_media_size,
	  METH_NOARGS,
	  "Retrieves the size of the media data" },

	{ "get_header_value",
	  (PyCFunction) pyewf_handle_get_header_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "Retrieves a header value by its name" },

	{ "get_header_values",
	  (PyCFunction) pyewf_handle_get_header_values,
	  METH_NOARGS,
	  "Retrieves all header values" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyTypeObject pyewf_handle_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyewf.handle",
	/* tp_basicsize */
	sizeof( pyewf_handle_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyewf_handle_free,
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
	"pyewf handle object (wraps libewf_handle_t)",
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
	pyewf_handle_object_methods,
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
	(initproc) pyewf_handle_initialize,
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

/* Initialize a pyewf handle object
 * Returns 1 if successful or -1 on handle
 */
int pyewf_handle_initialize(
     pyewf_handle_t *pyewf_handle,
     liberror_error_t **error )
{
	static char *function = "pyewf_handle_initialize";

	if( libewf_handle_initialize(
	     &( pyewf_handle->handle ),
	     error ) != 1 )
	{
		/* TODO something with error */

		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: failed to initialize libewf handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Frees a pyewf handle object
 */
void pyewf_handle_free(
      pyewf_handle_t *pyewf_handle )
{
	pyewf_handle->ob_type->tp_free(
	 (PyObject*) pyewf_handle );
}

/* Signals the handle to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_signal_abort(
           pyewf_handle_t *pyewf_handle )
{
	liberror_error_t *error = NULL;
	static char *function   = "pyewf_handle_signal_abort";

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	if( pyewf_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle - missing libewf handle.",
		 function );

		return( NULL );
	}
	if( libewf_handle_signal_abort(
	     pyewf_handle->handle,
	     &error ) != 1 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to signal abort.",
		 function );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( Py_None );
}

/* Open EWF file(s)
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_open(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	liberror_error_t *error     = NULL;
	char **filenames            = NULL;
	static char *keyword_list[] = { "files", NULL };
	PyObject *sequence_object   = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyewf_handle_open";
	size_t filename_size        = 0;
	int number_of_filenames     = 0;
	int filename_iterator       = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	      keywords,
	      "O",
	      keyword_list,
	      &sequence_object ) == 0 )
	{
		return( NULL );
	}
	if( PySequence_Check(
	     sequence_object ) == 0 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: argument: files must be a list or tuple.",
		 function );

		return( NULL );
	}
	number_of_filenames = PySequence_Size(
	                       sequence_object );

	if( ( number_of_filenames <= 0 )
	 || ( number_of_filenames > (int) UINT16_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid number of files.",
		 function );

		return( NULL );
	}
	filenames = (char **) memory_allocate(
	                       sizeof( char * ) * number_of_filenames );

	if( filenames == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create filenames.",
		 function );

		return( NULL );
	}
	if( memory_set(
	     filenames,
	     0,
	     sizeof( char * ) * number_of_filenames ) == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to clear filenames.",
		 function );

		memory_free(
		 filenames );

		return( NULL );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		string_object = PySequence_GetItem(
		                 sequence_object,
		                 filename_iterator );

		filename_size = PyString_Size(
		                 string_object );

		filenames[ filename_iterator ] = (char *) memory_allocate(
		                                           sizeof( char ) * filename_size );

		if( filenames[ filename_iterator ] == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create filename: %d.",
			 function,
			 filename_iterator + 1 );

			for( ; filename_iterator > 0; filename_iterator-- )
			{
				memory_free(
				 filenames[ filename_iterator - 1 ] );
			}
			memory_free(
			 filenames );

			return( NULL );
		}
		if( libcstring_narrow_string_copy(
		     filenames[ filename_iterator ],
		     PyString_AsString(
		      string_object ),
		     filename_size ) == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to set filename: %d.",
			 function,
			 filename_iterator + 1 );

			for( ; filename_iterator > 0; filename_iterator-- )
			{
				memory_free(
				 filenames[ filename_iterator - 1 ] );
			}
			memory_free(
			 filenames );

			return( NULL );
		}
		( filenames[ filename_iterator ] )[ filename_size - 1 ] = 0;

		Py_DECREF(
		 string_object );
	}
	if( libewf_handle_open(
	     pyewf_handle->handle,
             filenames,
             number_of_filenames,
             LIBEWF_OPEN_READ,
	     &error ) != 1 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to open file(s).",
		 function );

		liberror_error_free(
		 &error );

		for( filename_iterator = 0;
		     filename_iterator < number_of_filenames;
		     filename_iterator++ )
		{
			memory_free(
			 filenames[ filename_iterator ] );
		}
		memory_free(
		 filenames );

		return( NULL );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

	return( Py_None );
}

/* Closes EWF file(s)
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_close(
           pyewf_handle_t *pyewf_handle )
{
	liberror_error_t *error = NULL;
	static char *function   = "pyewf_handle_close";

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	if( pyewf_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle - missing libewf handle.",
		 function );

		return( NULL );
	}
	if( libewf_handle_close(
	     pyewf_handle->handle,
	     &error ) != 0 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to close file(s).",
		 function );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	if( libewf_handle_free(
	     &( pyewf_handle->handle ),
	     &error ) != 1 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to free libewf handle.",
		 function );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( Py_None );
}

/* Reads a buffer of media data from EWF file(s)
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_handle_read_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	liberror_error_t *error     = NULL;
	PyObject *result_data       = NULL;
	static char *function       = "pyewf_handle_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int read_size               = -1;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	if( pyewf_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle - missing libewf handle.",
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

	read_count = libewf_handle_read_buffer(
	              pyewf_handle->handle,
	              PyString_AsString(
	               result_data ),
	              (size_t) read_size,
	              &error );

	if( read_count != (ssize_t) read_size )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to read data (requested %d, returned %" PRIzd ").",
		 function,
		 read_size,
		 read_count );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( result_data );
}

/* Reads a buffer of media data at a specific offset from EWF file(s)
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_handle_read_random(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	liberror_error_t *error     = NULL;
	PyObject *result_data       = NULL;
	static char *function       = "pyewf_handle_read_random";
	static char *keyword_list[] = { "size", "offset", NULL };
	ssize_t read_count          = 0;
	off64_t read_offset         = 0;
	int read_size               = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	if( pyewf_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle - missing libewf handle.",
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
	result_data = PyString_FromStringAndSize(
	               NULL,
	               read_size );

	read_count = libewf_handle_read_random(
	              pyewf_handle->handle,
	              PyString_AsString(
	               result_data ),
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	if( read_count != (ssize_t) read_size )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to read data (requested %d, returned %" PRIzd ").",
		 function,
		 read_size,
		 read_count );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( result_data );
}

/* Writes a buffer of media data to EWF file(s)
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_handle_write_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	liberror_error_t *error     = NULL;
	PyObject *result_data       = NULL;
	static char *function       = "pyewf_handle_write_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t write_count         = 0;
	int write_size              = -1;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	if( pyewf_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle - missing libewf handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &write_size ) == 0 )
	{
		return( NULL );
	}
	if( write_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( write_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	result_data = PyString_FromStringAndSize(
	               NULL,
	               write_size );

	write_count = libewf_handle_write_buffer(
	               pyewf_handle->handle,
	               PyString_AsString(
	                result_data ),
	               write_size,
	               &error );

	if( write_count != (ssize_t) write_size )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to write data (requested %" PRId32 ", returned %" PRIzd ").",
		 function,
		 write_size,
		 write_count );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( result_data );
}

/* Seeks a certain offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_seek_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	liberror_error_t *error     = NULL;
	static char *function       = "pyewf_handle_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	if( pyewf_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle - missing libewf handle.",
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
	if( libewf_handle_seek_offset(
	     pyewf_handle->handle,
	     offset,
	     whence,
	     &error ) < 0 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to seek offset.",
		 function );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( Py_None );
}

/* Retrieves the current offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_offset(
           pyewf_handle_t *pyewf_handle )
{
	liberror_error_t *error = NULL;
	static char *function   = "pyewf_handle_get_offset";
	off64_t current_offset  = 0;

	if( libewf_handle_get_offset(
	     pyewf_handle->handle,
	     &current_offset,
	     &error ) != 1 )
	{
		/* TODO something with error */

		PyErr_Format(
		 PyExc_IOError,
		 "%s: failed to retrieve offset.",
		 function );

		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         current_offset ) );
}


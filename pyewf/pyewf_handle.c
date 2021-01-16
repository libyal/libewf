/*
 * Python object wrapper of libewf_handle_t
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (C) 2008-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyewf_codepage.h"
#include "pyewf_error.h"
#include "pyewf_file_entry.h"
#include "pyewf_file_objects_io_pool.h"
#include "pyewf_handle.h"
#include "pyewf_integer.h"
#include "pyewf_libbfio.h"
#include "pyewf_libcerror.h"
#include "pyewf_libclocale.h"
#include "pyewf_libewf.h"
#include "pyewf_python.h"
#include "pyewf_unused.h"

#if !defined( LIBEWF_HAVE_BFIO )

LIBEWF_EXTERN \
int libewf_handle_open_file_io_pool(
     libewf_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libewf_error_t **error );

#endif /* !defined( LIBEWF_HAVE_BFIO ) */

PyMethodDef pyewf_handle_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyewf_handle_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the handle to abort the current activity." },

	/* Functions to access the media data */

	{ "open",
	  (PyCFunction) pyewf_handle_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filenames, mode='r') -> None\n"
	  "\n"
	  "Opens a handle from a sequence (list) of all the segment filenames.\n"
	  "Use pyewf.glob() to determine the segment filenames from first (e.g. E01)." },

	{ "open_file_objects",
	  (PyCFunction) pyewf_handle_open_file_objects,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_objects(file_objects, mode='r') -> None\n"
	  "\n"
	  "Opens a handle using a list of file-like objects." },

	{ "close",
	  (PyCFunction) pyewf_handle_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a handle." },

	{ "read_buffer",
	  (PyCFunction) pyewf_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of media data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyewf_handle_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> String\n"
	  "\n"
	  "Reads a buffer of media data at a specific offset." },

	{ "write_buffer",
	  (PyCFunction) pyewf_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "write_buffer(buffer) -> None\n"
	  "\n"
	  "Writes a buffer of media data." },

	{ "write_buffer_at_offset",
	  (PyCFunction) pyewf_handle_write_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "write_buffer_at_offset(buffer, offset) -> None\n"
	  "\n"
	  "Writes a buffer of media data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyewf_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the media data." },

	{ "get_offset",
	  (PyCFunction) pyewf_handle_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the media data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pyewf_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> String\n"
	  "\n"
	  "Reads a buffer of media data." },

	{ "write",
	  (PyCFunction) pyewf_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "write(buffer, size) -> None\n"
	  "\n"
	  "Writes a buffer of media data." },

	{ "seek",
	  (PyCFunction) pyewf_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the media data." },

	{ "tell",
	  (PyCFunction) pyewf_handle_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the media data." },

	/* Functions to access the metadata */

	{ "get_media_size",
	  (PyCFunction) pyewf_handle_get_media_size,
	  METH_NOARGS,
	  "get_media_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the media data." },

	{ "get_sectors_per_chunk",
	  (PyCFunction) pyewf_handle_get_sectors_per_chunk,
	  METH_NOARGS,
	  "get_sectors_per_chunk() -> Integer\n"
	  "\n"
	  "Retrieves the number of sectors per chunk." },

	{ "get_bytes_per_sector",
	  (PyCFunction) pyewf_handle_get_bytes_per_sector,
	  METH_NOARGS,
	  "get_bytes_per_sector() -> Integer\n"
	  "\n"
	  "Retrieves the number of bytes per sector." },

	{ "get_number_of_sectors",
	  (PyCFunction) pyewf_handle_get_number_of_sectors,
	  METH_NOARGS,
	  "get_number_of_sectors() -> Integer\n"
	  "\n"
	  "Retrieves the number of sectors of the media data." },

	{ "get_chunk_size",
	  (PyCFunction) pyewf_handle_get_chunk_size,
	  METH_NOARGS,
	  "get_chunk_size() -> Integer\n"
	  "\n"
	  "Retrieves the chunk size." },

	{ "get_error_granularity",
	  (PyCFunction) pyewf_handle_get_error_granularity,
	  METH_NOARGS,
	  "get_error_granularity() -> Integer\n"
	  "\n"
	  "Retrieves the error granularity." },

	{ "get_compression_method",
	  (PyCFunction) pyewf_handle_get_compression_method,
	  METH_NOARGS,
	  "get_compression_method() -> Integer\n"
	  "\n"
	  "Retrieves the compression method." },

	{ "get_media_type",
	  (PyCFunction) pyewf_handle_get_media_type,
	  METH_NOARGS,
	  "get_media_type() -> Integer\n"
	  "\n"
	  "Retrieves the media type." },

	{ "get_media_flags",
	  (PyCFunction) pyewf_handle_get_media_flags,
	  METH_NOARGS,
	  "get_media_flags() -> Integer\n"
	  "\n"
	  "Retrieves the media flags." },

	{ "get_format",
	  (PyCFunction) pyewf_handle_get_format,
	  METH_NOARGS,
	  "get_format() -> Integer\n"
	  "\n"
	  "Retrieves the format." },

	{ "get_header_codepage",
	  (PyCFunction) pyewf_handle_get_header_codepage,
	  METH_NOARGS,
	  "get_header_codepage() -> String\n"
	  "\n"
	  "Returns the codepage used for header strings." },

	{ "set_header_codepage",
	  (PyCFunction) pyewf_handle_set_header_codepage,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_header_codepage(codepage) -> None\n"
	  "\n"
	  "Set the codepage used for header strings.\n"
	  "Expects the codepage to be a String containing a Python codec definition." },

	{ "get_header_value",
	  (PyCFunction) pyewf_handle_get_header_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_header_value(identifier) -> Unicode string or None\n"
	  "\n"
	  "Retrieves a header value by its identifier (name)." },

	{ "get_header_values",
	  (PyCFunction) pyewf_handle_get_header_values,
	  METH_NOARGS,
	  "get_header_values() -> Dictionary\n"
	  "\n"
	  "Retrieves all header values." },

	{ "get_hash_value",
	  (PyCFunction) pyewf_handle_get_hash_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_hash_value(identifier) -> Unicode string or None\n"
	  "\n"
	  "Retrieves a hash value by its identifier (name)." },

	{ "get_hash_values",
	  (PyCFunction) pyewf_handle_get_hash_values,
	  METH_NOARGS,
	  "get_hash_values() -> Dictionary\n"
	  "\n"
	  "Retrieves all hash values." },

	/* Functions to access the (single) file entries */

	{ "get_root_file_entry",
	  (PyCFunction) pyewf_handle_get_root_file_entry,
	  METH_NOARGS,
	  "get_root_file_entry() -> Object\n"
	  "\n"
	  "Retrieves the root file entry." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyewf_handle_object_get_set_definitions[] = {

	{ "header_codepage",
	  (getter) pyewf_handle_get_header_codepage,
	  (setter) pyewf_handle_set_header_codepage_setter,
	  "The codepage used for header strings.",
	  NULL },

	{ "media_size",
	  (getter) pyewf_handle_get_media_size,
	  (setter) 0,
	  "The media size.",
	  NULL },

	{ "sectors_per_chunk",
	  (getter) pyewf_handle_get_sectors_per_chunk,
	  (setter) 0,
	  "The number of sectors per chunk.",
	  NULL },

	{ "bytes_per_sector",
	  (getter) pyewf_handle_get_bytes_per_sector,
	  (setter) 0,
	  "The number of bytes per sector.",
	  NULL },

	{ "number_of_sectors",
	  (getter) pyewf_handle_get_number_of_sectors,
	  (setter) 0,
	  "The media number of sectors.",
	  NULL },

	{ "chunk_size",
	  (getter) pyewf_handle_get_chunk_size,
	  (setter) 0,
	  "The chunk size.",
	  NULL },

	{ "error_granularity",
	  (getter) pyewf_handle_get_error_granularity,
	  (setter) 0,
	  "The error granularity.",
	  NULL },

	{ "compression_method",
	  (getter) pyewf_handle_get_compression_method,
	  (setter) 0,
	  "The compression method.",
	  NULL },

	{ "media_type",
	  (getter) pyewf_handle_get_media_type,
	  (setter) 0,
	  "The media type.",
	  NULL },

	{ "media_flags",
	  (getter) pyewf_handle_get_media_flags,
	  (setter) 0,
	  "The media flags.",
	  NULL },

	{ "format",
	  (getter) pyewf_handle_get_format,
	  (setter) 0,
	  "The format.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyewf_handle_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

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
	pyewf_handle_object_get_set_definitions,
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
	(initproc) pyewf_handle_init,
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

/* Initializes a handle object
 * Returns 0 if successful or -1 on error
 */
int pyewf_handle_init(
     pyewf_handle_t *pyewf_handle )
{
	static char *function    = "pyewf_handle_init";
	libcerror_error_t *error = NULL;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	pyewf_handle->handle       = NULL;
	pyewf_handle->file_io_pool = NULL;

	if( libewf_handle_initialize(
	     &( pyewf_handle->handle ),
	     &error ) != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a handle object
 */
void pyewf_handle_free(
      pyewf_handle_t *pyewf_handle )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyewf_handle_free";
	int result                  = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return;
	}
	if( pyewf_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle - missing libewf handle.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyewf_handle );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_free(
	          &( pyewf_handle->handle ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free handle.",
		 function );

		libcerror_error_free(
		 &error );
	}
	ob_type->tp_free(
	 (PyObject*) pyewf_handle );
}

/* Signals the handle to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_signal_abort(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_handle_signal_abort";
	int result               = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_signal_abort(
	          pyewf_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Open a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_open(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t *filename                = NULL;
	wchar_t **filenames              = NULL;
	const char *errors               = NULL;
	char *narrow_string              = NULL;
	size_t narrow_string_size        = 0;
	int is_unicode_string            = 0;
#else
	char *filename                   = NULL;
	char **filenames                 = NULL;
#endif
	PyObject *filename_string_object = NULL;
	PyObject *sequence_object        = NULL;
	PyObject *string_object          = NULL;
	PyObject *utf8_string_object     = NULL;
	libcerror_error_t *error         = NULL;
	static char *function            = "pyewf_handle_open";
	static char *keyword_list[]      = { "filenames", "mode", NULL };
	char *mode                       = NULL;
	Py_ssize_t sequence_size         = 0;
	size_t filename_length           = 0;
	int access_flags                 = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

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
	      "O|s",
	      keyword_list,
	      &sequence_object,
	      &mode ) == 0 )
	{
		return( NULL );
	}
	if( PySequence_Check(
	     sequence_object ) == 0 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: argument: filenames must be a sequence object.",
		 function );

		return( NULL );
	}
	if( mode == NULL )
	{
		access_flags = LIBEWF_OPEN_READ;
	}
	else if( mode[ 0 ] == 'r' )
	{
		if( ( mode[ 1 ] == 0 )
		  || ( ( mode[ 1 ] == 'b' )
		   &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBEWF_OPEN_READ;
		}
	}
	else if( mode[ 0 ] == 'w' )
	{
		if( ( mode[ 1 ] == 0 )
		 || ( ( mode[ 1 ] == 'b' )
		  &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBEWF_OPEN_WRITE;
		}
	}
	else if( mode[ 0 ] == 'a' )
	{
		if( ( mode[ 1 ] == 0 )
		 || ( ( mode[ 1 ] == 'b' )
		  &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBEWF_OPEN_WRITE_RESUME;
		}
	}
	if( access_flags == 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	sequence_size = PySequence_Size(
	                 sequence_object );

	if( sequence_size > (Py_ssize_t) INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence size value exceeds maximum.",
		 function );

		goto on_error;
	}
	number_of_filenames = (int) sequence_size;

	if( ( number_of_filenames <= 0 )
	 || ( number_of_filenames > (int) UINT16_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid number of files.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	filenames = (wchar_t **) PyMem_Malloc(
	                          sizeof( wchar_t * ) * number_of_filenames );
#else
	filenames = (char **) PyMem_Malloc(
	                       sizeof( char * ) * number_of_filenames );
#endif
	if( filenames == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create filenames.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( memory_set(
	     filenames,
	     0,
	     sizeof( wchar_t * ) * number_of_filenames ) == NULL )
#else
	if( memory_set(
	     filenames,
	     0,
	     sizeof( char * ) * number_of_filenames ) == NULL )
#endif
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to clear filenames.",
		 function );

		PyMem_Free(
		 filenames );

		return( NULL );
	}
	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		string_object = PySequence_GetItem(
		                 sequence_object,
		                 filename_index );

		PyErr_Clear();

		result = PyObject_IsInstance(
			  string_object,
			  (PyObject *) &PyUnicode_Type );

		if( result == -1 )
		{
			pyewf_error_fetch_and_raise(
			 PyExc_ValueError,
			 "%s: unable to determine if the sequence object: %d is of type unicode.",
			 function,
			 filename_index );

			goto on_error;
		}
		else if( result == 0 )
		{
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
			else if( result == 0 )
			{
				PyErr_Format(
				 PyExc_TypeError,
				 "%s: unsupported string object type",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			else
			{
				is_unicode_string = 0;
			}
#endif
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		else
		{
			is_unicode_string = 1;
		}
#endif
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( is_unicode_string != 0 )
		{
			filename = (wchar_t *) PyUnicode_AsUnicode(
			                        string_object );
		}
		else
		{
#if PY_MAJOR_VERSION >= 3
			narrow_string = PyBytes_AsString(
			                 string_object );

			narrow_string_size = PyBytes_Size(
			                      string_object );
#else
			narrow_string = PyString_AsString(
			                 string_object );

			narrow_string_size = PyString_Size(
			                      string_object );
#endif
			filename_string_object = PyUnicode_Decode(
						  narrow_string,
						  narrow_string_size,
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
			filename = (wchar_t *) PyUnicode_AsUnicode(
			                        filename_string_object );
		}
		filename_length = wide_string_length(
		                   filename );
#else
		/* A Unicode string object can be converted into UFT-8 formatted narrow string
		 */
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyewf_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			goto on_error;
		}
#if PY_MAJOR_VERSION >= 3
		filename = PyBytes_AsString(
		            utf8_string_object );
#else
		filename = PyString_AsString(
		            utf8_string_object );
#endif
		filename_length = narrow_string_length(
		                   filename );

#endif /* #if defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filenames[ filename_index ] = (wchar_t *) PyMem_Malloc(
		                                           sizeof( wchar_t ) * ( filename_length + 1 ) );
#else
		filenames[ filename_index ] = (char *) PyMem_Malloc(
		                                        sizeof( char ) * ( filename_length + 1 ) );
#endif
		if( filenames[ filename_index ] == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create filename: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( wide_string_copy(
		     filenames[ filename_index ],
		     filename,
		     filename_length ) == NULL )
#else
		if( narrow_string_copy(
		     filenames[ filename_index ],
		     filename,
		     filename_length ) == NULL )
#endif
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to set filename: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
		( filenames[ filename_index ] )[ filename_length ] = 0;

		if( utf8_string_object != NULL )
		{
			Py_DecRef(
			 utf8_string_object );

			utf8_string_object = NULL;
		}
		if( filename_string_object != NULL )
		{
			Py_DecRef(
			 filename_string_object );

			filename_string_object = NULL;
		}
		/* The string object was reference by PySequence_GetItem
		 */
		Py_DecRef(
		 string_object );

		string_object = NULL;
	}
	Py_BEGIN_ALLOW_THREADS

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libewf_handle_open_wide(
	          pyewf_handle->handle,
	          filenames,
	          number_of_filenames,
	          access_flags,
	          &error );
#else
	result = libewf_handle_open(
	          pyewf_handle->handle,
	          filenames,
	          number_of_filenames,
	          access_flags,
	          &error );
#endif
	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		PyMem_Free(
		 filenames[ filename_index ] );
	}
	PyMem_Free(
	 filenames );

	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( filename_string_object != NULL )
	{
		Py_DecRef(
		 filename_string_object );
	}
	if( string_object != NULL )
	{
		Py_DecRef(
		 string_object );
	}
	if( filenames != NULL )
	{
		for( ; filename_index > 0; filename_index-- )
		{
			PyMem_Free(
			 filenames[ filename_index - 1 ] );
		}
		PyMem_Free(
		 filenames );
	}
	return( NULL );
}

/* Opens a handle using a list of file-like objects
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_open_file_objects(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sequence_object   = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyewf_handle_open_file_objects";
	static char *keyword_list[] = { "file_objects", "mode", NULL };
	char *mode                  = NULL;
	int access_flags            = 0;
	int result                  = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &sequence_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( PySequence_Check(
	     sequence_object ) == 0 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: argument: file_objects must be a sequence object.",
		 function );

		return( NULL );
	}
	if( mode == NULL )
	{
		access_flags = LIBEWF_OPEN_READ;
	}
	else if( mode[ 0 ] == 'r' )
	{
		if( ( mode[ 1 ] == 0 )
		  || ( ( mode[ 1 ] == 'b' )
		   &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBEWF_OPEN_READ;
		}
	}
	else if( mode[ 0 ] == 'w' )
	{
		if( ( mode[ 1 ] == 0 )
		 || ( ( mode[ 1 ] == 'b' )
		  &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBEWF_OPEN_WRITE;
		}
	}
	else if( mode[ 0 ] == 'a' )
	{
		if( ( mode[ 1 ] == 0 )
		 || ( ( mode[ 1 ] == 'b' )
		  &&  ( mode[ 2 ] == 0 ) ) )
		{
			access_flags = LIBEWF_OPEN_WRITE_RESUME;
		}
	}
	if( access_flags == 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	if( pyewf_file_objects_pool_initialize(
	     &( pyewf_handle->file_io_pool ),
	     sequence_object,
	     LIBBFIO_OPEN_READ,
	     &error ) != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO pool.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_open_file_io_pool(
	          pyewf_handle->handle,
	          pyewf_handle->file_io_pool,
	          access_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyewf_handle->file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( pyewf_handle->file_io_pool ),
		 NULL );
	}
	return( NULL );
}

/* Closes a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_close(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_handle_close";
	int result               = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pyewf handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_close(
	          pyewf_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyewf_handle->file_io_pool != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_pool_free(
		          &( pyewf_handle->file_io_pool ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO pool.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Reads a buffer of media data
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_handle_read_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyewf_handle_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	char *buffer                = NULL;
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
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libewf_handle_read_buffer(
	              pyewf_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads a buffer of media data at a specific offset
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_handle_read_buffer_at_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyewf_handle_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t read_offset         = 0;
	ssize_t read_count          = 0;
	int read_size               = 0;

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
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libewf_handle_read_buffer_at_offset(
	              pyewf_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Writes a buffer of media data
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_handle_write_buffer(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyewf_handle_write_buffer";
	static char *keyword_list[] = { "buffer", NULL };
	char *buffer                = NULL;
	Py_ssize_t buffer_size      = 0;
	ssize_t write_count         = 0;

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
	     &string_object ) == 0 )
	{
		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	buffer = PyBytes_AsString(
	          string_object );

	buffer_size = PyBytes_Size(
	               string_object );
#else
	buffer = PyString_AsString(
	          string_object );

	buffer_size = PyString_Size(
	               string_object );
#endif
	if( ( buffer_size < 0 )
	 || ( buffer_size > (Py_ssize_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument buffer size value out of bounds.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	write_count = libewf_handle_write_buffer(
	               pyewf_handle->handle,
	               (uint8_t *) buffer,
	               (size_t) buffer_size,
	               &error );

	Py_END_ALLOW_THREADS

	if( write_count != (ssize_t) buffer_size )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to write data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Writes a buffer of media data at a specific offset
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pyewf_handle_write_buffer_at_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyewf_handle_write_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t write_offset        = 0;
	Py_ssize_t buffer_size      = 0;
	ssize_t write_count         = 0;

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
	     "O|L",
	     keyword_list,
	     &string_object,
	     &write_offset ) == 0 )
	{
		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	buffer = PyBytes_AsString(
	          string_object );

	buffer_size = PyBytes_Size(
	               string_object );
#else
	buffer = PyString_AsString(
	          string_object );

	buffer_size = PyString_Size(
	               string_object );
#endif
	if( ( buffer_size < 0 )
	 || ( buffer_size > (Py_ssize_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument buffer size value out of bounds.",
		 function );

		return( NULL );
	}
	if( write_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument write offset value less than zero.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	write_count = libewf_handle_write_buffer_at_offset(
	               pyewf_handle->handle,
	               (uint8_t *) buffer,
	               (size_t) buffer_size,
	               write_offset,
	               &error );

	Py_END_ALLOW_THREADS

	if( write_count != (ssize_t) buffer_size )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to write data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Seeks a certain offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_seek_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
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

	offset = libewf_handle_seek_offset(
	          pyewf_handle->handle,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

 	if( offset == -1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

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
PyObject *pyewf_handle_get_offset(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyewf_handle_get_offset";
	off64_t current_offset   = 0;
	int result               = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_offset(
	          pyewf_handle->handle,
	          &current_offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_signed_new_from_64bit(
	                  (int64_t) current_offset );

	return( integer_object );
}

/* Retrieves the root file entry
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_root_file_entry(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error             = NULL;
	libewf_file_entry_t *root_file_entry = NULL;
	PyObject *file_entry_object          = NULL;
	static char *function                = "pyewf_handle_get_root_file_entry";
	int result                           = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_root_file_entry(
	          pyewf_handle->handle,
	          &root_file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve root file entry.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	file_entry_object = pyewf_file_entry_new(
	                     root_file_entry,
	                     (PyObject *) pyewf_handle );

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
	if( root_file_entry != NULL )
	{
		libewf_file_entry_free(
		 &root_file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the size of the media data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_media_size(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyewf_handle_get_media_size";
	size64_t media_size      = 0;
	int result               = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_media_size(
	          pyewf_handle->handle,
	          &media_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve media size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) media_size );

	return( integer_object );
}

/* Retrieves the number of sectors per chunk
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_sectors_per_chunk(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *integer_object   = NULL;
	static char *function      = "pyewf_handle_get_sectors_per_chunk";
	uint32_t sectors_per_chunk = 0;
	int result                 = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_sectors_per_chunk(
	          pyewf_handle->handle,
	          &sectors_per_chunk,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sectors per chunk.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) sectors_per_chunk );

	return( integer_object );
}

/* Retrieves the number of bytes per sector
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_bytes_per_sector(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error  = NULL;
	PyObject *integer_object  = NULL;
	static char *function     = "pyewf_handle_get_bytes_per_sector";
	uint32_t bytes_per_sector = 0;
	int result                = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_bytes_per_sector(
	          pyewf_handle->handle,
	          &bytes_per_sector,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve bytes per sectors.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) bytes_per_sector );

	return( integer_object );
}

/* Retrieves the number of sectors
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_number_of_sectors(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error   = NULL;
	PyObject *integer_object   = NULL;
	static char *function      = "pyewf_handle_get_number_of_sectors";
	uint64_t number_of_sectors = 0;
	int result                 = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_number_of_sectors(
	          pyewf_handle->handle,
	          &number_of_sectors,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sectors.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  number_of_sectors );

	return( integer_object );
}

/* Retrieves the chunk size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_chunk_size(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error  = NULL;
	PyObject *integer_object  = NULL;
	static char *function     = "pyewf_handle_get_chunk_size";
	size32_t chunk_size       = 0;
	int result                = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_chunk_size(
	          pyewf_handle->handle,
	          &chunk_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve chunk size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) chunk_size );

	return( integer_object );
}

/* Retrieves the error granularity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_error_granularity(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error   = NULL;
	PyObject *integer_object   = NULL;
	static char *function      = "pyewf_handle_get_error_granularity";
	uint32_t error_granularity = 0;
	int result                 = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_error_granularity(
	          pyewf_handle->handle,
	          &error_granularity,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve error granularity.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) error_granularity );

	return( integer_object );
}

/* Retrieves the compression method
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_compression_method(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	static char *function       = "pyewf_handle_get_compression_method";
	uint16_t compression_method = 0;
	int result                  = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_compression_method(
	          pyewf_handle->handle,
	          &compression_method,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve compression method.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) compression_method );

	return( integer_object );
}

/* Retrieves the media type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_media_type(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyewf_handle_get_media_type";
	uint8_t media_type       = 0;
	int result               = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_media_type(
	          pyewf_handle->handle,
	          &media_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve media type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) media_type );

	return( integer_object );
}

/* Retrieves the media flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_media_flags(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyewf_handle_get_media_flags";
	uint8_t media_flags      = 0;
	int result               = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_media_flags(
	          pyewf_handle->handle,
	          &media_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve media flags.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) media_flags );

	return( integer_object );
}

/* Retrieves the format
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_format(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyewf_handle_get_format";
	uint8_t format           = 0;
	int result               = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_format(
	          pyewf_handle->handle,
	          &format,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve format.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyewf_integer_unsigned_new_from_64bit(
	                  (uint64_t) format );

	return( integer_object );
}

/* Retrieves the codepage used for ASCII strings in the header
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_get_header_codepage(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	const char *codepage_string = NULL;
	static char *function       = "pyewf_handle_get_header_codepage";
	int header_codepage         = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( libewf_handle_get_header_codepage(
	     pyewf_handle->handle,
	     &header_codepage,
	     &error ) != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve header codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	codepage_string = pyewf_codepage_to_string(
	                   header_codepage );

	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported header codepage: %d.",
		 function,
		 header_codepage );

		return( NULL );
	}
	string_object = PyUnicode_FromString(
	                 codepage_string );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert codepage string into string object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Sets the codepage used for ASCII strings in the header
 * Returns 1 if successful or -1 on error
 */
int pyewf_handle_set_header_codepage_from_string(
     pyewf_handle_t *pyewf_handle,
     const char *codepage_string )
{
	libcerror_error_t *error      = NULL;
	static char *function         = "pyewf_handle_set_header_codepage_from_string";
	size_t codepage_string_length = 0;
	uint32_t feature_flags        = 0;
	int header_codepage           = 0;
	int result                    = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid codepage string.",
		 function );

		return( -1 );
	}
	codepage_string_length = narrow_string_length(
	                          codepage_string );

	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	if( libclocale_codepage_copy_from_string(
	     &header_codepage,
	     codepage_string,
	     codepage_string_length,
	     feature_flags,
	     &error ) != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_RuntimeError,
		 "%s: unable to determine header codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_set_header_codepage(
	          pyewf_handle->handle,
	          header_codepage,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set header codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the codepage used for ASCII strings in the header
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyewf_handle_set_header_codepage(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	static char *keyword_list[] = { "codepage", NULL };
	char *codepage_string       = NULL;
	int result                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &codepage_string ) == 0 )
	{
		return( NULL );
	}
	result = pyewf_handle_set_header_codepage_from_string(
	          pyewf_handle,
	          codepage_string );

	if( result != 1 )
	{
		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Sets the codepage used for ASCII strings in the header
 * Returns a Python object if successful or NULL on error
 */
int pyewf_handle_set_header_codepage_setter(
     pyewf_handle_t *pyewf_handle,
     PyObject *string_object,
     void *closure PYEWF_ATTRIBUTE_UNUSED )
{
	PyObject *utf8_string_object = NULL;
	static char *function        = "pyewf_handle_set_ascii_codepage_setter";
	char *codepage_string        = NULL;
	int result                   = 0;

	PYEWF_UNREFERENCED_PARAMETER( closure )

	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyewf_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		/* The codepage string should only contain ASCII characters.
		 */
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyewf_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( -1 );
		}
#if PY_MAJOR_VERSION >= 3
		codepage_string = PyBytes_AsString(
				   utf8_string_object );
#else
		codepage_string = PyString_AsString(
				   utf8_string_object );
#endif
		if( codepage_string == NULL )
		{
			return( -1 );
		}
		result = pyewf_handle_set_header_codepage_from_string(
		          pyewf_handle,
		          codepage_string );

		if( result != 1 )
		{
			return( -1 );
		}
		return( 0 );
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

		return( -1 );
	}
	else if( result != 0 )
	{
#if PY_MAJOR_VERSION >= 3
		codepage_string = PyBytes_AsString(
		                   string_object );
#else
		codepage_string = PyString_AsString(
		                   string_object );
#endif
		if( codepage_string == NULL )
		{
			return( -1 );
		}
		result = pyewf_handle_set_header_codepage_from_string(
			  pyewf_handle,
			  codepage_string );

		if( result != 1 )
		{
			return( -1 );
		}
		return( 0 );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( -1 );
}

/* Retrieves a header value
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_header_value(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error              = NULL;
	PyObject *string_object               = NULL;
	static char *function                 = "pyewf_handle_get_header_value";
	static char *keyword_list[]           = { "identifier", NULL };
	const char *errors                    = NULL;
	char *header_value_identifier         = NULL;
	char *header_value                    = NULL;
	size_t header_value_identifier_length = 0;
	size_t header_value_size              = 0;
	int result                            = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &header_value_identifier ) == 0 )
	{
		return( NULL );
	}
	header_value_identifier_length = narrow_string_length(
	                                  header_value_identifier );

	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_utf8_header_value_size(
	          pyewf_handle->handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          &header_value_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve UTF-8 header value: %s size.",
		 function,
		 header_value_identifier );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if header value is present
	 */
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	header_value = (char *) PyMem_Malloc(
	                         sizeof( char ) * header_value_size );

	if( header_value == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create header value.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_utf8_header_value(
	          pyewf_handle->handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          (uint8_t *) header_value,
	          header_value_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve UTF-8 header value: %s.",
		 function,
		 header_value_identifier );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if the header value is present
	 */
	else if( result == 0 )
	{
		PyMem_Free(
		 header_value );

		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 header_value,
	                 (Py_ssize_t) header_value_size - 1,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 header value: %s into Unicode.",
		 function,
		 header_value_identifier );

		goto on_error;
	}
	PyMem_Free(
	 header_value );

	return( string_object );

on_error:
	if( header_value != NULL )
	{
		PyMem_Free(
		 header_value );
	}
	return( NULL );
}

/* Retrieves the header values
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_header_values(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error              = NULL;
	PyObject *dictionary_object           = NULL;
	PyObject *string_object               = NULL;
	static char *function                 = "pyewf_handle_get_header_values";
	const char *errors                    = NULL;
	char *header_value                    = NULL;
	char *header_value_identifier         = NULL;
	size_t header_value_identifier_length = 0;
	size_t header_value_identifier_size   = 0;
	size_t header_value_size              = 0;
	uint32_t number_of_header_values      = 0;
	uint32_t header_value_index           = 0;
	int result                            = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_number_of_header_values(
	          pyewf_handle->handle,
	          &number_of_header_values,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve number of header values.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	dictionary_object = PyDict_New();

	for( header_value_index = 0;
	     header_value_index < number_of_header_values;
	     header_value_index++ )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libewf_handle_get_header_value_identifier_size(
		          pyewf_handle->handle,
		          header_value_index,
		          &header_value_identifier_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve header value: %d identifier size.",
			 function,
			 header_value_index );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		header_value_identifier = (char *) PyMem_Malloc(
		                                    sizeof( char ) * header_value_identifier_size );

		if( header_value_identifier == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create header value identifier.",
			 function );

			goto on_error;
		}
		Py_BEGIN_ALLOW_THREADS

		result = libewf_handle_get_header_value_identifier(
		          pyewf_handle->handle,
		          header_value_index,
		          (uint8_t *) header_value_identifier,
		          header_value_identifier_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve header value: %d identifier.",
			 function,
			 header_value_index );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		header_value_identifier_length = narrow_string_length(
						  header_value_identifier );

		Py_BEGIN_ALLOW_THREADS

		result = libewf_handle_get_utf8_header_value_size(
		          pyewf_handle->handle,
		          (uint8_t *) header_value_identifier,
		          header_value_identifier_length,
		          &header_value_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 header value: %s size.",
			 function,
			 header_value_identifier );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		/* Ignore empty header values
		 */
		if( ( result != 0 )
		 && ( header_value_size > 0 ) )
		{
			header_value = (char *) PyMem_Malloc(
			                         sizeof( char ) * header_value_size );

			if( header_value == NULL )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to create header value.",
				 function );

				goto on_error;
			}
			Py_BEGIN_ALLOW_THREADS

			result = libewf_handle_get_utf8_header_value(
			          pyewf_handle->handle,
			          (uint8_t *) header_value_identifier,
			          header_value_identifier_length,
			          (uint8_t *) header_value,
			          header_value_size,
			          &error );

			Py_END_ALLOW_THREADS

			if( result != 1 )
			{
				pyewf_error_raise(
				 error,
				 PyExc_IOError,
				 "%s: unable to retrieve UTF-8 header value: %s.",
				 function,
				 header_value_identifier );

				libcerror_error_free(
				 &error );

				goto on_error;
			}
			/* Pass the string length to PyUnicode_DecodeUTF8
			 * otherwise it makes the end of string character is part
			 * of the string
			 */
			string_object = PyUnicode_DecodeUTF8(
			                 header_value,
			                 header_value_size - 1,
			                 errors );

			if( string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert UTF-8 header value: %s into Unicode.",
				 function,
				 header_value_identifier );

				goto on_error;
			}
			if( PyDict_SetItemString(
			     dictionary_object,
			     header_value_identifier,
			     string_object ) != 0 )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to set header value: %s in dictionary.",
				 function,
				 header_value_identifier );

				goto on_error;
			}
			string_object = NULL;

			PyMem_Free(
			 header_value );

			header_value = NULL;
		}
		PyMem_Free(
		 header_value_identifier );

		header_value_identifier = NULL;
	}
	return( dictionary_object );

on_error:
	if( string_object != NULL )
	{
		Py_DecRef(
		 string_object );
	}
	if( header_value != NULL )
	{
		PyMem_Free(
		 header_value );
	}
	if( header_value_identifier != NULL )
	{
		PyMem_Free(
		 header_value_identifier );
	}
	if( dictionary_object != NULL )
	{
		Py_DecRef(
		 dictionary_object );
	}
	return( NULL );
}

/* Retrieves a hash value
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_hash_value(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error             = NULL;
	PyObject *string_object             = NULL;
	static char *function               = "pyewf_handle_get_hash_value";
	static char *keyword_list[]         = { "identifier", NULL };
	const char *errors                  = NULL;
	char *hash_value_identifier         = NULL;
	char *hash_value                    = NULL;
	size_t hash_value_identifier_length = 0;
	size_t hash_value_size              = 0;
	int result                          = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &hash_value_identifier ) == 0 )
	{
		return( NULL );
	}
	hash_value_identifier_length = narrow_string_length(
	                                hash_value_identifier );

	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_utf8_hash_value_size(
	          pyewf_handle->handle,
	          (uint8_t *) hash_value_identifier,
	          hash_value_identifier_length,
	          &hash_value_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve UTF-8 hash value: %s size.",
		 function,
		 hash_value_identifier );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if hash value is present
	 */
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	hash_value = (char *) PyMem_Malloc(
	                       sizeof( char ) * hash_value_size );

	if( hash_value == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create hash value.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_utf8_hash_value(
	          pyewf_handle->handle,
	          (uint8_t *) hash_value_identifier,
	          hash_value_identifier_length,
	          (uint8_t *) hash_value,
	          hash_value_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve UTF-8 hash value: %s.",
		 function,
		 hash_value_identifier );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if the hash value is present
	 */
	else if( result == 0 )
	{
		PyMem_Free(
		 hash_value );

		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 hash_value,
	                 (Py_ssize_t) hash_value_size - 1,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 hash value: %s into Unicode.",
		 function,
		 hash_value_identifier );

		goto on_error;
	}
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

/* Retrieves the hash values
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_hash_values(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments PYEWF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error            = NULL;
	PyObject *dictionary_object         = NULL;
	PyObject *string_object             = NULL;
	static char *function               = "pyewf_handle_get_hash_values";
	const char *errors                  = NULL;
	char *hash_value                    = NULL;
	char *hash_value_identifier         = NULL;
	size_t hash_value_identifier_length = 0;
	size_t hash_value_identifier_size   = 0;
	size_t hash_value_size              = 0;
	uint32_t number_of_hash_values      = 0;
	uint32_t hash_value_index           = 0;
	int result                          = 0;

	PYEWF_UNREFERENCED_PARAMETER( arguments )

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_number_of_hash_values(
	          pyewf_handle->handle,
	          &number_of_hash_values,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyewf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve number of hash values.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	dictionary_object = PyDict_New();

	for( hash_value_index = 0;
	     hash_value_index < number_of_hash_values;
	     hash_value_index++ )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libewf_handle_get_hash_value_identifier_size(
		          pyewf_handle->handle,
		          hash_value_index,
		          &hash_value_identifier_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve hash value: %d identifier size.",
			 function,
			 hash_value_index );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		hash_value_identifier = (char *) PyMem_Malloc(
		                                  sizeof( char ) * hash_value_identifier_size );

		if( hash_value_identifier == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create hash value identifier.",
			 function );

			goto on_error;
		}
		Py_BEGIN_ALLOW_THREADS

		result = libewf_handle_get_hash_value_identifier(
		          pyewf_handle->handle,
		          hash_value_index,
		          (uint8_t *) hash_value_identifier,
		          hash_value_identifier_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve hash value: %d identifier.",
			 function,
			 hash_value_index );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		hash_value_identifier_length = narrow_string_length(
		                                hash_value_identifier );

		Py_BEGIN_ALLOW_THREADS

		result = libewf_handle_get_utf8_hash_value_size(
		          pyewf_handle->handle,
		          (uint8_t *) hash_value_identifier,
		          hash_value_identifier_length,
		          &hash_value_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyewf_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 hash value: %s size.",
			 function,
			 hash_value_identifier );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		/* Ignore empty hash values
		 */
		if( ( result != 0 )
		 && ( hash_value_size > 0 ) )
		{
			hash_value = (char *) PyMem_Malloc(
			                       sizeof( char ) * hash_value_size );

			if( hash_value == NULL )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to create hash value.",
				 function );

				goto on_error;
			}
			Py_BEGIN_ALLOW_THREADS

			result = libewf_handle_get_utf8_hash_value(
			          pyewf_handle->handle,
			          (uint8_t *) hash_value_identifier,
			          hash_value_identifier_length,
			          (uint8_t *) hash_value,
			          hash_value_size,
			          &error );

			Py_END_ALLOW_THREADS

			if( result != 1 )
			{
				pyewf_error_raise(
				 error,
				 PyExc_IOError,
				 "%s: unable to retrieve UTF-8 hash value: %s.",
				 function,
				 hash_value_identifier );

				libcerror_error_free(
				 &error );

				goto on_error;
			}
			/* Pass the string length to PyUnicode_DecodeUTF8
			 * otherwise it makes the end of string character is part
			 * of the string
			 */
			string_object = PyUnicode_DecodeUTF8(
			                 hash_value,
			                 hash_value_size - 1,
			                 errors );

			if( string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert UTF-8 hash value: %s into Unicode.",
				 function,
				 hash_value_identifier );

				goto on_error;
			}
			if( PyDict_SetItemString(
			     dictionary_object,
			     hash_value_identifier,
			     string_object ) != 0 )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to set hash value: %s in dictionary.",
				 function,
				 hash_value_identifier );

				goto on_error;
			}
			string_object = NULL;

			PyMem_Free(
			 hash_value );

			hash_value = NULL;
		}
		PyMem_Free(
		 hash_value_identifier );

		hash_value_identifier = NULL;
	}
	return( dictionary_object );

on_error:
	if( string_object != NULL )
	{
		Py_DecRef(
		 string_object );
	}
	if( hash_value != NULL )
	{
		PyMem_Free(
		 hash_value );
	}
	if( hash_value_identifier != NULL )
	{
		PyMem_Free(
		 hash_value_identifier );
	}
	if( dictionary_object != NULL )
	{
		Py_DecRef(
		 dictionary_object );
	}
	return( NULL );
}


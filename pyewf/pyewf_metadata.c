/*
 *  Metadata functions for the Python object definition of the libewf handle
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

#include "pyewf.h"
#include "pyewf_codepage.h"
#include "pyewf_handle.h"
#include "pyewf_libcerror.h"
#include "pyewf_libclocale.h"
#include "pyewf_libcstring.h"
#include "pyewf_libewf.h"
#include "pyewf_metadata.h"
#include "pyewf_python.h"

/* Retrieves the size of the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_media_size(
           pyewf_handle_t *pyewf_handle )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyewf_handle_get_media_size";
	size64_t media_size      = 0;
	int result               = 0;

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
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve media size.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve media size.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if defined( HAVE_LONG_LONG )
	if( media_size > (size64_t) LLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: media size value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         (long long) media_size ) );
#else
	if( media_size > (size64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: media size value exceeds maximum.",
		 function );

		return( NULL );
	}
	return( PyLong_FromLong(
	         (long) media_size ) );
#endif
}

/* Retrieves the codepage used for header strings
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_header_codepage(
           pyewf_handle_t *pyewf_handle )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	const char *codepage_string = NULL;
	static char *function       = "pyewf_handle_get_header_codepage";
	int header_codepage         = 0;
	int result                  = 0;

	if( pyewf_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_get_header_codepage(
	          pyewf_handle->handle,
	          &header_codepage,
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
			 "%s: unable to retrieve header codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve header codepage.\n%s",
			 function,
			 error_string );
		}
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
	string_object = PyString_FromString(
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

/* Sets the codepage used for header strings
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_set_header_codepage(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error      = NULL;
	char *codepage_string         = NULL;
	static char *keyword_list[]   = { "codepage", NULL };
	static char *function         = "pyewf_handle_set_header_codepage";
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

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &codepage_string ) == 0 )
        {
                return( NULL );
        }
	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid codepage string.",
		 function );

		return( NULL );
	}
	codepage_string_length = libcstring_narrow_string_length(
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
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libewf_handle_set_header_codepage(
	          pyewf_handle->handle,
	          header_codepage,
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
			 "%s: unable to set ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to set ASCII codepage.\n%s",
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

/* Retrieves a header value
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_header_value(
           pyewf_handle_t *pyewf_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

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
	header_value_identifier_length = libcstring_narrow_string_length(
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
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 header value: %s size.",
			 function,
			 header_value_identifier );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 header value: %s size.\n%s",
			 function,
			 header_value_identifier,
			 error_string );
		}
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
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 header value: %s.",
			 function,
			 header_value_identifier );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 header value: %s.\n%s",
			 function,
			 header_value_identifier,
			 error_string );
		}
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
           pyewf_handle_t *pyewf_handle )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	libcerror_error_t *error               = NULL;
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

	if( result == -1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve number of header values.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve number of header values.\n%s",
			 function,
			 error_string );
		}
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
			if( libcerror_error_backtrace_sprint(
			     error,
			     error_string,
			     PYEWF_ERROR_STRING_SIZE ) == -1 )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve header value: %d identifier size.",
				 function,
				 header_value_index );
			}
			else
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve header value: %d identifier size.\n%s",
				 function,
				 header_value_index,
				 error_string );
			}
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
			if( libcerror_error_backtrace_sprint(
			     error,
			     error_string,
			     PYEWF_ERROR_STRING_SIZE ) == -1 )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve header value: %d identifier.",
				 function,
				 header_value_index );
			}
			else
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve header value: %d identifier.\n%s",
				 function,
				 header_value_index,
				 error_string );
			}
			libcerror_error_free(
			 &error );

			goto on_error;
		}
		header_value_identifier_length = libcstring_narrow_string_length(
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
			if( libcerror_error_backtrace_sprint(
			     error,
			     error_string,
			     PYEWF_ERROR_STRING_SIZE ) == -1 )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve UTF-8 header value: %s size.",
				 function,
				 header_value_identifier );
			}
			else
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve UTF-8 header value: %s size.\n%s",
				 function,
				 header_value_identifier,
				 error_string );
			}
			libcerror_error_free(
			 &error );

			goto on_error;
		}
		/* Ignore emtpy header values
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
				if( libcerror_error_backtrace_sprint(
				     error,
				     error_string,
				     PYEWF_ERROR_STRING_SIZE ) == -1 )
				{
					PyErr_Format(
					 PyExc_IOError,
					 "%s: unable to retrieve UTF-8 header value: %s.",
					 function,
					 header_value_identifier );
				}
				else
				{
					PyErr_Format(
					 PyExc_IOError,
					 "%s: unable to retrieve UTF-8 header value: %s.\n%s",
					 function,
					 header_value_identifier,
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
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

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
	hash_value_identifier_length = libcstring_narrow_string_length(
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
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 hash value: %s size.",
			 function,
			 hash_value_identifier );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 hash value: %s size.\n%s",
			 function,
			 hash_value_identifier,
			 error_string );
		}
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
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 hash value: %s.",
			 function,
			 hash_value_identifier );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve UTF-8 hash value: %s.\n%s",
			 function,
			 hash_value_identifier,
			 error_string );
		}
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
           pyewf_handle_t *pyewf_handle )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

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

	if( result == -1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve number of hash values.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve number of hash values.\n%s",
			 function,
			 error_string );
		}
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
			if( libcerror_error_backtrace_sprint(
			     error,
			     error_string,
			     PYEWF_ERROR_STRING_SIZE ) == -1 )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve hash value: %d identifier size.",
				 function,
				 hash_value_index );
			}
			else
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve hash value: %d identifier size.\n%s",
				 function,
				 hash_value_index,
				 error_string );
			}
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
			if( libcerror_error_backtrace_sprint(
			     error,
			     error_string,
			     PYEWF_ERROR_STRING_SIZE ) == -1 )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve hash value: %d identifier.",
				 function,
				 hash_value_index );
			}
			else
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve hash value: %d identifier.\n%s",
				 function,
				 hash_value_index,
				 error_string );
			}
			libcerror_error_free(
			 &error );

			goto on_error;
		}
		hash_value_identifier_length = libcstring_narrow_string_length(
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
			if( libcerror_error_backtrace_sprint(
			     error,
			     error_string,
			     PYEWF_ERROR_STRING_SIZE ) == -1 )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve UTF-8 hash value: %s size.",
				 function,
				 hash_value_identifier );
			}
			else
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to retrieve UTF-8 hash value: %s size.\n%s",
				 function,
				 hash_value_identifier,
				 error_string );
			}
			libcerror_error_free(
			 &error );

			goto on_error;
		}
		/* Ignore emtpy hash values
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
				if( libcerror_error_backtrace_sprint(
				     error,
				     error_string,
				     PYEWF_ERROR_STRING_SIZE ) == -1 )
				{
					PyErr_Format(
					 PyExc_IOError,
					 "%s: unable to retrieve UTF-8 hash value: %s.",
					 function,
					 hash_value_identifier );
				}
				else
				{
					PyErr_Format(
					 PyExc_IOError,
					 "%s: unable to retrieve UTF-8 hash value: %s.\n%s",
					 function,
					 hash_value_identifier,
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


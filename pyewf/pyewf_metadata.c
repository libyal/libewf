/*
 *  Metadata functions for the Python object definition of the libewf handle
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include <libewf.h>

#include "pyewf.h"
#include "pyewf_handle.h"
#include "pyewf_metadata.h"
#include "pyewf_python.h"

/* Retrieves the size of the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_media_size(
           pyewf_handle_t *pyewf_handle )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	liberror_error_t *error = NULL;
	static char *function   = "pyewf_handle_get_media_size";
	size64_t media_size     = 0;

	if( libewf_handle_get_media_size(
	     pyewf_handle->handle,
	     &media_size,
	     &error ) != 1 )
	{
		if( liberror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEWF_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve media size.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: failed to retrieve media size.\n%s",
			 function,
			 error_string );
		}
		liberror_error_free(
		 &error );

		return( NULL );
	}
	return( PyLong_FromLongLong(
	         media_size ) );
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

	liberror_error_t *error               = NULL;
	PyObject *string_object               = NULL;
	static char *function                 = "pyewf_handle_get_header_value";
	static char *keyword_list[]           = { "identifier", NULL };
	const char *errors                    = NULL;
	char *header_value_identifier         = NULL;
	char *header_value                    = NULL;
	size_t header_value_identifier_length = 0;
	size_t header_value_size              = 0;
	int result                            = 0;

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

	result = libewf_handle_get_utf8_header_value_size(
	          pyewf_handle->handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          &header_value_size,
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
		liberror_error_free(
		 &error );

		return( NULL );
	}
	/* Check if header value is present
	 */
	else if( result == 0 )
	{
		return( Py_None );
	}
	header_value = (char *) memory_allocate(
	                         sizeof( char ) * header_value_size );

	if( header_value == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create header value.",
		 function );

		return( NULL );
	}
	result = libewf_handle_get_utf8_header_value(
	          pyewf_handle->handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          (uint8_t *) header_value,
	          header_value_size,
	          NULL );

	if( result == -1 )
	{
		if( liberror_error_backtrace_sprint(
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
		liberror_error_free(
		 &error );

		memory_free(
		 header_value );

		return( NULL );
	}
	/* Check if the header value is present
	 */
	else if( result == 0 )
	{
		memory_free(
		 header_value );

		return( Py_None );
	}
	string_object = PyUnicode_DecodeUTF8(
	                 header_value,
	                 (Py_ssize_t) header_value_size,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 header value: %s into Unicode.",
		 function,
		 header_value_identifier );

		memory_free(
		 header_value );

		return( NULL );
	}
	memory_free(
	 header_value );

	return( string_object );
}

/* Retrieves the header values
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_header_values(
           pyewf_handle_t *pyewf_handle )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	liberror_error_t *error               = NULL;
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

	if( libewf_handle_get_number_of_header_values(
	     pyewf_handle->handle,
	     &number_of_header_values,
	     &error ) != 1 )
	{
		if( liberror_error_backtrace_sprint(
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
		liberror_error_free(
		 &error );

		return( NULL );
	}
	dictionary_object = PyDict_New();

	for( header_value_index = 0;
	     header_value_index < number_of_header_values;
	     header_value_index++ )
	{
		if( libewf_handle_get_header_value_identifier_size(
		     pyewf_handle->handle,
		     header_value_index,
		     &header_value_identifier_size,
		     &error ) != 1 )
		{
			if( liberror_error_backtrace_sprint(
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
			liberror_error_free(
			 &error );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		header_value_identifier = (char *) memory_allocate(
		                                    sizeof( char ) * header_value_identifier_size );

		if( header_value_identifier == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create header value identifier.",
			 function );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		if( libewf_handle_get_header_value_identifier(
		     pyewf_handle->handle,
		     header_value_index,
		     (uint8_t *) header_value_identifier,
		     header_value_identifier_size,
		     &error ) != 1 )
		{
			if( liberror_error_backtrace_sprint(
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
			liberror_error_free(
			 &error );

			memory_free(
			 header_value_identifier );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		header_value_identifier_length = libcstring_narrow_string_length(
						  header_value_identifier );

		result = libewf_handle_get_utf8_header_value_size(
		          pyewf_handle->handle,
		          (uint8_t *) header_value_identifier,
		          header_value_identifier_length,
		          &header_value_size,
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
			liberror_error_free(
			 &error );

			memory_free(
			 header_value_identifier );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		/* Ignore emtpy header values
		 */
		if( ( result != 0 )
		 && ( header_value_size > 0 ) )
		{
			header_value = (char *) memory_allocate(
			                         sizeof( char ) * header_value_size );

			if( header_value == NULL )
			{
				memory_free(
				 header_value_identifier );

				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to create header value.",
				 function );

				memory_free(
				 header_value_identifier );

				Py_DecRef(
				 dictionary_object );

				return( NULL );
			}
			if( libewf_handle_get_utf8_header_value(
			     pyewf_handle->handle,
			     (uint8_t *) header_value_identifier,
			     header_value_identifier_length,
			     (uint8_t *) header_value,
			     header_value_size,
			     &error ) != 1 )
			{
				if( liberror_error_backtrace_sprint(
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
				liberror_error_free(
				 &error );

				memory_free(
				 header_value );
				memory_free(
				 header_value_identifier );

				Py_DecRef(
				 dictionary_object );

				return( NULL );
			}
			string_object = PyUnicode_DecodeUTF8(
			                 header_value,
			                 header_value_size,
			                 errors );

			if( string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert UTF-8 header value: %s into Unicode.",
				 function,
				 header_value_identifier );

				memory_free(
				 header_value );
				memory_free(
				 header_value_identifier );

				Py_DecRef(
				 dictionary_object );

				return( NULL );
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

				memory_free(
				 header_value );
				memory_free(
				 header_value_identifier );

				Py_DecRef(
				 string_object );
				Py_DecRef(
				 dictionary_object );

				return( NULL );
			}
			memory_free(
			 header_value );
		}
		memory_free(
		 header_value_identifier );
	}
	return( dictionary_object );
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

	liberror_error_t *error             = NULL;
	PyObject *string_object             = NULL;
	static char *function               = "pyewf_handle_get_hash_value";
	static char *keyword_list[]         = { "identifier", NULL };
	const char *errors                  = NULL;
	char *hash_value_identifier         = NULL;
	char *hash_value                    = NULL;
	size_t hash_value_identifier_length = 0;
	size_t hash_value_size              = 0;
	int result                          = 0;

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

	result = libewf_handle_get_utf8_hash_value_size(
	          pyewf_handle->handle,
	          (uint8_t *) hash_value_identifier,
	          hash_value_identifier_length,
	          &hash_value_size,
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
		liberror_error_free(
		 &error );

		return( NULL );
	}
	/* Check if hash value is present
	 */
	else if( result == 0 )
	{
		return( Py_None );
	}
	hash_value = (char *) memory_allocate(
	                       sizeof( char ) * hash_value_size );

	if( hash_value == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create hash value.",
		 function );

		return( NULL );
	}
	result = libewf_handle_get_utf8_hash_value(
	          pyewf_handle->handle,
	          (uint8_t *) hash_value_identifier,
	          hash_value_identifier_length,
	          (uint8_t *) hash_value,
	          hash_value_size,
	          NULL );

	if( result == -1 )
	{
		if( liberror_error_backtrace_sprint(
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
		liberror_error_free(
		 &error );

		memory_free(
		 hash_value );

		return( NULL );
	}
	/* Check if the hash value is present
	 */
	else if( result == 0 )
	{
		memory_free(
		 hash_value );

		return( Py_None );
	}
	string_object = PyUnicode_DecodeUTF8(
	                 hash_value,
	                 (Py_ssize_t) hash_value_size,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 hash value: %s into Unicode.",
		 function,
		 hash_value_identifier );

		memory_free(
		 hash_value );

		return( NULL );
	}
	memory_free(
	 hash_value );

	return( string_object );
}

/* Retrieves the hash values
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_handle_get_hash_values(
           pyewf_handle_t *pyewf_handle )
{
	char error_string[ PYEWF_ERROR_STRING_SIZE ];

	liberror_error_t *error             = NULL;
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

	if( libewf_handle_get_number_of_hash_values(
	     pyewf_handle->handle,
	     &number_of_hash_values,
	     &error ) != 1 )
	{
		if( liberror_error_backtrace_sprint(
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
		liberror_error_free(
		 &error );

		return( NULL );
	}
	dictionary_object = PyDict_New();

	for( hash_value_index = 0;
	     hash_value_index < number_of_hash_values;
	     hash_value_index++ )
	{
		if( libewf_handle_get_hash_value_identifier_size(
		     pyewf_handle->handle,
		     hash_value_index,
		     &hash_value_identifier_size,
		     &error ) != 1 )
		{
			if( liberror_error_backtrace_sprint(
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
			liberror_error_free(
			 &error );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		hash_value_identifier = (char *) memory_allocate(
		                                  sizeof( char ) * hash_value_identifier_size );

		if( hash_value_identifier == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create hash value identifier.",
			 function );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		if( libewf_handle_get_hash_value_identifier(
		     pyewf_handle->handle,
		     hash_value_index,
		     (uint8_t *) hash_value_identifier,
		     hash_value_identifier_size,
		     &error ) != 1 )
		{
			if( liberror_error_backtrace_sprint(
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
			liberror_error_free(
			 &error );

			memory_free(
			 hash_value_identifier );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		hash_value_identifier_length = libcstring_narrow_string_length(
						  hash_value_identifier );

		result = libewf_handle_get_utf8_hash_value_size(
		          pyewf_handle->handle,
		          (uint8_t *) hash_value_identifier,
		          hash_value_identifier_length,
		          &hash_value_size,
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
			liberror_error_free(
			 &error );

			memory_free(
			 hash_value_identifier );

			Py_DecRef(
			 dictionary_object );

			return( NULL );
		}
		/* Ignore emtpy hash values
		 */
		if( ( result != 0 )
		 && ( hash_value_size > 0 ) )
		{
			hash_value = (char *) memory_allocate(
			                       sizeof( char ) * hash_value_size );

			if( hash_value == NULL )
			{
				memory_free(
				 hash_value_identifier );

				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to create hash value.",
				 function );

				memory_free(
				 hash_value_identifier );

				Py_DecRef(
				 dictionary_object );

				return( NULL );
			}
			if( libewf_handle_get_utf8_hash_value(
			     pyewf_handle->handle,
			     (uint8_t *) hash_value_identifier,
			     hash_value_identifier_length,
			     (uint8_t *) hash_value,
			     hash_value_size,
			     &error ) != 1 )
			{
				if( liberror_error_backtrace_sprint(
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
				liberror_error_free(
				 &error );

				memory_free(
				 hash_value );
				memory_free(
				 hash_value_identifier );

				Py_DecRef(
				 dictionary_object );

				return( NULL );
			}
			string_object = PyUnicode_DecodeUTF8(
			                 hash_value,
			                 hash_value_size,
			                 errors );

			if( string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert UTF-8 hash value: %s into Unicode.",
				 function,
				 hash_value_identifier );

				memory_free(
				 hash_value );
				memory_free(
				 hash_value_identifier );

				Py_DecRef(
				 dictionary_object );

				return( NULL );
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

				memory_free(
				 hash_value );
				memory_free(
				 hash_value_identifier );

				Py_DecRef(
				 string_object );
				Py_DecRef(
				 dictionary_object );

				return( NULL );
			}
			memory_free(
			 hash_value );
		}
		memory_free(
		 hash_value_identifier );
	}
	return( dictionary_object );
}


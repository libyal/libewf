/*
 * File Object definition for libewf Python bindings
 *
 * Copyright (c) 2008, David Collett <david.collett@gmail.com>
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>
 *
 * Integration into libewf package by Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <narrow_string.h>
#include <memory.h>
#include <wide_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* Fix HAVE_FSTAT define in pyport.h
 */
#undef HAVE_FSTAT

#include <Python.h>

#include <libewf.h>

#include "pyewf_file.h"

/* Initialize a pyewf file object
 * Returns 0 if successful or -1 on error
 */
int pyewf_file_initialize(
     pyewf_file_t *pyewf_file,
     PyObject *arguments,
     PyObject *keywords )
{
	char **filenames            = NULL;
	static char *keyword_list[] = { "files", NULL };
	PyObject *sequence_object   = NULL;
	PyObject *string_object     = NULL;
	size_t filename_size        = 0;
	int number_of_filenames     = 0;
	int filename_iterator       = 0;

	pyewf_file->read_offset = 0;
	pyewf_file->media_size  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	      keywords,
	      "O",
	      keyword_list,
	      &sequence_object ) == 0 )
	{
		return( -1 );
	}
	if( PySequence_Check(
	     sequence_object ) == 0 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "Argument: files must be a list or tuple" );

		return( -1 );
	}
	number_of_filenames = PySequence_Size(
	                       sequence_object );

	if( ( number_of_filenames <= 0 )
	 || ( number_of_filenames > (int) UINT16_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "Invalid number of files" );

		return( -1 );
	}
	filenames = (char **) memory_allocate(
	                       sizeof( char * ) * number_of_filenames );

	if( filenames == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "Unable to create filenames" );

		return( -1 );
	}
	if( memory_set(
	     filenames,
	     0,
	     sizeof( char * ) * number_of_filenames ) == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "Unable to clear filenames" );

		memory_free(
		 filenames );

		return( -1 );
	}
	for( filename_iterator = 0; filename_iterator < number_of_filenames; filename_iterator++ )
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
			 "Unable to create filename" );

			for( ; filename_iterator > 0; filename_iterator-- )
			{
				memory_free(
				 filenames[ filename_iterator - 1 ] );
			}
			memory_free(
			 filenames );

			return( -1 );
		}
		if( narrow_string_copy(
		     filenames[ filename_iterator ],
		     PyString_AsString(
		      string_object ),
		     filename_size ) == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "Unable to set filename" );

			for( ; filename_iterator > 0; filename_iterator-- )
			{
				memory_free(
				 filenames[ filename_iterator - 1 ] );
			}
			memory_free(
			 filenames );

			return( -1 );
		}
		( filenames[ filename_iterator ] )[ filename_size - 1 ] = 0;

		Py_DECREF(
		 string_object );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_initialize(
	     &( pyewf_file->handle ),
	     NULL ) != 1 )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "libewf_handle_initialize failed to initialize handle" );

		for( filename_iterator = 0; filename_iterator < number_of_filenames; filename_iterator++ )
		{
			memory_free(
			 filenames[ filename_iterator ] );
		}
		memory_free(
		 filenames );

		return( -1 );
	}
	if( libewf_handle_open(
	     pyewf_file->handle,
             filenames,
             number_of_filenames,
             LIBEWF_OPEN_READ,
	     NULL ) != 1 )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "libewf_handle_open failed to open file(s)" );

		for( filename_iterator = 0; filename_iterator < number_of_filenames; filename_iterator++ )
		{
			memory_free(
			 filenames[ filename_iterator ] );
		}
		memory_free(
		 filenames );

		return( -1 );
	}
#else
	pyewf_file->handle = libewf_open(
	                      filenames,
	                      number_of_filenames,
	                      LIBEWF_OPEN_READ );

	if( pyewf_file->handle == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "libewf_open failed to open file(s)" );

		for( filename_iterator = 0; filename_iterator < number_of_filenames; filename_iterator++ )
		{
			memory_free(
			 filenames[ filename_iterator ] );
		}
		memory_free(
		 filenames );

		return( -1 );
	}
#endif
	for( filename_iterator = 0; filename_iterator < number_of_filenames; filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

#if defined( HAVE_V2_API )
	if( libewf_handle_get_media_size(
	     pyewf_file->handle,
	     &( pyewf_file->media_size ),
	     NULL ) != 1 )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "libewf_handle_get_media_size failed to retrieve media size" );

		return( -1 );
	}
#else
	if( libewf_get_media_size(
	     pyewf_file->handle,
	     &( pyewf_file->media_size ) ) != 1 )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "libewf_get_media_size failed to retrieve media size" );

		return( -1 );
	}
#endif
	return( 0 );
}

/* Frees a pyewf file object
 */
void pyewf_file_free(
      pyewf_file_t *pyewf_file )
{
	pyewf_file->ob_type->tp_free(
	 (PyObject*) pyewf_file );
}

/* Closes a pyewf file object
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject* pyewf_file_close(
           pyewf_file_t *pyewf_file )
{
#if defined( HAVE_V2_API )
	if( libewf_handle_close(
	     pyewf_file->handle,
	     NULL ) != 0 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_handle_close failed to close file(s)" ) );
	}
	if( libewf_handle_free(
	     &( pyewf_file->handle ),
	     NULL ) != 1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_handle_free failed to free handle" ) );
	}
#else
	if( libewf_close(
	     pyewf_file->handle ) != 0 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_close failed to close file(s)" ) );
	}
#endif
	return( Py_None );
}

/* Read media data from a pyewf file object
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject* pyewf_file_read(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *result_data       = NULL;
	static char *keyword_list[] = {"size", NULL};
	int read_size               = -1;
	ssize_t read_count          = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	/* Adjust the read size if the size not given or is larger than the size of the media data
	 */
	if( ( read_size < 0 )
	 || ( pyewf_file->read_offset + (off64_t) read_size ) > (off64_t) pyewf_file->media_size )
	{
		read_size = pyewf_file->media_size - pyewf_file->read_offset;
	}
	if( read_size < 0 )
	{
		read_size = 0;
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
	{
		read_size = INT_MAX;
	}
	result_data = PyString_FromStringAndSize(
	               NULL,
	               read_size );

#if defined( HAVE_V2_API )
	read_count = libewf_handle_read_buffer(
	              pyewf_file->handle,
	              PyString_AsString(
	               result_data),
	              read_size,
	              NULL );

	if( read_count != (ssize_t) read_size )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_handle_read_buffer failed to read data (requested %" PRId32 ", returned %" PRIzd ")",
		         read_size,
		         read_count ) );
	}
#else
	read_count = libewf_read_buffer(
	              pyewf_file->handle,
	              PyString_AsString(
	               result_data),
	              read_size );

	if( read_count != (ssize_t) read_size )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_read_buffer failed to read data (requested %" PRId32 ", returned %" PRIzd ")",
		         read_size,
		         read_count ) );
	}
#endif
	pyewf_file->read_offset += read_count;

	return( result_data );
}

/* Seeks a certain offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject* pyewf_file_seek_offset(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords )
{
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments, keywords,
	      "L|i",
	      keyword_list, 
	      &offset,
	      &whence) == 0 )
	{
		return( NULL );
	}
	switch( whence )
	{
		case 0:
			pyewf_file->read_offset = offset;

			break;

		case 1:
			pyewf_file->read_offset += offset;

			break;

		case 2:
			pyewf_file->read_offset = pyewf_file->media_size + offset;

			break;

		default:
			return( PyErr_Format(
			         PyExc_ValueError,
			         "Invalid argument (whence): %d",
			         whence ) );
	}
#if defined( HAVE_V2_API )
	if( libewf_handle_seek_offset(
	     pyewf_file->handle,
	     offset,
	     whence,
	     NULL ) < 0 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_handle_seek_offset failed (tried to seek to %" PRIu64 " - %" PRIu64 ")",
		         pyewf_file->read_offset,
		         pyewf_file->media_size ) );
	}
#else
	if( libewf_seek_offset(
	     pyewf_file->handle,
	     pyewf_file->read_offset ) < 0 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_seek_offset failed (tried to seek to %" PRIu64 " - %" PRIu64 ")",
		         pyewf_file->read_offset,
		         pyewf_file->media_size ) );
	}
#endif
	return( Py_None );
}

/* Retrieves the current offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject* pyewf_file_get_offset(
           pyewf_file_t *pyewf_file )
{
	off64_t current_offset = 0;

#if defined( HAVE_V2_API )
	if( libewf_handle_get_offset(
	     pyewf_file->handle,
	     &current_offset,
	     NULL ) != 1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_handle_get_offset failed to get offset" ) );
	}
#else
	current_offset = libewf_get_offset(
	                  pyewf_file->handle );
#endif

	return( PyLong_FromLongLong(
	         current_offset ) );
}

/* Retrieves a header value
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_file_get_header_value(
           pyewf_file_t *pyewf_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object               = NULL;
	static char *keyword_list[]           = { "identifier", NULL };
	char *header_value_identifier         = NULL;
	char *header_value                    = NULL;
	size_t header_value_size              = 0;
	int result                            = 0;

#if defined( HAVE_V2_API )
	size_t header_value_identifier_length = 0;
#endif

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &header_value_identifier ) == 0 )
	{
		return( NULL );
	}
#if !defined( HAVE_V2_API )
	/* Make sure the header values are parsed
	 */
	if( libewf_parse_header_values(
	     pyewf_file->handle,
	     LIBEWF_DATE_FORMAT_CTIME ) == -1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_parse_header_values failed to parse header values" ) );
	}
#endif
#if defined( HAVE_V2_API )
	header_value_identifier_length = narrow_string_length(
	                                  header_value_identifier );

	result = libewf_handle_get_header_value_size(
	          pyewf_file->handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          &header_value_size,
	          NULL );

	if( result == -1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
	                 "libewf_handle_get_header_value_size unable to retrieve header value size: %s",
		         header_value_identifier ) );
	}
#else
	result = libewf_get_header_value_size(
	          pyewf_file->handle,
	          header_value_identifier,
	          &header_value_size );

	if( result == -1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
	                 "libewf_get_header_value_size unable to retrieve header value size: %s",
		         header_value_identifier ) );
	}
#endif
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
		 "Unable to create header value" );

		return( NULL );
	}
#if defined( HAVE_V2_API )
	result = libewf_handle_get_header_value(
	          pyewf_file->handle,
	          (uint8_t *) header_value_identifier,
	          header_value_identifier_length,
	          (uint8_t *) header_value,
	          header_value_size,
	          NULL );

	if( result == -1 )
	{
		memory_free(
		 header_value );

		return( PyErr_Format(
		         PyExc_IOError,
	                 "libewf_handle_get_header_value_size unable to retrieve header value: %s",
		         header_value_identifier ) );
	}
#else
	result = libewf_get_header_value(
	          pyewf_file->handle,
	          header_value_identifier,
	          header_value,
	          header_value_size );

	if( result == -1 )
	{
		memory_free(
		 header_value );

		return( PyErr_Format(
		         PyExc_IOError,
	                 "libewf_get_header_value unable to retrieve header value: %s",
		         header_value_identifier ) );
	}
#endif
	/* Check if header value is present
	 */
	else if( result == 0 )
	{
		memory_free(
		 header_value );

		return( Py_None );
	}
	string_object = PyString_FromString(
	                 header_value );

	memory_free(
	 header_value );

	return( string_object );
}

/* Retrieves the header values
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyewf_file_get_header_values(
           pyewf_file_t *pyewf_file )
{
	PyObject *dictionary_object           = NULL;
	PyObject *string_object               = NULL;
	char *header_value                    = NULL;
	char *header_value_identifier         = NULL;
	uint32_t amount_of_header_values      = 0;
	uint32_t header_value_iterator        = 0;
	size_t header_value_size              = 0;
	size_t header_value_identifier_size   = 0;

#if defined( HAVE_V2_API )
	size_t header_value_identifier_length = 0;
#endif

#if !defined( HAVE_V2_API )
	/* Make sure the header values are parsed
	 */
	if( libewf_parse_header_values(
	     pyewf_file->handle,
	     LIBEWF_DATE_FORMAT_CTIME ) == -1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_parse_header_values failed to parse header values" ) );
	}
#endif
#if defined( HAVE_V2_API )
	if( libewf_handle_get_amount_of_header_values(
	     pyewf_file->handle,
	     &amount_of_header_values,
	     NULL ) != 1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_handle_get_amount_of_header_values failed to retrieve amount of header values" ) );
	}
#else
	if( libewf_get_amount_of_header_values(
	     pyewf_file->handle,
	     &amount_of_header_values ) != 1 )
	{
		return( PyErr_Format(
		         PyExc_IOError,
		         "libewf_get_amount_of_header_values failed to retrieve amount of header values" ) );
	}
#endif
	dictionary_object = PyDict_New();

	for( header_value_iterator = 0; header_value_iterator < amount_of_header_values; header_value_iterator++ )
	{
#if defined( HAVE_V2_API )
		if( libewf_handle_get_header_value_identifier_size(
		     pyewf_file->handle,
		     header_value_iterator,
		     &header_value_identifier_size,
		     NULL ) != 1 )
		{
			return( PyErr_Format(
				 PyExc_IOError,
				 "libewf_handle_get_header_value_identifier_size unable to retrieve header value identifier size: %d",
				 header_value_iterator ) );
		}
#else
		if( libewf_get_header_value_identifier_size(
		     pyewf_file->handle,
		     header_value_iterator,
		     &header_value_identifier_size ) != 1 )
		{
			return( PyErr_Format(
				 PyExc_IOError,
				 "libewf_get_header_value_identifier_size unable to retrieve header value identifier size: %d",
				 header_value_iterator ) );
		}
#endif
		header_value_identifier = (char *) memory_allocate(
		                                    sizeof( char ) * header_value_identifier_size );

		if( header_value_identifier == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "Unable to create header value identifier" );

			return( NULL );
		}
#if defined( HAVE_V2_API )
		if( libewf_handle_get_header_value_identifier(
		     pyewf_file->handle,
		     header_value_iterator,
		     (uint8_t *) header_value_identifier,
		     header_value_identifier_size,
		     NULL ) != 1 )
		{
			memory_free(
			 header_value_identifier );

			return( PyErr_Format(
				 PyExc_IOError,
				 "libewf_handle_get_header_value_identifier unable to retrieve header value identifier: %d",
				 header_value_iterator ) );
		}
#else
		if( libewf_get_header_value_identifier(
		     pyewf_file->handle,
		     header_value_iterator,
		     header_value_identifier,
		     header_value_identifier_size ) != 1 )
		{
			memory_free(
			 header_value_identifier );

			return( PyErr_Format(
				 PyExc_IOError,
				 "libewf_get_header_value_identifier unable to retrieve header value identifier: %d",
				 header_value_iterator ) );
		}
#endif
#if defined( HAVE_V2_API )
		header_value_identifier_length = narrow_string_length(
						  header_value_identifier );

		if( libewf_handle_get_header_value_size(
		     pyewf_file->handle,
		     (uint8_t *) header_value_identifier,
		     header_value_identifier_length,
		     &header_value_size,
		     NULL ) != 1 )
		{
			memory_free(
			 header_value_identifier );

			return( PyErr_Format(
				 PyExc_IOError,
				 "libewf_handle_get_header_value_size unable to retrieve header value size: %s",
				 header_value_identifier ) );
		}
#else
		if( libewf_get_header_value_size(
		     pyewf_file->handle,
		     header_value_identifier,
		     &header_value_size ) != 1 )
		{
			memory_free(
			 header_value_identifier );

			return( PyErr_Format(
				 PyExc_IOError,
				 "libewf_get_header_value_size unable to retrieve header value size: %s",
				 header_value_identifier ) );
		}
#endif
		header_value = (char *) memory_allocate(
		                         sizeof( char ) * header_value_size );

		if( header_value == NULL )
		{
			memory_free(
			 header_value_identifier );

			PyErr_Format(
			 PyExc_MemoryError,
			 "Unable to create header value" );

			return( NULL );
		}
		/* Ignore emtpy header values
		 */
#if defined( HAVE_V2_API )
		if( libewf_handle_get_header_value(
		     pyewf_file->handle,
		     (uint8_t *) header_value_identifier,
		     header_value_identifier_length,
		     (uint8_t *) header_value,
		     header_value_size,
		     NULL ) == 1 )
#else
		if( libewf_get_header_value(
		     pyewf_file->handle,
		     header_value_identifier,
		     header_value,
		     header_value_size ) == 1 )
#endif
		{
			string_object = PyString_FromFormat(
			                 header_value );

			PyDict_SetItemString(
			 dictionary_object,
			 header_value_identifier,
			 string_object );

			Py_DECREF(
			 string_object );
		}
		memory_free(
		 header_value_identifier );
		memory_free(
		 header_value );
	}
	return( dictionary_object );
}


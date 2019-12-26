/*
 * Logical Evidence File (LEF) extended attribute functions
 *
 * Copyright (C) 2006-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libewf_debug.h"
#include "libewf_lef_extended_attribute.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libuna.h"

/* Creates an extended attribute
 * Make sure the value lef_extended_attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_extended_attribute_initialize(
     libewf_lef_extended_attribute_t **lef_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_extended_attribute_initialize";

	if( lef_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *lef_extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
	*lef_extended_attribute = memory_allocate_structure(
	                           libewf_lef_extended_attribute_t );

	if( *lef_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extended attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *lef_extended_attribute,
	     0,
	     sizeof( libewf_lef_extended_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extended attribute.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *lef_extended_attribute != NULL )
	{
		memory_free(
		 *lef_extended_attribute );

		*lef_extended_attribute = NULL;
	}
	return( -1 );
}

/* Frees an extended attribute
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_extended_attribute_free(
     libewf_lef_extended_attribute_t **lef_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_extended_attribute_free";

	if( lef_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *lef_extended_attribute != NULL )
	{
		if( ( *lef_extended_attribute )->name != NULL )
		{
			memory_free(
			 ( *lef_extended_attribute )->name );
		}
		if( ( *lef_extended_attribute )->value != NULL )
		{
			memory_free(
			 ( *lef_extended_attribute )->value );
		}
		memory_free(
		 *lef_extended_attribute );

		*lef_extended_attribute = NULL;
	}
	return( 1 );
}

/* Clones the extended attribute
 * Returns 1 if successful or -1 on error
 */
int libewf_lef_extended_attribute_clone(
     libewf_lef_extended_attribute_t **destination_lef_extended_attribute,
     libewf_lef_extended_attribute_t *source_lef_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libewf_lef_extended_attribute_clone";

	if( destination_lef_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination extended attribute.",
		 function );

		return( -1 );
	}
	if( *destination_lef_extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination extended attribute value already set.",
		 function );

		return( -1 );
	}
	if( source_lef_extended_attribute == NULL )
	{
		*destination_lef_extended_attribute = NULL;

		return( 1 );
	}
	*destination_lef_extended_attribute = memory_allocate_structure(
			                       libewf_lef_extended_attribute_t );

	if( *destination_lef_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination extended attribute.",
		 function );

		goto on_error;
	}
	( *destination_lef_extended_attribute )->name  = NULL;
	( *destination_lef_extended_attribute )->value = NULL;

	if( source_lef_extended_attribute->name != NULL )
	{
		( *destination_lef_extended_attribute )->name = (uint8_t *) memory_allocate(
		                                                             sizeof( uint8_t ) * source_lef_extended_attribute->name_size );

		if( ( *destination_lef_extended_attribute )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_lef_extended_attribute )->name,
		     source_lef_extended_attribute->name,
		     source_lef_extended_attribute->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination name.",
			 function );

			goto on_error;
		}
		( *destination_lef_extended_attribute )->value_size = source_lef_extended_attribute->value_size;
	}
	if( source_lef_extended_attribute->value != NULL )
	{
		( *destination_lef_extended_attribute )->value = (uint8_t *) memory_allocate(
		                                                              sizeof( uint8_t ) * source_lef_extended_attribute->value_size );

		if( ( *destination_lef_extended_attribute )->value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination value.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_lef_extended_attribute )->value,
		     source_lef_extended_attribute->value,
		     source_lef_extended_attribute->value_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source to destination value.",
			 function );

			goto on_error;
		}
		( *destination_lef_extended_attribute )->value_size = source_lef_extended_attribute->value_size;
	}
	return( 1 );

on_error:
	if( *destination_lef_extended_attribute != NULL )
	{
		memory_free(
		 *destination_lef_extended_attribute );

		*destination_lef_extended_attribute = NULL;
	}
	return( -1 );
}

/* Reads an extended attribute
 * Returns the number of bytes read or -1 on error
 */
ssize_t libewf_lef_extended_attribute_read_data(
         libewf_lef_extended_attribute_t *lef_extended_attribute,
         const uint8_t *data,
         size_t data_size,
         libcerror_error_t **error )
{
	static char *function = "libewf_lef_extended_attribute_read_data";
	size_t data_offset    = 0;
	uint32_t name_size    = 0;
	uint32_t value_size   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( lef_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( lef_extended_attribute->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute - name value already set.",
		 function );

		return( -1 );
	}
	if( lef_extended_attribute->value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute - value value already set.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 13 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( data[ 5 ] ),
	 name_size );

	byte_stream_copy_to_uint32_little_endian(
	 &( data[ 9 ] ),
	 value_size );

	data_offset = 13;

	if( name_size > 0 )
	{
#if SIZEOF_SIZE_T < 4
		if( ( (uint32_t) name_size > ( (uint32_t) SSIZE_MAX / 2 ) )
		 || ( name_size > ( data_size - data_offset ) ) )
#elif SIZEOF_SIZE_T == 4
		if( ( (size_t) name_size > ( (size_t) SSIZE_MAX / 2 ) )
		 || ( name_size > ( data_size - data_offset ) ) )
#else
		if( name_size > ( data_size - data_offset ) )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
		name_size *= 2;

		data_offset += name_size;
	}
	if( value_size > 0 )
	{
#if SIZEOF_SIZE_T < 4
		if( ( (uint32_t) value_size > ( (uint32_t) SSIZE_MAX / 2 ) )
		 || ( value_size > ( data_size - data_offset ) ) )
#elif SIZEOF_SIZE_T == 4
		if( ( (size_t) value_size > ( (size_t) SSIZE_MAX / 2 ) )
		 || ( value_size > ( data_size - data_offset ) ) )
#else
		if( value_size > ( data_size - data_offset ) )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value size value out of bounds.",
			 function );

			goto on_error;
		}
		value_size *= 2;

		data_offset += value_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
	 	 "%s: extended attribute data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_offset,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 data,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown2\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data[ 4 ] );

		libcnotify_printf(
		 "%s: name size\t\t\t: %" PRIu32 "\n",
		 function,
		 name_size );

		libcnotify_printf(
		 "%s: value size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = 13;

	if( name_size > 0 )
	{
		lef_extended_attribute->name = (uint8_t *) memory_allocate(
		                                            sizeof( uint8_t ) * name_size );

		if( lef_extended_attribute->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     lef_extended_attribute->name,
		     &( data[ data_offset ] ),
		     name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set name.",
			 function );

			goto on_error;
		}
		lef_extended_attribute->name_size = name_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libewf_debug_print_utf16_string_value(
			     function,
			     "name\t\t\t\t",
			     lef_extended_attribute->name,
			     lef_extended_attribute->name_size,
			     LIBUNA_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print UTF-16 string value.",
				 function );

				goto on_error;
			}
		}
#endif
		data_offset += name_size;
	}
	if( value_size > 0 )
	{
		lef_extended_attribute->value = (uint8_t *) memory_allocate(
		                                             sizeof( uint8_t ) * value_size );

		if( lef_extended_attribute->value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     lef_extended_attribute->value,
		     &( data[ data_offset ] ),
		     value_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set value.",
			 function );

			goto on_error;
		}
		lef_extended_attribute->value_size = value_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libewf_debug_print_utf16_string_value(
			     function,
			     "value\t\t\t\t",
			     lef_extended_attribute->value,
			     lef_extended_attribute->value_size,
			     LIBUNA_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print UTF-16 string value.",
				 function );

				goto on_error;
			}
		}
#endif
		data_offset += value_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( (ssize_t) data_offset );

on_error:
	if( lef_extended_attribute->value != NULL )
	{
		memory_free(
		 lef_extended_attribute->value );

		lef_extended_attribute->value = NULL;
	}
	lef_extended_attribute->value_size = 0;

	if( lef_extended_attribute->name != NULL )
	{
		memory_free(
		 lef_extended_attribute->name );

		lef_extended_attribute->name = NULL;
	}
	lef_extended_attribute->name_size = 0;

	return( -1 );
}


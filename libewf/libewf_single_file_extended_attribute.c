/*
 * Single file extended_attribute functions
 *
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_single_file_extended_attribute.h"
#include "libewf_definitions.h"
#include "libfguid_definitions.h"
#include "libewf_libclocale.h"

/* Creates a single file extended_attribute
 * Make sure the value single_file_extended_attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_initialize(
     libewf_single_file_extended_attribute_t **single_file_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_initialize";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( *single_file_extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file extended_attribute value already set.",
		 function );

		return( -1 );
	}
	*single_file_extended_attribute = memory_allocate_structure(
	                      libewf_single_file_extended_attribute_t );

	if( *single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create single file extended_attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *single_file_extended_attribute,
	     0,
	     sizeof( libewf_single_file_extended_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear single file extended_attribute.",
		 function );

		goto on_error;
	}

	return( 1 );

on_error:
	if( *single_file_extended_attribute != NULL )
	{
		memory_free(
		 *single_file_extended_attribute );

		*single_file_extended_attribute = NULL;
	}
	return( -1 );
}

/* Frees a single file extended_attribute
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_free(
     libewf_single_file_extended_attribute_t **single_file_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_free";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( *single_file_extended_attribute != NULL )
	{
		if( ( *single_file_extended_attribute )->name != NULL )
		{
			memory_free(
			 ( *single_file_extended_attribute )->name );
		}
		if( ( *single_file_extended_attribute )->value != NULL )
		{
			memory_free(
			 ( *single_file_extended_attribute )->value);
		}		
		memory_free(
		 *single_file_extended_attribute );

		*single_file_extended_attribute = NULL;
	}
	return( 1 );
}

/* Clones the single file extended_attribute
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_clone(
     libewf_single_file_extended_attribute_t **destination_single_file_extended_attribute,
     libewf_single_file_extended_attribute_t *extended_attribute_single_file_extended_attribute,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_clone";

	if( destination_single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( *destination_single_file_extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination single file extended_attribute value already set.",
		 function );

		return( -1 );
	}
	if( extended_attribute_single_file_extended_attribute == NULL )
	{
		*destination_single_file_extended_attribute = NULL;

		return( 1 );
	}
	*destination_single_file_extended_attribute = memory_allocate_structure(
			                  libewf_single_file_extended_attribute_t );

	if( *destination_single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination single file extended_attribute.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_single_file_extended_attribute,
	     extended_attribute_single_file_extended_attribute,
	     sizeof( libewf_single_file_extended_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy extended_attribute to destination single file extended_attribute.",
		 function );

		memory_free(
		 *destination_single_file_extended_attribute );

		*destination_single_file_extended_attribute = NULL;

		return( -1 );
	}
	( *destination_single_file_extended_attribute )->name  = NULL;
	( *destination_single_file_extended_attribute )->value = NULL;
	
	if( extended_attribute_single_file_extended_attribute->name != NULL )
	{
		( *destination_single_file_extended_attribute )->name = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * extended_attribute_single_file_extended_attribute->name_size );

		if( ( *destination_single_file_extended_attribute )->name == NULL )
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
		     ( *destination_single_file_extended_attribute )->name,
		     extended_attribute_single_file_extended_attribute->name,
		     extended_attribute_single_file_extended_attribute->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy extended_attribute to destination name.",
			 function );

			goto on_error;
		}
		( *destination_single_file_extended_attribute )->name_size = extended_attribute_single_file_extended_attribute->name_size;
	}

	if( extended_attribute_single_file_extended_attribute->value != NULL )
	{
		( *destination_single_file_extended_attribute )->value = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * extended_attribute_single_file_extended_attribute->value_size );

		if( ( *destination_single_file_extended_attribute )->value == NULL )
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
		     ( *destination_single_file_extended_attribute )->value,
		     extended_attribute_single_file_extended_attribute->value,
		     extended_attribute_single_file_extended_attribute->value_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy extended_attribute to destination value.",
			 function );

			goto on_error;
		}
		( *destination_single_file_extended_attribute )->value_size = extended_attribute_single_file_extended_attribute->value_size;
	}

	return( 1 );

on_error:
	if( *destination_single_file_extended_attribute != NULL )
	{
		if( ( *destination_single_file_extended_attribute )->name != NULL )
		{
			memory_free(
			 ( *destination_single_file_extended_attribute )->name );
		}
		if( ( *destination_single_file_extended_attribute )->value != NULL )
		{
			memory_free(
			 ( *destination_single_file_extended_attribute )->value );
		}
		memory_free(
		 *destination_single_file_extended_attribute );

		*destination_single_file_extended_attribute = NULL;
	}
	return( -1 );
}

/* Retrieves the id
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_id(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint32_t *id,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_id";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( id == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid access time.",
		 function );

		return( -1 );
	}
	*id = single_file_extended_attribute->id;

	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_type(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_type";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = single_file_extended_attribute->type;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf8_name_size(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf8_name_size";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = single_file_extended_attribute->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf8_name(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf8_name";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-8 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_extended_attribute->name == NULL )
	 || ( single_file_extended_attribute->name_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_extended_attribute->name_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) utf8_string,
		     (char *) single_file_extended_attribute->name,
		     single_file_extended_attribute->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_extended_attribute->name_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf16_name_size(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf16_name_size";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( ( single_file_extended_attribute->name == NULL )
	 || ( single_file_extended_attribute->name_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_extended_attribute->name,
		     single_file_extended_attribute->name_size,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf16_name(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf16_name";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-16 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_extended_attribute->name == NULL )
	 || ( single_file_extended_attribute->name_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_extended_attribute->name,
		     single_file_extended_attribute->name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy name to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf8_value_size(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf8_value_size";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = single_file_extended_attribute->value_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded value value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf8_value(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf8_value";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-8 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_extended_attribute->value == NULL )
	 || ( single_file_extended_attribute->value_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_extended_attribute->value_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid UTF-8 string size value too small.",
			 function );

			return( -1 );
		}
		if( narrow_string_copy(
		     (char *) utf8_string,
		     (char *) single_file_extended_attribute->value,
		     single_file_extended_attribute->value_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_extended_attribute->value_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf16_value_size(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf16_value_size";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( ( single_file_extended_attribute->value == NULL )
	 || ( single_file_extended_attribute->value_size == 0 ) )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf8(
		     single_file_extended_attribute->value,
		     single_file_extended_attribute->value_size,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded value value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_extended_attribute_get_utf16_value(
     libewf_single_file_extended_attribute_t *single_file_extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_extended_attribute_get_utf16_value";

	if( single_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file extended_attribute.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid UTF-16 string size value too small.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( single_file_extended_attribute->value == NULL )
	 || ( single_file_extended_attribute->value_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_extended_attribute->value,
		     single_file_extended_attribute->value_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}


int libewf_single_file_decode_extended_attribute_parse_item(
	libewf_single_file_extended_attribute_t *ea_item,
	uint8_t **bufCur,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_decode_extended_attribute_parse_item";
	size_t narrow_name_size = 0;	
	size_t narrow_value_size = 0;

	extended_attribute_header_t *header = (extended_attribute_header_t*)*bufCur;
	ea_item->level = header->level;
	*bufCur += sizeof(extended_attribute_header_t);

	if (libuna_byte_stream_size_from_utf16(
		(libuna_utf16_character_t *)*bufCur,
		header->name_size,
		libclocale_codepage,
		&narrow_name_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_CONVERSION,
			LIBCERROR_CONVERSION_ERROR_GENERIC,
			"%s: unable to determine name size.",
			function);

		return(-1);
	}

	ea_item->name = (uint8_t *)memory_allocate(
								sizeof(uint8_t) * narrow_name_size);	
	if (ea_item->name == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create name.",
			function);

		goto on_error;
	}

	if (libuna_utf8_string_copy_from_utf16(
		(libuna_utf8_character_t *)ea_item->name,
		narrow_name_size,
		(libuna_utf16_character_t *)*bufCur,
		header->name_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_CONVERSION,
			LIBCERROR_CONVERSION_ERROR_GENERIC,
			"%s: unable to set narrow name string.",
			function);

		return(-1);
	}

	ea_item->name[narrow_name_size - 1] = 0;
	ea_item->name_size = narrow_name_size;
	*bufCur += header->name_size * sizeof(wchar_t);

	if (libuna_byte_stream_size_from_utf16(
		(libuna_utf16_character_t *)*bufCur,
		header->value_size,
		libclocale_codepage,
		&narrow_value_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_CONVERSION,
			LIBCERROR_CONVERSION_ERROR_GENERIC,
			"%s: unable to determine value size.",
			function);

		return(-1);
	}

	ea_item->value = (uint8_t *)memory_allocate(
								sizeof(uint8_t) * narrow_value_size);	
	if (ea_item->value == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create value.",
			function);

		goto on_error;
	}

	if (libuna_utf8_string_copy_from_utf16(
		(libuna_utf8_character_t *)ea_item->value,
		narrow_value_size,
		(libuna_utf16_character_t *)*bufCur,
		header->value_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_CONVERSION,
			LIBCERROR_CONVERSION_ERROR_GENERIC,
			"%s: unable to set narrow value string.",
			function);

		return(-1);
	}
	ea_item->value[narrow_value_size - 1] = 0;
	ea_item->value_size = narrow_value_size;
	*bufCur += header->value_size * sizeof(wchar_t);
	
	return(1);

on_error:
	return (-1);
}

int libewf_single_file_decode_extended_attribute_parse_buffer(
	libcdata_tree_node_t *parent_extended_attribute_node,
	libewf_single_file_extended_attribute_t *parent_extended_attribute_item,
	uint8_t **bufCur,
	const uint8_t *bufMax,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_decode_extended_attribute_parse_buffer";
	libcdata_tree_node_t                    *extended_attribute_node;
	extended_attribute_header_t             *curItem;		
	libewf_single_file_extended_attribute_t *ea_item = NULL;

	while (*bufCur < bufMax)
	{
		extended_attribute_node = NULL;
		curItem = (extended_attribute_header_t*)*bufCur;
		if ((int)curItem->level <= (uint32_t)parent_extended_attribute_item->level) // we need to bounce back up until we have the correct parent
		{
			return -1;
		}
		if (libcdata_tree_node_initialize(
			&extended_attribute_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				"%s: unable to create extended attribute node.",
				function);

			goto on_error;
		}

		if (libewf_single_file_extended_attribute_initialize(
			&ea_item,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				"%s: unable to create single file extended_attribute.",
				function);

			goto on_error;
		}

		libewf_single_file_decode_extended_attribute_parse_item(ea_item, bufCur, error);

		if (curItem->folder)
		{
			libewf_single_file_decode_extended_attribute_parse_buffer(extended_attribute_node, ea_item, bufCur, bufMax, error);
		}
		else
		{
			libewf_single_file_decode_extended_attribute_parse_buffer(parent_extended_attribute_node, parent_extended_attribute_item, bufCur, bufMax, error);
		}

		if (libcdata_tree_node_set_value(
			extended_attribute_node,
			(intptr_t *)ea_item,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				"%s: unable to set single file extended_attribute in node.",
				function);

			goto on_error;
		}

		if (libcdata_tree_node_append_node(
			parent_extended_attribute_node,
			extended_attribute_node,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				"%s: unable to append single file subject node to parent.",
				function);

			goto on_error;
		}

	}	
	return( 1 );

on_error:
	return ( -1 );
}

int libewf_single_file_decode_extended_attribute(
	libcdata_tree_node_t *parent_file_extended_attribute_node,
	uint8_t *encoded_tree,
	size_t encoded_tree_size,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_decode_extended_attribute";

	// convert stringified data to bytes
	size_t buffer_size = ( encoded_tree_size / 2 ) + ( encoded_tree_size % 2 );
	uint8_t *buffer = (uint8_t *)memory_allocate(
		sizeof(uint8_t) * buffer_size);

	uint8_t c[3] = { 0 };
	uint8_t *encoded_tree_end = encoded_tree + encoded_tree_size;
	int i = 0;
	long l; 
	uint8_t* bufCur;
	uint8_t* bufMax;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute_root = NULL;

	while (encoded_tree < encoded_tree_end)
	{
		c[0] = *encoded_tree++;
		c[1] = *encoded_tree++;
		l = strtol((char *)c, NULL, 16);

		buffer[i++] = (uint8_t)l;
	}

	// parse buffer
	if (libewf_single_file_extended_attribute_initialize(
		&single_file_extended_attribute_root,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable to create single file extended_attribute root.",
			function);

		goto on_error;
	}

	bufCur = buffer;
	bufMax = bufCur + buffer_size;

	libewf_single_file_decode_extended_attribute_parse_item(single_file_extended_attribute_root, &bufCur, error);

	if (libcdata_tree_node_set_value(
		parent_file_extended_attribute_node,
		(intptr_t *)single_file_extended_attribute_root,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to set single file extended_attribute in node.",
			function);

		goto on_error;
	}

	libewf_single_file_decode_extended_attribute_parse_buffer(parent_file_extended_attribute_node, single_file_extended_attribute_root, &bufCur, bufMax, error);

	if (buffer)
		memory_free(buffer);

	return(1);

on_error:
	if (buffer)
		memory_free(buffer);
	return (-1);
}
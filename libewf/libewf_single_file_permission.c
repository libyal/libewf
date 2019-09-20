/*
 * Single file permission functions
 *
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_single_file_permission.h"
#include "libewf_definitions.h"
#include "libfguid_definitions.h"

/* Creates a single file permission
 * Make sure the value single_file_permission is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_initialize(
     libewf_single_file_permission_t **single_file_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_initialize";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
		 function );

		return( -1 );
	}
	if( *single_file_permission != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file permission value already set.",
		 function );

		return( -1 );
	}
	*single_file_permission = memory_allocate_structure(
	                      libewf_single_file_permission_t );

	if( *single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create single file permission.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *single_file_permission,
	     0,
	     sizeof( libewf_single_file_permission_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear single file permission.",
		 function );

		goto on_error;
	}

	return( 1 );

on_error:
	if( *single_file_permission != NULL )
	{
		memory_free(
		 *single_file_permission );

		*single_file_permission = NULL;
	}
	return( -1 );
}

/* Frees a single file permission
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_free(
     libewf_single_file_permission_t **single_file_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_free";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
		 function );

		return( -1 );
	}
	if( *single_file_permission != NULL )
	{
		if( ( *single_file_permission )->name != NULL )
		{
			memory_free(
			 ( *single_file_permission )->name );
		}
		if( ( *single_file_permission )->uid != NULL )
		{
			memory_free(
			 ( *single_file_permission )->uid);
		}	
		memory_free(
		 *single_file_permission );

		*single_file_permission = NULL;
	}
	return( 1 );
}

/* Clones the single file permission
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_clone(
     libewf_single_file_permission_t **destination_single_file_permission,
     libewf_single_file_permission_t *permission_single_file_permission,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_clone";

	if( destination_single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination single file permission.",
		 function );

		return( -1 );
	}
	if( *destination_single_file_permission != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination single file permission value already set.",
		 function );

		return( -1 );
	}
	if( permission_single_file_permission == NULL )
	{
		*destination_single_file_permission = NULL;

		return( 1 );
	}
	*destination_single_file_permission = memory_allocate_structure(
			                  libewf_single_file_permission_t );

	if( *destination_single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination single file permission.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_single_file_permission,
	     permission_single_file_permission,
	     sizeof( libewf_single_file_permission_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy permission to destination single file permission.",
		 function );

		memory_free(
		 *destination_single_file_permission );

		*destination_single_file_permission = NULL;

		return( -1 );
	}
	( *destination_single_file_permission )->name = NULL;
    ( *destination_single_file_permission )->uid  = NULL;
	
	if( permission_single_file_permission->name != NULL )
	{
		( *destination_single_file_permission )->name = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * permission_single_file_permission->name_size );

		if( ( *destination_single_file_permission )->name == NULL )
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
		     ( *destination_single_file_permission )->name,
		     permission_single_file_permission->name,
		     permission_single_file_permission->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy permission to destination name.",
			 function );

			goto on_error;
		}
		( *destination_single_file_permission )->name_size = permission_single_file_permission->name_size;
	}
	
	if( permission_single_file_permission->uid != NULL)
	{
		( *destination_single_file_permission )->uid = ( uint8_t *)memory_allocate(
			                                                sizeof( uint8_t ) * permission_single_file_permission->uid_size );

		if( ( *destination_single_file_permission )->uid == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create destination GUID.",
			function );

			goto on_error;
		}
		if( memory_copy(
			( *destination_single_file_permission )->uid,
			permission_single_file_permission->uid,
			permission_single_file_permission->uid_size ) == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			"%s: unable to copy permission to destination uid.",
			function );

			goto on_error;
		}
		( *destination_single_file_permission )->uid_size = permission_single_file_permission->uid_size;
	}
	return( 1 );

on_error:
	if( *destination_single_file_permission != NULL )
	{
        if( ( *destination_single_file_permission)->uid != NULL )
        {
            memory_free(
             ( *destination_single_file_permission )->uid );
        }
		if( ( *destination_single_file_permission )->name != NULL )
		{
			memory_free(
			 ( *destination_single_file_permission )->name );
		}
		memory_free(
		 *destination_single_file_permission );

		*destination_single_file_permission = NULL;
	}
	return( -1 );
}

/* Retrieves the id
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_id(
     libewf_single_file_permission_t *single_file_permission,
     uint32_t *id,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_id";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	*id = single_file_permission->id;

	return( 1 );
}

/* Retrieves the props
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_props(
	libewf_single_file_permission_t *single_file_permission,
	uint32_t *props,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_permission_get_props";

	if (single_file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalprops single file permission.",
			function);

		return(-1);
	}
	if (props == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalprops access time.",
			function);

		return(-1);
	}
	*props = single_file_permission->props;

	return(1);
}

/* Retrieves the access_mask
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_access_mask(
	libewf_single_file_permission_t *single_file_permission,
	uint32_t *access_mask,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_permission_get_access_mask";

	if (single_file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalaccess_mask single file permission.",
			function);

		return(-1);
	}
	if (access_mask == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalaccess_mask access time.",
			function);

		return(-1);
	}
	*access_mask = single_file_permission->access_mask;

	return(1);
}

/* Retrieves the inheritance_flags
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_inheritance_flags(
	libewf_single_file_permission_t *single_file_permission,
	uint32_t *inheritance_flags,
	libcerror_error_t **error)
{
	static char *function = "libewf_single_file_permission_get_inheritance_flags";

	if (single_file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalinheritance_flags single file permission.",
			function);

		return(-1);
	}
	if (inheritance_flags == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalinheritance_flags access time.",
			function);

		return(-1);
	}
	*inheritance_flags = single_file_permission->inheritance_flags;

	return(1);
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_type(
     libewf_single_file_permission_t *single_file_permission,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_type";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	*type = single_file_permission->type;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_utf8_name_size(
     libewf_single_file_permission_t *single_file_permission,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf8_name_size";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	*utf8_string_size = single_file_permission->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_utf8_name(
     libewf_single_file_permission_t *single_file_permission,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf8_name";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	if( ( single_file_permission->name == NULL )
	 || ( single_file_permission->name_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_permission->name_size )
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
		     (char *) single_file_permission->name,
		     single_file_permission->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_permission->name_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_utf16_name_size(
     libewf_single_file_permission_t *single_file_permission,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf16_name_size";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
		 function );

		return( -1 );
	}
	if( ( single_file_permission->name == NULL )
	 || ( single_file_permission->name_size == 0 ) )
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
		     single_file_permission->name,
		     single_file_permission->name_size,
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
int libewf_single_file_permission_get_utf16_name(
     libewf_single_file_permission_t *single_file_permission,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf16_name";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	if( ( single_file_permission->name == NULL )
	 || ( single_file_permission->name_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_permission->name,
		     single_file_permission->name_size,
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

/* Retrieves the size of the UTF-8 encoded uid
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_utf8_uid_size(
     libewf_single_file_permission_t *single_file_permission,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf8_uid_size";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	*utf8_string_size = single_file_permission->uid_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded uid value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_utf8_uid(
     libewf_single_file_permission_t *single_file_permission,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf8_uid";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	if( ( single_file_permission->uid == NULL )
	 || ( single_file_permission->uid_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_permission->uid_size )
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
		     (char *) single_file_permission->uid,
		     single_file_permission->uid_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_permission->uid_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded uid
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_utf16_uid_size(
     libewf_single_file_permission_t *single_file_permission,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf16_uid_size";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
		 function );

		return( -1 );
	}
	if( ( single_file_permission->uid == NULL )
	 || ( single_file_permission->uid_size == 0 ) )
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
		     single_file_permission->uid,
		     single_file_permission->uid_size,
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

/* Retrieves the UTF-16 encoded uid value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_permission_get_utf16_uid(
     libewf_single_file_permission_t *single_file_permission,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_permission_get_utf16_uid";

	if( single_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file permission.",
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
	if( ( single_file_permission->uid == NULL )
	 || ( single_file_permission->uid_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_permission->uid,
		     single_file_permission->uid_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy uid to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}
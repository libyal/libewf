/*
 * Single file subject functions
 *
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_libuna.h"
#include "libewf_single_file_subject.h"
#include "libewf_definitions.h"
#include "libfguid_definitions.h"

/* Creates a single file subject
 * Make sure the value single_file_subject is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_initialize(
     libewf_single_file_subject_t **single_file_subject,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_initialize";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
		 function );

		return( -1 );
	}
	if( *single_file_subject != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid single file subject value already set.",
		 function );

		return( -1 );
	}
	*single_file_subject = memory_allocate_structure(
	                      libewf_single_file_subject_t );

	if( *single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create single file subject.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *single_file_subject,
	     0,
	     sizeof( libewf_single_file_subject_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear single file subject.",
		 function );

		goto on_error;
	}

	return( 1 );

on_error:
	if( *single_file_subject != NULL )
	{
		memory_free(
		 *single_file_subject );

		*single_file_subject = NULL;
	}
	return( -1 );
}

/* Frees a single file subject
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_free(
     libewf_single_file_subject_t **single_file_subject,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_free";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
		 function );

		return( -1 );
	}
	if( *single_file_subject != NULL )
	{
		if( ( *single_file_subject )->name != NULL )
		{
			memory_free(
			 ( *single_file_subject )->name );
		}
		if( ( *single_file_subject )->number != NULL )
		{
			memory_free(
			 ( *single_file_subject )->number);
		}	
		if( ( *single_file_subject )->comment != NULL )
		{
			memory_free(
			 ( *single_file_subject )->comment);
		}		
        if( ( *single_file_subject )->guid != NULL )
        {
            memory_free(
             ( *single_file_subject )->guid );
        }
		memory_free(
		 *single_file_subject );

		*single_file_subject = NULL;
	}
	return( 1 );
}

/* Clones the single file subject
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_clone(
     libewf_single_file_subject_t **destination_single_file_subject,
     libewf_single_file_subject_t *subject_single_file_subject,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_clone";

	if( destination_single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination single file subject.",
		 function );

		return( -1 );
	}
	if( *destination_single_file_subject != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination single file subject value already set.",
		 function );

		return( -1 );
	}
	if( subject_single_file_subject == NULL )
	{
		*destination_single_file_subject = NULL;

		return( 1 );
	}
	*destination_single_file_subject = memory_allocate_structure(
			                  libewf_single_file_subject_t );

	if( *destination_single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination single file subject.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_single_file_subject,
	     subject_single_file_subject,
	     sizeof( libewf_single_file_subject_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy subject to destination single file subject.",
		 function );

		memory_free(
		 *destination_single_file_subject );

		*destination_single_file_subject = NULL;

		return( -1 );
	}
	( *destination_single_file_subject )->name    = NULL;
	( *destination_single_file_subject )->number  = NULL;
	( *destination_single_file_subject )->comment = NULL;
    ( *destination_single_file_subject )->guid    = NULL;
	
	if( subject_single_file_subject->name != NULL )
	{
		( *destination_single_file_subject )->name = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * subject_single_file_subject->name_size );

		if( ( *destination_single_file_subject )->name == NULL )
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
		     ( *destination_single_file_subject )->name,
		     subject_single_file_subject->name,
		     subject_single_file_subject->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy subject to destination name.",
			 function );

			goto on_error;
		}
		( *destination_single_file_subject )->name_size = subject_single_file_subject->name_size;
	}

	if( subject_single_file_subject->comment != NULL )
	{
		( *destination_single_file_subject )->comment = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * subject_single_file_subject->comment_size );

		if( ( *destination_single_file_subject )->comment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination comment.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_subject )->comment,
		     subject_single_file_subject->comment,
		     subject_single_file_subject->comment_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy subject to destination comment.",
			 function );

			goto on_error;
		}
		( *destination_single_file_subject )->comment_size = subject_single_file_subject->comment_size;
	}

	if( subject_single_file_subject->number != NULL )
	{
		( *destination_single_file_subject )->number = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * subject_single_file_subject->number_size);

		if( ( *destination_single_file_subject )->number == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination number.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_single_file_subject )->number,
		     subject_single_file_subject->number,
		     subject_single_file_subject->number_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy subject to destination number.",
			 function );

			goto on_error;
		}
		( *destination_single_file_subject )->number_size = subject_single_file_subject->number_size;
	}

	if( subject_single_file_subject->guid != NULL)
	{
		( *destination_single_file_subject )->guid = ( uint8_t *)memory_allocate(
			                                                sizeof( uint8_t ) * subject_single_file_subject->guid_size );

		if( ( *destination_single_file_subject )->guid == NULL )
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
			( *destination_single_file_subject )->guid,
			subject_single_file_subject->guid,
			subject_single_file_subject->guid_size ) == NULL )
		{
			libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			"%s: unable to copy subject to destination GUID.",
			function );

			goto on_error;
		}
		( *destination_single_file_subject )->guid_size = subject_single_file_subject->guid_size;
	}
	return( 1 );

on_error:
	if( *destination_single_file_subject != NULL )
	{
        if( ( *destination_single_file_subject)->guid != NULL )
        {
            memory_free(
             ( *destination_single_file_subject )->guid );
        }
		if( ( *destination_single_file_subject )->name != NULL )
		{
			memory_free(
			 ( *destination_single_file_subject )->name );
		}
		if( ( *destination_single_file_subject )->comment != NULL )
		{
			memory_free(
			 ( *destination_single_file_subject )->comment );
		}
		if( ( *destination_single_file_subject )->number != NULL )
		{
			memory_free(
			 ( *destination_single_file_subject )->number );
		}
		memory_free(
		 *destination_single_file_subject );

		*destination_single_file_subject = NULL;
	}
	return( -1 );
}

/* Retrieves the id
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_id(
     libewf_single_file_subject_t *single_file_subject,
     uint32_t *id,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_id";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	*id = single_file_subject->id;

	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_type(
     libewf_single_file_subject_t *single_file_subject,
     uint8_t *type,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_type";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	*type = single_file_subject->type;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf8_name_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf8_name_size";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	*utf8_string_size = single_file_subject->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf8_name(
     libewf_single_file_subject_t *single_file_subject,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf8_name";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	if( ( single_file_subject->name == NULL )
	 || ( single_file_subject->name_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_subject->name_size )
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
		     (char *) single_file_subject->name,
		     single_file_subject->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_subject->name_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf16_name_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf16_name_size";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
		 function );

		return( -1 );
	}
	if( ( single_file_subject->name == NULL )
	 || ( single_file_subject->name_size == 0 ) )
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
		     single_file_subject->name,
		     single_file_subject->name_size,
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
int libewf_single_file_subject_get_utf16_name(
     libewf_single_file_subject_t *single_file_subject,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf16_name";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	if( ( single_file_subject->name == NULL )
	 || ( single_file_subject->name_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_subject->name,
		     single_file_subject->name_size,
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

/* Retrieves the size of the UTF-8 encoded comment
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf8_comment_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf8_comment_size";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	*utf8_string_size = single_file_subject->comment_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded comment value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf8_comment(
     libewf_single_file_subject_t *single_file_subject,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf8_comment";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	if( ( single_file_subject->comment == NULL )
	 || ( single_file_subject->comment_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_subject->comment_size )
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
		     (char *) single_file_subject->comment,
		     single_file_subject->comment_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_subject->comment_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded comment
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf16_comment_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf16_comment_size";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
		 function );

		return( -1 );
	}
	if( ( single_file_subject->comment == NULL )
	 || ( single_file_subject->comment_size == 0 ) )
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
		     single_file_subject->comment,
		     single_file_subject->comment_size,
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

/* Retrieves the UTF-16 encoded comment value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf16_comment(
     libewf_single_file_subject_t *single_file_subject,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf16_comment";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	if( ( single_file_subject->comment == NULL )
	 || ( single_file_subject->comment_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_subject->comment,
		     single_file_subject->comment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy comment to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf8_number_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf8_number_size";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	*utf8_string_size = single_file_subject->number_size;

	return( 1 );
}

/* Retrieves the UTF-8 encoded number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf8_number(
     libewf_single_file_subject_t *single_file_subject,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf8_number";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	if( ( single_file_subject->number == NULL )
	 || ( single_file_subject->number_size == 0 ) )
	{
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( utf8_string_size < single_file_subject->number_size )
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
		     (char *) single_file_subject->number,
		     single_file_subject->number_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
		utf8_string[ single_file_subject->number_size - 1 ] = 0;
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded number
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf16_number_size(
     libewf_single_file_subject_t *single_file_subject,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf16_number_size";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
		 function );

		return( -1 );
	}
	if( ( single_file_subject->number == NULL )
	 || ( single_file_subject->number_size == 0 ) )
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
		     single_file_subject->number,
		     single_file_subject->number_size,
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

/* Retrieves the UTF-16 encoded number value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_single_file_subject_get_utf16_number(
     libewf_single_file_subject_t *single_file_subject,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_single_file_subject_get_utf16_number";

	if( single_file_subject == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid single file subject.",
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
	if( ( single_file_subject->number == NULL )
	 || ( single_file_subject->number_size == 0 ) )
	{
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libuna_utf16_string_copy_from_utf8(
		     utf16_string,
		     utf16_string_size,
		     single_file_subject->number,
		     single_file_subject->number_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy number to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded GUID value
* Returns 1 if successful, 0 if value not present or -1 on error
*/
int libewf_single_file_subject_get_utf8_guid(
  libewf_single_file_subject_t *single_file_subject,
  uint8_t *utf8_string,
  size_t utf8_string_size,
  libcerror_error_t **error)
{
  static char *function = "libewf_single_file_subject_get_utf8_guid";

  if (single_file_subject == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid single file subject.",
      function);

    return(-1);
  }
  if (utf8_string == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid UTF-8 string.",
      function);

    return(-1);
  }
  if (utf8_string_size > (size_t)SSIZE_MAX)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
      "%s: invalid UTF-8 string size value exceeds maximum.",
      function);

    return(-1);
  }
  if (utf8_string_size < single_file_subject->guid_size)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
      "%s: UTF-8 string too small.",
      function);

    return(-1);
  }
  if (single_file_subject->guid_size == 0)
  {
    return(0);
  }
  if (memory_copy(
    utf8_string,
    single_file_subject->guid,
    single_file_subject->guid_size) == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_MEMORY,
      LIBCERROR_MEMORY_ERROR_COPY_FAILED,
      "%s: unable to copy GUID to UTF-8 string.",
      function);

    return(-1);
  }
  return(1);
}

/* Retrieves the UTF-16 encoded GUID value
* Returns 1 if successful, 0 if value not present or -1 on error
*/
int libewf_single_file_subject_get_utf16_guid(
  libewf_single_file_subject_t *single_file_subject,
  uint16_t *utf16_string,
  size_t utf16_string_size,
  libcerror_error_t **error)
{
  static char *function = "libewf_single_file_subject_get_utf16_guid";

  if (single_file_subject == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid single file subject.",
      function);

    return(-1);
  }
  if (utf16_string == NULL)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
      "%s: invalid UTF-16 string.",
      function);

    return(-1);
  }
  if (utf16_string_size > (size_t)SSIZE_MAX)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
      "%s: invalid UTF-16 string size value exceeds maximum.",
      function);

    return(-1);
  }
  if (utf16_string_size < single_file_subject->guid_size)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
      LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
      "%s: UTF-16 string too small.",
      function);

    return(-1);
  }
  if (single_file_subject->guid_size == 0)
  {
    return(0);
  }
  if (libuna_utf16_string_copy_from_utf8(
    utf16_string,
    utf16_string_size,
    single_file_subject->guid,
    single_file_subject->guid_size,
    error) != 1)
  {
    libcerror_error_set(
      error,
      LIBCERROR_ERROR_DOMAIN_RUNTIME,
      LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
      "%s: unable to copy GUID to UTF-16 string.",
      function);

    return(-1);
  }
  return(1);
}

/*
 * File extended_attribute functions
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_file_extended_attribute.h"
#include "libewf_handle.h"
#include "libewf_file_entry.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_extended_attribute.h"
#include "libewf_single_file_extended_attribute_tree.h"
#include "libewf_types.h"

/* Creates a file extended_attribute
 * Make sure the value file_extended_attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_initialize(
     libewf_file_extended_attribute_t **file_extended_attribute,
	 libewf_internal_file_entry_t *internal_file_entry,
     libcdata_tree_node_t *file_extended_attribute_tree_node,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_initialize";

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	if( *file_extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file extended_attribute value already set.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = memory_allocate_structure(
	                       	libewf_internal_file_extended_attribute_t );

	if( internal_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file extended_attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_extended_attribute,
	     0,
	     sizeof( libewf_internal_file_extended_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file extended_attribute.",
		 function );

		memory_free(
		 internal_file_extended_attribute );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_extended_attribute->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_file_extended_attribute->internal_file_entry               = internal_file_entry;
	internal_file_extended_attribute->file_extended_attribute_tree_node = file_extended_attribute_tree_node;

	*file_extended_attribute = (libewf_file_extended_attribute_t *) internal_file_extended_attribute;

	return( 1 );

on_error:
	if( internal_file_extended_attribute != NULL )
	{
		memory_free(
		 internal_file_extended_attribute );
	}
	return( -1 );
}

/* Frees a file extended_attribute
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_free(
     libewf_file_extended_attribute_t **file_extended_attribute,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_free";
	int result                                                                  = 1;

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	if( *file_extended_attribute != NULL )
	{
		internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) *file_extended_attribute;
		*file_extended_attribute         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_extended_attribute->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The internal_handle and file_extended_attribute_tree_node references are freed elsewhere
		 */
		memory_free(
		 internal_file_extended_attribute );
	}
	return( result );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_type(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint8_t *type,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_type";

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_extended_attribute->file_extended_attribute_tree_node,
	     (intptr_t **) &single_file_extended_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file extended_attribute tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_extended_attribute_get_type(
	     single_file_extended_attribute,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf8_name_size(
     libewf_file_extended_attribute_t *file_extended_attribute,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf8_name_size";

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_extended_attribute->file_extended_attribute_tree_node,
	     (intptr_t **) &single_file_extended_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file extended_attribute tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_extended_attribute_get_utf8_name_size(
	     single_file_extended_attribute,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf8_name(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf8_name";

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_extended_attribute->file_extended_attribute_tree_node,
	     (intptr_t **) &single_file_extended_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file extended_attribute tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_extended_attribute_get_utf8_name(
	     single_file_extended_attribute,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf16_name_size(
     libewf_file_extended_attribute_t *file_extended_attribute,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf16_name_size";

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_extended_attribute->file_extended_attribute_tree_node,
	     (intptr_t **) &single_file_extended_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file extended_attribute tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_extended_attribute_get_utf16_name_size(
	     single_file_extended_attribute,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf16_name(
     libewf_file_extended_attribute_t *file_extended_attribute,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf16_name";

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_value(
	     internal_file_extended_attribute->file_extended_attribute_tree_node,
	     (intptr_t **) &single_file_extended_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file extended_attribute tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_extended_attribute_get_utf16_name(
	     single_file_extended_attribute,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf8_value_size(
	libewf_file_extended_attribute_t *file_extended_attribute,
	size_t *utf8_value_size,
	libcerror_error_t **error)
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf8_value_size";

	if (file_extended_attribute == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file extended_attribute.",
			function);

		return(-1);
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *)file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_extended_attribute->file_extended_attribute_tree_node,
		(intptr_t **)&single_file_extended_attribute,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file extended_attribute tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_extended_attribute_get_utf8_value_size(
		single_file_extended_attribute,
		utf8_value_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 value size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded value value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf8_value(
	libewf_file_extended_attribute_t *file_extended_attribute,
	uint8_t *utf8_value,
	size_t utf8_value_size,
	libcerror_error_t **error)
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf8_value";

	if (file_extended_attribute == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file extended_attribute.",
			function);

		return(-1);
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *)file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_extended_attribute->file_extended_attribute_tree_node,
		(intptr_t **)&single_file_extended_attribute,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file extended_attribute tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_extended_attribute_get_utf8_value(
		single_file_extended_attribute,
		utf8_value,
		utf8_value_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 value.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf16_value_size(
	libewf_file_extended_attribute_t *file_extended_attribute,
	size_t *utf16_value_size,
	libcerror_error_t **error)
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf16_value_size";

	if (file_extended_attribute == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file extended_attribute.",
			function);

		return(-1);
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *)file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_extended_attribute->file_extended_attribute_tree_node,
		(intptr_t **)&single_file_extended_attribute,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file extended_attribute tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_extended_attribute_get_utf16_value_size(
		single_file_extended_attribute,
		utf16_value_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 value size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded value value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_utf16_value(
	libewf_file_extended_attribute_t *file_extended_attribute,
	uint16_t *utf16_value,
	size_t utf16_value_size,
	libcerror_error_t **error)
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libewf_single_file_extended_attribute_t *single_file_extended_attribute     = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_utf16_value";

	if (file_extended_attribute == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file extended_attribute.",
			function);

		return(-1);
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *)file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to grab read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	if (libcdata_tree_node_get_value(
		internal_file_extended_attribute->file_extended_attribute_tree_node,
		(intptr_t **)&single_file_extended_attribute,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file extended_attribute tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_extended_attribute_get_utf16_value(
		single_file_extended_attribute,
		utf16_value,
		utf16_value_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 value.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to release read/write lock for reading.",
			function);

		return(-1);
	}
#endif
	return(1);

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
		internal_file_extended_attribute->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the number of sub file extended_attributes
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_number_of_sub_file_extended_attributes(
     libewf_file_extended_attribute_t *file_extended_attribute,
     int *number_of_sub_file_extended_attributes,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_number_of_sub_file_extended_attributes";
	int result                                                                  = 0;

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) file_extended_attribute;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libcdata_tree_node_get_number_of_sub_nodes(
	          internal_file_extended_attribute->file_extended_attribute_tree_node,
	          number_of_sub_file_extended_attributes,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file extended_attributes.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file extended_attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
int libewf_file_extended_attribute_get_sub_file_extended_attribute(
     libewf_file_extended_attribute_t *file_extended_attribute,
     int sub_file_extended_attribute_index,
     libewf_file_extended_attribute_t **sub_file_extended_attribute,
     libcerror_error_t **error )
{
	libewf_internal_file_extended_attribute_t *internal_file_extended_attribute = NULL;
	libcdata_tree_node_t *sub_node                                              = NULL;
	static char *function                                                       = "libewf_file_extended_attribute_get_sub_file_extended_attribute";

	if( file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file extended_attribute.",
		 function );

		return( -1 );
	}
	internal_file_extended_attribute = (libewf_internal_file_extended_attribute_t *) file_extended_attribute;

	if( internal_file_extended_attribute->file_extended_attribute_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file extended_attribute - missing file extended_attribute tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file extended_attribute.",
		 function );

		return( -1 );
	}
	if( *sub_file_extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file extended_attribute already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_tree_node_get_sub_node_by_index(
	     internal_file_extended_attribute->file_extended_attribute_tree_node,
             sub_file_extended_attribute_index,
             &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file extended_attribute tree node.",
		 function );

		goto on_error;
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub node.",
		 function );

		goto on_error;
	}
	if( libewf_file_extended_attribute_initialize(
	     sub_file_extended_attribute,
	     internal_file_extended_attribute->internal_file_entry,
	     sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub file extended_attribute.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_extended_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_extended_attribute->read_write_lock,
	 NULL );
#endif
	return( -1 );
}
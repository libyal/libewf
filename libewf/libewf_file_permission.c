/*
 * File permission functions
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_file_permission.h"
#include "libewf_handle.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_libcthreads.h"
#include "libewf_single_file_permission.h"
#include "libewf_single_file_permission_tree.h"
#include "libewf_types.h"

/* Creates a file permission
 * Make sure the value file_permission is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_initialize(
     libewf_file_permission_t **file_permission,
     libewf_internal_handle_t *internal_handle,
     libcdata_tree_node_t *file_permission_tree_node,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	static char *function                             = "libewf_file_permission_initialize";

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	if( *file_permission != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file permission value already set.",
		 function );

		return( -1 );
	}
	internal_file_permission = memory_allocate_structure(
	                       libewf_internal_file_permission_t );

	if( internal_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file permission.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_permission,
	     0,
	     sizeof( libewf_internal_file_permission_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file permission.",
		 function );

		memory_free(
		 internal_file_permission );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_permission->read_write_lock ),
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
	internal_file_permission->internal_handle           = internal_handle;
	internal_file_permission->file_permission_tree_node = file_permission_tree_node;

	*file_permission = (libewf_file_permission_t *) internal_file_permission;

	return( 1 );

on_error:
	if( internal_file_permission != NULL )
	{
		memory_free(
		 internal_file_permission );
	}
	return( -1 );
}

/* Frees a file permission
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_free(
     libewf_file_permission_t **file_permission,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	static char *function                                       = "libewf_file_permission_free";
	int result                                                  = 1;

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	if( *file_permission != NULL )
	{
		internal_file_permission = (libewf_internal_file_permission_t *) *file_permission;
		*file_permission         = NULL;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_permission->read_write_lock ),
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
		/* The internal_handle and file_permission_tree_node references are freed elsewhere
		 */
		memory_free(
		 internal_file_permission );
	}
	return( result );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_type(
     libewf_file_permission_t *file_permission,
     uint8_t *type,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_type";

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	     internal_file_permission->file_permission_tree_node,
	     (intptr_t **) &single_file_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file permission tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_permission_get_type(
	     single_file_permission,
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
	     internal_file_permission->read_write_lock,
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
	 internal_file_permission->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the permission props
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_props(
	libewf_file_permission_t *file_permission,
	uint32_t *props,
	libcerror_error_t **error)
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_props";

	if (file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file permission.",
			function);

		return(-1);
	}
	internal_file_permission = (libewf_internal_file_permission_t *)file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->file_permission_tree_node,
		(intptr_t **)&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file permission tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_permission_get_props(
		single_file_permission,
		props,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve props.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the permission access_mask
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_access_mask(
	libewf_file_permission_t *file_permission,
	uint32_t *access_mask,
	libcerror_error_t **error)
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_access_mask";

	if (file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file permission.",
			function);

		return(-1);
	}
	internal_file_permission = (libewf_internal_file_permission_t *)file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->file_permission_tree_node,
		(intptr_t **)&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file permission tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_permission_get_access_mask(
		single_file_permission,
		access_mask,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve access_mask.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the permission inheritance_flags
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_inheritance_flags(
	libewf_file_permission_t *file_permission,
	uint32_t *inheritance_flags,
	libcerror_error_t **error)
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_inheritance_flags";

	if (file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file permission.",
			function);

		return(-1);
	}
	internal_file_permission = (libewf_internal_file_permission_t *)file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->file_permission_tree_node,
		(intptr_t **)&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file permission tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_permission_get_inheritance_flags(
		single_file_permission,
		inheritance_flags,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve inheritance_flags.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf8_name_size(
     libewf_file_permission_t *file_permission,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf8_name_size";

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	     internal_file_permission->file_permission_tree_node,
	     (intptr_t **) &single_file_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file permission tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_permission_get_utf8_name_size(
	     single_file_permission,
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
	     internal_file_permission->read_write_lock,
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
	 internal_file_permission->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-8 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf8_name(
     libewf_file_permission_t *file_permission,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf8_name";

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	     internal_file_permission->file_permission_tree_node,
	     (intptr_t **) &single_file_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file permission tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_permission_get_utf8_name(
	     single_file_permission,
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
	     internal_file_permission->read_write_lock,
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
	 internal_file_permission->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf16_name_size(
     libewf_file_permission_t *file_permission,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf16_name_size";

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	     internal_file_permission->file_permission_tree_node,
	     (intptr_t **) &single_file_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file permission tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_permission_get_utf16_name_size(
	     single_file_permission,
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
	     internal_file_permission->read_write_lock,
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
	 internal_file_permission->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the UTF-16 encoded name value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf16_name(
     libewf_file_permission_t *file_permission,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf16_name";

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	     internal_file_permission->file_permission_tree_node,
	     (intptr_t **) &single_file_permission,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from file permission tree node.",
		 function );

		goto on_error;
	}
	if( libewf_single_file_permission_get_utf16_name(
	     single_file_permission,
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
	     internal_file_permission->read_write_lock,
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
	 internal_file_permission->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded uid
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf8_uid_size(
	libewf_file_permission_t *file_permission,
	size_t *utf8_uid_size,
	libcerror_error_t **error)
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf8_uid_size";

	if (file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file permission.",
			function);

		return(-1);
	}
	internal_file_permission = (libewf_internal_file_permission_t *)file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->file_permission_tree_node,
		(intptr_t **)&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file permission tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_permission_get_utf8_uid_size(
		single_file_permission,
		utf8_uid_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 uid size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-8 encoded uid value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf8_uid(
	libewf_file_permission_t *file_permission,
	uint8_t *utf8_uid,
	size_t utf8_uid_size,
	libcerror_error_t **error)
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf8_uid";

	if (file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file permission.",
			function);

		return(-1);
	}
	internal_file_permission = (libewf_internal_file_permission_t *)file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->file_permission_tree_node,
		(intptr_t **)&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file permission tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_permission_get_utf8_uid(
		single_file_permission,
		utf8_uid,
		utf8_uid_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 uid.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the size of the UTF-16 encoded uid
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf16_uid_size(
	libewf_file_permission_t *file_permission,
	size_t *utf16_uid_size,
	libcerror_error_t **error)
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf16_uid_size";

	if (file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file permission.",
			function);

		return(-1);
	}
	internal_file_permission = (libewf_internal_file_permission_t *)file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->file_permission_tree_node,
		(intptr_t **)&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file permission tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_permission_get_utf16_uid_size(
		single_file_permission,
		utf16_uid_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-16 uid size.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the UTF-16 encoded uid value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_utf16_uid(
	libewf_file_permission_t *file_permission,
	uint16_t *utf16_uid,
	size_t utf16_uid_size,
	libcerror_error_t **error)
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *single_file_permission     = NULL;
	static char *function                                       = "libewf_file_permission_get_utf16_uid";

	if (file_permission == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			"%s: invalid file permission.",
			function);

		return(-1);
	}
	internal_file_permission = (libewf_internal_file_permission_t *)file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_grab_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->file_permission_tree_node,
		(intptr_t **)&single_file_permission,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve value from file permission tree node.",
			function);

		goto on_error;
	}
	if (libewf_single_file_permission_get_utf16_uid(
		single_file_permission,
		utf16_uid,
		utf16_uid_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to retrieve UTF-8 uid.",
			function);

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if (libcthreads_read_write_lock_release_for_read(
		internal_file_permission->read_write_lock,
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
		internal_file_permission->read_write_lock,
		NULL);
#endif
	return(-1);
}

/* Retrieves the uid of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_number_of_sub_file_permissions(
     libewf_file_permission_t *file_permission,
     int *number_of_sub_file_permissions,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	static char *function                                       = "libewf_file_permission_get_number_of_sub_file_permissions";
	int result                                                  = 0;

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	          internal_file_permission->file_permission_tree_node,
	          number_of_sub_file_permissions,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve uid of sub file entries.",
		 function );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_permission->read_write_lock,
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

/* Retrieves the sub file permission for the specific index
 * Returns 1 if successful or -1 on error
 */
int libewf_file_permission_get_sub_file_permission(
     libewf_file_permission_t *file_permission,
     int sub_file_permission_index,
     libewf_file_permission_t **sub_file_permission,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libcdata_tree_node_t *sub_node                              = NULL;
	static char *function                                       = "libewf_file_permission_get_sub_file_permission";

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

	if( internal_file_permission->file_permission_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file permission - missing file permission tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file permission.",
		 function );

		return( -1 );
	}
	if( *sub_file_permission != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file permission already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	     internal_file_permission->file_permission_tree_node,
             sub_file_permission_index,
             &sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file permission tree node.",
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
	if( libewf_file_permission_initialize(
	     sub_file_permission,
	     internal_file_permission->internal_handle,
	     sub_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub file permission.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_permission->read_write_lock,
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
	 internal_file_permission->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the sub file permission for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub file permission or -1 on error
 */
int libewf_file_permission_get_sub_file_permission_by_id(
     libewf_file_permission_t *file_permission,
	 uint32_t id,
     libewf_file_permission_t **sub_file_permission,
     libcerror_error_t **error )
{
	libewf_internal_file_permission_t *internal_file_permission = NULL;
	libewf_single_file_permission_t *sub_single_file_permission = NULL;
	libcdata_tree_node_t *sub_node                              = NULL;
	static char *function                                       = "libewf_file_permission_get_sub_file_permission_by_id";
	int result                                                  = 0;

	if( file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file permission.",
		 function );

		return( -1 );
	}
	internal_file_permission = (libewf_internal_file_permission_t *) file_permission;

	if( internal_file_permission->file_permission_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file permission - missing file permission tree node.",
		 function );

		return( -1 );
	}
	if( sub_file_permission == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file permission.",
		 function );

		return( -1 );
	}
	if( *sub_file_permission != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub file permission already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_permission->read_write_lock,
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
	result = libewf_single_file_permission_tree_get_sub_node_by_id(
	          internal_file_permission->file_permission_tree_node,
	          id,
	          &sub_node,
	          &sub_single_file_permission,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file permission by id.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libewf_file_permission_initialize(
		     sub_file_permission,
		     internal_file_permission->internal_handle,
		     sub_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub file permission.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_permission->read_write_lock,
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

on_error:
#if defined( HAVE_LIBEWF_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_read(
	 internal_file_permission->read_write_lock,
	 NULL );
#endif
	return( -1 );
}


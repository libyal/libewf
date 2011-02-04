/*
 * The libmfdata header wrapper
 *
 * Copyright (c) 2006-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_LIBMFDATA_H )
#define _LIBEWF_LIBMFDATA_H

#include <common.h>

/* Define HAVE_LOCAL_LIBMFDATA for local use of libmfdata
 */
#if defined( HAVE_LOCAL_LIBMFDATA )

#include <libmfdata_cache.h>
#include <libmfdata_definitions.h>
#include <libmfdata_file.h>
#include <libmfdata_file_list.h>
#include <libmfdata_list.h>
#include <libmfdata_list_element.h>
#include <libmfdata_segment.h>
#include <libmfdata_segment_table.h>
#include <libmfdata_types.h>

#elif defined( HAVE_LIBMFDATA_H )

/* If libtool DLL support is enabled set LIBMFDATA_DLL_IMPORT
 * before including libmfdata.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBMFDATA_DLL_IMPORT
#endif

#include <libmfdata.h>

#else
#error Missing libmfdata.h
#endif

#endif


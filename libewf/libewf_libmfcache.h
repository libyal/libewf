/*
 * The internal libmfcache header
 *
 * Copyright (c) 2006-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEWF_LIBMFCACHE_H )
#define _LIBEWF_LIBMFCACHE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBMFCACHE for local use of libmfcache
 */
#if defined( HAVE_LOCAL_LIBMFCACHE )

#include <libmfcache_cache.h>
#include <libmfcache_date_time.h>
#include <libmfcache_definitions.h>
#include <libmfcache_types.h>

#elif defined( HAVE_LIBMFCACHE_H )

/* If libtool DLL support is enabled set LIBMFCACHE_DLL_IMPORT
 * before including libmfcache.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBMFCACHE_DLL_IMPORT
#endif

#include <libmfcache.h>

#else
#error Missing libmfcache.h
#endif

#endif


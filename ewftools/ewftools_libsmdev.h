/*
 * The internal libsmdev header
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EWFTOOLS_LIBSMDEV_H )
#define _EWFTOOLS_LIBSMDEV_H

#include <common.h>

#if defined( HAVE_LOCAL_LIBSMDEV )

#include <libsmdev_definitions.h>
#include <libsmdev_handle.h>
#include <libsmdev_support.h>
#include <libsmdev_types.h>

#else

/* If libtool DLL support is enabled set LIBSMDEV_DLL_IMPORT
 * before including libcerror.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBSMDEV_DLL_IMPORT
#endif

#include <libsmdev.h>

#endif /* defined( HAVE_LOCAL_LIBSMDEV ) */

#endif /* !defined( _EWFTOOLS_LIBSMDEV_H ) */


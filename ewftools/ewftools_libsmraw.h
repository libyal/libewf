/*
 * The internal libsmraw header
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EWFTOOLS_LIBSMRAW_H )
#define _EWFTOOLS_LIBSMRAW_H

#include <common.h>

#if defined( HAVE_LOCAL_LIBSMRAW )
#include <libsmraw_definitions.h>
#include <libsmraw_handle.h>
#include <libsmraw_metadata.h>
#include <libsmraw_support.h>
#include <libsmraw_types.h>

#elif defined( HAVE_LIBSMRAW_H )
#include <libsmraw.h>

#endif

#endif


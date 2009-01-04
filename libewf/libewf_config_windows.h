/*
 * Configuration file for Windows native compilation
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * - All advertising materials mentioning features or use of this software
 *   must acknowledge the contribution by people stated in the acknowledgements.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBEWF_CONFIG_WINDOWS_H
#define _LIBEWF_CONFIG_WINDOWS_H

#include "libewf_config.h"

/* Windows does not have other platform specific headers
 */
#undef HAVE_CYGWIN_FS_H
#undef HAVE_LINUX_FS_H
#undef HAVE_SYS_DISKLABEL_H
#undef HAVE_SYS_DISK_H

/* Windows does not have <inttypes.h> or <stdint.h>
 */
#undef HAVE_INTTYPES_H
#undef HAVE_STDINT_H

/* Windows does not have <unistd.h> but uses <io.h> instead
 */
#undef HAVE_UNISTD_H
#define HAVE_IO_H 1

/* Windows does not have <sys/ioctl.h> and <sys/utsname.h>
 */
#undef HAVE_SYS_IOCTL_H
#undef HAVE_SYS_UTSNAME_H

/* Use Windows crypto API instead of libcrypto or equivalent
 */
#undef HAVE_OPENSSL_MD5_H
#undef HAVE_OPENSSL_OPENSSLV_H
#undef HAVE_LIBCRYPTO
#define HAVE_WINCPRYPT_H 1

/* Use Windows GUID (UUID) calculation algorithms instead of libuuid
 */
#undef HAVE_UUID_UUID_H
#undef HAVE_LIBUUID
#undef HAVE_UUID_GENERATE_RANDOM
#undef HAVE_UUID_GENERATE_TIME

/* Windows does not have getopt or <glob.h>
 */
#undef HAVE_GETOPT
#undef HAVE_GLOB_H

/* Windows does not have <signal.h>
 */
#undef HAVE_SIGNAL_H

/* If wide character support was enabled
 * Make use of the wide character support functions
 */
#ifdef HAVE_WIDE_CHARACTER_TYPE
#define HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS 1
#endif

/* Define a string for the acquiry system value in the header values
 */
#ifdef LIBEWF_OPERATING_SYSTEM
#undef LIBEWF_OPERATING_SYSTEM
#endif

#define LIBEWF_OPERATING_SYSTEM "Microsoft Windows"

/* Some functions are different within the Windows API
 */
#define HAVE_WINDOWS_API 1

#endif

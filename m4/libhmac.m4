dnl Functions for libhmac

dnl Function to detect if libhmac is available
AC_DEFUN([AC_CHECK_LIBHMAC],
 [dnl Check if parameters were provided
 AS_IF(
  [test x"$ac_cv_with_libhmac" != x && test "x$ac_cv_with_libhmac" != xno && test "x$ac_cv_with_libhmac" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libhmac"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libhmac}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libhmac}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libhmac])
   ])
  ])

 AS_IF(
  [test x"$ac_cv_with_libhmac" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libhmac.h])

  AS_IF(
   [test "x$ac_cv_header_libhmac_h" = xno],
   [ac_cv_libhmac=no],
   [ac_cv_libhmac=yes
   AC_CHECK_LIB(
    hmac,
    libhmac_get_version,
    [ac_cv_libhmac_dummy=yes],
    [ac_cv_libhmac=no])

    dnl TODO check if all LIBHMAC functions are available
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_DEFINE(
   [HAVE_LIBHMAC],
   [1],
   [Define to 1 if you have the `hmac' library (-lhmac).])
  LIBS="-lhmac $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [ac_cv_libhmac_md5=libhmac
  ac_cv_libhmac_sha1=libhmac
  ac_cv_libhmac_sha256=libhmac],
  [ac_cv_libhmac_md5=no
  ac_cv_libhmac_sha1=no
  ac_cv_libhmac_sha256=no])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_SUBST(
   [HAVE_LIBHMAC],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBHMAC],
   [0])
  ])
 ])

dnl Function to detect whether a certain #define is present in a certain WINAPI header
AC_DEFUN([AC_CHECK_WINAPI_DEFINE],
 [AC_CACHE_CHECK(
  [whether $1 defines $2],
  [ac_cv_winapi_define_$2],
  [AC_LANG_PUSH(C)
  AC_COMPILE_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <windows.h>
#include <$1>

#if !defined( $2 )
#error $2 is not defined
#endif]]
    [[]] )],
   [ac_cv_winapi_define_$2=yes],
   [ac_cv_winapi_define_$2=no])
   AC_LANG_POP(C) ])
 ])

dnl Function to detect if wincrypt MD5, SHA1 and SHA256 functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBHMAC_WINCRYPT],
 [AC_SUBST(
  [LIBCRYPTO_LIBADD],
  [-ladvapi32])
 
 AC_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_MD5])
 
 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_MD5" = xyes],
  [ac_cv_libhmac_md5=libadvapi32],
  [ac_cv_libhmac_md5=local])
 
 AC_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA1])
 
 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_SHA1" = xyes],
  [ac_cv_libhmac_sha1=libadvapi32],
  [ac_cv_libhmac_sha1=local])
 
 AC_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA_256])
 
 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_SHA_256" = xyes],
  [ac_cv_libhmac_sha256=libadvapi32],
  [ac_cv_libhmac_sha256=local])
 ])
 
dnl Function to detect whether openssl/evp.h can be used in combination with zlib.h
AC_DEFUN([AC_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE],
 [AC_CACHE_CHECK(
  [if openssl/evp.h can be used in combination with zlib.h],
  [ac_cv_openssl_evp_zlib_compatible],
  [AC_LANG_PUSH(C)
  AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <zlib.h>
#include <openssl/evp.h>]],
    [[ ]] )],
   [ac_cv_openssl_evp_zlib_compatible=yes],
   [ac_cv_openssl_evp_zlib_compatible=no])
  AC_LANG_POP(C)])
 ])

dnl Function to detect if openssl EVP MD5, SHA1 and SHA256 functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBHMAC_OPENSSL_EVP],
 [AC_CHECK_HEADERS([openssl/evp.h])
 
 AS_IF(
  [test "x$ac_cv_header_openssl_evp_h" = xno],
  [ac_cv_libcrypto=no],
  [AC_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE
 
  AS_IF(
   [test "x$ac_cv_openssl_evp_zlib_compatible" = xyes],
   [ac_cv_libcrypto=evp],
   [ac_cv_libcrypto=no])
 ])

 dnl Check if all required libcrypto (openssl) EVP MD functions are available
 AS_IF(
  [test "x$ac_cv_libcrypto" = xevp],
  [AC_CHECK_LIB(
   crypto,
   EVP_MD_CTX_init,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libcrypto=no])
  AC_CHECK_LIB(
   crypto,
   EVP_MD_CTX_cleanup,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libcrypto=no])
  AC_CHECK_LIB(
   crypto,
   EVP_DigestInit_ex,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libcrypto=no])
  AC_CHECK_LIB(
   crypto,
   EVP_DigestUpdate,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libcrypto=no])
  AC_CHECK_LIB(
   crypto,
   EVP_DigestFinal_ex,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libcrypto=no])
  ])
 
 AS_IF(
  [test "x$ac_cv_libcrypto" = xevp],
  [AC_CHECK_LIB(
   crypto,
   EVP_md5,
   [ac_cv_libhmac_md5=libcrypto_evp],
   [ac_cv_libhmac_md5=no])
  AC_CHECK_LIB(
   crypto,
   EVP_sha1,
   [ac_cv_libhmac_sha1=libcrypto_evp],
   [ac_cv_libhmac_sha1=no])
  AC_CHECK_LIB(
   crypto,
   EVP_sha256,
   [ac_cv_libhmac_sha256=libcrypto_evp],
   [ac_cv_libhmac_sha256=no])

  AS_IF(
   [test x"$ac_cv_libhmac_md5" = xlibcrypto_evp],
   [AC_DEFINE(
    [HAVE_EVP_MD5],
    [1],
    [Define to 1 if you have the `EVP_md5' function".])
   ])
  AS_IF(
   [test x"$ac_cv_libhmac_sha1" = xlibcrypto_evp],
   [AC_DEFINE(
    [HAVE_EVP_SHA1],
    [1],
    [Define to 1 if you have the `EVP_sha1' function".])
   ])
  AS_IF(
   [test x"$ac_cv_libhmac_sha256" = xlibcrypto_evp],
   [AC_DEFINE(
    [HAVE_EVP_SHA256],
    [1],
    [Define to 1 if you have the `EVP_sha256' function".])
   ])
  ])
 ])

dnl Function to detect if openssl MD5 functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBHMAC_OPENSSL_MD5],
 [AC_CHECK_HEADERS([openssl/md5.h])
 
 AS_IF(
  [test "x$ac_cv_header_openssl_md5_h" = xno],
  [ac_cv_libhmac_md5=no],
  [AC_CHECK_LIB(
   crypto,
   MD5_Init,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Update,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Final,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_md5=no])
  ])
 ])
 
dnl Function to detect if openssl SHA1 functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBHMAC_OPENSSL_SHA1],
 [AC_CHECK_HEADERS([openssl/sha.h])
 
 AS_IF(
  [test "x$ac_cv_header_openssl_sha_h" = xno],
  [ac_cv_libhmac_sha1=no],
  [AC_CHECK_LIB(
   crypto,
   SHA1_Init,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_sha1=no])
  AC_CHECK_LIB(
   crypto,
   SHA1_Update,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_sha1=no])
  AC_CHECK_LIB(
   crypto,
   SHA1_Final,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_sha1=no])
  ])
 ])
 
dnl Function to detect if openssl SHA256 functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBHMAC_OPENSSL_SHA256],
 [AC_CHECK_HEADERS([openssl/sha.h])
 
 AS_IF(
  [test "x$ac_cv_header_openssl_sha_h" = xno],
  [ac_cv_libhmac_sha256=no],
  [AC_CHECK_LIB(
   crypto,
   SHA256_Init,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_sha256=no])
  AC_CHECK_LIB(
   crypto,
   SHA256_Update,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_sha256=no])
  AC_CHECK_LIB(
   crypto,
   SHA256_Final,
   [ac_cv_libhmac_dummy=yes],
   [ac_cv_libhmac_sha256=no])
  ])
 ])
 
dnl Function to detect if libhmac dependencies are available
AC_DEFUN([AC_CHECK_LOCAL_LIBHMAC],
 [dnl Check if parameters were provided 
 AS_IF(
  [test x"$ac_cv_with_openssl" != x && test "x$ac_cv_with_openssl" != xno && test "x$ac_cv_with_openssl" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_openssl"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_openssl}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_openssl}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_openssl])
   ])
  ])

 ac_cv_libcrypto=no 
 ac_cv_libhmac_md5=no
 ac_cv_libhmac_sha1=no
 ac_cv_libhmac_sha256=no

 dnl Check for Windows crypto API support
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [AC_CHECK_LOCAL_LIBHMAC_WINCRYPT])

 dnl Check for libcrypto (openssl) support
 AS_IF(
  [test "x$ac_cv_with_openssl" != xno],
  [dnl Check for libcrypto (openssl) EVP support
  AS_IF(
   [test "x$ac_cv_libhmac_md5" = xno && test "x$ac_cv_libhmac_sha1" = xno && test "x$ac_cv_libhmac_sha256" = xno],
   [AC_CHECK_LOCAL_LIBHMAC_OPENSSL_EVP])

  dnl Check for libcrypto (openssl) MD5 support
  AS_IF(
   [test "x$ac_cv_with_openssl" != xno && test "x$ac_cv_libhmac_md5" = xno],
   [AC_CHECK_LOCAL_LIBHMAC_OPENSSL_MD5])
 
  dnl Check for libcrypto (openssl) SHA1 support
  AS_IF(
   [test "x$ac_cv_with_openssl" != xno && test "x$ac_cv_libhmac_sha1" = xno],
   [AC_CHECK_LOCAL_LIBHMAC_OPENSSL_SHA1])
  
  dnl Check for libcrypto (openssl) SHA256 support
  AS_IF(
   [test "x$ac_cv_with_openssl" != xno && test "x$ac_cv_libhmac_sha256" = xno],
   [AC_CHECK_LOCAL_LIBHMAC_OPENSSL_SHA256])
  ])
  
 dnl Fallback to local versions if necessary 
 AS_IF(
  [test "x$ac_cv_libhmac_md5" = xno],
  [ac_cv_libhmac_md5=local])
 
 AS_IF(
  [test "x$ac_cv_libhmac_sha1" = xno],
  [ac_cv_libhmac_sha1=local])
  
 AS_IF(
  [test "x$ac_cv_libhmac_sha256" = xno],
  [ac_cv_libhmac_sha256=local])

 AS_IF(
  [test "x$ac_cv_libcrypto" = xno],
  [AS_IF(
   [test "x$ac_cv_libhmac_md5" = xlibcrypto || test "x$ac_cv_libhmac_sha1" = xlibcrypto || test "x$ac_cv_libhmac_sha256" = xlibcrypto],
   [ac_cv_libcrypto=yes])
  ])

 dnl Setup libcrypto (openssl) parameters
 AS_IF(
  [test "x$ac_cv_libcrypto" = xevp],
  [AC_DEFINE(
   [HAVE_OPENSSL_EVP_H],
   [1],
   [Define to 1 if you have the <openssl/evp.h> header file.])
  AC_SUBST(
   [HAVE_OPENSSL_EVP_H],
   [1]) ],
  [AC_SUBST(
   [HAVE_OPENSSL_EVP_H],
   [0])
  ])
 
 AS_IF(
  [test "x$ac_cv_libcrypto" != xno],
  [AC_CHECK_HEADERS([openssl/opensslv.h])

  AC_SUBST(
   [LIBCRYPTO_LIBADD],
   ["-lcrypto"])
  AC_DEFINE(
   [HAVE_LIBCRYPTO],
   [1],
   [Define to 1 if you have the 'crypto' library (-lcrypto).])
  dnl Enforce the dynamic loader library to be included if available
  AC_CHECK_LIB(
   dl,
   dlopen,
   [AC_SUBST(
    [LIBDL_LIBADD],
    ["-ldl"])],
   [])
  ])
 ])


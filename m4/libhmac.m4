dnl Functions for libhmac
dnl
dnl Version: 20111007

dnl Function to detect if libhmac is available
dnl ac_libhmac_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBHMAC_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libhmac" != x && test "x$ac_cv_with_libhmac" != xno && test "x$ac_cv_with_libhmac" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libhmac"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libhmac}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libhmac}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libhmac])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libhmac" != xno],
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

dnl Function to detect if libhmac dependencies are available
AC_DEFUN([AX_LIBHMAC_CHECK_LOCAL],
 [ac_cv_libhmac_md5=no
 ac_cv_libhmac_sha1=no
 ac_cv_libhmac_sha256=no

 dnl Check for Windows crypto API support
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [AX_WINCRYPT_CHECK_LIB

  AS_IF(
   [test "x$ac_cv_wincrypt" != xno],
   [AX_WINCRYPT_CHECK_MD5
   AX_WINCRYPT_CHECK_SHA1
   AX_WINCRYPT_CHECK_SHA256

   ac_cv_libhmac_md5=$ac_cv_wincrypt_md5
   ac_cv_libhmac_sha1=$ac_cv_wincrypt_sha1
   ac_cv_libhmac_sha256=$ac_cv_wincrypt_sha256
  ])
 ])

 dnl Check for libcrypto (openssl) support
 AS_IF(
  [test "x$ac_cv_libhmac_md5" = xno && test "x$ac_cv_libhmac_sha1" = xno && test "x$ac_cv_libhmac_sha256" = xno],
  [AX_LIBCRYPTO_CHECK_ENABLE

  AS_IF(
   [test "x$ac_cv_libcrypto" != xno],
   [AX_LIBCRYPTO_CHECK_MD5
   AX_LIBCRYPTO_CHECK_SHA1
   AX_LIBCRYPTO_CHECK_SHA256

   ac_cv_libhmac_md5=$ac_cv_libcrypto_md5
   ac_cv_libhmac_sha1=$ac_cv_libcrypto_sha1
   ac_cv_libhmac_sha256=$ac_cv_libcrypto_sha256
  ])
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
 ])

dnl Function to detect how to enable libhmac
AC_DEFUN([AX_LIBHMAC_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libhmac],
  [libhmac],
  [search for libhmac in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBHMAC_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libhmac" != xyes],
  [AX_LIBHMAC_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBHMAC],
   [1],
   [Define to 1 if the local version of libhmac is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBHMAC],
   [1])
  AC_SUBST(
   [LIBHMAC_CPPFLAGS],
   [-I../libhmac])
  AC_SUBST(
   [LIBHMAC_LIBADD],
   [../libhmac/libhmac.la])

  ac_cv_libhmac=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBHMAC],
  [test "x$ac_cv_libhmac" = xlocal])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_SUBST(
   [ax_libhmac_pc_libs_private],
   [-lhmac])
  ])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_SUBST(
   [ax_libhmac_spec_requires],
   [libhmac])
  AC_SUBST(
   [ax_libhmac_spec_build_requires],
   [libhmac-devel])
  ])
 ])


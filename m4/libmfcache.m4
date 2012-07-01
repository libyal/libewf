dnl Functions for libmfcache
dnl
dnl Version: 20120519

dnl Function to detect if libmfcache is available
dnl ac_libmfcache_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBMFCACHE_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libmfcache" != x && test "x$ac_cv_with_libmfcache" != xno && test "x$ac_cv_with_libmfcache" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libmfcache"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libmfcache}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libmfcache}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libmfcache])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libmfcache" = xno],
  [ac_cv_libmfcache=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libmfcache],
    [libmfcache >= 20120425],
    [ac_cv_libmfcache=yes],
    [ac_cv_libmfcache=no])
   ])

  AS_IF(
   [test "x$ac_cv_libmfcache" = xyes],
   [ac_cv_libmfcache_CPPFLAGS="$pkg_cv_libmfcache_CFLAGS"
   ac_cv_libmfcache_LIBADD="$pkg_cv_libmfcache_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libmfcache.h])
 
   AS_IF(
    [test "x$ac_cv_header_libmfcache_h" = xno],
    [ac_cv_libmfcache=no],
    [dnl Check for the individual functions
    ac_cv_libmfcache=yes

    AC_CHECK_LIB(
     mfcache,
     libmfcache_get_version,
     [ac_cv_libmfcache_dummy=yes],
     [ac_cv_libmfcache=no])
  
    dnl TODO add functions

    ac_cv_libmfcache_LIBADD="-lmfcache"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libmfcache" = xyes],
  [AC_DEFINE(
   [HAVE_LIBMFCACHE],
   [1],
   [Define to 1 if you have the `mfcache' library (-lmfcache).])
  ])

 AS_IF(
  [test "x$ac_cv_libmfcache" = xyes],
  [AC_SUBST(
   [HAVE_LIBMFCACHE],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBMFCACHE],
   [0])
  ])
 ])

dnl Function to detect if libmfcache dependencies are available
AC_DEFUN([AX_LIBMFCACHE_CHECK_LOCAL],
 [dnl Types used in libmfcache/libmfcache_date_time.h
 AC_STRUCT_TM

 dnl Headers included in libmfcache/libmfcache_date_time.h
 AC_HEADER_TIME

 dnl Date and time functions used in libmfcache/libmfcache_date_time.h
 AC_CHECK_FUNCS(
  [time],
  [],
  [AC_MSG_FAILURE(
   [Missing function: time],
   [1])
  ])
 ])

dnl Function to detect how to enable libmfcache
AC_DEFUN([AX_LIBMFCACHE_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libmfcache],
  [libmfcache],
  [search for libmfcache in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBMFCACHE_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libmfcache" != xyes],
  [AX_LIBMFCACHE_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBMFCACHE],
   [1],
   [Define to 1 if the local version of libmfcache is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBMFCACHE],
   [1])

  ac_cv_libmfcache_CPPFLAGS="-I../libmfcache";
  ac_cv_libmfcache_LIBADD="../libmfcache/libmfcache.la";

  ac_cv_libmfcache=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBMFCACHE],
  [test "x$ac_cv_libmfcache" = xlocal])
 AS_IF(
  [test "x$ac_cv_libmfcache_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBMFCACHE_CPPFLAGS],
   [$ac_cv_libmfcache_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libmfcache_LIBADD" != "x"],
  [AC_SUBST(
   [LIBMFCACHE_LIBADD],
   [$ac_cv_libmfcache_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libmfcache" = xyes],
  [AC_SUBST(
   [ax_libmfcache_pc_libs_private],
   [-lmfcache])
  ])

 AS_IF(
  [test "x$ac_cv_libmfcache" = xyes],
  [AC_SUBST(
   [ax_libmfcache_spec_requires],
   [libmfcache])
  AC_SUBST(
   [ax_libmfcache_spec_build_requires],
   [libmfcache-devel])
  ])
 ])

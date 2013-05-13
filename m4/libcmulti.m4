dnl Functions for libcmulti
dnl
dnl Version: 20130303

dnl Function to detect if libcmulti is available
dnl ac_libcmulti_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCMULTI_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libcmulti" != x && test "x$ac_cv_with_libcmulti" != xno && test "x$ac_cv_with_libcmulti" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libcmulti"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libcmulti}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libcmulti}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libcmulti])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libcmulti" = xno],
  [ac_cv_libcmulti=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libcmulti],
    [libcmulti >= 20120701],
    [ac_cv_libcmulti=yes],
    [ac_cv_libcmulti=no])
   ])

  AS_IF(
   [test "x$ac_cv_libcmulti" = xyes],
   [ac_cv_libcmulti_CPPFLAGS="$pkg_cv_libcmulti_CFLAGS"
   ac_cv_libcmulti_LIBADD="$pkg_cv_libcmulti_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libcmulti.h])

   AS_IF(
    [test "x$ac_cv_header_libcmulti_h" = xno],
    [ac_cv_libcmulti=no],
    [dnl Check for the individual functions
    ac_cv_libcmulti=yes

    AC_CHECK_LIB(
     cmulti,
     libcmulti_get_version,
     [ac_cv_libcmulti_dummy=yes],
     [ac_cv_libcmulti=no])
   
    dnl TODO add functions
 
    ac_cv_libcmulti_LIBADD="-lcmulti"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libcmulti" = xyes],
  [AC_DEFINE(
   [HAVE_LIBCMULTI],
   [1],
   [Define to 1 if you have the `cmulti' library (-lcmulti).])
  ])

 AS_IF(
  [test "x$ac_cv_libcmulti" = xyes],
  [AC_SUBST(
   [HAVE_LIBCMULTI],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBCMULTI],
   [0])
  ])
 ])

dnl Function to detect how to enable libcmulti
AC_DEFUN([AX_LIBCMULTI_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libcmulti],
  [libcmulti],
  [search for libcmulti in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBCMULTI_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libcmulti" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBCMULTI],
   [1],
   [Define to 1 if the local version of libcmulti is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBCMULTI],
   [1])

  ac_cv_libcmulti_CPPFLAGS="-I../libcmulti";
  ac_cv_libcmulti_LIBADD="../libcmulti/libcmulti.la";

  ac_cv_libcmulti=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBCMULTI],
  [test "x$ac_cv_libcmulti" = xlocal])
 AS_IF(
  [test "x$ac_cv_libcmulti_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBCMULTI_CPPFLAGS],
   [$ac_cv_libcmulti_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libcmulti_LIBADD" != "x"],
  [AC_SUBST(
   [LIBCMULTI_LIBADD],
   [$ac_cv_libcmulti_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libcmulti" = xyes],
  [AC_SUBST(
   [ax_libcmulti_pc_libs_private],
   [-lcmulti])
  ])

 AS_IF(
  [test "x$ac_cv_libcmulti" = xyes],
  [AC_SUBST(
   [ax_libcmulti_spec_requires],
   [libcmulti])
  AC_SUBST(
   [ax_libcmulti_spec_build_requires],
   [libcmulti-devel])
  ])
 ])


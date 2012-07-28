dnl Functions for bzip2
dnl
dnl Version: 20120728

dnl Function to detect if bzip2 is available
AC_DEFUN([AX_BZIP2_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_bzip2" != x && test "x$ac_cv_with_bzip2" != xno && test "x$ac_cv_with_bzip2" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_bzip2"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_bzip2}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_bzip2}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_bzip2])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_bzip2" = xno],
  [ac_cv_bzip2=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [bzip2],
    [bzip2 >= 1.0],
    [ac_cv_bzip2=bzip2],
    [ac_cv_bzip2=no])
   ])

  AS_IF(
   [test "x$ac_cv_bzip2" = xbzip2],
   [ac_cv_bzip2_CPPFLAGS="$pkg_cv_bzip2_CFLAGS"
   ac_cv_bzip2_LIBADD="$pkg_cv_bzip2_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([bzlib.h])

   AS_IF(
    [test "x$ac_cv_header_bzlib_h" = xno],
    [ac_cv_bzip2=no],
    [dnl Check for the individual functions
    ac_cv_bzip2=bzip2
    AC_CHECK_LIB(
     bz2,
     BZ2_bzlibVersion,
     [],
     [ac_cv_bzip2=no])

    AC_CHECK_LIB(
     bz2,
     BZ2_bzBuffToBuffCompress,
     [ac_bzip2_dummy=yes],
     [ac_cv_bzip2=no])

    AC_CHECK_LIB(
     bz2,
     BZ2_bzBuffToBuffDecompress,
     [ac_bzip2_dummy=yes],
     [ac_cv_bzip2=no])

    ac_cv_bzip2_LIBADD="-lbz2";
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_bzip2" = xbzip2],
  [AC_DEFINE(
   [HAVE_BZIP2],
   [1],
   [Define to 1 if you have the 'bzip2' library (-lbz2).])
  ])

 AS_IF(
  [test "x$ac_cv_bzip2" != xno],
  [AC_SUBST(
   [HAVE_BZIP2],
   [1]) ],
  [AC_SUBST(
   [HAVE_BZIP2],
   [0])
  ])
 ])

dnl Function to detect how to enable bzip2
AC_DEFUN([AX_BZIP2_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [bzip2],
  [bzip2],
  [search for bzip2 in includedir and libdir or in the specified DIR, or no if not to use bzip2],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_BZIP2_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_bzip2_CPPFLAGS" != "x"],
  [AC_SUBST(
   [BZIP2_CPPFLAGS],
   [$ac_cv_bzip2_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_bzip2_LIBADD" != "x"],
  [AC_SUBST(
   [BZIP2_LIBADD],
   [$ac_cv_bzip2_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_bzip2" = xbzip2],
  [AC_SUBST(
   [ax_bzip2_pc_libs_private],
   [-lbz2])
  ])

 AS_IF(
  [test "x$ac_cv_bzip2" = xbzip2],
  [AC_SUBST(
   [ax_bzip2_spec_requires],
   [bzip2-libs])
  AC_SUBST(
   [ax_bzip2_spec_build_requires],
   [bzip2-devel])
  ])
 ])


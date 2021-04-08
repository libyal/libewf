dnl Checks for bzip2 required headers and functions
dnl
dnl Version: 20210403

dnl Function to detect if bzip2 is available
AC_DEFUN([AX_BZIP2_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_bzip2" = xno],
    [ac_cv_bzip2=no],
    [ac_cv_bzip2=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_bzip2" != x && test "x$ac_cv_with_bzip2" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_bzip2"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_bzip2}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_bzip2}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_bzip2],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [bzip2],
          [bzip2 >= 1.0.6],
          [ac_cv_bzip2=bzlib],
          [ac_cv_bzip2=check])
        ])
      AS_IF(
        [test "x$ac_cv_bzip2" = xbzlib],
        [ac_cv_bzip2_CPPFLAGS="$pkg_cv_bzip2_CFLAGS"
        ac_cv_bzip2_LIBADD="$pkg_cv_bzip2_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_bzip2" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([bzlib.h])

      AS_IF(
        [test "x$ac_cv_header_bzlib_h" = xno],
        [ac_cv_bzip2=no],
        [dnl Check for the individual functions
        ac_cv_bzip2=bzlib

        AC_CHECK_LIB(
          bz2,
          BZ2_bzlibVersion,
          [],
          [ac_cv_bzip2=no])

        AS_IF(
          [test "x$ac_cv_lib_bzipb_BZ2_bzlibVersion" = xno],
          [AC_MSG_FAILURE(
            [Missing function: BZ2_bzlibVersion in library: bzlib.],
            [1])
          ])

        ac_cv_bzip2_LIBADD="-lbz2";
        ])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_bzip2" = xbzlib],
    [AC_DEFINE(
      [HAVE_BZLIB],
      [1],
      [Define to 1 if you have the 'bzlib' library (-lbz2).])
    ])

  AS_IF(
    [test "x$ac_cv_bzip2" != xno],
    [AC_SUBST(
      [HAVE_BZLIB],
      [1]) ],
    [AC_SUBST(
      [HAVE_BZLIB],
      [0])
    ])
  ])

dnl Function to detect if the BZ2_bzBuffToBuffDecompress function is available
AC_DEFUN([AX_BZIP2_CHECK_DECOMPRESS],
  [AS_IF(
    [test "x$ac_cv_bzip2" != xbzlib],
    [ac_cv_bzip2_decompress=local],
    [AC_CHECK_LIB(
      bz2,
      BZ2_bzBuffToBuffDecompress,
      [ac_cv_bzip2_decompress=bzlib],
      [ac_cv_bzip2_decompress=local])

    AS_IF(
      [test "x$ac_cv_bzip2_decompress" = xbzlib],
      [AC_DEFINE(
        [HAVE_BZLIB_DECOMPRESS],
        [1],
        [Define to 1 if you have the `BZ2_bzBuffToBuffDecompress' function.])
      ])
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
    [test "x$ac_cv_bzip2" = xbzlib],
    [AC_SUBST(
      [ax_bzip2_pc_libs_private],
      [-lbz2])
    ])

  AS_IF(
    [test "x$ac_cv_bzip2" = xbzlib],
    [AC_SUBST(
      [ax_bzip2_spec_requires],
      [bzip2])
    AC_SUBST(
      [ax_bzip2_spec_build_requires],
      [bzip2-devel])
    AC_SUBST(
      [ax_bzip2_static_spec_requires],
      [bzip2-static])
    AC_SUBST(
      [ax_bzip2_static_spec_build_requires],
      [bzip2-static])
    ])
  ])


ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	include \
	common \
	libcstring \
	liberror \
	libnotify \
	libuna \
	libbfio \
	libfvalue \
	libmfcache \
	libmfdata \
	libewf \
	libhmac \
	libodraw \
	libsmdev \
	libsmraw \
	libsystem \
	ewftools \
	ewf.net \
	pyewf \
	po \
	documents \
	manuals \
	tests \
	examples \
	macosx \
	borlandc \
	msvscpp

DPKG_FILES = \
	dpkg/changelog \
	dpkg/changelog.in \
       	dpkg/compat \
       	dpkg/control \
       	dpkg/copyright \
       	dpkg/copyright.in \
       	dpkg/rules \
       	dpkg/libewf-dev.docs \
       	dpkg/libewf.docs \
       	dpkg/libewf-tools.docs

GETTEXT_FILES = \
	config.rpath \
	po/Makevars.in

PKGCONFIG_FILES = \
	libewf.pc.in

SPEC_FILES = \
	libewf.spec \
	libewf.spec.in

EXTRA_DIST = \
	README.dll \
	README.macosx \
	README.mingw \
	README.static \
	$(DPKG_FILES) \
	$(GETTEXT_FILES) \
	$(PKGCONFIG_FILES) \
	$(SPEC_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = \
	libewf.pc

libtool: @LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck

lib: library

library:
	(cd $(srcdir)/common && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcstring && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/liberror && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libnotify && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libmfcache && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libmfdata && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libewf && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) $(AM_MAKEFLAGS))

distclean: clean
	/bin/rm -f Makefile
	/bin/rm -f config.status
	/bin/rm -f config.cache
	/bin/rm -f config.log
	/bin/rm -f libewf.pc
	/bin/rm -f libewf.spec
	/bin/rm -f dpkg/changelog
	/bin/rm -f dpkg/shlibs.local.ex
	@for dir in ${subdirs}; do \
		(cd $$dir && $(MAKE) distclean) \
		|| case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

splint:
	(cd $(srcdir)/libcstring && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/liberror && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libnotify && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libmfcache && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libmfdata && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libewf && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libhmac && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libodraw && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsmdev && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsmraw && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsystem && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/ewftools && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/tests && $(MAKE) splint $(AM_MAKEFLAGS))


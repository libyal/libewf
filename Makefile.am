ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	include \
	common \
	libcstring \
	liberror \
	libnotify \
	libuna \
	libbfio \
	libmfdata \
	libfvalue \
	libewf \
	libsmdev \
	libsmraw \
	libsystem \
	ewftools \
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
	dpkg/cron.d.ex \
	dpkg/emacsen-install.ex \
	dpkg/emacsen-startup.ex \
	dpkg/emacsen-remove.ex \
	dpkg/init.d.ex \
	dpkg/libewf.install \
	dpkg/libewf.dirs \
	dpkg/libewf.docs \
	dpkg/libewf.postinst.debhelper \
	dpkg/libewf.postrm.debhelper \
	dpkg/libewf-default.ex \
	dpkg/libewf-dev.dirs \
	dpkg/libewf-dev.docs \
	dpkg/libewf-dev.install \
	dpkg/libewf-tools.dirs \
	dpkg/libewf-tools.docs \
	dpkg/libewf-tools.install \
	dpkg/libewf.doc-base.EX \
	dpkg/manpage.1.ex \
	dpkg/manpage.sgml.ex \
	dpkg/manpage.xml.ex \
	dpkg/menu.ex \
	dpkg/rules \
	dpkg/postinst.ex \
	dpkg/postrm.ex \
	dpkg/preinst.ex \
	dpkg/prerm.ex \
	dpkg/shlibs.local.ex \
	dpkg/shlibs.local.ex.in \
	dpkg/watch.ex

SPEC_FILES = \
	libewf.spec \
	libewf.spec.in

EXTRA_DIST = \
	config.rpath  \
	README.mingw \
	po/Makevars.in \
	libewf.pc \
	libewf.pc.in \
	$(DPKG_FILES) \
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
	(cd $(srcdir)/libmfdata && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) $(AM_MAKEFLAGS))
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
	(cd $(srcdir)/libmfdata && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libewf && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsmdev && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsmraw && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsystem && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/ewftools && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/tests && $(MAKE) splint $(AM_MAKEFLAGS))


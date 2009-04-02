SUBDIRS = \
	include \
	common \
	liberror \
	libuna \
	libbfio \
	libewf \
	ewftools \
	pyewf \
	documents \
	manuals \
	macosx \
	msvscpp

DEBIAN_FILES = \
	debian/changelog \
	debian/changelog.in \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/cron.d.ex \
	debian/emacsen-install.ex \
	debian/emacsen-startup.ex \
	debian/emacsen-remove.ex \
	debian/init.d.ex \
	debian/libewf.install \
	debian/libewf.dirs \
	debian/libewf.docs \
	debian/libewf.postinst.debhelper \
	debian/libewf.postrm.debhelper \
	debian/libewf-default.ex \
	debian/libewf-dev.dirs \
	debian/libewf-dev.docs \
	debian/libewf-dev.install \
	debian/libewf-tools.dirs \
	debian/libewf-tools.docs \
	debian/libewf-tools.install \
	debian/libewf.doc-base.EX \
	debian/manpage.1.ex \
	debian/manpage.sgml.ex \
	debian/manpage.xml.ex \
	debian/menu.ex \
	debian/rules \
	debian/postinst.ex \
	debian/postrm.ex \
	debian/preinst.ex \
	debian/prerm.ex \
	debian/shlibs.local.ex \
	debian/shlibs.local.ex.in \
	debian/watch.ex

REDHAT_FILES = \
	libewf.spec \
	libewf.spec.in

EXTRA_DIST = \
	$(DEBIAN_FILES) \
	$(REDHAT_FILES) \
	libewf.pc \
	libewf.pc.in

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = \
	libewf.pc

libtool: @LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck

lib: library

library:
	(cd $(srcdir)/common && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/liberror && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libewf && $(MAKE) $(AM_MAKEFLAGS))

distclean: clean
	/bin/rm -f Makefile
	/bin/rm -f config.status
	/bin/rm -f config.cache
	/bin/rm -f config.log
	/bin/rm -f libewf.pc
	/bin/rm -f libewf.spec
	/bin/rm -f debian/changelog
	/bin/rm -f debian/shlibs.local.ex
	@for dir in ${subdirs}; do \
		(cd $$dir && $(MAKE) distclean) \
		|| case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

splint:
	(cd $(srcdir)/liberror && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libewf && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/ewftools && $(MAKE) splint $(AM_MAKEFLAGS))


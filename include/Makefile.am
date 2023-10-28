include_HEADERS = \
	libewf.h

pkginclude_HEADERS = \
	libewf/codepage.h \
	libewf/definitions.h \
	libewf/error.h \
	libewf/extern.h \
	libewf/features.h \
	libewf/types.h

EXTRA_DIST = \
	libewf.h.in \
	libewf/definitions.h.in \
	libewf/features.h.in \
	libewf/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libewf.h
	-rm -f libewf/definitions.h
	-rm -f libewf/features.h
	-rm -f libewf/types.h
	-rm -f Makefile


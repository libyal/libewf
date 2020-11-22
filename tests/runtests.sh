#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libewf/.libs/libewf.1.dylib && test -f ./pyewf/.libs/pyewf.so;
then
	install_name_tool -change /usr/local/lib/libewf.1.dylib ${PWD}/libewf/.libs/libewf.1.dylib ./pyewf/.libs/pyewf.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};


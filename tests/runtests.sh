#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libewf/.libs/libewf.1.dylib" ] && [ -f ./pyewf/.libs/pyewf.so ]
then
    install_name_tool -change /usr/local/lib/libewf.1.dylib "${PWD}/libewf/.libs/libewf.1.dylib" ./pyewf/.libs/pyewf.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}


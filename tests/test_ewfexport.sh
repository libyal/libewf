#!/bin/bash
#
# ewfexport testing script
#
# Copyright (c) 2006-2010, Joachim Metz <jbmetz@users.sourceforge.net>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

INPUT="input";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

EWFEXPORT="../ewftools/ewfexport";

function test_export_raw
{ 
	echo "Testing ewfexport to raw of input:" $*;

	mkdir ${TMP};

${EWFEXPORT} -q $* <<EOI
raw
tmp/export.raw



EOI

	RESULT=$?;

	rm -rf ${TMP};

	return ${RESULT};
}

function test_export_encase5
{ 
	echo "Testing ewfexport to encase5 of input:" $*;

	mkdir ${TMP};

${EWFEXPORT} -q $* <<EOI
encase5
tmp/export
none
no




EOI

	RESULT=$?;

	rm -rf ${TMP};

	return ${RESULT};
}

function test_export_encase6
{ 
	echo "Testing ewfexport to encase6 of input:" $*;

	mkdir ${TMP};

${EWFEXPORT} -q -d sha1 $* <<EOI
encase6
tmp/export
none
no




EOI

	RESULT=$?;

	rm -rf ${TMP};

	return ${RESULT};
}

function test_export_ewfx
{ 
	echo "Testing ewfexport to ewfx of input:" $*;

	mkdir ${TMP};

${EWFEXPORT} -q -d sha1 $* <<EOI
ewfx
tmp/export
none
no




EOI

	RESULT=$?;

	rm -rf ${TMP};

	return ${RESULT};
}

if ! test -x ${EWFEXPORT};
then
	echo "Missing executable: ${EWFEXPORT}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No input directory found, to test read create input directory and place test files in directory.";

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/*.[eE]01 | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in input directory, to test read place test files in directory.";

	exit ${EXIT_IGNORE};
fi

for FILENAME in `${LS} ${INPUT}/*.[eE]01 | ${TR} ' ' '\n'`;
do
	if ! test_export_raw "${FILENAME}";
	then
		exit ${EXIT_FAILURE};
	fi
done

for FILENAME in `${LS} ${INPUT}/*.[eE]01 | ${TR} ' ' '\n'`;
do
	if ! test_export_encase5 "${FILENAME}";
	then
		exit ${EXIT_FAILURE};
	fi
done

for FILENAME in `${LS} ${INPUT}/*.[eE]01 | ${TR} ' ' '\n'`;
do
	if ! test_export_encase6 "${FILENAME}";
	then
		exit ${EXIT_FAILURE};
	fi
done

for FILENAME in `${LS} ${INPUT}/*.[eE]01 | ${TR} ' ' '\n'`;
do
	if ! test_export_ewfx "${FILENAME}";
	then
		exit ${EXIT_FAILURE};
	fi
done

exit ${EXIT_SUCCESS};


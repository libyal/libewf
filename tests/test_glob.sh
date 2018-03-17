#!/bin/bash
# Library glob testing script
#
# Version: 20180317

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

chr()
{
	local CHR_VALUE=`expr \( \( $1 / 64 \) \* 100 \) + \( \( \( $1 % 64 \) / 8 \) \* 10 \) + \( $1 % 8 \)`;

	printf \\${CHR_VALUE};
} 

seq()
{
	local VALUE=$1;

	local SEQUENCE="";

	while test ${VALUE} -le $2;
	do

		if test ${VALUE} -le 9;
		then
			SEQUENCE="${SEQUENCE} 0${VALUE}";
		else
			SEQUENCE="${SEQUENCE} ${VALUE}";
		fi

		VALUE=`expr ${VALUE} + 1`;
	done

	echo ${SEQUENCE};
}

test_glob_sequence2()
{ 
	local TEST_EXECUTABLE=$1;
	local BASENAME=$2;
	local SCHEMA=$3;
	local FILENAMES=$4;

	local TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	if test "${OSTYPE}" = "msys";
	then
		TEST_PATH="${TMPDIR}\\${BASENAME}";
		FILENAMES=`echo ${FILENAMES} | sed "s?^?${TMPDIR}\\\\\\\\?" | sed "s? ? ${TMPDIR}\\\\\\\\?g"`;
	else
		TEST_PATH="${TMPDIR}/${BASENAME}";
		FILENAMES=`echo ${FILENAMES} | sed "s?^?${TMPDIR}/?" | sed "s? ? ${TMPDIR}/?g"`;
	fi
	echo ${FILENAMES} > ${TMPDIR}/input;

	touch ${FILENAMES};

	TEST_DESCRIPTION="";

	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}" "${TEST_PATH}" > ${TMPDIR}/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		sed 's/\r\n/\n/' -i ${TMPDIR}/output;

		if ! cmp -s ${TMPDIR}/input ${TMPDIR}/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf ${TMPDIR};

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_glob_sequence3()
{ 
	local TEST_EXECUTABLE=$1;
	local BASENAME=$2;
	local SCHEMA=$3;
	local FILENAME=$4;
	local LAST=$5;

	local RESULT=`echo ${SCHEMA} | egrep "^[.][esEL]01$"`;
	local IS_VALID=$?;

	if test ${IS_VALID} -ne 0;
	then
		echo "Unsupported schema: ${SCHEMA}";

		exit ${EXIT_FAILURE};
	fi

	RESULT=`echo ${LAST} | egrep "^[e-zE-Z][0-9a-zA-Z][0-9a-zA-Z]$"`;
	IS_VALID=$?;

	if test ${IS_VALID} -ne 0;
	then
		echo "Unsupported last: ${LAST}";

		exit ${EXIT_FAILURE};
	fi

	FIRST_LETTER=`echo ${SCHEMA} | cut -c 2`;

	RESULT=`echo ${LAST} | egrep "^${FIRST_LETTER}[0-9][0-9]$"`;
	LAST_IS_NUMERIC=$?;

	if test ${LAST_IS_NUMERIC} -eq 0;
	then
		LAST=`echo ${LAST} | cut -c '2 3'`;

		SEQUENCE=`seq 1 ${LAST}`;
	else
		SEQUENCE=`seq 1 99`;
	fi

	FILENAMES=`for NUMBER in ${SEQUENCE}; do echo -n "${FILENAME}.${FIRST_LETTER}${NUMBER} "; echo $FILE; done`;

	if test ${LAST_IS_NUMERIC} -ne 0;
	then
		RESULT=`echo ${LAST} | egrep "^[A-Z][A-Z][A-Z]$"`;
		IS_UPPER_CASE=$?;

		SECOND_ITERATOR=0;
		THIRD_ITERATOR=0;

		if test ${IS_UPPER_CASE} -eq 0;
		then
			if test ${FIRST_LETTER} = "E";
			then
				FIRST_ITERATOR=4;

			elif test ${FIRST_LETTER} = "L";
			then
				FIRST_ITERATOR=11;
			fi

			FIRST_BYTE_VALUE=`expr 65 + ${FIRST_ITERATOR}`;
			SECOND_BYTE_VALUE=`expr 65 + ${SECOND_ITERATOR}`;
			THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
		else
			if test ${FIRST_LETTER} = "e";
			then
				FIRST_ITERATOR=4;

			elif test ${FIRST_LETTER} = "s";
			then
				FIRST_ITERATOR=18;
			fi

			FIRST_BYTE_VALUE=`expr 97 + ${FIRST_ITERATOR}`;
			SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
			THIRD_BYTE_VALUE=`expr 97 + ${THIRD_ITERATOR}`;
		fi

		FIRST_LETTER=`chr ${FIRST_BYTE_VALUE}`;
		SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
		THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;

		EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}";

		until test ${EXTENSION} = ${LAST};
		do
			FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";

			THIRD_ITERATOR=`expr ${THIRD_ITERATOR} + 1`;

			if test ${THIRD_ITERATOR} -ge 26;
			then
				SECOND_ITERATOR=`expr ${SECOND_ITERATOR} + 1`;

				THIRD_ITERATOR=0;
			fi

			if test ${SECOND_ITERATOR} -ge 26;
			then
				FIRST_ITERATOR=`expr ${FIRST_ITERATOR} + 1`;

				SECOND_ITERATOR=0;
			fi

			if test ${FIRST_ITERATOR} -ge 26;
			then
				break;
			fi

			if test ${IS_UPPER_CASE} -eq 0;
			then
				FIRST_BYTE_VALUE=`expr 65 + ${FIRST_ITERATOR}`;
				SECOND_BYTE_VALUE=`expr 65 + ${SECOND_ITERATOR}`;
				THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
			else
				FIRST_BYTE_VALUE=`expr 97 + ${FIRST_ITERATOR}`;
				SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
				THIRD_BYTE_VALUE=`expr 97 + ${THIRD_ITERATOR}`;
			fi

			FIRST_LETTER=`chr ${FIRST_BYTE_VALUE}`;
			SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
			THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;

			EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}";
		done

		FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";
	fi

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	if test "${OSTYPE}" = "msys";
	then
		TEST_PATH="${TMPDIR}\\${BASENAME}";
		FILENAMES=`echo ${FILENAMES} | sed "s?^?${TMPDIR}\\\\\\\\?" | sed "s? ? ${TMPDIR}\\\\\\\\?g"`;
	else
		TEST_PATH="${TMPDIR}/${BASENAME}";
		FILENAMES=`echo ${FILENAMES} | sed "s?^?${TMPDIR}/?" | sed "s? ? ${TMPDIR}/?g"`;
	fi
	echo ${FILENAMES} > ${TMPDIR}/input;

	touch ${FILENAMES};

	TEST_DESCRIPTION="";

	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}" "${TEST_PATH}" > ${TMPDIR}/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		sed 's/\r\n/\n/' -i ${TMPDIR}/output;

		if ! cmp -s ${TMPDIR}/input ${TMPDIR}/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf ${TMPDIR};

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_glob_sequence4()
{ 
	local TEST_EXECUTABLE=$1;
	local BASENAME=$2;
	local SCHEMA=$3;
	local FILENAME=$4;
	local LAST=$5;

	local RESULT=`echo ${SCHEMA} | egrep "^[.][EL]x01$"`;
	local IS_VALID=$?;

	if test ${IS_VALID} -ne 0;
	then
		echo "Unsupported schema: ${SCHEMA}";

		exit ${EXIT_FAILURE};
	fi

	RESULT=`echo ${LAST} | egrep "^[EL][x-z][0-9A-Z][0-9A-Z]$"`;
	IS_VALID=$?;

	if test ${IS_VALID} -ne 0;
	then
		echo "Unsupported last: ${LAST}";

		exit ${EXIT_FAILURE};
	fi

	FIRST_LETTER=`echo ${SCHEMA} | cut -c 2`;
	SECOND_LETTER=`echo ${SCHEMA} | cut -c 3`;

	RESULT=`echo ${LAST} | egrep "^${FIRST_LETTER}${SECOND_LETTER}[0-9][0-9]$"`;
	LAST_IS_NUMERIC=$?;

	if test ${LAST_IS_NUMERIC} -eq 0;
	then
		LAST=`echo ${LAST} | cut -c '3 4'`;

		SEQUENCE=`seq 1 ${LAST}`;
	else
		SEQUENCE=`seq 1 99`;
	fi

	FILENAMES=`for NUMBER in ${SEQUENCE}; do echo -n "${FILENAME}.${FIRST_LETTER}${SECOND_LETTER}${NUMBER} "; echo $FILE; done`;

	if test ${LAST_IS_NUMERIC} -ne 0;
	then
		SECOND_ITERATOR=23;
		THIRD_ITERATOR=0;
		FOURTH_ITERATOR=0;

		SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
		THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
		FOURTH_BYTE_VALUE=`expr 65 + ${FOURTH_ITERATOR}`;

		SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
		THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;
		FOURTH_LETTER=`chr ${FOURTH_BYTE_VALUE}`;

		EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}${FOURTH_LETTER}";

		until test ${EXTENSION} = ${LAST};
		do
			FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";

			FOURTH_ITERATOR=`expr ${FOURTH_ITERATOR} + 1`;

			if test ${FOURTH_ITERATOR} -ge 26;
			then
				THIRD_ITERATOR=`expr ${THIRD_ITERATOR} + 1`;

				FOURTH_ITERATOR=0;
			fi

			if test ${THIRD_ITERATOR} -ge 26;
			then
				SECOND_ITERATOR=`expr ${SECOND_ITERATOR} + 1`;

				THIRD_ITERATOR=0;
			fi

			if test ${SECOND_ITERATOR} -ge 26;
			then
				break;
			fi

			SECOND_BYTE_VALUE=`expr 97 + ${SECOND_ITERATOR}`;
			THIRD_BYTE_VALUE=`expr 65 + ${THIRD_ITERATOR}`;
			FOURTH_BYTE_VALUE=`expr 65 + ${FOURTH_ITERATOR}`;

			SECOND_LETTER=`chr ${SECOND_BYTE_VALUE}`;
			THIRD_LETTER=`chr ${THIRD_BYTE_VALUE}`;
			FOURTH_LETTER=`chr ${FOURTH_BYTE_VALUE}`;

			EXTENSION="${FIRST_LETTER}${SECOND_LETTER}${THIRD_LETTER}${FOURTH_LETTER}";
		done

		FILENAMES="${FILENAMES} ${FILENAME}.${EXTENSION}";
	fi

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	if test "${OSTYPE}" = "msys";
	then
		TEST_PATH="${TMPDIR}\\${BASENAME}";
		FILENAMES=`echo ${FILENAMES} | sed "s?^?${TMPDIR}\\\\\\\\?" | sed "s? ? ${TMPDIR}\\\\\\\\?g"`;
	else
		TEST_PATH="${TMPDIR}/${BASENAME}";
		FILENAMES=`echo ${FILENAMES} | sed "s?^?${TMPDIR}/?" | sed "s? ? ${TMPDIR}/?g"`;
	fi
	echo ${FILENAMES} > ${TMPDIR}/input;

	touch ${FILENAMES};

	TEST_DESCRIPTION="";

	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_EXECUTABLE}" "${TEST_PATH}" > ${TMPDIR}/output;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		sed 's/\r\n/\n/' -i ${TMPDIR}/output;

		if ! cmp -s ${TMPDIR}/input ${TMPDIR}/output;
		then
			RESULT=${EXIT_FAILURE};
		fi
	fi

	rm -rf ${TMPDIR};

	echo -n "Testing glob: for basename: ${BASENAME} and schema: ${SCHEMA} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_LIBRARY_TESTS};
then
	exit ${EXIT_IGNORE};
fi

OPERATING_SYSTEM=`uname -o 2> /dev/null`;

if test "${OPERATING_SYSTEM}" = "Cygwin" || test "${OPERATING_SYSTEM}" = "Msys";
then
	# The glob tests run very slow on Cygwin and Msys.

	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="./ewf_test_glob";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="ewf_test_glob.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

# .e01

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.e01" ".e01" "PREFIX.e01";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.e01" ".e01" "PREFIX.e01 PREFIX.e02 PREFIX.e03 PREFIX.e04 PREFIX.e05 PREFIX.e06 PREFIX.e07 PREFIX.e08 PREFIX.e09";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.e01" ".e01" "PREFIX.e01 PREFIX.e02 PREFIX.e03 PREFIX.e04 PREFIX.e05 PREFIX.e06 PREFIX.e07 PREFIX.e08 PREFIX.e09 PREFIX.e10 PREFIX.e11";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.e01" ".e01" "PREFIX" "eba";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.e01" ".e01" "PREFIX" "faa";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

# .s01

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.s01" ".s01" "PREFIX.s01";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.s01" ".s01" "PREFIX.s01 PREFIX.s02 PREFIX.s03 PREFIX.s04 PREFIX.s05 PREFIX.s06 PREFIX.s07 PREFIX.s08 PREFIX.s09";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.s01" ".s01" "PREFIX.s01 PREFIX.s02 PREFIX.s03 PREFIX.s04 PREFIX.s05 PREFIX.s06 PREFIX.s07 PREFIX.s08 PREFIX.s09 PREFIX.s10 PREFIX.s11";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.s01" ".s01" "PREFIX" "sba";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.s01" ".s01" "PREFIX" "taa";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

# .E01

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.E01" ".E01" "PREFIX.E01";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.E01" ".E01" "PREFIX.E01 PREFIX.E02 PREFIX.E03 PREFIX.E04 PREFIX.E05 PREFIX.E06 PREFIX.E07 PREFIX.E08 PREFIX.E09";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.E01" ".E01" "PREFIX.E01 PREFIX.E02 PREFIX.E03 PREFIX.E04 PREFIX.E05 PREFIX.E06 PREFIX.E07 PREFIX.E08 PREFIX.E09 PREFIX.E10 PREFIX.E11";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.E01" ".E01" "PREFIX" "EBA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.E01" ".E01" "PREFIX" "FAA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

# .L01

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.L01" ".L01" "PREFIX.L01";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.L01" ".L01" "PREFIX.L01 PREFIX.L02 PREFIX.L03 PREFIX.L04 PREFIX.L05 PREFIX.L06 PREFIX.L07 PREFIX.L08 PREFIX.L09";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.L01" ".L01" "PREFIX.L01 PREFIX.L02 PREFIX.L03 PREFIX.L04 PREFIX.L05 PREFIX.L06 PREFIX.L07 PREFIX.L08 PREFIX.L09 PREFIX.L10 PREFIX.L11";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.L01" ".L01" "PREFIX" "LBA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence3 "${TEST_EXECUTABLE}" "PREFIX.L01" ".L01" "PREFIX" "MAA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

# .Ex01

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.Ex01" ".Ex01" "PREFIX.Ex01";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.Ex01" ".Ex01" "PREFIX.Ex01 PREFIX.Ex02 PREFIX.Ex03 PREFIX.Ex04 PREFIX.Ex05 PREFIX.Ex06 PREFIX.Ex07 PREFIX.Ex08 PREFIX.Ex09";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.Ex01" ".Ex01" "PREFIX.Ex01 PREFIX.Ex02 PREFIX.Ex03 PREFIX.Ex04 PREFIX.Ex05 PREFIX.Ex06 PREFIX.Ex07 PREFIX.Ex08 PREFIX.Ex09 PREFIX.Ex10 PREFIX.Ex11";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence4 "${TEST_EXECUTABLE}" "PREFIX.Ex01" ".Ex01" "PREFIX" "ExBA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence4 "${TEST_EXECUTABLE}" "PREFIX.Ex01" ".Ex01" "PREFIX" "EyAA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

# .Lx01

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.Lx01" ".Lx01" "PREFIX.Lx01";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.Lx01" ".Lx01" "PREFIX.Lx01 PREFIX.Lx02 PREFIX.Lx03 PREFIX.Lx04 PREFIX.Lx05 PREFIX.Lx06 PREFIX.Lx07 PREFIX.Lx08 PREFIX.Lx09";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence2 "${TEST_EXECUTABLE}" "PREFIX.Lx01" ".Lx01" "PREFIX.Lx01 PREFIX.Lx02 PREFIX.Lx03 PREFIX.Lx04 PREFIX.Lx05 PREFIX.Lx06 PREFIX.Lx07 PREFIX.Lx08 PREFIX.Lx09 PREFIX.Lx10 PREFIX.Lx11";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence4 "${TEST_EXECUTABLE}" "PREFIX.Lx01" ".Lx01" "PREFIX" "LxBA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

test_glob_sequence4 "${TEST_EXECUTABLE}" "PREFIX.Lx01" ".Lx01" "PREFIX" "LyAA";
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};


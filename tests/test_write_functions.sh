#!/bin/bash
# Library API write functions testing script
#
# Version: 20160328

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\)/\1/'`;

TEST_PROFILE="lib${TEST_PREFIX}";
TEST_FUNCTIONS="write write_chunk";
OPTION_SETS="";

TEST_TOOL_DIRECTORY=".";

test_write()
{ 
	local TEST_EXECUTABLE=$1;
	local MEDIA_SIZE=$2;
	local MAXIMUM_SEGMENT_SIZE=$3;
	local COMPRESSION_LEVEL=$4;

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	run_test_with_arguments ${TEST_EXECUTABLE} -B ${MEDIA_SIZE} -c `echo ${COMPRESSION_LEVEL} | cut -c 1` -S ${MAXIMUM_SEGMENT_SIZE} "${TMPDIR}/write";

	RESULT=$?;

	rm -rf ${TMPDIR};

	echo -n "Testing write chunk with media size: ${MEDIA_SIZE}, maximum segment size: ${MAXIMUM_SEGMENT_SIZE} and compression level: ${COMPRESSION_LEVEL} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

test_api_write_function()
{
	local TEST_PROFILE=$1;
	local TEST_FUNCTION=$2;
	local OPTION_SETS=$3;

	local TEST_TOOL="${TEST_PREFIX}_test_${TEST_FUNCTION}";

	local TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}";

	if ! test -x "${TEST_EXECUTABLE}";
	then
		TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}.exe";
	fi

	if ! test -x "${TEST_EXECUTABLE}";
	then
		echo "Missing test executable: ${TEST_EXECUTABLE}";

		return ${EXIT_FAILURE};
	fi
	echo "Testing API function: lib${TEST_PREFIX}_${TEST_FUNCTION}";

	for COMPRESSION_LEVEL in none empty-block fast best;
	do
		test_write "${TEST_EXECUTABLE}" 0 0 "${COMPRESSION_LEVEL}";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		test_write "${TEST_EXECUTABLE}" 0 10000 "${COMPRESSION_LEVEL}";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		test_write "${TEST_EXECUTABLE}" 100000 0 "${COMPRESSION_LEVEL}";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		test_write "${TEST_EXECUTABLE}" 100000 10000 "${COMPRESSION_LEVEL}";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			return ${RESULT};
		fi

		echo "";
	done

	return ${RESULT};
}

if ! test -z ${SKIP_LIBRARY_TESTS};
then
	exit ${EXIT_IGNORE};
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

for TEST_FUNCTION in ${TEST_FUNCTIONS};
do
	test_api_write_function "${TEST_PROFILE}" "${TEST_FUNCTION}" "${OPTION_SETS}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};


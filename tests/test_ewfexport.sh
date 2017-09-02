#!/bin/bash
# Export tool testing script
#
# Version: 20170902

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

OPTION_SETS="format:encase1 format:encase2 format:encase3 format:encase4 format:encase5 format:encase6 format:encase7 format:encase7-v2 format:ewf format:ewfx format:ftk format:linen5 format:linen6 format:linen7 format:raw format:smart deflate:none deflate:empty-block deflate:fast deflate:best blocksize:16 blocksize:32 blocksize:128 blocksize:256 blocksize:512 blocksize:1024 blocksize:2048 blocksize:4096 blocksize:8192 blocksize:16384 blocksize:32768 hash:sha1 hash:sha256 hash:all";
OPTIONS="-q -texport -u";

INPUT_DIRECTORY="input";
INPUT_GLOB="*.[Ees]*01";

test_callback()
{
	local TMPDIR=$1;
	local TEST_SET_DIRECTORY=$2;
	local TEST_OUTPUT=$3;
	local TEST_EXECUTABLE=$4;
	local TEST_INPUT=$5;
	shift 5;
	local ARGUMENTS=$@;

	TEST_EXECUTABLE=`readlink -f ${TEST_EXECUTABLE}`;
	INPUT_FILE_FULL_PATH=`readlink -f "${INPUT_FILE}"`;

	(cd ${TMPDIR} && run_test_with_input_and_arguments "${TEST_EXECUTABLE}" "${INPUT_FILE_FULL_PATH}" ${ARGUMENTS[@]});
	local RESULT=$?;

	if test -f "${TMPDIR}/export.raw";
	then
		local TEST_LOG="${TEST_OUTPUT}.log";

		if test "${PLATFORM}" = "Darwin";
		then
			(cd ${TMPDIR} && md5 export.* | sort -k 2 > "${TEST_LOG}");
		else
			(cd ${TMPDIR} && md5sum export.* | sort -k 2 > "${TEST_LOG}");
		fi

		local TEST_RESULTS="${TMPDIR}/${TEST_LOG}";
		local STORED_TEST_RESULTS="${TEST_SET_DIRECTORY}/${TEST_LOG}.gz";

		if test -f "${STORED_TEST_RESULTS}";
		then
			# Using zcat here since zdiff has issues on Mac OS X.
			# Note that zcat on Mac OS X requires the input from stdin.
			zcat < "${STORED_TEST_RESULTS}" | diff "${TEST_RESULTS}" -;
			RESULT=$?;
		else
			gzip ${TEST_RESULTS};

			mv "${TEST_RESULTS}.gz" ${TEST_SET_DIRECTORY};
		fi
	else
		run_test_with_input_and_arguments "${VERIFY_TOOL}" ${TMPDIR}/export.* -q > /dev/null;
		RESULT=$?;
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="../ewftools/ewfexport";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="../ewftools/ewfexport.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

	exit ${EXIT_FAILURE};
fi

VERIFY_TOOL="../ewftools/ewfverify";

if ! test -x "${VERIFY_TOOL}";
then
	VERIFY_TOOL="../ewftools/ewfverify.exe";
fi

if ! test -x "${VERIFY_TOOL}";
then
	echo "Missing executable: ${VERIFY_TOOL}";

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

PLATFORM=`uname -s`;

source ${TEST_RUNNER};

if test "${PLATFORM}" = "Darwin";
then
	assert_availability_binary md5;
else
	assert_availability_binary md5sum;
fi

run_test_on_input_directory "ewfexport" "ewfexport" "with_callback" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}" "${OPTIONS}";
RESULT=$?;

exit ${RESULT};


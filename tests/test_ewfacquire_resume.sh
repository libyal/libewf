#!/bin/bash
# Acquire tool testing script
#
# Version: 20160328

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\)/\1/'`;

test_write_resume()
{ 
	INPUT_FILE=$1;
	RESUME_OFFSET=$2;

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	run_test_with_input_and_arguments "${ACQUIRE_TOOL}" "${INPUT_FILE}" -b 64 -c deflate:none -C Case -D Description -E Evidence -e Examiner -f encase5 -m removable -M logical -N Notes -q -S 650MB -t ${TMPDIR}/acquire_resume -u > /dev/null;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${VERIFY_TOOL}" ${TMPDIR}/acquire_resume.E01 -q > /dev/null;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${TRUNCATE_TOOL}" ${TMPDIR}/acquire_resume.E01 ${RESUME_OFFSET} > /dev/null;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${ACQUIRE_TOOL}" "${INPUT_FILE}" -q -R -t ${TMPDIR}/acquire_resume.E01 -u > ${TMPDIR}/output;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		run_test_with_input_and_arguments "${VERIFY_TOOL}" ${TMPDIR}/acquire_resume.E01 -q > /dev/null;

		RESULT=$?;
	fi
	rm -rf ${TMPDIR};

	if test ${RESULT} -ne ${EXIT_IGNORE};
	then
		echo -n "Testing ewfacquire resume of at offset: ${RESUME_OFFSET}";

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			echo " (FAIL)";
		else
			echo " (PASS)";
		fi
	fi
	return ${RESULT};
}

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

ACQUIRE_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}acquire";

if ! test -x "${ACQUIRE_TOOL}";
then
	ACQUIRE_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}acquire.exe";
fi

if ! test -x "${ACQUIRE_TOOL}";
then
	echo "Missing executable: ${ACQUIRE_TOOL}";

	exit ${EXIT_FAILURE};
fi

TRUNCATE_TOOL="./ewf_test_truncate";

if ! test -x "${TRUNCATE_TOOL}";
then
	ACQUIRE_TOOL="ewf_test_truncate.exe";
fi

if ! test -x ${TRUNCATE_TOOL};
then
	echo "Missing executable: ${TRUNCATE_TOOL}";

	exit ${EXIT_FAILURE};
fi

VERIFY_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}verify";

if ! test -x "${VERIFY_TOOL}";
then
	VERIFY_TOOL="../${TEST_PREFIX}tools/${TEST_PREFIX}verify.exe";
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

source ${TEST_RUNNER};

FILENAME="input/raw/floppy.raw";

if ! test -f ${FILENAME};
then
	exit ${EXIT_IGNORE};
fi

FILESIZE=`ls -l ${FILENAME} | awk '{ print $5 }'`;

if test ${FILESIZE} -ne 1474560;
then
	exit ${EXIT_IGNORE};
fi

test_write_resume "${FILENAME}" 1478560
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

test_write_resume "${FILENAME}" 1477351
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

test_write_resume "${FILENAME}" 1478432
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

test_write_resume "${FILENAME}" 1477296
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

test_write_resume "${FILENAME}" 1477008
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

test_write_resume "${FILENAME}" 1476736
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

test_write_resume "${FILENAME}" 3584
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${RESULT};
fi

exit ${RESULT};


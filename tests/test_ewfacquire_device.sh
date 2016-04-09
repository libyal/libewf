#!/bin/bash
# Acquire tool testing script
#
# Version: 20160403

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\)/\1/'`;

TEST_PROFILE="${TEST_PREFIX}acquire_device";

TEST_SET_DIR="input/.${TEST_PROFILE}";

run_ewfacquire_device()
{ 
	local TEST_SET_DIR=$1;
	local TEST_DESCRIPTION=$2;
	local INPUT_DEVICE=$3;
	local INPUT_SIZE=$4;
	local PROCESSING_OPTIONS=();
	local ACQUIRE_OPTIONS=();

	IFS=" " read -a PROCESSING_OPTIONS <<< $5;
	IFS=" " read -a ACQUIRE_OPTIONS <<< $6;

	local INPUT_NAME=`basename ${INPUT_DEVICE}`;

	local TEST_OUTPUT="${INPUT_NAME}";

	local TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	/usr/bin/time -v ${ACQUIRE_TOOL} -B ${INPUT_SIZE} -C Case -D Description -E Evidence -e Examiner -m removable -M logical -N Notes -P 512 -q -t ${TMPDIR}/${INPUT_NAME}.acquire -u ${PROCESSING_OPTIONS[@]} ${ACQUIRE_OPTIONS[@]} ${INPUT_DEVICE};

	local RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
        then
                /usr/bin/time -v ${VERIFY_TOOL} ${PROCESSING_OPTIONS[@]} ${TMPDIR}/${INPUT_NAME}.acquire.*;

                RESULT=$?;
        fi
	rm -rf ${TMPDIR};

	echo -n "Testing ${TEST_DESCRIPTION} with input: ${INPUT_DEVICE}";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
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

# Set upper virtual memory limit to 1 GiB
ulimit -Sv 1048576;

local PROCESSING_OPTIONS="-j4 -x";

for FORMAT in encase7 encase7-v2;
do
	# > 2 GiB test
	INPUT_SIZE=$(( ( 2 * 1024 * 1024 * 1024 ) + 512 ));

	ACQUIRE_OPTIONS="-cdeflate:empty-block -f${FORMAT}";

	if ! run_ewfacquire_device "${TEST_SET_DIR}" "${TEST_PROFILE}" "/dev/zero" ${INPUT_SIZE} "${PROCESSING_OPTIONS}" "${ACQUIRE_OPTIONS}";
	then
		exit ${EXIT_FAILURE};
	fi

	ACQUIRE_OPTIONS="-cdeflate:none -f${FORMAT}";

	if ! run_ewfacquire_device "${TEST_SET_DIR}" "${TEST_PROFILE}" "/dev/urandom" ${INPUT_SIZE} "${PROCESSING_OPTIONS}" "${ACQUIRE_OPTIONS}";
	then
		exit ${EXIT_FAILURE};
	fi

	# Test +64 segment files.
	ACQUIRE_OPTIONS="-cdeflate:none -f${FORMAT} -S16MiB";

	if ! run_ewfacquire_device "${TEST_SET_DIR}" "${TEST_PROFILE}" "/dev/urandom" ${INPUT_SIZE} "${PROCESSING_OPTIONS}" "${ACQUIRE_OPTIONS}";
	then
		exit ${EXIT_FAILURE};
	fi

	# > 4 GiB test
	INPUT_SIZE=$(( ( 4 * 1024 * 1024 * 1024 ) + 512 ));

	ACQUIRE_OPTIONS="-cdeflate:empty-block -f${FORMAT}";

	if ! run_ewfacquire_device "${TEST_SET_DIR}" "${TEST_PROFILE}" "/dev/zero" ${INPUT_SIZE} "${PROCESSING_OPTIONS}" "${ACQUIRE_OPTIONS}";
	then
		exit ${EXIT_FAILURE};
	fi

	ACQUIRE_OPTIONS="-cdeflate:none -f${FORMAT}";

	if ! run_ewfacquire_device "${TEST_SET_DIR}" "${TEST_PROFILE}" "/dev/urandom" ${INPUT_SIZE} "${PROCESSING_OPTIONS}" "${ACQUIRE_OPTIONS}";
	then
		exit ${EXIT_FAILURE};
	fi

	# > 2 TiB test
	INPUT_SIZE=$(( ( 2 * 1024 * 1024 * 1024 * 1024 ) + 512 ));

	ACQUIRE_OPTIONS="-cdeflate:empty-block -f${FORMAT}";

	if ! run_ewfacquire_device "${TEST_SET_DIR}" "${TEST_PROFILE}" "/dev/zero" ${INPUT_SIZE} "${PROCESSING_OPTIONS}" "${ACQUIRE_OPTIONS}";
	then
		exit ${EXIT_FAILURE};
	fi

	# > 8 TiB test
	INPUT_SIZE=$(( ( 8 * 1024 * 1024 * 1024 * 1024 ) + 512 ));

	ACQUIRE_OPTIONS="-cdeflate:empty-block -f${FORMAT}";

	if ! run_ewfacquire_device "${TEST_SET_DIR}" "${TEST_PROFILE}" "/dev/zero" ${INPUT_SIZE} "${PROCESSING_OPTIONS}" "${ACQUIRE_OPTIONS}";
	then
		exit ${EXIT_FAILURE};
	fi
done

exit ${EXIT_SUCCESS};


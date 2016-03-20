#!/bin/bash
# Acquire tool testing script
#
# Version: 20160320

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`pwd`;
TEST_PREFIX=`dirname ${TEST_PREFIX}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib//'`;

test_write_resume()
{ 
	INPUT_FILE=$1;
	RESUME_OFFSET=$2;

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	${ACQUIRE_TOOL} -b 64 -c deflate:none -C Case -D Description -E Evidence -e Examiner -f encase5 -m removable -M logical -N Notes -q -S 650MB -t ${TMPDIR}/resume -u ${INPUT_FILE} > /dev/null;

	RESULT=$?;

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		${VERIFY_TOOL} -q ${TMPDIR}/resume.E01 > /dev/null;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		${TRUNCATE_TOOL} ${RESUME_OFFSET} ${TMPDIR}/resume.E01 > /dev/null;

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		${TEST_RUNNER} ${TMPDIR} ${ACQUIRE_TOOL} -q -R ${INPUT_FILE}  > ${TMPDIR}/output <<EOI
${TMPDIR}/resume.E01


yes
EOI

		RESULT=$?;
	fi

	if test ${RESULT} -eq ${EXIT_SUCCESS};
	then
		${VERIFY_TOOL} -q ${TMPDIR}/resume.E01 > /dev/null;

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

if ! test -x ${TEST_RUNNER};
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -x ${TEST_RUNNER};
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

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

if ! test_write_resume "${FILENAME}" 1478560
then
	exit ${EXIT_FAILURE};
fi

if ! test_write_resume "${FILENAME}" 1477351
then
	exit ${EXIT_FAILURE};
fi

if ! test_write_resume "${FILENAME}" 1478432
then
	exit ${EXIT_FAILURE};
fi

if ! test_write_resume "${FILENAME}" 1477296
then
	exit ${EXIT_FAILURE};
fi

if ! test_write_resume "${FILENAME}" 1477008
then
	exit ${EXIT_FAILURE};
fi

if ! test_write_resume "${FILENAME}" 1476736
then
	exit ${EXIT_FAILURE};
fi

if ! test_write_resume "${FILENAME}" 3584
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};


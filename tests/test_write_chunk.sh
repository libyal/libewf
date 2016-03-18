#!/bin/bash
# Library write chunk testing script
#
# Version: 20160125

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`pwd`;
TEST_PREFIX=`dirname ${TEST_PREFIX}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib//'`;

TEST_EXECUTABLE="${TEST_PREFIX}_test_write_chunk";

test_write()
{ 
	MEDIA_SIZE=$1;
	MAXIMUM_SEGMENT_SIZE=$2;
	COMPRESSION_LEVEL=$3;

	TMPDIR="tmp$$";

	rm -rf ${TMPDIR};
	mkdir ${TMPDIR};

	${TEST_RUNNER} ${TMPDIR} ${TEST_WRITE} -B ${MEDIA_SIZE} -c `echo ${COMPRESSION_LEVEL} | cut -c 1` -S ${MAXIMUM_SEGMENT_SIZE} "${TMPDIR}/write";

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

TEST_WRITE="./${TEST_EXECUTABLE}";

if ! test -x ${TEST_WRITE};
then
	TEST_WRITE="${TEST_EXECUTABLE}.exe";
fi

if ! test -x ${TEST_WRITE};
then
	echo "Missing executable: ${TEST_WRITE}";

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

for COMPRESSION_LEVEL in none empty-block fast best;
do
	if ! test_write 0 0 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write 0 10000 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write 100000 0 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_write 100000 10000 ${COMPRESSION_LEVEL}
	then
		exit ${EXIT_FAILURE};
	fi

	echo "";
done

exit ${EXIT_SUCCESS};


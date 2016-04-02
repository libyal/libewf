#!/bin/bash
# Script to create the ewfexport option sets
#
# Version: 20160402

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

INPUT_GLOB="*.[Ees]*01";

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in ${LIST};
	do
		if test ${LINE} = ${SEARCH};
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

TEST_PROFILE_DIR="input/.ewfexport";
IGNORE_FILE="${TEST_PROFILE_DIR}/ignore";
IGNORE_LIST="";

if test -f "${IGNORE_FILE}";
then
	IGNORE_LIST=`cat ${IGNORE_FILE} | sed '/^#/d'`;
fi

for INPUT_DIR in input/*;
do
	if ! test -d "${INPUT_DIR}";
	then
		continue
	fi
	INPUT_NAME=`basename ${INPUT_DIR}`;

	if list_contains "${IGNORE_LIST}" "${INPUT_NAME}";
	then
		continue
	fi
	TEST_SET_DIR="${TEST_PROFILE_DIR}/${INPUT_NAME}";

	if ! test -d "${TEST_SET_DIR}";
	then
		mkdir "${TEST_SET_DIR}";
	fi
	INPUT_FILES=`ls ${INPUT_DIR}/${INPUT_GLOB}`;

	for INPUT_FILE in ${INPUT_FILES};
	do
		INPUT_NAME=`basename ${INPUT_FILE}`;

		# Test the different supported formats. 
		for FORMAT in encase1 encase2 encase3 encase4 encase5 encase6 encase7 encase7-v2 ewf ewfx ftk linen5 linen6 linen7 raw smart;
		do
			OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.format:${FORMAT}";

			echo "Creating option set file: ${OUTPUT_FILE}";

			echo "-f${FORMAT} -j0" > "${OUTPUT_FILE}";
		done

		# Test the different supported compression methods and levels.
		for COMPRESSION_METHOD in deflate;
		do
			# TODO: add bzip2 support for encase7-v2
			if test COMPRESSION_METHOD = "bzip2";
			then
				continue;
			fi

			for COMPRESSION_LEVEL in none empty-block fast best;
			do
				OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.${COMPRESSION_METHOD}:${COMPRESSION_LEVEL}";

				echo "Creating option set file: ${OUTPUT_FILE}";

				echo "-c${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} -fencase7 -j0" > "${OUTPUT_FILE}";
			done
		done

		# Test the different supported chunk sizes.
		for CHUNK_SIZE in 16 32 128 256 512 1024 2048 4096 8192 16384 32768;
		do
			OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.blocksize:${CHUNK_SIZE}";

			echo "Creating option set file: ${OUTPUT_FILE}";

			echo "-b${CHUNK_SIZE} -fencase7 -j0" > "${OUTPUT_FILE}";
		done

		# Test the different supported formats. 
		for HASHING_METHOD in sha1 sha256;
		do
			OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.hash:${HASHING_METHOD}";

			echo "Creating option set file: ${OUTPUT_FILE}";

			echo "-d${HASHING_METHOD} -fencase7 -j0" > "${OUTPUT_FILE}";
		done

		OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.hash:all";

		echo "Creating option set file: ${OUTPUT_FILE}";

		echo "-dsha1,sha256 -fencase7 -j0" > "${OUTPUT_FILE}";
	done
done

exit ${EXIT_SUCCESS};


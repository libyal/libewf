#!/bin/bash
# Script to create the ewfexport option sets
#
# Version: 20160318

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

		for FORMAT in raw ewf encase1 encase2 encase3 encase4 encase5 encase6 encase7 linen5 linen6 linen7 ftk smart ewfx encase7-v2;
		do
			OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.${FORMAT}";

			echo "Creating option set file: ${OUTPUT_FILE}";

			echo "-f${FORMAT}" > ${OUTPUT_FILE};
		done

		# TODO: add bzip2 support
		for COMPRESSION_METHOD in deflate;
		do
			for COMPRESSION_LEVEL in none empty-block fast best;
			do
				OUTPUT_FILE="${TEST_SET_DIR}/${INPUT_NAME}.${COMPRESSION_METHOD}:${COMPRESSION_LEVEL}";

				echo "Creating option set file: ${OUTPUT_FILE}";

				echo "-c${COMPRESSION_METHOD}:${COMPRESSION_LEVEL} -f${FORMAT}" > ${OUTPUT_FILE};
			done
		done
	done
done

exit ${EXIT_SUCCESS};


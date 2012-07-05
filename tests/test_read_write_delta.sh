#!/bin/bash
#
# Expert Witness Compression Format (EWF) library read/write testing script
#
# Copyright (c) 2006-2012, Joachim Metz <joachim.metz@gmail.com>
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

AWK="awk";
CUT="cut";
LS="ls";
TR="tr";

test_read_write_delta()
{ 
	MEDIA_SIZE=$1;
	CHUNK_SIZE=$2;
	COMPRESSION_LEVEL=$3;
	WRITE_OFFSET=$4;
	WRITE_SIZE=$5;

	shift 2;

	mkdir ${TMP};

	./${EWF_TEST_WRITE} -b ${CHUNK_SIZE} -B ${MEDIA_SIZE} -c `echo ${COMPRESSION_LEVEL} | ${CUT} -c 1` ${TMP}/write;

	RESULT=$?;

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		FILENAMES=`${LS} ${TMP}/write.* | ${TR} '\n' ' '`;

		./${EWF_TEST_READ_WRITE_DELTA} -B ${WRITE_SIZE} -o ${WRITE_OFFSET} -t ${TMP}/read_write ${FILENAMES};

		RESULT=$?;
	fi

	if [ ${RESULT} -eq ${EXIT_SUCCESS} ];
	then
		if [ ${WRITE_OFFSET} -lt ${MEDIA_SIZE} ];
		then
			if [ -e ${TMP}/read_write.d01 ];
			then
				FILESIZE=`${LS} -l ${TMP}/read_write.d01 | ${AWK} '{ print $5 }'`;

				CHUNK_SIZE=`expr ${CHUNK_SIZE} \* 512`;

				if [ `expr ${WRITE_OFFSET} + ${WRITE_SIZE}` -gt ${MEDIA_SIZE} ];
				then
					WRITE_SIZE=`expr ${MEDIA_SIZE} - ${WRITE_OFFSET}`;
				fi

				NUMBER_OF_CHUNKS=`expr ${WRITE_SIZE} / ${CHUNK_SIZE}`;
				REMAINING_SIZE=`expr ${WRITE_SIZE} % ${CHUNK_SIZE}`;

				if [ ${REMAINING_SIZE} -gt 0 ];
				then
					# Check if the remaining size is part of the last chunk
					if [ `expr ${WRITE_OFFSET} + \( \( ${NUMBER_OF_CHUNKS} + 1 \) \* ${CHUNK_SIZE} \)` -le ${MEDIA_SIZE} ];
					then
						NUMBER_OF_CHUNKS=`expr ${NUMBER_OF_CHUNKS} + 1`;
						REMAINING_SIZE=0;
					fi
				fi

				CHUNK_OFFSET=`expr \( ${WRITE_OFFSET} / ${CHUNK_SIZE} \) \* ${CHUNK_SIZE}`;

				# Check if the written data passes a chunk boundary
				if [ ${WRITE_OFFSET} -gt ${CHUNK_OFFSET} ];
				then
					if [ `expr ${WRITE_OFFSET} + ${WRITE_SIZE}` -gt `expr ${CHUNK_OFFSET} + ${CHUNK_SIZE}` ];
					then
						NUMBER_OF_CHUNKS=`expr ${NUMBER_OF_CHUNKS} + 1`;
					fi
				fi

				if [ ${NUMBER_OF_CHUNKS} -gt 0 ];
				then
					# Add the section start, the delta chunk header and chunk checksum
					CALCULATED=`expr 76 + 18 + ${CHUNK_SIZE} + 4`;

					# Multiply by the amount of chunks altered
					CALCULATED=`expr ${CALCULATED} \* ${NUMBER_OF_CHUNKS}`;
				fi

				if [ ${REMAINING_SIZE} -gt 0 ];
				then
					# Add the section start, the delta chunk header and chunk checksum of the last chunk
					CALCULATED=`expr ${CALCULATED} + 76 + 18 + ${REMAINING_SIZE} + 4`;
				fi

				# Add the segment file header and the done or next section
				CALCULATED=`expr 13 + ${CALCULATED} + 76`;

				if [ ${FILESIZE} -eq ${CALCULATED} ];
				then
					RESULT=${EXIT_SUCCESS};
				else
					RESULT=${EXIT_FAILURE};
				fi
			else
				RESULT=${EXIT_FAILURE};
			fi
		else
			if [ -e ${TMP}/read_write.d01 ];
			then
				RESULT=${EXIT_FAILURE};
			else
				RESULT=${EXIT_SUCCESS};
			fi
		fi
	fi

	rm -rf ${TMP};

	echo -n "Testing read/write with offset: ${WRITE_OFFSET} and size: ${WRITE_SIZE} and compression level: ${COMPRESSION_LEVEL} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

EWF_TEST_WRITE="ewf_test_write";

if ! test -x ${EWF_TEST_WRITE};
then
	EWF_TEST_WRITE="ewf_test_write.exe";
fi

if ! test -x ${EWF_TEST_WRITE};
then
	echo "Missing executable: ${EWF_TEST_WRITE}";

	exit ${EXIT_FAILURE};
fi

EWF_TEST_READ_WRITE_DELTA="ewf_test_read_write_delta";

if ! test -x ${EWF_TEST_READ_WRITE_DELTA};
then
	EWF_TEST_READ_WRITE_DELTA="ewf_test_read_write_delta.exe";
fi

if ! test -x ${EWF_TEST_READ_WRITE_DELTA};
then
	echo "Missing executable: ${EWF_TEST_READ_WRITE_DELTA}";

	exit ${EXIT_FAILURE};
fi

for COMPRESSION_LEVEL in none empty-block fast best;
do
	if ! test_read_write_delta 100000 64 ${COMPRESSION_LEVEL} 0 100000
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_read_write_delta 100000 64 ${COMPRESSION_LEVEL} 512 512
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_read_write_delta 100000 64 ${COMPRESSION_LEVEL} 32512 512
	then
		exit ${EXIT_FAILURE};
	fi

	if ! test_read_write_delta 100000 64 ${COMPRESSION_LEVEL} 32512 40000
	then
		exit ${EXIT_FAILURE};
	fi
done

exit ${EXIT_SUCCESS};


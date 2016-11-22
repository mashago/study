#!/bin/bash

set -u

echo -e "---- `date` ----"

SSH_HOST="masha@mashacoder.nat123.net"
SSH_PORT=22200

echo "sleep 1" | ssh -p ${SSH_PORT} ${SSH_HOST}
RESULT=$?
echo "RESULT=${RESULT}"

if [ ${RESULT} -ne 0 ]; then
	echo "CONNECT FAIL"
	#/home/masha/Documents/nat123/shundown.sh
	#/home/masha/Documents/nat123/startup.sh
else
	echo "CONNECT OK"
fi

echo -e "----------------\n"


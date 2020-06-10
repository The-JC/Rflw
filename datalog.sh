#!/bin/bash

ARCH="${ARCH=arm}"
REMOTE="${REMOTE=localhost:3333}"
FREQUENCY="${FREQUENCY=64000000}"
FILE="${FILE=swo.log}"

echo "Connecting to gdb $REMOTE"

> $FILE

gdb-multiarch -n -q \
	-iex "set architecture $ARCH" \
	-iex "target remote $REMOTE" \
	-iex 'mon reset halt' \
	-iex "mon tpiu config internal $FILE uart false $FREQUENCY" \
	-iex 'mon itm port 0 on' \
	-iex 'mon resume' \
	-iex 'set confirm off' \
	-iex 'quit' \
	build/Rflw.elf

TIME=$((60*15))
echo "Wait for $TIME s"

for i in $(seq 0 $TIME); do
	PERCENT=$((100*i/TIME))
	for j in {0..100}; do
		if [[ $PERCENT -ge j ]]; then
			echo -ne '#'
		else
			echo -ne ' '
		fi
	done
	echo -ne " [$PERCENT%] (${i}s from ${TIME}s)\r"
	sleep 1s
done

python3 tools/datalog.py $FILE

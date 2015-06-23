#!/bin/bash

############### declarations ###############
executable=$1
parameters=$2
inputFile=$3
command="$executable $parameters $inputFile"

kleeLastDir=$4
fileNamePattern="test*.assert.err"
searchPattern="$kleeLastDir/$fileNamePattern"

############### run ###############
echo "Run: $command"

$command &
processID=$!

# kill -0 checks if signal can be sent to process
# otherwise klee process terminated
while kill -0 "$processID" 2>/dev/null; do
	if ls $searchPattern 1> /dev/null 2>&1; then
		# assertion error exists
		kill -SIGKILL "$processID"
		break
	fi

	sleep 1
done

wait


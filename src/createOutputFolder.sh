#!/bin/bash

folderPath=$1
folderName=$2
folderNumber=0

while [ true ]; do
	mkdir "$folderPath/$folderName-$folderNumber" > /dev/null 2> /dev/null
	output=$?

	if [ $output -eq 1 ]; then
		# folder already exists
		folderNumber=$(($folderNumber + 1))
	else
		break
	fi
done

exit $folderNumber


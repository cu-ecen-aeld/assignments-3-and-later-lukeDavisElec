#!/bin/bash

# Exit with a return value of 1 if the wrong number of args are supplied
if [ $# -ne 2 ]
then
	echo Incorrect number of args supplied
	exit 1
fi

# Check if the specified directory exists
if [ -d $1 ]
then
	numFiles=$(find $1 -type f | wc -l)
	numLines=$(grep -r $2 $1* | wc -l)
	echo The number of files are $numFiles and the number of matching lines are $numLines
	exit 0	
fi

# Exit with a return value of 1 if the previous directory check failed
echo Directory does not exist
exit 1

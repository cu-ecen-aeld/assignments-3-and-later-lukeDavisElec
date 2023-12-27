#!/bin/bash

# Exit with a return value of 1 if the wrong number of args are supplied
if [ $# -ne 2 ]
then
	echo Incorrect number of args supplied
	exit 1
fi

# Write string argument to specified file path or exit with value 1 if file can not be created
echo $2 > $1 || ( echo Could not create file $1; exit 1 )

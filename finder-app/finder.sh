#!/bin/sh

if [ $# -ne 2 ]
then
  echo "Invalid number of arguments"
  exit 1
fi

if [ -n "$1" ]
then
  filesdir=$1
fi

if [ -n "$2" ]
then
  searchstr=$2
fi

if ! [ -d $filesdir ]
then
  echo "File directory does not exist."
  exit 1
fi

numfiles=$(ls $filesdir | wc -l)
numlines=$(grep -R $searchstr $filesdir | wc -l)

echo "The number of files are $numfiles and the number of matching lines are $numlines"

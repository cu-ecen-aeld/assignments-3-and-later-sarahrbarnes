#! /bin/bash

if [ $# -ne 2 ]
then
  echo "Invalid number of arguments"
  exit 1
fi

if [ -n "$1" ]
then
  writefile=$1
fi

if [ -n "$2" ]
then
  writestr=$2
fi

if ! [ -e $writefile ]
then
  touch $writefile
fi

if ! [ -w "$writefile" ] ; then
    echo 'cannot write to %s\n' "$writefile"
    exit 1
fi

echo -e $writestr > $writefile

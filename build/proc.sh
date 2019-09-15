#!/bin/bash
num=`uname -r`
  echo "$num"
while [[ 1 ]]
do
	pid=$(pgrep  $1)
	cmd="cat /proc/"$pid"/status"
	$cmd
	$num
        sleep 1
done


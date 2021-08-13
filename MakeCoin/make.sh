#!/bin/bash
while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		 ./MakeCoin $runid
		echo "============================================================================"
	fi
done <../validID.txt

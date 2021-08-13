#!/bin/bash
echo "gamma time window: >30 ns"
while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		./delaydelay $runid 30 2000
		echo "============================================================================"
	fi
done <../validID.txt

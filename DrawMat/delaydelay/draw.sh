#!/bin/bash
while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		./delaydelay $runid
		echo "============================================================================"
	fi
done <../../validID.txt

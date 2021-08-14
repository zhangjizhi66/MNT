#!/bin/bash
while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		./promptdelaytime $runid
		echo "============================================================================"
	fi
done <../../validID.txt

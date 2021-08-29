#!/bin/bash
if [ ! -d "../COIN_ROOT" ]; then
  mkdir ../COIN_ROOT
fi

while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		 ./MakeCoin $runid
		echo "============================================================================"
	fi
done <../validID.txt

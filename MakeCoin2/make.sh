#!/bin/bash
if [ ! -d "../COIN_ROOT2" ]; then
  mkdir ../COIN_ROOT2
fi

make MakeCoin2

while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		 ./MakeCoin2 $runid
		echo "============================================================================"
	fi
done <../validID.txt

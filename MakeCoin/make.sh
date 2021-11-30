#!/bin/bash
if [ ! -d "../new_COIN_ROOT" ]; then
  echo "making directory ..."
  mkdir ../new_COIN_ROOT
fi

make MakeCoin

while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		 ./MakeCoin $runid
		echo "============================================================================"
	fi
done <../validID.txt

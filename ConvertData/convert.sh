#!/bin/bash
if [ ! -d "../CHICO_ROOT" ]; then
  mkdir ../CHICO_ROOT
fi

if [ ! -d "../GAMMAGT_ROOT" ]; then
  mkdir ../GAMMAGT_ROOT
fi

if [ ! -d "../GAMMATK_ROOT" ]; then
  mkdir ../GAMMATK_ROOT
fi

make ConvertData

while read -d ' ' runid
do
	if [ $runid -ge $1 ]
	then
		./ConvertData $runid
		echo "============================================================================"
	fi
done <../validID.txt

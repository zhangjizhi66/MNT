#!/bin/bash
i=116
j=156

while (( $i<=$j ))
do
    echo "making Gpd cube for mass $i ..."
    root -l -q gmat.cpp\($i\)
    let "i++"
done

i=178
j=218

while (( $i<=$j ))
do
    echo "making Gpd cube for mass $i ..."
    root -l -q gmat.cpp\($i\)
    let "i++"
done

echo "All gmat done!"

#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

./makeGddroot 80 89
./makeGddroot 90 99
./makeGddroot 100 109
./makeGddroot 110 129
./makeGddroot 130 139
./makeGddroot 140 159
./makeGddroot 160 169
./makeGddroot 170 179
./makeGddroot 180 189
./makeGddroot 190 199
./makeGddroot 200 209
./makeGddroot 210 219

echo "merging root files ......"
rm ROOT/Gdd.root
hadd ROOT/Gdd.root ROOT/Gdd_80_89.root ROOT/Gdd_90_99.root ROOT/Gdd_100_109.root ROOT/Gdd_110_129.root ROOT/Gdd_130_139.root ROOT/Gdd_140_159.root ROOT/Gdd_160_169.root ROOT/Gdd_170_179.root ROOT/Gdd_180_189.root ROOT/Gdd_190_199.root ROOT/Gdd_200_209.root ROOT/Gdd_210_219.root
echo "Done!"
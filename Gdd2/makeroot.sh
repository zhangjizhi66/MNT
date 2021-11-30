#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

make makeGddroot2

./makeGddroot2 80 89
./makeGddroot2 90 99
./makeGddroot2 100 109
./makeGddroot2 110 129
./makeGddroot2 130 139
./makeGddroot2 140 159
./makeGddroot2 160 169
./makeGddroot2 170 179
./makeGddroot2 180 189
./makeGddroot2 190 199
./makeGddroot2 200 209
./makeGddroot2 210 219

echo "merging root files ......"
rm ROOT/Gdd2.root
hadd ROOT/Gdd2.root ROOT/Gdd2_80_89.root ROOT/Gdd2_90_99.root ROOT/Gdd2_100_109.root ROOT/Gdd2_110_129.root ROOT/Gdd2_130_139.root ROOT/Gdd2_140_159.root ROOT/Gdd2_160_169.root ROOT/Gdd2_170_179.root ROOT/Gdd2_180_189.root ROOT/Gdd2_190_199.root ROOT/Gdd2_200_209.root ROOT/Gdd2_210_219.root
echo "Done!"
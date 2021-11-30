#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

make makechicoroot

./makechicoroot 80 89
./makechicoroot 90 99
./makechicoroot 100 109
./makechicoroot 110 129
./makechicoroot 130 139
./makechicoroot 140 159
./makechicoroot 160 169
./makechicoroot 170 179
./makechicoroot 180 189
./makechicoroot 190 199
./makechicoroot 200 209
./makechicoroot 210 219

echo "merging root files ......"
rm ROOT/chico.root
hadd ROOT/chico.root ROOT/chico_80_89.root ROOT/chico_90_99.root ROOT/chico_100_109.root ROOT/chico_110_129.root ROOT/chico_130_139.root ROOT/chico_140_159.root ROOT/chico_160_169.root ROOT/chico_170_179.root ROOT/chico_180_189.root ROOT/chico_190_199.root ROOT/chico_200_209.root ROOT/chico_210_219.root
echo "Done!"
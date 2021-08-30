#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

make makeGdroot

./makeGdroot 80 89
./makeGdroot 90 99
./makeGdroot 100 109
./makeGdroot 110 129
./makeGdroot 130 139
./makeGdroot 140 159
./makeGdroot 160 169
./makeGdroot 170 179
./makeGdroot 180 189
./makeGdroot 190 199
./makeGdroot 200 209
./makeGdroot 210 219

echo "merging root files ......"
rm ROOT/Gd.root
hadd ROOT/Gd.root ROOT/Gd_80_89.root ROOT/Gd_90_99.root ROOT/Gd_100_109.root ROOT/Gd_110_129.root ROOT/Gd_130_139.root ROOT/Gd_140_159.root ROOT/Gd_160_169.root ROOT/Gd_170_179.root ROOT/Gd_180_189.root ROOT/Gd_190_199.root ROOT/Gd_200_209.root ROOT/Gd_210_219.root
echo "Done!"
#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

make makeGppproot2

./makeGppproot2 80 89
./makeGppproot2 90 99
./makeGppproot2 100 109
./makeGppproot2 110 129
./makeGppproot2 130 139
./makeGppproot2 140 159
./makeGppproot2 160 169
./makeGppproot2 170 179
./makeGppproot2 180 189
./makeGppproot2 190 199
./makeGppproot2 200 209
./makeGppproot2 210 219

echo "merging root files ......"
rm ROOT/Gppp2.root
hadd ROOT/Gppp2.root ROOT/Gppp2_80_89.root ROOT/Gppp2_90_99.root ROOT/Gppp2_100_109.root ROOT/Gppp2_110_129.root ROOT/Gppp2_130_139.root ROOT/Gppp2_140_159.root ROOT/Gppp2_160_169.root ROOT/Gppp2_170_179.root ROOT/Gppp2_180_189.root ROOT/Gppp2_190_199.root ROOT/Gppp2_200_209.root ROOT/Gppp2_210_219.root
echo "Done!"
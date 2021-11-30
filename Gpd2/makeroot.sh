#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

make makeGpdroot2

./makeGpdroot2 80 89
./makeGpdroot2 90 99
./makeGpdroot2 100 109
./makeGpdroot2 110 129
./makeGpdroot2 130 139
./makeGpdroot2 140 159
./makeGpdroot2 160 169
./makeGpdroot2 170 179
./makeGpdroot2 180 189
./makeGpdroot2 190 199
./makeGpdroot2 200 209
./makeGpdroot2 210 219

echo "merging root files ......"
rm ROOT/Gpd2.root
hadd ROOT/Gpd2.root ROOT/Gpd2_80_89.root ROOT/Gpd2_90_99.root ROOT/Gpd2_100_109.root ROOT/Gpd2_110_129.root ROOT/Gpd2_130_139.root ROOT/Gpd2_140_159.root ROOT/Gpd2_160_169.root ROOT/Gpd2_170_179.root ROOT/Gpd2_180_189.root ROOT/Gpd2_190_199.root ROOT/Gpd2_200_209.root ROOT/Gpd2_210_219.root
echo "Done!"
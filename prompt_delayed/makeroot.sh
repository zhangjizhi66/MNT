#!/bin/bash
./makeGpdroot 80 89
./makeGpdroot 90 99
./makeGpdroot 100 109
./makeGpdroot 110 129
./makeGpdroot 130 139
./makeGpdroot 140 159
./makeGpdroot 160 169
./makeGpdroot 170 179
./makeGpdroot 180 189
./makeGpdroot 190 199
./makeGpdroot 200 209
./makeGpdroot 210 219

echo "merging root files ......"
rm ROOT/Gpd.root
hadd ROOT/Gpd.root ROOT/Gpd_80_89.root ROOT/Gpd_90_99.root ROOT/Gpd_100_109.root ROOT/Gpd_110_129.root ROOT/Gpd_130_139.root ROOT/Gpd_140_159.root ROOT/Gpd_160_169.root ROOT/Gpd_170_179.root ROOT/Gpd_180_189.root ROOT/Gpd_190_199.root ROOT/Gpd_200_209.root ROOT/Gpd_210_219.root
echo "Done!"
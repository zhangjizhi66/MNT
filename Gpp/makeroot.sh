#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

make makeGpproot

./makeGpproot 80 89
./makeGpproot 90 99
./makeGpproot 100 109
./makeGpproot 110 129
./makeGpproot 130 139
./makeGpproot 140 159
./makeGpproot 160 169
./makeGpproot 170 179
./makeGpproot 180 189
./makeGpproot 190 199
./makeGpproot 200 209
./makeGpproot 210 219

echo "merging root files ......"
rm ROOT/Gpp.root
hadd ROOT/Gpp.root ROOT/Gpp_80_89.root ROOT/Gpp_90_99.root ROOT/Gpp_100_109.root ROOT/Gpp_110_129.root ROOT/Gpp_130_139.root ROOT/Gpp_140_159.root ROOT/Gpp_160_169.root ROOT/Gpp_170_179.root ROOT/Gpp_180_189.root ROOT/Gpp_190_199.root ROOT/Gpp_200_209.root ROOT/Gpp_210_219.root
echo "Done!"
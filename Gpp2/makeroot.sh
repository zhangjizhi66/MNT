#!/bin/bash
if [ ! -d "ROOT" ]; then
  mkdir ROOT
fi

make makeGpproot2

./makeGpproot2 80 89
./makeGpproot2 90 99
./makeGpproot2 100 109
./makeGpproot2 110 129
./makeGpproot2 130 139
./makeGpproot2 140 159
./makeGpproot2 160 169
./makeGpproot2 170 179
./makeGpproot2 180 189
./makeGpproot2 190 199
./makeGpproot2 200 209
./makeGpproot2 210 219

echo "merging root files ......"
rm ROOT/Gpp2.root
hadd ROOT/Gpp2.root ROOT/Gpp2_80_89.root ROOT/Gpp2_90_99.root ROOT/Gpp2_100_109.root ROOT/Gpp2_110_129.root ROOT/Gpp2_130_139.root ROOT/Gpp2_140_159.root ROOT/Gpp2_160_169.root ROOT/Gpp2_170_179.root ROOT/Gpp2_180_189.root ROOT/Gpp2_190_199.root ROOT/Gpp2_200_209.root ROOT/Gpp2_210_219.root
echo "Done!"
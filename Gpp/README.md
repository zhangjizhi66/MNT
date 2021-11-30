# 搭建 prompt-prompt 矩阵

## 必需文件

../COIN_ROOT/coin%04d.root

../validID.txt  （记录 runid 编号）

## makeroot.sh

1、在当前目录创建 ROOT 文件夹

2、用 make 方法创建 makeGpproot 可执行文件

3、以 10-20 个 run 为一组，生成 ROOT/Gpp_%d_%d.root 文件

4、合并以上 ROOT 文件为 ROOT/Gpp.root

### 运行

./makeroot.sh

## makeGpproot：生成 ROOT 文件

将 prompt gamma 的能量、时间按 x轴、y轴存为 ROOT 文件。ROOT 文件路径为 ./ROOT/Gpp_%d_%d.root

G：使用 gtk 类型的 gamma

pp：prompt-prompt

### 运行

./makeGpproot (runid) (runid)

两个参数分别为 runid 的上下界，可以不是实际的 runid

### 分支

为节省空间，加快运行速度，所有分支均为 Short_t 类型

np/S：在 COIN_ROOT/coin%04d.root 文件的一个事件中产生了多少 prompt gamma 符合事件（例如一对 gamma 产生 2 个事件） 

ePx/S、ePy/S：能量（keV），根据类弹核进行多普勒修正

eTx/S、eTy/S：能量（keV），根据类靶核进行多普勒修正

pseuQ/S、thetaT/S：赝Q值(keV)、类靶核散射角(deg)

### cut 条件

prompt 条件：gamma 与 Chico 的时间差 <= 30 ns

符合条件：两 delayed gamma 时间差 <= 20 ns 

gtk 条件：品质因数 Gfom <= 0.8

能量范围：Ge <= 2000

多重性：分支 nd < 6

## gamma 符合分析代码

根据 [https://github.com/zhihuanli/gamma-gamma-coincidence-analysis/tree/master/prompt%20gg] 修改而成

### gmatrix.cpp

从 ROOT/Gdd.root 生成 gamma 符合矩阵，并用 radware 方法减扣本底

在 ROOT 环境下运行 .x gmatrix.cpp（jupyter 中运行 gROOT->ProcessLine(".x gmatrix.cpp") ）

### gg.C

载入 gamma 符合分析函数

在 ROOT 环境下运行 .x gg.C（jupyter 中运行 gROOT->ProcessLine(".x gg.C") ）


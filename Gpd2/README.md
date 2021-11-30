# 搭建 prompt-delayed 矩阵（改进版）

## 改进点

用散射角-质量二维谱进行类弹类靶鉴别（而非飞行时间差-散射角二维谱，可参考 ../chico/ana_chico.ipynb ）

用116-156、178-218范围内的各种质量分别进行多普勒修正，要看哪个核的gamma就用相应质量数修正的矩阵(而非逐事件计算的质量，分辨太低)

作并道处理，将y轴(prompt)增加为 2keV/bin

## 必需文件

../COIN_ROOT2/coin%04d.root

../validID.txt  （记录 runid 编号）

## 1. makeGpdroot2

将 prompt 和 delayed gamma 的能量、时间以及与多普勒修正相关的碎片信息存为 ROOT 文件（注意：本程序不作多普勒修正）

G：使用 gtk 类型的 gamma

pd：prompt-delayed

生成的 ROOT 文件路径为 ./ROOT/Gpd2_%d_%d.root

### 分支

为节省空间，加快运行速度，所有分支均为 Short_t 类型

nd/S、np/S：delayed 和 prompt gamma 数目

ed/S、ep/S：delayed、prompt 能量(keV)

td/S、tp/S：相对 Chico 事件的时间(ns)

cosP/S、cosT/S：prompt gamma 与类弹、类靶的夹角(rad/1e4)

pP/S、pT/S：类弹类靶动量，SI单位制下写作p=mvc/MeV=mv*3e8/1.602e-13

### cut 条件

delayed 条件：gamma 与 Chico 的时间差 > 30 ns

prompt 条件：gamma 与 Chico 的时间差 <= 20 ns

gtk 条件：品质因数 Gfom <= 0.8

能量范围：Ge <= 2000

多重性暂不加限制

### 独立运行

./makeGpdroot2 (runid) (runid)

两个参数分别为 runid 的上下界，可以不是实际的 runid

### 批量运行：makeroot.sh

./makeroot.sh

1、在当前目录创建 ROOT 文件夹

2、用 make 方法创建 makeGpdroot2 可执行文件

3、以 10-20 个 run 为一组，生成 ROOT/Gpd2_%d_%d.root 文件

4、合并以上 ROOT 文件为 ROOT/Gpd2.root

## 2. gmat.cpp

对 prompt 作多普勒修正（修正公式中的质量数需要用户传参），并生成矩阵

x轴为 delayed，y轴为 prompt

生成 ROOT 文件 ROOT/Gpdmat2_%d.root （%d为质量数）

### 独立运行

root gmat.cpp\(%d\)  （%d为质量数）

### 批量运行

./gmat.sh

传参数为 116-156, 178-218

## 3. gamma 矩阵开窗：ggm.C

由于多普勒修正错误的 gamma 会在开窗谱部分区域形成不太大的鼓包，因此 RadWare 减本底方法不再适用（减完会出现负值区域）

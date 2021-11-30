# 搭建 prompt-delayed 矩阵

## 必需文件

../COIN_ROOT/coin%04d.root

../validID.txt  （记录 runid 编号）

## makeroot.sh

1、在当前目录创建 ROOT 文件夹

2、用 make 方法创建 makeGpdroot 可执行文件

3、以 10-20 个 run 为一组，生成 ROOT/Gpd_%d_%d.root 文件

4、合并以上 ROOT 文件为 ROOT/Gpd.root

### 运行

./makeroot.sh

## makeGpdroot：生成 ROOT 文件

将 prompt 和 delayed gamma 的能量、时间存为 ROOT 文件。ROOT 文件路径为 ./ROOT/Gpd_%d_%d.root

G：使用 gtk 类型的 gamma

pd：prompt-delayed

### 运行

./makeGpdroot (runid) (runid)

两个参数分别为 runid 的上下界，可以不是实际的 runid

### 分支

为节省空间，加快运行速度，所有分支均为 Short_t 类型

nd/S、np/S：delayed 和 prompt gamma 数目

ed/S、epP/S、epT/S：delayed、prompt corrected by Projectile、prompt corrected by Target 能量（keV）

td/S、tp/S：时间（ns，相对 Chico 事件）

pseuQ/S、thetaT/S：赝Q值(keV)、类靶核散射角(deg)

### cut 条件

delayed 条件：gamma 与 Chico 的时间差 > 30 ns

prompt 条件：gamma 与 Chico 的时间差 <= 20 ns

gtk 条件：品质因数 Gfom <= 0.8

能量范围：Ge <= 2000

多重性：分支 nd <= 2，np <= 2

## gamma 符合分析代码

根据 [https://github.com/zhihuanli/gamma-gamma-coincidence-analysis/tree/master/delayed%20gg] 修改而成

### makeaggmat.C

从 ROOT/Gpd.root 生成 gamma 符合矩阵，用 radware **非对称**本底矩阵减扣本底

在 ROOT 环境下运行 .x makeaggmat.C（jupyter 中运行 gROOT->ProcessLine(".x makeaggmat.C") ）

### agg.C

载入 gamma 符合分析函数

在 ROOT 环境下运行 .x agg.C（jupyter 中运行 gROOT->ProcessLine(".x agg.C") ）


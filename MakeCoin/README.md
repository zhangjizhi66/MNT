# 组织符合事件

## 必需文件

../CHICO_ROOT/Chico%04d.root

../GAMMAGT_ROOT/GT%04d.root

../GAMMATK_ROOT/TK%04d.root

./ChicoCut

../validID.txt


## MakeCoin：组织符合事件

根据 timestamp 将三类事件组织为符合事件

### 事件的 timestamp 分布特点

参考 ../ConvertData/ana.ipynb

1、相邻 Chico 事件的最小时间间隔为 40 us

2、前一 Chico 事件发生后 2 us 到后一 Chico 事件发生前 2 us 之间，没有任何 gamma 事件

因此本程序将 1 个 Chico 事件和其前后 2 us 内的 gamma 事件打包为一个符合事件

### 分支

#### Chico

pid/I：Chico 事件的 id，含义未知

ts/l：timestamp(ns)

dT/I：两核子的飞行时间差(ns)

dofP/F dofT/F：核子飞行距离

thetaP/F thetaT/F：核子飞行极角（散射角）(rad)

phiP/F phiT/F：核子飞行方位角(rad)

massT/F massP/F：核子质量(amu)；在本程序中利用 theta phi 通过两体动力学算出

betaT/F betaP/F：核子速度(c)，即相对论因子；在本程序中利用 theta phi 通过两体动力学算出

pP/F pT/F：核子动量

pseuQ/F realQ/F：赝Q值和真实Q值(keV)；由于核子质量的分辨不好，用原来的弹核和靶核的质量代替，算出的Q值为赝Q值

注：本程序中利用 theta - dT 二维图和 ./ChicoCut 中的 massCut 对类弹和类靶核作了鉴别，相应物理量分支分别以 P 和 T 标记

#### GAMMAGT

ng/I：符合事件中的 GAMMAGT 事件数目

gid\[ng]/I：探测器编号

gdt\[ng]/I：与 Chico 事件的时间差(ns)

ge\[ng]/F geP\[ng]/F geT\[ng]/F：原始能量、根据类弹(P)和类靶(T)核作多普勒修正后的能量(keV)；多普勒修正于本程序中完成

gtheta\[ng]/F：gamma 飞行极角(rad)

gphi\[ng]/F：gamma 飞行方位角(rad)

gcosP\[ng]/F gcosT\[ng]/F：gamma 与类弹(P)和类靶(T)核的飞行轨道夹角余弦，在本程序中算出

gx\[ng]/F gy\[ng]/F gz\[ng]/F：点火的探测器空间坐标

#### GAMMATK

nG/I：符合事件中的 GAMMATK 事件数目

Gid\[nG]/I：探测器编号

Gdt\[nG]/I：与 Chico 事件的时间差(ns)

Ge\[nG]/F GeP\[nG]/F GeT\[nG]/F：原始能量、根据类弹(P)和类靶(T)核作多普勒修正后的能量(keV)；多普勒修正于本程序中完成

Gtheta\[nG]/F：gamma 飞行极角(rad)

Gphi\[nG]/F：gamma 飞行方位角(rad)

GcosP\[nG]/F GcosT\[nG]/F：gamma 与类弹(P)和类靶(T)核的飞行轨道夹角余弦，在本程序中算出

Gndet\[nG]/I：在 tracking 过程中被加合的 gamma 数目

Gfom\[nG]/F：品质因数，表征 tracking 结果的置信程度；参考的信任范围为 Gfom < 0.8

Gx0\[nG]/F Gy0\[nG]/F Gz0\[nG]/F Ge0\[nG]/F：第一作用点的探测器空间坐标、损失的能量(keV)

Gx1\[nG]/F Gy1\[nG]/F Gz1\[nG]/F Ge1\[nG]/F：第二作用点的探测器空间坐标、损失的能量(keV)

### Cut 条件

0 < massP < 334

0 < massT < 334

0 < betaP < 1

0 < betaT < 1

### 运行

./MakeCoin (runid)

参数为需要运行的 runid

## make.sh：批量运行 MakeCoin

### 运行

./make.sh (runid)

脚本将批量运行大于等于给定参数的 runid，参数可以不是实际的 runid

## ana.ipynb

作图对比 /data/d3/DataZhu/ana 程序与本程序生成的 ROOT 文件


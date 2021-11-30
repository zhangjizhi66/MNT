# 组织符合事件（改进版）

## 改进点

用散射角-质量二维谱进行类弹类靶鉴别（而非飞行时间差-散射角二维谱，可参考 id_PLF_TLF.ipynb ）

## 必需文件

../CHICO_ROOT/Chico%04d.root

../GAMMAGT_ROOT/GT%04d.root

../GAMMATK_ROOT/TK%04d.root

./ChicoCut

../validID.txt

## 1. mass_theta_mat.cpp

读入 ../CHICO_ROOT/Chico%04d.root

在不作弹靶鉴别的情况下计算碎片各自的质量，并画散射角-质量二维谱（遍历所有run）

注意：由于未作弹靶鉴别，因此没有修正碎片在靶内的能量损失（因为修正公式因弹靶而异），得到的质量分布中心约为 130 和 204

生成 TH2D 写入 mass_theta_mat.root

### 运行

root mass_theta_mat.cpp

## 2. MakeCoin2

根据 timestamp 将三类事件组织为符合事件，并读入上面生成的 mass_theta_mat.root 作弹靶鉴别（弹靶鉴别参考 id_PLF_TLF.ipynb）

生成 ROOT 文件路径：../COIN_ROOT2/coin%04d.root

### 符合事件组织方法

参考 ../ConvertData/time_distribution.ipynb

各类事件时间分布特点：

1、相邻 Chico 事件的最小时间间隔为 40 us

2、前一 Chico 事件发生后 2 us 到后一 Chico 事件发生前 2 us 之间，没有任何 gamma 事件

因此本程序将 1 个 Chico 事件和其前后 2 us 内的 gamma 事件打包为一个符合事件

### 生成的 ROOT 文件的分支

#### Chico

pid/I：Chico 事件的 id，含义未知

ts/l：timestamp(ns)

dT/I：两核子的飞行时间差(ns)

dofP/F dofT/F：核子飞行距离

thetaP/F thetaT/F：核子飞行极角（散射角）(rad)

phiP/F phiT/F：核子飞行方位角(rad)

massT/F massP/F：核子质量(amu)；在本程序中利用 theta phi 通过两体动力学算出

betaT/F betaP/F：核子速度(c)，即相对论因子；在本程序中利用 theta phi 通过两体动力学算出

pP/F pT/F：核子动量，SI单位制下写作p=mvc/MeV=mv*3e8/1.602e-13

pseuQ/F realQ/F：赝Q值和真实Q值(keV)；由于核子质量的分辨不好，用原来的弹核和靶核的质量代替，算出的Q值为赝Q值

注：本程序的类弹和类靶物理量分支分别以 P 和 T 标记

#### GAMMAGT

ng/I：符合事件中的 GAMMAGT 事件数目

gid\[ng]/I：探测器编号

gdt\[ng]/I：与 Chico 事件的时间差(ns)

ge\[ng]/F：能量(keV)；注意：不作多普勒修正

gtheta\[ng]/F：gamma 飞行极角(rad)

gphi\[ng]/F：gamma 飞行方位角(rad)

gcosP\[ng]/F gcosT\[ng]/F：gamma 与类弹(P)和类靶(T)核的飞行轨道夹角余弦，在本程序中算出

gx\[ng]/F gy\[ng]/F gz\[ng]/F：点火的探测器空间坐标

#### GAMMATK

nG/I：符合事件中的 GAMMATK 事件数目

Gid\[nG]/I：探测器编号

Gdt\[nG]/I：与 Chico 事件的时间差(ns)

Ge\[nG]/F：能量(keV)；注意：不作多普勒修正

Gtheta\[nG]/F：gamma 飞行极角(rad)

Gphi\[nG]/F：gamma 飞行方位角(rad)

GcosP\[nG]/F GcosT\[nG]/F：gamma 与类弹(P)和类靶(T)核的飞行轨道夹角余弦，在本程序中算出

Gndet\[nG]/I：在 tracking 过程中被加合的 gamma 数目

Gfom\[nG]/F：品质因数，表征 tracking 结果的置信程度；参考的信任范围为 Gfom < 0.8

Gx0\[nG]/F Gy0\[nG]/F Gz0\[nG]/F Ge0\[nG]/F：第一作用点的探测器空间坐标、损失的能量(keV)

Gx1\[nG]/F Gy1\[nG]/F Gz1\[nG]/F Ge1\[nG]/F：第二作用点的探测器空间坐标、损失的能量(keV)

### 独立运行

./MakeCoin2 (runid)

参数为需要运行的 runid

### 批量运行：make.sh

./make.sh (runid)

脚本将批量运行大于等于给定参数的 runid，参数可以不是实际的 runid

1、创建 ../COIN_ROOT2 文件夹

2、用 make 方法创建 MakeCoin2 可执行文件

3、从 ../validID.txt 中读取 runid，批量运行 MakeCoin2 程序

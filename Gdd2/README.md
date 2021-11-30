# 搭建 delayed-delayed 矩阵（改进版）

## 改进点

使用 MakeCoin2 生成的 ROOT 文件作为输入，因此 Gdd2 相比 Gdd 实际上是用散射角-质量二维谱 cut 出了多核子转移的事件（例如熔合裂变事件被扔掉）

## 必需文件

../COIN_ROOT/coin%04d.root

../validID.txt  （记录 runid 编号）

## makeGddroot：生成 ROOT 文件

将 delayed gamma 的能量、时间按 x轴、y轴存为 ROOT 文件。生成 ROOT 文件路径为 ./ROOT/Gdd2_%d_%d.root

G：使用 gtk 类型的 gamma

dd：delayed-delayed

### 分支

为节省空间，加快运行速度，所有分支均为 Short_t 类型

nd/S：在 COIN_ROOT2/coin%04d.root 文件的一个事件中产生了多少 delayed gamma 符合事件（例如一对 gamma 产生 2 个事件） 

ex/S、ey/S：能量（keV）

tx/S、ty/S：时间（ns，相对 Chico 事件）

### cut 条件

delayed 条件：gamma 与 Chico 的时间差 > 30 ns

符合条件：两 delayed gamma 时间差 <= 20 ns 

gtk 条件：品质因数 Gfom <= 0.8

能量范围：Ge <= 2000

多重性暂不加限制

### 独立运行

./makeGddroot2 (runid) (runid)

两个参数分别为 runid 的上下界，可以不是实际的 runid

### 批量运行：makeroot.sh

./makeroot.sh

1、在当前目录创建 ROOT 文件夹

2、用 make 方法创建 makeGddroot2 可执行文件

3、以 10-20 个 run 为一组，生成 ROOT/Gdd2_%d_%d.root 文件

4、合并以上 ROOT 文件为 ROOT/Gdd2.root

## gamma 矩阵开窗：ggm.C


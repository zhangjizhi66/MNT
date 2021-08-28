# 将 data 文件转换为 ROOT 文件


## 必需文件
/data/d3/DataZhu/TRACKED_DATA/data%04d.dat
../validID.txt
./CAL_DATA

## ConvertData：将原始数据转化为 ROOT 文件
将原始数据转化为 ROOT 文件，三类事件（Chico、GAMMAGT、GAMMATK）分别存储

### 运行
./ConvertData (runid)
参数为需要转化为 ROOT 文件的 runid

### Chico 事件
输出：../CHICO_ROOT/Chico%04d.root

#### 分支
id/I：含义未知
ts/D：timestamp，原始数据中单位为 10 ns，本程序中转化为 ns
dT/I：两核子飞行时间差(ns)
dL/F、dR/F：核子飞行距离
thetaL/F、thetaR/F：核子飞行极角（散射角）(rad)
phiL/F、phiR/F：核子飞行方位角(rad)
注：Chico 的任一半均包含全同的呈环形排列的 10 片 ppac，以某一对称轴分割，左 5 片标记为 L，右 5 片标记为 R

### GAMMAGT 事件
输出：../GAMMAGT_ROOT/GT%04d.root

#### 分支
cid/I：每个 GRETINA 探头包含 4 片 HPGe，原始文件中分别记录探头编号和 HPGe 编号，本程序将编号统一为 (探头编号\*4 + HPGe 编号)
ts/D：timestamp (ns)，本程序中已用 ./CAL_DATA/gcidOff.txt 对 timestamp 进行刻度（加上一个 offset）
e/F：能量(keV)，本程序中已用 ./CAL_DATA/CC.cal 对能量进行刻度
x/F、y/F、z/F：探测器空间坐标
theta/F：gamma 飞行极角(rad)
phi/F：gamma 飞行方位角(rad)

### GAMMATK 事件
输出：../GAMMATK_ROOT/TK%04d.root

#### 分支
fid/I：gamma 首次点火的探测器编号，编号含义与 cid 相同
ts/l：timestamp (ns)，本程序中已用 ./CAL_DATA/gfidOff.txt 对 timestamp 进行刻度（加上一个 offset）
e/F：能量(keV)，为 tracking 后加合的总能量；在原始文件中已经刻度
theta/F：gamma 进入 GRETINA 之前的飞行极角(rad)，在本程序中通过第一作用点坐标算出
phi/F：gamma 进入 GRETINA 之前的飞行方位角(rad)，在本程序中通过第一作用点坐标算出
ndet/I：tracking 过程追踪到的 gamma 数目
fom/F：品质因数，表征 tracking 结果的可信度；参考的信任标准为 fom < 0.8
x0/F y0/F z0/F e0/F：第一作用点的坐标以及损失的能量(keV)
x1/F y1/F z1/F e1/F：第二作用点的坐标以及损失的能量(keV)

## convert.sh：批量运行 ConvertData 程序
从 ../validID.txt 中读取 runid，批量运行 ConvertData 程序

### 运行
./convert.sh (runid)
脚本将批量运行大于等于给定参数的 runid，参数可以不是实际的 runid

## ana.ipynb：分析三类事件的时间分布情况

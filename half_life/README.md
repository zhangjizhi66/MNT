# 计算 136Ba 10+ isomer 的寿命

## 必需文件

../COIN_ROOT/coin%04d.root

../validID.txt

## makeroot.sh

1、在当前目录创建 ROOT 文件夹

2、用 make 方法创建 makeGdroot 可执行文件

3、以 10-20 个 run 为一组，生成 ROOT/Gd_%d_%d.root 文件

4、合并以上 ROOT 文件为 ROOT/Gd.root

### 运行

./makeroot.sh

## makeGdroot

将 delayed gamma 信息另存为 ROOT 文件

### 分支

为节省空间、提高运行和调试速度，所有分支均为 Short_t

nd/S：来自同一符合事件的 delayed gamma 数目 

e/S：能量(keV)

t/S：与 Chico 事件的时间差(ns)

### cut 条件

delayed 条件：t > 30 ns

多重性限制：nd <= 4

### 运行

./makeGdroot (runid) (runid)

两个参数分别为需要运行的 runid 上下界，参数可以不是实际的 runid

## ana.ipynb

136Ba 10+ isomer 寿命计算
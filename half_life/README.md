# 计算 136Ba 10+ isomer 的寿命

## 必需文件

../COIN_ROOT/coin%04d.root

../validID.txt

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

本程序没有编写脚本，可直接运行 ./makeGdroot 82 217 ，生成文件 ROOT/Gd.root 用于 ana.ipynb 的分析

## ana.ipynb：半衰期计算
# 多核子转移 (Multi-Nucleon Transfer)

## 实验信息

束流：136Xe @ 1088 MeV

靶：198Pt

探测阵列：Chico2 + GRETINA @ Argonne National Laboratory


## 数据处理

### 0、起点

/data/d3/DataZhu/TRACKED_DATA/data%04d.dat

### 1、转换为 ROOT 文件

./ConvertData

### 2、组织符合事件

./MakeCoin

### 3、数据分析

延迟 gamma 符合矩阵：           ./delayed_delayed

瞬发-延迟 gamma 矩阵：           ./prompt_delayed

136Ba 的 10+ isomer 半衰期计算：    ./half_life

瞬发 gamma 符合矩阵：           ./prompt_prompt
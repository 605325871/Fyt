import pandas as pd
import matplotlib.pyplot as plt

# 读取数据到pandas DataFrame
df = pd.read_csv('log.txt', sep=' ', header=None, names=['id', 'TotalAlloc', 'TotalFree', 'TotalFreed', 'HeapIdle'])

# 重置索引为整数类型
df = df.reset_index()

# 取出指标列
total_alloc = df['TotalAlloc'].values
total_free = df['TotalFree'].values
total_freed = df['TotalFreed'].values
heap_idle = df['HeapIdle'].values

# 取出时间列
time = (df.index + 1) * 100

# 创建一个新的图形
fig, ax = plt.subplots()

# 绘制每个指标的曲线
ax.plot(time, total_alloc, label='TotalAlloc')
ax.plot(time, total_free, label='TotalFree')
ax.plot(time, total_freed, label='TotalFreed')
ax.plot(time, heap_idle, label='HeapIdle')

# 添加标题和标签
ax.set_title('Memory Allocation')
ax.set_xlabel('Time')
ax.set_ylabel('Bytes')

# 添加图例
ax.legend()

# 显示图形
plt.show()
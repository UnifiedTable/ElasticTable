import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
plt.rc('font', family='Arial')
times = ["Area1","Area2","Area3","Area4","Area5"]
fib = [1501816,555953,677369,373087,788589]
x = [i for i in range(len(times))]

# 柱状图的宽度
bar_width = 0.35

# 调整lambdas以便于柱状图的绘制，避免重叠

plt.figure(figsize=(6,4.5))
plt.yscale('linear')
# plt.xticks(np.arange(0.9, 1.21, 0.05))
plt.xticks(x, times)
plt.tick_params(labelsize=19)
plt.ylim(0,1600000)
ax1 = plt.gca()
ax1.yaxis.set_major_formatter(ScalarFormatter())  # 使用科学计数法格式化纵坐标轴
ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
ax1.yaxis.get_offset_text().set_fontsize(19)

# 绘制柱状图
plt.bar(x, fib, width=bar_width, color='#2e75b6', alpha=1, edgecolor='black', linewidth=2)

plt.xlabel('Dataset', fontweight='bold', fontsize=20)
plt.ylabel('Size', fontweight='bold', fontsize=20)

# 设置图例
# plt.legend(loc='upper left', ncol=1, handlelength=3)
# leg = plt.gca().get_legend()
# ltext = leg.get_texts()
# plt.setp(ltext, fontweight='bold', fontsize=20)

# 设置网格
ax1.grid(True, linestyle='--', axis='x')
ax1.grid(True, linestyle='--', axis='y')

plt.tight_layout()

# 保存图像
plt.savefig('./ExpFigures/FIB_size_area.pdf')
plt.show()

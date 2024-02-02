import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

plt.rc('font', family='Arial')
times = [2019,2020,2021,2022,2023,2024]
fib = [5518787,8798918,12424696,15181339,13833024,17580260]
x = [i for i in range(len(times))]

# 柱状图的宽度
bar_width = 0.35

# 调整lambdas以便于柱状图的绘制，避免重叠

plt.figure(figsize=(6,4.5))
# plt.xticks(np.arange(0.9, 1.21, 0.05))
plt.xticks(x, times)
plt.tick_params(labelsize=19)
ax1 = plt.gca()
ax1.yaxis.set_major_formatter(ScalarFormatter())  # 使用科学计数法格式化纵坐标轴
ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
ax1.yaxis.get_offset_text().set_fontsize(19)

# 绘制柱状图
plt.bar(x, fib, width=bar_width, color='#2e75b6', alpha=1, edgecolor='black', linewidth=2)

plt.xlabel('Year', fontweight='bold', fontsize=20)
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
plt.savefig('./ExpFigures/FIB_size_year.pdf')
plt.show()

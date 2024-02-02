import numpy as np
import matplotlib.pyplot as plt

plt.rc('font', family='Arial')
times = ["1K","4K","16K","64K"]
src = [2.01,2.24,100.194,634.282]
dst = [5.47,9.5,88.517,402.256]
total = [src[i]+dst[i] for i in range(len(src))]
# src = [i/1000 for i in src]
# dst = [i/1000 for i in dst]
x = [i for i in range(len(times))]

# 柱状图的宽度
bar_width = 0.25

# 调整lambdas以便于柱状图的绘制，避免重叠
x_a = [i-bar_width for i in x]
x_b = [i for i in x]
x_c = [i+bar_width for i in x]
plt.figure(figsize=(6,4.5))
plt.yscale('log')
# plt.xticks(np.arange(0.9, 1.21, 0.05))
plt.xticks(x_b, times)
plt.tick_params(labelsize=19)
ax1 = plt.gca()

# 绘制柱状图
plt.bar(x_a, src, width=bar_width, label=r'SrcIP', color='#2e75b6', alpha=1, edgecolor='black', linewidth=2)
plt.bar(x_b, dst, width=bar_width, label=r'DstIP', color='#ed7d31', alpha=1, edgecolor='black', linewidth=2)
plt.bar(x_c, total, width=bar_width, label=r'Total', color='#70ad47', alpha=1, edgecolor='black', linewidth=2)

plt.xlabel('ACL Entry Size', fontweight='bold', fontsize=20)
plt.ylabel('Memory(KB)', fontweight='bold', fontsize=20)

# 设置图例
plt.legend(loc='upper left', ncol=1, handlelength=3)
leg = plt.gca().get_legend()
ltext = leg.get_texts()
plt.setp(ltext, fontweight='bold', fontsize=20)

# 设置网格
ax1.grid(True, linestyle='--', axis='x')
ax1.grid(True, linestyle='--', axis='y')

plt.tight_layout()

# 保存图像
plt.savefig('./ExpFigures/acl_mem.pdf')
plt.show()

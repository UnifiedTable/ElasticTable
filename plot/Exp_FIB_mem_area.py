import numpy as np
import matplotlib.pyplot as plt

plt.rc('font', family='Arial')
times = ["Area1","Area2","Area3","Area4","Area5"]
ut = [497595,395993,456669,261505,510264]
cf = [895680,781632,861184,799744,906432]
ch = [3519881,1303014,1587583,874422,1848255]
ut = [i/1000 for i in ut]
cf = [i/1000 for i in cf]
ch = [i/1000 for i in ch]
x = [i for i in range(len(times))]

# 柱状图的宽度
bar_width = 0.25

# 调整lambdas以便于柱状图的绘制，避免重叠
x_a = [i-bar_width for i in x]
x_b = x
x_c = [i+bar_width for i in x]

plt.figure(figsize=(6,4.5))
plt.yscale('linear')
# plt.xticks(np.arange(0.9, 1.21, 0.05))
plt.xticks(x_b, times)
plt.tick_params(labelsize=19)
ax1 = plt.gca()

# 绘制柱状图
plt.bar(x_a, ut, width=bar_width, label=r'UnifiedTable', color='#2e75b6', alpha=1, edgecolor='black', linewidth=2)
plt.bar(x_b, cf, width=bar_width, label=r'SAIL', color='#ed7d31', alpha=1, edgecolor='black', linewidth=2)
plt.bar(x_c, ch, width=bar_width, label=r'PBF', color='#70ad47', alpha=1, edgecolor='black', linewidth=2)

plt.xlabel('Dataset', fontweight='bold', fontsize=20)
plt.ylabel('Memory(KB)', fontweight='bold', fontsize=20)

# 设置图例
plt.legend(loc='upper right', ncol=1, handlelength=3)
leg = plt.gca().get_legend()
ltext = leg.get_texts()
plt.setp(ltext, fontweight='bold', fontsize=20)

# 设置网格
ax1.grid(True, linestyle='--', axis='x')
ax1.grid(True, linestyle='--', axis='y')

plt.tight_layout()

# 保存图像
plt.savefig('./ExpFigures/FIB_area.pdf')
plt.show()

import numpy as np
import matplotlib.pyplot as plt

plt.rc('font', family='Arial')
times = [1,2,4,8,16]
ut = [616.449,1140.32,2265.07,4431.64,8539.14]
cf = [308.104,591.75,1150.57,2251.27,4412.77]
ch = [63.614,90.796,130.003,183.175,261.069]
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
plt.bar(x_a, ut, width=bar_width, label=r'ElasticTable', color='#2e75b6', alpha=1, edgecolor='black', linewidth=2)
plt.bar(x_b, cf, width=bar_width, label=r'CuckooFilter', color='#ed7d31', alpha=1, edgecolor='black', linewidth=2)
plt.bar(x_c, ch, width=bar_width, label=r'BasicHash', color='#70ad47', alpha=1, edgecolor='black', linewidth=2)

plt.xlabel('Memory(KB)', fontweight='bold', fontsize=20)
plt.ylabel('Entries', fontweight='bold', fontsize=20)

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
plt.savefig('./ExpFigures/em_mac_mem_entry.pdf')
plt.show()

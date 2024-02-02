import numpy as np
import matplotlib.pyplot as plt
plt.rc('font', family='Arial')
lambdas = np.arange(0.9, 1.20, 0.02)
success_rate1 = [0.997, 0.998, 1, 0.995, 0.997, 0.997, 0.991, 0.877, 0.356, 0.032, 0, 0, 0, 0, 0]
success_rate2 = [1, 0.998, 1, 1, 1, 0.999, 0.998, 0.985, 0.423, 0.003, 0, 0, 0, 0, 0]
success_rate3 = [1, 1, 1, 0.987, 0.988, 1, 1, 0.984, 0.505, 0.008, 0, 0, 0, 0, 0]
plt.figure(figsize=(6,4.5))
plt.yscale('linear')
plt.xticks(np.arange(0.9, 1.21, 0.05))
plt.tick_params(labelsize = 19)
ax1 = plt.gca()
plt.tick_params(labelsize = 19)

plt.plot(lambdas, success_rate1, label=r'$0.5\times 10^4$', color='#2e75b6', linestyle='-', marker='s', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate2, label=r'$1.0\times 10^4$', color='#ed7d31', linestyle='-', marker='D', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate3, label=r'$1.5\times 10^4$', color='#70ad47', linestyle='-', marker='^', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)

plt.xlabel('$\\lambda$',fontweight='bold',fontsize=20)
plt.ylabel('Success Rate',fontweight='bold',fontsize=20)

plt.legend(loc = 'lower left', ncol=1, handlelength=3)
leg = plt.gca().get_legend() #或leg=ax.get_legend()
ltext = leg.get_texts()
plt.setp(ltext,fontweight='bold',fontsize = 20)
#plt.setp(ltext,fontsize = 22)

ax1.grid(True, linestyle='--', axis='x')
ax1.grid(True, linestyle='--', axis='y')
#plt.legend(fontsize = 16)
# plt.grid(True, which = 'both', linestyle='--', axis='y')
# plt.grid(True, linestyle='--', axis='x')
plt.tight_layout()

plt.savefig('./ExpFigures/static_success_rate_vs_lambda_large_range_mac.pdf')
plt.show()

import numpy as np
import matplotlib.pyplot as plt
plt.rc('font', family='Arial')
lambdas = np.arange(1, 1.101, 0.005)
success_rate1 = [1, 1, 1, 1, 1, 1, 0.99, 0.97, 0.96, 0.88, 0.79, 0.51, 0.36, 0.23, 0.11, 0.03, 0.02, 0, 0, 0, 0]
success_rate2 = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.94, 0.5, 0.06, 0, 0, 0, 0, 0, 0, 0]
success_rate3 = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.85, 0, 0, 0, 0, 0, 0, 0, 0]
plt.figure(figsize=(6,4.5))
plt.yscale('linear')
plt.xticks(np.arange(1, 1.101, 0.02))
plt.tick_params(labelsize = 19)
ax1 = plt.gca()
plt.tick_params(labelsize = 19)

plt.plot(lambdas, success_rate1, label=r'$1.0\times 10^4$', color='#2e75b6', linestyle='-', marker='s', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate2, label=r'$1.0\times 10^5$', color='#ed7d31', linestyle='-', marker='D', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate3, label=r'$1.0\times 10^6$', color='#70ad47', linestyle='-', marker='^', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)

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

plt.savefig('./ExpFigures/static_success_rate_vs_lambda_small_range_random.pdf')
plt.show()

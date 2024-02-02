import numpy as np
import matplotlib.pyplot as plt
plt.rc('font', family='Arial')
lambdas = np.arange(1, 1.101, 0.005)
success_rate1 = [0.995, 0.996, 0.992, 0.99, 0.988, 0.974, 0.955, 0.91, 0.847, 0.75, 0.647, 0.494, 0.383, 0.25, 0.147, 0.07, 0.04, 0.009, 0.002, 0.002, 0]
success_rate2 = [0.999, 1, 1, 1, 1, 0.999, 0.996, 0.982, 0.958, 0.902, 0.783, 0.608, 0.416, 0.221, 0.106, 0.033, 0.008, 0.001, 0, 0, 0]
success_rate3 = [1, 0.976, 0.988, 0.981, 0.983, 0.983, 0.987, 0.985, 0.972, 0.963, 0.902, 0.739, 0.542, 0.266, 0.098, 0.019, 0.005, 0, 0, 0, 0]
plt.figure(figsize=(6,4.5))
plt.yscale('linear')
plt.xticks(np.arange(1, 1.101, 0.02))
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

plt.savefig('./ExpFigures/static_success_rate_vs_lambda_small_range_real.pdf')
plt.show()

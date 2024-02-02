import numpy as np
import matplotlib.pyplot as plt
plt.rc('font', family='Arial')
lambdas = np.arange(0.8, 1.5, 0.04)
success_rate1 = [1, 0.9999, 0.9997, 0.9963, 0.9736, 0.9337, 0.9035, 0.8706, 0.8446, 0.8169, 0.7797, 0.7563, 0.7234, 0.7026, 0.6706, 0.6343, 0.6151, 0.5853]
success_rate2 = [1, 0.99998, 0.99997, 0.98809, 0.95442, 0.92424, 0.89791, 0.86905, 0.84256, 0.81286, 0.78136, 0.75378, 0.72463, 0.69899, 0.66906, 0.63705, 0.6103, 0.58176]
success_rate3 = [1, 0.999998, 0.999962, 0.981413, 0.951722, 0.923854, 0.896229, 0.868468, 0.83974, 0.8106, 0.781742, 0.752362, 0.723282, 0.695194, 0.666481, 0.639021, 0.610175, 0.584041]
plt.figure(figsize=(6,4.5))
plt.yscale('linear')
plt.xticks(np.arange(0.8, 1.51, 0.1))
plt.ylim(0, 1.05)
plt.tick_params(labelsize = 19)
ax1 = plt.gca()
plt.tick_params(labelsize = 19)

plt.plot(lambdas, success_rate1, label=r'$1.0\times 10^4$', color='#2e75b6', linestyle='-', marker='s', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate2, label=r'$1.0\times 10^5$', color='#ed7d31', linestyle='-', marker='D', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate3, label=r'$1.0\times 10^6$', color='#70ad47', linestyle='-', marker='^', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)

plt.xlabel('$\\lambda$',fontweight='bold',fontsize=20)
plt.ylabel('Success Rate',fontweight='bold',fontsize=20)

plt.legend(loc = 'lower right', ncol=1, handlelength=3)
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

plt.savefig('./ExpFigures/dynamic_success_rate_vs_lambda_random.pdf')
plt.show()


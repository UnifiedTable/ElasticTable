import numpy as np
import matplotlib.pyplot as plt
plt.rc('font', family='Arial')
lambdas = np.arange(0.8, 1.5, 0.04)
success_rate1 = [0, 3.60109, 8.19018, 32.3707, 17.3038, 2.41384, 1.58838, 1.37236, 1.2743, 1.14123, 1.00608, 0.910429, 0.885815, 0.781437, 0.787481, 0.715376, 0.677196, 0.611093]
success_rate2 = [0, 3.81657, 8.95855, 15.4246, 2.75745, 1.93835, 1.61319, 1.41188, 1.22422, 1.1033, 1.01562, 0.917227, 0.849142, 0.7928, 0.74516, 0.690267, 0.64299, 0.609784]
success_rate3 = [0, 3.72928, 9.60985, 5.24352, 2.51733, 1.91531, 1.58406, 1.37673, 1.22006, 1.10041, 1.01575, 0.924602, 0.857646, 0.79591, 0.742099, 0.691379, 0.650528, 0.607963]

plt.figure(figsize=(6,4.5))
plt.yscale('linear')
plt.xticks(np.arange(0.8, 1.51, 0.1))
plt.ylim(0, 40)
plt.tick_params(labelsize = 19)
ax1 = plt.gca()
plt.tick_params(labelsize = 19)

plt.plot(lambdas, success_rate1, label=r'$1.0\times 10^4$', color='#2e75b6', linestyle='-', marker='s', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate2, label=r'$1.0\times 10^5$', color='#ed7d31', linestyle='-', marker='D', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate3, label=r'$1.0\times 10^6$', color='#70ad47', linestyle='-', marker='^', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)

plt.xlabel('$\\lambda$',fontweight='bold',fontsize=20)
plt.ylabel('Average Change Number',fontweight='bold',fontsize=20)

plt.legend(loc = 'upper right', ncol=1, handlelength=3)
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

plt.savefig('./ExpFigures/dynamic_ave_num_vs_lambda_random.pdf')
plt.show()


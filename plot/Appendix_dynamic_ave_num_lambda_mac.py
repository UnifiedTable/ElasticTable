import numpy as np
import matplotlib.pyplot as plt
plt.rc('font', family='Arial')
lambdas = np.arange(0.8, 1.5, 0.04)
success_rate1 = [0, 3.55775, 7.60948, 25.1023, 45.865, 6.3485, 1.94666, 1.47638, 1.2442, 1.13218, 1.02731, 0.929497, 0.882509, 0.802818, 0.730832, 0.695527, 0.648848, 0.60623]
success_rate2 = [0, 3.4361, 8.07302, 29.368, 19.9414, 2.57531, 1.7029, 1.42714, 1.24185, 1.12509, 1.0079, 0.929492, 0.858112, 0.798305, 0.740127, 0.69628, 0.64955, 0.610525]
success_rate3 = [0, 3.66778, 8.14539, 28.2969, 11.5131, 2.22216, 1.67496, 1.41633, 1.23279, 1.11542, 1.00867, 0.935105, 0.847296, 0.806528, 0.744651, 0.694523, 0.650483, 0.614523]


plt.figure(figsize=(6,4.5))
plt.yscale('linear')
plt.xticks(np.arange(0.8, 1.51, 0.1))
plt.ylim(0, 55)
plt.tick_params(labelsize = 19)
ax1 = plt.gca()
plt.tick_params(labelsize = 19)

plt.plot(lambdas, success_rate1, label=r'$0.5\times 10^4$', color='#2e75b6', linestyle='-', marker='s', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate2, label=r'$1.0\times 10^4$', color='#ed7d31', linestyle='-', marker='D', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
plt.plot(lambdas, success_rate3, label=r'$1.5\times 10^4$', color='#70ad47', linestyle='-', marker='^', markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)

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

plt.savefig('./ExpFigures/dynamic_ave_num_vs_lambda_mac.pdf')
plt.show()

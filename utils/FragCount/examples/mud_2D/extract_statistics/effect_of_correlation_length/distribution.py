# Author: Jake Vanderplas <jakevdp@cs.washington.edu>
#
import math
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from scipy.stats import norm
from sklearn.neighbors import KernelDensity
import csv

param = {'density': True}

num_samples = 10
prefix = "../../"
infix = ["exp_cartesian_5_5","exp_cartesian_10_10","exp_cartesian_20_20","sqexp_cartesian_5_5","sqexp_cartesian_10_10","sqexp_cartesian_20_20"]
postfix = "/out_size_step_221.dat"

#----------------------------------------------------------------------
# Compare coefficients of correlation
fig1, ax1 = plt.subplots(2, len(infix))
fig2, ax2 = plt.subplots()

for i in range(len(infix)):
    size = []
    for n in range(num_samples):
        with open(prefix+infix[i]+"_rho_0_sample_"+str(n+1)+postfix) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=' ')
            for row in csv_reader:
                size.append(float(row[0]))
    size = np.asarray(size)
    min = np.amin(size)
    max = np.amax(size)
    X_plot = np.linspace(min, max, 200)
    bins = np.linspace(min, max, 50)
    ax1[0,i].hist(size, bins=bins, fc='#AAAAFF')
    ax1[0,i].title.set_text(infix[i])

    # Gaussian KDE
    kde = KernelDensity(kernel='gaussian', bandwidth=(max-min)/60).fit(size.reshape(-1,1))
    log_dens = kde.score_samples(X_plot.reshape(-1,1))
    ax1[1,i].plot(X_plot, np.exp(log_dens))
    ax2.plot(X_plot, np.exp(log_dens), label=infix[i])

    concat = np.column_stack((X_plot, np.exp(log_dens)))
    np.savetxt("distribution_"+infix[i]+".csv", concat, delimiter=" ")

ax1[0,0].set_ylabel('count')
ax1[1,0].set_ylabel('estimated pdf')
ax1[1,0].set_xlabel('fragment size')

ax2.legend(loc='upper right')
# ax2.set_xlim(0,200)
ax2.set_xlabel('fragment size')
ax2.set_ylabel('estimated pdf')

plt.show()

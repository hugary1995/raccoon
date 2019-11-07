# Author: Jake Vanderplas <jakevdp@cs.washington.edu>
#
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from scipy.stats import norm
from sklearn.neighbors import KernelDensity
import csv

param = {'density': True}

num_samples = 10
prefix = "../../"
kernels = ["exp","sqexp"]
infix = ["0","0.5","1"]
postfix = "/out_size_step_221.dat"

#----------------------------------------------------------------------
# Compare coefficients of correlation
fig1, ax1 = plt.subplots(2, 6)
fig2, ax2 = plt.subplots()

for k in range(len(kernels)):
    for i in range(len(infix)):
        size = []
        for n in range(num_samples):
            with open(prefix+kernels[k]+"_cartesian_5_5_rho_"+infix[i]+"_sample_"+str(n+1)+postfix) as csv_file:
                csv_reader = csv.reader(csv_file, delimiter=' ')
                for row in csv_reader:
                    size.append(float(row[0]))
        size = np.asarray(size)
        min = np.amin(size)
        max = np.amax(size)
        X_plot = np.linspace(min, max, 200)
        bins = np.linspace(min, max, 50)
        ax1[0,k*len(infix)+i].hist(size, bins=bins, fc='#AAAAFF')
        ax1[0,k*len(infix)+i].title.set_text(kernels[k]+" "+infix[i])

        # Gaussian KDE
        kde = KernelDensity(kernel='gaussian', bandwidth=5).fit(size.reshape(-1,1))
        log_dens = kde.score_samples(X_plot.reshape(-1,1))
        ax1[1,k*len(infix)+i].plot(X_plot, np.exp(log_dens))
        ax2.plot(X_plot, np.exp(log_dens), label=kernels[k]+" "+infix[i])

        concat = np.column_stack((X_plot, np.exp(log_dens)))
        np.savetxt("distribution_"+kernels[k]+"_rho_"+str(i+1)+".csv", concat, delimiter=" ")

ax1[0,0].set_ylabel('count')
ax1[1,0].set_ylabel('estimated pdf')
ax1[1,0].set_xlabel('fragment size')

ax2.legend(loc='upper right')
# ax2.set_xlim(0,200)
ax2.set_xlabel('fragment size')
ax2.set_ylabel('estimated pdf')

plt.show()

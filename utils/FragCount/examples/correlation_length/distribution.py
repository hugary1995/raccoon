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
prefix = "sample_"
postfix = "_size_step_221.dat"
L = ['5','10','15','20']

#----------------------------------------------------------------------
# Compare coefficients of correlation
fig1, ax1 = plt.subplots(2, 4)
fig2, ax2 = plt.subplots()

for i in range(len(L)):
    size = []
    for n in range(num_samples):
        with open(L[i]+"/"+prefix+str(n+1)+postfix) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=' ')
            for row in csv_reader:
                size.append(float(row[0]))
    size = np.asarray(size)
    min = np.amin(size)
    max = np.amax(size)
    X_plot = np.linspace(0, 300, 100)
    bins = np.linspace(min, max, 50)
    ax1[0,i].hist(size, bins=bins, fc='#AAAAFF')
    ax1[0,i].title.set_text("L = "+L[i])

    # Gaussian KDE
    kde = KernelDensity(kernel='gaussian', bandwidth=20).fit(size.reshape(-1,1))
    log_dens = kde.score_samples(X_plot.reshape(-1,1))
    ax1[1,i].plot(X_plot, np.exp(log_dens))
    ax2.plot(X_plot, np.exp(log_dens), label="L = "+L[i])

    concat = np.column_stack((X_plot, np.exp(log_dens)))
    np.savetxt("dist_"+L[i]+".dat", concat, delimiter=" ")

ax1[0,0].set_ylabel('count')
ax1[1,0].set_ylabel('estimated pdf')
ax1[1,0].set_xlabel('fragment size')

ax2.legend(loc='upper right')
ax2.set_xlim(0,300)
ax2.set_xlabel('fragment size')
ax2.set_ylabel('estimated pdf')

plt.show()

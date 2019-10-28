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
prefix = "rho_"
postfix = "_size_step_221.dat"
infix = ["0","0.25","0.5","0.75","1"]

#----------------------------------------------------------------------
# Compare coefficients of correlation
fig1, ax1 = plt.subplots(2, 5)
fig2, ax2 = plt.subplots()

for i in range(len(infix)):
    size = []
    for n in range(num_samples):
        with open("sample_"+str(n+1)+"/"+prefix+infix[i]+postfix) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=' ')
            # skip header
            csv_reader.next()
            for row in csv_reader:
                size.append(float(row[0]))
    size = np.asarray(size)
    min = np.amin(size)
    max = np.amax(size)
    X_plot = np.linspace(min, max, 1000)
    bins = np.linspace(min, max, 50)
    ax1[0,i].hist(size, bins=bins, fc='#AAAAFF')
    ax1[0,i].title.set_text("rho = "+infix[i])

    # Gaussian KDE
    kde = KernelDensity(kernel='gaussian', bandwidth=5).fit(size.reshape(-1,1))
    log_dens = kde.score_samples(X_plot.reshape(-1,1))
    ax1[1,i].plot(X_plot, np.exp(log_dens))
    ax2.plot(X_plot, np.exp(log_dens), label="row = "+infix[i])

ax1[0,0].set_ylabel('count')
ax1[1,0].set_ylabel('estimated pdf')
ax1[1,0].set_xlabel('fragment size')

ax2.legend(loc='upper right')
ax2.set_xlim(0,200)
ax2.set_xlabel('fragment size')
ax2.set_ylabel('estimated pdf')

#----------------------------------------------------------------------
# Compare sample wise
fig3, ax3 = plt.subplots(2, 10, sharey='row')
fig4, ax4 = plt.subplots()

for n in range(num_samples):
    size = []
    for i in range(len(infix)):
        with open("sample_"+str(n+1)+"/"+prefix+infix[i]+postfix) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=' ')
            # skip header
            csv_reader.next()
            for row in csv_reader:
                size.append(float(row[0]))
    size = np.asarray(size)
    min = np.amin(size)
    max = np.amax(size)
    X_plot = np.linspace(min, max, 1000)
    bins = np.linspace(min, max, 50)
    ax3[0,n].hist(size, bins=bins, fc='#AAAAFF')
    ax3[0,n].title.set_text("sample = "+str(n+1))

    # Gaussian KDE
    kde = KernelDensity(kernel='gaussian', bandwidth=5).fit(size.reshape(-1,1))
    log_dens = kde.score_samples(X_plot.reshape(-1,1))
    ax3[1,n].plot(X_plot, np.exp(log_dens))
    ax4.plot(X_plot, np.exp(log_dens), label="sample "+str(n+1))

ax3[0,0].set_ylabel('count')
ax3[1,0].set_ylabel('estimated pdf')
ax3[1,0].set_xlabel('fragment size')

ax4.legend(loc='upper right')
ax4.set_xlim(0,200)
ax4.set_xlabel('fragment size')
ax4.set_ylabel('estimated pdf')


plt.show()

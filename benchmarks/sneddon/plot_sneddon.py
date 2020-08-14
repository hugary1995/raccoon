import pandas
import matplotlib.pyplot as plt
import numpy as np
import math
from progressbar import ProgressBar

pbar = ProgressBar()

opening = np.zeros(69)
x_coord = np.zeros(69)

ax = plt.gca()

for i in pbar(range(69)):
    data = pandas.read_csv(
        'mechanical_fracture_uo_out_cod' + str(i + 1) + "_0001.csv")
    cod = data["cod"]
    x = data["x"]
    y = data["y"]
    x_coord[i] = x[i]
    for j in range(0, data.shape[0] - 1):
        opening[i] = opening[i] + (y[j + 1] - y[j]) * (cod[j + 1] + cod[j]) / 2

plt.plot(x_coord, -opening, label='Numerical solution', color='k',
         linestyle='-', linewidth=1, markersize=6, markerfacecolor='none')

x0 = np.arange(1.8, 2.2, 0.001)

plt.plot(x0, 4 * 1.0e-3 * 0.2 / 1.0 * (1.0 - (x0 - 2.0)**2 / 0.2**2)**0.5,
         label='Analytical solution', color='k', linestyle='-.', linewidth=1, markersize=6, markerfacecolor='none')

plt.title("Sneddon Problem")
plt.xlabel("X")
plt.ylabel("Crack opening displacement")

plt.tight_layout()

ax.legend()
plt.savefig('sneddon.pdf')

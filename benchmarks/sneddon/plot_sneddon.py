import pandas
import matplotlib.pyplot as plt
import numpy as np
import math

xis = [1]
ls = [0.00625]
rs = [6, 7, 8]

for xi in xis:
    for l in ls:
        for r in rs:
            opening = np.zeros(65)
            x_coord = np.zeros(65)
            for i in range(65):
                data = pandas.read_csv(
                    "l_" + str(l) + "_xi_" + str(xi) + "_r_" + str(r) + "/cod_cod" + str(i + 1) + "_0001.csv")
                cod = data["cod"]
                x = data["x"]
                y = data["y"]
                x_coord[i] = x[i]
                for j in range(0, data.shape[0] - 1):
                    opening[i] = opening[i] + \
                        (y[j + 1] - y[j]) * (cod[j + 1] + cod[j]) / 2

            plt.plot(x_coord, -opening, label="l = " + str(l) + ", xi = " + str(xi) + ", h = " + str(0.25 / pow(2, r)),
                     linestyle='-', linewidth=1)

            x0 = np.arange(1.8, 2.2, 0.001)

plt.plot(x0, 2 * 1.0e-3 * 0.2 * (1.0 - 0.04) / 1.0 * (1.0 - (x0 - 2.0)**2 / 0.2**2)**0.5,
         label='Analytical solution', color='k', linestyle='-.', linewidth=1)

plt.title("Sneddon Problem")
plt.xlabel("X")
plt.ylabel("Crack opening displacement")

plt.tight_layout()

ax = plt.gca()
ax.legend()

plt.show()

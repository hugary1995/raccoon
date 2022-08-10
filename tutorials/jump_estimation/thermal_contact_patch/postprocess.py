import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data/l_over_h_16_T_0002.csv")

y = df["y"]
T = df["T"]

k = 1
h = 5e-3
u_jump = 0.01
h_bar = h / u_jump
k_bar = h_bar / (h_bar + k)
Tl = 0
Tr = 100
T_sol_l = k_bar * (Tr - Tl) * y + Tl
T_sol_r = k_bar * (Tr - Tl) * y + Tr - k_bar * (Tr + Tl)

plt.plot(y, T, label="approximation")
plt.plot(y, T_sol_l, label="solution l")
plt.plot(y, T_sol_r, label="solution r")
plt.xlim(0)
plt.ylim(0)
plt.legend()
plt.show()

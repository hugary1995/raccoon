import pandas as pd
import matplotlib.pyplot as plt

l = 0.025
bandwidth = 4 * l

df_w = pd.read_csv("data/l_over_h_32_w_0001.csv")
df_uy_plus = pd.read_csv("data/l_over_h_16_uy+_0001.csv")
df_uy_minus = pd.read_csv("data/l_over_h_16_uy-_0001.csv")

x = df_w["x"]
w = df_w["w"]
uy_plus = df_uy_plus["disp_y"]
uy_minus = df_uy_minus["disp_y"]

plt.plot(x, w * bandwidth, label="approximation")
plt.plot(x, uy_plus - uy_minus, label="reference")
plt.ylim(0)
plt.legend()
plt.show()

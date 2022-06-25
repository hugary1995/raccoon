import pandas as pd
import matplotlib.pyplot as plt

l = 0.024
bandwidth = 4 * l

df_w = pd.read_csv("data/e_0.0015_w_0001.csv")
df_uy_plus = pd.read_csv("data/e_0.0015_uy+_0001.csv")
df_uy_minus = pd.read_csv("data/e_0.0015_uy-_0001.csv")

x = df_w["x"]
w = df_w["w"]
uy_plus = df_uy_plus["disp_y"]
uy_minus = df_uy_minus["disp_y"]

plt.plot(x, w * bandwidth)
plt.plot(x, uy_plus - uy_minus)
plt.show()

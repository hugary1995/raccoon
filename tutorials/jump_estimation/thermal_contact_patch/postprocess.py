import pandas as pd
import matplotlib.pyplot as plt

df_wT = pd.read_csv("data/l_over_h_8_wT_y_0001.csv")
df_T_plus = pd.read_csv("data/l_over_h_8_T+_0001.csv")
df_T_minus = pd.read_csv("data/l_over_h_8_T-_0001.csv")

x = df_wT["x"]
wT = df_wT["wT_y"]
T_plus = df_T_plus["T"]
T_minus = df_T_minus["T"]

plt.plot(x, wT, label="approximation")
plt.plot(x, T_plus - T_minus, label="reference")
plt.ylim(0)
plt.legend()
plt.show()

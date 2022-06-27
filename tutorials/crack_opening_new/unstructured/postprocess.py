import pandas as pd
import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter1d


l = 0.024
h = 0.00078125
bandwidth = 4 * l

df_w = pd.read_csv("data/h_" + str(h) + "_w_0001.csv")
df_uy_plus = pd.read_csv("../structured/data/l_over_h_16_uy+_0001.csv")
df_uy_minus = pd.read_csv("../structured/data/l_over_h_16_uy-_0001.csv")

x = df_w["x"]
w = df_w["w"]
uy_plus = df_uy_plus["disp_y"]
uy_minus = df_uy_minus["disp_y"]
uy_jump = uy_plus - uy_minus
uy_jump_smoothed = gaussian_filter1d(uy_jump, sigma=20)

plt.plot(x, w * bandwidth, label="approximation")
plt.plot(x, uy_jump_smoothed, label="reference")
plt.ylim(0)
plt.legend()
plt.show()

import pandas as pd
import matplotlib.pyplot as plt

xs = [0.1, 0.2, 0.3, 0.4, 0.5]

fig, ax = plt.subplots(1, len(xs))

for i, x in enumerate(xs):
    df_regularized = pd.read_csv("data/l_over_h_8_T_" + str(x) + "_0002.csv")
    df_sharp = pd.read_csv("data/sharp_T_" + str(x) + "_0011.csv")

    ax[i].plot(df_regularized["y"], df_regularized["T"],
               label="phase field crack")
    ax[i].plot(df_sharp["y"], df_sharp["T"], label="sharp crack")
    ax[i].set_xlim(0)
    ax[i].set_ylim(0)
    ax[i].legend()

plt.show()

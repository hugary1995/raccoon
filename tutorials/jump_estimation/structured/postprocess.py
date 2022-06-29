import pandas as pd
import matplotlib.pyplot as plt

for l_over_h in [4, 8, 16]:
    df_w = pd.read_csv("data/l_over_h_" + str(l_over_h) + "_w_0001.csv")
    df_uy_plus = pd.read_csv(
        "data/l_over_h_" + str(l_over_h) + "_uy+_0001.csv")
    df_uy_minus = pd.read_csv(
        "data/l_over_h_" + str(l_over_h) + "_uy-_0001.csv")

    x = df_w["x"]
    w = df_w["w"]
    uy_plus = df_uy_plus["disp_y"]
    uy_minus = df_uy_minus["disp_y"]
    df = pd.DataFrame({"x": x, "w": w, "w_ref": uy_plus - uy_minus})
    df.to_csv("data/l_over_h_" + str(l_over_h) + ".csv", index=False)

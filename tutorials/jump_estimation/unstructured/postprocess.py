import pandas as pd
import matplotlib.pyplot as plt

for h in [0.00625, 0.003125, 0.0015625]:
    df_w = pd.read_csv("data/h_" + str(h) + "_w_0001.csv")
    df_uy_plus = pd.read_csv(
        "data/h_" + str(h) + "_uy+_0001.csv")
    df_uy_minus = pd.read_csv(
        "data/h_" + str(h) + "_uy-_0001.csv")

    x = df_w["x"]
    w = df_w["w"]
    uy_plus = df_uy_plus["disp_y"]
    uy_minus = df_uy_minus["disp_y"]
    df = pd.DataFrame({"x": x, "w": w, "w_ref": uy_plus - uy_minus})
    df.to_csv("data/h_" + str(h) + ".csv", index=False)

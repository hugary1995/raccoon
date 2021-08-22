import pandas as pd
import matplotlib.pyplot as plt

# regularization length
l = 0.02
support = 4 * l

# prefix
prefix = "data/"

# file names
file_names = ["correct_normal_w_crack_tip", "correct_normal_wo_crack_tip",
              "levelset_normal_w_crack_tip", "levelset_normal_wo_crack_tip"]

# file name containing reference displacement information
file_name_u = "correct_normal_w_crack_tip"

# vector postprocessor name for u_y_plus and u_y_minus
u_y_name = "disp_y"
u_y_plus_name = "u_y_plus"
u_y_minus_name = "u_y_minus"

# crack length name and values
crack_length_name = "a"
crack_length_values = ["0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8"]

# crack opening displacement name and values
cod_name = "w"

# suffix
suffix = "0001.csv"

# file name delimiter
file_name_delimiter = "_"

for crack_length in crack_length_values:
    fig, ax = plt.subplots(figsize=(8, 4))

    # plot numerically computed crack opening displacements
    for file_name in file_names:
        file_name_combined = file_name_delimiter.join(
            [prefix + file_name, crack_length_name, crack_length, cod_name, suffix])
        data = pd.read_csv(file_name_combined)
        ax.plot(data["x"], data[cod_name] * support, label=file_name)

    # plot reference crack opening displacement u_y_plus - u_y_minus
    file_name_u_y_plus = file_name_delimiter.join(
        [prefix + file_name_u, crack_length_name, crack_length, u_y_plus_name, suffix])
    u_y_plus_data = pd.read_csv(file_name_u_y_plus)
    file_name_u_y_minus = file_name_delimiter.join(
        [prefix + file_name_u, crack_length_name, crack_length, u_y_minus_name, suffix])
    u_y_minus_data = pd.read_csv(file_name_u_y_minus)
    ax.plot(u_y_plus_data["x"], u_y_plus_data[u_y_name] -
            u_y_minus_data[u_y_name], label="reference")

    ax.set_xlim([0, 1])
    ax.set_ylim([-0.001, 0.01])
    ax.legend(bbox_to_anchor=(1.05, 1.0), loc='upper left')
    ax.set_title("crack length = " + crack_length)
    fig.tight_layout()
    fig.savefig("crack_length_" + crack_length + ".png")

# plt.show()

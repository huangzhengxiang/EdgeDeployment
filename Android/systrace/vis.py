import os
import sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

if __name__ == "__main__":
    csv = pd.read_csv("data/cpufreq_results.csv", header=None, index_col=0)
    print(csv)
    sorted_index = sorted(list(csv.index), key=lambda s: int(s.replace("cpu", "")), reverse=False)
    fig, ax = plt.subplots(nrows=len(sorted_index), ncols=1, sharex=True)
    for i in range(len(sorted_index)):
        data = csv.loc[sorted_index[i]]*1e-6
        ax[i].plot(list(range(0,len(data))), data)
        ax[i].set_ylabel("GHz", fontsize=12)
        if max(data) == min(data):
            ax[i].set_yticks([0, max(data)])
        else:
            ax[i].set_yticks([0, min(data), max(data)])
        ax[i].set_xticks([0, len(data)-1])
        # ax[i].tick_params(axis="y", labelsize=9, pad=10)
    fig.tight_layout()
    os.makedirs("pic", exist_ok=True)
    plt.savefig("pic/fig.png", dpi=1000)
    print(np.mean(csv.loc["cpu0"])*1e-6)
    print(np.mean(csv.loc["cpu6"])*1e-6)
    print(np.mean(csv.loc["cpu7"])*1e-6)

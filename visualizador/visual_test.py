import struct
from Data_Manager import Data_Manager

import numpy as np
import math
import matplotlib.pyplot as plt

def graph_data():
    num_subplots = len(datman.M_Thread_Array)

    t = np.linspace(0,2*math.pi,400)
    a = np.sin(t)
    b = np.cos(t)
    c = a + b

    plt.plot(t, a, t, b, t, c)
    plt.show()


if __name__ == "__main__":
    datman = Data_Manager()
    datman.get_data("6785.bin")
    datman.get_data("6786.bin")
    datman.get_data("6787.bin")

    graph_data()

import struct
from os import walk
from Data_Manager import Data_Manager

import numpy as np
import math
import matplotlib.pyplot as plt

measurement_path = "test/measurements/"


def graph_data():
    # TOTAL TIME
    num_threads_x_time = plt.figure(figsize=(8, 8))
    num_threads_x_time.suptitle("Total Time vs Number of Images")

    temp = len(datman.M_Thread_Array)
    graph_rows = math.ceil(temp/2)
    cont = 1

    for i in datman.M_Thread_Array:
        num_threads_x_time.add_subplot(
            graph_rows, 2, cont).title.set_text("Threads: " + str(i.id))
        num_images = [cycle * int(i.id) for cycle in i.measurements[0].cycle]

        for m in i.measurements:
            plt.plot(num_images, m.time, label=name_changer(
                m.file_name), marker="o")

        plt.legend(bbox_to_anchor=(0.95, 0.1), loc='right', borderaxespad=0.)
        plt.xlabel("Number of Images")
        plt.ylabel("Total Time")
        plt.tight_layout()

        cont += 1

    # AVERAGE TIME
    num_threads_x_avg_time = plt.figure(figsize=(8, 8))
    num_threads_x_avg_time.suptitle("Average Time vs Number of Images")

    cont = 1
    for i in datman.M_Thread_Array:
        num_threads_x_avg_time.add_subplot(
            graph_rows, 2, cont).title.set_text("Threads: " + str(i.id))
        num_images = [cycle * int(i.id) for cycle in i.measurements[0].cycle]

        for m in i.measurements:
            avg_time = []
            for x, times in enumerate(m.time):
                avg_time.append(times/num_images[x])

            plt.plot(num_images, avg_time, label=name_changer(
                m.file_name), marker="o")

        plt.legend(bbox_to_anchor=(0.95, 0.1), loc='right', borderaxespad=0.)
        plt.xlabel("Number of Images")
        plt.ylabel("Average Time")
        plt.tight_layout()
        cont += 1

    # THROUGHPUT
    tput_x_avg_time = plt.figure(figsize=(8, 8))
    tput_x_avg_time.suptitle("Throughput vs Number of Images")

    cont = 1
    for i in datman.M_Thread_Array:
        tput_x_avg_time.add_subplot(
            graph_rows, 2, cont).title.set_text("Threads: " + str(i.id))
        num_images = [cycle * int(i.id) for cycle in i.measurements[0].cycle]

        for m in i.measurements:
            avg_time = []
            for x, times in enumerate(m.time):
                avg_time.append(1/(times/num_images[x]))

            plt.plot(num_images, avg_time, label=name_changer(
                m.file_name), marker="o")

        plt.legend(bbox_to_anchor=(0.95, 0.1), loc='right', borderaxespad=0.)
        plt.xlabel("Number of Images")
        plt.ylabel("Throughput (images/second)")
        plt.tight_layout()
        cont += 1

    plt.show()


def name_changer(name):
    if name == (measurement_path + "6785.bin"):
        return "Secuencial"
    elif name == (measurement_path + "6786.bin"):
        return "Heavy Process"
    elif name == (measurement_path + "6787.bin"):
        return "Pre-Heavy Process"
    else:
        return name


if __name__ == "__main__":
    datman = Data_Manager()
    _, _, filenames = next(walk(measurement_path), (None, None, []))
    for f_name in filenames:
        datman.get_data(measurement_path + f_name)

    graph_data()

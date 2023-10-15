import pandas as pd
import matplotlib.pyplot as plt
from ImportSave import serial2_csv

# import numpy as np

serial2_csv(arduino_com_port="COM4", file_path="line_follower_data.csv")

# read csv and assign names
lf_df = pd.read_csv(
    "line_follower_data.csv",
    header=0,
    names=["ir_1", "ir_2", "ir_3", "left_motor", "right_motor", "time"],
)

## delete last row with end signal 0,0,0
lf_df = lf_df[:-1]

# Example Values
"""
time_values = np.linspace(0, 3600, 5)
lf_df = {
    "time": time_values,
    "ir_1": [10, 15, 20, 25, 30],
    "ir_2": [12, 18, 24, 30, 36],
    "ir_3": [14, 21, 28, 35, 42],
    "left_motor": [0.5, 0.6, 0.7, 0.8, 0.9],
    "right_motor": [0.6, 0.7, 0.8, 0.9, 1.0],
}
"""

##Plotting
fig, ax = plt.subplots()
fig.subplots_adjust(right=0.75)

# second y axis
twin = ax.twinx()

(p1,) = ax.plot(lf_df["time"], lf_df["ir_1"], "c-", label="IR_1")
(p2,) = ax.plot(lf_df["time"], lf_df["ir_2"], "m-", label="IR_2")
(p3,) = ax.plot(lf_df["time"], lf_df["ir_3"], "y-", label="IR_3")
(p4,) = twin.plot(lf_df["time"], lf_df["left_motor"], "k-", label="Left Motor")
(p5,) = twin.plot(lf_df["time"], lf_df["right_motor"], "g-", label="Right Motor")

ax.set_xlabel("Time")
ax.set_ylabel("IR Value(Voltage)")
twin.set_ylabel("Motor Speed")

ax.legend(handles=[p1, p2, p3, p4, p5])

plt.show()

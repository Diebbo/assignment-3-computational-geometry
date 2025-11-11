import numpy as np
import os

# move to the directory parent to print the script file
os.chdir(os.path.dirname(os.path.abspath(__file__)))
os.chdir("..")

file_name = "parabola_points.txt"


a, b, c = 1.0, 0.0, 0.0

# number of points
n = 302
# x values
x_vals = np.linspace(-2, 2, n)
# compute y values
y_vals = a * x_vals**2 + b * x_vals + c

# write to file
with open(file_name, "w") as f:
    f.write(f"{n}\n")
    for x, y in zip(x_vals, y_vals):
        f.write(f"{x:.1f} {y:.1f}\n")


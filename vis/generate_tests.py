import numpy as np # type: ignore
import os # type: ignore

# move to the directory parent to print the script file
if not os.path.exists("build/tests/"):
    os.mkdir("build/tests/")

LOWER_X = -10.0
UPPER_X = 10.0
R = 10.0

types = {
    # generate the points according to the function
    "parabola": lambda x:  x**2,
    # uniformly distributed points in a square
    "square": lambda x: np.random.uniform(LOWER_X, UPPER_X),
    # points on a circle
    "circle": lambda x: np.random.choice([(R**2 - x**2)**0.5, -(R**2 - x**2)**0.5])
}

for shape in types.keys():
    path = os.path.join("build/tests/", f"{shape}_tests")
    if not os.path.exists(path):
        os.mkdir(path)

    for size in [2 **i for i in range(8, 20)]:
        file_name = f"parabola_points_{size}.txt"


# number of points
        n = size
# x values
        x_vals = np.random.uniform(LOWER_X, UPPER_X, n)
# compute y values
        y_vals = [types[shape](x) for x in x_vals]

# write to file
        with open(os.path.join(path, file_name), "w") as f:
            f.write(f"{n}\n")
            for x, y in zip(x_vals, y_vals):
                f.write(f"{x:.1f} {y:.1f}\n")


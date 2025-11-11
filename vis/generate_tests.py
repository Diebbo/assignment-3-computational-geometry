import numpy as np
import random
import os

# create output folder
os.makedirs("build/tests/", exist_ok=True)
types = {
    "parabola": lambda x: x**2,
    "square": lambda x: np.random.uniform(LOWER_X, UPPER_X),
    "circle": lambda x: np.random.choice([(R**2 - x**2)**0.5, -(R**2 - x**2)**0.5])
}

for shape in types.keys():
    path = os.path.join("build/tests/", f"{shape}_tests")
    os.makedirs(path, exist_ok=True)

    for size in [2**i for i in range(8, 20)]:
        print(f"Generating {shape} test with size {size}")
        LOWER_X = -10.0 * (size / 256)
        UPPER_X = 10.0 * (size / 256)
        R = 10.0 * (size / 256)

        # generate unique x values
        x_vals = set()
        limit_low, limit_high = (LOWER_X, UPPER_X) if shape != "circle" else (-R, R)

        # keep generating until enough unique x values
        while len(x_vals) < size:
            x = round(random.uniform(limit_low, limit_high), 3)
            x_vals.add(x)

        x_vals = sorted(x_vals)
        y_vals = [types[shape](x) for x in x_vals]

        file_name = f"{shape}_points_{size}.txt"
        with open(os.path.join(path, file_name), "w") as f:
            f.write(f"{size}\n")
            for x, y in zip(x_vals, y_vals):
                f.write(f"{x:.3f} {y:.3f}\n")

from manim import RED, WHITE, Arrow, Axes, Dot, MovingCameraScene  # type: ignore

import os

# open script directory and go one level up to find points.txt
script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(os.path.join(script_dir, ".."))

ponints_file = 'points.txt'
path = os.path.exists(ponints_file)
if not path:
    raise FileNotFoundError(f"{ponints_file} not found in {os.getcwd()}")

# read points from file 'points.txt'
pts = []
with open(ponints_file, 'r') as f:
    # first line is number of points
    n = int(f.readline().strip())
    for line in f:
        x, y = map(float, line.strip().split())
        pts.append((x, y))

# hull = []
# with open('hull.txt', 'r') as f:
#     n2 = int(f.readline().strip())
#     for line in f:
#         x, y = map(float, line.strip().split())
#         hull.append((x, y))
hull = [(-1, 0), (1, 2), (4, 3), (6, 1), (2, -2)]
hull = [(x, y, 0) for x, y in hull]


class CreateCircle(MovingCameraScene):
    def construct(self):
        points = []
        for x, y in pts:
            color = RED if (x, y, 0) in hull else WHITE
            point = Dot([x, y, 0], color=color, radius=0.05)
            self.add(point)
            points.append(point)

        for i in range(len(hull) - 1):
            p1 = hull[i][0], hull[i][1], 0
            p2 = hull[i + 1][0], hull[i + 1][1], 0
            self.add(Arrow(p1, p2, color=RED))

        self.camera.auto_zoom(points)

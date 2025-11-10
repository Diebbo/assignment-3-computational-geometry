from manim import RED, WHITE, Arrow, Axes, Dot, MovingCameraScene

pts = [
    (1, 1),
    (2, 0),
    (3, 1),
    (2, 2),
    (2, 1),
]
hull = [
    (1, 1),
    (2, 2),
    (3, 1),
    (2, 0),
]
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

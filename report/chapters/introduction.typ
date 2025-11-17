#import "../bench.typ" as bench

This report compares the results obtained from implementing some Convex Hulls algorithms in both 2D.

We decided to implement the tests in C++ due to its performance, the availability of standard libraries and ease of handling with abstract data types.

It's important to note that the performance in this case are highly dependent on the hardware and the specific implementation details. Moreover, we will run the comparison on the same machine to ensure a fair comparison, with the following specifications:
- CPU: Intel i5-1135G7, 4 cores, 8 threads, 4.2 GHz
- RAM: 8 GB
- OS: Arch Linux - Kernel 6.16.7-arch1-1

== Utility Classes and Functions

To manage the 2d plane we defined a simple Point and Line class, that would handle the sidedness tests and distance calculations(see @lst:pointline).

#figure(
  caption: [Point and Line class definitions],  kind: auto,
```cpp
class Point {
public:
    float x;
    float y;
    // ...
};
void showValue(const Point &person, std::ostream &os);
class Line {
public:
    Point p1;
    Point p2;
    Line(float m, float q);
    Line(const Point& p1, const Point& p2);
    // ...
};
class Triangle {
public:
    Point p1;
    Point p2;
    Point p3;
    Triangle(const Point& p1, const Point& p2, const Point& p3);
};
```
)<lst:pointline>

Another implementation strategy that we decided to adopt was to define a ConvexHull interface (see @lst:interface) that would be implemented by each algorithm class. This way we could easily switch between algorithms and have a common method to call.

#figure(
caption: [ConvexHull interface definition],
kind: auto,
```cpp
/* Convex Hull Interface */
template <typename T>
class ConvexHull {
public:
    /* Every algorithm must implement the lower and upper hull and merge them */
    virtual T compute(const T& points) const = 0;
};
using Points = std::vector<Point>;
```
)<lst:interface>

At last we defined some aliases to make the code more readable:

#let aliases = {
  ```cpp
using TPoint = std::pair<Point, Point>;
using Points = std::vector<Point>;
```
}

#figure(
caption: [Type aliases for Points and TPoint],
kind: auto,
aliases
)<lst:aliases>


== Benchmarking strategy

To archive the most precise results possible we decided to adopt the Google Benchmarking library made ad hoc for C++. The code reads the input from a txt file containing the points coordinates and runs the benchmark for a predefined number of iterations. 

The point generation is done through a python script that creates different distributions (circle, square, parabola).

#let benchmark_code = {
  ```python
# ...
types = {
    "parabola": lambda x: x**2,
    "square": lambda x: np.random.uniform(LOWER_X, UPPER_X),
    "circle": lambda x: np.random.uniform(-(R**2 - x**2) ** 0.5, ((R**2 - x**2) ** 0.5)),
}

for shape in types.keys():
    for size in [2**i for i in range(8, 20)]:
        LOWER_X = -10.0 * (size / 256)
        UPPER_X = 10.0 * (size / 256)
        R = 10.0 * (size / 256)

        # generate unique x values
        x_vals = set()
        limit_low, limit_high = (LOWER_X, UPPER_X) if shape != "circle" else (-R, R)

        # keep generating until enough UNIQUE x values
        while len(x_vals) < size:
            x = round(random.uniform(limit_low, limit_high), 3)
            x_vals.add(x)

        x_vals = sorted(x_vals)
        y_vals = [types[shape](x) for x in x_vals]

        # writing to files
        # ...
```
}

#figure(
caption: [Python code snippet for point generation],
kind: auto,
benchmark_code
)<lst:pointgen>

The following lines from the script generates different distributions of points:

#bench.diagram_points("circle", "1024"),
#bench.diagram_points("square", "1024"),
#bench.diagram_points("parabola", "1024"),

The results are then saved in a csv file that can be later analyzed and plotted directly from Typst.

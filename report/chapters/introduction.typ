This report compares the results obtained from implementing some Convex Hulls algorithms in both 2D.

We decided to implement the tests in C++ due to its performance, the availability of standard libraries and ease of handling with abstract data types.


== Utility Classes and Functions

To manage the 2d plane we defined a simple Point and Line class, that would handle the sidedness tests and distance calculations(see @lst:pointline).

#figure(
  caption: [Point and Line class definitions],
  kind: auto,
```cpp
class Point {
public:
    float x;
    float y;
    Point();
    Point(float x, float y);
    bool operator==(const Point&) const;
    bool operator!=(const Point&) const;
    std::string to_string() const;
};
void showValue(const Point &person, std::ostream &os);
class Line {
public:
    Point p1;
    Point p2;
    Line();
    Line(float m, float q);
    Line(const Point& p1, const Point& p2);
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

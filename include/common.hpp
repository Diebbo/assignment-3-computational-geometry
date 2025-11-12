#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>
#include <ostream>

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


/* linked list */

/* Convex Hull Interface */
template <typename T>
class ConvexHull {
public:
    /* Every algorithm must implement the lower and upper hull and merge them */
    virtual T compute(const T& points) const = 0;
};

using Points = std::vector<Point>;

#endif // COMMON_HPP

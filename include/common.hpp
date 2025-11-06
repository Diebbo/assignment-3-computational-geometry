#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>
#include <iostream>

/* Point Class */
class Point {
public:
    float x;
    float y;
    Point(float x = 0, float y = 0) : x(x), y(y) {}
};

class Line {
protected:
    float m; // slope
    float q; // y-intercept
public:
    Point p1;
    Point p2;
    Line(const Point& p1, const Point& p2) : p1(p1), p2(p2) {}
    Line() : p1(Point()), p2(Point()) {}
};

class Triangle {
public:
    Point p1;
    Point p2;
    Point p3;
    Triangle(const Point& p1, const Point& p2, const Point& p3) : p1(p1), p2(p2), p3(p3) {}
};


/* linked list */

/* Convex Hull Interface */
template <typename T>
class ConvexHull {
public:
    /* Every algorithm must implement the lower and upper hull and merge them */
    virtual T compute(const T& points) = 0;
};

using Points = std::vector<Point>;

#endif // COMMON_HPP

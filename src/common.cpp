#include <common.hpp>

Point::Point(float x, float y) : x(x), y(y) {}
Point::Point() : x(0), y(0) {}
bool Point::operator==(Point const& other) {
    return this->x == other.x && this->y == other.y;
}

Line::Line() : p1(), p2() {}
Line::Line(Point const& a, Point const& b) : p1(a), p2(b) {}

Triangle::Triangle(Point const& a, Point const& b, Point const& c) : p1(a), p2(b), p3(c) {}

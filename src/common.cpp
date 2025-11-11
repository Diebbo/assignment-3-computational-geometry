#include <common.hpp>
#include <iostream>
#include <sstream>

Point::Point(float x, float y) : x(x), y(y) {}
Point::Point() : x(0), y(0) {}
bool Point::operator==(Point const& other) const {
    return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(Point const& other) const {
    return !(*this == other);
}

std::string Point::to_string() const {
  std::ostringstream o;
  o << "(" << this->x << ", " << this->y << ")";
  return o.str();
}

void showValue(const Point &pt, std::ostream &o) {
  o << "(" << pt.x << "," << pt.y << ")";
}

Line::Line() : p1(), p2() {}
Line::Line(Point const& a, Point const& b) : p1(a), p2(b) {}

Triangle::Triangle(Point const& a, Point const& b, Point const& c) : p1(a), p2(b), p3(c) {}

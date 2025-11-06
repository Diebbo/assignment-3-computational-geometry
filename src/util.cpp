#include "../include/util.hpp"

namespace util {
float sidedness(const Point &p1, const Point &p2, const Point &p3) {
  return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

float sidedness(const Line &l, const Point &p) {
  return sidedness(l.p1, l.p2, p);
}

bool is_inside(const Triangle &t, const Point &p) {
  /* The point must be on the same side of all the triangle's edges */
  float d1 = sidedness(t.p1, t.p2, p);
  float d2 = sidedness(t.p2, t.p3, p);
  float d3 = sidedness(t.p3, t.p1, p);

  // std::cout << "d1: " << d1 << ", d2: " << d2 << ", d3: " << d3 << std::endl;

  bool all_positive = (d1 > 0) && (d2 > 0) && (d3 > 0);
  bool all_negative = (d1 < 0) && (d2 < 0) && (d3 < 0);
  return all_positive || all_negative;
}

float partial_distance(const Line &l, const Point &p) {
  // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_an_equation
  // but we are not considering the denominator as it would be the same for all points
  return std::abs((l.p2.y - l.p1.y) * p.x - (l.p2.x - l.p1.x) * p.y + l.p2.x * l.p1.y - l.p2.y * l.p1.x);
}

} // namespace util

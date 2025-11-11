#include <util.hpp>
#include <common.hpp>
#include <graham_scan.hpp>
#include <algorithm>

bool point_cmp_x(const Point &a, const Point &b) {
  if (a.x != b.x) {
    return a.x < b.x;
  } else {
    return a.y > b.y;
  }
}

/**
 * Returns whether the three given points make the expected turn. The `side` parameter can be:
 * - `1`, meaning that the three points make a right turn, or
 * - `-1`, meaning that they do a left turn
 */
bool turns(float side, const Point& a, const Point& b, const Point& c) {
  auto sidedness = util::sidedness(Line(a, c), b);
  return sidedness * side <= 0;
}

void compute_inner(const Points& points, Points& half, float side) {
  half.clear();
  half.push_back(points[0]);
  half.push_back(points[1]);
  for (size_t i = 2; i < points.size(); i++) {
    while (half.size() >= 2 && turns(side, half[half.size()-2], half[half.size()-1], points[i])) {
      half.pop_back();
    }

    half.push_back(points[i]);
  }
}

Points GrahamScan::compute(const Points &points) {
  if (points.size() <= 2)
    return Points();

  Points pts(points);

  std::sort(pts.begin(), pts.end(), point_cmp_x);

  Points upper;
  compute_inner(pts, upper, 1.0);
  Points lower;
  compute_inner(pts, lower, -1.0);

  for (int i = lower.size()-2; i >= 1; i--) {
    upper.push_back(lower[i]);
  }
  return upper;
}

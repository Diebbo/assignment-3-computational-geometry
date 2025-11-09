#include <graham_scan.hpp>

bool point_cmp_x(const Point &a, const Point &b) { return a.x < b.x; }

Points GrahamScan::compute(const Points &points) {
  Points hull(points);
  std::sort(hull.begin(), hull.end(),
            [](const Point &a, const Point &b) { return a.x < b.x; });
  return hull;
}

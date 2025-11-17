#include "common.hpp"
#include <cassert>
#include <limits>
#include <util.hpp>

namespace util {
double sidedness(const Point &p1, const Point &p2, const Point &p3) {
  // cast to double to avoid precision issues
  const double dx_32 = p3.x - p2.x;
  const double dy_12 = p1.y - p2.y;
  const double dy_32 = p3.y - p2.y;
  const double dx_12 = p1.x - p2.x;
  return (dx_32 * dy_12) - (dy_32 * dx_12);
}

double sidedness(const Line &l, const Point &p) {
  return sidedness(l.p1, l.p2, p);
}

bool isLeft(const Point &p1, const Point &p2, const Point &p3) {
  const double dx_32 = p3.x - p2.x;
  const double dy_12 = p1.y - p2.y;
  const double dy_32 = p3.y - p2.y;
  const double dx_12 = p1.x - p2.x;

  return (dx_32 * dy_12) > (dy_32 * dx_12);

}


bool isLeft(const Line &l, const Point &p) { return isLeft(l.p1, l.p2, p); }

bool is_inside(const Triangle &t, const Point &p) {
  /* The point must be on the same side of all the triangle's edges */
  double d1 = sidedness(t.p1, t.p2, p);
  double d2 = sidedness(t.p2, t.p3, p);
  double d3 = sidedness(t.p3, t.p1, p);

  // std::cout << "d1: " << d1 << ", d2: " << d2 << ", d3: " << d3 << std::endl;

  bool all_positive = (d1 > 0) && (d2 > 0) && (d3 > 0);
  bool all_negative = (d1 < 0) && (d2 < 0) && (d3 < 0);
  return all_positive || all_negative;
}

double partial_distance(const Line &l, const Point &p) {
  // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_an_equation
  // but we are not considering the denominator as it would be the same for all
  // points
  return sidedness(l, p);
}

bool is_inside(const Points &polygon, const Point &p) {
  size_t n = polygon.size();
  if (n < 3) {
    return false; // A polygon must have at least 3 vertices
  }

  float initial_side = sidedness(polygon[0], polygon[1], p);
  for (size_t i = 1; i < n; ++i) {
    float current_side = sidedness(polygon[i], polygon[(i + 1) % n], p);
    if (initial_side * current_side < 0) {
      return false; // Point is on the opposite side of an edge
    }
  }
  return true; // Point is inside the polygon
}

bool is_partial_inside(const Points &polygon, const Point &p) {
  size_t n = polygon.size();
  if (n < 3) {
    return false; // A polygon must have at least 3 vertices
  }

  float initial_side = sidedness(polygon[0], polygon[1], p);
  for (size_t i = 1; i < n - 1; ++i) {
    float current_side = sidedness(polygon[i], polygon[(i + 1) % n], p);
    if (initial_side * current_side < 0) {
      return false; // Point is on the opposite side of an edge
    }
  }
  return true; // Point is inside the polygon
}

bool is_hull(const Points &hull, const Points &points) {
  for (const auto &p : points) {
    bool on_hull = false;

    // Check if point is on hull
    for (const auto &hp : hull) {
      if (p == hp) {
        on_hull = true;
        break;
      }
    }

    if (!on_hull) {
      // Check if point is inside hull
      // A point is inside if it's on the same side of ALL edges
      if (!is_inside(hull, p)) {
        return false;
      }
    }
  }
  return true;
}

bool is_partial_hull(const Points &hull, const Points &points) {
  for (const auto &p : points) {
    bool on_hull = false;

    // Check if point is on hull
    for (const auto &hp : hull) {
      if (p.x == hp.x && p.y == hp.y) {
        on_hull = true;
        break;
      }
    }

    if (!on_hull) {
      // Check if point is inside hull
      // A point is inside if it's on the same side of ALL edges
      if (!is_partial_inside(hull, p)) {
        return false;
      }
    }
  }
  return true;
}

template <typename T>
bool is_valid_inside(const T &polygon, const Point &p) {
  size_t n = polygon.size();
  if (n < 3) {
    return false; // A polygon must have at least 3 vertices
  }

  for (auto it = polygon.begin(); it != polygon.end(); ++it) {
    auto it2 = it;
    auto p1 = *it2;
    if (++it2 == polygon.end()) it2 = polygon.begin();
    auto p2 = *it2;
    if (isLeft(Line(p1, p2), p)) {
      // print sideness value
      std::cout << "Sideness value: " << sidedness(Line(p1, p2), p) << std::endl;
      return false; // Point is on the left side of an edge
    }
  }
  return true; // Point is inside the polygon
}

template <typename T>
bool is_valid_hull(const T &hull, const Points &points) {
  // check that the hull is convex
  size_t n = hull.size();
  if (n < 3) {
    std::cout << "Hull has less than 3 vertices." << std::endl;
    return false; // A hull must have at least 3 vertices
  }

  auto it = hull.begin();
  for (size_t i = 0; i < n; ++i) {
    auto it2 = it;
    auto i1 = *it2;
    if (++it2 == hull.end()) it2 = hull.begin();
    auto i2 = *it2;
    if (++it2 == hull.end()) it2 = hull.begin();
    auto i3 = *it2;

    if (util::isLeft(i1, i2, i3)) {
      print_points(hull);

      std::cout << "Hull is not convex: "
                << i << " " << sidedness(i1, i2, i3) << std::endl
                << "a=Point(" << i1.x << ", " << i2.y << "); "
                << "b=Point(" << i2.x << ", " << i2.y << "); "
                << "c=Point(" << i3.x << ", " << i3.y << ")" << std::endl;

      return false; // Hull is not convex
    }
  }

  // check that all points are either on the hull or inside it
  for (const auto &p : points) {
    bool on_hull = false;

    // Check if point is on hull
    for (const auto &hp : hull) {
      if (p == hp) {
        on_hull = true;
        break;
      }
    }

    if (!on_hull) {
      // Check if point is inside hull
      // A point is inside if it's on the same side of ALL edges
      if (!is_valid_inside<T>(hull, p)) {
        std::cout << "Point (" << p.x << ", " << p.y << ") is outside the hull."
                  << std::endl;
        
        return false;
      }
    }
  }

  return true;
}

template bool is_valid_hull<Points>(const Points &hull, const Points &points);
template bool is_valid_hull<PointsList>(const PointsList &hull, const Points &points);
template bool is_valid_hull<PointsDeque>(const PointsDeque &hull, const Points &points);

// Tuple of two points
using TPoint = std::pair<Point, Point>;

/* Compute the extremes for the points: leftmost and rightmost.
 * It cover the edge case where multiple points have the same x-coordinate but
 * different y-coordinates.
 *
 * returns a pair of points <(leftmost_ymin, leftmost_ymax), (rightmost_ymin,
 * rightmost_ymax)>
 */
std::pair<TPoint, TPoint> findExtremePointsCases(const Points &points) {
  Point leftPointYMin = points[0];
  Point leftPointYMax = points[0];
  Point rightPointYMin = points[0];
  Point rightPointYMax = points[0];

  for (const auto &p : points) {
    if (p.x < leftPointYMin.x) { // new leftmost point found
      leftPointYMin = p;
      leftPointYMax = p;
    } else if (p.x == leftPointYMin.x) { // same x-coordinate as current
                                         // leftmost, check y-coordinates
      if (p.y < leftPointYMin.y) {
        leftPointYMin = p;
      }
      if (p.y > leftPointYMax.y) {
        leftPointYMax = p;
      }
    }

    if (p.x > rightPointYMax.x) { // new rightmost point found
      rightPointYMin = p;
      rightPointYMax = p;
    } else if (p.x == rightPointYMax.x) { // same x-coordinate as current
                                          // rightmost, check y-coordinates
      if (p.y < rightPointYMin.y) {
        rightPointYMin = p;
      }
      if (p.y > rightPointYMax.y) {
        rightPointYMax = p;
      }
    }
  }
  return {{leftPointYMin, leftPointYMax}, {rightPointYMin, rightPointYMax}};
}

Line findExtremePoints(const Points &points, bool upper) {
  // Find leftmost and rightmost points with highest y in case of ties
  Point minPoint = points[0];
  Point maxPoint = points[0];
  if (upper) {
    for (const auto &p : points) {
      if (p.x < minPoint.x || (p.x == minPoint.x && p.y > minPoint.y)) {
        minPoint = p;
      }
      if (p.x > maxPoint.x || (p.x == maxPoint.x && p.y > maxPoint.y)) {
        maxPoint = p;
      }
    }
    return Line{minPoint, maxPoint};
  } else {
    for (const auto &p : points) {
      if (p.x < minPoint.x || (p.x == minPoint.x && p.y < minPoint.y)) {
        minPoint = p;
      }
      if (p.x > maxPoint.x || (p.x == maxPoint.x && p.y < maxPoint.y)) {
        maxPoint = p;
      }
    }
    return Line{maxPoint, minPoint};
  }
}

void print_results_comparison(const Points &grhamPoints,
                              const Points &quickHullPoints,
                              const Points &mbcPoints,
                              const std::string &label) {
  // save points into path ./output/label/{grham, quick, mcb}.txt
  std::string base_path = "./build/output/" + label + "/";
  std::string graham_path = base_path + "graham";
  std::string quickhull_path = base_path + "quick";
  std::string mbc_path = base_path + "mbc";

  // create output directory if it doesn't exist
  std::string command = "mkdir -p " + base_path;
  system(command.c_str());

  // write graham points
  std::ofstream graham_file(graham_path);
  for (const auto &p : grhamPoints) {
    graham_file << p.x << " " << p.y << std::endl;
  }
  graham_file.close();

  // write quickhull points
  std::ofstream quickhull_file(quickhull_path);
  for (const auto &p : quickHullPoints) {
    quickhull_file << p.x << " " << p.y << std::endl;
  }
  quickhull_file.close();

  // write mbc points
  std::ofstream mbc_file(mbc_path);
  for (const auto &p : mbcPoints) {
    mbc_file << p.x << " " << p.y << std::endl;
  }
  mbc_file.close();
}

} // namespace util

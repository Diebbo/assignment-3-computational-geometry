#include <limits>
#include <util.hpp>
#include <cassert>

namespace util {
double sidedness(const Point &p1, const Point &p2, const Point &p3) {
  // cast to double to avoid precision issues
  double x1 = static_cast<double>(p1.x);
  double y1 = static_cast<double>(p1.y);
  double x2 = static_cast<double>(p2.x);
  double y2 = static_cast<double>(p2.y);
  double x3 = static_cast<double>(p3.x);
  double y3 = static_cast<double>(p3.y);

  return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
  //return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

double sidedness(const Line &l, const Point &p) {
  return sidedness(l.p1, l.p2, p);
}

bool isLeft(const Point &p1, const Point &p2, const Point &p3) {
  // return (p3.x - p2.x) * (p1.y - p2.y) - (p3.y - p2.y) * (p1.x - p2.x) >
  // std::numeric_limits<float>::epsilon();

  double x1 = static_cast<double>(p1.x);
  double y1 = static_cast<double>(p1.y);
  double x2 = static_cast<double>(p2.x);
  double y2 = static_cast<double>(p2.y);
  double x3 = static_cast<double>(p3.x);
  double y3 = static_cast<double>(p3.y);

  std::cout.precision(17);
  bool float_result = (p3.x - p2.x) * (p1.y - p2.y) - (p3.y - p2.y) * (p1.x - p2.x) >
                      std::numeric_limits<float>::epsilon();
  bool double_result = x3 * y1 + x2 * y1 + y3 * x2 > x3 * y2 + y3 * x1 + y2 * x1;
  assert(float_result == double_result);
  return x3 * y1 + x2 * y1 + y3 * x2 > x3 * y2 + y3 * x1 + y2 * x1;
}

/*

  return (p3.x - p2.x) * (p1.y - p2.y) > (p3.y - p2.y) * (p1.x - p2.x);


  return p3.x*p1.y - p3.xP2.y - p2.x*p1.y + p2.x*p2.y > p3.y*p1.x - p3.y*p2.x -
  p2.y*p1.x + p2.y*p2.x;

  return p3.x*p1.y + p2.x*p1.y + p3.y*p2.x > p3.x*p2.y + p3.y*p1.x + p2.y*p1.x;
*/

bool isRight(const Point &p1, const Point &p2, const Point &p3) {
  return (p3.x - p2.x) * (p1.y - p2.y) - (p3.y - p2.y) * (p1.x - p2.x) <
         -std::numeric_limits<float>::epsilon();
}

bool isLeft(const Line &l, const Point &p) { return isLeft(l.p1, l.p2, p); }

bool isRight(const Line &l, const Point &p) { return isRight(l.p1, l.p2, p); }

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
  // but we are not considering the denominator as it would be the same for all
  // points
  return std::abs((l.p2.y - l.p1.y) * p.x - (l.p2.x - l.p1.x) * p.y +
                  l.p2.x * l.p1.y - l.p2.y * l.p1.x);
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

bool is_valid_inside(const Points &polygon, const Point &p) {
  size_t n = polygon.size();
  if (n < 3) {
    return false; // A polygon must have at least 3 vertices
  }

  for (size_t i = 0; i < n; ++i) {
    if (isLeft(Line(polygon[i], polygon[(i + 1) % n]), p)) {
      return false; // Point is on the left side of an edge
    }
  }
  return true; // Point is inside the polygon
}

bool is_valid_hull(const Points &hull, const Points &points) {
  // check that the hull is convex
  size_t n = hull.size();
  if (n < 3) {
    std::cout << "Hull has less than 3 vertices." << std::endl;
    return false; // A hull must have at least 3 vertices
  }

  for (size_t i = 0; i < n; ++i) {
    if (util::isLeft(hull[i], hull[(i + 1) % n], hull[(i + 2) % n])) {
      //print_points(hull);

      std::cout << "Hull is not convex: " << i << " "
                << sidedness(hull[i], hull[(i + 1) % n], hull[(i + 2) % n])
                << std::endl
                << "a=Point(" << hull[i].x << ", " << hull[i].y << "); b=Point("
                << hull[(i + 1) % n].x << ", " << hull[(i + 1) % n].y
                << "); c=Point(" << hull[(i + 2) % n].x << ", "
                << hull[(i + 2) % n].y << ")" << std::endl;

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
      if (!is_valid_inside(hull, p)) {
        std::cout << "Point (" << p.x << ", " << p.y << ") is outside the hull."
                  << std::endl;
        return false;
      }
    }
  }

  return true;
}

void print_points(const Points &points) {
  for (const auto &p : points) {
    std::cout << "(" << p.x << ", " << p.y << ")  ";
  }
  std::cout << std::endl;
}

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
  } else {
    for (const auto &p : points) {
      if (p.x < minPoint.x || (p.x == minPoint.x && p.y < minPoint.y)) {
        minPoint = p;
      }
      if (p.x > maxPoint.x || (p.x == maxPoint.x && p.y < maxPoint.y)) {
        maxPoint = p;
      }
    }
  }
  return Line{maxPoint, minPoint};
}
void print_results_comparison(const Points &grhamPoints, const Points &quickHullPoints, const Points &mbcPoints, const std::string &label) {
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

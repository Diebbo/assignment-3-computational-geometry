#include "common.hpp"
#include <cassert>
#include <graham_scan.hpp>
#include <iostream>
#include <marriage_before_conquest.hpp>
#include <ostream>
#include <quickhull.hpp>
#include <rapidcheck.h>
#include <util.hpp>

bool verify_points_in_hull(const Points &points, const Points &hull);

namespace rc {
template <> struct Arbitrary<Point> {
  static Gen<Point> arbitrary() {
    return gen::build<Point>(
      gen::construct<Point>(),
      gen::set(&Point::x, rc::gen::inRange(-100, 100)),
      gen::set(&Point::y, rc::gen::inRange(-100, 100))
    );
  }
};
} // namespace rc

int main() {
  /* Quick test of the sidedness function */
  Point p1(0, 0);
  Point p2(1, 1);
  Point p3(1, 0);
  float side = util::sidedness(p1, p2, p3);
  assert(side < 0); // p3 is below the line p1p2

  /* Quick test of the is_inside function */
  Point p4(0.6, 0.5);
  bool inside = util::is_inside(Triangle(p1, p2, p3), p4);
  assert(inside == true);

  /* Define a point container and read points from a file */
  Points pointContainer;
  util::read_points_from_file("points.txt", pointContainer);
  std::cout << "Read " << pointContainer.size() << " points from file."
            << std::endl;
  for (const auto &p : pointContainer) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  }

  /* We generate manually some points and compute their convex hull */
  Points points = {Point(0, 0),  Point(1, 1), Point(0.5, 1.5),
                   Point(1, -1), Point(2, 0), Point(1.5, 1.5)};

  ConvexHull<Points> *convexHullAlgorithm = new GrahamScan();
  Points hull = convexHullAlgorithm->compute(points);

  // TODO
  std::cout << "TODO" << std::endl;

  /* Quick Hull */
  ConvexHull<Points> *quickHullAlgorithm = new QuickHullNS::QuickHull();
  Points qhull = quickHullAlgorithm->compute(points);

  std::cout << "QuickHull Convex Hull Points:" << std::endl;
  for (const auto &p : qhull) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  }

  verify_points_in_hull(points, qhull);

  Points mbc_hull = MarriageNS::MarriageBeforeConquest().compute(points);
  if (util::is_partial_hull(mbc_hull, points)) {
    std::cout << "Test Pass: Marriage Before Conquest hull is valid"
              << std::endl;
  } else {
    std::cout << "Test Fail: Marriage Before Conquest hull is NOT valid"
              << std::endl;
  }

  rc::check("automatic test", []() {
    int num = *rc::gen::inRange(5, 50);
    std::vector<float> xs = *rc::gen::container<std::vector<float>>(num, rc::gen::nonZero<float>());
    std::vector<float> ys = *rc::gen::container<std::vector<float>>(num, rc::gen::nonZero<float>());
    std::vector<Point> pts;
    for (int i = 0; i < num; i++) {
      pts.push_back(Point(xs[i], ys[i]));
    }

    auto hull = GrahamScan().compute(pts);
    auto hull2 = QuickHullNS::QuickHull().compute(pts);
    // auto hull3 = MarriageNS::MarriageBeforeConquest().compute(pts);
    //RC_ASSERT(hull == hull3);
    // RC_ASSERT(hull2 == hull3);
    RC_ASSERT(util::is_hull(pts, hull2));
    RC_ASSERT(hull == hull2);
  });

  return 0;
}

bool verify_points_in_hull(const Points &points, const Points &hull) {
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
      int inside = util::sidedness(hull[0], hull[1], p);

      for (size_t i = 1; i < hull.size() - 1; i++) {
        Point p1 = hull[i];
        Point p2 = hull[(i + 1) % hull.size()];

        // For a counter-clockwise hull, all interior points should be
        // on the left side (positive sidedness) of all edges
        float side = util::sidedness(p1, p2, p);

        if (inside * side < 0) {
          return false;
        }
      }
    }
  }
  return true;
}

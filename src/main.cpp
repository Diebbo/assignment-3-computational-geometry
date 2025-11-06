#include "../include/graham_scan.hpp"
#include "../include/quickhull.hpp"
#include "../include/marriage_before_conquest.hpp"
#include "../include/util.hpp"
#include <cassert>
#include <iostream>
#include <ostream>

void verify_points_in_hull(const Points &points, const Points &hull);

// test purpose
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

  /* Create an instance of GrahamScan and compute the convex hull using
   * dependency injection for the compute method */
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

    /* Quick Hull */
  ConvexHull<Points> *marriageBeforeConquest = new MarriageNS::MarriageBeforeConquest();
  Points mbc_hull = marriageBeforeConquest->compute(points);

  std::cout << "QuickHull Convex Hull Points:" << std::endl;
  for (const auto &p : qhull) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  }

  if (util::is_partial_hull(mbc_hull, points)) {
    std::cout << "Test Pass: Marriage Before Conquest hull is valid" << std::endl;
  } else {
    std::cout << "Test Fail: Marriage Before Conquest hull is NOT valid" << std::endl;
  }

  return 0;
}

void verify_points_in_hull(const Points &points, const Points &hull) {
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
      bool miao = true;

      for (size_t i = 1; i < hull.size() - 1; i++) {
        Point p1 = hull[i];
        Point p2 = hull[(i + 1) % hull.size()];

        // For a counter-clockwise hull, all interior points should be
        // on the left side (positive sidedness) of all edges
        float side = util::sidedness(p1, p2, p);

        if (inside * side < 0) {
          miao = false;
          std::cout << "Point (" << p.x << ", " << p.y
                    << ") is outside the hull edge from (" << p1.x << ", "
                    << p1.y << ") to (" << p2.x << ", " << p2.y << ")"
                    << std::endl;
          break;
        }
      }

      assert(miao);
    }
  }
  std::cout << "Test Pass: points inside" << std::endl;
}

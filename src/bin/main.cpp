#include "common.hpp"
#include <cassert>
#include <graham_scan.hpp>
#include <iostream>
#include <marriage_before_conquest.hpp>
#include <ostream>
#include <quickhull.hpp>
#include <random>
#include <util.hpp>
#include <vector>

bool verify_points_in_hull(const Points &points, const Points &hull);

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
  // Points points = {Point(0, 0),  Point(1, 1), Point(0.5, 1.5),
  //                  Point(1, -1), Point(2, 0), Point(1.5, 1.5)};
  Points points = pointContainer;

  ConvexHull<Points> *convexHullAlgorithm = new GrahamScan();
  Points hull = convexHullAlgorithm->compute(points);

  /* Quick Hull */
  ConvexHull<Points> *quickHullAlgorithm = new QuickHullNS::QuickHull();
  Points qhull = quickHullAlgorithm->compute(points);

  std::cout << "QuickHull Convex Hull Points:" << std::endl;
  for (const auto &p : qhull) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  }

  bool is_correct = verify_points_in_hull(points, qhull);
  if (is_correct) {
    std::cout << "Test Pass: QuickHull hull is valid" << std::endl;
  } else {
    std::cout << "Test Fail: QuickHull hull is NOT valid" << std::endl;
  }

  Points mbc_hull = MarriageNS::MarriageBeforeConquest().compute(points);
  if (util::is_hull(mbc_hull, points)) {
    std::cout << "Test Pass: Marriage Before Conquest hull is valid"
              << std::endl;
  } else {
    std::cout << "Test Fail: Marriage Before Conquest hull is NOT valid"
              << std::endl;
  }
  std::cout << "MBC Convex Hull Points:" << std::endl;
  for (const auto &p : mbc_hull) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  }

  std::random_device rd;
  for (int i = 0; i < 100; i++) {
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(5, 50);
    int len = distr(gen);

    std::uniform_real_distribution<> distr2(-10, 10);
    std::vector<Point> pts(len);
    for (int i = 0; i < len; i++) {
      pts[i] = Point(distr2(gen), distr2(gen));
    }

    auto hull = GrahamScan().compute(pts);
    auto hull2 = QuickHullNS::QuickHull().compute(pts);
    auto hull3 = MarriageNS::MarriageBeforeConquest().compute(pts);
    assert(util::is_hull(pts, hull));
    assert(util::is_hull(pts, hull2));
    assert(util::is_hull(pts, hull3));
    // assert(hull == hull2);
    // assert(hull == hull3);
  }

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

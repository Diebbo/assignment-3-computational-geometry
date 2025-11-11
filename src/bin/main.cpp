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

Points testAglorithm(ConvexHull<Points> *algorithm, const Points &points,
                   const std::string &name) {
  std::cout << "------ Testing " << name << " algorithm..." << std::endl;
  Points hull = algorithm->compute(points);
  bool is_correct = util::is_hull(hull, points);
  if (is_correct) {
    std::cout << "Test Pass: Hull is valid" << std::endl;
  } else {
    std::cout << "Test Fail: Hull is NOT valid" << std::endl;
  }
  return hull;
}

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
  util::read_points_from_file("parabola_points.txt", pointContainer);
  std::cout << "Read " << pointContainer.size() << " points from file."
            << std::endl;
  for (const auto &p : pointContainer) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  }

  /* Test Graham Scan Algorithm */
  Points hull = testAglorithm(new GrahamScan(), pointContainer, "Graham Scan");

  /* Test QuickHull Algorithm */
  Points hull2 = testAglorithm(new QuickHullNS::QuickHull(), pointContainer, "QuickHull");

  /* Test Marriage Before Conquest Algorithm */
  Points hull3 = testAglorithm(new MarriageNS::MarriageBeforeConquest(), pointContainer, "Marriage Before Conquest");

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
    assert(util::is_valid_hull(hull, pts));
    //assert(util::is_valid_hull(hull2, pts));
    assert(util::is_valid_hull(hull3, pts));
    // assert(hull == hull2);
    assert(hull == hull3);
  }

  return 0;
}

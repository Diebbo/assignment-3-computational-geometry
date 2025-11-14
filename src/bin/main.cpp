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

Points testAlgorithm(ConvexHull<Points> *algorithm, const Points &points,
                   const std::string &name) {
  std::cout << "------ Testing " << name << " algorithm..." << std::endl;
  Points hull = algorithm->compute(points);
  bool is_correct = util::is_valid_hull(hull, points);
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
  util::print_points(pointContainer);
  /* Test Graham Scan Algorithm */
  Points hull = testAlgorithm(new GrahamScan(), pointContainer, "Graham Scan");
  util::print_points(hull);
  /* Test QuickHull Algorithm */
  Points hull2 = testAlgorithm(new QuickHullNS::QuickHull(), pointContainer, "QuickHull");

  /* Test Marriage Before Conquest Algorithm */
  Points hull3 = testAlgorithm(new MarriageNS::MarriageBeforeConquest(), pointContainer, "Marriage Before Conquest");

  // =================

  // TODO: da rimuovere, solo per testare i casi limite in dev time

  // (1, -1)  (2, 0)  (0, 0)  (0.5, 1.5)  (1, 1)  (1.5, 1.5)

  // Points failPoints = {Point(1, -1),    Point(2, 0), Point(0, 0),
  //                      Point(0.5, 1.5), Point(1, 1), Point(1.5, 1.5)};
  Points failPoints = {Point(1, -1),    Point(1, 0), Point(1, 2.5),
                       Point(1, 1.5), Point(1, 1), Point(1, -1.5)};
  Points grahamHull = GrahamScan().compute(failPoints);
  std::cout << "Graham Hull Points:" << std::endl;
  util::print_points(grahamHull);
  Points quickHull = QuickHullNS::QuickHull().compute(failPoints);
  std::cout << "QuickHull Points:" << std::endl;
  util::print_points(quickHull);
  Points mbc_hull2 = MarriageNS::MarriageBeforeConquest().compute(failPoints);
  std::cout << "MBC Hull Points:" << std::endl;
  util::print_points(mbc_hull2);

  //==================

  // read 1024 points from file and print the hull points
  Points bigPointContainer;
  const std::vector<std::string> shape = {"circle", "square", "parabola"};
  for (const auto &s : shape) {
    std::string filepath = "build/tests/" + s + "/1024";
    std::string filename = filepath;
    util::read_points_from_file(filename, bigPointContainer);
    std::cout << "Read " << bigPointContainer.size()
              << " points from file: " << filename << std::endl;
    Points grahamHull =
        testAlgorithm(new GrahamScan(), bigPointContainer,
                      "Graham Scan on " + s + " shape");
    Points quickHull =
        testAlgorithm(new QuickHullNS::QuickHull(), bigPointContainer,
                      "QuickHull on " + s + " shape");
    Points mbc_hull = testAlgorithm(
        new MarriageNS::MarriageBeforeConquest(), bigPointContainer,
        "Marriage Before Conquest on " + s + " shape");
    const std::string label = s;
    util::print_results_comparison(grahamHull, quickHull, mbc_hull, label);
    bigPointContainer.clear();
  }

  return 0;
}

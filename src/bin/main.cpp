#include "common.hpp"
#include <cassert>
#include <graham_scan.hpp>
#include <iostream>
#include <marriage_before_conquest.hpp>
#include <ostream>
#include <quickhull.hpp>
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

  /* Test Graham Scan Algorithm */
  Points hull = testAlgorithm(new GrahamScan(), pointContainer, "Graham Scan");

  /* Test QuickHull Algorithm */
  Points hull2 =
      testAlgorithm(new QuickHullNS::QuickHull(), pointContainer, "QuickHull");

  /* Test Marriage Before Conquest Algorithm */
  Points hull3 = testAlgorithm(new MarriageNS::MarriageBeforeConquest(),
                               pointContainer, "Marriage Before Conquest");

  /* Test Marriage Before Conquest V2 Algorithm */
  Points hull4 = testAlgorithm(new MarriageNS::MarriageBeforeConquestV2(),
                               pointContainer, "Marriage Before Conquest V2");

  // read 1024 points from file and print the hull points
  Points bigPointContainer;
  const std::vector<std::string> shape = {"circle", "square", "parabola"};
  for (const auto &s : shape) {
    std::string filepath = "build/tests/" + s + "/1024";
    std::string filename = filepath;
    util::read_points_from_file(filename, bigPointContainer);
    std::cout << "Read " << bigPointContainer.size()
              << " points from file: " << filename << std::endl;
    Points grahamHull = testAlgorithm(new GrahamScan(), bigPointContainer,
                                      "Graham Scan on " + s + " shape");
    Points quickHull =
        testAlgorithm(new QuickHullNS::QuickHull(), bigPointContainer,
                      "QuickHull on " + s + " shape");
    Points mbc_hull = testAlgorithm(
        new MarriageNS::MarriageBeforeConquest(), bigPointContainer,
        "Marriage Before Conquest on " + s + " shape");
    Points mbc_hull2 = testAlgorithm(
        new MarriageNS::MarriageBeforeConquestV2(), bigPointContainer,
        "Marriage Before Conquest V2 on " + s + " shape");
    const std::string label = s;
    util::print_results_comparison(grahamHull, quickHull, mbc_hull, label);
    bigPointContainer.clear();
  }

  return 0;
}

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

template <typename T>
T testAlgorithm(ConvexHull<T> *algorithm, const Points &points,
                const std::string &name) {
  std::cout << "------ Testing " << name << " algorithm..." << std::endl;
  T hull = algorithm->compute(points);
  bool is_correct = util::is_valid_hull<T>(hull, points);
  if (is_correct) {
    std::cout << "Test Pass: Hull is valid" << std::endl;
  } else {
    std::cout << "Test Fail: Hull is NOT valid" << std::endl;
  }
  return hull;
}

int main() {
  /* Quick test of the sidedness function */
  // Point p1(0, 0);
  // Point p2(1, 1);
  // Point p3(1, 0);
  // float side = util::sidedness(p1, p2, p3);
  // assert(side < 0); // p3 is below the line p1p2
  //
  // /* Quick test of the is_inside function */
  // Point p4(0.6, 0.5);
  // bool inside = util::is_inside(Triangle(p1, p2, p3), p4);
  // assert(inside == true);
  //
  // /* Define a point container and read points from a file */
  // Points pointContainer;
  // util::read_points_from_file("parabola_points.txt", pointContainer);
  // std::cout << "Read " << pointContainer.size() << " points from file."
  //           << std::endl;
  //
  // for (const auto &p : pointContainer) {
  //   std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  // }
  // util::print_points(pointContainer);
  //
  // /* Test Graham Scan Algorithm */
  // auto hull = testAlgorithm(new GrahamScan<Points>(), pointContainer,
  //                           "Graham Scan - std::vector");
  // util::print_points(hull);
  // auto hull4 = testAlgorithm(new GrahamScan<PointsList>(), pointContainer,
  //                            "Graham Scan - std::list");
  // util::print_points(hull4);
  // auto hull5 = testAlgorithm(new GrahamScan<PointsDeque>(), pointContainer,
  //                            "Graham Scan - std::deque");
  // util::print_points(hull5);
  //
  // /* Test QuickHull Algorithm */
  // Points hull2 =
  //     testAlgorithm(new QuickHullNS::QuickHull(), pointContainer, "QuickHull");
  // util::print_points(hull2);
  //
  // /* Test Marriage Before Conquest Algorithm */
  // Points hull3 = testAlgorithm(new MarriageNS::MarriageBeforeConquest(),
  //                              pointContainer, "Marriage Before Conquest");
  // std::cout << "MBC  correct Hull Points:" << std::endl;
  // util::print_points(hull3);
  //
  // // =================
  //
  // // TODO: da rimuovere, solo per testare i casi limite in dev time
  //
  // // (1, -1)  (2, 0)  (0, 0)  (0.5, 1.5)  (1, 1)  (1.5, 1.5)
  //
  // // Points failPoints = {Point(1, -1),    Point(2, 0), Point(0, 0),
  // //                      Point(0.5, 1.5), Point(1, 1), Point(1.5, 1.5)};
  // Points failPoints = {Point(1, -1),  Point(1, 0), Point(1, 2.5),
  //                      Point(1, 1.5), Point(1, 1), Point(1, -1.5)};
  // Points grahamHull = GrahamScan<Points>().compute(failPoints);
  // std::cout << "Graham Hull Points with std::vector:" << std::endl;
  // util::print_points(grahamHull);
  // auto grahamHullList = GrahamScan<PointsList>().compute(failPoints);
  // std::cout << "Graham Hull Points with std::list:" << std::endl;
  // util::print_points(grahamHullList);
  // auto grahamHullDeque = GrahamScan<PointsDeque>().compute(failPoints);
  // std::cout << "Graham Hull Points with std::deque:" << std::endl;
  // util::print_points(grahamHullDeque);
  // Points quickHull = QuickHullNS::QuickHull().compute(failPoints);
  // std::cout << "QuickHull Points:" << std::endl;
  // util::print_points(quickHull);
  // Points mbc_hull2 = MarriageNS::MarriageBeforeConquest().compute(failPoints);
  // std::cout << "MBC Hull Points:" << std::endl;
  // util::print_points(mbc_hull2);

  // read 1024 points from file and print the hull points
  Points bigPointContainer;
  const std::vector<std::string> shape = {"circle", "square", "parabola"};
  for (const auto &s : shape) {
    std::string filepath = "build/tests/" + s + "/256";
    std::string filename = filepath;
    util::read_points_from_file(filename, bigPointContainer);
    std::cout << "Read " << bigPointContainer.size()
              << " points from file: " << filename << std::endl;
    Points grahamHull =
        testAlgorithm(new GrahamScan<Points>(), bigPointContainer,
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
    util::print_results_comparison(grahamHull, quickHull, mbc_hull, mbc_hull2, label);
    bigPointContainer.clear();
  }

  //==================

  std::random_device rd;
  for (int i = 0; i < 1000; i++) {
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(5, 50);
    int len = distr(gen);

    std::uniform_real_distribution<> distr2(-10, 10);
    std::vector<Point> pts(len);
    for (int i = 0; i < len; i++) {
      pts[i] = Point(distr2(gen), distr2(gen));
    }

    auto hull = GrahamScan<Points>().compute(pts);
    auto hull2 = GrahamScan<PointsList>().compute(pts);
    auto hull3 = GrahamScan<PointsDeque>().compute(pts);
    auto hull4 = QuickHullNS::QuickHull().compute(pts);
    auto hull5 = MarriageNS::MarriageBeforeConquest().compute(pts);
    auto hull6 = MarriageNS::MarriageBeforeConquestV2().compute(pts);

    assert(util::is_valid_hull(hull, pts));
    assert(util::is_valid_hull(hull2, pts));
    assert(util::is_valid_hull(hull3, pts));
    assert(util::is_valid_hull(hull4, pts));
    assert(util::is_valid_hull(hull5, pts));

    assert(hull == std::vector(hull2.begin(), hull2.end()));
    assert(hull == std::vector(hull3.begin(), hull3.end()));
    assert(hull == hull4);
    assert(hull == hull5);
    //assert(hull == hull6);
  }

  return 0;
}

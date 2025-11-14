#ifndef UTIL_HPP
#define UTIL_HPP

#include <common.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace util {
/* Determine if point p is above line l
 *
 * A point is considered "above" a line if it lies on the side of the line
 * that is opposite to the direction of the line's normal vector.
 *
 * Parameters:
 *  - p: The point to check
 *  - l: The line to compare against
 *
 * Returns:
 *  - > 0 if point is above the line
 *  - < 0 if point is below the line
 *  - = 0 if point is on the line
 */
double sidedness(const Line &l, const Point &p);
double sidedness(const Point &p1, const Point &p2, const Point &p3);

bool isLeft(const Point &p1, const Point &p2, const Point &p3, bool tollerance = false);
bool isLeft(const Line &l, const Point &p, bool tollerance = false);

/* Determine if point p is inside triangle t
 *
 * A point is considered "inside" a triangle if it lies within the area
 * bounded by the triangle's three edges.
 *
 * Parameters:
 *  - t: The triangle to check against
 *  - p: The point to check
 *
 * Returns:
 *  true if point is inside the triangle
 *  false otherwise
 */
bool is_inside(const Triangle &t, const Point &p);

bool is_valid_inside(const Points &polygon, const Point &p);

bool is_valid_hull(const Points &hull, const Points &points);

float partial_distance(const Line &l, const Point &p);
void print_points(const Points &points);

/* Read points from a file
 *
 * The file should contain points in the following format:
 * x1 y1
 * x2 y2
 * ...
 *
 * Parameters:
 *  - filename: The name of the file to read from
 *  - container: The container to store the points in (e.g., Points)
 */
template <typename Container>
void read_points_from_file(const std::string &filename, Container &container) {
  static_assert(std::is_same_v<typename Container::value_type, Point>,
                "Container must hold Point objects");
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }
  int n;
  file >> n;

  float x, y;
  for (int i = 0; i < n; ++i) {
    file >> x >> y;
    container.emplace_back(x, y);
  }
}


// Tuple of two points
using TPoint = std::pair<Point, Point>;

/* Compute the extremes for the points: leftmost and rightmost.
 * It cover the edge case where multiple points have the same x-coordinate but different y-coordinates.
 *
 * returns a pair of points <(leftmost_ymin, leftmost_ymax), (rightmost_ymin, rightmost_ymax)>
 */
std::pair<TPoint, TPoint> findExtremePointsCases(const Points &points);

Line findExtremePoints(const Points &points, bool upper = true);


/* Print the results of the three algorithms into files
 *
 * The files will be saved in the following paths:
 * ./output/{label}/graham.txt
 * ./output/{label}/quickhull.txt
 * ./output/{label}/mbc.txt
 *
 * Parameters:
 *  - grhamPoints: The points from Graham Scan algorithm
 *  - quickHullPoints: The points from QuickHull algorithm
 *  - mbcPoints: The points from Marriage Before Conquest algorithm
 *  - label: The label to use for the output directory
 */
void print_results_comparison(const Points &grhamPoints, const Points &quickHullPoints, const Points &mbcPoints, const std::string &label);

} // namespace util

#endif // UTIL_HPP

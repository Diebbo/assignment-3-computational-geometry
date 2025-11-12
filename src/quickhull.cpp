#include <quickhull.hpp>
#include <util.hpp>

/* Convex Hull Factory */
using namespace QuickHullNS;

const int TOP_HULL = 1;
const int BOTTOM_HULL = -1;

// Tuple of two points
using TPoint = std::pair<Point, Point>;

/* Compute the extremes for the points: leftmost and rightmost.
 * It cover the edge case where multiple points have the same x-coordinate but different y-coordinates.
 *
 * returns a pair of points <(leftmost_ymin, leftmost_ymax), (rightmost_ymin, rightmost_ymax)>
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
    } else if (p.x == leftPointYMin.x) { // same x-coordinate as current leftmost, check y-coordinates
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
    } else if (p.x == rightPointYMax.x) { // same x-coordinate as current rightmost, check y-coordinates
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

std::pair<Point, Point> findExtremePoints(const Points &points) {
  Point minPoint = points[0];
  Point maxPoint = points[0];

  for (const auto &p : points) {
    if (p.x < minPoint.x) {
      minPoint = p;
    }
    if (p.x > maxPoint.x) {
      maxPoint = p;
    }
  }
  return {minPoint, maxPoint};
}

Points QuickHull::compute(const Points &points) const {
  /* ·
   * To initialize, find the point q1 with the smallest x-coordinate and the
   * point q2 with the largest x- coordinate, and form the line segment s by
   * connecting them. Then prune all the points below s. · QuickHull(q1 q2 , P )
   */
  Points upper_points = Points();
  Points lower_points = Points();
  Points hull = Points();

  Point q1upper, q2upper;
  Point q1lower, q2lower;

  auto [q1, q2] = findExtremePointsCases(points);
  // TPoint q1 = extremes.first;
  // TPoint q2 = extremes.second;
  // check if the leftmost points are the same, otherwise we compute differenyly and merge afterwards
  if (q1.first.y == q1.second.y) {
    q1upper = q1lower = q1.first;
  } else {
    q1upper = q1.second;
    q1lower = q1.first;
  }

  if (q2.first.x == q2.second.x) {
    q2upper = q2lower = q2.first;
  } else {
    q2upper = q2.second;
    q2lower = q2.first;
  }

  hull.push_back(q1upper);

  for (const auto &p : points) {
    auto s = util::sidedness(q1upper, q2upper, p);
    auto s2 = util::sidedness(q1lower, q2lower, p);
    if (s > 0) {
      upper_points.push_back(p);
    }
    if (s2 < 0) {
      lower_points.push_back(p);
    }
  }

  /* Recursively find the upper and lower hulls */
  QuickHull::findTopHullRecursive(q1upper, q2upper, upper_points, hull);

  // conclude the cycle of the upper hull with the last point of the upper hull if needed
  // another check to avoid duplicates
  if (hull.empty() || !(hull.back() == q2upper))
    hull.push_back(q2upper);

  // Only add q2lower if it's different from the last point in hull
  if (hull.empty() || !(hull.back() == q2lower))
    hull.push_back(q2lower);

  QuickHull::findBottomHullRecursive(q1lower, q2lower, lower_points, hull);

  // conclude the cycle of the bottom hull with the last point of the bottom hull if needed
  if (hull.empty() || !(hull.back() == q1lower))
    hull.push_back(q1lower);

  return hull;
}

void QuickHull::findTopHullRecursive(const Point &p1, const Point &p2,
                                   const Points &points, Points &hull) const {
  QuickHull::findHullRecursive(p1, p2, points, hull, TOP_HULL);
}

void QuickHull::findBottomHullRecursive(const Point &p1, const Point &p2,
                                   const Points &points, Points &hull) const {
  QuickHull::findHullRecursive(p1, p2, points, hull, BOTTOM_HULL);
}

void QuickHull::findHullRecursive(const Point &p1, const Point &p2,
                                   const Points &points, Points &hull,
                                   int side_multiplier) const {
  /* No more points left */
  if (points.empty()) {
    return;
  }
  if (points.size() == 1) {
    hull.push_back(points[0]);
    return;
  }

  /* 1. Find the point q on one side of s that has the largest distance to s. */
  float maxDistance = -1.0f;
  Point q;
  for (const auto &p : points) {
    float distance = util::partial_distance(Line(p1, p2), p);
    if (distance > maxDistance) {
      maxDistance = distance;
      q = p;
    }
  }

  /* 2. Add q to the convex hull */
  // NOTE: This is done after the recursive calls to maintain the correct order

  /* 3. Partition the remaining points into two subsets Pℓ and Pr */
  Points leftSet = Points();
  Points rightSet = Points();
  for (const auto &p : points) {
    if (util::sidedness(p1, q, p) * side_multiplier > 0) {
      leftSet.push_back(p);
    } else if (util::sidedness(q, p2, p) * side_multiplier > 0) {
      rightSet.push_back(p);
    }
  }

  /* 4. Recurse on the two subsets */
  // if bottom hull i recurr on the right side first
  if (side_multiplier == BOTTOM_HULL) {
    findHullRecursive(q, p2, rightSet, hull, side_multiplier);
    hull.push_back(q);
    findHullRecursive(p1, q, leftSet, hull, side_multiplier);
  } else { // UPPER HULL
    findHullRecursive(p1, q, leftSet, hull, side_multiplier);
    hull.push_back(q);
    findHullRecursive(q, p2, rightSet, hull, side_multiplier);
  }
}

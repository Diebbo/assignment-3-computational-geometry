#include <quickhull.hpp>
#include <util.hpp>

/* Convex Hull Factory */
using namespace QuickHullNS;

const int TOP_HULL = 1;
const int BOTTOM_HULL = -1;

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

Points QuickHull::compute(const Points &points) {
  /* ·
   * To initialize, find the point q1 with the smallest x-coordinate and the
   * point q2 with the largest x- coordinate, and form the line segment s by
   * connecting them. Then prune all the points below s. · QuickHull(q1 q2 , P )
   */
  Points upper_points = Points();
  Points lower_points = Points();
  Points hull = Points();

  Point q1, q2;
  std::tie(q1, q2) = findExtremePoints(points);
  hull.push_back(q1);

  for (const auto &p : points) {
    auto s = util::sidedness(q1, q2, p);
    if (s > 0) {
      upper_points.push_back(p);
    } else if (s < 0) {
      lower_points.push_back(p);
    }
  }

  /* Recursively find the upper and lower hulls */
  QuickHull::findTopHullRecursive(q1, q2, upper_points, hull);
  hull.push_back(q2);
  QuickHull::findBottomHullRecursive(q1, q2, lower_points, hull);

  return hull;
}

void QuickHull::findTopHullRecursive(const Point &p1, const Point &p2,
                                   const Points &points, Points &hull) {
  QuickHull::findHullRecursive(p1, p2, points, hull, TOP_HULL);
}

void QuickHull::findBottomHullRecursive(const Point &p1, const Point &p2,
                                   const Points &points, Points &hull) {
  QuickHull::findHullRecursive(p1, p2, points, hull, BOTTOM_HULL);
}

void QuickHull::findHullRecursive(const Point &p1, const Point &p2,
                                   const Points &points, Points &hull,
                                   int side_multiplier) {
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
  } else {
    findHullRecursive(p1, q, leftSet, hull, side_multiplier);
    hull.push_back(q);
    findHullRecursive(q, p2, rightSet, hull, side_multiplier);
  }
}

#include "../include/quickhull.hpp"

/* Convex Hull Factory */
using namespace QuickHullNS;

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
point q2 with the largest x- coordinate, and form the line segment s by
connecting them. Then prune all the points below s. · QuickHull(q1 q2 , P )
      */
  Points prunedPoints = Points();
  Points hull = Points();

  Point q1, q2;
  std::tie(q1, q2) = findExtremePoints(points);
  hull.push_back(q1);

  for (const auto &p : points) {
    if (util::sidedness(q1, q2, p) > 0) {
      prunedPoints.push_back(p);
    }
  }

  QuickHull::QuickHullRecursive(q1, q2, prunedPoints, hull);

  hull.push_back(q2);

  return hull;
}

void QuickHull::QuickHullRecursive(const Point &p1, const Point &p2,
                                   const Points &points, Points &hull) {
  /* No more points left */
  if (points.empty()) {
    return;
  }
  /* 1. Find the point q above s that has the largest distance to s. */
  float maxDistance = -1.0f;
  Point q;
  for (const auto &p : points) {
    float distance = util::partial_distance(Line(p1, p2), p);
    // it would be the same to use sidedness because algebraically they are the same:
    // it's basically the area of the triangle formed by p1, p2 and p multiplied by 2 and divided by the length of the base (p2 - p1)
    // aka half the determinant of the matrix formed by the vectors
    if (distance > maxDistance) {
      maxDistance = distance;
      q = p;
    }
  }

  /* 2. Add q to the convex hull */

  // hull.push_back(q);
  //  NOTE: This is done after the recursive calls to maintain the correct order

  /* 3. Prune all the points inside the triangle formed by q, q1 and q2.
   * Partition the remaining points into two subsets Pℓ and Pr consisting
   * of points that lie to the left of q and points that lie to the right of q
   */
  Points leftSet = Points();
  Points rightSet = Points();
  for (const auto &p : points) {
    if (util::sidedness(p1, q, p) > 0) {
      leftSet.push_back(p);
    } else if (util::sidedness(q, p2, p) > 0) {
      rightSet.push_back(p);
    }
  }

  /* 4. Connect q1 to q to form segment s1 and then connect q2 to q to form
   * segment s2 . */
  /* 5. Recurse on QuickHull(s1 , Pℓ ) and QuickHull(s2 , Pr ) */
  QuickHullRecursive(p1, q, leftSet, hull);

  hull.push_back(q);
  QuickHullRecursive(q, p2, rightSet, hull);
}

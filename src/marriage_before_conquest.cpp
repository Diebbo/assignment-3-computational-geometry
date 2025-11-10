#include <algorithm>
#include <marriage_before_conquest.hpp>
#include <random>
#include <util.hpp>

/* Convex Hull Factory */
using namespace MarriageNS;

Line MarriageBeforeConquest::findBridge(const Points &points, float midX) {
  /* Find the bridge (tangent) for the given set of points */
  Line bridge;
  if (points.front().x < midX) {
    bridge = {points.front(), points.back()};
  } else {
    bridge = {points.back(), points.front()};
  }

  for (size_t i = 0; i < points.size(); ++i) {
    const auto &p = points[i];
    if (util::isAbove(bridge, p)) {
      /* Point is above the bridge, update the bridge */
      /* p will be in the new bridge, check if is left or right of midX
      if p is left then set bridge.p1 = p
        loop on previous points to check if they're the best option for
      p2 else set bridge.p2 = p loop on previous points to check if
      they're the best option for p1
      */
      if (p.x < midX) {
        bridge.p1 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = points[j];
          if (q.x >= midX) {
            if (util::isAbove(bridge, q)) {
              bridge.p2 = q;
            }
          }
        }
      } else {
        bridge.p2 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = points[j];
          if (q.x <= midX) {
            if (util::isAbove(bridge, q)) {
              bridge.p1 = q;
            }
          }
        }
      }
    }
  }

  return bridge;
}

void MarriageBeforeConquest::MarriageBeforeConquestRecursive(
    const Points &points, Points &hull) {
  /* If points.size() < 3, add them to the hull */
  if (points.empty()) {
    return;
  } else if (points.size() == 1) {
    hull.push_back(points[0]);
    return;
  } else if (points.size() == 2) {
    // add first the leftmost point
    if (points[0].x < points[1].x) {
      hull.push_back(points[0]);
      hull.push_back(points[1]);
    } else if (points[0].x > points[1].x) {
      hull.push_back(points[1]);
      hull.push_back(points[0]);
    } else {
      // same x, add the lower one first
      if (points[0].y < points[1].y) {
        hull.push_back(points[1]);
      } else {
        hull.push_back(points[0]);
      }
    }
    return;
  }

  Point minIt = points.front();
  Point maxIt = points.back();

  float midX = (minIt.x + maxIt.x) / 2.0f;

  Line bridge = findBridge(points, midX);

  Points leftSet;
  Points rightSet;

  for (const auto &p : points) {
    if (p.x <= bridge.p1.x) {
      leftSet.push_back(p);
    } else if (p.x >= bridge.p2.x) {
      rightSet.push_back(p);
    }
  }

  MarriageBeforeConquestRecursive(leftSet, hull);
  MarriageBeforeConquestRecursive(rightSet, hull);
}

Points MarriageBeforeConquest::compute(const Points &points) {

  Points hull = Points();

  std::random_device rd;
  std::default_random_engine rng(rd());

  std::vector<Point> shuffledPoints = points;
  std::shuffle(shuffledPoints.begin(), shuffledPoints.end(), rng);
  
  MarriageBeforeConquestRecursive(shuffledPoints, hull);

  return hull;
}

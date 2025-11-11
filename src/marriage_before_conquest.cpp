#include <algorithm>
#include <iostream>
#include <marriage_before_conquest.hpp>
#include <random>
#include <type_traits>
#include <util.hpp>

/* Convex Hull Factory */
using namespace MarriageNS;

Line MarriageBeforeConquest::findUpperBridge(const Points &points) {
  /* Find the bridge (tangent) for the given set of points */
  Line bridge;
  if (points.front().x <= points.back().x) {
    bridge = {points.front(), points.back()};
  } else {
    bridge = {points.back(), points.front()};
  }

  float midX = (bridge.p1.x + bridge.p2.x) / 2.0f;

  for (size_t i = 0; i < points.size(); ++i) {
    const auto &p = points[i];
    if (util::isLeft(bridge, p)) {
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
            if (util::isLeft(bridge, q)) {
              bridge.p2 = q;
            }
          }
        }
      } else {
        bridge.p2 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = points[j];
          if (q.x <= midX) {
            if (util::isLeft(bridge, q)) {
              bridge.p1 = q;
            }
          }
        }
      }
    }
  }

  return bridge;
}

Line MarriageBeforeConquest::findLowerBridge(const Points &points) {
  /* Find the bridge (tangent) for the given set of points */
  Line bridge;
  if (points.front().x > points.back().x) {
    bridge = {points.front(), points.back()};
  } else {
    bridge = {points.back(), points.front()};
  }

  float midX = (bridge.p1.x + bridge.p2.x) / 2.0f;

  for (size_t i = 0; i < points.size(); ++i) {
    const auto &p = points[i];
    if (util::isLeft(bridge, p)) {

      if (p.x > midX) {
        bridge.p1 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = points[j];
          if (q.x <= midX) {
            if (util::isLeft(bridge, q)) {
              bridge.p2 = q;
            }
          }
        }
      } else {
        bridge.p2 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = points[j];
          if (q.x >= midX) {
            if (util::isLeft(bridge, q)) {
              bridge.p1 = q;
            }
          }
        }
      }
    }
  }

  return bridge;
}

void MarriageBeforeConquest::MBCUpperRecursive(const Points &points,
                                               Points &hull) {
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

  Line bridge = findUpperBridge(points);

  Points leftSet;
  Points rightSet;

  for (const auto &p : points) {
    if (p.x <= bridge.p1.x) {
      leftSet.push_back(p);
    } else if (p.x >= bridge.p2.x) {
      rightSet.push_back(p);
    }
  }

  MBCUpperRecursive(leftSet, hull);
  MBCUpperRecursive(rightSet, hull);
}

void MarriageBeforeConquest::MBCLowerRecursive(const Points &points,
                                               Points &hull) {
  /* If points.size() < 3, add them to the hull */
  if (points.empty()) {
    return;
  } else if (points.size() == 1) {
    hull.push_back(points[0]);
    return;
  } else if (points.size() == 2) {
    // add first the rightmost point
    if (points[0].x > points[1].x) {
      hull.push_back(points[0]);
      hull.push_back(points[1]);
    } else if (points[0].x < points[1].x) {
      hull.push_back(points[1]);
      hull.push_back(points[0]);
    } else {
      // same x, add only the lower one
      if (points[0].y > points[1].y) {
        hull.push_back(points[1]);
      } else {
        hull.push_back(points[0]);
      }
    }
    return;
  }

  Line bridge = findLowerBridge(points);

  Points leftSet;
  Points rightSet;

  for (const auto &p : points) {
    if (p.x <= bridge.p2.x) {
      leftSet.push_back(p);
    } else if (p.x >= bridge.p1.x) {
      rightSet.push_back(p);
    }
  }

  MBCLowerRecursive(rightSet, hull);
  MBCLowerRecursive(leftSet, hull);
}

Points MarriageBeforeConquest::compute(const Points &points) {

  Points hull = Points();

  std::random_device rd;
  std::default_random_engine rng(rd());

  std::vector<Point> shuffledPoints = points;
  std::shuffle(shuffledPoints.begin(), shuffledPoints.end(), rng);

  //print points
  // std::cout << "Shuffled Points:" << std::endl;
  // util::print_points(shuffledPoints);

  MBCUpperRecursive(shuffledPoints, hull);
  if (shuffledPoints.size() <= 2) {
    return hull;
  }

  hull.pop_back(); // remove last point to avoid duplication of rightmost point
  MBCLowerRecursive(shuffledPoints, hull);
  hull.pop_back(); // remove last point to avoid duplication of leftmost point
  return hull;
}

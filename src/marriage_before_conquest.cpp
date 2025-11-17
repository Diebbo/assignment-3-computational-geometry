#include <algorithm>
#include <cstddef>
#include <marriage_before_conquest.hpp>
#include <random>
#include <util.hpp>

using namespace MarriageNS;

Line MarriageBeforeConquest::findUpperBridge(const Points &points) const {
  /* Find the upper bridge for the given set of points */

  Point p1, p2;
  p1 = points[0];
  Line bridge = {p1, p1};
  Point maxY = points[0];

  for (size_t i = 1; i < points.size(); ++i) {
    p2 = points[i];
    if (p2.y > maxY.y) {
      maxY = p2;
    }
    if (p1.x != p2.x) {
      if (p1.x < p2.x) {
        bridge = {p1, p2};
      } else {
        bridge = {p2, p1};
      }
      break;
    }
  }

  if (bridge.p1.x == bridge.p2.x) {
    // all points have the same x
      // for upper bridge, take the highest point
    bridge = {maxY, maxY};
    return bridge;
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

Line MarriageBeforeConquest::findLowerBridge(const Points &points) const {
  /* Find the lower bridge for the given set of points */

  Point p1, p2;
  p1 = points[0];
  Line bridge = {p1, p1};

  Point minY = points[0];
  for (size_t i = 1; i < points.size(); ++i) {
    p2 = points[i];
    if (p2.y < minY.y) {
      minY = p2;
    }
    if (p1.x != p2.x) {
      if (p1.x > p2.x) {
        bridge = {p1, p2};
      } else {
        bridge = {p2, p1};
      }
      break;
    }
  }
  if (bridge.p1.x == bridge.p2.x) {
    // all points have the same x
    // for lower bridge, take the lowest point
    bridge = {minY, minY};
    return bridge;
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
                                               Points &hull) const {
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

  if (bridge.p1 == bridge.p2) {
    hull.push_back(bridge.p1);
    return;
  }

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
                                               Points &hull) const {
  /* If points.size() < 3, add them to the hull */
  if (points.empty()) {
    return;
  } else if (points.size() == 1) {
    if (hull.empty() || hull.back() != points[0]) {
      hull.push_back(points[0]);
    }
    return;
  } else if (points.size() == 2) {
    // add first the rightmost point
    if (points[0].x > points[1].x) {
      if (hull.empty() || hull.back() != points[0]) {
        hull.push_back(points[0]);
      }
      hull.push_back(points[1]);
    } else if (points[0].x < points[1].x) {
      if (hull.empty() || hull.back() != points[1]) {
        hull.push_back(points[1]);
      }
      hull.push_back(points[0]);
    } else {
      // same x, add only the lower one
      if (points[0].y > points[1].y) {
        if (hull.empty() || hull.back() != points[1]) {
          hull.push_back(points[1]);
        }
      } else {
        if (hull.empty() || hull.back() != points[0]) {
          hull.push_back(points[0]);
        }
      }
    }
    return;
  }

  Line bridge = findLowerBridge(points);

  if (bridge.p1 == bridge.p2) {
    hull.push_back(bridge.p1);
    return;
  }

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

Points MarriageBeforeConquest::compute(const Points &points) const {

  if (points.size() <= 2) {
    return points;
  }

  Points hull = Points();

  std::random_device rd;
  std::default_random_engine rng(rd());

  std::vector<Point> shuffledPoints = points;
  std::shuffle(shuffledPoints.begin(), shuffledPoints.end(), rng);

  MBCUpperRecursive(shuffledPoints, hull);

  MBCLowerRecursive(shuffledPoints, hull);
  if (hull.front() == hull.back()) {
    hull.pop_back(); // remove last point to avoid duplication of leftmost point
  }
  return hull;
}

// MarriageBeforeConquestV2 Implementation

Line MarriageBeforeConquestV2::findUpperBridge(const Points &points,
                                               const Line &extremes) const {
  /* Find the upper bridge for the given set of points */

  Point p1, p2;
  p1 = extremes.p1;
  p2 = extremes.p2;
  Line bridge = {p1, p2};
  if (p1.x == p2.x) {
    // all points have the same x
    return bridge;
  }

  float midX = (bridge.p1.x + bridge.p2.x) / 2.0f;

  Points L, R;

  for (size_t i = 0; i < points.size(); ++i) {
    const auto &p = points[i];
    if (util::isLeft(extremes, p)) {
      if (p.x < midX) {
        L.push_back(p);
      } else {
        R.push_back(p);
      }
    }
  }

  auto condition = [extremes](const Point &p) {
    return util::isLeft(extremes, p) || p == extremes.p1 || p == extremes.p2;
  };

  Points prunedPoints;

  // Use std::copy_if to copy values that satisfy the condition into
  // prunedPoints
  std::copy_if(points.begin(), points.end(), std::back_inserter(prunedPoints),
               condition);

  /*
  New way:
  itero su L
  se nuovo punto maggiore, allora aggiorno p1 e cerco nuovo p2


  */
  for (size_t i = 0; i < prunedPoints.size(); ++i) {
    const auto &p = prunedPoints[i];
    if (util::isLeft(bridge, p)) {
      /* Point is above the bridge and above the extremes, update the bridge */
      if (p.x < midX) {
        bridge.p1 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = prunedPoints[j];
          if (q.x >= midX) {
            if (util::isLeft(bridge, q)) {
              bridge.p2 = q;
            }
          }
        }
      } else {
        bridge.p2 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = prunedPoints[j];
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

Line MarriageBeforeConquestV2::findLowerBridge(const Points &points,
                                               const Line &extremes) const {
  /* Find the lower bridge for the given set of points */

  Point p1, p2;
  p1 = extremes.p1;
  p2 = extremes.p2;
  Line bridge = {p1, p2};
  if (p1.x == p2.x) {
    // all points have the same x
    return bridge;
  }

  float midX = (bridge.p1.x + bridge.p2.x) / 2.0f;

  auto condition = [extremes](const Point &p) {
    return util::isLeft(extremes, p) || p == extremes.p1 || p == extremes.p2;
  };

  Points prunedPoints;

  // Use std::copy_if to copy values that satisfy the condition into
  // prunedPoints
  std::copy_if(points.begin(), points.end(), std::back_inserter(prunedPoints),
               condition);

  for (size_t i = 0; i < prunedPoints.size(); ++i) {
    const auto &p = prunedPoints[i];
    if (util::isLeft(extremes, p) && util::isLeft(bridge, p)) {
      /* Point is under the bridge and under the extremes, update the bridge */
      if (p.x > midX) {
        bridge.p1 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = prunedPoints[j];
          if (q.x <= midX) {
            if (util::isLeft(bridge, q)) {
              bridge.p2 = q;
            }
          }
        }
      } else {
        bridge.p2 = p;
        for (size_t j = 0; j < i; ++j) {
          const auto &q = prunedPoints[j];
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

void MarriageBeforeConquestV2::MBCUpperRecursive(const Points &points,
                                                 Points &hull) const {
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

  Line extremes = util::findExtremePoints(points, true);

  Line bridge = findUpperBridge(points, extremes);

  if (bridge.p1 == bridge.p2) {
    hull.push_back(bridge.p1);
    return;
  }

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

void MarriageBeforeConquestV2::MBCLowerRecursive(const Points &points,
                                                 Points &hull) const {
  /* If points.size() < 3, add them to the hull */
  if (points.empty()) {
    return;
  } else if (points.size() == 1) {
    if (hull.empty() || hull.back() != points[0]) {
      hull.push_back(points[0]);
    }
    return;
  } else if (points.size() == 2) {
    // add first the rightmost point
    if (points[0].x > points[1].x) {
      if (hull.empty() || hull.back() != points[0]) {
        hull.push_back(points[0]);
      }
      hull.push_back(points[1]);
    } else if (points[0].x < points[1].x) {
      if (hull.empty() || hull.back() != points[1]) {
        hull.push_back(points[1]);
      }
      hull.push_back(points[0]);
    } else {
      // same x, add only the highest one
      if (points[0].y > points[1].y) {
        if (hull.empty() || hull.back() != points[1]) {
          hull.push_back(points[1]);
        }
      } else {
        if (hull.empty() || hull.back() != points[0]) {
          hull.push_back(points[0]);
        }
      }
    }
    return;
  }

  Line extremes = util::findExtremePoints(points, false);

  Line bridge = findLowerBridge(points, extremes);

  if (bridge.p1 == bridge.p2) {
    hull.push_back(bridge.p1);
    return;
  }

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

Points MarriageBeforeConquestV2::compute(const Points &points) const {

  if (points.size() <= 2) {
    return points;
  }

  Points hull = Points();

  std::random_device rd;
  std::default_random_engine rng(rd());

  std::vector<Point> shuffledPoints = points;
  std::shuffle(shuffledPoints.begin(), shuffledPoints.end(), rng);

  MBCUpperRecursive(shuffledPoints, hull);

  MBCLowerRecursive(shuffledPoints, hull);
  if (hull.front() == hull.back()) {
    hull.pop_back(); // remove last point to avoid duplication of leftmost point
  }
  return hull;
}

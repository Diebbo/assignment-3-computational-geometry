= Marriage Before Conquest Algorithm <app:mbc>
```cpp
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
```

= Marriage Before Conquest Recursive Function <app:mbc_recursive>

```cpp
void MarriageBeforeConquest::MBCUpperRecursive(const Points &points, Points &hull) const {
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
```

= Marriage Before Conquest Find Upper Bridge Function <app:mbc_find_upper_bridge>
```cpp
Line MarriageBeforeConquest::findUpperBridge(const Points &points) const {

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

    bridge = {maxY, maxY};
    return bridge;
  }

  float midX = (bridge.p1.x + bridge.p2.x) / 2.0f;

  for (size_t i = 0; i < points.size(); ++i) {
    const auto &p = points[i];
    if (util::isLeft(bridge, p)) {

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
```
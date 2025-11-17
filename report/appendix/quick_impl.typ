= QuickHull Algorithm <app:quickhull>
```cpp
Points QuickHull::compute(const Points &points) const {
  Points upper_points = Points();
  Points lower_points = Points();
  Points hull = Points();

  Point q1upper, q2upper;
  Point q1lower, q2lower;

  auto [q1, q2] = util::findExtremePointsCases(points);
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
    if (util::isLeft(q1upper, q2upper, p)) {
      upper_points.push_back(p);
    }
    if (util::isLeft(q2lower, q1lower, p)) {
      lower_points.push_back(p);
    }
  }

  QuickHull::findHullRecursive(q1upper, q2upper, upper_points, hull);
  if (hull.empty() || !(hull.back() == q2upper))
    hull.push_back(q2upper);

  if (hull.empty() || !(hull.back() == q2lower))
    hull.push_back(q2lower);

  QuickHull::findHullRecursive(q2lower, q1lower, lower_points, hull);

  if (hull.empty() || !(hull.back() == q1lower || hull.front() == q1lower))
    hull.push_back(q1lower);

  return hull;
}
```

= QuickHull Recursive Function <app:quickhull_recursive>

```cpp
void QuickHull::findHullRecursive(const Point &p1, const Point &p2,
                                  const Points &points, Points &hull) const {
  /* No more points left */
  if (points.empty()) {
    return;
  }
  if (points.size() == 1) {
    hull.push_back(points[0]);
    return;
  }

  /* 1. Find the point q on one side of s that has the largest distance to s. */
  double maxDistance = -1.0;
  Point q;
  for (const auto &p : points) {
    double distance = util::partial_distance(Line(p1, p2), p);
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
    if (util::isLeft(p1, q, p)) {
      leftSet.push_back(p);
    } else if (util::isLeft(q, p2, p)) {
      rightSet.push_back(p);
    }
  }

  /* 4. Recurse on the two subsets */
  // if bottom hull i recurr on the right side first

  findHullRecursive(p1, q, leftSet, hull);
  hull.push_back(q);
  findHullRecursive(q, p2, rightSet, hull);
}
```

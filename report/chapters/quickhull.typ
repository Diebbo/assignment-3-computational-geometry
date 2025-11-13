#import "../bench.typ" as bench


== QuickHull Algorithm

=== Implementation

In the following section, we will present the implementation of the QuickHull algorithm and the benchmarks we performed to evaluate its performance.

#figure(
  caption: [QuickHull working principle],
  kind: auto,
grid(
    columns: 2,
  image("../assets/Quickhull_example3.svg.png", width: 50%),
  image("../assets/Quickhull_example6.svg.png", width: 50%)
  )
)

Firstly, we will introduce some of the utility function created to support the core algorithm(see @lst:extremes).

#figure(
  caption: [Function to find extreme points with vertical alignment cases],
  kind: auto,
```cpp
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
```
)<lst:extremes>

The function shown above finds the extreme points in a set of 2D points, specifically the leftmost and rightmost points, while handling cases where multiple points share the same x-coordinate but have different y-coordinates (i.e., vertical alignments).

#figure(
  caption: [Compute function for QuickHull algorithm],
  kind: auto,
```cpp
Points QuickHull::compute(const Points &points) const {
  Points upper_points = Points();
  Points lower_points = Points();
  Points hull = Points();

  Point q1upper, q2upper;
  Point q1lower, q2lower;

  auto [q1, q2] = findExtremePointsCases(points);
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
  QuickHull::findTopHullRecursive(q1upper, q2upper, upper_points, hull);

  if (hull.empty() || !(hull.back() == q2upper))
    hull.push_back(q2upper);
  if (hull.empty() || !(hull.back() == q2lower))
    hull.push_back(q2lower);

  QuickHull::findBottomHullRecursive(q1lower, q2lower, lower_points, hull);
  if (hull.empty() || !(hull.back() == q1lower))
    hull.push_back(q1lower);
  return hull;
}
```
)<lst:compute>

As we have mentioned before, the compute function is the core of all the ConvexHull classes. In this particular case, it initializes the QuickHull algorithm by finding the extreme points and partitioning the input points into upper and lower subsets. It then calls the recursive functions to compute the upper and lower hulls, finally combining them into a single convex hull.

A lot of the extra steps are made to cover edge cases where multiple points share the same x-coordinate, ensuring that the algorithm correctly identifies and processes these points without introducing duplicates in the final hull.

The recursive functions used to compute the upper and lower hulls are shown below @lst:recursive.

#figure(
  caption: [Recursive functions for QuickHull algorithm],
  kind: auto,
  ```cpp
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
  if (points.empty()) { return; }
  if (points.size() == 1) { hull.push_back(points[0]); return; }

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
  ```
) <lst:recursive>

=== Complexity Analysis

Let's analyze the time complexity of the QuickHull algorithm:
- The algorithm starts by finding the extreme points, which takes $O(n)$ time.
- The partitioning of points into upper and lower subsets also takes $O(n)$ time.
- The recursive step involves finding the point with the maximum distance from the line segment, which takes O(n) time in the worst case.
- Then we recur two times on subsets of points $O(2 T(n/2))$.

Let's now set up the recurrence relation for the time complexity:

$ T(n) = T(k) + T(n - k) + O(n) $

Where $k$ is the number of points in one of the subsets. In the worst case, $k$ can be as large as $n-1$, leading to unbalanced partitions. This results in a time complexity of $O(n^2)$ in the worst case.

=== Benchmarks

In our benchmarks, we expect the QuickHull algorithm to perform efficiently on average, with a time complexity of $O(n log n)$ for random distributions of points. However, for some testcases such as a parabola distribution, we might observe performance degradation towards the worst-case scenario of $O(n^2)$.

#bench.diagram_points("circle", "1024"),
#bench.diagram_points("square", "1024"),
#bench.diagram_points("parabola", "1024"),

Above are some example distributions of points used in our benchmarks.

The results of our benchmarks shown in the diagram below respects our expectations, with QuickHull performing well on circular and square distributions, while showing increased computation time on the parabola distribution as the number of points increases.

// full page
#bench.lq.diagram(
  bench.plot_bench("quick", "circle"),
  bench.plot_bench("quick", "square"),
  bench.plot_bench("quick", "parabola"),
  xaxis: (label: "Number of elements", scale: bench.log2),
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)

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
)<lst:compute>

As we have mentioned before, the compute function is the core of all the ConvexHull classes. In this particular case, it initializes the QuickHull algorithm by finding the extreme points and partitioning the input points into upper and lower subsets. It then calls the recursive functions to compute the upper and lower hulls, finally combining them into a single convex hull.

A lot of the extra steps are made to cover edge cases where multiple points share the same x-coordinate, ensuring that the algorithm correctly identifies and processes these points without introducing duplicates in the final hull.

The recursive functions used to compute the upper and lower hulls are shown below @lst:recursive.

#figure(
  caption: [Recursive functions for QuickHull algorithm],
  kind: auto,
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

Following up the results for the QuickHull algorithm in linear scale:
// full page
#bench.lq.diagram(
  bench.plot_bench("quick", "circle"),
  bench.plot_bench("quick", "square"),
  bench.plot_bench("quick", "parabola"),
  xaxis: (label: "Number of elements"),
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)

And in log-log scale. //(see @fig:quickhull-bench-loglog).

#bench.lq.diagram(
  bench.plot_bench("quick", "circle"),
  bench.plot_bench("quick", "square"),
  bench.plot_bench("quick", "parabola"),
  xaxis: (label: "Number of elements"),
  xscale: "log",
  yscale: "log",
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)<fig:quickhull-bench-loglog>

=== Optimizations

In order to have a more fair comparison we decided to test the algorithm on also a compiler-optimized version with the -O3 flag enabled. The results are shown below.

#bench.lq.diagram(
  bench.plot_bench("quick", "circle_optimized"),
  bench.plot_bench("quick", "square_optimized"),
  bench.plot_bench("quick", "parabola_optimized"),
  xaxis: (label: "Number of elements"),
  xscale: "log",
  yscale: "log",
  legend: (position: top + left),
  width: 100%,
  height: 6cm,
)<fig:quickhull-bench-loglog>

As we could expect, the optimization provided by the compiler improved the performance by a almost 10x factor across all the different distributions. But the overall behavior of the algorithm remained the same.

=== Visual Results

Following up are some visualizations of the convex hulls computed by the QuickHull algorithm on different point distributions.

#bench.plot_hull("circle", 256, "quick")
#bench.plot_hull("parabola", 256, "quick")
#bench.plot_hull("square", 256, "quick")




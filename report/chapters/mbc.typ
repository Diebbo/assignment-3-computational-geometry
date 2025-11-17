#import "../bench.typ" as bench


== Marriage Before Conquest Algorithm

=== Implementation

In the following section, we will present the implementation of the Marriage Before Conquest algorithm and the benchmarks we performed to evaluate its performance.

#figure(
  caption: [Marriage Before Conquest working principle],
  kind: auto,
grid(
    columns: 2,
  image("../assets/mbc-idea.png", width: 100%),
  )
)

Firstly, we will introduce some of the utility function created to support the core algorithm(see).

#figure(
  caption: [Function to find the relative position of a point with respect to a line segment],
  kind: auto,
```cpp
bool isLeft(const Point &p1, const Point &p2, const Point &p3) {
  const double dx_32 = p3.x - p2.x;
  const double dy_12 = p1.y - p2.y;
  const double dy_32 = p3.y - p2.y;
  const double dx_12 = p1.x - p2.x;

  return (dx_32 * dy_12) > (dy_32 * dx_12);
}
```
)

The function above checks if a point p3 is to the left of the line segment formed by points p1 and p2. This is useful for determining the relative position of points during the construction of the convex hull.

#figure(
  caption: [Function to find extreme horizontal points with upper or lower hull cases],
  kind: auto,
```cpp
Line findExtremePoints(const Points &points, bool upper) {
  // Find leftmost and rightmost points with highest y in case of ties
  Point minPoint = points[0];
  Point maxPoint = points[0];
  if (upper) {
    for (const auto &p : points) {
      if (p.x < minPoint.x || (p.x == minPoint.x && p.y > minPoint.y)) {
        minPoint = p;
      }
      if (p.x > maxPoint.x || (p.x == maxPoint.x && p.y > maxPoint.y)) {
        maxPoint = p;
      }
    }
    return Line{minPoint, maxPoint};
  } else {
    for (const auto &p : points) {
      if (p.x < minPoint.x || (p.x == minPoint.x && p.y < minPoint.y)) {
        minPoint = p;
      }
      if (p.x > maxPoint.x || (p.x == maxPoint.x && p.y < maxPoint.y)) {
        maxPoint = p;
      }
    }
    return Line{maxPoint, minPoint};
  }
}
```
)

The function shown above finds the extreme points in a set of 2D points, specifically the leftmost and rightmost points, while handling cases where multiple points share the same x-coordinate but have different y-coordinates, based on whether we are considering the upper or lower hull.

#figure(
  caption: [Compute function for Marriage Before Conquest algorithm],
  kind: auto,
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
)

The compute function above implements the core logic of the Marriage Before Conquest algorithm. It first checks if the number of points is less than or equal to two, in which case it simply returns the points as they already form a convex hull. For larger sets of points, it initializes an empty hull and shuffles the input points randomly to ensure a good distribution for the algorithm's performance. It then calls the recursive functions for constructing the upper and lower hulls, finally ensuring that the leftmost point is not duplicated in the final hull.

#figure(
  caption: [Recursive functions for QuickHull algorithm],
  kind: auto,
  ```cpp
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
  ```
)

The MBCUpperRecursive function above is a recursive implementation of the upper hull construction in the Marriage Before Conquest algorithm. It handles base cases for small sets of points and recursively finds the upper bridge, partitioning the points into left and right subsets for further processing, after finding a valid bridge.

#figure(
  caption: [Function to find the upper bridge in Marriage Before Conquest algorithm],
  kind: auto,
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
)

The _findUpperBridge_ function above identifies the upper bridge for a given set of points in the Marriage Before Conquest algorithm. It initializes the bridge with two points and iteratively refines it by checking the relative positions of other points, ensuring that the bridge correctly represents the upper boundary of the convex hull.

The algorithm for finding the upper bridge takes $O(n)$ expected time, but in the worst case, it can take $O(n^2)$ time due to the nested loops when updating the bridge points. That's why the algorithm relies on randomization (in the _compute_ function) to achieve better average performance.

It is worth noting that the implementation of the lower hull construction and the corresponding bridge finding function follows a similar logic, adjusted for the lower hull case.


=== Complexity Analysis

Let's analyze the time complexity of the Marriage Before Conquest algorithm.

The algorithm starts by shuffling the points, which takes $O(n)$ time.


Finding a bridge takes $O(n)$ time. Points that cannot contribute to the convex hull on either side of the median line are discarded. The algorithm then proceeds recursively on the remaining points to compute the upper and lower parts of the convex hull.

For a balanced partitioning on the median

At each recursion level $i$, the algorithm solves at most  $2^i$ subproblems, each containing at most $n/2^i$ points. Since each subproblem identifies a single edge of the convex hull, the total number of subproblems is bounded by $h$, the number of points on the hull. In the worst case, when no points can be discarded early, the recursion depth is $O(log h)$, and each level processes $O(n)$ points in linear time. This results in an overall time complexity of $O(n log h)$.



=== Benchmarks

In our benchmarks, we expect the Marriage Before Conquest algorithm to perform efficiently on average, with a time complexity of $O(n log n)$ for random distributions of points.

#bench.diagram_points("circle", "1024"),
#bench.diagram_points("square", "1024"),
#bench.diagram_points("parabola", "1024"),

Above are some example distributions of points used in our benchmarks.

The results of our benchmarks shown in the diagram below respects our expectations, with QuickHull performing well on circular and square distributions, while showing increased computation time on the parabola distribution as the number of points increases.

Following up the results for the QuickHull algorithm in linear scale:
// full page
// #bench.lq.diagram(
//   bench.plot_bench("marriage", "circle"),
//   bench.plot_bench("marriage", "square"),
//   bench.plot_bench("marriage", "parabola"),
//   xaxis: (label: "Number of elements"),
//   legend: (position: top + left),
//   width: 100%,
//   height: 6cm,
// )

And in log-log scale. //(see @fig:marriagehull-bench-loglog).

// #bench.lq.diagram(
//   bench.plot_bench("marriage", "circle"),
//   bench.plot_bench("marriage", "square"),
//   bench.plot_bench("marriage", "parabola"),
//   xaxis: (label: "Number of elements"),
//   xscale: "log",
//   yscale: "log",
//   legend: (position: top + left),
//   width: 100%,
//   height: 6cm,
// )<fig:mbc-bench-loglog>


=== Results

// #bench.plot_hull("circle", 256, "marriage")
// #bench.plot_hull("parabola", 256, "marriage")
// #bench.plot_hull("square", 256, "marriage")
